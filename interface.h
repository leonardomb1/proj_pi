#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <string.h>

// Define nome da empresa a ser utilizado em funções
#define NOME_EMPRESA "Safe Tech ME"


// Definição de Cores para utilização na Tela.
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define PAD_SQL 20

// Primeiro Menu após login
void cabecaMenuInicio()
{
    printf(ANSI_COLOR_GREEN "Sistema de recursos empresariais - %s \n" ANSI_COLOR_RESET, NOME_EMPRESA);
    printf("Escolha uma opcao:\n");
    printf("1 - Menu de administracao de funcionarios\n");
    printf("2 - Menu de administracao de vendas\n");    
    printf("3 - Sair do Sistema\n");
    printf("4 - Entrar com outro usuario\n");
}

// Menu de administração de funcionarios
void cabecaMenuFuncionario()
{
    printf(ANSI_COLOR_GREEN "Modulo de administracao de funcionarios\n" ANSI_COLOR_RESET);
    printf("Escolha uma opcao:\n");
    printf("1 - Criar um novo cadastro\n");
    printf("2 - Ler um cadastro pelo cpf\n");
    printf("3 - Atualizar um cadastro\n");
    printf("4 - Deletar um cadastro pelo cpf\n");
    printf("5 - Listar todos os cadastros\n");
    printf("6 - Alteracao salarial\n");
    printf("7 - Cadastro de Cargo\n");
    printf("8 - Exportar relatorio\n");
    printf("9 - Sair do modulo\n");
}

// Menu de administracao de vendas
void cabecaMenuVenda()
{
    printf(ANSI_COLOR_GREEN "Modulo de administracao de vendas\n" ANSI_COLOR_RESET);
    printf("Escolha uma opcao:\n");
    printf("1 - Cadastrar cliente\n");
    printf("2 - Cadastrar uma venda\n");
    printf("3 - Deletar um cadastro pelo CNPJ\n");
    printf("4 - Listar todos os cadastros\n");
    printf("5 - Listar todas as vendas\n");
    printf("6 - Exportar relatorio\n");
    printf("7 - Sair do modulo\n");
}

// Sub-menu de alteração de funcionarios
void escolhaAlteracaoFuncionario()
{
    printf("Escolha uma opcao:\n");
    printf("1 - Alterar Cargo\n");
    printf("2 - Alterar Departamento\n");
    printf("3 - Alterar Filial\n");
    printf("4 - Alterar Situacao na Folha\n");
    printf("5 - Alterar Funcao\n");
    printf("6 - Alterar Verificacao Dependente\n");
    printf("7 - Sair\n");
}


// Função que calcula celula da tabela de dados de retorno do servidor SQL
void imprimeCelula(const char* dados, int largura_max) {
    int tam_dados = strlen(dados); // atribui-se o tamanho dos dados do retorno do servidor em uma variavel
    int espacamento = largura_max - tam_dados; 
    int espaco_esquerda = espacamento / 2;
    int espaco_direita = espacamento - espaco_esquerda;

    // Imprime celula com espaçamento e barra vertical
    printf("| %*s%s%*s |", espaco_esquerda, "", dados, espaco_direita, "");
}

void escolhaAlteracaoSalarial()
{
    printf("Escolha uma opcao:\n");
    printf("1 - Promocao de Cargo\n");
    printf("2 - Promocao em Funcao\n");
    printf("3 - Aumento em Acrescimo\n");
    printf("4 - Aumento em Porcentual\n");
    printf("5 - Reajuste da Folha\n");
    printf("6 - Sair\n");
}

void errosPrint(int fn)
{
    (fn != 1) ? printf(ANSI_COLOR_RED "\nErro numero: %i" ANSI_COLOR_RESET, fn) : 1; // Imprime o retorno, somente nos casos de erro, da função de tipo int passada como parametro.
}

char msg_erro_entrada[30] = "Entrada Invalida!\n";

// Vetor de Strings, utilizado para imprimir nome da empresa em arte ASCII.
char* nome_empresa_banner[] = {
"",
" .d8888b.        d8888 8888888888 8888888888      88888888888 8888888888 .d8888b.  888    888", 
"d88P  Y88b      d88888 888        888                 888     888       d88P  Y88b 888    888", 
"Y88b.          d88P888 888        888                 888     888       888    888 888    888", 
" 'Y888b.      d88P 888 8888888    8888888             888     8888888   888        8888888888", 
"    'Y88b.   d88P  888 888        888                 888     888       888        888    888", 
"      '888  d88P   888 888        888                 888     888       888    888 888    888", 
"Y88b  d88P d8888888888 888        888                 888     888       Y88b  d88P 888    888", 
" 'Y8888P' d88P     888 888        8888888888          888     8888888888 'Y8888P'  888    888" 
};

// Função para centralização do texto, colocando espaçamento na direita e na esquerda.
void centralizaTextoPdAmbos(char *texto, int largura) {
    int centraliza = (largura - strlen(texto)) / 2; // Usado para calcular posição do texto, tamanho do texto subtraído da largura da tela.
    printf("%*s%s%*s", centraliza, "", texto, centraliza, "");
}

// Função para centralização do texto, colocando espaçamento apenas na esquerda.
void centralizaTextoPdEsquerda(char *texto, int largura) {
    int centraliza = (largura - strlen(texto)) / 2; // Usado para calcular posição do texto, tamanho do texto subtraído da largura da tela.
    printf("%*s%s", centraliza, "", texto);
}


#endif //INTERFACE_H
