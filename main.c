#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

int main()
{
    PlayerQueue playerQueue;

    srand(time(NULL));

    // before game begins
    pre_message();

    initialize_players(&playerQueue);
    initialize_board();

    // in game operations

    // while (winner == -1)
    // {
    //     printf("\nPlayer %d's turn.\n", current_player + 1);
    //     print_board(players);

    //     int roll = roll_dice();
    //     printf("Player %d rolled a %d.\n", current_player + 1, roll);

    //     // Handle consecutive sixes
    //     if (roll == 6)
    //     {
    //         players[current_player].consecutive_sixes++;
    //         if (players[current_player].consecutive_sixes >= 3)
    //         {
    //             printf("Player %d rolled three sixes in a row, turn passed.\n", current_player + 1);
    //             players[current_player].consecutive_sixes = 0;   // Reset counter
    //             current_player = (current_player + 1) % PLAYERS; // Pass turn
    //             continue;                                        // Skip the rest of the turn
    //         }
    //     }
    //     else
    //     {
    //         players[current_player].consecutive_sixes = 0; // Reset counter if not a six
    //     }

    //     if (can_move_from_base(&players[current_player], roll))
    //     {
    //         for (int i = 0; i < PIECES; i++)
    //         {
    //             if (players[current_player].pieces[i] == -1)
    //             {
    //                 move_piece(&players[current_player], i, roll, players, current_player);
    //                 break;
    //             }
    //         }
    //     }
    //     else
    //     {
    //         int moved = 0;
    //         for (int i = 0; i < PIECES; i++)
    //         {
    //             if (players[current_player].pieces[i] >= 0 && players[current_player].pieces[i] < WINNING_POSITION)
    //             {
    //                 move_piece(&players[current_player], i, roll, players, current_player);
    //                 moved = 1;
    //                 break;
    //             }
    //         }
    //         if (!moved)
    //         {
    //             printf("Player %d could not move any piece.\n", current_player + 1);
    //         }
    //     }

    //     if (has_won(&players[current_player]))
    //     {
    //         winner = current_player;
    //     }

    //     // Handle turn change: if not rolling a six, move to the next player
    //     if (roll != 6)
    //     {
    //         current_player = (current_player + 1) % PLAYERS;
    //     }
    // }

    // printf("Player %d has won the game!\n", winner + 1);
    return 0;
}