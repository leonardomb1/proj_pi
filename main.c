#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sql.h>
#include <sqlext.h>
#include <conio.h>
#include "estruturas.h"
#include "servico.h"
#include "interface.h"
#include "modulo.h"

void menuLogin();
void menuPrincipal();
void centralizaTextoPdAmbos();
void centralizaTextoPdEsquerda();

int main()
{
    menuLogin();
    return 0;
}

void menuPrincipal()
{
    int opcao;
    do
    {
        cabecaMenuInicio();
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
            printf(msg_erro_entrada);
            sleep(1);
            system("cls");
            break;
        }
    } while (opcao != 3 || opcao !=4);
}

void centralizaTextoPdAmbos(char *texto, int largura) {
    int pad = (largura - strlen(texto)) / 2;
    printf("%*s%s%*s", pad, "", texto, pad, "");
}

void centralizaTextoPdEsquerda(char *texto, int largura) {
    int pad = (largura - strlen(texto)) / 2;
    printf("%*s%s", pad, "", texto);
}

void menuLogin() {
    iniciarServico();
    system("cls");
    char barra[] = "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#";
    int largura = 125;
    do
    {
        int i = 0;
        char ch;
        usrid = 0;
        for(int i = 0; i<4; i++) {centralizaTextoPdAmbos(barra, largura); printf("\n");}

        centralizaTextoPdEsquerda("Usuario: ", largura);
        scanf("%s", usuario);
        
        centralizaTextoPdEsquerda("Senha: ", largura);

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
        centralizaTextoPdEsquerda("", 100);
        validacaoLogin(usuario, senha);
        if (usrid==0)
        {
            centralizaTextoPdEsquerda("", 100);
            printf("  Usuario nao Autorizado!\n");
            system("pause");
            system("cls");
        }

    } while (usrid == 0);

    liberarServico();
    
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