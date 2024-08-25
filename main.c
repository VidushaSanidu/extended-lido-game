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

    initialize_players(&players);
    initialize_board();

    // in game progress
    // while (true){
        
        int dice = roll_dice();
        int hunt = nearest_hunt(players[currentUser],dice);

        if (players[currentUser].color == RED){
            printf("RED player rolled %d",dice);
            if (dice == 6){
                if (players[2].piecesInBase == 4){
                    move_to_x(players[2]);
                }else if (hunt != -1){
                    
                }
            }
        }
    // }

    return 0;
}