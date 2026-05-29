#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include "../utils/data.h"

typedef struct emprestimo Emprestimo;
typedef struct lista_emprestimo ListaEmprestimo;
ListaEmprestimo* criar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo, int codigo_livro, int codigo_usuario, Data data);
void libera_lista_emprestimo(ListaEmprestimo *lista_emprestimo);
void finalizar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo);
Emprestimo *buscar_emprestimo(ListaEmprestimo *lista_emprestimo, int codigo);
Emprestimo **buscar_emprestimos_usuario(ListaEmprestimo *lista_emprestimo, int codigo_usuario, int apenas_pendentes);

// Helpers de interface (usados pelo CLI em main.c)
void imprimir_emprestimo(const Emprestimo *emprestimo);          // exibe um emprestimo
void listar_emprestimos(ListaEmprestimo *lista_emprestimo);      // exibe todos os emprestimos
int emprestimo_get_codigo(const Emprestimo *emprestimo);         // codigo do emprestimo
int emprestimo_get_codigo_livro(const Emprestimo *emprestimo);   // codigo do livro emprestado

#endif