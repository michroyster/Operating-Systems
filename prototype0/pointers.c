#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DynamicArray{
    int y;
    int x;
    char *board;
} DynamicArray;

typedef struct Player {
    char name[64];
    char date[64];
    int totalGenerations;
} Player;

void playerInfo(Player *player);
void setDimensions(DynamicArray *array, int y, int x);
void initializeFromFile(DynamicArray *array);
void generations(DynamicArray *array);
void printArray(DynamicArray *array);
void copyArray(DynamicArray *dest, DynamicArray *origin);
int countActive(int y, int x, DynamicArray *array);
int gameOverCheck(DynamicArray *array);
void displayGeneration(int gen, DynamicArray *ptrArray);

int main()
{
    DynamicArray dynamicArray; // Declare Struct
    DynamicArray *ptrArray = &dynamicArray; // Declare Array and set it to dynamicArray address

    // Read into file
    initializeFromFile(ptrArray);

    // Get player info
    Player player;
    Player *ptrPlayer = &player;
    playerInfo(ptrPlayer);


    displayGeneration(player.totalGenerations, ptrArray);



    free(ptrArray->board);
    return 0;
}

void setDimensions(DynamicArray *array, int y, int x)
{
    array->y = y;
    array->x = x;
}

//Good
void initializeFromFile(DynamicArray *array)
{
    FILE *file = fopen("state.txt", "r");
    if (file == NULL){
        printf("Unable to open file");
        exit(1);
    }

    char buffer[200];
    fgets(buffer, sizeof(buffer), file);

    // Get Dimensions
    sscanf(buffer, "%d %d", &array->y, &array->x);

    // Allocate Memory for 2D array
    array->board = (char *)malloc(sizeof(char) * array->y * array->x);

    char *token;
    char *temp = malloc(sizeof(char) * array->x);
    int i = 0;
    while(!feof(file))
    {
        fgets(buffer, sizeof(buffer), file);
        token = strtok(buffer, " ");
        //printf("%c", *token);
        strcpy(temp, token);
        while (token != NULL)
        {
            token = strtok(NULL, " ");
            if (token != NULL){
                //printf("%c", *token);
                strcat(temp, token);
            }
        }
        strcpy((array->board + i * array->x), temp);
        i++;
    }

    free(temp);
    fclose(file);
}

void generations(DynamicArray *array)
{
    // DynamicArray *temp = malloc(sizeof(array)); // For CSX
    DynamicArray temporary;
    DynamicArray *temp = &temporary; // For my machine
    copyArray(temp, array);
    int activeCount = 0;
    int active = 0;
    int i, j;
    // printf("generations");
    for (i = 0; i <  array->y; i++){
        for (j = 0; j < array->x; j++) {
            activeCount = countActive(i, j, array);
            if (*(array->board + i * array->y + j) == 'X'){
                // Rule 1 and 2.
                if(activeCount < 2 || activeCount > 3)
                {
                    *(temp->board + i * temp->y + j) = '-';
                }
                // Rule 3.
                else if (activeCount == 2 || activeCount == 3)
                {
                    *(temp->board + i * temp->y + j) = 'X';
                }
            }
            else
            {
                // Rule 4.
                if (activeCount == 3)
                {
                    *(temp->board + i * temp->y + j) = 'X';
                }
            }
        }
    }
    copyArray(array, temp);
    //printf("test");
    free(temp->board);
    // free(temp);
}

//Good
void printArray(DynamicArray *array)
{
    int i, j;
    for (i = 0; i <  array->y; i++){
        for (j = 0; j < array->x; j++) {
            printf("%c ", *(array->board + i * array->y + j));
        }
        printf("\n");
    }
}

//Good
void copyArray(DynamicArray *dest, DynamicArray *origin)
{
    dest->y = origin->y;
    dest->x = origin->x;

    dest->board = (char *)malloc(sizeof(char) * dest->y * dest->x);

    int i, j;
    for (i = 0; i < origin->y; i++){
        for (j = 0; j < origin->x; j++) {
            *(dest->board + i * dest->y + j) = *(origin->board + i * origin->y + j);
        }
    }
}

//Good
int countActive(int y, int x, DynamicArray *array)
{
    int neighbors = 0;
    int above = 0;
    int below = 0;
    int left = 0;
    int right = 0;
    int offset = array->y * y + x;
    // printf("\nOffset: %d\n", offset);
    // printf("y=%d, array->y = %d\n", y, array->y);
    // *(array->board + offset) = 'O';
    // Checking for possible neighbors.
    if (y * array->y - array->y >= 0) above = 1;
    if (y < array->y) below = 1;
    if (x - 1 >= 0) left = 1;
    if (x + 1 < array->x) right = 1;

    // printf("above=%d, below=%d, left=%d, right=%d\n", above, below, left, right);
    // Counting active neighbors if they exist
    if (above)
    {
        if (*(array->board - array->y + offset) == 'X') neighbors++;
        if (left)
        {
            if(*(array->board - array->y - 1 + offset) == 'X') neighbors++;
        }
        if (right)
        {
            if (*(array->board - array->y + 1 + offset) == 'X') neighbors++;
        }
    }
    if (left)
    {
        if (*(array->board - 1 + offset) == 'X') neighbors++;

    }
    if (right)
    {
        if (*(array->board + 1 + offset) == 'X') neighbors++;
    }
    if (below)
    {
        if (*(array->board + array->y + offset) == 'X') neighbors++;
        if (left)
        {
            if (*(array->board + array->y - 1 + offset) == 'X') neighbors++;
        }
        if (right)
        {
            if (*(array->board + array->y + 1 + offset) == 'X') neighbors++;
        }
    }
    return neighbors;
}

void playerInfo(Player *player)
{
    char name[64];
    char date[64];
    int gen;
    printf("Enter player name: ");
    scanf("%s", name);
    printf("Enter date: ");
    scanf("%s", date);
    printf("Enter number of generations: ");
    scanf("%d", &gen);

    strcpy(player->name, name);
    strcpy(player->date, date);
    player->totalGenerations = gen;
}

int gameOverCheck(DynamicArray *array)
{
    int i, j;
    for (i = 0; i <  array->y; i++){
        for (j = 0; j < array->x; j++) {
            if(*(array->board + i * array->y + j) == 'X')
                return 0;
        }
    }
    return 1;
}

void displayGeneration(int gen, DynamicArray *ptrArray)
{
    int gameOver = 0;
    printf("Initial generation:\n");
    printArray(ptrArray);
    int i = 0;
    for (i = 1; i <= gen && !gameOver; i++)
    {
        printf("generation: %d\n", i);
        generations(ptrArray);
        printArray(ptrArray);
        if (gameOver = gameOverCheck(ptrArray))
            printf("\nAfter generation %d, all cells in the board are inactive!\n\n", i);
    }
}

