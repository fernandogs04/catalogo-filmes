#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct NO
{
    char titulo[255];
    int posicao_primeiro_caracter;
    int ano;
    char duracao[255];
    char diretor[255];
    char genero[255];
    bool assistiu;
    bool gostou;

    struct NO *proximo;
};

struct LISTA
{
    struct NO* topo = NULL;
    struct NO* fim = NULL;
    int quantidade = 0;
};

int main()
{
    struct LISTA* lista_filmes = (struct LISTA*) malloc(sizeof(struct LISTA));
    lista_filmes->topo = NULL;
    lista_filmes->fim = NULL;
    lista_filmes->quantidade = 0;

    NO* topo = (struct NO*) malloc(sizeof(struct NO));
    strcpy(topo->titulo, "Rua Cloverfield, 10");
    topo->ano = 2004;
    strcpy(topo->duracao, "2h 10m");
    strcpy(topo->diretor, "Dan Trachtenberg");
    strcpy(topo->genero, "Ficcao Cientifica");
    topo->assistiu = true;
    topo->gostou = true;

    NO* proximo = (struct NO*) malloc(sizeof(struct NO));
    strcpy(proximo->titulo, "The End of Evangelion");
    proximo->ano = 1997;
    strcpy(proximo->duracao, "1h 13m");
    strcpy(proximo->diretor, "Hideaki Anno");
    strcpy(proximo->genero, "Animacao");
    proximo->assistiu = true;
    proximo->gostou = false;

    topo->proximo = proximo;

    lista_filmes->topo = topo;
    lista_filmes->fim = proximo;

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
                    printf("Mostrar catalogo ainda nao implementado!\n\n");
                    break;
                }
            case 2:
                {
                    printf("Cadastrar filme ainda nao implementado!\n\n");
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

    return 0;
}
