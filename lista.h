#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct ponto {
    void *dados;
    struct ponto *prox;
} Ponto;

typedef struct lista {
    Ponto *inicio;
    int tamanho;
} Lista;

Lista *criar()
{
    Lista *lista = (Lista *)malloc(sizeof(Lista));
    lista->inicio = NULL;
    lista->tamanho = 0;
    return lista;
}

void add(Lista *lista, void *dados) {
    Ponto *ponto = (Ponto *)malloc(sizeof(Ponto));
    ponto->dados = dados;
    ponto->prox = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = ponto;
    } else {
        Ponto *atual = lista->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = ponto;
    }

    lista->tamanho++;
}

void *retorna(Lista *lista, int indice) {
    if (indice < 0 || indice >= lista->tamanho) {
        return NULL;
    }

    Ponto *atual = lista->inicio;
    for (int i = 0; i < indice; i++) {
        atual = atual->prox;
    }

    return atual->dados;
}


void liberar(void *dados, size_t tamanho_struct) {
    Ponto *atual = ((Lista *)dados)->inicio;
    while (atual != NULL) {
        Ponto *prox = atual->prox;
        free(atual->prox);
        free(atual);
        atual = prox;
    }
    free(dados);
}

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

#endif //LISTA_H