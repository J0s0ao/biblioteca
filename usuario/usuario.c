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

ListaUsuario* remover_usuario(ListaUsuario* lista_usuario, int codigo) {
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
            return lista_usuario;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return lista_usuario;
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

void listar_usuarios(ListaUsuario* lista_usuario) {
    ListaUsuario* atual = lista_usuario;

    while (atual != NULL) {
        printf("Código: %d, Nome: %s\n", atual->usuario->codigo, atual->usuario->nome);
        atual = atual->proximo;
    }
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

//Exibe os dados de um unico usuario
void imprimir_usuario(const Usuario* usuario) {
    if (usuario == NULL) {
        printf("Usuário inexistente.\n");
        return;
    }
    printf("Código: %d, Nome: %s\n", usuario->codigo, usuario->nome);
}

Usuario** buscar_usuario_por_nome(ListaUsuario* lista_usuario, char* pesquisa) {
    if (lista_usuario == NULL || pesquisa == NULL) {
        return NULL;
    }

    int quantidade = 0;
    Usuario** resultados = (Usuario**)malloc(sizeof(Usuario*));
    if (resultados == NULL) {
        return NULL;
    }

    ListaUsuario* atual = lista_usuario;
    while (atual != NULL) {
        if (strstr(atual->usuario->nome, pesquisa) != NULL) {
            Usuario** novo_resultados = (Usuario**)realloc(resultados, (quantidade + 2) * sizeof(Usuario*));
            if (novo_resultados == NULL) {
                free(resultados);
                return NULL;
            }
            resultados = novo_resultados;
            resultados[quantidade++] = atual->usuario;
        }
        atual = atual->proximo;
    }

    if (quantidade == 0) {
        free(resultados);
        return NULL;
    }

    resultados[quantidade] = NULL;
    return resultados;
}





    
