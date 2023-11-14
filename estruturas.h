#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Funcionario
{
    char cpf[12];
    char nome[31];
    char admissao[9];
    int cargo;
    int funcao;
    int dependente;
    int filial;
    int depto;
    float salario;
    int situacao;
} Funcionario;

typedef struct odbcConfig
{
    char driver[256];
    char server[256];
    char database[256];
    char user[256];
    char password[256];
} OdbcConfig;

#endif //ESTRUTURAS_H