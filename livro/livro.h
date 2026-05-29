#ifndef LIVRO_H
#define LIVRO_H

typedef struct livro Livro;
typedef struct lista_livro ListaLivro;

ListaLivro* criar_livro(ListaLivro* lista_livro, int codigo, char* titulo, char* autor, char* ano_publicacao, int quantidade_disponivel, int quantidade_emprestada);//feito

void liberar_lista_livro(ListaLivro* lista_livro); // Usa função free() //feito

int editar_livro(ListaLivro* lista_livro, int codigo, char* novo_titulo, char* novo_autor, char* novo_ano_publicacao, int nova_quantidade_disponivel);//feito

// Observação para editar livro: nova_quantidade_disponível não pode ser menor do que a quantidade já emprestada. Se o argumento não respeitar essa regra, retornar 1. Se não, editar o livro e retornar 0. 
ListaLivro* remover_livro(ListaLivro* lista_livro, int codigo);//feito

Livro* buscar_livro(ListaLivro* lista_livro, int codigo);//feito

Livro** buscar_livro_por_nome(ListaLivro* lista_livro, char* pesquisa);//feito

ListaLivro* listar_livros(ListaLivro* lista_livro); //feito

// Helpers de interface (usados pelo CLI em main.c)
void imprimir_livro(const Livro* livro);                 // exibe os dados de um unico livro
int livro_get_quantidade_disponivel(const Livro* livro); // quantos exemplares estao disponiveis
int livro_emprestar(ListaLivro* lista_livro, int codigo); // disponivel--, emprestada++. 0=ok, 1=indisponivel
int livro_devolver(ListaLivro* lista_livro, int codigo);  // disponivel++, emprestada--. 0=ok, 1=erro

#endif