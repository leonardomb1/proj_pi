#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

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

typedef struct Venda
{
    char cnpj[15];
    int prod;
    int qtde_venda;
    float valor_venda;
} Venda;

typedef struct OdbcConfig
{
    char driver[256];
    char server[256];
    char database[256];
    char user[256];
    char password[256];
} OdbcConfig;

#endif //ESTRUTURAS_H