#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"

PilhaRegistro *criar_pilha_registro(void) {
    PilhaRegistro *pilha = (PilhaRegistro *)malloc(sizeof(PilhaRegistro));
    if (pilha == NULL) {
        fprintf(stderr, "[registro] Erro: falha ao alocar pilha.\n");
        return NULL;
    }
    pilha->topo           = NULL;
    pilha->tamanho        = 0;
    pilha->proximo_codigo = 1;
    return pilha;
}

void destruir_pilha_registro(PilhaRegistro *pilha) {
    if (pilha == NULL) return;
    while (!pilha_registro_vazia(pilha)) {
        NoRegistro *removido = desempilhar_registro(pilha);
        free(removido);
    }
    free(pilha);
}

int empilhar_registro(PilhaRegistro *pilha,
                      const char *tipo,
                      const char *operacao,
                      const char *descricao) {
    if (pilha == NULL) return 0;

    NoRegistro *novo = (NoRegistro *)malloc(sizeof(NoRegistro));
    if (novo == NULL) {
        fprintf(stderr, "[registro] Erro: falha ao alocar no.\n");
        return 0;
    }

    novo->codigo = pilha->proximo_codigo++;
    strncpy(novo->tipo,      tipo,      sizeof(novo->tipo)      - 1);
    strncpy(novo->operacao,  operacao,  sizeof(novo->operacao)  - 1);
    strncpy(novo->descricao, descricao, sizeof(novo->descricao) - 1);
    novo->tipo[sizeof(novo->tipo)           - 1] = '\0';
    novo->operacao[sizeof(novo->operacao)   - 1] = '\0';
    novo->descricao[sizeof(novo->descricao) - 1] = '\0';

    novo->anterior = pilha->topo;
    pilha->topo    = novo;
    pilha->tamanho++;
    return 1;
}

NoRegistro *desempilhar_registro(PilhaRegistro *pilha) {
    if (pilha == NULL || pilha_registro_vazia(pilha)) {
        printf("[registro] Pilha vazia, nada a desempilhar.\n");
        return NULL;
    }

    NoRegistro *removido = pilha->topo;
    pilha->topo          = removido->anterior;
    removido->anterior   = NULL;
    pilha->tamanho--;
    return removido;
}

NoRegistro *peek_registro(const PilhaRegistro *pilha) {
    if (pilha == NULL || pilha_registro_vazia(pilha)) return NULL;
    return pilha->topo;
}

int pilha_registro_vazia(const PilhaRegistro *pilha) {
    return (pilha == NULL || pilha->topo == NULL);
}

static void exibir_no_registro(const NoRegistro *no) {
    if (no == NULL) return;
    printf("  Codigo   : %d\n", no->codigo);
    printf("  Tipo     : %s\n", no->tipo);
    printf("  Operacao : %s\n", no->operacao);
    printf("  Descricao: %s\n", no->descricao);
}

void exibir_topo_registro(const PilhaRegistro *pilha) {
    printf("\n--- Registro no topo da pilha ---\n");
    if (pilha_registro_vazia(pilha)) {
        printf("  Pilha vazia.\n");
        return;
    }
    exibir_no_registro(pilha->topo);
}

void listar_todos_registros(const PilhaRegistro *pilha) {
    printf("\n--- Historico de registros (do mais recente ao mais antigo) ---\n");
    if (pilha_registro_vazia(pilha)) {
        printf("  Nenhum registro encontrado.\n");
        return;
    }

    const NoRegistro *atual = pilha->topo;
    int posicao = 1;
    while (atual != NULL) {
        printf("\n[%d]\n", posicao++);
        exibir_no_registro(atual);
        atual = atual->anterior;
    }
    printf("\nTotal de registros: %d\n", pilha->tamanho);
}