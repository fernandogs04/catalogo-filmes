#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define PRETO "\x1b[38;5;0m"
#define VERMELHO "\x1b[38;5;9m"
#define VERMELHO_ESCURO "\x1b[38;5;1m"
#define VERDE "\x1b[38;5;46m"
#define VERDE_ESCURO "\x1b[38;5;22m"
#define AMARELO "\x1b[38;5;226m"
#define AMARELO_ESCURO "\x1b[38;5;142m"
#define AZUL "\x1b[38;5;21m"
#define AZUL_ESCURO "\x1b[38;5;18m"
#define MAGENTA "\x1b[38;5;200m"
#define MAGENTA_ESCURO "\x1b[38;5;90m"
#define CIANO "\x1b[38;5;51m"
#define CIANO_ESCURO "\x1b[38;5;30m"
#define CINZA_CLARO "\x1b[38;5;250m"
#define CINZA "\x1b[38;5;245m"
#define CINZA_ESCURO "\x1b[38;5;238m"
#define BRANCO "\x1b[38;5;15m"

#define RESET "\x1B[0m"

struct FILME
{
    char titulo[255];
    int ano;
    char duracao[255];
    char diretor[255];
    char genero[255];
    bool assistiu;
    bool gostou;
};

struct NO
{
    struct FILME *filme;

    struct NO *proximo;
};

struct LISTA
{
    struct NO* topo = NULL;
    struct NO* fim = NULL;
    int quantidade = 0;
};

void limpar_tela()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void esperar_enter()
{
    printf(BRANCO "Aperte " VERDE "ENTER" BRANCO " para continuar...\n");
    printf(RESET);
    fflush(stdout);
    
    #ifdef _WIN32
        while (_kbhit()) _getch();
        while (_getch() != '\r');
    #else
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt)
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        while (getchar() != '\n');

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif
}

void adicionar_filme_a_lista(LISTA *lista, FILME *novo_filme)
{
    struct NO* no = (struct NO*) malloc(sizeof(struct NO));
    no->filme = novo_filme;

    struct NO *aux = lista->topo;
    lista->topo = no;
    lista->topo->proximo = aux;
    if (aux == NULL)
    {
        lista->fim = lista->topo;
    }

    lista->quantidade++;
}

void cadastrar_filme(struct LISTA *lista, char titulo[255], int ano, char duracao[255], char diretor[255], char genero[255], bool assistiu, bool gostou)
{
    struct FILME* novo_filme = (struct FILME*) malloc(sizeof(struct FILME));
    strcpy(novo_filme->titulo, titulo);
    novo_filme->ano = ano;
    strcpy(novo_filme->duracao, duracao);
    strcpy(novo_filme->diretor, diretor);
    strcpy(novo_filme->genero, genero);
    novo_filme->assistiu = assistiu;
    novo_filme->gostou = gostou;

    adicionar_filme_a_lista(lista, novo_filme);
}

void liberar_lista(struct LISTA *lista)
{
    struct NO *atual = lista->topo;
    struct NO *proximo;

    while(atual != NULL)
    {
        proximo = atual->proximo;
        free(atual->filme);
        free(atual);
        atual = proximo;
    }

    lista->topo = NULL;
    lista->fim = NULL;
    lista->quantidade = 0;

    free(lista);
}

LISTA* buscar_filmes(LISTA* lista_filmes)
{
    printf("\n\n");
    fflush(stdin);
    printf(BRANCO "Digite o tipo de busca (" VERDE "titulo" BRANCO ", " VERDE "ano" BRANCO "): ");
    printf(RESET);
    char tipo_busca[255];
    scanf(" %[^\n]", tipo_busca);

    LISTA* lista_resultado = (struct LISTA*) malloc(sizeof(struct LISTA));
    lista_resultado->topo = NULL;
    lista_resultado->fim = NULL;
    lista_resultado->quantidade = 0;

    NO* atual = lista_filmes->topo;

    if (strcmp(tipo_busca, "titulo") == 0)
    {
        fflush(stdin);
        printf(BRANCO "Digite o termo para buscar no titulo: ");
        printf(RESET);
        char termo[255];
        scanf(" %[^\n]", termo);

        // Deixar termo em minúsculo e tirar quebra de linha
        for (int i = 0; i < strlen(termo); i++) {
            termo[i] = tolower(termo[i]);
        }
        termo[strlen(termo) - 1] = '\0';

        while(atual != NULL)
        {
            // Deixar titulo em minúsculo
            char titulo_filme[255] = "";
            for (int i = 0; i < strlen(atual->filme->titulo); i++) {
                titulo_filme[i] = tolower(atual->filme->titulo[i]);
            }
            if (strstr(titulo_filme, termo) != NULL)
            {
                adicionar_filme_a_lista(lista_resultado, atual->filme);
            }
            atual = atual->proximo;
        }
    }
    else if (strcmp(tipo_busca, "ano") == 0)
    {
        fflush(stdin);
        printf(BRANCO "Digite o ano para buscar: ");
        printf(RESET);
        int ano_busca;
        scanf("%d", &ano_busca);

        while(atual != NULL)
        {
            if (atual->filme->ano == ano_busca)
            {
                adicionar_filme_a_lista(lista_resultado, atual->filme);
            }
            atual = atual->proximo;
        }
    }
    else
    {
        printf(VERMELHO "Tipo de busca invalido.\n\n\n");
        printf(RESET);
        return lista_resultado;
    }

    return lista_resultado;
}

FILME* escolher_filme(LISTA *lista)
{
    FILME* filme_escolhido = NULL;

    if (lista->quantidade == 1)
    {
        return lista->topo->filme;
    }
    else
    {
        printf(BRANCO "Resultados da busca:\n");
        FILME *resultados[lista->quantidade];

        NO *atual = lista->topo;
        for (int i = 0; i < lista->quantidade; i++)
        {
            resultados[i] = atual->filme;
            printf(BRANCO " %i " VERDE "%s " AZUL "(" VERDE "%i" AZUL ") - " VERDE "%s\n", i + 1, atual->filme->titulo, atual->filme->ano, atual->filme->duracao);
            printf(RESET);
            atual = atual->proximo;
        }

        fflush(stdin);
        printf(BRANCO "Escolha um filme (" VERDE "1" BRANCO ", " VERDE "2" BRANCO "): ");
        printf(RESET);
        int indice_filme_escolhido = 0;
        scanf("%i", &indice_filme_escolhido);
        indice_filme_escolhido--;

        if (indice_filme_escolhido < 0 || indice_filme_escolhido >= lista->quantidade)
        {
            printf(VERMELHO"Numero incorreto ou nao encontrado na lista.\n\n\n");
            printf(RESET);
            return NULL;
        }

        filme_escolhido = resultados[indice_filme_escolhido];
    }
    return filme_escolhido;
}

void retirar_filme_do_comeco(LISTA* lista)
{
    struct NO* aux = lista->topo;
    FILME* valor = aux->filme;
    lista->topo = lista->topo->proximo;

    if (lista->topo == NULL)
    {
        lista->fim = NULL;
    }
    free(aux);
    free(valor);
}

void retirar_filme_do_meio(LISTA* lista, FILME* filme_escolhido)
{
    struct NO* anterior = lista->topo;
    struct NO* atual = lista->topo->proximo;
    struct NO* proximo = lista->topo->proximo->proximo;

    while(atual->filme != filme_escolhido)
    {
        anterior = atual;
        atual = proximo;
        proximo = proximo->proximo;
    }

    NO* aux = atual;

    anterior->proximo = proximo;

    free(aux->filme);
    free(aux);
}

void retirar_filme_do_final(LISTA* lista)
{
    if (lista->topo->proximo == NULL)
    {
        FILME* valor = lista->topo->filme;
        free(lista->topo);
        free(valor);

        lista->topo = NULL;
        lista->fim = NULL;

        return;
    }

    struct NO* ultimo = lista->topo;
    struct NO* penultimo = NULL;
    while(ultimo->proximo != NULL)
    {
        penultimo = ultimo;
        ultimo = ultimo->proximo;
    }

    FILME* valor = ultimo->filme;
    penultimo-> proximo = NULL;
    lista->fim = penultimo;
    free(ultimo);
    free(valor);

    return;
}

void deletar_filme(LISTA* lista, FILME* filme_escolhido)
{
    if (lista->topo->filme == filme_escolhido)
    {
        retirar_filme_do_comeco(lista);
        return;
    }
    else if (lista->fim->filme == filme_escolhido)
    {
        retirar_filme_do_final(lista);
        return;
    }

    retirar_filme_do_meio(lista, filme_escolhido);

    lista->quantidade--;
}

void trocar_filmes(LISTA *lista, NO *&anterior, NO *&atual, NO *&proximo)
{
    if (lista->topo == atual)
    {
        struct NO *aux = proximo->proximo;
        lista->topo = proximo;
        proximo->proximo = atual;
        atual->proximo = aux;

        anterior = lista->topo;

        return;
    }

    struct NO *proximo_do_proximo = proximo->proximo;
    struct NO *aux = atual;

    atual = proximo;
    anterior->proximo = atual;

    proximo = aux;
    atual->proximo = proximo;
    proximo->proximo = proximo_do_proximo;

    anterior = atual;
    atual = atual->proximo;
}

void ordernar_por_ano(LISTA *lista)
{
    if (lista->topo == NULL && lista->fim == NULL && lista->quantidade == 0)
    {
        printf(VERMELHO "Nenhum filme cadastrado!\n");
        printf(RESET);
        return;
    }

    for (int k = 1; k < lista->quantidade; k++)
    {
        struct NO *anterior = NULL;
        struct NO *atual = lista->topo;
        for (int j = 0; j < lista->quantidade - 1; j++)
        {
            struct NO *proximo = atual->proximo;

            if (proximo->filme->ano < atual->filme->ano)
            {
                trocar_filmes(lista, anterior, atual, proximo);
                continue;
            }

            anterior = atual;
            atual = atual->proximo;
        }
    }
}

void ordernar_por_titulo(LISTA *lista)
{
    if (lista->topo == NULL && lista->fim == NULL && lista->quantidade == 0)
    {
        printf(VERMELHO "Nenhum filme cadastrado!\n");
        printf(RESET);
        return;
    }

    for (int k = 1; k < lista->quantidade; k++)
    {
        struct NO *anterior = NULL;
        struct NO *atual = lista->topo;
        for (int j = 0; j < lista->quantidade - 1; j++)
        {
            struct NO *proximo = atual->proximo;

            if (strcmp(atual->filme->titulo, proximo->filme->titulo) > 0)
            {
                trocar_filmes(lista, anterior, atual, proximo);
                continue;
            }

            anterior = atual;
            atual = atual->proximo;
        }
    }
}

int calcular_pontuacao_diretor_e_genero(LISTA *lista_filmes, char diretor[255], char genero[255])
{
    int pontuacao_diretor = 0;
    int pontuacao_genero = 0;
    struct NO *filme_assistido = lista_filmes->topo;
    while (filme_assistido != NULL)
    {
        if (!filme_assistido->filme->assistiu)
        {
            filme_assistido = filme_assistido->proximo;
            continue;
        }

        if (strcmp(filme_assistido->filme->diretor, diretor) == 0)
        {
            if (filme_assistido->filme->gostou)
            {
                pontuacao_diretor += 1;
            }
            else
            {
                pontuacao_diretor -= 1;
            }
        }

        if (strcmp(filme_assistido->filme->genero, genero) == 0)
        {
            if (filme_assistido->filme->gostou)
            {
                pontuacao_genero += 1;
            }
            else
            {
                pontuacao_genero -= 1;
            }
        }

        filme_assistido = filme_assistido->proximo;
    }

    return pontuacao_diretor + pontuacao_genero;
}

void mostrar_filme(FILME *filme_escolhido)
{
    printf(VERDE "%s" AZUL " (%d)" RESET "\n", filme_escolhido->titulo, filme_escolhido->ano);
    printf(BRANCO "Duracao: " VERDE "%s" BRANCO "\n", filme_escolhido->duracao);
    printf(BRANCO "Diretor: " VERDE "%s" BRANCO "\n", filme_escolhido->diretor);
    printf(BRANCO "Genero: " VERDE "%s" BRANCO "\n", filme_escolhido->genero);

    printf(BRANCO "Assistiu: ");
    if (filme_escolhido->assistiu)
    {
        printf(VERDE "Sim\n");

        printf(BRANCO "Gostou: ");
        if (filme_escolhido->gostou)
        {
            printf(VERDE "Sim\n");
        }
        else
        {
            printf(VERMELHO "Nao\n");
        }
    }
    else
    {
        printf(VERMELHO "Nao\n");
    }
}

int main()
{
    struct LISTA* lista_filmes = (struct LISTA*) malloc(sizeof(struct LISTA));
    lista_filmes->topo = NULL;
    lista_filmes->fim = NULL;
    lista_filmes->quantidade = 0;

    cadastrar_filme(lista_filmes, "Emoji Filme", 2018, "2h", "Tony Leondis", "Animacao", false, false);
    cadastrar_filme(lista_filmes, "Scott Pilgrim Contra o Mundo", 2007, "3h 10m", "Edgar Wright", "Comedia", true, true);
    cadastrar_filme(lista_filmes, "The Batman", 2022, "3h", "Matt Reeves", "Crime", false, false);
    cadastrar_filme(lista_filmes, "Parasita", 2019, "2h 13m", "Bong Joon Ho", "Comedia", true, true);
    cadastrar_filme(lista_filmes, "Expresso do Amanha", 2013, "2h 7m", "Bong Joon Ho", "Ficcao Cientifica", true, true);
    cadastrar_filme(lista_filmes, "Rua Cloverfield, 10", 2004, "2h 10m", "Dan Trachtenberg", "Ficcao Cientifica", true, true);
    cadastrar_filme(lista_filmes, "The End of Evangelion", 1997, "1h 13m", "Hideaki Anno", "Animacao", true, false);
    cadastrar_filme(lista_filmes, "Filme Lego", 2014, "2h", "Christopher Miller", "Animacao", true, false);
    cadastrar_filme(lista_filmes, "Mickey 17", 2025, "2h 17m", "Bong Joon Ho", "Ficcao Cientifica", false, false);

    printf(BRANCO "================================\n");
    printf(VERDE "       Catalogo de filmes       \n");
    printf(BRANCO "================================\n");
    printf(RESET);

    printf(BRANCO "Integrantes:\n");
    printf(AZUL "Gabriel Vismeck Costa Stabel - " VERDE "2022102006\n");
    printf(AZUL "Fernando Galvao Smaniotto - " VERDE "2022102099\n");
    printf(AZUL "Kimberly Rayanne - " VERDE "2022102025\n");
    printf(AZUL "Maria Carolina - " VERDE "2022202695\n");
    printf(RESET);

    while(true)
    {
        int opcao_selecionada = 0;

        printf(BRANCO "================================\n");
        printf(VERDE "1 " AZUL "- Mostrar catalogo\n");
        printf(VERDE "2 " AZUL "- Cadastrar filme\n");
        printf(VERDE "3 " AZUL "- Buscar filme\n");
        printf(VERDE "4 " AZUL "- Pegar recomendacao de filme\n");
        printf(VERDE "5 " AZUL "- Remover filme\n");
        printf(VERDE "6 " AZUL "- Exibir todos detalhes de um filme\n");
        printf(VERDE "7 " AZUL "- Editar filme\n");
        printf(VERDE "0 " AZUL "- Sair\n");
        printf(BRANCO "================================\n");
        printf("\n\nDigite a opcao(ex: " VERDE "1" BRANCO "): ");
        printf(RESET);
        scanf("%i", &opcao_selecionada);

        limpar_tela();

        switch(opcao_selecionada)
        {
            case 1:
                {
                    printf("\n");
                    if (lista_filmes->topo == NULL && lista_filmes->fim == NULL)
                    {
                        printf(VERMELHO "Nenhum filme cadastrado!\n\n");
                        printf(RESET);

                        esperar_enter();
                        limpar_tela();

                        break;
                    }

                    if (lista_filmes->quantidade > 1)
                    {
                        printf("\n\n");
                        fflush(stdin);
                        printf(BRANCO "Como deseja ordenar os filmes? (" VERDE "ano" BRANCO ", " VERDE "titulo" BRANCO "): ");
                        printf(RESET);
                        char opcao[255];
                        scanf(" %[^\n]", &opcao);

                        if (strcmp(opcao, "ano") == 0)
                        {
                            ordernar_por_ano(lista_filmes);
                        }
                        else if (strcmp(opcao, "titulo") == 0)
                        {
                            ordernar_por_titulo(lista_filmes);
                        }
                        else
                        {
                            printf(VERMELHO "Tipo de ordenacao invalido.\n\n");
                            printf(RESET);

                            esperar_enter();
                            limpar_tela();

                            break;
                        }
                    }

                    printf(BRANCO "================================\n");
                    printf(VERDE "       CATALOGO DE FILMES       \n");
                    printf(BRANCO"================================\n");
                    printf(RESET);

                    struct NO* atual = lista_filmes->topo;

                    while(atual != NULL)
                    {
                        printf(BRANCO " * " VERDE "%s " AZUL "(" VERDE "%i" AZUL ") - " VERDE"%s\n", atual->filme->titulo, atual->filme->ano, atual->filme->duracao);
                        printf(RESET);

                        atual = atual->proximo;
                    }
                    printf("\n\n");

                    esperar_enter();
                    limpar_tela();
                    break;
                }
            case 2:
                {
                    printf("\n");
                    fflush(stdin);
                    printf(BRANCO "Digite o nome do filme (ex: " VERDE "Mickey 17" BRANCO"): ");
                    printf(RESET);
                    char titulo[255];
                    scanf(" %[^\n]", &titulo);

                    fflush(stdin);
                    printf(BRANCO "Digite o ano do filme (ex: " VERDE "2025" BRANCO"): ");
                    printf(RESET);
                    int ano;
                    scanf("%i", &ano);

                    fflush(stdin);
                    printf(BRANCO "Digite a duracao do filme (ex: " VERDE "2h 17m" BRANCO"): ");
                    printf(RESET);
                    char duracao[255];
                    scanf(" %[^\n]", &duracao);

                    fflush(stdin);
                    printf(BRANCO "Digite o diretor do filme (ex: " VERDE "Zack Snyder" BRANCO"): ");
                    printf(RESET);
                    char diretor[255];
                    scanf(" %[^\n]", &diretor);

                    fflush(stdin);
                    printf(BRANCO "Digite o genero do filme (ex: " VERDE "Comedia" BRANCO"): ");
                    printf(RESET);
                    char genero[255];
                    scanf(" %[^\n]", &genero);

                    fflush(stdin);
                    printf(BRANCO "Ja assistiu o filme? (" VERDE "sim" BRANCO"/" VERDE "nao" BRANCO"): ");
                    printf(RESET);
                    char assistiu_texto[255];
                    scanf(" %[^\n]", &assistiu_texto);
                    bool assistiu = strcmp(assistiu_texto, "sim") == 0;

                    bool gostou = false;
                    if (assistiu)
                    {
                        fflush(stdin);
                        printf(BRANCO "Gostou do filme? (" VERDE "sim" BRANCO"/" VERDE "nao" BRANCO"): ");
                        printf(RESET);
                        char gostou_texto[255];
                        scanf(" %[^\n]", &gostou_texto);
                        gostou = strcmp(gostou_texto, "sim") == 0;
                    }

                    cadastrar_filme(
                        lista_filmes,
                        titulo,
                        ano,
                        duracao,
                        diretor,
                        genero,
                        assistiu,
                        gostou
                    );

                    printf(VERDE "Filme cadastrado com sucesso!\n");
                    printf(RESET);
                    printf("\n\n");

                    esperar_enter();
                    limpar_tela();

                    break;
                }
            case 3:
                {
                    LISTA* lista_resultado = buscar_filmes(lista_filmes);

                    if (lista_resultado->quantidade == 0)
                    {
                        printf(VERMELHO "Nenhum filme encontrado.\n\n\n");
                        printf(RESET);
                        free(lista_resultado);

                        esperar_enter();
                        limpar_tela();

                        break;
                    }

                    printf(BRANCO "Resultados da busca:\n");
                    NO* atual = lista_resultado->topo;
                    while(atual != NULL)
                    {
                        printf(BRANCO " * " VERDE "%s " AZUL "(" VERDE "%i" AZUL ") - " VERDE"%s\n", atual->filme->titulo, atual->filme->ano, atual->filme->duracao);
                        printf(RESET);

                        atual = atual->proximo;
                    }
                    printf("\n\n");

                    esperar_enter();
                    limpar_tela();

                    // Liberar lista sem liberar filme
                    atual = lista_resultado->topo;
                    NO *proximo;

                    while(atual != NULL)
                    {
                        proximo = atual->proximo;
                        free(atual);
                        atual = proximo;
                    }

                    lista_resultado->topo = NULL;
                    lista_resultado->fim = NULL;
                    lista_resultado->quantidade = 0;

                    free(lista_resultado);
                    break;
                }
            case 4:
                {
                    struct FILME *recomendado = NULL;
                    int max_pontuacao = INT_MIN;

                    struct NO *atual = lista_filmes->topo;
                    while (atual != NULL)
                    {
                        if (atual->filme->assistiu)
                        {
                            atual = atual->proximo;
                            continue;
                        }

                        int pontuacao_total = calcular_pontuacao_diretor_e_genero(lista_filmes, atual->filme->diretor, atual->filme->genero);

                        if (pontuacao_total > max_pontuacao || recomendado == NULL)
                        {
                            max_pontuacao = pontuacao_total;
                            recomendado = atual->filme;
                        }
                        atual = atual->proximo;
                    }

                    if (recomendado != NULL)
                    {
                        printf(BRANCO "O filme indicado e " VERDE "%s " AZUL "(" VERDE "%i" AZUL ")\n", recomendado->titulo, recomendado->ano);
                    }
                    else
                    {
                        printf(VERMELHO "Voce ja assistiu todos os filmes cadastrados.\n");
                        printf(RESET);
                    }
                    printf("\n\n");

                    esperar_enter();
                    limpar_tela();

                    break;
                }
            case 5:
                {
                    LISTA* lista_resultado = buscar_filmes(lista_filmes);

                    if (lista_resultado->quantidade == 0)
                    {
                        printf(VERMELHO "Nenhum filme encontrado.\n\n\n");
                        printf(RESET);
                        free(lista_resultado);

                        esperar_enter();
                        limpar_tela();

                        break;
                    }

                    FILME* filme_escolhido = escolher_filme(lista_resultado);
                    if (filme_escolhido == NULL)
                    {
                        break;
                    }

                    fflush(stdin);
                    printf(BRANCO "Quer deletar o filme " VERDE "%s" BRANCO " " AZUL "(" VERDE "%i" AZUL ")" BRANCO "? (" VERDE "sim" BRANCO "/" VERDE "nao" BRANCO "): ", filme_escolhido->titulo, filme_escolhido->ano);
                    printf(RESET);
                    char deletar_texto[255];
                    scanf(" %[^\n]", &deletar_texto);
                    bool deletar = strcmp(deletar_texto, "sim") == 0;

                    if (deletar)
                    {
                        deletar_filme(lista_filmes, filme_escolhido);
                        printf(VERDE "Filme deletado como sucesso!\n");
                        printf(RESET);
                    }

                    printf("\n\n");

                    esperar_enter();
                    limpar_tela();

                    // Liberar lista sem liberar filme
                    NO* atual = lista_resultado->topo;
                    NO *proximo;

                    while(atual != NULL)
                    {
                        proximo = atual->proximo;
                        free(atual);
                        atual = proximo;
                    }

                    lista_resultado->topo = NULL;
                    lista_resultado->fim = NULL;
                    lista_resultado->quantidade = 0;

                    free(lista_resultado);
                    break;
                }
            case 6:
                {
                    LISTA* lista_resultado = buscar_filmes(lista_filmes);

                    if (lista_resultado->quantidade == 0)
                    {
                        printf(VERMELHO "Nenhum filme encontrado.\n\n\n");
                        printf(RESET);
                        free(lista_resultado);
                        break;
                    }

                    FILME* filme_escolhido = escolher_filme(lista_resultado);
                    if (filme_escolhido == NULL)
                    {
                        break;
                    }

                    mostrar_filme(filme_escolhido);

                    printf(RESET);
                    printf("\n\n");

                    esperar_enter();
                    limpar_tela();

                    // Liberar lista sem liberar filme
                    NO* atual = lista_resultado->topo;
                    NO *proximo;

                    while(atual != NULL)
                    {
                        proximo = atual->proximo;
                        free(atual);
                        atual = proximo;
                    }

                    lista_resultado->topo = NULL;
                    lista_resultado->fim = NULL;
                    lista_resultado->quantidade = 0;

                    free(lista_resultado);
                    break;
                }
            case 7:
                {
                    LISTA* lista_resultado = buscar_filmes(lista_filmes);

                    if (lista_resultado->quantidade == 0)
                    {
                        printf(VERMELHO "Nenhum filme encontrado.\n\n\n");
                        printf(RESET);
                        free(lista_resultado);
                        break;
                    }

                    FILME* filme_escolhido = escolher_filme(lista_resultado);
                    if (filme_escolhido == NULL)
                    {
                        break;
                    }

                    mostrar_filme(filme_escolhido);

                    int opcao_edicao = 0;
                    do
                    {
                        printf(BRANCO "\nEscolha um campo para editar:\n");
                        printf(VERDE "1" BRANCO " - Titulo\n");
                        printf(VERDE "2" BRANCO " - Ano\n");
                        printf(VERDE "3" BRANCO " - Duracao\n");
                        printf(VERDE "4" BRANCO " - Diretor\n");
                        printf(VERDE "5" BRANCO " - Genero\n");
                        printf(VERDE "6" BRANCO " - Status (assistiu)\n");
                        if (filme_escolhido->assistiu)
                        {
                            printf(VERDE "7" BRANCO " - Avaliacao (gostou)\n");
                        }
                        printf(VERDE "0" BRANCO " - Finalizar edicao\n");
                        printf(BRANCO "Opcao: ");
                        printf(RESET);
                        scanf("%i", &opcao_edicao);
                
                        switch (opcao_edicao)
                        {
                            case 1:
                                {
                                    printf(BRANCO "Novo titulo: ");
                                    printf(RESET);
                                    scanf(" %[^\n]", filme_escolhido->titulo);
                                    break;
                                }
                            case 2:
                                {
                                    printf(BRANCO "Novo ano: ");
                                    printf(RESET);
                                    scanf("%d", &filme_escolhido->ano);
                                    break;
                                }
                            case 3:
                                {
                                    printf(BRANCO "Nova duracao: ");
                                    printf(RESET);
                                    scanf(" %[^\n]", filme_escolhido->duracao);
                                    break;
                                }
                            case 4:
                                {
                                    printf(BRANCO "Novo diretor: ");
                                    printf(RESET);
                                    scanf(" %[^\n]", filme_escolhido->diretor);
                                    break;
                                }
                            case 5:
                                {
                                    printf(BRANCO "Novo genero: ");
                                    printf(RESET);
                                    scanf(" %[^\n]", filme_escolhido->genero);
                                    break;
                                }
                            case 6:
                                {
                                    char resposta[4];
                                    printf(BRANCO "Ja assistiu? (sim/nao): ");
                                    printf(RESET);
                                    scanf(" %[^\n]", resposta);
                                    bool assistiu = strcmp(resposta, "sim") == 0;
                                    filme_escolhido->assistiu = assistiu;
                    
                                    if (assistiu)
                                    {
                                        printf(BRANCO "Gostou do filme? (sim/nao): ");
                                        printf(RESET);
                                        scanf(" %[^\n]", resposta);
                                        bool gostou = strcmp(resposta, "sim") == 0;
                                        filme_escolhido->gostou = gostou;
                                    }
                                    else
                                    {
                                        filme_escolhido->gostou = false;
                                    }
                                    break;
                                }
                            case 7:
                                {
                                    if (filme_escolhido->assistiu)
                                    {
                                        char resposta[4];
                                        printf(BRANCO "Gostou do filme? (sim/nao): ");
                                        printf(RESET);
                                        scanf(" %[^\n]", resposta);
                                        bool gostou = strcmp(resposta, "sim") == 0;
                                        filme_escolhido->gostou = gostou;
                                    }
                                    break;
                                }
                            case 0:
                                {
                                    printf(VERDE "Edicao concluida!\n\n");
                                    printf(RESET);
                                    break;
                                }
                            default:
                                {
                                    printf(VERMELHO "Opcao invalida!\n");
                                    printf(RESET);
                                }
                        }
                    } while (opcao_edicao != 0);

                    printf(RESET);
                    printf("\n\n");

                    esperar_enter();
                    limpar_tela();

                    // Liberar lista sem liberar filme
                    NO* atual = lista_resultado->topo;
                    NO *proximo;

                    while(atual != NULL)
                    {
                        proximo = atual->proximo;
                        free(atual);
                        atual = proximo;
                    }

                    lista_resultado->topo = NULL;
                    lista_resultado->fim = NULL;
                    lista_resultado->quantidade = 0;

                    free(lista_resultado);
                    break;
                }
            case 0:
                printf(VERDE "Tchau!\n");
                printf(RESET);
                return 0;
            default:
                printf(VERMELHO "Opcao nao existente ou digitada incorretamente. Favor digitar novamente.\n\n");
                printf(RESET);

                esperar_enter();
                limpar_tela();

                break;
        }
    }

    liberar_lista(lista_filmes);
    return 0;
}
