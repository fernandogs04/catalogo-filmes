#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    printf("Digite o tipo de busca (titulo, ano): ");
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
        printf("Digite o termo para buscar no titulo: ");
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
        printf("Digite o ano para buscar: ");
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
        printf("Tipo de busca invalido.\n\n\n");
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
        printf("Resultados da busca:\n");
        FILME *resultados[lista->quantidade];

        NO *atual = lista->topo;
        for (int i = 0; i < lista->quantidade; i++)
        {
            resultados[i] = atual->filme;
            printf(" %i %s (%i) - %s\n", i + 1, atual->filme->titulo, atual->filme->ano, atual->filme->duracao);
            atual = atual->proximo;
        }

        fflush(stdin);
        printf("Escolha um filme (1, 2, etc): ");
        int indice_filme_escolhido = 0;
        scanf("%i", &indice_filme_escolhido);
        indice_filme_escolhido--;

        if (indice_filme_escolhido < 0 || indice_filme_escolhido >= lista->quantidade)
        {
            printf("Numero incorreto ou nao encontrado na lista.\n\n\n");
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
        printf("Nenhum filme cadastrado!\n");
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
        printf("Nenhum filme cadastrado!\n");
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

    printf("================================\n");
    printf("       Catalogo de filmes       \n");
    printf("================================\n");
    printf("Integrantes:\n");
    printf("Gabriel Vismeck Costa Stabel - 2022102006\n");
    printf("Fernando Galvao Smaniotto - 2022102099\n");
    printf("Kimberly Rayanne - 2022102025\n");
    printf("Maria Carolina - 2022202695\n");

    while(true)
    {
        int opcao_selecionada = 0;

        printf("================================\n");
        printf("1 - Mostrar catalogo\n");
        printf("2 - Cadastrar filme\n");
        printf("3 - Buscar filme\n");
        printf("4 - Pegar recomendacao de filme\n");
        printf("5 - Remover filme\n");
        printf("9 - Sair\n");
        printf("================================\n");
        printf("\n\nDigite a opcao(ex: 1): ");
        scanf("%i", &opcao_selecionada);

        switch(opcao_selecionada)
        {
            case 1:
                {
                    printf("\n");
                    if (lista_filmes->topo == NULL && lista_filmes->fim == NULL)
                    {
                        printf("Nenhum filme cadastrado!\n");
                        break;
                    }

                    printf("================================\n");
                    printf("       CATALOGO DE FILMES       \n");
                    printf("================================\n");

                    if (lista_filmes->quantidade > 1)
                    {
                        printf("\n\n");
                        fflush(stdin);
                        printf("Como deseja ordenar os filmes? (ano, titulo): ");
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
                            printf("Tipo de ordenacao invalido.\n");
                            break;
                        }
                    }

                    struct NO* atual = lista_filmes->topo;

                    while(atual != NULL)
                    {
                        printf(" * %s (%i) - %s\n", atual->filme->titulo, atual->filme->ano, atual->filme->duracao);

                        atual = atual->proximo;
                    }
                    printf("\n\n");
                    break;
                }
            case 2:
                {
                    printf("\n");
                    fflush(stdin);
                    printf("Digite o nome do filme (ex: Mickey 17): ");
                    char titulo[255];
                    scanf(" %[^\n]", &titulo);

                    fflush(stdin);
                    printf("Digite o ano do filme (ex: 2025): ");
                    int ano;
                    scanf("%i", &ano);

                    fflush(stdin);
                    printf("Digite a duracao do filme (ex: 2h 17m): ");
                    char duracao[255];
                    scanf(" %[^\n]", &duracao);

                    fflush(stdin);
                    printf("Digite o diretor do filme (ex: Zack Snyder): ");
                    char diretor[255];
                    scanf(" %[^\n]", &diretor);

                    fflush(stdin);
                    printf("Digite o genero do filme (ex: Comedia): ");
                    char genero[255];
                    scanf(" %[^\n]", &genero);

                    fflush(stdin);
                    printf("Ja assistiu o filme? (sim/nao): ");
                    char assistiu_texto[255];
                    scanf(" %[^\n]", &assistiu_texto);
                    bool assistiu = strcmp(assistiu_texto, "sim") == 0;

                    bool gostou = false;
                    if (assistiu)
                    {
                        fflush(stdin);
                        printf("Gostou do filme? (sim/nao): ");
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
                    printf("\n\n");
                    break;
                }
            case 3:
                {
                    LISTA* lista_resultado = buscar_filmes(lista_filmes);

                    if (lista_resultado->quantidade == 0)
                    {
                        printf("Nenhum filme encontrado.\n\n\n");
                        free(lista_resultado);
                        break;
                    }

                    printf("Resultados da busca:\n");
                    NO* atual = lista_resultado->topo;
                    while(atual != NULL)
                    {
                        printf(" * %s (%i) - %s\n", atual->filme->titulo, atual->filme->ano, atual->filme->duracao);
                        atual = atual->proximo;
                    }
                    printf("\n\n");

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
                        printf("O filme indicado e %s (%i)\n", recomendado->titulo, recomendado->ano);
                    }
                    else
                    {
                        printf("Voce ja assistiu todos os filmes cadastrados.\n");
                    }
                    printf("\n\n");
                    break;
                }
            case 5:
                {
                    LISTA* lista_resultado = buscar_filmes(lista_filmes);

                    if (lista_resultado->quantidade == 0)
                    {
                        printf("Nenhum filme encontrado.\n\n\n");
                        free(lista_resultado);
                        break;
                    }

                    FILME* filme_escolhido = escolher_filme(lista_resultado);
                    if (filme_escolhido == NULL)
                    {
                        break;
                    }

                    fflush(stdin);
                    printf("Quer deletar o filme %s (%i)? (sim/nao): ", filme_escolhido->titulo, filme_escolhido->ano);
                    char deletar_texto[255];
                    scanf(" %[^\n]", &deletar_texto);
                    bool deletar = strcmp(deletar_texto, "sim") == 0;

                    if (deletar)
                    {
                        deletar_filme(lista_filmes, filme_escolhido);
                        printf("Filme deletado como sucesso!\n");
                    }

                    printf("\n\n");

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
            case 9:
                printf("Tchau!\n");
                return 0;
            default:
                printf("Opcao nao existente ou digitada incorretamente. Favor digitar novamente.\n\n");
                break;
        }
    }

    liberar_lista(lista_filmes);
    return 0;
}
