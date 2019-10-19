#include <stdio.h>
#include <stdlib.h>
#include "functions.h"



int main()
{
    int dimension, **arrayptr;
    int cpt, lines, l=0, columns=0;
    char **ptr, player='O';
    int Opponents;

    printf("Welcome ! \n");
    printf("Introduce the size of the board you want (square at least of 6*6 and max 26*26):\n");     //max due to the number of letter in the alphabet
    do
    {
        scanf("%d",&dimension);
    }
    while ((dimension<6 || dimension > 26) || dimension%2!=0);

    //dynamical allocation
    ptr = (char**) malloc(dimension*sizeof(char*));

    if (ptr == NULL)
    {
        printf ("Error ptr\n");
        exit (0);
    }

    for (cpt=0; cpt<dimension; cpt++)
    {
        *(ptr+cpt) = (char*) malloc(dimension*sizeof(char));
    }

    arrayptr = (int**) malloc((dimension*dimension-4) * sizeof(int*));  //dimension*dimension-4 is the maximum of possibilities a player can have to play

    if (arrayptr == NULL)
    {
        printf("Error arrayptr\n");
        exit(0);
    }

    for (cpt = 0; cpt < (dimension*dimension-4); cpt++)
        *(arrayptr+cpt)= (int*) malloc (2*sizeof(int));     //we only need 2 columns, 1 for the ordinate and 1 for the abscissa

    // we fill the array with dots
    fillArray (dimension, ptr);


    // We finally display the array
    displayBoard(dimension,ptr,player);

    Opponents = whoplays();

    // we display the keys to use to play
    while (helpKeys(Opponents, player)==8 && ( endOfGame(dimension, ptr, player, arrayptr, &l) != 4 && endOfGame(dimension, ptr, player, arrayptr, &l) != 2 && endOfGame(dimension, ptr, player, arrayptr, &l) != 1))
    {
        //We check the player can play
        if (passTurn (dimension, ptr, player, arrayptr, &l))
        {

            displayPossibilitiesOfThePlayer (arrayptr, l, dimension, player, Opponents);

            do
            {
                columns = enterCoordinates(dimension, &lines, columns, arrayptr, player, Opponents, l);
            }
            while (checkMoves( arrayptr, l, lines, columns) == -1);
            l=0;

            returnPawns ( dimension, ptr, player, lines, columns);

            displayBoard(dimension, ptr, player);


            if (player =='O')
                player= 'X';
            else
                player = 'O';

            // we check we can still continue
            endOfGame (dimension, ptr, player, arrayptr, &l);

        }
        else
        {
            displayBoard(dimension, ptr, player);


            if (player =='O')
                player= 'X';
            else
                player = 'O';

            // we check we can still continue
            endOfGame (dimension, ptr, player, arrayptr, &l);
        }
    }

    determineWinner (dimension, ptr);

    freeptr (ptr,dimension);

    for (cpt = 0 ; cpt<2 ; cpt ++)
        free  (*(arrayptr+cpt));
    free(arrayptr);

    printf("thanks for playing!");
    return 0;
}


