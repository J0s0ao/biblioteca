#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"
#include "utils/data.h"
#include "registro/registro.h"

/* ============================================================
 * Helpers de entrada/saida
 * ============================================================ */

// Descarta o restante da linha que ficou no buffer de entrada
static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Le um inteiro de forma robusta (reexibe o prompt enquanto a entrada for invalida)
static int ler_int(const char *prompt) {
    int valor;
    int resultado;
    printf("%s", prompt);
    while ((resultado = scanf("%d", &valor)) != 1) {
        if (resultado == EOF) {
            return 0; // entrada encerrada (EOF): trata como "sair/voltar"
        }
        limpar_buffer();
        printf("Entrada invalida. %s", prompt);
    }
    limpar_buffer(); // remove o '\n' para nao atrapalhar leituras de texto seguintes
    return valor;
}

// Le uma linha de texto (aceita espacos) sem deixar o '\n' no destino
static void ler_texto(const char *prompt, char *destino, int tamanho) {
    printf("%s", prompt);
    if (fgets(destino, tamanho, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    } else {
        destino[0] = '\0';
    }
}

// Pergunta de confirmacao (1 = sim, qualquer outro = nao)
static int confirmar(const char *pergunta) {
    return ler_int(pergunta) == 1;
}

// Duplica uma string em memoria propria (criar_usuario/editar_usuario guardam o ponteiro, nao copiam)
static char *duplicar_string(const char *origem) {
    size_t n = strlen(origem) + 1;
    char *novo = (char *)malloc(n);
    if (novo != NULL) {
        memcpy(novo, origem, n);
    }
    return novo;
}

// Registra uma operacao no historico (pilha de registros)
static void registrar(PilhaRegistro *pilha, const char *tipo, const char *operacao, const char *descricao) {
    if (pilha != NULL) {
        empilhar_registro(pilha, tipo, operacao, descricao);
    }
}

/* ============================================================
 * Menus (apenas exibicao)
 * ============================================================ */

static void exibir_menu_principal(void) {
    printf("\n=== Biblioteca ===\n");
    printf("1 - Catalogo\n");
    printf("2 - Administracao\n");
    printf("0 - Sair\n");
}

static void exibir_menu_catalogo(void) {
    printf("\n=== Catalogo ===\n");
    printf("1 - Ver todos os livros\n");
    printf("2 - Buscar livro por nome\n");
    printf("0 - Voltar\n");
}

static void exibir_menu_administracao(void) {
    printf("\n=== Administracao ===\n");
    printf("1 - Gerenciar livros\n");
    printf("2 - Gerenciar usuarios\n");
    printf("3 - Gerenciar emprestimos\n");
    printf("4 - Ver historico\n");
    printf("0 - Voltar\n");
}

static void exibir_menu_livros(void) {
    printf("\n=== Gerenciar Livros ===\n");
    printf("1 - Cadastrar livro\n");
    printf("2 - Editar livro\n");
    printf("3 - Remover livro\n");
    printf("4 - Ver livros\n");
    printf("5 - Buscar livro por nome\n");
    printf("0 - Voltar\n");
}

static void exibir_menu_usuarios(void) {
    printf("\n=== Gerenciar Usuarios ===\n");
    printf("1 - Cadastrar usuario\n");
    printf("2 - Editar usuario\n");
    printf("3 - Remover usuario\n");
    printf("4 - Ver usuarios\n");
    printf("5 - Buscar usuario por nome\n");
    printf("0 - Voltar\n");
}

static void exibir_menu_emprestimos(void) {
    printf("\n=== Gerenciar Emprestimos ===\n");
    printf("1 - Realizar emprestimo\n");
    printf("2 - Devolver livro\n");
    printf("3 - Ver emprestimos\n");
    printf("0 - Voltar\n");
}

// Autenticacao simples do administrador
static int autenticar_admin(void) {
    const char *senha_correta = "1234";
    char senha_informada[50];
    ler_texto("Digite a senha do administrador: ", senha_informada, sizeof(senha_informada));
    return strcmp(senha_informada, senha_correta) == 0;
}

/* ============================================================
 * 1) Catalogo
 * ============================================================ */

// Busca um livro por nome e imprime os resultados
static void buscar_livro_nome_e_exibir(ListaLivro *lista_livros) {
    char pesquisa[50];
    ler_texto("Digite o nome (ou parte) do livro: ", pesquisa, sizeof(pesquisa));

    Livro **resultados = buscar_livro_por_nome(lista_livros, pesquisa);
    if (resultados == NULL) {
        printf("Nenhum livro encontrado.\n");
        return;
    }
    printf("\n--- Resultados da busca ---\n");
    for (int i = 0; resultados[i] != NULL; i++) {
        imprimir_livro(resultados[i]);
        printf("-----------------------------\n");
    }
    free(resultados);
}

static void menu_catalogo(ListaLivro *lista_livros) {
    int opcao = -1;
    while (opcao != 0) {
        exibir_menu_catalogo();
        opcao = ler_int("Escolha uma opcao: ");
        switch (opcao) {
        case 1: // Ver todos os livros
            printf("\n--- Catalogo de livros ---\n");
            if (lista_livros == NULL) {
                printf("Nenhum livro cadastrado.\n");
            } else {
                listar_livros(lista_livros);
            }
            break;
        case 2: // Buscar livro por nome
            buscar_livro_nome_e_exibir(lista_livros);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }
}

/* ============================================================
 * 2.1) Gerenciar Livros
 * ============================================================ */

static void cadastrar_livro(ListaLivro **lista_livros, int *proximo_codigo, PilhaRegistro *pilha) {
    char titulo[50], autor[50], ano[19];
    ler_texto("Titulo: ", titulo, sizeof(titulo));
    ler_texto("Autor: ", autor, sizeof(autor));
    ler_texto("Ano de publicacao: ", ano, sizeof(ano));
    int quantidade = ler_int("Quantidade de exemplares: ");

    ListaLivro *nova = criar_livro(*lista_livros, *proximo_codigo, titulo, autor, ano, quantidade, 0);
    if (nova == NULL) {
        printf("Falha ao cadastrar o livro.\n");
        return;
    }
    *lista_livros = nova;
    printf("Livro cadastrado com codigo %d.\n", *proximo_codigo);
    registrar(pilha, "LIVRO", "CADASTRO", titulo);
    (*proximo_codigo)++;
}

static void editar_livro_menu(ListaLivro *lista_livros, PilhaRegistro *pilha) {
    int codigo = ler_int("Codigo do livro a editar: ");
    Livro *livro = buscar_livro(lista_livros, codigo);
    if (livro == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    }
    printf("\n--- Livro encontrado ---\n");
    imprimir_livro(livro);
    if (!confirmar("E este o livro? (1=Sim / 0=Nao): ")) {
        printf("Operacao cancelada.\n");
        return;
    }

    char titulo[50], autor[50], ano[19];
    ler_texto("Novo titulo: ", titulo, sizeof(titulo));
    ler_texto("Novo autor: ", autor, sizeof(autor));
    ler_texto("Novo ano de publicacao: ", ano, sizeof(ano));
    int quantidade = ler_int("Nova quantidade disponivel: ");

    if (editar_livro(lista_livros, codigo, titulo, autor, ano, quantidade) == 0) {
        printf("Livro editado com sucesso.\n");
        registrar(pilha, "LIVRO", "EDICAO", titulo);
    } else {
        printf("Edicao invalida (a quantidade disponivel nao pode ser menor que a emprestada).\n");
    }
}

static void remover_livro_menu(ListaLivro **lista_livros, PilhaRegistro *pilha) {
    int codigo = ler_int("Codigo do livro a remover: ");
    Livro *livro = buscar_livro(*lista_livros, codigo);
    if (livro == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    }
    printf("\n--- Livro encontrado ---\n");
    imprimir_livro(livro);
    if (!confirmar("Confirma a remocao? (1=Sim / 0=Nao): ")) {
        printf("Operacao cancelada.\n");
        return;
    }
    *lista_livros = remover_livro(*lista_livros, codigo);
    printf("Livro removido.\n");
    registrar(pilha, "LIVRO", "REMOCAO", "livro removido");
}

static void gerenciar_livros(ListaLivro **lista_livros, int *proximo_codigo, PilhaRegistro *pilha) {
    int opcao = -1;
    while (opcao != 0) {
        exibir_menu_livros();
        opcao = ler_int("Escolha uma opcao: ");
        switch (opcao) {
        case 1:
            cadastrar_livro(lista_livros, proximo_codigo, pilha);
            break;
        case 2:
            editar_livro_menu(*lista_livros, pilha);
            break;
        case 3:
            remover_livro_menu(lista_livros, pilha);
            break;
        case 4:
            printf("\n--- Livros ---\n");
            if (*lista_livros == NULL) {
                printf("Nenhum livro cadastrado.\n");
            } else {
                listar_livros(*lista_livros);
            }
            break;
        case 5:
            buscar_livro_nome_e_exibir(*lista_livros);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }
}

/* ============================================================
 * 2.2) Gerenciar Usuarios
 * ============================================================ */

static void cadastrar_usuario(ListaUsuario **lista_usuarios, int *proximo_codigo, PilhaRegistro *pilha) {
    char nome[100], senha[100];
    ler_texto("Nome: ", nome, sizeof(nome));
    ler_texto("Senha: ", senha, sizeof(senha));

    char *nome_copia = duplicar_string(nome);
    char *senha_copia = duplicar_string(senha);
    if (nome_copia == NULL || senha_copia == NULL) {
        printf("Falha ao alocar memoria.\n");
        free(nome_copia);
        free(senha_copia);
        return;
    }

    ListaUsuario *nova = criar_usuario(*lista_usuarios, *proximo_codigo, nome_copia, senha_copia);
    if (nova == NULL) {
        printf("Falha ao cadastrar o usuario.\n");
        free(nome_copia);
        free(senha_copia);
        return;
    }
    *lista_usuarios = nova;
    printf("Usuario cadastrado com codigo %d.\n", *proximo_codigo);
    registrar(pilha, "USUARIO", "CADASTRO", nome);
    (*proximo_codigo)++;
}

static void editar_usuario_menu(ListaUsuario *lista_usuarios, PilhaRegistro *pilha) {
    int codigo = ler_int("Codigo do usuario a editar: ");
    Usuario *usuario = buscar_usuario(lista_usuarios, codigo);
    if (usuario == NULL) {
        printf("Usuario nao encontrado.\n");
        return;
    }
    printf("\n--- Usuario encontrado ---\n");
    imprimir_usuario(usuario);
    if (!confirmar("E este o usuario? (1=Sim / 0=Nao): ")) {
        printf("Operacao cancelada.\n");
        return;
    }

    char nome[100], senha[100];
    ler_texto("Novo nome: ", nome, sizeof(nome));
    ler_texto("Nova senha: ", senha, sizeof(senha));

    char *nome_copia = duplicar_string(nome);
    char *senha_copia = duplicar_string(senha);
    if (nome_copia == NULL || senha_copia == NULL) {
        printf("Falha ao alocar memoria.\n");
        free(nome_copia);
        free(senha_copia);
        return;
    }
    editar_usuario(lista_usuarios, codigo, nome_copia, senha_copia);
    printf("Usuario editado com sucesso.\n");
    registrar(pilha, "USUARIO", "EDICAO", nome);
}

static void remover_usuario_menu(ListaUsuario **lista_usuarios, ListaEmprestimo *lista_emprestimos, PilhaRegistro *pilha) {
    int codigo = ler_int("Codigo do usuario a remover: ");
    Usuario *usuario = buscar_usuario(*lista_usuarios, codigo);
    if (usuario == NULL) {
        printf("Usuario nao encontrado.\n");
        return;
    }

    // Proibe a remocao se o usuario tiver emprestimos pendentes
    Emprestimo **pendentes = buscar_emprestimos_usuario(lista_emprestimos, codigo, 1);
    if (pendentes != NULL) {
        printf("Usuario possui emprestimos ativos. Remocao proibida.\n");
        free(pendentes);
        return;
    }

    printf("\n--- Usuario encontrado ---\n");
    imprimir_usuario(usuario);
    if (!confirmar("Confirma a remocao? (1=Sim / 0=Nao): ")) {
        printf("Operacao cancelada.\n");
        return;
    }
    *lista_usuarios = remover_usuario(*lista_usuarios, codigo);
    printf("Usuario removido.\n");
    registrar(pilha, "USUARIO", "REMOCAO", "usuario removido");
}

static void buscar_usuario_nome_e_exibir(ListaUsuario *lista_usuarios) {
    char pesquisa[100];
    ler_texto("Digite o nome (ou parte) do usuario: ", pesquisa, sizeof(pesquisa));

    Usuario **resultados = buscar_usuario_por_nome(lista_usuarios, pesquisa);
    if (resultados == NULL) {
        printf("Nenhum usuario encontrado.\n");
        return;
    }
    printf("\n--- Resultados da busca ---\n");
    for (int i = 0; resultados[i] != NULL; i++) {
        imprimir_usuario(resultados[i]);
    }
    free(resultados);
}

static void gerenciar_usuarios(ListaUsuario **lista_usuarios, ListaEmprestimo *lista_emprestimos,
                               int *proximo_codigo, PilhaRegistro *pilha) {
    int opcao = -1;
    while (opcao != 0) {
        exibir_menu_usuarios();
        opcao = ler_int("Escolha uma opcao: ");
        switch (opcao) {
        case 1:
            cadastrar_usuario(lista_usuarios, proximo_codigo, pilha);
            break;
        case 2:
            editar_usuario_menu(*lista_usuarios, pilha);
            break;
        case 3:
            remover_usuario_menu(lista_usuarios, lista_emprestimos, pilha);
            break;
        case 4:
            printf("\n--- Usuarios ---\n");
            if (*lista_usuarios == NULL) {
                printf("Nenhum usuario cadastrado.\n");
            } else {
                listar_usuarios(*lista_usuarios);
            }
            break;
        case 5:
            buscar_usuario_nome_e_exibir(*lista_usuarios);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }
}

/* ============================================================
 * 2.3) Gerenciar Emprestimos
 * ============================================================ */

static void realizar_emprestimo(ListaLivro *lista_livros, ListaUsuario *lista_usuarios,
                                ListaEmprestimo **lista_emprestimos, int *proximo_codigo,
                                PilhaRegistro *pilha) {
    int codigo_livro = ler_int("Codigo do livro: ");
    Livro *livro = buscar_livro(lista_livros, codigo_livro);
    if (livro == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    }
    if (livro_get_quantidade_disponivel(livro) <= 0) {
        printf("Nao ha exemplares disponiveis. Emprestimo proibido.\n");
        return;
    }
    printf("\n--- Livro ---\n");
    imprimir_livro(livro);
    if (!confirmar("Confirma o livro? (1=Sim / 0=Nao): ")) {
        printf("Operacao cancelada.\n");
        return;
    }

    int codigo_usuario = ler_int("Codigo do usuario: ");
    Usuario *usuario = buscar_usuario(lista_usuarios, codigo_usuario);
    if (usuario == NULL) {
        printf("Usuario nao encontrado.\n");
        return;
    }

    // Verifica se o usuario ja tem este livro emprestado (pendente)
    Emprestimo **pendentes = buscar_emprestimos_usuario(*lista_emprestimos, codigo_usuario, 1);
    if (pendentes != NULL) {
        for (int i = 0; pendentes[i] != NULL; i++) {
            if (emprestimo_get_codigo_livro(pendentes[i]) == codigo_livro) {
                printf("Usuario ja possui este livro emprestado. Emprestimo proibido.\n");
                free(pendentes);
                return;
            }
        }
        free(pendentes);
    }

    printf("\n--- Usuario ---\n");
    imprimir_usuario(usuario);
    if (!confirmar("Confirma o usuario? (1=Sim / 0=Nao): ")) {
        printf("Operacao cancelada.\n");
        return;
    }

    Data data;
    data.dia = ler_int("Dia: ");
    data.mes = ler_int("Mes: ");
    data.ano = ler_int("Ano: ");

    // Decrementa exemplar disponivel e cria o emprestimo
    if (livro_emprestar(lista_livros, codigo_livro) != 0) {
        printf("Nao foi possivel reservar o exemplar.\n");
        return;
    }
    ListaEmprestimo *nova = criar_emprestimo(*lista_emprestimos, *proximo_codigo, codigo_livro, codigo_usuario, data);
    if (nova == NULL) {
        printf("Falha ao registrar o emprestimo.\n");
        livro_devolver(lista_livros, codigo_livro); // desfaz a reserva
        return;
    }
    *lista_emprestimos = nova;
    printf("Emprestimo realizado com codigo %d.\n", *proximo_codigo);
    registrar(pilha, "EMPRESTIMO", "REALIZACAO", "emprestimo realizado");
    (*proximo_codigo)++;
}

static void devolver_livro(ListaLivro *lista_livros, ListaEmprestimo *lista_emprestimos, PilhaRegistro *pilha) {
    int codigo_livro = ler_int("Codigo do livro: ");
    Livro *livro = buscar_livro(lista_livros, codigo_livro);
    if (livro == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    }
    printf("\n--- Livro ---\n");
    imprimir_livro(livro);
    if (!confirmar("Confirma o livro? (1=Sim / 0=Nao): ")) {
        printf("Operacao cancelada.\n");
        return;
    }

    int codigo_usuario = ler_int("Codigo do usuario: ");

    // Localiza o emprestimo pendente deste usuario para este livro
    Emprestimo **pendentes = buscar_emprestimos_usuario(lista_emprestimos, codigo_usuario, 1);
    int codigo_emprestimo = -1;
    if (pendentes != NULL) {
        for (int i = 0; pendentes[i] != NULL; i++) {
            if (emprestimo_get_codigo_livro(pendentes[i]) == codigo_livro) {
                codigo_emprestimo = emprestimo_get_codigo(pendentes[i]);
                break;
            }
        }
        free(pendentes);
    }
    if (codigo_emprestimo == -1) {
        printf("Emprestimo pendente nao encontrado. Operacao cancelada.\n");
        return;
    }

    livro_devolver(lista_livros, codigo_livro);          // atualiza a quantidade emprestada
    finalizar_emprestimo(lista_emprestimos, codigo_emprestimo); // marca como devolvido
    printf("Livro devolvido com sucesso.\n");
    registrar(pilha, "EMPRESTIMO", "DEVOLUCAO", "livro devolvido");
}

static void gerenciar_emprestimos(ListaLivro *lista_livros, ListaUsuario *lista_usuarios,
                                  ListaEmprestimo **lista_emprestimos, int *proximo_codigo,
                                  PilhaRegistro *pilha) {
    int opcao = -1;
    while (opcao != 0) {
        exibir_menu_emprestimos();
        opcao = ler_int("Escolha uma opcao: ");
        switch (opcao) {
        case 1:
            realizar_emprestimo(lista_livros, lista_usuarios, lista_emprestimos, proximo_codigo, pilha);
            break;
        case 2:
            devolver_livro(lista_livros, *lista_emprestimos, pilha);
            break;
        case 3:
            printf("\n--- Emprestimos ---\n");
            listar_emprestimos(*lista_emprestimos);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }
}

/* ============================================================
 * 2) Administracao
 * ============================================================ */

static void menu_administracao(ListaLivro **lista_livros, ListaUsuario **lista_usuarios,
                               ListaEmprestimo **lista_emprestimos,
                               int *proximo_codigo_livro, int *proximo_codigo_usuario,
                               int *proximo_codigo_emprestimo, PilhaRegistro *pilha) {
    int opcao = -1;
    while (opcao != 0) {
        exibir_menu_administracao();
        opcao = ler_int("Escolha uma opcao: ");
        switch (opcao) {
        case 1:
            gerenciar_livros(lista_livros, proximo_codigo_livro, pilha);
            break;
        case 2:
            gerenciar_usuarios(lista_usuarios, *lista_emprestimos, proximo_codigo_usuario, pilha);
            break;
        case 3:
            gerenciar_emprestimos(*lista_livros, *lista_usuarios, lista_emprestimos,
                                  proximo_codigo_emprestimo, pilha);
            break;
        case 4:
            listar_todos_registros(pilha);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }
}

/* ============================================================
 * Programa principal
 * ============================================================ */

int main(void) {
    // Inicializacao das listas
    ListaLivro *lista_livros = NULL;
    ListaUsuario *lista_usuarios = NULL;
    ListaEmprestimo *lista_emprestimos = NULL;

    // Codigos de auto-incremento
    int proximo_codigo_livro = 1;
    int proximo_codigo_usuario = 1;
    int proximo_codigo_emprestimo = 1;

    // Pilha de historico de operacoes
    PilhaRegistro *pilha_registros = criar_pilha_registro();

    int opcao = -1;

    printf("Bem-vindo à biblioteca!\n");

    while (opcao != 0) {
        exibir_menu_principal();
        opcao = ler_int("Escolha uma opcao: ");
        switch (opcao) {
        case 1: // Catalogo
            menu_catalogo(lista_livros);
            break;
        case 2: // Administracao (exige senha)
            if (autenticar_admin()) {
                printf("Acesso liberado ao painel de administrador.\n");
                menu_administracao(&lista_livros, &lista_usuarios, &lista_emprestimos,
                                   &proximo_codigo_livro, &proximo_codigo_usuario,
                                   &proximo_codigo_emprestimo, pilha_registros);
            } else {
                printf("Senha incorreta. Acesso negado.\n");
            }
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }

    // Libera memoria
    liberar_lista_livro(lista_livros);
    liberar_lista_usuario(lista_usuarios);
    libera_lista_emprestimo(lista_emprestimos);
    destruir_pilha_registro(pilha_registros);

    return 0;
}
