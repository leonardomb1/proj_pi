#ifndef FILEMAN_H
#define FILEMAN_H

Lista *carregar_arquivo(char *nome_arquivo)
{
    Lista *lista = criar_lista();
    FILE *arq = fopen(nome_arquivo, "r");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return lista;
    }
    char linha[300];
    fgets(linha, 300, arq); // Ignorando a primeira linha do arquivo (cabecalho)
    while (fgets(linha, 300, arq) != NULL)
    {
        Funcionario *funcionario = (Funcionario *)malloc(sizeof(Funcionario));
        char *token = strtok(linha, ";");
        strcpy(funcionario->cpf, token);
        token = strtok(NULL, ";");
        strcpy(funcionario->nome, token);
        token = strtok(NULL, ";");
        funcionario->mes = atoi(token);
        token = strtok(NULL, ";");
        funcionario->ano = atoi(token);
        token = strtok(NULL, ";");
        funcionario->salario = atof(token);
        token = strtok(NULL, "\n");
        strcpy(funcionario->cargo, token);
        inserir_final(lista, funcionario);
    }
    fclose(arq);
    return lista;
}

// Criando uma função para salvar os dados dos funcionarios em um arquivo
void salvar_arquivo(char *nome_arquivo, Lista *lista)
{
    FILE *arq = fopen(nome_arquivo, "w");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(arq, "cpf;nome_func;mes;ano;salario;cargo\n"); // Escrevendo o cabeÃ§alho do arquivo
    Funcionario *aux = lista->inicio;
    while (aux != NULL)
    {
        fprintf(arq, "%s;%s;%d;%d;%f;%s\n", aux->cpf, aux->nome, aux->mes, aux->ano, aux->salario, aux->cargo); // Escrevendo os dados de cada funcionario no arquivo
        aux = aux->prox;
    }
    fclose(arq);
}

#endif //FILEMAN_H