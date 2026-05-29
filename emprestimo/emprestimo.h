#ifndef EMRPESTIMO_H
#define EMPRESTIMO_H

typedef struct emprestimo Emprestimo;
typedef struct lista_emprestimo ListaEmprestimo;
ListaEmprestimo* criar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo, int codigo_livro, int codigo_usuario, Data data);
void libera_lista_emprestimo(ListaEmprestimo *lista_emprestimo);
void finalizar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo);
Emprestimo *buscar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo);
Emprestimo **buscar_emprestimos_usuario(ListaEmprestimo *lista_emprestimo, int codigo_usuario, int apenas_pendentes);
#endif