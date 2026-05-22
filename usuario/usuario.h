#ifndef USUARIO_H
#define USUARIO_H

typedef struct usuario Usuario;
typedef struct lista_usuario ListaUsuario;
void liberar_lista_usuario(ListaUsuario* lista_usuario);
void remover_usuario(ListaUsuario* lista_usuario, int codigo);
void editar_usuario(ListaUsuario* lista_usuario, int codigo, char* novo_nome, char* nova_senha);
Usuario* buscar_usuario(ListaUsuario* lista_usuario, int codigo);
Usuario** buscar_usuario_por_nome(ListaUsuario* lista_usuario, char* pesquisa);
ListaUsuario* adicionar_usuario(ListaUsuario* lista_usuario, int codigo, char* nome, char* senha);




#endif