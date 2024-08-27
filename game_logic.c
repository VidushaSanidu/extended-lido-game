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
    else if (num == 2)
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

void rotateCount(int *current)
{
    *current = (*current + 1) % PLAYERS;
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
    Piece *piece = &players[index].pieces[pieceIndex];
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
    piece->position = (newPosition + STANDARD_CELLS) % STANDARD_CELLS;
    piece->blockDirection = -1;
    if (standardCells[piece->position].type == MYSTERY)
    {
        handle_mystery(players[index].color, pieceIndex, roll_dice());
    }
    else
    {
        standardCells[piece->position].noOfPiece++;
        standardCells[piece->position].currentColor = get_color(index);
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
            Piece *piece = &players[index].pieces[i];
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
            piece->position = (newPosition + STANDARD_CELLS) % STANDARD_CELLS;
            standardCells[piece->position].noOfPiece++;
            standardCells[piece->position].currentColor = get_color(index);
        }
    }
}

// handle when land on mystery cell
void handle_mystery(PlayerColor index, int pieceIndex, int option)
{

    Piece *piece = &players[index].pieces[pieceIndex];
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
        standardCells[piece->position].currentColor = get_color(index);

        if (coin_toss() == true)
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
        standardCells[piece->position].currentColor = get_color(index);

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
            standardCells[piece->position].currentColor = get_color(index);
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
        standardCells[piece->position].currentColor = get_color(index);

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
        standardCells[piece->position].currentColor = get_color(index);

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

// find nearest single hunt
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
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
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
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
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
                    if (standardCells[j].noOfPiece > noOfPeices && standardCells[j].currentColor != player.color)
                    {
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
                    if (standardCells[j].noOfPiece > noOfPeices && standardCells[j].currentColor != player.color)
                    {
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

// find non blockabale free slot for single piece
BlockedResult find_non_blockable_single(Player player, int max)
{
    int gap = 60;
    BlockedResult data;
    data.pieceNo = -1;
    data.count = -1;

    for (int i = 0; i < PIECES; i++)
    {
        if (player.pieces[i].status == ONTRACK)
        {
            int isBlocked = false;
            int count = 0;
            if (player.pieces[i].direction == clock)
            {
                for (int j = player.pieces[i].position + 1; j <= max; j++)
                {
                    count++;
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
                        isBlocked = true;
                        count--;
                        break;
                    }
                }
            }
            else
            {
                for (int j = player.pieces[i].position - 1; j >= max; j--)
                {
                    count++;
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
                        isBlocked = true;
                        count--;
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
            else if (newGap < gap && isBlocked == true && count > 0)
            {
                gap = newGap;
                data.pieceNo = i;
                data.count = count;
            }
        }
    }
    return data;
}

// find non blockabale free slot for piece block
BlockedResult find_non_blockable_block(Player player, int max)
{
    int gap = 60;
    BlockedResult data;
    data.pieceNo = -1;
    data.count = -1;

    for (int i = 0; i < PIECES; i++)
    {
        Piece piece = player.pieces[i];
        if (piece.status != ONTRACK)
            continue;

        int count = standardCells[piece.position].noOfPiece;
        if (count > 1)
        {
            int countt = 0;
            int isBlocked = false;
            if (piece.blockDirection == clock)
            {
                for (int j = piece.position + 1; j <= max / count; j++)
                {
                    countt++;
                    if (standardCells[j].noOfPiece > count && standardCells[j].currentColor != player.color)
                    {
                        isBlocked = true;
                        countt--;
                        break;
                    }
                }
            }
            else
            {
                countt++;
                for (int j = piece.position - 1; j >= max / count; j--)
                {
                    if (standardCells[j].noOfPiece > count && standardCells[j].currentColor != player.color)
                    {
                        isBlocked = true;
                        countt--;
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
            else if (newGap < gap && isBlocked == true && countt > 0)
            {
                gap = newGap;
                data.pieceNo = i;
                data.count = countt * count;
            }
        }
    }
    return data;
}

// find a way to create a block
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
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
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
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
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

// find any mystory box first
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
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
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
                    if (standardCells[j].noOfPiece > 1 && standardCells[j].currentColor != player.color)
                    {
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

// movement functions //
void move_to_x(int cUser)
{
    Player player = players[cUser];
    int index = 4 - player.piecesInBase;
    int position = START_POINTS[player.color];

    players[cUser].pieces[index].position = position;
    standardCells[position].noOfPiece++;
    standardCells[position].currentColor = player.color;
    players[cUser].pieces[index].status = ONTRACK;
    players[cUser].pieces[index].direction = coin_toss() ;
    players[cUser].piecesInBase -- ;

    printf("\n%s player moves piece %c%d to the starting point. \n", colorNames[player.color], colorNames[player.color][0], index + 1);
    printf("%s player now has %d/4 pieces on the board and %d/4 pieces on the base.\n", colorNames[player.color], index + 1, players[cUser].piecesInBase);
}

void standard_single_move(int cUser, int pieceNo, int roll)
{
    int oldPostion = players[cUser].pieces[pieceNo].position;
    single_move(cUser, pieceNo, roll);
    int newPosition = players[cUser].pieces[pieceNo].position;

    printf("\n%s moves piece %d from location %d to %d by %d units in %s direction.\n", colorNames[cUser], pieceNo +1, oldPostion, newPosition, roll, direction[players[cUser].pieces[pieceNo].direction - 1]);
}

void standard_block_move(int cUser, int pieceNo, int roll)
{
    int oldPostion = players[cUser].pieces[pieceNo].position;
    block_move(cUser, pieceNo, roll);
    int newPosition = players[cUser].pieces[pieceNo].position;

    printf("\n%s moves pieces from location %d to %d in %s direction.\n", colorNames[cUser], oldPostion, newPosition, direction[players[cUser].pieces[pieceNo].blockDirection -1]);
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
    int onBoard = get_board_count(players[target.color]);

    single_move(cUser, hunt.pieceNo, roll);
    players[cUser].pieces[hunt.pieceNo].capturedPieces++;

    printf("\n%s pieces lands on square %d, captures %s, and returns it to the base.\n", colorNames[cUser], hunt.huntIndex, colorNames[target.color]);
    printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", colorNames[target.color], onBoard, players[target.color].piecesInBase);
}

void block_capturing_move(int cUser, HuntResult hunt, int roll)
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

    block_move(cUser, hunt.pieceNo, roll);

    for (int i = 0; i < PIECES; i++)
    {
        if (players[cUser].pieces[i].position == hunt.huntIndex)
        {
            players[cUser].pieces[i].capturedPieces++;
        }
    }

    printf("\n%s pieces lands on square %d, captures %s, and returns it to the base.\n", colorNames[cUser], hunt.huntIndex, colorNames[target.color]);
    printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", colorNames[cUser], onBoard, players[cUser].piecesInBase);
}

void blockable_single_move(int cUser, int pieceNo, int count)
{

    int oldPostion = players[cUser].pieces[pieceNo].position;
    single_move(cUser, pieceNo, count);
    int newPosition = players[cUser].pieces[pieceNo].position;

    printf("\n%s does not have other pieces in the board to move instead of the blocked piece. Moved the piece which is the cell before the block.\n", colorNames[cUser]);
}

void blockable_block_move(int cUser, int pieceNo, int count)
{
    int oldPostion = players[cUser].pieces[pieceNo].position;
    block_move(cUser, pieceNo, count * 2);
    int newPosition = players[cUser].pieces[pieceNo].position;

    printf("\n%s does not have other pieces in the board to move instead of the blocked piece. Moved the piece which is the cell before the block.\n", colorNames[cUser]);
}

void home_straight_move(int cUser, int dice)
{
    for (int i = 0; i < PIECES; i++)
    {
        if (players[cUser].pieces[i].status == STRAIGHT && players[cUser].pieces[i].straightCount == dice)
        {
            players[cUser].pieces[i].status = HOME;
            players[cUser].piecesInHome++;
            printf("\n%s piece %d reach to HOME!\n", colorNames[cUser], i);
            continue;
        }
    }
}

void cantMove(int cUser)
{
    printf("\n%s does not have other pieces in the board to move instead of the blocked piece. Ignoring the throw and moving on to the next player\n", colorNames[cUser]);
}

void find_blocks_move(int cUser, int pieceNo, int value, int roll)
{
    int oldPostion = players[cUser].pieces[pieceNo].position;
    int direction;
    for (int i = 0; i < PIECES; i++)
    {
        if (players[cUser].pieces[i].position == value)
        {
            direction = players[cUser].pieces[i].blockDirection;
            break;
        }
    }

    single_move(cUser, pieceNo, roll);
    int newPosition = players[cUser].pieces[pieceNo].position;
    players[cUser].pieces[pieceNo].blockDirection = direction;

    printf("\n%spiece %d is blocked from moving from %d to %d.\n", colorNames[cUser], pieceNo, oldPostion, newPosition);
}

void find_boxs_move(int cUser, int pieceNo, int value, int roll)
{
    single_move(cUser, pieceNo, roll);
}

// other //

void reset_piece(PlayerColor color, int peiceNo)
{
    players[color].pieces[peiceNo].position = -1;
    players[color].pieces[peiceNo].approchCount = 0;
    players[color].pieces[peiceNo].auraDuration = 0;
    players[color].pieces[peiceNo].auraType = NORMAL;
    players[color].pieces[peiceNo].blockDirection = 0;
    players[color].pieces[peiceNo].capturedPieces = 0;
    players[color].pieces[peiceNo].status = BASE;
}

int get_board_count(Player player)
{
    return 4 - (player.piecesInBase + player.piecesInHome);
}

// print status
void print_status(int mstry)
{
    for (int i = 0; i < PLAYERS; i++)
    {
        PlayerColor col = players[i].color;
        printf("\n%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", colorNames[col], get_board_count(players[i]), players[i].piecesInBase);
        printf("============================\n");
        printf("Location of pieces %s\n", colorNames[col]);
        printf("============================\n");
        for (int j = 0; j < PIECES; j++)
        {
            Piece p = players[i].pieces[j];
            if (p.status == BASE)
                printf("Piece %d > Base.\n", j);
            else if (p.status == HOME)
                printf("Piece %d > Home.\n", j);
            else if (p.status == STRAIGHT)
                printf("Piece %d > Home Staright.\n", j);
            else if (p.status == ONTRACK)
            {
                printf("Piece %d > %d.\n", j, p.position);
            }
        }
    }
    printf("The mystery cell is at %d\n", mstry);
    printf("============================\n");
}

void print_winner(int current)
{
    printf("\n%s player wins!!!\n\n", colorNames[current]);
}

// generate mystory
void generate_mystory(int *mystry)
{
    if (standardCells[*mystry].type == MYSTERY)
        standardCells[*mystry].type = BASIC;

    int random = (rand() % 51) + 1;
    if (standardCells[random].noOfPiece > 0)
        generate_mystory(mystry);

    *mystry = random;
    standardCells[random].type = MYSTERY;
}
