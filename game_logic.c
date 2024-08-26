#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

// constant array data
const int START_POINTS[PLAYERS] = START_POINTS_V;             //  Y, B, R, G
const int APPROACH_POSITIONS[PLAYERS] = APPROACH_POSITIONS_V; //  Y, B, R, G

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
            players[i].pieces[j].blockDirection = -1;
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
void single_move(PlayerColor index, int pieceIndex, int roll)
{
    Piece *piece = players[index].pieces[pieceIndex];
    int newPosition;
    int old = piece->position;
    standardCells[old].noOfPiece--;
    standardCells[old].currentColor = DEFAULT;

    if (piece->direction == clock)
    {
        newPosition = (piece->position + roll);
        for (int i = piece->position; i <= newPosition; i++)
        {
            if (APPROACH_POSITIONS[players[index].color] == i)
            {
                int gap = HOME_STRAIGHT_CELLS - abs(newPosition - i);
                if (piece->capturedPieces > 0 || gap <= 0)
                {
                    piece->status = HOME;
                    players[index].piecesInHome++;
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
            if (APPROACH_POSITIONS[players[index].color] == i)
            {
                piece->approchCount++;
                if (piece->approchCount > 1)
                {
                    int gap = HOME_STRAIGHT_CELLS - abs(i - newPosition);

                    if (piece->capturedPieces > 0 || gap <= 0)
                    {
                        piece->status = HOME;
                        players[index].piecesInHome++;
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
    piece->blockDirection = -1;
    if (standardCells[piece->position].type == MYSTERY)
    {
        int option = roll_dice();
        handle_mystery(players[index].color, pieceIndex, option);
    }
    else
    {
        standardCells[piece->position].noOfPiece++;
        standardCells[piece->position].currentColor = colorNames[index];
    }
}

void block_move(PlayerColor index, int pieceIndex, int roll)
{
    int newPosition;
    int pos = players[index].pieces[pieceIndex].position;
    int count = standardCells[pos].noOfPiece;

    standardCells[pos].noOfPiece = 0;
    standardCells[pos].currentColor = DEFAULT;

    for (int i = 0; i < PIECES; i++)
    {
        if (players[index].pieces[i].position == pos)
        {
            Piece *piece = players[index].pieces[i];
            if (piece->blockDirection == clock)
            {
                newPosition = (pos + roll / count);
                for (int j = pos; j <= newPosition; j++)
                {
                    if (APPROACH_POSITIONS[index] == i)
                    {
                        piece->status = HOME;
                        players[index].piecesInHome++;
                        return;
                    }
                }
            }
            else
            {
                newPosition = (pos - roll / count);
                for (int j = pos; j >= newPosition; j--)
                {
                    if (APPROACH_POSITIONS[index] == i)
                    {
                        piece->status = HOME;
                        players[index].piecesInHome++;
                        return;
                    }
                }
            }
            piece->position = newPosition % STANDARD_CELLS;
            standardCells[piece->position].noOfPiece++;
            standardCells[piece->position].currentColor = colorNames[index];
        }
    }
}

// handle when land on mystery cell
void handle_mystery(PlayerColor index, int pieceIndex, int option)
{

    Piece *piece = players[index].pieces[pieceIndex];
    printf("\n%s player lands on a mystery\n", colorNames[index]);

    switch (option)
    {
    case 1:
        if (standardCells[BHAWANA_I].noOfPiece > 0)
        {
            printf("Bhawana is occupied.\n");
            handle_mystery(index, pieceIndex, roll_dice());
        }

        printf("%s piece %d teleported to Bhawana.\n", colorNames[index], pieceIndex + 1);
        piece->position = BHAWANA_I;
        standardCells[piece->position].noOfPiece++;
        standardCells[piece->position].currentColor = colorNames[index];

        int aura = coin_toss();
        if (aura == true)
        {
            printf("%s piece %d  feels energized, and movement speed doubles.\n", colorNames[index], pieceIndex + 1);
            piece->auraDuration = 4;
            piece->auraType = POWERED;
        }
        else
        {
            printf("%s piece %d  feels sick, and movement speed halves..\n", colorNames[index], pieceIndex + 1);
            piece->auraDuration = 4;
            piece->auraType = SICK;
        }

        break;

    case 2:
        if (standardCells[KOTUWA_I].noOfPiece > 0)
        {
            printf("Kotuwa is occupied.\n");
            handle_mystery(index, pieceIndex, roll_dice());
        }
        printf("%s piece %d teleported to Kotuwa.\n", colorNames[index], pieceIndex + 1);
        piece->position = KOTUWA_I;
        standardCells[piece->position].noOfPiece++;
        standardCells[piece->position].currentColor = colorNames[index];

        printf("%s piece %d attends briefing and cannot move for four rounds.\n", colorNames[index], pieceIndex + 1);
        piece->auraDuration = 4;
        piece->auraType = FREEZE;

        break;

    case 3:
        printf("%s piece %d teleported to Pita-Kotuwa.\n", colorNames[index], pieceIndex + 1);
        piece->position = PITA_KOTUWA_I;

        if (piece->direction == clock)
        {
            if (standardCells[PITA_KOTUWA_I].noOfPiece > 0)
            {
                printf("Pita-Kotuwa is occupied.\n");
                handle_mystery(index, pieceIndex, roll_dice());
            }
            printf("%s piece %d which was moving clockwise, has changed to moving coun- terclockwise.\n", colorNames[index], pieceIndex + 1);
            piece->direction = antiClock;
            standardCells[piece->position].noOfPiece++;
            standardCells[piece->position].currentColor = colorNames[index];
        }
        else
        {
            printf("%s piece %d is moving in a counterclockwise direction. Teleporting to Kotuwa from Pita-Kotuwa.\n", colorNames[index], pieceIndex + 1);
            handle_mystery(index, pieceIndex, 2);
        }

        break;

    case 4:
        printf("%s piece %d teleported to Base.\n", colorNames[index], pieceIndex + 1);
        reset_piece(index, pieceIndex);

        break;

    case 5:
        if (standardCells[START_POINTS[index]].noOfPiece > 0)
        {
            printf("Startpoint is occupied.\n");
            handle_mystery(index, pieceIndex, roll_dice());
        }
        printf("%s piece %d teleported to startPoint.\n", colorNames[index], pieceIndex + 1);
        piece->position = START_POINTS[index];
        standardCells[piece->position].noOfPiece++;
        standardCells[piece->position].currentColor = colorNames[index];

        break;

    case 6:
        if (standardCells[APPROACH_POSITIONS[index]].noOfPiece > 0)
        {
            printf("Approch cell is occupied.\n");
            handle_mystery(index, pieceIndex, roll_dice());
        }
        printf("%s piece %d teleported to Approch cell.\n", colorNames[index], pieceIndex + 1);
        piece->position = APPROACH_POSITIONS[index];
        standardCells[piece->position].noOfPiece++;
        standardCells[piece->position].currentColor = colorNames[index];

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
            int isBlocked = false;
            int target;
            if (player.pieces[i].direction == clock)
            {
                target = player.pieces[i].position + max;
                for (int j = player.pieces[i].position + 1; j <= max; j++)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color){
                        isBlocked = true;
                        break;
                    }
                } 
            }
            else
            {
                target = player.pieces[i].position - max;
                for (int j = player.pieces[i].position - 1; j >= max; j--)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color){
                        isBlocked = true;
                        break;
                    }
                } 
                
            }
            if (isBlocked == false && standardCells[target].noOfPiece == 1 && standardCells[target].currentColor != player.color && gap > find_gap(player.pieces[i]))
            {
                gap = find_gap(player.pieces[i]);
                result.huntIndex = target;
                result.pieceNo = i;
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
        if (player.pieces[i].status != ONTRACK)
            continue;

        int noOfPeices = standardCells[player.pieces[i].position].noOfPiece;
        max = max / noOfPeices;
        if (noOfPeices > 1)
        {
            int isBlocked = false;
            int target;
            if (player.pieces[i].blockDirection == clock)
            {
                target = player.pieces[i].position + max;
                for (int j = player.pieces[i].position + 1; j <= max; j++)
                {
                    if (standardCells[j].noOfPiece > noOfPeices && standardCells[j].currentColor != player.color){
                        isBlocked = true;
                        break;
                    }
                } 
            }
            else
            {
                target = player.pieces[i].position - max;
                for (int j = player.pieces[i].position - 1; j >= max; j--)
                {
                    if (standardCells[j].noOfPiece > noOfPeices && standardCells[j].currentColor != player.color){
                        isBlocked = true;
                        break;
                    }
                } 
                
            }
            if (isBlocked == false && standardCells[target].noOfPiece <= noOfPeices && standardCells[target].currentColor != player.color && gap > find_gap(player.pieces[i]))
            {
                gap = find_gap(player.pieces[i]);
                result.huntIndex = target;
                result.pieceNo = i;
            }
        }
    }
    return result;
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

BlockedResult find_non_blockable_single(Player player, int max)
{
    int gap = 60;
    BlockedResult data;
    data.pieceNo = -1;
    data.prevIndex = -1;
    int prev = -1;

    for (int i = 0; i < PIECES; i++)
    {
        if (player.pieces[i].status == ONTRACK)
        {
            int isBlocked = false;
            if (player.pieces[i].direction == clock)
            {
                for (int j = player.pieces[i].position + 1; j <= max; j++)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
                        isBlocked = true;
                        prev = j - 1;
                        break;
                    }
                }
            }
            else
            {
                for (int j = player.pieces[i].position - 1; j >= max; j--)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
                        isBlocked = true;
                        prev = j + 1;
                        break;
                    }
                }
            }
            int newGap = find_gap(player.pieces[i]);
            if (newGap < gap && isBlocked == false)
            {
                gap = newGap;
                data.pieceNo = i;
            }
            else if (newGap < gap && isBlocked == true && prev != player.pieces[i].position )
            {
                gap = newGap;
                data.pieceNo = i;
                data.prevIndex = prev;
            }
        }
    }
    return data;
}

BlockedResult find_non_blockable_block(Player player, int max)
{
    int gap = 60;
    BlockedResult data;
    data.pieceNo = -1;
    data.prevIndex = -1;
    int prev = -1;

    for (int i = 0; i < PIECES; i++)
    {
        Piece piece = player.pieces[i];
        if (piece.status != ONTRACK)
            continue;

        int count = standardCells[piece.position].noOfPiece; 
        if ( count > 1)
        {
            int isBlocked = false;
            if (piece.blockDirection == clock)
            {
                for (int j = piece.position + 1; j <= max/count; j++)
                {
                    if (standardCells[j].noOfPiece > count && standardCells[j].currentColor != player.color)
                    {
                        isBlocked = true;
                        prev = j - 1;
                        break;
                    }
                }
            }
            else
            {
                for (int j = piece.position - 1; j >= max/count; j--)
                {
                    if (standardCells[j].noOfPiece > count && standardCells[j].currentColor != player.color)
                    {
                        isBlocked = true;
                        prev = j + 1;
                        break;
                    }
                }
            }
            int newGap = find_gap(piece);
            if (newGap < gap && isBlocked == false)
            {
                gap = newGap;
                data.pieceNo = i;
            }
            else if (newGap < gap && isBlocked == true && prev != piece.position )
            {
                gap = newGap;
                data.pieceNo = i;
                data.prevIndex = prev;
            }
        }
    }
    return data;
}

FindBlockResult find_blocks(Player player, int max)
{
    FindBlockResult data;
    data.pieceNo = -1;
    data.blockIndex = -1;

    for (int i = 0; i < PIECES; i++)
    {
        Piece piece = player.pieces[i];
        if (piece.status == ONTRACK)
        {
            int isBlocked = false;
            int target;
            if (player.pieces[i].direction == clock)
            {
                target = player.pieces[i].position + max;
                for (int j = player.pieces[i].position + 1; j <= max; j++)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color){
                        isBlocked = true;
                        break;
                    }
                } 
            }
            else
            {
                target = player.pieces[i].position - max;
                for (int j = player.pieces[i].position - 1; j >= max; j--)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color){
                        isBlocked = true;
                        break;
                    }
                } 
                
            }
            if (isBlocked == false && standardCells[target].noOfPiece >= 1 && standardCells[target].currentColor == player.color)
            {
                data.blockIndex = target;
                data.pieceNo = i;
                return data;
            }
        }
    }
    return data;
}

BoxResult find_mystory_box(Player player, int max)
{
    BoxResult data;
    data.pieceNo = -1;
    data.boxIndex = -1;

    for (int i = 0; i < PIECES; i++)
    {
        Piece piece = player.pieces[i];
        if (piece.status == ONTRACK)
        {
            int isBlocked = false;
            int target;
            if (player.pieces[i].direction == clock)
            {
                target = player.pieces[i].position + max;
                for (int j = player.pieces[i].position + 1; j <= max; j++)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color){
                        isBlocked = true;
                        break;
                    }
                } 
            }
            else
            {
                target = player.pieces[i].position - max;
                for (int j = player.pieces[i].position - 1; j >= max; j--)
                {
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color){
                        isBlocked = true;
                        break;
                    }
                } 
                
            }
            if (isBlocked == false && standardCells[target].noOfPiece == 0 && standardCells[target].type == MYSTERY)
            {
                data.boxIndex = target;
                data.pieceNo = i;
                return data;
            }
        }
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

void single_capturing_move(int cUser, HuntResult hunt, int roll)
{
    Player target = players[standardCells[hunt.huntIndex].currentColor];

    for (int i = 0; i < PIECES; i++)
    {
        if (target.pieces[i].position == hunt.huntIndex)
        {
            reset_piece(target.color, i);
            players[target.color].piecesInBase++;
        }
    }
    int oldPos = players[cUser].pieces[hunt.pieceNo].position;
    int onBoard = get_board_count(players[cUser]);

    single_move(cUser, hunt.pieceNo, roll);
    players[cUser].pieces[hunt.pieceNo].capturedPieces++;

    printf("\n%s pieces lands on square %d, captures %s, and returns it to the base.\n", colorNames[cUser], hunt.huntIndex, colorNames[target.color]);
    printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", colorNames[cUser], onBoard, players[cUser].piecesInBase);
}

void reset_piece(PlayerColor color, int peiceNo)
{
    players[color].pieces[peiceNo].position = -1;
    players[color].pieces[peiceNo].approchCount = 0;
    players[color].pieces[peiceNo].auraDuration = 0;
    players[color].pieces[peiceNo].auraType = DEFAULT;
    players[color].pieces[peiceNo].blockDirection = 0;
    players[color].pieces[peiceNo].capturedPieces = 0;
    players[color].pieces[peiceNo].isBlocked = false;
    players[color].pieces[peiceNo].status = BASE;
}

int get_board_count(Player player)
{
    return 4 - (player.piecesInBase + player.piecesInHome);
}

void blocking_move(Piece piece, int value, int pieceNo)
{
}
