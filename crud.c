#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Definindo a estrutura do funcionario
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

// Definindo uma estrutura para representar uma lista encadeada
typedef struct lista
{
    Funcionario *inicio;
    int tamanho;
} Lista;

// Criando uma função para inicializar uma lista vazia
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

// Criando uma função para carregar os dados dos funcionarios a partir de um arquivo
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

void cabecalho()
{
    printf("=============================================\n");
    printf("==================SISTEMA====================\n");
    printf("=============================================\n");
}

// Criando uma função para mostrar o menu de opções do sistema CRUD
void mostrar_menu()
{
    printf("Sistema de administração de funcionarios\n");
    printf("Escolha uma opção:\n");
    printf("1 - Criar um novo cadastro\n");
    printf("2 - Ler um cadastro pelo cpf\n");
    printf("3 - Atualizar um cadastro\n");
    printf("4 - Deletar um cadastro pelo cpf\n");
    printf("5 - Listar todos os cadastros\n");
    printf("6 - Sair do sistema\n");
}

// Criando a função principal do programa
int main()
{
    // Carregando os dados dos funcionarios a partir do arquivo funcionarios.txt
    Lista *lista = carregar_arquivo("funcionarios.txt");
    char cpf[11];
    Funcionario *busca_cpfs = buscar_funcionario(lista, cpf);
    // Variavel para armazenar a opção escolhida pelo usuario
    int opcao;

    // laço para repetir o menu até o usuario sair do sistema
    do
    {
        cabecalho();
        mostrar_menu(); // Mostrando o menu de opção
        printf("Digite a sua opção: ");
        scanf("%d", &opcao);
        getchar();
        switch (opcao)
        {
        case 1:                                        // Criar um novo cadastro funcionario
            Funcionario *novo = ler_funcionario();     // Lendo os dados do funcionario a partir do teclado
            inserir_final(lista, novo);                // Inserindo o funcionario no final da lista
            salvar_arquivo("funcionarios.txt", lista); // Salvando os dados da lista no arquivo
            printf("Cadastro criado com sucesso.\n");
            sleep(1);
            system("cls");
            break;
        case 2: // Ler um cadastro pelo cpf
            cpf[11];
            printf("Digite o cpf do funcionario que deseja ler: ");
            scanf("%[^\n]", cpf);
            getchar();
            Funcionario *busca = busca_cpfs; // Buscando o funcionario na lista pelo cpf
            if (busca == NULL)
            {
                printf("Funcionario não encontrado.\n");
            }
            else
            {
                imprimir_funcionario(busca); // Imprimindo o funcionario na tela
            }
            system("pause");
            system("cls");
            break;
        case 3: // Atualizar um funcionario pelo cpf
            cpf[11];
            printf("Digite o cpf do cadastro que deseja atualizar: ");
            scanf("%[^\n]", cpf);
            getchar();
            Funcionario *atual = busca_cpfs; // Buscando o funcionario na lista pelo cpf
            if (atual == NULL)
            {
                printf("Funcionario não encontrado.\n");
            }
            else
            {
                Funcionario *novo = ler_funcionario(); // Lendo os novos dados do funcionario a partir do teclado
                strcpy(atual->cpf, novo->cpf);         // Copiando os novos dados para o funcionario antigo
                strcpy(atual->nome, novo->nome);
                atual->mes = novo->mes;
                atual->ano = novo->ano;
                atual->salario = novo->salario;
                strcpy(atual->cargo, novo->cargo);
                free(novo);                                // Liberando a memoria do funcionario temporario
                salvar_arquivo("funcionarios.txt", lista); // Salvando os dados da lista no arquivo
                printf("Cadastro atualizado com sucesso.\n");
            }
            sleep(1);
            system("cls");
            break;
        case 4: // Deletar um funcionario pelo cpf
            cpf[11];
            printf("Digite o cpf do cadastro que deseja deletar: ");
            scanf("%[^\n]", cpf);
            getchar();
            int removido = remover_funcionario(lista, cpf); // Removendo o funcionario da lista pelo cpf
            if (removido == 0)
            {
                printf("Cadastro não encontrado.\n");
            }
            else
            {
                salvar_arquivo("funcionario.txt", lista); // Salvando os dados do funcionario no arquivo
                printf("Cadastro deletado com sucesso.\n");
            }
            sleep(1);
            system("cls");
            break;
        case 5:                    // Listar todos os funcionarios
            imprimir_lista(lista); // Imprimindo a lista de funcionarios na tela
            system("pause");
            system("cls");
            break;
        case 6: // Sair do sistema
            printf("Obrigado por usar o sistema de administração de funcionarios. Até mais!\n");
            sleep(1);
            system("cls");
            break;
        default: // opção invalida
            printf("Opção inválida! Tente novamente.\n");
            sleep(1);
            system("cls");
        }

    } while (opcao != 6);

    // Liberando a memória da lista de funcionarios
    liberar_lista(lista);

    return 0;
}