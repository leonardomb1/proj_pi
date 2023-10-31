#ifndef STRUCTDEF_H
#define STRUCTDEF_H

typedef struct funcionario
{
    char cpf[11];
    char nome[100];
    char cargo[100];
    int mes;
    int ano;
    float salario;
    struct funcionario *prox;
} Funcionario;

typedef struct produto
{
    int codigo_produto;
    char desc_produto[30];
    char armazem[2];
    char endereco[5];
    float custo_unitario;
    struct produto *prox;
} Produto;

typedef struct lista
{
    Funcionario *inicio;
    int tamanho;
} Lista;

#endif //STRUCTDEF_H