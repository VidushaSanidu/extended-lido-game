#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

// constant data
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
        return RED;
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
            players[i].pieces[j].position = -1;
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

HuntResult get_nearest_hunt_for_single(Player player, int max)
{
    HuntResult result;
    result.hunt = -1;
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
                        result.hunt = j;
                        result.pieceIndex = i;
                    }
                }
            }else{
                for (int j = player.pieces[i].position; j > max; j--)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color) break;
                    if (standardCells[j].noOfPiece == 1 && standardCells[j].currentColor != player.color && gap > player.pieces[i].toWin)
                    {
                        result.hunt = j;
                        result.pieceIndex = i;
                        
                    }
                }
            }
        }
    }
    return result;
}

HuntResult get_nearest_hunt_for_block(Player player, int max)
{
    HuntResult result;
    result.hunt = -1;
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
                        result.hunt = j;
                        result.pieceIndex = i;
                        
                    }
                }
            }else{
                for (int j = player.pieces[i].position; j > max; j--)
                {
                    if (standardCells[j].noOfPiece > standardCells[i].noOfPiece && standardCells[j].currentColor != player.color) break;
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color && gap > player.pieces[i].toWin)
                    {
                        result.hunt = j;
                        result.pieceIndex = i;
                    }
                }
            }
        }
    }
    return result;
}

HuntResult nearest_hunt(Player player, int max){
    HuntResult single = get_nearest_hunt_for_single(player,max);
    HuntResult block = get_nearest_hunt_for_block(player,max/2);

    HuntResult final;
    if (player.pieces[single.pieceIndex].toWin > player.pieces[block.pieceIndex].toWin){
        final = block;
    }else {
        final = single;
    }
    return final;
}

void move_to_x(Player player)
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

BlockedResult find_non_blockable_piece(Player player,int max){
    int gap = 60;
    BlockedResult data;
    data.result.position = -1;
    int blockedPiece = -1;
    int blockedCell = -1;

    for (int i = 0; i < PIECES; i++)
    {
        if (player.pieces[i].status == ONTRACK && player.pieces[i].isBlocked == false)
        {
            int isBlocked = false;
            if (player.pieces[i].direction == clock)
            {
                for (int j = player.pieces[i].position; j < max; j++)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
                        blockedPiece = i;
                        blockedCell = j;  
                        isBlocked = true;
                        break;             
                    }

                }
                if (standardCells[player.pieces[i].position + max].noOfPiece == 0 && player.pieces[i].toWin < gap && isBlocked == false ){
                    data.result = player.pieces[i];
                }

            }else{
                for (int j = player.pieces[i].position; j > max; j--)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
                        blockedPiece = i;
                        blockedCell = j;  
                        isBlocked = true;
                        break;             
                    }

                }
                if (standardCells[player.pieces[i].position - max].noOfPiece == 0 && player.pieces[i].toWin < gap && isBlocked == false ){
                    data.result = player.pieces[i];
                }
            }
        }
    }
    if (data.result.position != -1){
        data.blockedCell = blockedCell;
        data.blockedPiece = blockedPiece;
    }
    return data;
}

void standard_move(Player player, int value, int pieceNo)
{
    Piece piece;
    PlayerColor color = player.color;
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

void capturing_move(Player players[],int cUser, HuntResult hunt){
    Player target = players[standardCells[hunt.hunt].currentColor];
    target.piecesInBase ++;

    for (int i = 0; i < PIECES; i++)
    {
        if(target.pieces[i].position == hunt.hunt){
            reset_piece(target.pieces[i]);
        }
    }
    int oldPos = players[cUser].pieces[hunt.pieceIndex].position;
    int onBoard = get_board_count(players[cUser]);

    for (int i = 0; i < PIECES; i++)
    {
        if(players[cUser].pieces[i].position == oldPos){
            players[cUser].pieces[i].position = hunt.hunt;
            players[cUser].pieces[i].capturedPieces ++;
            players[cUser].pieces[i].toWin -= abs(hunt.hunt - oldPos);
        }
    }
    standardCells[hunt.hunt].currentColor = standardCells[oldPos].currentColor;
    standardCells[hunt.hunt].noOfPiece = standardCells[oldPos].noOfPiece;
    standardCells[oldPos].currentColor = DEFAULT;
    standardCells[oldPos].noOfPiece = 0;

    printf("\n%s pieces lands on square %d, captures %s, and returns it to the base.\n",colorNames[cUser],hunt.hunt,colorNames[target.color]);
    printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n",colorNames[cUser],onBoard,players[cUser].piecesInBase);

}

void reset_piece(Piece piece){
    piece.position = -1;
    piece.approchCount = 0;
    piece.auraDuration = 0;
    piece.auraType = DEFAULT;
    piece.blockDirection = 0;
    piece.capturedPieces = 0;
    piece.isBlocked = false;
    piece.status = BASE;
    piece.toWin = 52;    
}

int get_board_count(Player player){
    return 4 - (player.piecesInBase + player.piecesInHome);
}


void blocking_move(Piece piece, int value, int pieceNo)
{
}
