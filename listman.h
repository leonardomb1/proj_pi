#ifndef LISTMAN_H
#define LISTMAN_H

Lista *criar_lista()
{
    Lista *lista = (Lista *)malloc(sizeof(Lista));
    lista->inicio = NULL;
    lista->tamanho = 0;
    return lista;
}

// Criando uma função para liberar a memoria de uma lista
void liberar_lista(Lista *lista)
{
    Funcionario *aux = lista->inicio;
    while (aux != NULL)
    {
        Funcionario *prox = aux->prox;
        free(aux);
        aux = prox;
    }
    free(lista);
}

// Criando uma função para inserir um funcionario no inicio da lista
void inserir_inicio(Lista *lista, Funcionario *funcionario)
{
    funcionario->prox = lista->inicio;
    lista->inicio = funcionario;
    lista->tamanho++;
}

// Criando uma função para inserir um funcionario no final da lista
void inserir_final(Lista *lista, Funcionario *funcionario)
{
    if (lista->inicio == NULL)
    {
        inserir_inicio(lista, funcionario);
    }
    else
    {
        Funcionario *aux = lista->inicio;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = funcionario;
        funcionario->prox = NULL;
        lista->tamanho++;
    }
}

// Criando uma função para remover um funcionario da lista pelo cpf
int remover_funcionario(Lista *lista, char *cpf)
{
    if (lista->inicio == NULL)
    {
        return 0; // Lista vazia
    }
    Funcionario *ant = NULL;
    Funcionario *aux = lista->inicio;
    while (aux != NULL && strcmp(aux->cpf, cpf) != 0)
    {
        ant = aux;
        aux = aux->prox;
    }
    if (aux == NULL)
    {
        return 0; // cpf não encontrado
    }
    if (ant == NULL)
    {
        lista->inicio = aux->prox;
    }
    else
    {
        ant->prox = aux->prox;
    }
    free(aux);
    lista->tamanho--;
    return 1; // cpf removido com sucesso
}

// Criando uma função para buscar um funcionario na lista pelo cpf
Funcionario *buscar_funcionario(Lista *lista, char *cpf)
{
    Funcionario *aux = lista->inicio;
    while (aux != NULL && strcmp(aux->cpf, cpf) != 0)
    {
        aux = aux->prox;
    }
    return aux; // Retorna o funcionario encontrado ou NULL se nÃ£o encontrar
}

// Criando uma função para imprimir o funcionario na tela
void imprimir_funcionario(Funcionario *funcionario)
{
    printf("Cpf: %s\n", funcionario->cpf);
    printf("Nome: %s\n", funcionario->nome);
    printf("Cargo: %s\n", funcionario->cargo);
    printf("Mes: %d\n", funcionario->mes);
    printf("Ano: %d\n", funcionario->ano);
    printf("Salario: %f\n", funcionario->salario);
}

// Criando uma função para imprimir a lista de funcionarios na tela
void imprimir_lista(Lista *lista)
{
    Funcionario *aux = lista->inicio;
    while (aux != NULL)
    {
        imprimir_funcionario(aux);
        printf("\n");
        aux = aux->prox;
    }
}

// Criando uma função para ler os dados de um funcionario a partir do teclado
Funcionario *ler_funcionario()
{
    Funcionario *funcionario = (Funcionario *)malloc(sizeof(Funcionario));
    printf("Digite o cpf do funcionario: ");
    scanf("%[^\n]", funcionario->cpf);
    getchar();
    printf("Digite o nome do funcionario: ");
    scanf("%[^\n]", funcionario->nome);
    getchar();
    printf("Digite o mes de admissao do funcionario: ");
    scanf(" %d", &funcionario->mes);
    getchar();
    printf("Digite o ano de admissao do funcionario: ");
    scanf(" %d", &funcionario->ano);
    getchar();
    printf("Digite o salario do funcionario: ");
    scanf(" %f", &funcionario->salario);
    getchar();
    printf("Digite o cargo do funcionario: ");
    scanf("%[^\n]", funcionario->cargo);
    getchar();
    return funcionario;
}



#endif //LISTMAN_H