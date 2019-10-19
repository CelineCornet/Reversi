#include <stdio.h>
#include <stdlib.h>
#include <myconio.h>
#include <time.h>

void fillArray (int dim, char **ptr)
{
    int cpt,cpt2;

    for (cpt = 0 ; cpt < dim ; cpt++)
    {
        for (cpt2 = 0 ; cpt2 < dim ; cpt2++)
        {
            *(*(ptr+cpt)+cpt2) = '.';
        }
    }
    //First pieces
    *(*(ptr+(dim)/2)+ (dim)/2-1) = *(*(ptr+(dim)/2-1)+ dim/2) = 'O';
    *(*(ptr+(dim)/2)+ (dim)/2) = *(*(ptr+(dim)/2-1)+ (dim)/2-1) = 'X';

}

void displayBoard (int dim, char **ptr, char player)
{
    int cpt, cpt2;
    char letter = 65;
    int pawnsX=0, pawnsO = 0;

    printf("the board is the following : \n");

    for (cpt = 0 ; cpt <= dim ; cpt++)  // cpt does not correspond to the line of the array
    {
        //we display the line's numbers
        textcolor(YELLOW);
        if (cpt==0)
            printf("    ");
        if (cpt > 0 && cpt < 10)
            printf(" %d  ",cpt);
        if (cpt>=10)
            printf ("%d  ",cpt);

        // we display the column's letters
        for (cpt2 = 1 ; cpt2 <= dim ; cpt2++)   //cpt2 doesn't correspond to the column of the array
        {
            if (cpt == 0)
            {
                textcolor(YELLOW);
                printf("%c ",letter);
                letter++;
            }
            if (cpt>=1)
            {
                if ( *(*(ptr+cpt-1)+cpt2-1) == 'X')
                {
                    textcolor(LIGHTRED);
                    printf("%c ",*(*(ptr+cpt-1)+cpt2-1));   //we subtract 1 because cpt and cpt2 doesn't correspond to the case of the array
                }
                if ( *(*(ptr+cpt-1)+cpt2-1) == 'O' )
                {
                    textcolor(LIGHTGREEN);
                    printf("%c ",*(*(ptr+cpt-1)+cpt2-1));   //we subtract 1 because cpt and cpt2 doesn't correspond to the case of the array
                }
                if ( *(*(ptr+cpt-1)+cpt2-1) == '.')
                {
                    textcolor(LIGHTGRAY);
                    printf("%c ", *(*(ptr+cpt-1)+cpt2-1));
                }
            }
        }
        printf("\n");
    }

    for (cpt=0 ; cpt <dim ; cpt++)
    {
        for (cpt2=0 ; cpt2<dim ; cpt2++)
        {
            if (*(*(ptr+cpt)+cpt2)=='X')
            {
                pawnsX++;
            }
            if (*(*(ptr+cpt)+cpt2)=='O')
            {
                pawnsO++;
            }
        }
    }


    textcolor(LIGHTRED);
    printf("        Number of X : %d\n", pawnsX);
    textcolor(LIGHTGREEN);
    printf("        Number of O : %d\n", pawnsO);


    textcolor(WHITE);
}

int helpKeys (int Opponents, char player)
{
    char commands;
    int result = 6;

    if (Opponents==3 || ( Opponents == 2 && player == 'X' ) )    // the turn is played automatically
    {
        return 8;
    }
    else
    {
        textcolor(LIGHTCYAN);
        printf("\n\nPress p if you want to keep playing");
        printf("\nPress e if you want to stop\n");
        textcolor(LIGHTMAGENTA);
        printf("Enter your command : ");

        do
        {
            scanf("%c",&commands);
        }
        while (commands!='e' && commands != 'p');

        textcolor(WHITE);

        if (commands == 'p')
            result = 8;
        return result;
    }
}

void secureBorder (int coordinate,int *firstval,int *lastval,int dim)
{
    *firstval = coordinate-1;
    if ( *firstval < 0 )
        *firstval = 0;

    *lastval = coordinate +1;
    if ( *lastval >= dim )
        *lastval = dim-1;
}

int passTurn (int dim, char **ptr, char player, int **arrayptr, int *l)
{
    int cpt,cpt2, cpt3, cpt4, diffline,line, col, diffcol, check;
    int firstline, lastline, firstcol, lastcol; //variables needed for the function secureBorder
    char otherplayer;

    if (player == 'O')
        otherplayer = 'X' ;
    else
        otherplayer = 'O' ;

    //loop to check every case of the board
    for (line = 0; line < dim; line++)
    {
        for (col = 0; col < dim; col++)
        {
            if (*(*(ptr+line)+col) == '.')
            {
                check=1;
                // if the case is empty, we check the cases next to it
                secureBorder (line, &firstline, &lastline, dim);
                secureBorder (col, &firstcol, &lastcol, dim);

                for (cpt=firstline; cpt<=lastline; cpt++)
                {
                    for (cpt2=firstcol; cpt2<=lastcol; cpt2++)
                    {

                        if (check && (cpt!=line || cpt2!=col))
                        {
                            if ( *(*(ptr+cpt)+cpt2) == otherplayer )
                            {
                                // we check the further pawns
                                cpt3=cpt;
                                cpt4=cpt2;

                                // we find in which direction we have to look further
                                diffline=line-cpt;
                                diffcol=col-cpt2;
                                while (cpt3-diffline>=0 && cpt3-diffline<dim && cpt4-diffcol>=0 && cpt4-diffcol<dim && *(*(ptr+cpt3-diffline)+cpt4-diffcol) != player && *(*(ptr+cpt3-diffline)+cpt4-diffcol) != '.' )
                                {
                                    // we look further
                                    cpt3-=diffline;
                                    cpt4-=diffcol;
                                }
                                if ( cpt3-diffline>=0 && cpt3-diffline<dim && cpt4-diffcol>=0 && cpt4-diffcol<dim && *(*(ptr+cpt3-diffline)+cpt4-diffcol) == player )
                                {
                                    //the player can play
                                    *(*(arrayptr+*l)+1)= line+1;      // we add 1 to make the value correspond to the number and letter displayed next to the board
                                    *(*(arrayptr+*l)+2)= col;
                                    *l+=1;
                                    check=0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (*l == 0)
    {
        // the player has to pass his/her turn
        return 0;
    }
    return 1;
}

int displayPossibilitiesOfThePlayer (int **arrayptr, int l, int dim,  char player, int Opponents)
{
    int cpt;
    char display;
    if (l == 0)
    {
        printf("Sorry, you have to pass your turn\n");
        return 0;
    }
    else
    {
        if (Opponents == 1 || (Opponents==2 && player !='X'))
        {
            printf("Do you want the possibilities of play ? If so press Y if not press N\n");
            do
            {
                scanf ("%c",&display);
            }
            while (display != 'Y' && display != 'N');
            if (display == 'Y')
            {
                textcolor(WHITE);
                printf("You can play in : \n");
                for (cpt=0; cpt<l; cpt++)
                {
                    // we convert the number of the column into the corresponding letter
                    printf("%d %c\n", *(*(arrayptr+cpt)+1), 65+*(*(arrayptr+cpt)+2) );
                }
            }
        }
    }

    return 1;
}

char enterCoordinates(int dimension,int *lines, char columns, int **arrayptr, char player, int Opponents, int l)
{
    int lin, i;

    textcolor(LIGHTCYAN);

    if (Opponents == 2)
    {
        if (player == 'O')
        {
            printf("enter your coordinates as so : 3D\n   ");

            do
            {
                scanf ("%d",&lin);
                scanf ("%c",&columns);

            }
            while ((lin<=0 || lin > dimension+1) || (columns<65 || columns > dimension+65));

            *lines=lin;                  // we want to be able to change the value of line so we take *line
        }
        else
        {
            i = ( rand()%l);                // this is if the player does not play himself
            *lines = *(*(arrayptr+i)+1);            // we take a random value from the array that shows the possibilities
            columns = 65+*(*(arrayptr+i)+2);
        }
    }

    else if (Opponents == 3)
    {
        i = ( rand()%l);
        *lines = *(*(arrayptr+i)+1);
        columns = 65+*(*(arrayptr+i)+2);

    }
    else
    {
        printf("enter your coordinates as so : 3D\n   ");
        do
        {
            scanf ("%d %c", &lin, &columns);
        }
        while (lin<=0 || lin > dimension+1 || columns<65 || columns > dimension+65);

        *lines=lin;
    }
    return columns;         // we return the column as the function is a 'char' function
}

int whoplays ()
{
    int choice;
    do
    {
        printf("Enter 1 if you want to play with your partner, \n2 if you are on your own against the computer,\n3 if you want to watch the game\n");
        scanf("%d", &choice);
    }
    while (choice>4 && choice<0);

    return choice;
}

int checkMoves (int **arrayptr, int l, int lines,char columns)
{
    int col, cpt;

    col = columns-65;

    //we check the value correspond to one of the coordinate found in the function passturn
    for (cpt=0; cpt<l; cpt++)
    {
        if (*(*(arrayptr+cpt)+1) == lines && *(*(arrayptr+cpt)+2) == col)
        {
            printf("You can play !\n");
            return 1;
        }
    }
    printf("You can't play here, please enter new coordinates\n");
    return -1;
}

void returnPawns (int dim, char **ptr, char player, int lines, int columns)
{
    int cpt,cpt2, cpt3, cpt4, cpt5, cpt6;
    int diffline,line, col, diffcol;
    int firstline, lastline, firstcol, lastcol; //variables needed for the function secureBorder
    char otherplayer;

    if (player == 'O')
        otherplayer = 'X' ;
    else
        otherplayer = 'O' ;

    col = columns - 65;
    line = lines-1 ;

    // fill current case
    *( *(ptr + line) + col) = player;

    // fill cases in all directions
    secureBorder (line, &firstline, &lastline, dim);
    secureBorder (col, &firstcol, &lastcol, dim);

    for (cpt=firstline; cpt<=lastline; cpt++)
    {
        for (cpt2=firstcol; cpt2<=lastcol; cpt2++)
        {
            if (cpt!=line || cpt2!=col)
            {
                if ( *(*(ptr+cpt)+cpt2) == otherplayer )
                {
                    // we check the further pawns
                    cpt3=cpt;
                    cpt4=cpt2;

                    // we find in which direction we have to look further
                    diffline=line-cpt;
                    diffcol=col-cpt2;

                    while (cpt3-diffline>=0 && cpt3-diffline<dim && cpt4-diffcol>=0 && cpt4-diffcol<dim && *(*(ptr+cpt3-diffline)+cpt4-diffcol) == otherplayer )
                    {
                        cpt3-=diffline;
                        cpt4-=diffcol;
                    }

                    if ( cpt3-diffline>=0 && cpt3-diffline<dim && cpt4-diffcol>=0 && cpt4-diffcol<dim && *(*(ptr+cpt3-diffline)+cpt4-diffcol) == player )
                    {
                        // we change the cases between the two found with player
                        cpt5=cpt3;
                        cpt6=cpt4;

                        while (cpt5 != line || cpt6 != col)
                        {
                            *( *(ptr + cpt5) + cpt6) = player;
                            cpt5 +=diffline;
                            cpt6 +=diffcol;
                        }
                    }
                }
            }
        }
    }
}

int endOfGame (int dimension,char  **ptr, char player, int **arrayptr, int *l)
{
    int cpt,cpt2, l2;
    int pawns=0;
    int cases=0;
    char otherplayer;
    char temp;

    l2=*l;

    if (player == 'O')
        otherplayer = 'X' ;
    else
        otherplayer = 'O' ;

    // no more pawns for one player or no more case to fill
    for (cpt=0 ; cpt <dimension ; cpt++)
    {
        for (cpt2=0 ; cpt2<dimension ; cpt2++)
        {
            if (*(*(ptr+cpt)+cpt2)==otherplayer)
            {
                pawns++;            // we check that there still is some pawns for the other player
                cases++;
            }
            if (*(*(ptr+cpt)+cpt2)==player)
            {
                cases++;            // we check that there is still some space for pawns to be put in
            }
        }
    }
    if (pawns==0)
        return 1;
    if (cases == (dimension*dimension))
        return 2;

    // no player can play
    temp=player;
    if (passTurn(dimension, ptr, player, arrayptr, &l2) == 0)
    {
        player = otherplayer;
        if (passTurn(dimension, ptr, player, arrayptr, &l2) == 0)
            return 4;
        else
            player = temp;
    }
    return 0;
}


void determineWinner (int dim, char **ptr)
{
    int playerO=0, playerX=0, cpt, cpt2;

    for (cpt=0; cpt<dim; cpt++)
    {
        for(cpt2=0; cpt2<dim; cpt2++)
        {
            if (*(*(ptr+cpt)+cpt2) == 'O')
                playerO+=1;
            if (*(*(ptr+cpt)+cpt2) == 'X')
                playerX+=1;
        }
    }
    if (playerO > playerX)
    {
        textcolor(LIGHTGREEN);
        printf("Player O win the game ! Congratulations !\n\n");
        textcolor(WHITE);
    }
    else
    {
        if (playerX > playerO)
        {
            textcolor(LIGHTRED);
            printf("Player X win the game ! Congratulations !\n\n");
            textcolor(WHITE);
        }
        else
        {
            textcolor(YELLOW);
            printf("there is a draw... let's have another go !\n\n");
            textcolor(WHITE);
        }
    }

}


void freeptr (char **ptr, int dimension)
{
    int cpt;

    for (cpt = 0 ; cpt<dimension ; cpt ++)
        free  (*(ptr+cpt));
    free(ptr);
}
