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

void moduloFuncionario(int cenario)
{
    Funcionario* novo = (Funcionario*)malloc(sizeof(Funcionario));
    char cmd[255];
    char cpf[12];
    switch (cenario)
    {
    case 1:
        printf("\nDigite o CPF do Funcionario (Apenas os digitos): ");       
        while(1)
        {
            if(scanf("%11s", novo->cpf) != 1)
            {
                getchar();
            }
            else
            {  
                break;
            }
        }

        getchar();

        while(1)
        {
            printf("\nDigite o Nome do Funcionario: ");
            int tam_buffer = 256;
            char* buffer = malloc(tam_buffer);
            if (buffer==NULL)
            {
                printf("Falha na alocação de Memoria!\n");
                sleep(1);
                system("cls");
            }

            if(fgets(buffer, tam_buffer, stdin) == NULL)
            {
                free(buffer);
                printf(msg_erro_entrada);
                getchar();
            }
            else
            {
                if ((strlen(buffer) > 0) && (buffer[strlen (buffer) - 1] == '\n'))
                {
                    buffer[strlen(buffer) - 1] = '\0';
                }

                strcpy(novo->nome, buffer);
                free(buffer);
                break;
            }
        }



        printf("\nDigite a Data de admissao no formato aaaammdd (ex: 20231231): ");
        while(1)
        {
            if(scanf("%8s", novo->admissao) != 1)
            {
                printf(msg_erro_entrada);
                getchar();
            }
            else
            {
                break;
            }
        }
        iniciarServico();
        printf("\nSelecione o codigo do cargo desejado:\n");
        leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CARGO', 'ID_REGCAR', 'NM_CARGO', '', '', '', '', '', '';");
        printf("\nDigite: ");
        while(1)
        {
            if(scanf("%1i", &novo->cargo) != 1)
            {
                getchar();
            }
            else
            {
                break;
            }
        }

        printf("\nSelecione o codigo da funcao desejada:\n");

        leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FUNCAO', 'ID_REGFNC', 'NM_FUNCAO', '', '', '', '', '', '';");
        printf("\nDigite: ");
        while(1)
        {
            if(scanf(" %1i", &novo->funcao) != 1)
            {
                getchar();
            }
            else
            {
                break;
            }
        }

        getchar();
        printf("\nO funcionario tem dependente? (S/N)\n");
        printf("\nDigite: ");
        while (1)
        {
            char escolha = getchar();

            if (escolha == '\n')
                continue;

            escolha = tolower(escolha);
            if (escolha == 's' || escolha == 'n')
            {
                novo->dependente = (escolha == 's') ? 1 : 0;
                break;
            }
            else
            {
                printf("%s", msg_erro_entrada);
                getchar();
            }
        }


        printf("\nSelecione o codigo da filial desejada:\n");
        leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FILIAL', 'ID_REGFIL', 'NM_FILIAL', '', '', '', '', '', '';");
        printf("\nDigite: ");
        while(1)
        {
            if(scanf(" %1i", &novo->filial) != 1)
            {
                printf("%s", msg_erro_entrada);
                getchar();
            }
            else
            {
                break;
            }
        }

        printf("\nSelecione o codigo do departamento desejado:\n");
        leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_DEPTO', 'ID_REGDPT', 'NM_DEPTO', '', '', '', '', '', '';");
        printf("\nDigite: ");
        while(1)
        {
            if(scanf(" %1i", &novo->depto) != 1)
            {
                printf("%s", msg_erro_entrada);
                getchar();
            }
            else
            {
                break;
            }
        }

        printf("\nInforme o Salario Inicial do Funcionario: ");
        while(1)
        {
            if(scanf(" %f", &novo->salario) != 1)
            {
                printf("%s", msg_erro_entrada);
                getchar();
            }
            else
            {
                break;
            }
        }

        snprintf(cmd, sizeof(cmd), "INSERT INTO TB_DIM_FUNCIO VALUES('%s', UPPER('%s'), '%s', DEFAULT, %i, %i, %i, %i, %i, %i, DEFAULT, DEFAULT) INSERT INTO TB_FAT_SALFUN VALUES('%s', %f, %i, DEFAULT, DEFAULT, NULL, NULL);",
                    novo->cpf, novo->nome, novo->admissao, novo->cargo, novo->funcao, novo->dependente, novo->filial, novo->depto, usrid, novo->cpf, novo->salario, usrid
                );
        
        inserir(cmd);
        liberarServico();
        free(novo);
        break;  

    case 2:
        printf("\nInforme o CPF do Funcionario: ");
        while(1)
        {
            if(scanf(" %11s", cpf) != 1)
            {
                printf("%s", msg_erro_entrada);
                getchar();
            }
            else
            {
                break;
            }
        }
        snprintf(cmd, sizeof(cmd), "SELECT NOME,[Dt Admis],[Sit Folha],Cargo,Salario FROM MOD.VW_FUNCIONARIOS WHERE CPF_PURO = '%s'", cpf);
        iniciarServico();
        leitura(dbc, cmd);
        liberarServico();
        printf("\n");
        system("pause");
        system("cls");
        break;

    case 3:
        printf("\nInforme o CPF do Funcionario: ");
        while(1)
        {
            if(scanf(" %11s", cpf) != 1)
            {
                printf("%s", msg_erro_entrada);
                getchar();
            }
            else
            {
                break;
            }
        }

        int opcao_select;  
        do
        {
            Funcionario* alterado = (Funcionario*)malloc(sizeof(Funcionario));
            iniciarServico();
            escolhaAlteracaoFuncionario();
            scanf(" %i", &opcao_select);
            switch (opcao_select)
            {
                case 1:
                    printf("\nSelecione o codigo do cargo desejado:\n");
                    leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CARGO', 'ID_REGCAR', 'NM_CARGO', '', '', '', '', '', '';");
                    printf("\nDigite: ");
                    while(1)
                    {
                        if(scanf("%1i", &alterado->cargo) != 1)
                        {
                            getchar();
                        }
                        else
                        {
                            break;
                        }
                    }
                    snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_CARGO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->cargo, usrid, cpf);
                    inserir(cmd);
                    free(alterado);
                    getchar();
                    system("cls");                    
                    break;
                case 2:
                    printf("\nSelecione o codigo do departamento desejado:\n");
                    leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_DEPTO', 'ID_REGDPT', 'NM_DEPTO', '', '', '', '', '', '';");
                    printf("\nDigite: ");
                    while(1)
                    {
                        if(scanf(" %1i", &alterado->depto) != 1)
                        {
                            printf("%s", msg_erro_entrada);
                            getchar();
                        }
                        else
                        {
                            break;
                        }
                    }
                    snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_DEPRTO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->depto, usrid, cpf);
                    inserir(cmd);
                    free(alterado);
                    getchar();
                    system("cls");  
                    break;
                case 3:
                    printf("\nSelecione o codigo da filial desejada:\n");
                    leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FILIAL', 'ID_REGFIL', 'NM_FILIAL', '', '', '', '', '', '';");
                    printf("\nDigite: ");
                    while(1)
                    {
                        if(scanf(" %1i", &alterado->filial) != 1)
                        {
                            printf("%s", msg_erro_entrada);
                            getchar();
                        }
                        else
                        {
                            break;
                        }
                    }
                    snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_FILIAL = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->filial, usrid, cpf);
                    inserir(cmd);

                    free(alterado);
                    getchar();
                    system("cls");  
                    break;
                case 4:
                    printf("\nSelecione o codigo da situacao da folha desejada:\n");
                    leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_SITFOL', 'ID_REGSFL', 'DS_SITFOL', '', '', '', '', '', '';");
                    printf("\nDigite: ");
                    while(1)
                    {
                        if(scanf(" %1i", &alterado->situacao) != 1)
                        {
                            printf("%s", msg_erro_entrada);
                            getchar();
                        }
                        else
                        {
                            break;
                        }
                    }
                    snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_SITFOL = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->situacao, usrid, cpf);
                    inserir(cmd);
                    free(alterado);
                    getchar();
                    system("cls");  
                    break;
                case 5:
                    printf("\nSelecione o codigo da funcao desejada:\n");
                    leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FUNCAO', 'ID_REGFNC', 'NM_FUNCAO', '', '', '', '', '', '';");
                    printf("\nDigite: ");
                    while(1)
                    {
                        if(scanf(" %1i", &alterado->funcao) != 1)
                        {
                            getchar();
                        }
                        else
                        {
                            break;
                        }
                    }
                    snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_FUNCAO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->funcao, usrid, cpf);
                    inserir(cmd);
                    free(alterado);
                    getchar();
                    system("cls");  
                    break;
                case 6:
                    printf("\nO funcionario tem dependente? (S/N)\n");
                    printf("\nDigite: ");
                    while (1)
                    {
                        char escolha = getchar();

                        if (escolha == '\n')
                            continue;

                        escolha = tolower(escolha);
                        if (escolha == 's' || escolha == 'n')
                        {
                            alterado->dependente = (escolha == 's') ? 1 : 0;
                            break;
                        }
                        else
                        {
                            printf("%s", msg_erro_entrada);
                            getchar();
                        }
                    }
                    snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET VF_DEPEND = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->dependente, usrid, cpf);
                    inserir(cmd);
                    free(alterado);
                    getchar();
                    system("cls");  
                    break;
                case 7:
                    free(alterado);
                    system("cls");
                    break;
                
                default:
                    printf(msg_erro_entrada);
                    getchar();
                    system("cls");
                    break;
            }
        } while (opcao_select =! 7);
        liberarServico();
        break;
    
    case 4:
        iniciarServico();
        printf("\nInforme o CPF do Funcionario: ");
        while(1)
        {
            if(scanf(" %11s", cpf) != 1)
            {
                printf("%s", msg_erro_entrada);
                getchar();
            }
            else
            {
                break;
            }
        }

        snprintf(cmd, sizeof(cmd), "EXEC atualizarValorRegistro 'TB_DIM_FUNCIO', '0', 'CP_FUNCIO', '%s'; EXEC atualizarValorRegistro 'TB_FAT_SALFUN', '0', 'CE_FUNCIO', '%s';", cpf, cpf);
        inserir(cmd);
        system("pause");
        system("cls");
        break;
    case 5:
        iniciarServico();
        leitura(dbc, "SELECT NOME,CPF,[Dt Admis],[Sit Folha],Cargo,Salario FROM MOD.VW_FUNCIONARIOS;");
        getchar();
        system("cls");
    case 6:
        system("cls");
        break;
    default:
        printf(msg_erro_entrada);
        sleep(1);
        system("cls");
        break;
    }
}

#endif //MODULO_H