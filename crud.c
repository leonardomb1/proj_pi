#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Definindo uma estrutura para representar um livro
typedef struct livro
{
    char titulo[100];
    char autor[100];
    int mes;
    int ano;
    char idioma[20];
    char tipo[20];
    struct livro *prox;
} Livro;

// Definindo uma estrutura para representar uma lista encadeada de livros
typedef struct lista
{
    Livro *inicio;
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

// Criando uma função para liberar a memória de uma lista
void liberar_lista(Lista *lista)
{
    Livro *aux = lista->inicio;
    while (aux != NULL)
    {
        Livro *prox = aux->prox;
        free(aux);
        aux = prox;
    }
    free(lista);
}

// Criando uma função para inserir um livro no início da lista
void inserir_inicio(Lista *lista, Livro *livro)
{
    livro->prox = lista->inicio;
    lista->inicio = livro;
    lista->tamanho++;
}

// Criando uma função para inserir um livro no final da lista
void inserir_final(Lista *lista, Livro *livro)
{
    if (lista->inicio == NULL)
    {
        inserir_inicio(lista, livro);
    }
    else
    {
        Livro *aux = lista->inicio;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = livro;
        livro->prox = NULL;
        lista->tamanho++;
    }
}

// Criando uma função para remover um livro da lista pelo título
int remover_titulo(Lista *lista, char *titulo)
{
    if (lista->inicio == NULL)
    {
        return 0; // Lista vazia
    }
    Livro *ant = NULL;
    Livro *aux = lista->inicio;
    while (aux != NULL && strcmp(aux->titulo, titulo) != 0)
    {
        ant = aux;
        aux = aux->prox;
    }
    if (aux == NULL)
    {
        return 0; // Livro não encontrado
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
    return 1; // Livro removido com sucesso
}

// Criando uma função para buscar um livro na lista pelo título
Livro *buscar_titulo(Lista *lista, char *titulo)
{
    Livro *aux = lista->inicio;
    while (aux != NULL && strcmp(aux->titulo, titulo) != 0)
    {
        aux = aux->prox;
    }
    return aux; // Retorna o livro encontrado ou NULL se não encontrar
}

// Criando uma função para imprimir um livro na tela
void imprimir_livro(Livro *livro)
{
    printf("Título: %s\n", livro->titulo);
    printf("Autor: %s\n", livro->autor);
    printf("Mês: %d\n", livro->mes);
    printf("Ano: %d\n", livro->ano);
    printf("Idioma: %s\n", livro->idioma);
    printf("Tipo: %s\n", livro->tipo);
}

// Criando uma função para imprimir a lista de livros na tela
void imprimir_lista(Lista *lista)
{
    Livro *aux = lista->inicio;
    while (aux != NULL)
    {
        imprimir_livro(aux);
        printf("\n");
        aux = aux->prox;
    }
}

// Criando uma função para ler os dados de um livro a partir do teclado
Livro *ler_livro()
{
    Livro *livro = (Livro *)malloc(sizeof(Livro));
    printf("Digite o título do livro: ");
    scanf("%[^\n]", livro->titulo);
    getchar();
    printf("Digite o autor do livro: ");
    scanf("%[^\n]", livro->autor);
    getchar();
    printf("Digite o mês de publicação do livro: ");
    scanf(" %d", &livro->mes);
    getchar();
    printf("Digite o ano de publicação do livro: ");
    scanf(" %d", &livro->ano);
    getchar();
    printf("Digite o idioma do livro: ");
    scanf("%[^\n]", livro->idioma);
    getchar();
    printf("Digite o tipo do livro: ");
    scanf("%[^\n]", livro->tipo);
    getchar();
    return livro;
}

// Criando uma função para carregar os dados dos livros a partir de um arquivo
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
    fgets(linha, 300, arq); // Ignorando a primeira linha do arquivo (cabeçalho)
    while (fgets(linha, 300, arq) != NULL)
    {
        Livro *livro = (Livro *)malloc(sizeof(Livro));
        char *token = strtok(linha, ";");
        strcpy(livro->titulo, token);
        token = strtok(NULL, ";");
        strcpy(livro->autor, token);
        token = strtok(NULL, ";");
        livro->mes = atoi(token);
        token = strtok(NULL, ";");
        livro->ano = atoi(token);
        token = strtok(NULL, ";");
        strcpy(livro->idioma, token);
        token = strtok(NULL, "\n");
        strcpy(livro->tipo, token);
        inserir_final(lista, livro);
    }
    fclose(arq);
    return lista;
}

// Criando uma função para salvar os dados dos livros em um arquivo
void salvar_arquivo(char *nome_arquivo, Lista *lista)
{
    FILE *arq = fopen(nome_arquivo, "w");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(arq, "titulo_livro;nome_autor;mes;ano;idioma;tipo_livro\n"); // Escrevendo o cabeçalho do arquivo
    Livro *aux = lista->inicio;
    while (aux != NULL)
    {
        fprintf(arq, "%s;%s;%d;%d;%s;%s\n", aux->titulo, aux->autor, aux->mes, aux->ano, aux->idioma, aux->tipo); // Escrevendo os dados de cada livro no arquivo
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
    printf("Sistema CRUD de Livros\n");
    printf("Escolha uma opção:\n");
    printf("1 - Criar um novo livro\n");
    printf("2 - Ler um livro pelo título\n");
    printf("3 - Atualizar um livro pelo título\n");
    printf("4 - Deletar um livro pelo título\n");
    printf("5 - Listar todos os livros\n");
    printf("6 - Sair do sistema\n");
}

// Criando a função principal do programa
int main()
{
    // Carregando os dados dos livros a partir do arquivo livros.txt
    Lista *lista = carregar_arquivo("livros.txt");

    // Variável para armazenar a opção escolhida pelo usuário
    int opcao;

    // Laço para repetir o menu até o usuário sair do sistema
    do
    {
        cabecalho();
        mostrar_menu(); // Mostrando o menu de opções
        printf("Digite a sua opção: ");
        scanf("%d", &opcao);
        getchar();
        switch (opcao)
        {
        case 1:                                  // Criar um novo livro
            Livro *novo = ler_livro();           // Lendo os dados do livro a partir do teclado
            inserir_final(lista, novo);          // Inserindo o livro no final da lista
            salvar_arquivo("livros.txt", lista); // Salvando os dados da lista no arquivo
            printf("Livro criado com sucesso.\n");
            sleep(1);
            system("cls");
            break;
        case 2: // Ler um livro pelo título
            char titulo[100];
            printf("Digite o título do livro que deseja ler: ");
            scanf("%[^\n]", titulo);
            getchar();
            Livro *busca = buscar_titulo(lista, titulo); // Buscando o livro na lista pelo título
            if (busca == NULL)
            {
                printf("Livro não encontrado.\n");
            }
            else
            {
                imprimir_livro(busca); // Imprimindo o livro na tela
            }
            system("pause");
            system("cls");
            break;
        case 3: // Atualizar um livro pelo título
            titulo[100];
            printf("Digite o título do livro que deseja atualizar: ");
            scanf("%[^\n]", titulo);
            getchar();
            Livro *atual = buscar_titulo(lista, titulo); // Buscando o livro na lista pelo título
            if (atual == NULL)
            {
                printf("Livro não encontrado.\n");
            }
            else
            {
                Livro *novo = ler_livro();           // Lendo os novos dados do livro a partir do teclado
                strcpy(atual->titulo, novo->titulo); // Copiando os novos dados para o livro antigo
                strcpy(atual->autor, novo->autor);
                atual->mes = novo->mes;
                atual->ano = novo->ano;
                strcpy(atual->idioma, novo->idioma);
                strcpy(atual->tipo, novo->tipo);
                free(novo);                          // Liberando a memória do livro temporário
                salvar_arquivo("livros.txt", lista); // Salvando os dados da lista no arquivo
                printf("Livro atualizado com sucesso.\n");
            }
            sleep(1);
            system("cls");
            break;
        case 4: // Deletar um livro pelo título
            titulo[100];
            printf("Digite o título do livro que deseja deletar: ");
            scanf("%[^\n]", titulo);
            getchar();
            int removido = remover_titulo(lista, titulo); // Removendo o livro da lista pelo título
            if (removido == 0)
            {
                printf("Livro não encontrado.\n");
            }
            else
            {
                salvar_arquivo("livros.txt", lista); // Salvando os dados da lista no arquivo
                printf("Livro deletado com sucesso.\n");
            }
            sleep(1);
            system("cls");
            break;
        case 5:                    // Listar todos os livros
            imprimir_lista(lista); // Imprimindo a lista de livros na tela
            system("pause");
            system("cls");
            break;
        case 6: // Sair do sistema
            printf("Obrigado por usar o sistema CRUD de livros. Até mais!\n");
            sleep(1);
            system("cls");
            break;
        default: // Opção inválida
            printf("Opção inválida. Tente novamente.\n");
            sleep(1);
            system("cls");
        }

    } while (opcao != 6);

    // Liberando a memória da lista de livros
    liberar_lista(lista);

    return 0;
}
