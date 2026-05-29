#ifndef REGISTRO_H
#define REGISTRO_H

typedef struct NoRegistro {
    int codigo;
    char tipo[32];
    char operacao[32];
    char descricao[128];
    struct NoRegistro *anterior;
} NoRegistro;

typedef struct {
    NoRegistro *topo;
    int tamanho;
    int proximo_codigo;
} PilhaRegistro;

PilhaRegistro *criar_pilha_registro(void);
void destruir_pilha_registro(PilhaRegistro *pilha);

int empilhar_registro(PilhaRegistro *pilha,
                      const char *tipo,
                      const char *operacao,
                      const char *descricao);

NoRegistro *desempilhar_registro(PilhaRegistro *pilha);
NoRegistro *peek_registro(const PilhaRegistro *pilha);
int pilha_registro_vazia(const PilhaRegistro *pilha);

void exibir_topo_registro(const PilhaRegistro *pilha);
void listar_todos_registros(const PilhaRegistro *pilha);

#endif