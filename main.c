#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

static void exibir_menu_usuario(void)
{
    printf("\n=== Biblioteca ===\n");
    printf("1 - Ver catalogo\n");
    printf("2 - Painel de Administrador\n");
    printf("0 - Sair\n");
}

static void exibir_menu_administrador(void)
{
    printf("\n=== Menu do Administrador ===\n");
    printf("1 - Gerenciar livros\n");
    printf("2 - Gerenciar usuarios\n");
    printf("3 - Gerenciar emprestimos\n");
    printf("0 - Sair\n");
}

static int autenticar_admin(void)
{
    const char *senha_correta = "1234";
    char senha_informada[50];

    printf("Digite a senha do administrador: ");
    if (scanf("%49s", senha_informada) != 1)
    {
        return 0;
    }

    return strcmp(senha_informada, senha_correta) == 0;
}

int main(void)
{
    
    int em_menu_administrador = 0;

    ListaLivro *lista_livros = NULL;
    ListaUsuario *lista_usuarios = NULL;
    ListaEmprestimo *lista_emprestimos = NULL;

    int proximo_codigo_livro = 1;
    int proximo_codigo_usuario = 1;
    int proximo_codigo_emprestimo = 1;

    int opcao = -1;

    printf("Bem-vindo à biblioteca!\n");

    while (opcao != 0)
    {
        exibir_menu_usuario();
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1)
        {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }

        switch (opcao)
        {
        case 1:
            printf("Aqui deve ser inserida a exibicao do catalogo de livros.\n");
            break;
        case 2:
            if (autenticar_admin())
            {
                printf("Acesso liberado ao painel de administrador.\n");
                if (em_menu_administrador)
                {
                    exibir_menu_administrador();
                }
                else
                {
                    exibir_menu_usuario();
                }
            }
            else
            {
                printf("Senha incorreta. Acesso negado.\n");
            }
            break;
        case 0:
            printf("Saindo...\n");
            if (em_menu_administrador)
            {
                switch (opcao)
                {
                case 1:
                    printf("Aqui deve ser inserida a gerencia de livros.\n");
                    break;
                case 2:
                    printf("Aqui deve ser inserida a gerencia de usuarios.\n");
                    break;
                case 3:
                    printf("Aqui deve ser inserida a gerencia de emprestimos.\n");
                    break;
                case 0:
                    em_menu_administrador = 0;
                    opcao = -1;
                    printf("Saindo do painel do administrador...\n");
                    break;
                default:
                    printf("Opcao invalida. Tente novamente.\n");
                    break;
                }
            }
            else
            {
                switch (opcao)
                {
                case 1:
                    printf("Aqui deve ser inserida a exibicao do catalogo de livros.\n");
                    break;
                case 2:
                    if (autenticar_admin())
                    {
                        printf("Acesso liberado ao painel de administrador.\n");
                        em_menu_administrador = 1;
                        opcao = -1;
                    }
                    else
                    {
                        printf("Senha incorreta. Acesso negado.\n");
                    }
                    break;
                case 0:
                    printf("Saindo...\n");
                    break;
                default:
                    printf("Opcao invalida. Tente novamente.\n");
                    break;
                }
            }
        }
    }
}