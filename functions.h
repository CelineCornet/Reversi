

void fillArray (int dim, char **ptr);
void displayBoard (int dim, char **ptr, char player);
int helpKeys (int Opponents, char player);
void secureBorder (int coordinate,int *firstval,int *lastval,int dim);
int passTurn (int dim, char **ptr, char player, int **arrayptr, int *l);
int displayPossibilitiesOfThePlayer (int **arrayptr, int l, int dim, char player, int Opponents);
char enterCoordinates(int dimension,int *lines, char columns, int **arrayptr, char player, int Opponents, int l);
int checkMoves (int **arrayptr, int l, int lines,char columns);
void returnPawns (int dim, char **ptr, char player, int line, int col);
int endOfGame (int dimension,char  **ptr, char player, int **arrayptr, int *l);
void determineWinner (int dim, char **ptr);
void freeptr (char **ptr, int dimension);
int whoplays();

