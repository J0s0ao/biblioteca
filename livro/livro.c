#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    // Copia campos de texto com segurança
    if (titulo) {
        strncpy(novo_livro->livro->titulo, titulo, sizeof(novo_livro->livro->titulo) - 1);
        novo_livro->livro->titulo[sizeof(novo_livro->livro->titulo) - 1] = '\0';
    } else {
        novo_livro->livro->titulo[0] = '\0';
    }
    if (autor) {
        strncpy(novo_livro->livro->autor, autor, sizeof(novo_livro->livro->autor) - 1);
        novo_livro->livro->autor[sizeof(novo_livro->livro->autor) - 1] = '\0';
    } else {
        novo_livro->livro->autor[0] = '\0';
    }
    if (ano_publicacao) {
        strncpy(novo_livro->livro->ano_publicacao, ano_publicacao, sizeof(novo_livro->livro->ano_publicacao) - 1);
        novo_livro->livro->ano_publicacao[sizeof(novo_livro->livro->ano_publicacao) - 1] = '\0';
    } else {
        novo_livro->livro->ano_publicacao[0] = '\0';
    }

    novo_livro->livro->quantidade_disponivel = quantidade_disponivel;
    novo_livro->livro->quantidade_emprestada = quantidade_emprestada;

    novo_livro->proximo = lista_livro;  
    return novo_livro;
}

//Remover livro
ListaLivro* remover_livro(ListaLivro* lista, int codigo) {
   
    ListaLivro* anterior = NULL;
    ListaLivro* atual = lista;

    while (atual != NULL && (atual->livro == NULL || atual->livro->codigo != codigo)) {
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
    
    if (atual->livro != NULL) {
        free(atual->livro);
    }
    free(atual);
    return lista;
}

//Editar livro
int editar_livro(ListaLivro* lista_livro, int codigo, char* novo_titulo, char* novo_autor, char* novo_ano_publicacao, int nova_quantidade_disponivel) {
    ListaLivro* atual = lista_livro;

    while (atual != NULL) {
        if (atual->livro != NULL && atual->livro->codigo == codigo) {
            if (nova_quantidade_disponivel < atual->livro->quantidade_emprestada) {
                return 1; // invalido
            }

            if (novo_titulo) {
                strncpy(atual->livro->titulo, novo_titulo, sizeof(atual->livro->titulo) - 1);
                atual->livro->titulo[sizeof(atual->livro->titulo) - 1] = '\0';
            }
            if (novo_autor) {
                strncpy(atual->livro->autor, novo_autor, sizeof(atual->livro->autor) - 1);
                atual->livro->autor[sizeof(atual->livro->autor) - 1] = '\0';
            }
            if (novo_ano_publicacao) {
                strncpy(atual->livro->ano_publicacao, novo_ano_publicacao, sizeof(atual->livro->ano_publicacao) - 1);
                atual->livro->ano_publicacao[sizeof(atual->livro->ano_publicacao) - 1] = '\0';
            }

            atual->livro->quantidade_disponivel = nova_quantidade_disponivel;
            return 0; // deu certo
        }
        atual = atual->proximo;
    }
    return 1; // nao encontrou o livro
}

//Buscar livro por código
Livro* buscar_livro(ListaLivro* lista_livro, int codigo) {
    ListaLivro* atual = lista_livro;
    while (atual != NULL) {
        if (atual->livro != NULL && atual->livro->codigo == codigo) {
            return atual->livro;
        }
        atual = atual->proximo;
    }
    return NULL;
}

//Buscar livro por nome
Livro** buscar_livro_por_nome(ListaLivro* lista_livro, char* pesquisa) {
    int count = 0;
    ListaLivro* atual = lista_livro;
    while (atual != NULL) {
        if (atual->livro != NULL && strstr(atual->livro->titulo, pesquisa) != NULL) {
            count++;
        }
        atual = atual->proximo;
    }
    if (count == 0) {
        return NULL;
    }
    Livro** resultados = (Livro**)malloc((count + 1) * sizeof(Livro*));
    if (resultados == NULL) {
        printf("Erro ao alocar memória para resultados de busca.\n");
        return NULL;
    }
    int index = 0;
    atual = lista_livro;
    while (atual != NULL) {
        if (atual->livro != NULL && strstr(atual->livro->titulo, pesquisa) != NULL) {
            resultados[index++] = atual->livro;
        }
        atual = atual->proximo;
    }
    resultados[index] = NULL; // terminador para facilitar iteração
    return resultados;
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

//Exibe os dados de um unico livro
void imprimir_livro(const Livro* livro) {
    if (livro == NULL) {
        printf("Livro inexistente.\n");
        return;
    }
    printf("Código: %d\n", livro->codigo);
    printf("Título: %s\n", livro->titulo);
    printf("Autor: %s\n", livro->autor);
    printf("Ano de Publicação: %s\n", livro->ano_publicacao);
    printf("Quantidade Disponível: %d\n", livro->quantidade_disponivel);
    printf("Quantidade Emprestada: %d\n", livro->quantidade_emprestada);
}

//Quantos exemplares estao disponiveis para um livro
int livro_get_quantidade_disponivel(const Livro* livro) {
    return livro ? livro->quantidade_disponivel : 0;
}

//Empresta um exemplar: disponivel--, emprestada++. Retorna 0 em sucesso, 1 se indisponivel/nao encontrado.
int livro_emprestar(ListaLivro* lista_livro, int codigo) {
    Livro* l = buscar_livro(lista_livro, codigo);
    if (l == NULL || l->quantidade_disponivel <= 0) {
        return 1;
    }
    l->quantidade_disponivel--;
    l->quantidade_emprestada++;
    return 0;
}

//Devolve um exemplar: disponivel++, emprestada--. Retorna 0 em sucesso, 1 se nada emprestado/nao encontrado.
int livro_devolver(ListaLivro* lista_livro, int codigo) {
    Livro* l = buscar_livro(lista_livro, codigo);
    if (l == NULL || l->quantidade_emprestada <= 0) {
        return 1;
    }
    l->quantidade_disponivel++;
    l->quantidade_emprestada--;
    return 0;
}

//Liberar momória da lista de livros
void liberar_lista_livro(ListaLivro* lista_livro) {
    ListaLivro* atual = lista_livro;
    while (atual != NULL) {
        ListaLivro* temp = atual;
        atual = atual->proximo;
        if (temp->livro != NULL) {
            free(temp->livro);
        }
        free(temp);
    }
}

//int codigo, nome, autor, ano_publicacao, quantidade_disponivel, quantidade_emprestada


// Livro
// - codigo
// - titulo
// - autor
// - ano_publicacao
// - quantidade_disponivel
// - quantidade_emprestada


