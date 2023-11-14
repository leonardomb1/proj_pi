#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>

#define NOME_EMPRESA "Safe Tec Ltd."
#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>

#define NOME_EMPRESA "Safe Tech Ltd."
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define PAD_SQL 20

void cabecaMenuInicio()
{
    printf("Sistema de recursos empresariais - %s \n", NOME_EMPRESA);
    printf("Escolha uma opcao:\n");
    printf("1 - Menu de cadastro de funcionarios\n");
    printf("2 - Menu de cadastro de produtos\n");    
    printf("3 - Sair do Sistema\n");
    printf("4 - Entrar com outro usuario\n");
}

void cabecaMenuFuncionario()
{
    printf("Sistema de administracao de funcionarios\n");
    printf("Escolha uma opcao:\n");
    printf("1 - Criar um novo cadastro\n");
    printf("2 - Ler um cadastro pelo cpf\n");
    printf("3 - Atualizar um cadastro\n");
    printf("4 - Deletar um cadastro pelo cpf\n");
    printf("5 - Listar todos os cadastros\n");
    printf("6 - Sair do modulo\n");
}

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

void printCell(const char* content, int maxWidth) {
    int contentLen = strlen(content);
    int padding = maxWidth - contentLen;
    int leftPadding = padding / 2;
    int rightPadding = padding - leftPadding;

    printf("| %*s%s%*s |", leftPadding, "", content, rightPadding, "");
}

char msg_erro_entrada[30] = "Entrada Invalida!\n";

char* nome_empresa_banner[] = {
        "",
        " ********             *********       **                          **   **        **    ",
        " **//////             /**/           /**                         /**  /**       /**    ",
        "/**         ******   *******  *****  ******  *****   *****        /** ******     /**   ",
        "/********* //////** ///**/   **///**///**/  **///** **///**       /**///**/   ******   ",
        "////////**  *******   /**   /*******  /**  /*******/**  //        /**  /**   **///**   ",
        "       /** **////**   /**   /**////   /**  /**//// /**   **       /**  /**  /**  /** **",
        " ******** //********  /**   //******  //** //******//*****        ***  //** //******/**",
        "////////   ////////   //     //////    //   //////  /////        ///    //   ////// // "
    };


#endif //INTERFACE_H
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define PAD_SQL 20

void cabecaMenuInicio()
{
    printf("Sistema de recursos empresariais - %s \n", NOME_EMPRESA);
    printf("Escolha uma opcao:\n");
    printf("1 - Menu de cadastro de funcionarios\n");
    printf("2 - Menu de cadastro de produtos\n");    
    printf("3 - Sair do Sistema\n");
    printf("4 - Entrar com outro usuario\n");
}

void cabecaMenuFuncionario()
{
    printf("Sistema de administracao de funcionarios\n");
    printf("Escolha uma opcao:\n");
    printf("1 - Criar um novo cadastro\n");
    printf("2 - Ler um cadastro pelo cpf\n");
    printf("3 - Atualizar um cadastro\n");
    printf("4 - Deletar um cadastro pelo cpf\n");
    printf("5 - Listar todos os cadastros\n");
    printf("6 - Sair do modulo\n");
}

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

void printCell(const char* content, int maxWidth) {
    int contentLen = strlen(content);
    int padding = maxWidth - contentLen;
    int leftPadding = padding / 2;
    int rightPadding = padding - leftPadding;

    printf("| %*s%s%*s |", leftPadding, "", content, rightPadding, "");
}

char msg_erro_entrada[30] = "Entrada Invalida!\n";

char* nome_empresa_banner[] = {
        "",
        " ********             *********       **                          **   **        **    ",
        " **//////             /**/           /**                         /**  /**       /**    ",
        "/**         ******   *******  *****  ******  *****   *****        /** ******     /**   ",
        "/********* //////** ///**/   **///**///**/  **///** **///**       /**///**/   ******   ",
        "////////**  *******   /**   /*******  /**  /*******/**  //        /**  /**   **///**   ",
        "       /** **////**   /**   /**////   /**  /**//// /**   **       /**  /**  /**  /** **",
        " ******** //********  /**   //******  //** //******//*****        ***  //** //******/**",
        "////////   ////////   //     //////    //   //////  /////        ///    //   ////// // "
    };


#endif //INTERFACE_H
