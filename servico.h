#ifndef SERVICO_H
#define SERVICO_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include "estruturas.h"
#include "interface.h"

char usuario[50];
char senha[50];
SQLINTEGER usrid;

SQLHENV env;
SQLHDBC dbc;
SQLHSTMT stmt;
SQLRETURN ret;

int leitorConfig(const char* arquivo, OdbcConfig* config)
{
    FILE* file = fopen(arquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo .ini.\n");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[256], value[256];
        if (sscanf(line, "%[^=]=%[^\n]", key, value) == 2) {
            if (strcmp(key, "Driver") == 0) {
                strncpy(config->driver, value, sizeof(config->driver));
            }else if (strcmp(key, "Server") == 0) {
                strncpy(config->server, value, sizeof(config->server));
            } else if (strcmp(key, "Database") == 0) {
                strncpy(config->database, value, sizeof(config->database));
            } else if (strcmp(key, "User") == 0) {
                strncpy(config->user, value, sizeof(config->user));
            } else if (strcmp(key, "Password") == 0) {
                strncpy(config->password, value, sizeof(config->password));
            }
        }
    }

    return 1;
    fclose(file);
}

void iniciarServico()
{
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    struct odbcConfig conn; 
    char conn_str[256];
    if (leitorConfig("config.ini", &conn)) {
        snprintf(conn_str, sizeof(conn_str),
                "DRIVER={%s};SERVER=%s;DATABASE=%s;UID=%s;PWD=%s",
                conn.driver, conn.server, conn.database,
                conn.user, conn.password);
    } else {
        printf("Falha na leitura do arquivo de configuracao.\n");
    }

    ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)conn_str, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (SQL_SUCCEEDED(ret)) {
        printf("Conectado ao Servidor.\n");
    }

    else {
        printf("Erro na conexao ao Azure SQL.\n");
    }
}

void inserir(char* consulta)
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret))
    {
        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if(SQL_SUCCEEDED(ret))
        {
            printf("Envio Realizado!");
        }
        else
        {
            SQLCHAR sqlState[6];
            SQLINTEGER nativeError;
            SQLCHAR messageText[256];
            SQLSMALLINT textLength;

            SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, sqlState, &nativeError, messageText, sizeof(messageText), &textLength);
            printf("Erro ao enviar dados: %s\n", messageText);
        }
    }

    else
    {
        SQLCHAR sqlState[6];
        SQLINTEGER nativeError;
        SQLCHAR messageText[256];
        SQLSMALLINT textLength;

        SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, sqlState, &nativeError, messageText, sizeof(messageText), &textLength);
        printf("Erro ao enviar dados: %s\n", messageText);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

}

void leitura(SQLHDBC dbc, const char* consulta) {

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret)) {

        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if (SQL_SUCCEEDED(ret)) {
            SQLSMALLINT num_colunas;
            SQLCHAR buffer[1024];
            SQLLEN len;
            SQLNumResultCols(stmt, &num_colunas);
            char nome_coluna[100];


        printf("\n");
        for (int i = 0; i < num_colunas; i++) {
            SQLDescribeCol(stmt, i + 1, nome_coluna, sizeof(nome_coluna), NULL, NULL, NULL, NULL, NULL);
            printCell(nome_coluna, PAD_SQL); // Adjust the width (e.g., 20 characters)
        }
        printf("\n");

        // Fetch and print rows
        while (SQLFetch(stmt) == SQL_SUCCESS) {
            for (int i = 0; i < num_colunas; i++) {
                while (SQLGetData(stmt, i + 1, SQL_C_CHAR, buffer, sizeof(buffer), &len) == SQL_SUCCESS) {
                    if (len > 0) {
                        printCell(buffer, PAD_SQL); // Adjust the width (e.g., 20 characters)
                    }
                }
            }
            printf("\n");
        }


        } else {
            SQLCHAR sqlState[6];
            SQLINTEGER nativeError;
            SQLCHAR messageText[256];
            SQLSMALLINT textLength;

            SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, sqlState, &nativeError, messageText, sizeof(messageText), &textLength);
            printf("Erro processando consulta: %s\n", messageText);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    } else {
        printf("Erro na alocacao de memoria driver SQL.\n");
    }
}

void liberarServico()
{
    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}

void validacaoLogin(char* usuario, char* senha) {
    SQLRETURN ret;
    SQLLEN len;

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret)) {
        char consulta[255];
        snprintf(consulta, sizeof(consulta),
            "SELECT TOP 1 ID_REGUSR FROM TB_DIM_USUARIO WHERE NM_USUAID = '%s' AND VL_PASSWD = '%s'",
            usuario, senha
        );

        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if (SQL_SUCCEEDED(ret)) {
        ret = SQLBindCol(stmt, 1, SQL_C_LONG, &usrid, sizeof(usrid), NULL);
        ret = SQLFetch(stmt);
        if (SQL_SUCCEEDED(ret)!=1) {
            printf("Erro resolvendo resultado: %d\n", ret);
            SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, NULL, NULL, NULL, 0, NULL); 
        }

        } else {
            printf("Erro processando consulta: %d\n", ret);
        }
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    } else {
        printf("Erro na alocacao de memoria driver SQL. %d\n", ret);
    }
}

#endif //SERVICO_H