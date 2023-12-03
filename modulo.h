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


// Este arquivo é utilizado para criar os cenários de escolha para o usuario nos diferentes modulos


void menuLogin();
void moduloFuncionario();
void moduloVenda();

// Funcão menu principal após autenticação do login. Mostra opções para o usuário selecionar.
void menuPrincipal()
{
    int opcao;
    do
    {
        cabecaMenuInicio(); // Chama interface inicial
        printf("Digite a sua opcao: ");
        scanf(" %i", &opcao);
        getchar();
        switch (opcao)
        {
        case 1:
            system("cls");
            moduloFuncionario();
            break;
        case 2:
            system("cls");
            moduloVenda();
            break;
        case 3:
            exit(1);
            break;
        case 4:
            system("cls");
            menuLogin();
            break;
        default:
            printf(msg_erro_entrada); // Caso for passado valor fora de 1 a 4
            sleep(1);
            system("cls");
            break;
        }
    } while (opcao != 3 || opcao !=4); // Se o usuario escolher sair ou entrar como outro usuario sai do laço.
}

// Função de entrada do usuário. Usada para atribuir sessão do usuário a variavel "usrid" depois de autenticada.
void menuLogin() {
    errosPrint(iniciarServico()); //Inicia serviço de conexão ao SQL com driver ODBC
    system("cls");
    char barra[] = "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#"; // Barra da interface
    int largura = 150;
    do
    {
        int i = 0;
        char ch;
        usrid = 0;
        for(int i = 0; i<4; i++) {centralizaTextoPdAmbos(barra, largura); printf("\n");}
        printf(ANSI_COLOR_GREEN);
        centralizaTextoPdAmbos("Sistema de Recursos Empresariais - V.1", largura);
        printf(ANSI_COLOR_RESET);

        printf(ANSI_COLOR_GREEN);
        centralizaTextoPdEsquerda("Usuario: ", 160); // Centraliza espaço na tela
        printf(ANSI_COLOR_RESET);
        scanf("%s", usuario);
        
        printf(ANSI_COLOR_GREEN);
        centralizaTextoPdEsquerda("Senha: ", largura); // Centraliza espaço na tela
        printf(ANSI_COLOR_RESET);
        // O trecho abaixo converte a entrada do usuário em asterisco, para não mostrar sua senha
        while (1) {
            ch = _getch();
            if (ch == '\r') { 
                break;
            } else if (isprint(ch)) {
                senha[i] = ch;
                i++;
                printf("*");
            } else if (ch == '\b') {
                if (i > 0) {
                    printf("\b \b");
                    i--;
                }
            }
        }

        senha[i] = '\0';

        printf("\n");
        centralizaTextoPdEsquerda("", 125); // Centraliza espaço na tela
        validacaoLogin(usuario, senha); // Envia, por meio de consulta, a entrada do usuario para autenticação.
        if (usrid==0) // Se não encontrado é retornado 0 por padrão. O que reinicia o laço de repetição
        {
            centralizaTextoPdEsquerda("", 125);
            printf("  Usuario nao Autorizado!\n");
            system("pause");
            system("cls");
        }

    } while (usrid == 0);

    liberarServico(); // Libera memoria e conexão com servidor.
    
    // Trecho abaixo imprime o nome da empresa em ASCII Art uma linha por vez
    for (int frame = 0; frame < sizeof(nome_empresa_banner) / sizeof(nome_empresa_banner[0]); frame++) {
        centralizaTextoPdAmbos("",30);
        printf(ANSI_COLOR_YELLOW "%s\n" ANSI_COLOR_RESET, nome_empresa_banner[frame]);
        fflush(stdout);
        usleep(300000);
    }

    printf("\n");

    printf("Pressione qualquer tecla para continuar...");
    system("pause");
    system("cls");
    menuPrincipal();
}


// Função que chama as outras funções relacionadas a vendas, a depender da escolha do usuário.
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
                getchar();
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

// Função que chama as outras funções relacionadas aos funcionarios, a depender da escolha do usuário.
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
                getchar();
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