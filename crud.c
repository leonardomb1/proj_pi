#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "structdef.h"
#include "listman.h"
#include "fileman.h"

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