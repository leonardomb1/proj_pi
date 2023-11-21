#ifndef MODULO_H
#define MODULO_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sql.h>
#include <sqlext.h>
#include "estruturas.h"
#include "servico.h"
#include "interface.h"
#include "cases.h"

void moduloVenda()
{
    int cenario;

    do
    {
        cabecaMenuVenda();
        printf("Digite a sua opcao: ");
        scanf(" %i", &cenario);

        switch (cenario)
        {
            case 1:
                cadastroCliente();
                system("cls");
                break;  

            case 2:
                cadastraVenda();
                getchar();
                system("cls");
                break;

            case 3:
                deletaCadastroCliente();
                getchar();
                system("pause");
                system("cls");
                break;
            case 4:
                mostrarTodosClientes();
                getchar();
                system("pause");
                system("cls");
                break;
            case 5:
                getchar();
                mostrarTodasVendas();
                system("cls");
                break;
            case 6:
                relatorioVendasCsv();
                getchar();
                system("cls");
                break;
            case 7:
                system("cls");
                break;
            default:
                printf(msg_erro_entrada);
                sleep(1);
                system("cls");
                break;
        }
    } while (cenario != 7);
     
}

void moduloFuncionario()
{
    int cenario;

    do
    {
        cabecaMenuFuncionario();
        printf("Digite a sua opcao: ");
        scanf(" %i", &cenario);

        switch (cenario)
        {
            case 1:
                criaCadastroFuncionario();
                system("cls");
                break;  

            case 2:
                mostrarFuncionario();
                getchar();
                system("pause");
                system("cls");
                break;

            case 3:
                atualizaCadastroFuncionario();
                getchar();
                system("pause");
                system("cls");
                break;
            
            case 4:
                deletaCadastroFuncionario();
                getchar();
                system("pause");
                system("cls");
                break;
            case 5:
                mostrarTodosFuncionarios();
                getchar();
                system("pause");
                system("cls");
                break;
            case 6:
                getchar();
                alterarSalario();
                system("cls");
                break;
            case 7:
                cadastroCargo();
                system("cls");
                break;
            case 8:
                relatorioFuncionariosCsv();
                getchar();
                system("cls");
                break;
            case 9:
                system("cls");
                break;
            default:
                printf(msg_erro_entrada);
                sleep(1);
                system("cls");
                break;
        }
    } while (cenario != 9);
     
}

#endif //MODULO_H