#ifndef SERVICO_H
#define SERVICO_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include "estruturas.h"
#include "interface.h"

#define SUCESSO 1
#define ERRO_CONFIG 10
#define ERRO_ALOCACAO_MEMORIA 11
#define ERRO_EXECUCAO_DRIVER 12
#define ERRO_EXECUTAR_CONSULTA 13
#define ERRO_INTERPRETACAO_CONSULTA 14
#define ERRO_ABRIR_ARQUIVO 15

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
        return ERRO_CONFIG;
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

    return SUCESSO;
    fclose(file);
}

int iniciarServico()
{
    int exec;
    char conn_str[256]; 
    struct OdbcConfig conn;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    exec = leitorConfig("config.ini", &conn);

    if (exec == 1) {
        snprintf(conn_str, sizeof(conn_str),
                "DRIVER={%s};SERVER=%s;DATABASE=%s;UID=%s;PWD=%s",
                conn.driver, conn.server, conn.database,
                conn.user, conn.password);
    }

    else {
        return exec;
    }

    ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)conn_str, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (SQL_SUCCEEDED(ret)) {
        return SUCESSO;
    }

    else {
        return ERRO_EXECUCAO_DRIVER;
    }
}

int inserir(char* consulta)
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret))
    {
        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if(SQL_SUCCEEDED(ret))
        {
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            printf("\nEnviado!");
            return SUCESSO;
        }
        else
        {
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return ERRO_EXECUTAR_CONSULTA;
        }
    }

    else
    {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return ERRO_ALOCACAO_MEMORIA;
    }
}

int leitura(SQLHDBC dbc, const char* consulta) {
    char nome_coluna[100];
    SQLSMALLINT num_colunas;
    SQLCHAR buffer[1024];
    SQLLEN len;

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret)) {

        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if (SQL_SUCCEEDED(ret)) {
            SQLNumResultCols(stmt, &num_colunas);

            printf("\n");
            for (int i = 0; i < num_colunas; i++) {
                SQLDescribeCol(stmt, i + 1, nome_coluna, sizeof(nome_coluna), NULL, NULL, NULL, NULL, NULL);
                printCell(nome_coluna, PAD_SQL);
            }
            printf("\n");

            while (SQLFetch(stmt) == SQL_SUCCESS) {
                for (int i = 0; i < num_colunas; i++) {
                    while (SQLGetData(stmt, i + 1, SQL_C_CHAR, buffer, sizeof(buffer), &len) == SQL_SUCCESS) {
                        if (len > 0) {
                            printCell(buffer, PAD_SQL);
                        }
                    }
                }
                printf("\n");
            }

            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return SUCESSO;
        } else {
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return ERRO_EXECUTAR_CONSULTA;
        }

    } else {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return ERRO_ALOCACAO_MEMORIA;
    }
}

void liberarServico()
{
    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}

int validacaoLogin(char* usuario, char* senha) {
    SQLRETURN ret;
    SQLLEN len;
    char consulta[255];

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret)) {
        snprintf(consulta, sizeof(consulta),
            "SELECT TOP 1 ID_REGUSR FROM TB_DIM_USUARIO WHERE NM_USUAID = '%s' AND VL_PASSWD = '%s'",
            usuario, senha
        );

        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);

        if (SQL_SUCCEEDED(ret)) {
            ret = SQLBindCol(stmt, 1, SQL_C_LONG, &usrid, sizeof(usrid), NULL);
            ret = SQLFetch(stmt);
            if (SQL_SUCCEEDED(ret)!=1) {
                SQLFreeHandle(SQL_HANDLE_DBC, dbc);
                return ERRO_INTERPRETACAO_CONSULTA;
            }
            SQLFreeHandle(SQL_HANDLE_DBC, dbc);
            return SUCESSO;
        } else {
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return ERRO_EXECUTAR_CONSULTA;
        }
    } else {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return ERRO_ALOCACAO_MEMORIA;
    }
}


int exportarRelatorio(SQLHDBC dbc, const char* arquivo, const char* consulta) {
    char nome_coluna[100];
    SQLSMALLINT num_colunas;
    SQLCHAR buffer[1024];
    SQLLEN len;

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret)) {
        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if (SQL_SUCCEEDED(ret)) {
            SQLNumResultCols(stmt, &num_colunas);

            FILE* arq = fopen(arquivo, "w");
            if (!arq) {
                SQLFreeHandle(SQL_HANDLE_STMT, stmt);
                return ERRO_ABRIR_ARQUIVO;
            }

            for (int i = 0; i < num_colunas; i++) {
                SQLDescribeCol(stmt, i + 1, nome_coluna, sizeof(nome_coluna), NULL, NULL, NULL, NULL, NULL);
                fprintf(arq, "\"%s\",", nome_coluna);
            }
            fprintf(arq, "\n");

            while (SQLFetch(stmt) == SQL_SUCCESS) {
                for (int i = 0; i < num_colunas; i++) {
                    while (SQLGetData(stmt, i + 1, SQL_C_CHAR, buffer, sizeof(buffer), &len) == SQL_SUCCESS) {
                        if (len > 0) {
                            fprintf(arq, "\"%s\",", buffer);
                        }
                    }
                }
                fprintf(arq, "\n");
            }

            fclose(arq);
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return SUCESSO;
        } else {
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return ERRO_EXECUTAR_CONSULTA;
        }
    } else {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return ERRO_ALOCACAO_MEMORIA;
    }
}


#endif //SERVICO_H