#ifndef ARQUIVAR_H
#define ARQUIVAR_H

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


Lista *carregar_arquivo(char *nome_arquivo, size_t tamanho_struct, void (*inserir)(Lista *, void *)) {
    Lista *lista = (Lista *)malloc(sizeof(Lista));
    lista->inicio = NULL;
    lista->tamanho = 0;

    FILE *arq = fopen(nome_arquivo, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return lista;
    }

    char linha[300];
    fgets(linha, 300, arq); // Ignorando a primeira linha do arquivo (cabecalho)
    while (fgets(linha, 300, arq) != NULL) {
        void *dados = malloc(tamanho_struct);
        char *token = strtok(linha, ";");
        for (int i = 0; i < tamanho_struct / sizeof(char *); i++) {
            char **campo = (char **)((char *)dados + i * sizeof(char *));
            *campo = strdup(token);
            token = strtok(NULL, ";");
        }
        inserir(lista, dados);
    }

    fclose(arq);
    return lista;
}

void salvar_arquivo(char *nome_arquivo, Lista *lista, size_t tamanho_struct, char **campos) {
    FILE *arq = fopen(nome_arquivo, "w");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < tamanho_struct / sizeof(char *); i++) {
        fprintf(arq, "%s", campos[i]);
        if (i < tamanho_struct / sizeof(char *) - 1) {
            fprintf(arq, ";");
        }
    }
    fprintf(arq, "\n");

    Ponto *atual = lista->inicio;
    while (atual != NULL) {
        void *dados = atual->dados;
        for (int i = 0; i < tamanho_struct / sizeof(char *); i++) {
            char **campo = (char **)((char *)dados + i * sizeof(char *));
            fprintf(arq, "%s", *campo);
            if (i < tamanho_struct / sizeof(char *) - 1) {
                fprintf(arq, ";");
            }
        }
        fprintf(arq, "\n");
        atual = atual->prox;
    }

    fclose(arq);
}

void imprimir_struct(void *dados, size_t tamanho_struct, char **campos) {
    for (int i = 0; i < tamanho_struct / sizeof(char *); i++) {
        char **campo = (char **)((char *)dados + i * sizeof(char *));
        printf("%s: %s\n", campos[i], *campo);
    }
}

void imprimir_lista(void *dados, size_t tamanho_struct, char **campos) {
    printf("Lista:\n");
    for (Ponto *atual = ((Lista *)dados)->inicio; atual != NULL; atual = atual->prox) {
        for (int i = 0; i < tamanho_struct / sizeof(char *); i++) {
            char **campo = (char **)((char *)atual->dados + i * sizeof(char *));
            printf("%s: %s\n", campos[i], *campo);
        }
        printf("\n");
    }
}

void *buscar(void *dados, size_t tamanho_struct, char **campos, char *chave, char *campo_procurado) {
    int indice_campo = -1;
    for (int i = 0; i < tamanho_struct / sizeof(char *); i++) {
        if (strcmp(campos[i], campo_procurado) == 0) {
            indice_campo = i;
            break;
        }
    }
    if (indice_campo == -1) {
        return NULL;
    }

    for (Ponto *atual = ((Lista *)dados)->inicio; atual != NULL; atual = atual->prox) {
        void *struct_dados = atual->dados;
        char **campo = (char **)((char *)struct_dados + indice_campo * sizeof(char *));
        if (strcmp(*campo, chave) == 0) {
            return struct_dados;
        }
    }

    return NULL;
}

void *ler_dados(size_t tamanho_struct, char **campos) {
    void *dados = malloc(tamanho_struct);
    for (int i = 0; i < tamanho_struct / sizeof(char *); i++) {
        printf("Digite o valor de %s: ", campos[i]);
        char **campo = (char **)((char *)dados + i * sizeof(char *));
        scanf(" %[^\n]", *campo);
        getchar();
    }
    return dados;
}

void atualizar_dados(void *dados, size_t tamanho_struct, char **campos, char *campo_procura,  void *(*buscar)(void *, size_t, char **, char *, char *), void *(*ler_dados)(size_t, char **)) {
    char chave[100];
    printf("Digite o valor da chave: ");
    scanf("%[^\n]", chave);
    getchar();

    void *struct_dados = buscar(dados, tamanho_struct, campos, chave, campo_procura);
    if (struct_dados == NULL) {
        printf("Registro nao encontrado.\n");
        return;
    }

    void *novos_dados = ler_dados(tamanho_struct, campos);
    memcpy(struct_dados, novos_dados, tamanho_struct);
    free(novos_dados);

    printf("Registro atualizado com sucesso.\n");
}

void remover_dados(void *dados, size_t tamanho_struct, char **campos, char *busca_campo,void *(*buscar)(void *, size_t, char **, char *, char *)) {
    char chave[100];
    printf("Digite o valor da chave: ");
    scanf("%[^\n]", chave);
    getchar();

    Ponto *atual = ((Lista *)dados)->inicio;
    Ponto *anterior = NULL;
    while (atual != NULL) {
        void *struct_dados = atual->dados;
        char **campo = NULL;
        for (int i = 0; i < tamanho_struct / sizeof(char *); i++) {
            if (strcmp(campos[i], busca_campo) == 0) {
                campo = (char **)((char *)struct_dados + i * sizeof(char *));
                break;
            }
        }
        if (campo != NULL && strcmp(*campo, chave) == 0) {
            if (anterior == NULL) {
                ((Lista *)dados)->inicio = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(struct_dados);
            free(atual);
            ((Lista *)dados)->tamanho--;
            printf("Registro removido com sucesso.\n");
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    printf("Registro nao encontrado.\n");
}

void novo_registro(Lista *lista, size_t tamanho_struct, char **campos, char *nome_arquivo, void *(*ler_dados)(size_t, char **), void (*add)(Lista *, void *)) {
    void *novo = ler_dados(tamanho_struct, campos);
    add(lista, novo);
    salvar_arquivo(nome_arquivo, lista, tamanho_struct, campos);
    printf("Cadastro criado com sucesso.\n");
}

#endif //ARQUIVAR_H