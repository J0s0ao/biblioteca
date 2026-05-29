#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

static void exibir_menu(void) {
    printf("\n=== Biblioteca ===\n");
    printf("1 - Gerenciar livros\n");
    printf("2 - Gerenciar usuarios\n");
    printf("3 - Gerenciar emprestimos\n");
    printf("0 - Sair\n");
}

int main(void) {
    ListaLivro* lista_livros = NULL;
    ListaUsuario* lista_usuarios = NULL;
    ListaEmprestimo* lista_emprestimos = NULL;

    int proximo_codigo_livro = 1;
    int proximo_codigo_usuario = 1;
    int proximo_codigo_emprestimo = 1;

    int opcao = -1;

    printf("Bem-vindo à biblioteca!\n");
    printf("Listas iniciadas: livros=%p usuarios=%p emprestimos=%p\n", (void*)lista_livros, (void*)lista_usuarios, (void*)lista_emprestimos);
    printf("Codigos iniciais: livro=%d usuario=%d emprestimo=%d\n", proximo_codigo_livro, proximo_codigo_usuario, proximo_codigo_emprestimo);

    while (opcao != 0) {
        exibir_menu();
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }

        switch (opcao) {
            case 1:
                printf("[livros] Interface ainda em construcao.\n");
                break;
            case 2:
                printf("[usuarios] Interface ainda em construcao.\n");
                break;
            case 3:
                printf("[emprestimos] Interface ainda em construcao.\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}