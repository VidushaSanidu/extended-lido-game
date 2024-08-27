#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

Player players[4];

int main()
{
    int winners = 0;
    int currentUser = 0;
    int rounds = 0;
    int mystry;

    srand(time(NULL));

    // before game begins
    pre_message();

    initialize_players(&currentUser);
    int first = currentUser;
    initialize_board();

    // in game progress
    while (winners <= 3){
        if (currentUser == first)
            rounds ++;

        if ( rounds%4 == 3 ){
            generate_mystory(&mystry);
        }

        if (rounds>1 && currentUser == first)
            print_status(mystry);
        

        if (players[currentUser].piecesInHome == 4){
            rotateCount(&currentUser);
            continue;
        }
        
        int dice = roll_dice();

        if (dice == 6){
            players[currentUser].consecutiveSixes ++;
        }else {
            players[currentUser].consecutiveSixes = 0;
        }

        if (players[currentUser].consecutiveSixes >= 3)
        {
            printf("\n player entered 6 three consecutive times.player changed.\n");
            rotateCount(&currentUser);
            continue;
        }

        home_straight_move(currentUser,dice);

        HuntResult singleHunt = get_nearest_hunt_for_single(players[currentUser],dice);
        HuntResult blockHunt = get_nearest_hunt_for_block(players[currentUser],dice);

        BlockedResult singleNoBlock = find_non_blockable_single(players[currentUser],dice);
        BlockedResult blockNoBlock = find_non_blockable_block(players[currentUser],dice);

        if (players[currentUser].color == RED){
            printf("RED player rolled %d",dice);
            if (dice == 6){
                if (singleHunt.huntIndex != -1){
                    single_capturing_move(currentUser,singleHunt, dice);
                } else if (blockHunt.huntIndex != -1){
                    block_capturing_move(currentUser,singleHunt, dice);
                } else if (players[currentUser].piecesInBase != 0){
                    move_to_x(players[currentUser]);
                }else {
                    if (singleNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_single_move(currentUser,singleNoBlock.pieceNo,dice);
                    } else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_block_move(currentUser,blockNoBlock.pieceNo,dice);
                    } else if (singleNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_single_move(currentUser,singleNoBlock.pieceNo,singleNoBlock.count);
                    }else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_block_move(currentUser,blockNoBlock.pieceNo,singleNoBlock.count);
                    }else{
                        cantMove(currentUser);
                    }
                }
            }else
            {
                if (singleHunt.huntIndex != -1){
                    single_capturing_move(currentUser,singleHunt, dice);
                } else if (blockHunt.huntIndex != -1){
                    block_capturing_move(currentUser,singleHunt, dice);
                } else if (players[currentUser].piecesInBase != 0){
                    move_to_x(players[currentUser]);
                }else {
                    if (singleNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_single_move(currentUser,singleNoBlock.pieceNo,dice);
                    } else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_block_move(currentUser,blockNoBlock.pieceNo,dice);
                    } else if (singleNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_single_move(currentUser,singleNoBlock.pieceNo,singleNoBlock.count);
                    }else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_block_move(currentUser,blockNoBlock.pieceNo,singleNoBlock.count);
                    }else{
                        cantMove(currentUser);
                    }
                }
            }
            
        }else if (players[currentUser].color == GREEN)
        {
            printf("GREEN player rolled %d",dice);
            if (dice == 6){
                if (singleHunt.huntIndex != -1){
                    single_capturing_move(currentUser,singleHunt, dice);
                } else if (blockHunt.huntIndex != -1){
                    block_capturing_move(currentUser,singleHunt, dice);
                } else if (players[currentUser].piecesInBase != 0){
                    move_to_x(players[currentUser]);
                }else {
                    if (singleNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_single_move(currentUser,singleNoBlock.pieceNo,dice);
                    } else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_block_move(currentUser,blockNoBlock.pieceNo,dice);
                    } else if (singleNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_single_move(currentUser,singleNoBlock.pieceNo,singleNoBlock.count);
                    }else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_block_move(currentUser,blockNoBlock.pieceNo,singleNoBlock.count);
                    }else{
                        cantMove(currentUser);
                    }
                }
            }else
            {
                if (singleHunt.huntIndex != -1){
                    single_capturing_move(currentUser,singleHunt, dice);
                } else if (blockHunt.huntIndex != -1){
                    block_capturing_move(currentUser,singleHunt, dice);
                } else {
                    if (singleNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_single_move(currentUser,singleNoBlock.pieceNo,dice);
                    } else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_block_move(currentUser,blockNoBlock.pieceNo,dice);
                    } else if (singleNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_single_move(currentUser,singleNoBlock.pieceNo,singleNoBlock.count);
                    }else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_block_move(currentUser,blockNoBlock.pieceNo,singleNoBlock.count);
                    }else{
                        cantMove(currentUser);
                    }
                }
            }
        }else if (players[currentUser].color == YELLOW)
        {
            printf("YELLOW player rolled %d",dice);
            if (dice == 6){
                if (singleHunt.huntIndex != -1){
                    single_capturing_move(currentUser,singleHunt, dice);
                } else if (blockHunt.huntIndex != -1){
                    block_capturing_move(currentUser,singleHunt, dice);
                } else if (players[currentUser].piecesInBase != 0){
                    move_to_x(players[currentUser]);
                }else {
                    if (singleNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_single_move(currentUser,singleNoBlock.pieceNo,dice);
                    } else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_block_move(currentUser,blockNoBlock.pieceNo,dice);
                    } else if (singleNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_single_move(currentUser,singleNoBlock.pieceNo,singleNoBlock.count);
                    }else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_block_move(currentUser,blockNoBlock.pieceNo,singleNoBlock.count);
                    }else{
                        cantMove(currentUser);
                    }
                }
            }else
            {
                if (singleHunt.huntIndex != -1){
                    single_capturing_move(currentUser,singleHunt, dice);
                } else if (blockHunt.huntIndex != -1){
                    block_capturing_move(currentUser,singleHunt, dice);
                } else {
                    if (singleNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_single_move(currentUser,singleNoBlock.pieceNo,dice);
                    } else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_block_move(currentUser,blockNoBlock.pieceNo,dice);
                    } else if (singleNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_single_move(currentUser,singleNoBlock.pieceNo,singleNoBlock.count);
                    }else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_block_move(currentUser,blockNoBlock.pieceNo,singleNoBlock.count);
                    }else{
                        cantMove(currentUser);
                    }
                }
            }
        }else if (players[currentUser].color == BLUE)
        {
            printf("BLUE player rolled %d",dice);
            if (dice == 6){
                if (singleHunt.huntIndex != -1){
                    single_capturing_move(currentUser,singleHunt, dice);
                } else if (blockHunt.huntIndex != -1){
                    block_capturing_move(currentUser,singleHunt, dice);
                } else if (players[currentUser].piecesInBase != 0){
                    move_to_x(players[currentUser]);
                }else {
                    if (singleNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_single_move(currentUser,singleNoBlock.pieceNo,dice);
                    } else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_block_move(currentUser,blockNoBlock.pieceNo,dice);
                    } else if (singleNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_single_move(currentUser,singleNoBlock.pieceNo,singleNoBlock.count);
                    }else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_block_move(currentUser,blockNoBlock.pieceNo,singleNoBlock.count);
                    }else{
                        cantMove(currentUser);
                    }
                }
            }else
            {
                if (singleHunt.huntIndex != -1){
                    single_capturing_move(currentUser,singleHunt, dice);
                } else if (blockHunt.huntIndex != -1){
                    block_capturing_move(currentUser,singleHunt, dice);
                } else {
                    if (singleNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_single_move(currentUser,singleNoBlock.pieceNo,dice);
                    } else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1){
                        standard_block_move(currentUser,blockNoBlock.pieceNo,dice);
                    } else if (singleNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_single_move(currentUser,singleNoBlock.pieceNo,singleNoBlock.count);
                    }else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0){
                        blockable_block_move(currentUser,blockNoBlock.pieceNo,singleNoBlock.count);
                    }else{
                        cantMove(currentUser);
                    }
                }
            }
        }
        

        if (players[currentUser].piecesInHome == 4){
            print_winner(currentUser);
            winners ++;
        }

        rotateCount(&currentUser);
    }

    return 0;
}