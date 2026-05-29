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
    ListaEmprestimo* atual = lista_emprestimo;

    while (atual != NULL) {
        if (atual->emprestimo->codigo == codigo) {
            atual->emprestimo->devolvido = 1;
        }

        atual = atual->proximo;
    }
}

Emprestimo *buscar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo) {
    ListaEmprestimo* atual = lista_emprestimo;

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

//Exibe os dados de um unico emprestimo
void imprimir_emprestimo(const Emprestimo *emprestimo) {
    if (emprestimo == NULL) {
        printf("Emprestimo inexistente.\n");
        return;
    }
    printf("Codigo: %d | Livro: %d | Usuario: %d | Data: %02d/%02d/%04d | Status: %s\n",
           emprestimo->codigo, emprestimo->codigo_livro, emprestimo->codigo_usuario,
           emprestimo->data.dia, emprestimo->data.mes, emprestimo->data.ano,
           emprestimo->devolvido ? "Devolvido" : "Pendente");
}

//Exibe todos os emprestimos da lista
void listar_emprestimos(ListaEmprestimo *lista_emprestimo) {
    ListaEmprestimo *atual = lista_emprestimo;
    if (atual == NULL) {
        printf("Nenhum emprestimo registrado.\n");
        return;
    }
    while (atual != NULL) {
        imprimir_emprestimo(atual->emprestimo);
        atual = atual->proximo;
    }
}

//Getters usados pelo CLI (struct opaca fora deste arquivo)
int emprestimo_get_codigo(const Emprestimo *emprestimo) {
    return emprestimo ? emprestimo->codigo : -1;
}

int emprestimo_get_codigo_livro(const Emprestimo *emprestimo) {
    return emprestimo ? emprestimo->codigo_livro : -1;
}
