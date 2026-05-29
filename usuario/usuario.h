#ifndef USUARIO_H
#define USUARIO_H

typedef struct usuario Usuario;
typedef struct lista_usuario ListaUsuario;
ListaUsuario* criar_usuario(ListaUsuario* lista_usuario, int codigo, char* nome, char* senha);
void liberar_lista_usuario(ListaUsuario* lista_usuario);
ListaUsuario* remover_usuario(ListaUsuario* lista_usuario, int codigo);
void editar_usuario(ListaUsuario* lista_usuario, int codigo, char* novo_nome, char* nova_senha);
Usuario* buscar_usuario(ListaUsuario* lista_usuario, int codigo);
Usuario** buscar_usuario_por_nome(ListaUsuario* lista_usuario, char* pesquisa);
ListaUsuario* adicionar_usuario(ListaUsuario* lista_usuario, int codigo, char* nome, char* senha);

// Helpers de interface (usados pelo CLI em main.c)
void listar_usuarios(ListaUsuario* lista_usuario);            // lista todos os usuarios
void listar_usuario(ListaUsuario* lista_usuario, int codigo); // lista um usuario por codigo
void imprimir_usuario(const Usuario* usuario);                // exibe os dados de um usuario




#endif