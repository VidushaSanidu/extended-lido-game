#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

// constant array data
const int START_POINTS[PLAYERS] = START_POINTS_V;             // Starting positions for Y, B, R, G
const int APPROACH_POSITIONS[PLAYERS] = APPROACH_POSITIONS_V; // Last cell before home straight for Y, B, R, G

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

// Standard path array

Cell standardCells[STANDARD_CELLS];

// PRE GAME FUNCTIONS //

// render welcome and player choose
void pre_message()
{
    printf("\n-------Welcome to Ludo CS-------\n\n");
    for (int i = 0; i < PLAYERS; i++)
    {
        printf("The %s player has four (04) pieces named %c1, %c2, %c3, and %c4.\n", colorNames[i], colorNames[i][0], colorNames[i][0], colorNames[i][0], colorNames[i][0]);
    }
}

// initializing players data
void initialize_players(int *current)
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

    initialize_queue();
    *current = maxIndex;

    printf("The order of a single round is %s, %s, %s, and %s.\n", colorNames[maxIndex], colorNames[(maxIndex + 1) % 4], colorNames[(maxIndex + 2) % 4], colorNames[(maxIndex + 3) % 4]);
}

void initialize_queue()
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
            players[i].pieces[j].auraType = NORMAL;
            players[i].pieces[j].capturedPieces = 0;
            players[i].pieces[j].straightCount = 0;
            players[i].pieces[j].approchCount = 0;
            players[i].pieces[j].isBlocked = false;
        }
    }
}

// initialize board data
void initialize_board()
{
    standardCells[BHAWANA_I].type = BHAWANA;
    standardCells[KOTUWA_I].type = KOTUWA;
    standardCells[PITA_KOTUWA_I].type = PITA_KOTUWA;
    for (int i = 0; i < STANDARD_CELLS; i++)
    {
        standardCells[i].currentColor = DEFAULT;
        standardCells[i].noOfPiece = 0;
    }
}

// IN GAME FUNCTIONS //

// handle movement when no blocks in front => win | land on mystery
void single_move(Player *player, int pieceIndex, int roll)
{
    Piece *piece = &player->pieces[pieceIndex];
    int newPosition;
    if (piece->direction == clock)
    {
        newPosition = (piece->position + roll);
        for (int i = piece->position; i <= newPosition; i++)
        {
            if (APPROACH_POSITIONS[player->color] == i)
            {
                int gap = HOME_STRAIGHT_CELLS - abs(newPosition - i);
                if (piece->capturedPieces > 0 || gap <= 0)
                {
                    piece->status = HOME;
                    player->piecesInHome++;
                    return;
                }
                piece->straightCount = gap;
                piece->status = STRAIGHT;
                return;
            }
        }
    }
    else
    {
        newPosition = (piece->position - roll);
        for (int i = piece->position; i >= newPosition; i--)
        {
            if (APPROACH_POSITIONS[player->color] == i)
            {
                piece->approchCount++;
                if (piece->approchCount > 1)
                {
                    int gap = HOME_STRAIGHT_CELLS - abs(i - newPosition);

                    if (piece->capturedPieces > 0 || gap <= 0)
                    {
                        piece->status = HOME;
                        player->piecesInHome++;
                        return;
                    }
                    piece->straightCount = gap;
                    piece->status = STRAIGHT;
                    return;
                }
            }
        }
    }
    piece->position = newPosition % STANDARD_CELLS;
    if (standardCells[piece->position].type == MYSTERY)
    {
        int option = roll_dice();
        handle_mystery(player, pieceIndex, option);
    }
}

void block_move(Player *player, int start, int roll)
{
    int newPosition;
    for (int i = 0; i < PIECES; i++)
    {
        if (&player->pieces[i].position == start)
        {
            Piece piece = player->pieces[i];
            if (&player->pieces[i].blockDirection == clock)
            {
                newPosition = (piece.position + roll);
                for (int j = start; j <= newPosition; j++)
                {
                    if (APPROACH_POSITIONS[player->color] == i)
                    {
                        int gap = HOME_STRAIGHT_CELLS - abs(newPosition - i);
                        if (piece.capturedPieces > 0 || gap <= 0)
                        {
                            piece.status = HOME;
                            player->piecesInHome++;
                            return;
                        }
                        piece.straightCount = gap;
                        piece.status = STRAIGHT;
                        return;
                    }
                }
            }
            else
            {
                newPosition = (piece.position  - roll);
                for (int j = piece.position; j >= newPosition; j--)
                {
                    if (APPROACH_POSITIONS[player->color] == j)
                    {
                        piece.approchCount++;
                        if (piece.approchCount > 1)
                        {
                            int gap = HOME_STRAIGHT_CELLS - abs(j - newPosition);

                            if (piece.capturedPieces > 0 || gap <= 0)
                            {
                                piece.status = HOME;
                                player->piecesInHome++;
                                return;
                            }
                            piece.straightCount = gap;
                            piece.status = STRAIGHT;
                            return;
                        }
                    }
                }
            }
            piece.position = newPosition % STANDARD_CELLS;
            if (standardCells[piece.position].type == MYSTERY)
            {
                int option = roll_dice();
                handle_mystery(player, i , option);
            }
        }   
    }    
}

// handle when land on mystery cell
void handle_mystery(Player *player, int pieceIndex, int option)
{
    Piece *piece = &player->pieces[pieceIndex];
    printf("\n%s player lands on a mystery\n", colorNames[player->color]);

    switch (option)
    {
    case 1:
        printf("%s piece %d teleported to Bhawana.\n", colorNames[player->color], pieceIndex + 1);
        piece->position = BHAWANA_I;

        int aura = coin_toss();
        if (aura == true)
        {
            printf("%s piece %d  feels energized, and movement speed doubles.\n", colorNames[player->color], pieceIndex + 1);
            piece->auraDuration = 4;
            piece->auraType = POWERED;
        }
        else
        {
            printf("%s piece %d  feels sick, and movement speed halves..\n", colorNames[player->color], pieceIndex + 1);
            piece->auraDuration = 4;
            piece->auraType = SICK;
        }

        break;

    case 2:
        printf("%s piece %d teleported to Kotuwa.\n", colorNames[player->color], pieceIndex + 1);
        piece->position = KOTUWA_I;

        printf("%s piece %d attends briefing and cannot move for four rounds.\n", colorNames[player->color], pieceIndex + 1);
        piece->auraDuration = 4;
        piece->auraType = FREEZE;

        break;

    case 3:
        printf("%s piece %d teleported to Pita-Kotuwa.\n", colorNames[player->color], pieceIndex + 1);
        piece->position = PITA_KOTUWA_I;

        if (piece->direction == clock)
        {
            printf("%s piece %d which was moving clockwise, has changed to moving coun- terclockwise.\n", colorNames[player->color], pieceIndex + 1);
            piece->direction = antiClock;
        }
        else
        {
            printf("%s piece %d is moving in a counterclockwise direction. Teleporting to Kotuwa from Pita-Kotuwa.\n", colorNames[player->color], pieceIndex + 1);
            handle_mystery(player, pieceIndex, 2);
        }

        break;

    case 4:
        printf("%s piece %d teleported to Base.\n", colorNames[player->color], pieceIndex + 1);
        reset_piece(*piece);

        break;

    case 5:
        printf("%s piece %d teleported to startPoint.\n", colorNames[player->color], pieceIndex + 1);
        piece->position = START_POINTS[player->color];

        break;

    case 6:
        printf("%s piece %d teleported to Approch cell.\n", colorNames[player->color], pieceIndex + 1);
        piece->position = APPROACH_POSITIONS[player->color];

        break;

    default:
        break;
    }
}

// find the gap between peice and win
int find_gap(Piece piece)
{
    PlayerColor col = standardCells[piece.position].currentColor;
    if (piece.direction == clock)
        return (APPROACH_POSITIONS[col] - piece.position + STANDARD_CELLS) % STANDARD_CELLS;
    else
        return (piece.position - APPROACH_POSITIONS[col] + STANDARD_CELLS) % STANDARD_CELLS;
}

// find nearest single hut
HuntResult get_nearest_hunt_for_single(Player player, int max)
{
    HuntResult result;
    result.huntIndex = -1;
    int gap = 60;
    for (int i = 0; i < PIECES; i++)
    {
        if (player.pieces[i].status == ONTRACK)
        {
            if (player.pieces[i].direction == clock)
            {
                for (int j = player.pieces[i].position + 1; j <= max; j++)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                        break;
                    if (standardCells[j].noOfPiece == 1 && standardCells[j].currentColor != player.color && gap > find_gap(player.pieces[i]))
                    {
                        result.huntIndex = j;
                        result.pieceNo = i;
                    }
                }
            }
            else
            {
                for (int j = player.pieces[i].position - 1; j >= max; j--)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                        break;
                    if (standardCells[j].noOfPiece == 1 && standardCells[j].currentColor != player.color && gap > find_gap(player.pieces[i]))
                    {
                        result.huntIndex = j;
                        result.pieceNo = i;
                    }
                }
            }
        }
    }
    return result;
}

// find nearest block hunt
HuntResult get_nearest_hunt_for_block(Player player, int max)
{
    HuntResult result;
    result.huntIndex = -1;
    int gap = 60;

    for (int i = 0; i < PIECES; i++)
    {
        if (player.pieces[i].status == ONTRACK)
          continue;
        
        int noOfPeices = standardCells[player.pieces[i].position].noOfPiece ;
        max = max / noOfPeices;
        if ( noOfPeices > 1)
        {
            if (player.pieces[i].blockDirection == clock)
            {
                for (int j = player.pieces[i].position + 1; j < max; j++)
                {
                    if (standardCells[j].noOfPiece > noOfPeices && standardCells[j].currentColor != player.color)
                        break;
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color && gap > find_gap(player.pieces[i]))
                    {
                        result.huntIndex = j;
                        result.pieceNo = i;
                    }
                }
            }
            else
            {
                for (int j = player.pieces[i].position -1 ; j >= max; j--)
                {
                    if (standardCells[j].noOfPiece > noOfPeices && standardCells[j].currentColor != player.color)
                        break;
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color && gap > find_gap(player.pieces[i]))
                    {
                        result.huntIndex = j;
                        result.pieceNo = i;
                    }
                }
            }
        }
    }
    return result;
}

// calculate the nearest hunt
HuntResult nearest_hunt(Player player, int max)
{
    HuntResult single = get_nearest_hunt_for_single(player, max);
    HuntResult block = get_nearest_hunt_for_block(player, max / 2);

    HuntResult final;
    if (find_gap(player.pieces[single.pieceIndex]) > find_gap(player.pieces[block.pieceIndex]))
        return block;
    else
        return single;
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

    printf("\n%s player moves piece %c%d to the starting point. \n", colorNames[player.color], colorNames[player.color][0], index + 1);
    printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", colorNames[player.color], index + 1, player.piecesInBase);
}

BlockedResult find_non_blockable_piece(Player player, int max)
{
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
                if (standardCells[player.pieces[i].position + max].noOfPiece == 0 && player.pieces[i].toWin < gap && isBlocked == false)
                {
                    data.result = player.pieces[i];
                }
            }
            else
            {
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
                if (standardCells[player.pieces[i].position - max].noOfPiece == 0 && player.pieces[i].toWin < gap && isBlocked == false)
                {
                    data.result = player.pieces[i];
                }
            }
        }
    }
    if (data.result.position != -1)
    {
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

    standardCells[oldPostion].noOfPiece--;
    standardCells[oldPostion].currentColor = DEFAULT;
    piece.position = newPosition;
    standardCells[piece.position].currentColor = color;

    printf("\n%s moves piece %d from location %d to %d by %d units in %s direction.\n", colorNames[color], pieceNo, oldPostion, newPosition, value, direction[piece.direction]);
}

void capturing_move(Player players[], int cUser, HuntResult hunt,int roll)
{
    Player target = players[standardCells[hunt.hunt].currentColor];

    for (int i = 0; i < PIECES; i++)
    {
        if (target.pieces[i].position == hunt.hunt)
        {
            reset_piece(target.pieces[i]);
            target.piecesInBase++;
        }
    }
    int oldPos = players[cUser].pieces[hunt.pieceIndex].position;
    int onBoard = get_board_count(players[cUser]);

    if (standardCells[oldPos].noOfPiece == 1){                              // 712
        move(&players[cUser],hunt.pieceIndex,roll);
        players[cUser].pieces[hunt.pieceIndex].capturedPieces ++;

    }else {

    }

    for (int i = 0; i < PIECES; i++)
    {
        if (players[cUser].pieces[i].position == oldPos)
        {
            move(players[cUser], i, );
            players[cUser].pieces[i].capturedPieces++;
        }
    }
    standardCells[hunt.hunt].currentColor = standardCells[oldPos].currentColor;
    standardCells[hunt.hunt].noOfPiece = standardCells[oldPos].noOfPiece;
    standardCells[oldPos].currentColor = DEFAULT;
    standardCells[oldPos].noOfPiece = 0;

    printf("\n%s pieces lands on square %d, captures %s, and returns it to the base.\n", colorNames[cUser], hunt.hunt, colorNames[target.color]);
    printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", colorNames[cUser], onBoard, players[cUser].piecesInBase);
}

void reset_piece(Piece piece)
{
    piece.position = -1;
    piece.approchCount = 0;
    piece.auraDuration = 0;
    piece.auraType = DEFAULT;
    piece.blockDirection = 0;
    piece.capturedPieces = 0;
    piece.isBlocked = false;
    piece.status = BASE;
}

int get_board_count(Player player)
{
    return 4 - (player.piecesInBase + player.piecesInHome);
}

void blocking_move(Piece piece, int value, int pieceNo)
{
}
