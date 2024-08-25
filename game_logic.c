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
    "GREEN"
    };

PlayerColor getColor(int num){
    if (num == 0) return YELLOW;
    else if (num == 1) return BLUE;
    else if (num == 1) return BLUE;
    else return GREEN;
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

void initialize_players(PlayerQueue *queue)
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

    initializeQueue(queue);
    queue->current = maxIndex;

    printf("The order of a single round is %s, %s, %s, and %s.\n", colorNames[maxIndex], colorNames[(maxIndex + 1) % 4], colorNames[(maxIndex + 2) % 4], colorNames[(maxIndex + 3) % 4]);
}

void initialize_board()
{
    for (int i = 0; i < PLAYERS; i++)
    {
        standardCells[START_POINTS[i]].type = START;
        standardCells[START_POINTS[i]].relatedColor = getColor(i);
        standardCells[APPROACH_POSITIONS[i]].type = APPROCH;
        standardCells[APPROACH_POSITIONS[i]].relatedColor = getColor(i);
        standardCells[HOME_ENTRIES[i]].type = APPROCH;
        standardCells[HOME_ENTRIES[i]].relatedColor = getColor(i);
    }

    standardCells[BHAWANA_I].type = BHAWANA;
    standardCells[KOTUWA_I].type = KOTUWA;
    standardCells[PITA_KOTUWA_I].type = PITA_KOTUWA;
    
}

void initializeQueue(PlayerQueue *q)
{
    q->current = 0;

    for (int i = 0; i < PLAYERS; i++)
    {
        // for (int i = 0; i < PLAYERS; i++) {
        //     for (int j = 0; j < PIECES; j++) {
        //         players[i].pieces[j] = -1; // All pieces start in base
        //     }
        // }
        q->players[i].pieces_in_home = 4;
        q->players[i].consecutive_sixes = 0;
    }
    q->players[0].color = YELLOW;
    q->players[1].color = BLUE;
    q->players[2].color = RED;
    q->players[3].color = GREEN;
}

// IN GAME FUNCTIONS

void moveToX (PlayerQueue *q){
    q->players[q->current].pieces[0].status = ONTRACK;
}

// int can_move_from_base(Player *player, int roll)
// {
//     if (roll == 6)
//     {
//         for (int i = 0; i < PIECES; i++)
//         {
//             if (player->pieces[i] == -1)
//             {
//                 return 1; // Can move a piece out of base
//             }
//         }
//     }
//     return 0;
// }

// void move_piece(Player* player, int piece_index, int roll, Player players[PLAYERS], int current_player) {
//     if (player->pieces[piece_index] == -1) {
//         // Move from base to starting position if roll is 6
//         if (roll == 6) {
//             player->pieces[piece_index] = BASE_POSITIONS[current_player];
//             player->passed_approach[piece_index] = 0; // Reset the approach counter
//             printf("Player %d's piece %d moved to starting position.\n", current_player + 1, piece_index + 1);
//             capture_piece(players, current_player, player->pieces[piece_index]); // Check for capturing an opponent's piece
//         }
//     } else if (player->pieces[piece_index] < WINNING_POSITION) {
//         int current_pos = player->pieces[piece_index];

//         // Handle movement within the home straight
//         if (current_pos >= HOME_ENTRY[current_player] && current_pos < WINNING_POSITION) {
//             int position_in_home_straight = current_pos - HOME_ENTRY[current_player] + 1;
//             int needed_roll = WINNING_POSITION - current_pos;

//             if (roll == needed_roll) {
//                 player->pieces[piece_index] = WINNING_POSITION;
//                 player->pieces_in_home++;
//                 printf("Player %d's piece %d reached home!\n", current_player + 1, piece_index + 1);
//             } else if (roll < needed_roll) {
//                 player->pieces[piece_index] += roll;
//                 printf("Player %d's piece %d moved to position %d in the home straight.\n", current_player + 1, piece_index + 1, player->pieces[piece_index]);
//             } else {
//                 printf("Player %d needs an exact roll of %d to reach home, move not possible.\n", current_player + 1, needed_roll);
//             }
//         }
//         // Handle movement counterclockwise on the board
//         else if (current_pos == APPROACH_POSITIONS[current_player]) {
//             player->passed_approach[piece_index]++;

//             if (player->passed_approach[piece_index] == 2 && roll <= HOME_STRAIGHT) {
//                 player->pieces[piece_index] = HOME_ENTRY[current_player] + (roll - 1);
//                 printf("Player %d's piece %d moved into home straight at position %d.\n", current_player + 1, piece_index + 1, player->pieces[piece_index]);
//             } else if (player->passed_approach[piece_index] < 2) {
//                 player->pieces[piece_index] -= roll;
//                 if (player->pieces[piece_index] < 0) {
//                     player->pieces[piece_index] += BOARD_SIZE; // Wrap around the board counterclockwise
//                 }
//                 printf("Player %d's piece %d moved to position %d.\n", current_player + 1, piece_index + 1, player->pieces[piece_index]);
//                 capture_piece(players, current_player, player->pieces[piece_index]);
//             } else {
//                 printf("Player %d needs an exact roll of %d to enter the home straight.\n", current_player + 1, HOME_STRAIGHT);
//             }
//         } else {
//             // Regular movement on the board
//             player->pieces[piece_index] -= roll;
//             if (player->pieces[piece_index] < 0) {
//                 player->pieces[piece_index] += BOARD_SIZE; // Wrap around the board counterclockwise
//             }
//             if (player->pieces[piece_index] == APPROACH_POSITIONS[current_player]) {
//                 player->passed_approach[piece_index]++;
//             }
//             printf("Player %d's piece %d moved to position %d.\n", current_player + 1, piece_index + 1, player->pieces[piece_index]);
//             capture_piece(players, current_player, player->pieces[piece_index]);
//         }
//     }
// }

int has_won(Player *player)
{
    return player->pieces_in_home == PIECES;
}

// void capture_piece(Player players[PLAYERS], int current_player, int position)
// {
//     for (int i = 0; i < PLAYERS; i++)
//     {
//         if (i != current_player)
//         { // Check only opponent players
//             for (int j = 0; j < PIECES; j++)
//             {
//                 if (players[i].pieces[j] == position)
//                 { // Opponent's piece is at the same position
//                     printf("Player %d's piece %d captured by Player %d!\n", i + 1, j + 1, current_player + 1);
//                     players[i].pieces[j] = -1; // Send opponent's piece back to base
//                 }
//             }
//         }
//     }
// }
