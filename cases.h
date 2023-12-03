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

// Função para cadastro do funcionário. Utiliza conexão com banco de dados para mostrar dados em tela e inserir dados no banco.
void criaCadastroFuncionario()
{
    Funcionario* novo = (Funcionario*)malloc(sizeof(Funcionario)); // Aloca-se memoria para uma nova instancia de funcionario
    char cmd[350]; // Variavel que armazena consulta no banco
    int dia[3], mes[3], ano[5]; // Variaveis utilizadas para a data de admissão do funcionario

    errosPrint(iniciarServico()); // Passa-se a função errosprint para resgatar os codigos de erro da função iniciarServiço e imprimir em tela. Inicia a conexão ao banco de dados
    printf("\nDigite o CPF do Funcionario (Apenas os digitos): ");       
    scanf(" %11s", novo->cpf);
    getchar();

    // Laço de repetição para poder inserir espaço no nome do funcionário.
    while(1)
    {
        printf("\nDigite o Nome do Funcionario: ");
        int tam_buffer = 256;
        char* buffer = (char*)malloc(tam_buffer); // Variavel temporaria para armazenamento de caracteres
        if (buffer==NULL)
        {
            errosPrint(ERRO_ALOCACAO_MEMORIA);
            sleep(1);
            system("cls");
        }

        if(fgets(buffer, tam_buffer, stdin) == NULL) // Caso a entrada for invalida ou vazia.
        {
            free(buffer);
            printf(msg_erro_entrada);
            getchar();
        }
        else
        {
            if ((strlen(buffer) > 0) && (buffer[strlen (buffer) - 1] == '\n')) // Tira os caracteres de nova linha e atribui caractere fim de linha.
            {
                buffer[strlen(buffer) - 1] = '\0';
            }

            strcpy(novo->nome, buffer); // copia o nome do funcionario, já tratado, para campo da struct que armazena o nome.
            free(buffer); // Libera memoria do buffer
            break;
        }
    }

    printf("\nDigite o Dia da admissao: ");
    scanf("%2s", &dia);
    printf("\nDigite o Mes da admissao: ");
    scanf("%2s", &mes);
    printf("\nDigite o Ano da admissao: ");
    scanf("%4s", &ano);

    // Interpola o dados digitados para o campo admissao da struct.
    snprintf(novo->admissao, sizeof(novo->admissao), "%02s%02s%02s", ano, mes, dia);

    // Trechos abaixo vão procurar os campos possíveis das tabelas em banco e depois solicitar a escolha ao usuário

    // Mostra os cargos disponiveis para seleção
    printf("\nSelecione o codigo do cargo desejado:\n");
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CARGO', 'ID_REGCAR', 'NM_CARGO', '', '', '', '', '', '';"));
    printf("\nDigite: ");
    scanf("%i", &novo->cargo);
    getchar();

    // Mostra as funções disponiveis para seleção
    printf("\nSelecione o codigo da funcao desejada:\n");
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FUNCAO', 'ID_REGFNC', 'NM_FUNCAO', '', '', '', '', '', '';"));
    printf("\nDigite: ");
    scanf("%i", &novo->funcao);
    getchar();

    printf("\nO funcionario tem dependente? (S/N)\n");
    printf("\nDigite: ");

    // Laço de repetição que valida escolha do usuario, se tiver dependentes continua perguntando quantos, caso contrario define dependentes como zero.
    while (1)
    {
        char escolha = getchar();
        if (escolha == '\n')
            continue;

        escolha = tolower(escolha);
        if (escolha == 's' || escolha == 'n')
        {
            if (escolha == 's')
            {
                printf("\nQuantos dependentes?\nDigite: ");
                scanf(" %i", novo->dependente);
                (novo->dependente < 0) ? novo->dependente = 0 : novo->dependente;         

                break;
            }
            else
            {
                novo->dependente = 0;
                break;
            }
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
    scanf(" %i", &novo->filial);
    getchar();

    printf("\nSelecione o codigo do departamento desejado:\n");
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_DEPTO', 'ID_REGDPT', 'NM_DEPTO', '', '', '', '', '', '';"));
    printf("\nDigite: ");
    scanf(" %i", &novo->depto);
    getchar();

    printf("\nInforme o Salario Inicial do Funcionario: ");
    scanf(" %f", &novo->salario);
    getchar();

    // Interpolação dos dados inseridos pelo usuario na struct novo funcionario dentro da string de comando e executa.
    snprintf(cmd, sizeof(cmd), "EXEC criaRegistroFuncionario '%s', '%s', '%s', %i, %i, %i, %i, %i, %f, %i;",
                novo->cpf, novo->nome, novo->admissao, novo->cargo, novo->funcao, novo->dependente, novo->filial, novo->depto, novo->salario, usrid
            );
    
    errosPrint(inserir(cmd)); // Executa comando, se acontecer erro imprime erro.
    liberarServico(); // Libera memoria de conexão e corta conexão
    free(novo); // Libera memoria struct
    printf("\nFuncionario cadastrado com sucesso!");
}

// Função mostra funcionario após usuario inserir cpf
void mostrarFuncionario()
{
    char cpf[12], cmd[255];
    printf("\nInforme o CPF do Funcionario: ");
    scanf(" %11s", cpf);
    // Interpolação de dados com cpf
    snprintf(cmd, sizeof(cmd), "SELECT NOME,[Dt Admis],[Sit Folha],Cargo,Salario FROM MOD.VW_FUNCIONARIOS WHERE CPF_PURO = '%s'", cpf);
    // Inicia Serviço
    errosPrint(iniciarServico());
    // Executa consulta
    errosPrint(leitura(dbc, cmd));
    // Libera conexão e memoria da conexão
    liberarServico();
    printf("\n");
}

// Função para atualização de cadastro de funcionarios após escolha do usuario. Mostra informações em tela e insere no banco
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
        Funcionario* alterado = (Funcionario*)malloc(sizeof(Funcionario)); // Aloca espaço em memoria HEAP para instancia de struct funcionario
        escolhaAlteracaoFuncionario();
        printf("\nDigite a sua opcao: ");
        scanf(" %i", &opcao_select);

        // Abaixo menu de opções para cada alteração de funcionario
        switch (opcao_select)
        {
            case 1:
                printf("\nSelecione o codigo do cargo desejado:\n");
                // Mostra opções disponiveis, caso contrario mostra codigo do erro
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CARGO', 'ID_REGCAR', 'NM_CARGO', '', '', '', '', '', '';"));
                printf("\nDigite: ");

                // Executa alteração do valor no banco após seleção do usuário.
                scanf("%i", &alterado->cargo);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_CARGO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->cargo, usrid, cpf);
                
                // Executa consulta, caso contrario mostra codigo do erro
                errosPrint(inserir(cmd));
                free(alterado); // Libera memoria
                getchar();
                getchar();
                system("cls");                    
                break;
            case 2:
                printf("\nSelecione o codigo do departamento desejado:\n");
                // Mostra opções disponiveis, caso contrario mostra codigo do erro
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_DEPTO', 'ID_REGDPT', 'NM_DEPTO', '', '', '', '', '', '';"));
                printf("\nDigite: ");
                
                // Executa alteração do valor no banco após seleção do usuário.
                scanf(" %i", &alterado->depto);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_DEPRTO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->depto, usrid, cpf);
                
                // Executa consulta, caso contrario mostra codigo do erro
                errosPrint(inserir(cmd));
                free(alterado); // Libera memoria
                getchar();
                getchar();
                system("cls");  
                break;
            case 3:
                printf("\nSelecione o codigo da filial desejada:\n");
                // Mostra opções disponiveis, caso contrario mostra codigo do erro
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FILIAL', 'ID_REGFIL', 'NM_FILIAL', '', '', '', '', '', '';"));
                printf("\nDigite: ");

                // Executa alteração do valor no banco após seleção do usuário.
                scanf(" %i", &alterado->filial);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_FILIAL = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->filial, usrid, cpf);
                // Executa consulta, caso contrario mostra codigo do erro
                errosPrint(inserir(cmd));

                free(alterado); // Libera memoria
                getchar();
                getchar();
                system("cls");  
                break;
            case 4:
                printf("\nSelecione o codigo da situacao da folha desejada:\n");
                // Mostra opções disponiveis, caso contrario mostra codigo do erro
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_SITFOL', 'ID_REGSFL', 'DS_SITFOL', '', '', '', '', '', '';"));
                printf("\nDigite: ");
                // Executa alteração do valor no banco após seleção do usuário.
                scanf(" %i", &alterado->situacao);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_SITFOL = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->situacao, usrid, cpf);
                // Executa consulta, caso contrario mostra codigo do erro]
                errosPrint(inserir(cmd));

                free(alterado); // Libera memoria
                getchar();
                getchar();
                system("cls");  
                break;
            case 5:
                printf("\nSelecione o codigo da funcao desejada:\n");
                // Mostra opções disponiveis, caso contrario mostra codigo do erro
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FUNCAO', 'ID_REGFNC', 'NM_FUNCAO', '', '', '', '', '', '';"));
                printf("\nDigite: ");
                // Executa alteração do valor no banco após seleção do usuário.
                scanf(" %i", &alterado->funcao);
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET CE_FUNCAO = %i, DT_REGALT = DEFAULT, CD_USUAAL = '%i' WHERE CP_FUNCIO = '%s';", alterado->funcao, usrid, cpf);
                // Executa consulta, caso contrario mostra codigo do erro
                errosPrint(inserir(cmd));

                free(alterado); // Libera memoria
                getchar();
                getchar();
                system("cls");  
                break;
            case 6:
                printf("\nO funcionario tem dependente? (S/N)\n");
                printf("\nDigite: ");

                // Laço de repetição que valida escolha do usuario, se tiver dependentes continua perguntando quantos, caso contrario define dependentes como zero.
                while (1)
                {
                    char escolha = getchar();
                    if (escolha == '\n')
                        continue;

                    escolha = tolower(escolha);
                    if (escolha == 's' || escolha == 'n')
                    {
                        if (escolha == 's')
                        {
                            printf("\nQuantos dependentes?\nDigite: ");
                            scanf(" %i", alterado->dependente);
                            (alterado->dependente < 0) ? alterado->dependente = 0 : alterado->dependente; // Validacao se for digitado inteiros negativos
                            break;
                        }
                        else
                        {
                            alterado->dependente = 0;
                            break;
                        }
                    }
                    else
                    {
                        printf("%s", msg_erro_entrada);
                        getchar();
                    }
                }

                // Executa alteração do valor no banco após seleção do usuário.
                snprintf(cmd, sizeof(cmd), "UPDATE TB_DIM_FUNCIO SET VL_DEPEND = %i, DT_REGALT = DEFAULT, CD_USUAAL = %i WHERE CP_FUNCIO = '%s';", alterado->dependente, usrid, cpf);
                // Executa consulta, caso contrario mostra codigo do erro
                errosPrint(inserir(cmd));
                getchar();
                getchar();
                free(alterado); // Libera memoria
                break;
            case 7:
                free(alterado); // Libera memoria
                break;
            
            default:
                printf(msg_erro_entrada);
                getchar();
                system("cls");
                break;
        }
    } while (opcao_select != 7);
    liberarServico(); // Libera memoria de conexão e encerra conexão
}

// Função para invalidação do cadastro do funcionário sem deletá-lo do banco.
void deletaCadastroFuncionario()
{
    char cpf[11], cmd[255];
    // Inicia Serviço
    errosPrint(iniciarServico());
    printf("\nInforme o CPF do Funcionario: ");
    scanf(" %11s", cpf);
    
    // Executa alteração do valor no banco após seleção do usuário.
    snprintf(cmd, sizeof(cmd), "EXEC atualizarValorRegistro 'TB_DIM_FUNCIO', '0', 'CP_FUNCIO', '%s'; EXEC atualizarValorRegistro 'TB_FAT_SALFUN', '0', 'CE_FUNCIO', '%s';", cpf, cpf);
    // Executa consulta, caso contrario mostra codigo do erro
    errosPrint(inserir(cmd));
    liberarServico(); // Libera memoria da conexão e corta conexão
}

// Função de consulta de funcionarios, mostra todos os funcionarios com registro valido.
void mostrarTodosFuncionarios()
{
    errosPrint(iniciarServico()); // inicia serviço
    // Imprime valores em tela
    errosPrint(leitura(dbc, "SELECT NOME,CPF,[Dt Admis],[Sit Folha],Cargo,Salario FROM MOD.VW_FUNCIONARIOS;"));
    liberarServico(); // Libera memoria de conexão e corta conexão 
}

// Função para atualização do registro de acordo com escolha do usuario.
void alterarSalario()
{
    errosPrint(iniciarServico()); // Inicia serviço, caso contrário mostra codigo do erro.
    char cmd[350], cpf[12];
    float novo_salario, acrescimo, percentual;
    int escolha_alteracao, cargo, funcao;

    // Laço de repetição para escolha do usuário.
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
                // Mostra opções disponiveis, caso contrario mostra codigo do erro
                printf("\nSelecione o codigo do cargo desejado:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CARGO', 'ID_REGCAR', 'NM_CARGO', '', '', '', '', '', '';"));
                printf("\nDigite: ");
                scanf("%i", &cargo);
                getchar();

                printf("\nInforme o novo salario: ");
                scanf("%f", &novo_salario);
                // Executa Invalidação da ultima movimentação salarial e executa alteração do registro de cargo na tabela funcionarios
                snprintf(cmd, sizeof(cmd), "EXEC invalidaUltMov '%s'; EXEC movPromocaoCargo '%s', %f, %i, %i;", cpf, cpf, novo_salario, cargo, usrid);
                // Executa consulta, caso contrario mostra codigo do erro
                errosPrint(inserir(cmd));
                getchar();
                getchar();
                break;
            case 2:
                printf("\nInforme o CPF do funcionario: ");
                scanf("%11s", &cpf);
                // Mostra opções disponiveis, caso contrario mostra codigo do erro
                printf("\nSelecione o codigo do funcao desejada:\n");
                errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_FUNCAO', 'ID_REGFNC', 'NM_FUNCAO', '', '', '', '', '', '';"));
                printf("\nDigite: ");
                scanf("%i", &funcao);
                getchar();
                
                printf("\nInforme o novo salario: ");
                scanf("%f", &novo_salario);
                // Executa Invalidação da ultima movimentação salarial e executa alteração do registro de cargo na tabela funcionarios
                snprintf(cmd, sizeof(cmd), "EXEC invalidaUltMov '%s';EXEC movPromocaoFuncao '%s', %f, %i, %i;", cpf, cpf, novo_salario, funcao, usrid);
                // Executa consulta, caso contrario mostra codigo do erro
                errosPrint(inserir(cmd));
                getchar();
                getchar();
                break;
            case 3:
                printf("\nInforme o CPF do funcionario: ");
                scanf("%11s", &cpf);

                printf("Informe o acrescimo a ser adicionado ao salario do funcionario: ");
                scanf("%f", &acrescimo);
                // Executa Invalidação da ultima movimentação salarial e executa alteração do registro de cargo na tabela funcionarios
                snprintf(cmd, sizeof(cmd), "EXEC invalidaUltMov '%s'; EXEC movAcrescimoSalario '%s', %f, %i;", cpf, cpf, acrescimo, usrid);
                // Executa consulta, caso contrario mostra codigo do erro
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
                // Executa Invalidação da ultima movimentação salarial e executa alteração do registro de cargo na tabela funcionarios
                snprintf(cmd, sizeof(cmd), "EXEC invalidaUltMov '%s';EXEC movAumentoPercSalario '%s', %f, %i;", cpf, cpf, percentual, usrid);
                // Executa consulta, caso contrario mostra codigo do erro
                errosPrint(inserir(cmd));
                getchar();
                getchar();
                break;
            case 5:
                printf("\nInforme o percentual de reajuste da folha: ");
                scanf("%f", &percentual);

                percentual = 1 + (percentual/100);
                // Atualiza toda a folha de pagamento co reajuste solicitado
                snprintf(cmd, sizeof(cmd), "EXEC alteracaoFolha %f, %i", percentual, usrid);
                // Executa consulta, caso contrario mostra codigo do erro
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
    
    
    liberarServico(); // Libera memoria da conexão e corta conexão com servidor
}

// Função para exportar retorno do servidor em csv
void relatorioFuncionariosCsv()
{
    errosPrint(iniciarServico()); // Inicia conexão com servidor
    // Executa e armazena retorno da função em variavel
    int rel = exportarRelatorio(dbc, "relatorio_funcionarios.csv", "SELECT * FROM MOD.VW_FUNCIONARIOS_DETALHADO;");
    // Validação de informação caso retorno não for 1, retorna o código do erro
    (rel==1) ? printf("\nRelatorio exportado!") : errosPrint(rel);
    liberarServico(); // Libera memoria da conexão e corta conexão com servidor
    getchar();
}

void cadastroCargo()
{
    errosPrint(iniciarServico());
    char cmd[255], cargo[30], cbo[7];
    int tam_buffer = 256;
    printf("\nDigite o Titulo do Cargo: ");
    getchar();
    char* buffer = (char*)malloc(tam_buffer); // Aloca-se memoria para interpretar entrada de usuario
    
    // Trecho abaixo recebe entrada do usuario com espaços e termina e depois copia dentro da váriavel cargo.
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

    snprintf(cmd, sizeof(cmd), "EXEC criaRegistroCargo '%s', '%s', %i;", cargo, cbo, usrid);
    // Executa consulta, caso contrario mostra codigo do erro
    errosPrint(inserir(cmd));
    liberarServico();
    getchar();
    getchar();
}

// Função para cadastro do cliente. Solicita os dados e atualiza no banco
void cadastroCliente()
{
    printf("\nDigite o nome do cliete: ");
    errosPrint(iniciarServico());
    char cmd[255],cnpj[15], nome_cliente[30];
    int tam_buffer = 256;
    getchar();

    // Trecho abaixo recebe entrada do usuario com espaços e termina e depois copia dentro da váriavel nome_cliente.
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
    snprintf(cmd, sizeof(cmd), "EXEC criaRegistroCliente '%s', '%s', %i", cnpj, nome_cliente, usrid);
    // Executa consulta, caso contrario mostra codigo do erro
    errosPrint(inserir(cmd));
    liberarServico();
    getchar();
    getchar();
}

// Função para cadastro da venda, mostra informação em tela para escolha do usuario e executa comando de atualização do banco
void cadastraVenda()
{
    errosPrint(iniciarServico());
    char cmd[255];
    Venda* nova = (Venda *)malloc(sizeof(Venda)); // Aloca-se espaço em memória para inicializar struct de vendas

    printf("\nDigite o CNPJ do Cliente desejado:\n");
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CLIENT', 'CP_CLIENT', 'NM_CLIENT', '', '', '', '', '', '';"));
    printf("\nDigite: ");
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

    snprintf(cmd, sizeof(cmd), "EXEC criaRegistroVenda '%s', %i, %f, %i, %i", nova->cnpj, nova->prod, nova->valor_venda, nova->qtde_venda, usrid);
    // Executa consulta, caso contrario mostra codigo do erro
    errosPrint(inserir(cmd)); 
    liberarServico();
    free(nova);
    getchar();
}

// Função para invalidação do cadastro do cliente. Atualiza registro no banco.
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
// Função para retornar todos os clientes. Faz consulta no banco.
void mostrarTodosClientes()
{
    errosPrint(iniciarServico());
    errosPrint(leitura(dbc, "EXEC selectNomeAlias 'TB_DIM_CLIENT', 'CP_CLIENT', 'NM_CLIENT', '', '', '', '', '', '';"));
    liberarServico();
}

// Função que solicita o usuario escolher relatorio e depois imprime este na tela
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

// Função para exportar retorno do servidor em csv
void relatorioVendasCsv()
{
    errosPrint(iniciarServico());
    // Executa e armazena retorno da função em variavel
    int rel = exportarRelatorio(dbc, "relatorio_vendas.csv", "SELECT * FROM MOD.VW_VENDAS;");
    // Validação de informação caso retorno não for 1, retorna o código do erro
    (rel==1) ? printf("\nRelatorio exportado!") : errosPrint(rel);
    liberarServico();
    getchar();
}

#endif //CASES_H