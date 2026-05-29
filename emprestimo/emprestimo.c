#include <stdio.h>
#include <stdlib.h>
#include "emprestimo.h"
#include "../utils/data.h"

struct emprestimo {
    int codigo;
    int codigo_livro;
    int codigo_usuario;
    Data data;
    int devolvido;
};

struct lista_emprestimo {
    Emprestimo* emprestimo;
    struct lista_emprestimo* proximo;
};

ListaEmprestimo* criar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo, int codigo_livro, int codigo_usuario, Data data) {
    ListaEmprestimo* novo = (ListaEmprestimo *)malloc(sizeof(ListaEmprestimo));

    if (novo == NULL) {
        printf("Erro ao alocar memória, não foi possível criar o empréstimo.\n");
        return NULL;
    }

    novo->emprestimo = (Emprestimo *)malloc(sizeof(Emprestimo));

    if (novo->emprestimo == NULL) {
        printf("Erro ao alocar memória, não foi possível criar o empréstimo.\n");
        free(novo);
        return NULL;
    }

    novo->emprestimo->codigo = codigo;
    novo->emprestimo->codigo_livro = codigo_livro;
    novo->emprestimo->codigo_usuario = codigo_usuario;
    novo->emprestimo->data = data;
    novo->emprestimo->devolvido = 0;

    novo->proximo = lista_emprestimo;

    return novo;
}

void libera_lista_emprestimo(ListaEmprestimo *lista_emprestimo) {
    ListaEmprestimo *proximo = lista_emprestimo;
    ListaEmprestimo *proximo_temporario;

    while (proximo != NULL) {
        proximo_temporario = proximo->proximo;
        free(proximo);
        proximo = proximo_temporario;
    }
}

void finalizar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo) {
    ListaEmprestimo* atual;

    while (atual != NULL) {
        if (atual->emprestimo->codigo == codigo) {
            atual->emprestimo->devolvido = 1;
        }

        atual = atual->proximo;
    }
}

Emprestimo *buscar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo) {
    ListaEmprestimo* atual;

    while (atual != NULL) {
        if (atual->emprestimo->codigo == codigo) {
            return atual->emprestimo;
        }

        atual = atual->proximo;
    }

    return NULL;
}

Emprestimo **buscar_emprestimos_usuario(ListaEmprestimo *lista_emprestimo, int codigo_usuario, int apenas_pendentes) {
    int total = 0;
    ListaEmprestimo *atual = lista_emprestimo;

    while (atual != NULL) {
        Emprestimo *e = atual->emprestimo;

        if (e->codigo_usuario == codigo_usuario) {
            if (!apenas_pendentes || e->devolvido == 0) {
                total++;
            }
        }

        atual = atual->proximo;
    }

    if (total == 0) return NULL;

    Emprestimo **resultado = (Emprestimo **)malloc((total + 1) * sizeof(Emprestimo *));

    if (resultado == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    int i = 0;
    atual = lista_emprestimo;

    while (atual != NULL) {
        Emprestimo *e = atual->emprestimo;

        if (e->codigo_usuario == codigo_usuario) {
            if (!apenas_pendentes || e->devolvido == 0) {
                resultado[i++] = e;
            }
        }

        atual = atual->proximo;
    }

    resultado[i] = NULL;

    return resultado;
}
