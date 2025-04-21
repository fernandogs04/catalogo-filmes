#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main()
{
    struct LISTA* lista_filmes = (struct LISTA*) malloc(sizeof(struct LISTA));
    lista_filmes->topo = NULL;
    lista_filmes->fim = NULL;
    lista_filmes->quantidade = 0;

    cadastrar_filme(lista_filmes, "The End of Evangelion", 1997, "1h 13m", "Hideaki Anno", "Animacao", true, false);
    cadastrar_filme(lista_filmes, "Rua Cloverfield, 10", 2004, "2h 10m", "Dan Trachtenberg", "Ficcao Cientifica", true, true);
    cadastrar_filme(lista_filmes, "Scott Pilgrim Contra o Mundo", 2007, "3h 10m", "Edgar Wright", "Comedia", true, true);
    cadastrar_filme(lista_filmes, "Expresso do Amanha", 2013, "2h 7m", "Bong Joon Ho", "Ficcao Cientifica", true, true);
    cadastrar_filme(lista_filmes, "Filme Lego", 2014, "2h", "Christopher Miller", "Animacao", true, false);
    cadastrar_filme(lista_filmes, "Emoji Filme", 2018, "2h", "Tony Leondis", "Animacao", false, false);
    cadastrar_filme(lista_filmes, "Parasita", 2019, "2h 13m", "Bong Joon Ho", "Comedia", true, true);
    cadastrar_filme(lista_filmes, "The Batman", 2022, "3h", "Matt Reeves", "Crime", false, false);
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
                    printf("Buscar filme ainda nao implementado!\n\n");
                    break;
                }
            case 4:
                {
                    printf("Pegar recomendacao de filme ainda nao implementado!\n\n");
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
