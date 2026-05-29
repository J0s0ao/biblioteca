#ifndef PILHA_H
#define PILHA_H

typedef enum {
    OP_LIVRO_CADASTRADO,
    OP_LIVRO_REMOVIDO,
    OP_RETIRADA,
    OP_DEVOLUCAO
} TipoOperacao;

typedef struct no_historico NoHistorico;

NoHistorico* empilhar(NoHistorico* topo, TipoOperacao tipo, int codigo_livro);
NoHistorico* desempilhar(NoHistorico* topo);
void imprimir_historico(NoHistorico* topo);
void liberar_historico(NoHistorico* topo);

#endif
