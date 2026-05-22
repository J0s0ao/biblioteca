#include <stdio.h>
#include <stdlib.h>
#include "livro.h"

struct livro
{
    int codigo;
    char titulo[50];
    char autor[50];
    char ano_publicacao[19];
    int quantidade_disponivel;
    int quantidade_emprestada;
};

struct lista_livro {
    Livro* livro;
    struct lista_livro* proximo;
};

ListaLivro* criar_livro(ListaLivro* lista_livro, int codigo, char* titulo, char* autor, char* ano_publicacao, int quantidade_disponivel, int quantidade_emprestada) {
    ListaLivro* novo_livro = (ListaLivro*)malloc(sizeof(ListaLivro));
    if (novo_livro == NULL) {
        printf("Erro ao alocar memória, não foi possível criar o livro.\n");
        return NULL;
    }
    novo_livro->livro = (Livro*)malloc(sizeof(Livro));
    if (novo_livro->livro == NULL) {
        printf("Erro ao alocar memória, não foi possível criar o livro.\n");
        free(novo_livro);
        return NULL;
    }
    novo_livro->livro->codigo = codigo;
    novo_livro->livro->quantidade_disponivel = quantidade_disponivel;
    novo_livro->livro->quantidade_emprestada = quantidade_emprestada;

    novo_livro->proximo = lista_livro;  
    return novo_livro;
}

//Remover livro
ListaLivro* remover_livro(ListaLivro* lista, int codigo) {
   
    ListaLivro* anterior = NULL;
    ListaLivro* atual = lista;

    while( atual != NULL && lista->livro->codigo != codigo) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        return lista;
    }

    if (anterior == NULL) {
        lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    
    free(atual->livro);
    free(atual);
    return lista;
}

//Editar livro
ListaLivro* editar_livro(ListaLivro* lista, int codigo, char* titulo, char* autor, char* ano_publicacao, int quantidade_disponivel, int quantidade_emprestada) {
    ListaLivro* atual = lista;

    while (atual != NULL) {
        if (atual->livro->codigo == codigo) {
            atual->livro->quantidade_disponivel = quantidade_disponivel;
            atual->livro->quantidade_emprestada = quantidade_emprestada;
            return lista;
        }
    }
    return lista;
}

//Listar livros
ListaLivro* listar_livros(ListaLivro* lista) {
    ListaLivro* atual = lista;
    while (atual != NULL) {
        printf("Código: %d\n", atual->livro->codigo);
        printf("Título: %s\n", atual->livro->titulo);
        printf("Autor: %s\n", atual->livro->autor);
        printf("Ano de Publicação: %s\n", atual->livro->ano_publicacao);
        printf("Quantidade Disponível: %d\n", atual->livro->quantidade_disponivel);
        printf("Quantidade Emprestada: %d\n", atual->livro->quantidade_emprestada);
        printf("-----------------------------\n");
        atual = atual->proximo;
    }
    return lista;
}



//int codigo, nome, autor, ano_publicacao, quantidade_disponivel, quantidade_emprestada


// Livro
// - codigo
// - titulo
// - autor
// - ano_publicacao
// - quantidade_disponivel
// - quantidade_emprestada


