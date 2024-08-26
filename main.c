#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

int main()
{
    Player players[4];
    int currentUser = 0;
    int winners[4];

    srand(time(NULL));

    // before game begins
    pre_message();

    initialize_players(players, &currentUser);
    initialize_board();

    // in game progress
    // while (true){
        
        // int dice = roll_dice();
        // HuntResult hunt = nearest_hunt(players[currentUser],dice);

        // if (players[currentUser].color == RED){
        //     printf("RED player rolled %d",dice);
        //     if (dice == 6){
        //         if (hunt.hunt != -1){
        //             hunting_move(players,currentUser,hunt);
        //         } else if (players[currentUser].piecesInBase != 0){
        //             move_to_x(players[currentUser]);
        //         }else {

        //         }
        //     }
        // }
    // }

    return 0;
}