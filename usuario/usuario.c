#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"

struct usuario {
    int codigo;
    char *nome;
    char *senha;
};

struct lista_usuario {
    Usuario* usuario;
    struct lista_usuario* proximo;
};


ListaUsuario* criar_usuario(ListaUsuario* lista_usuario, int codigo, char* nome, char* senha) {
    Usuario* novo_usuario = (Usuario*)malloc(sizeof(Usuario));
    
    if (novo_usuario == NULL) {
        printf("Erro ao alocar memória para o usuário.\n");
        return NULL;
    }

    novo_usuario->codigo = codigo;
    novo_usuario->nome = nome;
    novo_usuario->senha = senha;

    ListaUsuario* novo_lista_usuario = (ListaUsuario*)malloc(sizeof(ListaUsuario));
    
    if (novo_lista_usuario == NULL) {
        printf("Erro ao alocar memória para a lista de usuários.\n");
        free(novo_usuario);
        return NULL;
    }

    novo_lista_usuario->usuario = novo_usuario;
    novo_lista_usuario->proximo = lista_usuario;

    return novo_lista_usuario;
}

void liberar_lista_usuario(ListaUsuario* lista_usuario) {
    ListaUsuario* atual = lista_usuario;

    while (atual != NULL) {
        ListaUsuario* proximo = atual->proximo;
        free(atual->usuario);
        free(atual);
        atual = proximo;
    }
}

void remover_usuario(ListaUsuario* lista_usuario, int codigo) {
    ListaUsuario* anterior = NULL;
    ListaUsuario* atual = lista_usuario;
  
    while (atual != NULL) {
        if (atual->usuario->codigo == codigo) {
            if (anterior == NULL) {
                lista_usuario = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual->usuario);
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

void editar_usuario(ListaUsuario* lista_usuario, int codigo, char* novo_nome, char* nova_senha) {
    ListaUsuario* atual = lista_usuario;

    while (atual != NULL) {
        if (atual->usuario->codigo == codigo) {
            atual->usuario->nome = novo_nome;
            atual->usuario->senha = nova_senha;
            return;
        }
        atual = atual->proximo;
    }
}

Usuario* buscar_usuario(ListaUsuario* lista_usuario, int codigo) {
    ListaUsuario* atual = lista_usuario;

    while (atual != NULL) {
        if (atual->usuario->codigo == codigo) {
            return atual->usuario;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void listar_usuario(ListaUsuario* lista_usuario, int codigo) {
    ListaUsuario* atual = lista_usuario;

    while (atual != NULL) {
        if (atual->usuario->codigo == codigo) {
            printf("Código: %d, Nome: %s\n", atual->usuario->codigo, atual->usuario->nome);
            return;
        }
        atual = atual->proximo;
    }
}

Usuario** buscar_usuario_por_nome(ListaUsuario* lista_usuario, char* pesquisa);
