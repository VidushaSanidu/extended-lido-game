#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

const int START_POINTS[PLAYERS] = {50, 11, 24, 37};      // Starting positions for Y, B, R, G
const int APPROACH_POSITIONS[PLAYERS] = {0, 13, 26, 39}; // Last cell before home straight for Y, B, R, G
const int HOME_ENTRIES[PLAYERS] = {51, 12, 25, 38};      // First position in the home straight for Y, B, R, G

const char *colorNames[] = {
    "YELLOW",
    "BLUE",
    "RED",
    "GREEN"};

const char *direction[] = {
    "Clockwise", "Anti-Clockwise"};

PlayerColor get_color(int num)
{
    if (num == 0)
        return YELLOW;
    else if (num == 1)
        return BLUE;
    else if (num == 1)
        return BLUE;
    else
        return GREEN;
}

int roll_dice()
{
    return (rand() % 6) + 1;
}

int coin_toss()
{
    return (rand() % 2) + 1;
}

Cell standardCells[STANDARD_CELLS];

// PRE GAME FUNCTIONS

void pre_message()
{
    printf("\n-------Welcome to Ludo CS-------\n\n");
    for (int i = 0; i < PLAYERS; i++)
    {
        printf("The %s player has four (04) pieces named %c1, %c2, %c3, and %c4.\n", colorNames[i], colorNames[i][0], colorNames[i][0], colorNames[i][0], colorNames[i][0]);
    }
}

void initialize_players(Player *players[], int current)
{
    int maxValue = 0;
    int maxIndex = 0;
    int value = 0;

    for (int i = 0; i < PLAYERS; i++)
    {
        value = roll_dice();
        printf("%s rolls %d \n", colorNames[i], value);
        if (value > maxValue)
        {
            maxValue = value;
            maxIndex = i;
        }
    }
    printf("\n%s player has the highest roll and will begin the game.\n", colorNames[maxIndex]);

    initialize_queue(players);
    current = maxIndex;

    printf("The order of a single round is %s, %s, %s, and %s.\n", colorNames[maxIndex], colorNames[(maxIndex + 1) % 4], colorNames[(maxIndex + 2) % 4], colorNames[(maxIndex + 3) % 4]);
}

void initialize_board()
{
    standardCells[BHAWANA_I].type = BHAWANA;
    standardCells[KOTUWA_I].type = KOTUWA;
    standardCells[PITA_KOTUWA_I].type = PITA_KOTUWA;
    for (int i = 0; i < STANDARD_CELLS; i++)
    {
        standardCells[i].currentColor = DEFAULT;
    }
}

void initialize_queue(Player players[])
{
    for (int i = 0; i < PLAYERS; i++)
    {
        players[i].piecesInBase = 4;
        players[i].piecesInHome = 0;
        players[i].consecutiveSixes = 0;
        players[i].color = get_color(i);
        for (int j = 0; j < PIECES; j++)
        {
            players[i].pieces[j].status = BASE;
            players[i].pieces[j].capturedPieces = 0;
            players[i].pieces[j].toWin = 52;
            players[i].pieces[j].straightCount = 0;
            players[i].pieces[j].approchCount = 0;
            players[i].pieces[j].isBlocked = false;
        }
    }
}

// IN GAME FUNCTIONS

int get_nearest_hunt_for_single(Player player, int max)
{
    int hunt = -1;
    int gap = 60;
    for (int i = 0; i < PIECES; i++)
    {
        if (player.pieces[i].status == ONTRACK)
        {
            if (player.pieces[i].direction == clock)
            {
                for (int j = player.pieces[i].position; j < max; j++)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color) break;
                    if (standardCells[j].noOfPiece == 1 && standardCells[j].currentColor != player.color && gap > player.pieces[i].toWin)
                    {
                        hunt = j;
                    }
                }
            }else{
                for (int j = player.pieces[i].position; j > max; j--)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color) break;
                    if (standardCells[j].noOfPiece == 1 && standardCells[j].currentColor != player.color && gap > player.pieces[i].toWin)
                    {
                        hunt = j;
                    }
                }
            }
        }
    }
}

int get_nearest_hunt_for_block(Player player, int max)
{
    int hunt = -1;
    int gap = 60;
    for (int i = 0; i < PIECES; i++)
    {
        if (player.pieces[i].status == ONTRACK && player.pieces[i].isBlocked == true)
        {
            if (player.pieces[i].blockDirection == clock)
            {
                for (int j = player.pieces[i].position; j < max; j++)
                {
                    if (standardCells[j].noOfPiece > standardCells[i].noOfPiece && standardCells[j].currentColor != player.color) break;
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color && gap > player.pieces[i].toWin)
                    {
                        hunt = j;
                    }
                }
            }else{
                for (int j = player.pieces[i].position; j < max; j++)
                {
                    if (standardCells[j].noOfPiece > standardCells[i].noOfPiece && standardCells[j].currentColor != player.color) break;
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color && gap > player.pieces[i].toWin)
                    {
                        hunt = j;
                    }
                }
            }
        }
    }
}

int nearest_hunt(Player player, int max){
    int single = get_nearest_hunt_for_single(player,max);
    int block = get_nearest_hunt_for_block(player,max);

    int hunt;
    

}

void move_to_x(Player player)
{
    if (player.piecesInBase > 0)
    {
        int index = 4 - player.piecesInBase;
        int position = START_POINTS[player.color];
        int direction = coin_toss();

        player.pieces[index].position = position;
        standardCells[position].noOfPiece++;
        standardCells[position].currentColor = player.color;
        player.pieces[index].status = ONTRACK;
        player.pieces[index].direction = direction;
        player.piecesInBase--;

        printf("\n%s player moves piece %s%d to the starting point. \n", colorNames[player.color], colorNames[player.color][0], index + 1);
        printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", colorNames[player.color], index + 1, player.piecesInBase);
    }
}

void standard_move(Piece piece, int value, int pieceNo)
{
    PlayerColor color = standardCells[piece.position].currentColor;
    int oldPostion = piece.position;
    int newPosition;
    if (piece.direction == clock)
        newPosition = oldPostion + value;
    else
        newPosition = oldPostion - value;

    piece.toWin -= value;
    standardCells[oldPostion].noOfPiece--;
    standardCells[oldPostion].currentColor = DEFAULT;
    piece.position = newPosition;
    standardCells[piece.position].currentColor = color;

    printf("\n%s moves piece %d from location %d to %d by %d units in %s direction.\n", colorNames[color], pieceNo, oldPostion, newPosition, direction[piece.direction]);
}

void blocking_move(Piece piece, int value, int pieceNo)
{
}
