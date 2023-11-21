#ifndef CASES_H
#define CASES_H

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

void criaCadastroFuncionario()
{
    Funcionario* novo = (Funcionario*)malloc(sizeof(Funcionario));
    char cmd[255];
    errosPrint(iniciarServico());
    printf("\nDigite o CPF do Funcionario (Apenas os digitos): ");       
    scanf(" %11s", novo->cpf);
    getchar();

    while(1)
    {
        printf("\nDigite o Nome do Funcionario: ");
        int tam_buffer = 256;
        char* buffer = (char*)malloc(tam_buffer);
        if (buffer==NULL)
        {
            errosPrint(ERRO_ALOCACAO_MEMORIA);
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
    scanf("%8s", novo->admissao);

    printf("\nSelecione o codigo do cargo desejado:\n");
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CARGO', 'ID_REGCAR', 'NM_CARGO', '', '', '', '', '', '';"));
    printf("\nDigite: ");
    scanf("%1i", &novo->cargo);


    printf("\nSelecione o codigo da funcao desejada:\n");
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FUNCAO', 'ID_REGFNC', 'NM_FUNCAO', '', '', '', '', '', '';"));
    printf("\nDigite: ");
    scanf("%1i", &novo->funcao);
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
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FILIAL', 'ID_REGFIL', 'NM_FILIAL', '', '', '', '', '', '';"));
    printf("\nDigite: ");
    scanf(" %1i", &novo->filial);
    getchar();

    printf("\nSelecione o codigo do departamento desejado:\n");
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_DEPTO', 'ID_REGDPT', 'NM_DEPTO', '', '', '', '', '', '';"));
    printf("\nDigite: ");
    scanf(" %1i", &novo->depto);
    getchar();

    printf("\nInforme o Salario Inicial do Funcionario: ");
    scanf(" %f", &novo->salario);
    getchar();

    snprintf(cmd, sizeof(cmd), "INSERT INTO TB_DIM_FUNCIO VALUES('%s', UPPER('%s'), '%s', DEFAULT, %i, %i, %i, %i, %i, %i, DEFAULT, DEFAULT, NULL, NULL) INSERT INTO TB_FAT_SALFUN VALUES('%s', %f, %i, DEFAULT, DEFAULT, NULL, NULL);",
                novo->cpf, novo->nome, novo->admissao, novo->cargo, novo->funcao, novo->dependente, novo->filial, novo->depto, usrid, novo->cpf, novo->salario, usrid
            );
    
    errosPrint(inserir(cmd));
    liberarServico();
    free(novo);
    printf("\nFuncionario cadastrado com sucesso!");
}

void mostrarFuncionario()
{
    char cpf[11], cmd[255];
    printf("\nInforme o CPF do Funcionario: ");
    scanf(" %11s", cpf);
    snprintf(cmd, sizeof(cmd), "SELECT NOME,[Dt Admis],[Sit Folha],Cargo,Salario FROM MOD.VW_FUNCIONARIOS WHERE CPF_PURO = '%s'", cpf);
    errosPrint(iniciarServico());
    errosPrint(leitura(dbc, cmd));
    liberarServico();
    printf("\n");
}

void atualizaCadastroFuncionario()
{
    char cpf[12], cmd[255];
    int opcao_select;
    errosPrint(iniciarServico());
    printf("\nInforme o CPF do Funcionario: ");
    scanf(" %11s", &cpf);

    do
    {
        system("cls");
        Funcionario* alterado = (Funcionario*)malloc(sizeof(Funcionario));
        escolhaAlteracaoFuncionario();
        printf("\nDigite a sua opcao: ");
        scanf(" %i", &opcao_select);
        switch (opcao_select)
        {
            case 1:
                printf("\nSelecione o codigo do cargo desejado:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CARGO', 'ID_REGCAR', 'NM_CARGO', '', '', '', '', '', '';"));
                printf("\nDigite: ");

                scanf("%1i", &alterado->cargo);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_CARGO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->cargo, usrid, cpf);
                
                errosPrint(inserir(cmd));
                free(alterado);
                getchar();
                getchar();
                system("cls");                    
                break;
            case 2:
                printf("\nSelecione o codigo do departamento desejado:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_DEPTO', 'ID_REGDPT', 'NM_DEPTO', '', '', '', '', '', '';"));
                printf("\nDigite: ");
                
                scanf(" %1i", &alterado->depto);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_DEPRTO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->depto, usrid, cpf);
                
                errosPrint(inserir(cmd));
                free(alterado);
                getchar();
                getchar();
                system("cls");  
                break;
            case 3:
                printf("\nSelecione o codigo da filial desejada:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FILIAL', 'ID_REGFIL', 'NM_FILIAL', '', '', '', '', '', '';"));
                printf("\nDigite: ");

                scanf(" %1i", &alterado->filial);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_FILIAL = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->filial, usrid, cpf);
                errosPrint(inserir(cmd));

                free(alterado);
                getchar();
                getchar();
                system("cls");  
                break;
            case 4:
                printf("\nSelecione o codigo da situacao da folha desejada:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_SITFOL', 'ID_REGSFL', 'DS_SITFOL', '', '', '', '', '', '';"));
                printf("\nDigite: ");

                scanf(" %1i", &alterado->situacao);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_SITFOL = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->situacao, usrid, cpf);
                errosPrint(inserir(cmd));

                free(alterado);
                getchar();
                getchar();
                system("cls");  
                break;
            case 5:
                printf("\nSelecione o codigo da funcao desejada:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FUNCAO', 'ID_REGFNC', 'NM_FUNCAO', '', '', '', '', '', '';"));
                printf("\nDigite: ");

                scanf(" %1i", &alterado->funcao);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_FUNCAO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->funcao, usrid, cpf);
                errosPrint(inserir(cmd));

                free(alterado);
                getchar();
                getchar();
                system("cls");  
                break;
            case 6:
                printf("\nO funcionario tem dependente? (S/N)\n");
                printf("\nDigite: ");
                while (1)
                {
                    char escolha = getchar();
                    if (escolha == '\n') {
                        continue;
                    }

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
                errosPrint(inserir(cmd));
                getchar();
                getchar();
                free(alterado);
                break;
            case 7:
                free(alterado);
                break;
            
            default:
                printf(msg_erro_entrada);
                getchar();
                system("cls");
                break;
        }
    } while (opcao_select != 7);
    liberarServico();
}

void deletaCadastroFuncionario()
{
    char cpf[11], cmd[255];
    errosPrint(iniciarServico());
    printf("\nInforme o CPF do Funcionario: ");
    scanf(" %11s", cpf);

    snprintf(cmd, sizeof(cmd), "EXEC atualizarValorRegistro 'TB_DIM_FUNCIO', '0', 'CP_FUNCIO', '%s'; EXEC atualizarValorRegistro 'TB_FAT_SALFUN', '0', 'CE_FUNCIO', '%s';", cpf, cpf);
    errosPrint(inserir(cmd));
    liberarServico();
}

void mostrarTodosFuncionarios()
{
    errosPrint(iniciarServico());
    errosPrint(leitura(dbc, "SELECT NOME,CPF,[Dt Admis],[Sit Folha],Cargo,Salario FROM MOD.VW_FUNCIONARIOS;"));
    liberarServico();
}

void alterarSalario()
{
    errosPrint(iniciarServico());
    char cmd[350], cpf[12];
    float novo_salario, acrescimo, percentual;
    int escolha_alteracao, cargo, funcao;

    do
    {
        system("cls");
        escolhaAlteracaoSalarial();
        printf("\nDigite sua opcao: ");
        scanf(" %i", &escolha_alteracao);
        switch (escolha_alteracao)
        {
            case 1:
                printf("\nInforme o CPF do funcionario: ");
                scanf("%11s", &cpf);
                
                printf("\nSelecione o codigo do cargo desejado:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CARGO', 'ID_REGCAR', 'NM_CARGO', '', '', '', '', '', '';"));
                printf("\nDigite: ");
                scanf("%1i", &cargo);

                printf("\nInforme o novo salario: ");
                scanf("%f", &novo_salario);

                snprintf(cmd, sizeof(cmd), "EXEC invalidaUltMov '%s'; INSERT INTO TB_FAT_SALFUN VALUES('%s', %f, %i, DEFAULT, DEFAULT, NULL, NULL); UPDATE TB_DIM_FUNCIO SET CE_CARGO = %i, CD_USUAAL = %i, DT_REGALT = DEFAULT WHERE CP_FUNCIO = '%s';", cpf, cpf, novo_salario, usrid, cargo, usrid, cpf);

                errosPrint(inserir(cmd));
                getchar();
                getchar();
                break;
            case 2:
                printf("\nInforme o CPF do funcionario: ");
                scanf("%11s", &cpf);

                printf("\nSelecione o codigo do funcao desejada:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FUNCAO', 'ID_REGFNC', 'NM_FUNCAO', '', '', '', '', '', '';"));
                printf("\nDigite: ");
                scanf("%1i", &funcao);

                printf("\nInforme o novo salario: ");
                scanf("%f", &novo_salario);
                
                snprintf(cmd, sizeof(cmd), "EXEC invalidaUltMov '%s'; INSERT INTO TB_FAT_SALFUN VALUES('%s', %f, %i, DEFAULT, DEFAULT, NULL, NULL); UPDATE TB_DIM_FUNCIO SET CE_FUNCAO = %i, CD_USUAAL = %i, DT_REGALT = DEFAULT WHERE CP_FUNCIO = '%s';", cpf, cpf, novo_salario, usrid, cargo, usrid, cpf);

                errosPrint(inserir(cmd));
                getchar();
                getchar();
                break;
            case 3:
                printf("\nInforme o CPF do funcionario: ");
                scanf("%11s", &cpf);

                printf("Informe o acrescimo a ser adicionado ao salario do funcionario: ");
                scanf("%f", &acrescimo);

                snprintf(cmd, sizeof(cmd), "EXEC invalidaUltMov %s;INSERT INTO TB_FAT_SALFUN (CE_FUNCIO, VL_SALARI, CE_USUACR, DT_REGALT) SELECT TOP 1 CE_FUNCIO, VL_SALARI + %f, %i, NULL FROM TB_FAT_SALFUN WHERE CE_FUNCIO = %s ORDER BY ID_REGSAL DESC;", cpf, acrescimo, usrid, cpf);

                errosPrint(inserir(cmd));
                getchar();
                getchar();
                break;
            case 4:
                printf("\nInforme o CPF do funcionario: ");
                scanf("%11s", &cpf);

                printf("Informe o percentual do acrescimo ao salario do funcionario: ");
                scanf("%f", &percentual);

                percentual = 1 + (percentual/100);

                snprintf(cmd, sizeof(cmd), "EXEC invalidaUltMov %s;INSERT INTO TB_FAT_SALFUN (CE_FUNCIO, VL_SALARI, CE_USUACR, DT_REGALT) SELECT TOP 1 CE_FUNCIO, VL_SALARI * %f, %i, NULL FROM TB_FAT_SALFUN WHERE CE_FUNCIO = %s ORDER BY ID_REGSAL DESC;", cpf, percentual, usrid, cpf);

                errosPrint(inserir(cmd));
                getchar();
                getchar();
                break;
            case 5:
                printf("\nInforme o percentual de reajuste da folha: ");
                scanf("%f", &percentual);

                percentual = 1 + (percentual/100);

                snprintf(cmd, sizeof(cmd), "EXEC alteracaoFolha %f, %i", percentual, usrid);

                errosPrint(inserir(cmd));
                getchar();
                getchar();
                break;
            case 6:
                system("cls");
                break;
            default:
                printf(msg_erro_entrada);
                getchar();
                break;
        }

    } while (escolha_alteracao!=6);
    
    
    liberarServico();
}

void relatorioFuncionariosCsv()
{
    errosPrint(iniciarServico());
    int rel = exportarRelatorio(dbc, "relatorio_funcionarios.csv", "SELECT * FROM MOD.VW_FUNCIONARIOS_DETALHADO;");
    (rel==1) ? printf("\nRelatorio exportado!") : errosPrint(rel);
    liberarServico();
    getchar();
}

void cadastroCargo()
{
    errosPrint(iniciarServico());
    char cmd[255], cargo[30], cbo[7];
    int tam_buffer = 256;
    printf("\nDigite o Titulo do Cargo: ");
    getchar();
    char* buffer = (char*)malloc(tam_buffer);
    if (buffer==NULL)
    {
        errosPrint(ERRO_ALOCACAO_MEMORIA);
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

        strcpy(cargo, buffer);
        free(buffer);
    }

    printf("\nDigite o CBO do Cargo: ");
    scanf(" %6s", &cbo);

    snprintf(cmd, sizeof(cmd), "INSERT INTO TB_DIM_CARGO VALUES(UPPER('%s'), '%s', %i, DEFAULT, DEFAULT, NULL, NULL);", cargo, cbo, usrid);
    errosPrint(inserir(cmd));
    liberarServico();
    getchar();
    getchar();
}

void cadastroCliente()
{
    printf("\nDigite o nome do cliete: ");
    errosPrint(iniciarServico());
    char cmd[255],cnpj[15], nome_cliente[30];
    int tam_buffer = 256;
    getchar();

    char* buffer = (char*)malloc(tam_buffer);
    if (buffer==NULL)
    {
        errosPrint(ERRO_ALOCACAO_MEMORIA);
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

        strcpy(nome_cliente, buffer);
        free(buffer);
    }

    
    printf("\nDigite o CNPJ do Cliente: ");
    scanf(" %14s", &cnpj);

    snprintf(cmd, sizeof(cmd), "INSERT INTO TB_DIM_CLIENT VALUES('%s', UPPER('%s'), %i, DEFAULT, DEFAULT, NULL, NULL);", cnpj, nome_cliente, usrid);
    errosPrint(inserir(cmd));
    liberarServico();
    getchar();
    getchar();
}

void cadastraVenda()
{
    errosPrint(iniciarServico());
    char cmd[255];
    Venda* nova = (Venda *)malloc(sizeof(Venda));

    printf("\nDigite o CNPJ do Cliente: ");
    scanf("%14s", nova->cnpj);

    printf("\nSelecione o codigo do produto desejado:\n");
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_PRODUT', 'ID_REGPRD', 'NM_PRODUT', '', '', '', '', '', '';"));
    printf("\nDigite: ");
    scanf("%i", &nova->prod);
    getchar();

    printf("\nDigite o Valor da venda: ");
    scanf("%f", &nova->valor_venda);
    getchar();

    printf("\nDigite a Quantidade da Venda: ");
    scanf("%d", &nova->qtde_venda);
    getchar();

    snprintf(cmd, sizeof(cmd), "INSERT INTO TB_FAT_VENDA VALUES('%s', %i, %f, %i, DEFAULT, DEFAULT, NULL, NULL, %i);", nova->cnpj, nova->prod, nova->valor_venda, usrid, nova->qtde_venda);
    errosPrint(inserir(cmd));
    liberarServico();
    free(nova);
    getchar();
}

void deletaCadastroCliente()
{
    char cnpj[15], cmd[255];
    errosPrint(iniciarServico());
    printf("\nInforme o CNPJ do Cliente: ");
    scanf(" %14s", cnpj);

    snprintf(cmd, sizeof(cmd), "EXEC atualizarValorRegistro 'TB_DIM_CLIENT', '0', 'CP_CLIENT', '%s';",cnpj);
    errosPrint(inserir(cmd));
    liberarServico();
}

void mostrarTodosClientes()
{
    errosPrint(iniciarServico());
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CLIENT', 'CP_CLIENT', 'NM_CLIENT', '', '', '', '', '', '';"));
    liberarServico();
}

void mostrarTodasVendas()
{
    int opcao;
    errosPrint(iniciarServico());
    printf("\nSelecione o tipo de relatorio: ");
    printf("\n1 - Vendas individuais");
    printf("\n2 - Vendas por Mes");
    printf("\nDigite: ");
    scanf(" %i", &opcao);

    switch (opcao)
    {
    case 1:
        errosPrint(leitura(dbc, "SELECT * FROM MOD.VW_VENDAS;"));
        getchar();
        getchar();
        break;
    case 2:
        errosPrint(leitura(dbc, "SELECT * FROM MOD.VW_VENDAS_MES ORDER BY [Data Venda] DESC;"));
        getchar();
        getchar();
    default:
        printf(msg_erro_entrada);
        getchar();
        break;
    }
    liberarServico();
}

void relatorioVendasCsv()
{
    errosPrint(iniciarServico());
    int rel = exportarRelatorio(dbc, "relatorio_vendas.csv", "SELECT * FROM MOD.VW_VENDAS;");
    (rel==1) ? printf("\nRelatorio exportado!") : errosPrint(rel);
    liberarServico();
    getchar();
}

#endif //CASES_H