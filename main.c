#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "arquivar.h"
#include "lista.h"

#define NOME_EMPRESA "Safe Start Ltd."

char *campos_funcionarios[] = {"CPF", "Nome", "Cargo", "Mes", "Ano", "Salario"};
char *campos_produto[] = {"Codigo Produto", "Descricao Produto", "Armazem", "Endereco", "Custo Unitario"};

void mostrar_menu_principal()
{
    printf("Sistema de recursos empresariais - %s \n", NOME_EMPRESA);
    printf("Escolha uma opcao:\n");
    printf("1 - Menu de cadastro de funcionarios\n");
    printf("2 - Menu de cadastro de produtos\n");    
    printf("3 - Sair do Sistema\n");
}

void mostrar_menu_funcionario()
{
    printf("Sistema de administração de funcionarios\n");
    printf("Escolha uma opção:\n");
    printf("1 - Criar um novo cadastro\n");
    printf("2 - Ler um cadastro pelo cpf\n");
    printf("3 - Atualizar um cadastro\n");
    printf("4 - Deletar um cadastro pelo cpf\n");
    printf("5 - Listar todos os cadastros\n");
    printf("6 - Sair do modulo\n");
}

void menu_principal();

void menu_funcionario()
{
    char arquivo_funcionarios[20] = "funcionarios.txt";
    Lista *lista = carregar_arquivo(arquivo_funcionarios, sizeof(Funcionario), (void (*)(Lista *, void *))add);
    char cpf[11];
    int opcao;

    do
    {
        mostrar_menu_funcionario();
        printf("Digite a sua opção: ");
        scanf(" %i", &opcao);
        getchar();

        switch (opcao)
        {
            case 1:
                criar();
                novo_registro(lista, sizeof(Funcionario), campos_funcionarios, arquivo_funcionarios, ler_dados, add);
                sleep(1);
                system("cls");
                break;

            case 2:
                printf("Digite o cpf do funcionario que deseja ler: ");
                scanf("%[^\n]", cpf);
                getchar();
                Funcionario *busca = buscar(lista, sizeof(Funcionario), campos_funcionarios, cpf, "CPF");
                if (busca == NULL)
                {
                    printf("Funcionario não encontrado.\n");
                }
                else
                {
                    imprimir_struct(busca, sizeof(Funcionario), campos_funcionarios);
                }
                system("cls");
                break;

            case 3:
                atualizar_dados(lista, sizeof(Funcionario), campos_funcionarios, "cpf", buscar, ler_dados);
                getchar();
                sleep(1);
                system("cls");
                break;

            case 4:
                remover_dados(lista, sizeof(Funcionario), campos_funcionarios, "cpf", buscar);
                sleep(1);
                system("cls");
                break;

            case 5:
                imprimir_lista(lista, sizeof(Funcionario), campos_funcionarios);
                printf("Pressione qualquer botao para sair.");
                getchar();
                system("cls");
                break;

            case 6:
                menu_principal();
                system("cls");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
                sleep(1);
                system("cls");

        }

    } while (opcao != 6);

    liberar(lista, sizeof(Funcionario));
}

void menu_principal()
{
    int opcao;
    do
    {
        mostrar_menu_principal();
        printf("Digite a sua opcao: ");
        scanf(" %i", &opcao);
        getchar();
        switch (opcao)
        {
        case 1:
            menu_funcionario();
            break;
        case 2:
            //menu_produto();
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
            sleep(1);
            system("cls");
        }
    } while (opcao != 3);
}

int main()
{
    menu_principal();
    return 0;
}