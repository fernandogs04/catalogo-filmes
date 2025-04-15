#include <stdio.h>

int main()
{
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
