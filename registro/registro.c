#include <stdio.h>
#include <stdlib.h>
#include "registro.h"

struct no_historico {
    TipoOperacao tipo;
    int codigo_livro;
    struct no_historico* abaixo;
};

NoHistorico* empilhar(NoHistorico* topo, TipoOperacao tipo, int codigo_livro) {
    NoHistorico* novo = malloc(sizeof(NoHistorico));
    novo->tipo = tipo;
    novo->codigo_livro = codigo_livro;
    novo->abaixo = topo;
    return novo;
}

NoHistorico* desempilhar(NoHistorico* topo) {
    if (topo == NULL) return NULL;
    NoHistorico* novo_topo = topo->abaixo;
    free(topo);
    return novo_topo;
}

void imprimir_historico(NoHistorico* topo) {
    const char* nomes[] = {
        "Livro cadastrado",
        "Livro removido",
        "Retirada",
        "Devolucao"
    };

    NoHistorico* atual = topo;
    while (atual != NULL) {
        printf("[%s] codigo: %d\n", nomes[atual->tipo], atual->codigo_livro);
        atual = atual->abaixo;
    }
}

void liberar_historico(NoHistorico* topo) {
    while (topo != NULL) {
        topo = desempilhar(topo);
    }
}