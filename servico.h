#ifndef SERVICO_H
#define SERVICO_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include "estruturas.h"
#include "interface.h"

// Definição dos tipos de retorno das funções de chamado ao servidor, para finalidade de debug.

#define SUCESSO 1
#define ERRO_CONFIG 10
#define ERRO_ALOCACAO_MEMORIA 11
#define ERRO_EXECUCAO_DRIVER 12
#define ERRO_EXECUTAR_CONSULTA 13
#define ERRO_INTERPRETACAO_CONSULTA 14
#define ERRO_ABRIR_ARQUIVO 15

char usuario[50];
char senha[50];

// Variavel que armazena retorno do SQL com ID da sessão do usuário.
SQLINTEGER usrid;

// Variaveis da biblioteca ODBC para C que inicializam o serviço de conexão.
SQLHENV env;
SQLHDBC dbc;
SQLHSTMT stmt;
SQLRETURN ret;

// Função que realiza a leitura do arquivo .ini de configuração.
int leitorConfig(const char* arquivo, OdbcConfig* config)
{
    FILE* arq_config = fopen(arquivo, "r");
    if (arq_config == NULL) {
        return ERRO_CONFIG;
    }

    // O trecho abaixo procura pela "palavra-chave" dentro do arquivo, tira o "=", e pega toda a linha até o caractere de nova linha.
    char line[256];
    while (fgets(line, sizeof(line), arq_config)) {
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
            } else if (strcmp(key, "Conn_mode") == 0) {
                strncpy(config->conn_mode, value, sizeof(config->conn_mode));
            }
        }
    }

    return SUCESSO;
    fclose(arq_config);
}

// Função de inicialização da conexão, aloca-se memoria para varíaveis de conexão e inicializa-se struct que armazena os dados da conexão, realizando a leitura do .ini.
int iniciarServico()
{
    int exec;
    char conn_str[256]; 
    struct OdbcConfig conn; // Inicialização da struct que armazena configuração, na STACK, sendo valido apenas na chamada desta função.

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env); // Alocação em memoria HEAP para ambiente SQL.
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0); 
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    // Leitura do arquivo de conexão config.ini
    exec = leitorConfig("config.ini", &conn);

    // Tipo de configuração de conexão, local ou em nuvem. Cada tipo de conexão têm uma configuração distinta.
    if (exec == SUCESSO && strcmp("local", conn.conn_mode) == 0) {
        snprintf(conn_str, sizeof(conn_str),
                "DRIVER={%s};SERVER=%s;DATABASE=%s;ENCRYPT=NO;Trusted_Connection=YES",
                conn.driver, conn.server, conn.database); // Conexão Local, utiliza tipo de autenticação Windows para acesso ao servidor.
    } else if (exec == 1 && strcmp("azure", conn.conn_mode) == 0) {
        snprintf(conn_str, sizeof(conn_str),
                "DRIVER={%s};SERVER=%s;DATABASE=%s;UID=%s;PWD=%s;",
                conn.driver, conn.server, conn.database, conn.user, conn.password); // Conexão Azure, tipo de autenticação Sql Login.
    }
    else {
        return exec; // Retorna o codigo do erro caso retorno da função leitorConfig for diferente de 1.
    }

    // Conexão com Servidor com configurações lidas.
    ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)conn_str, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (SQL_SUCCEEDED(ret)) {
        return SUCESSO;
    }

    else {
        return ERRO_EXECUCAO_DRIVER;
    }
}

// Função para execução de consulta simples em SQL. Usada para execução de Procedure e Update.
int inserir(char* consulta)
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret))
    {
        // Execução da consulta dada no parametro da função
        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if(SQL_SUCCEEDED(ret))
        {
            // Libera memoria da consulta
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            printf("\nEnviado!");
            return SUCESSO;
        }
        else
        {
            // Libera memoria da consulta
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return ERRO_EXECUTAR_CONSULTA;
        }
    }

    else
    {
        // Libera memoria da consulta
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return ERRO_ALOCACAO_MEMORIA;
    }
}

// Função de consulta SQL com retorno esperado. Executa consulta e interpreta resultado em tela do prompt comando.
int leitura(SQLHDBC dbc, const char* consulta) {
    char nome_coluna[100];
    SQLSMALLINT num_colunas;
    SQLCHAR buffer[1024]; // Cria-se uma variavel temporaria buffer para interpretar os resultados da consulta.
    SQLLEN len;

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret)) {

        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if (SQL_SUCCEEDED(ret)) {
             // Utilizado para verificar quantidade de colunas na tabela, usado nos laços seguintes como parametro.
            SQLNumResultCols(stmt, &num_colunas);

            // Laço de repetição que imprime retorno do cabeçalho em tela.
            printf("\n" ANSI_COLOR_YELLOW);
            for (int i = 0; i < num_colunas; i++) {
                SQLDescribeCol(stmt, i + 1, nome_coluna, sizeof(nome_coluna), NULL, NULL, NULL, NULL, NULL);
                imprimeCelula(nome_coluna, PAD_SQL);
            }
            printf("\n" ANSI_COLOR_RESET);

            // Laço de repetição que imprime retorno dos registros da tabela em tela, quando sucedido.
            while (SQLFetch(stmt) == SQL_SUCCESS) {
                for (int i = 0; i < num_colunas; i++) {
                    while (SQLGetData(stmt, i + 1, SQL_C_CHAR, buffer, sizeof(buffer), &len) == SQL_SUCCESS) {
                        if (len > 0) {
                            imprimeCelula(buffer, PAD_SQL);
                        }
                    }
                }
                printf("\n");
            }
            // Libera memoria da consulta
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return SUCESSO;
        } else {
            // Libera memoria da consulta
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return ERRO_EXECUTAR_CONSULTA;
        }

    } else {
        // Libera memoria da consulta
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return ERRO_ALOCACAO_MEMORIA;
    }
}

// Libera serviço de conexão com servidor. Libera memoria alocada e corta conexão.
void liberarServico()
{
    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}

// Função especializada para validação do login do usuário. Realiza consulta em banco e atribui retorno, se encontrado, a variável "usrid" que age como ID da sessão do usuário 
int validacaoLogin(char* usuario, char* senha) {
    SQLRETURN ret;
    SQLLEN len;
    char consulta[255]; // Variavel que armazena consulta

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (SQL_SUCCEEDED(ret)) {
        // Interpola a consulta do banco com entrada do usuário
        snprintf(consulta, sizeof(consulta),
            "SELECT TOP 1 ID_REGUSR FROM TB_DIM_USUARIO WHERE NM_USUAID = '%s' AND VL_PASSWD = '%s'",
            usuario, senha
        );
        // Execução da consulta
        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);

        // Atribui a variavel "usrid" o ID da sessão do usuário.
        if (SQL_SUCCEEDED(ret)) {
            ret = SQLBindCol(stmt, 1, SQL_C_LONG, &usrid, sizeof(usrid), NULL);
            ret = SQLFetch(stmt);
            if (SQL_SUCCEEDED(ret)!=1) {
                SQLFreeHandle(SQL_HANDLE_DBC, dbc);
                return ERRO_INTERPRETACAO_CONSULTA;
            }
            // Libera memoria da conexão
            SQLFreeHandle(SQL_HANDLE_DBC, dbc);
            return SUCESSO;
        } else {
            // Libera memoria da consulta
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return ERRO_EXECUTAR_CONSULTA;
        }
    } else {
        // Libera memoria da consulta
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return ERRO_ALOCACAO_MEMORIA;
    }
}

// Função para exportação do retorno do servidor em arquivo. Utiliza-se uma consulta com retorno e inicializa uma variável do tipo config para armazenar o retorno, em modo "w"
int exportarRelatorio(SQLHDBC dbc, const char* arquivo, const char* consulta) {
    char nome_coluna[100];
    SQLSMALLINT num_colunas;
    SQLCHAR buffer[1024];
    SQLLEN len;

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    // O trecho abaixo recebe o retorno da variável em um arquivo passado no parametro da função, escrevendo neste em modo "w"
    if (SQL_SUCCEEDED(ret)) {
        ret = SQLExecDirect(stmt, (SQLCHAR*)consulta, SQL_NTS);
        if (SQL_SUCCEEDED(ret)) {
            // Utilizado para verificar quantidade de colunas na tabela, usado nos laços seguintes como parametro.
            SQLNumResultCols(stmt, &num_colunas);


            // Abre o arquivo, caso não esse exista será criado.
            FILE* arq = fopen(arquivo, "w");
            if (!arq) {
                SQLFreeHandle(SQL_HANDLE_STMT, stmt);
                return ERRO_ABRIR_ARQUIVO;
            }
            // Laço para retornar o cabeçalho da tabela.
            for (int i = 0; i < num_colunas; i++) {
                SQLDescribeCol(stmt, i + 1, nome_coluna, sizeof(nome_coluna), NULL, NULL, NULL, NULL, NULL);
                fprintf(arq, "\"%s\",", nome_coluna);
            }
            fprintf(arq, "\n");

            // Laço para retornar os registros da tabela.
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

            // Libera memoria da consulta e fecha arquivo
            fclose(arq);
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return SUCESSO;
        } else {
            // Libera memoria da consulta
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            return ERRO_EXECUTAR_CONSULTA;
        }
    } else {
        // Libera memoria da consulta
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return ERRO_ALOCACAO_MEMORIA;
    }
}


#endif //SERVICO_H