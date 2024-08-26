#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

Player players[4];

int main()
{
    int winners = 0;
    int currentUser = 0;

    srand(time(NULL));

    // before game begins
    pre_message();

    initialize_players(&currentUser);
    initialize_board();

    // in game progress
    while (winners <= 3){
        
        int dice = roll_dice();
        HuntResult singleHunt = get_nearest_hunt_for_single(players[currentUser],dice);
        HuntResult blockHunt = get_nearest_hunt_for_block(players[currentUser],dice);

        if (players[currentUser].color == RED){
            printf("RED player rolled %d",dice);
            if (dice == 6){
                if (singleHunt.hunt != -1){
                    single_capturing_move(players,currentUser,hunt,6);
                } else if (blockHunt.hunt != -1){
                    block_capturing_move(players,currentUser,hunt,6);
                } else if (players[currentUser].piecesInBase != 0){
                    move_to_x(players[currentUser]);
                }else {

                }
            }
        }
    }

    return 0;
}