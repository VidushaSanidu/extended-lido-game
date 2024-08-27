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
    int mystry = -1;

    srand(time(NULL));

    // before game begins
    pre_message();

    initialize_players(&currentUser);
    int first = currentUser;
    initialize_board();

    // in game progress
    while (winners <= 0)
    {
        // count rounds
        if (currentUser == first) {
            rounds++;

            // generate mystery cell
            if (rounds % 4 == 0)
            {
                generate_mystory(&mystry);
            }
        }

        // print state after each round
        if (rounds > 1 && currentUser == first)
            print_status(mystry);

        // pass already won players
        if (players[currentUser].piecesInHome == 4)
        {
            rotateCount(&currentUser);
            continue;
        }

        // make a roll
        int dice = roll_dice();

        // track consecutive sixes
        if (dice == 6)
        {
            players[currentUser].consecutiveSixes++;
        }
        else
        {
            players[currentUser].consecutiveSixes = 0;
        }

        if (players[currentUser].consecutiveSixes >= 3)
        {
            printf("\n player entered 6 three consecutive times.player changed.\n");
            rotateCount(&currentUser);
            continue;
        }

        // handle home straight pieces
        home_straight_move(currentUser, dice);

        // generate results according to the strategy
        HuntResult singleHunt = get_nearest_hunt_for_single(players[currentUser], dice);
        HuntResult blockHunt = get_nearest_hunt_for_block(players[currentUser], dice);

        BlockedResult singleNoBlock = find_non_blockable_single(players[currentUser], dice);
        BlockedResult blockNoBlock = find_non_blockable_block(players[currentUser], dice);

        FindBlockResult findBlockSingle = find_blocks(players[currentUser], dice);
        BoxResult boxResults = find_mystory_box(players[currentUser], dice);

        if (players[currentUser].color == RED)
        {
            printf("RED player rolled %d", dice);

            if (singleHunt.huntIndex != -1)
            {
                single_capturing_move(currentUser, singleHunt, dice);
            }
            else if (blockHunt.huntIndex != -1)
            {
                block_capturing_move(currentUser, blockHunt, dice);
            }
            else if (dice == 6 && players[currentUser].piecesInBase != 0 && standardCells[28].noOfPiece == 0)
            {
                move_to_x(currentUser);
            }
            else if (singleNoBlock.pieceNo != -1 && singleNoBlock.count <= -1)
            {
                standard_single_move(currentUser, singleNoBlock.pieceNo, dice);
            }
            else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1)
            {
                standard_block_move(currentUser, blockNoBlock.pieceNo, dice);
            }
            else if (singleNoBlock.pieceNo != -1 && singleNoBlock.count > 0)
            {
                blockable_single_move(currentUser, singleNoBlock.pieceNo, singleNoBlock.count);
            }
            else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0)
            {
                blockable_block_move(currentUser, blockNoBlock.pieceNo, blockNoBlock.count);
            }
            else if (findBlockSingle.blockIndex != -1)
            {
                find_blocks_move(currentUser, findBlockSingle.pieceNo, findBlockSingle.blockIndex, dice);
            }
            else if (boxResults.boxIndex != -1)
            {
                find_boxs_move(currentUser, boxResults.pieceNo, boxResults.boxIndex, dice);
            }
            else
            {
                cantMove(currentUser);
            }
        }
        else if (players[currentUser].color == GREEN)
        {
            printf("GREEN player rolled %d", dice);

            if (findBlockSingle.blockIndex != -1)
            {
                find_blocks_move(currentUser, findBlockSingle.pieceNo, findBlockSingle.blockIndex, dice);
            }
            else if (dice == 6 && players[currentUser].piecesInBase != 0 && standardCells[41].noOfPiece == 0)
            {
                move_to_x(currentUser);
            }
            else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1)
            {
                standard_block_move(currentUser, blockNoBlock.pieceNo, dice);
            }
            else if (blockHunt.huntIndex != -1)
            {
                block_capturing_move(currentUser, blockHunt, dice);
            }
            else if (singleNoBlock.pieceNo != -1 && singleNoBlock.count <= -1)
            {
                standard_single_move(currentUser, singleNoBlock.pieceNo, dice);
            }
            else if (singleHunt.huntIndex != -1)
            {
                single_capturing_move(currentUser, singleHunt, dice);
            }
            else if (singleNoBlock.pieceNo != -1 && singleNoBlock.count > 0)
            {
                blockable_single_move(currentUser, singleNoBlock.pieceNo, singleNoBlock.count);
            }
            else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0)
            {
                blockable_block_move(currentUser, blockNoBlock.pieceNo, blockNoBlock.count);
            }
            else if (boxResults.boxIndex != -1)
            {
                find_boxs_move(currentUser, boxResults.pieceNo, boxResults.boxIndex, dice);
            }
            else
            {
                cantMove(currentUser);
            }
        }
        else if (players[currentUser].color == YELLOW)
        {
            printf("YELLOW player rolled %d", dice);

            if (dice == 6 && players[currentUser].piecesInBase != 0 && standardCells[2].noOfPiece == 0)
            {
                move_to_x(currentUser);
            }
            else if (blockHunt.huntIndex != -1)
            {
                block_capturing_move(currentUser, blockHunt, dice);
            }
            else if (singleHunt.huntIndex != -1)
            {
                single_capturing_move(currentUser, singleHunt, dice);
            }
            else if (singleNoBlock.pieceNo != -1 && singleNoBlock.count <= -1)
            {
                standard_single_move(currentUser, singleNoBlock.pieceNo, dice);
            }
            else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1)
            {
                standard_block_move(currentUser, blockNoBlock.pieceNo, dice);
            }
            else if (singleNoBlock.pieceNo != -1 && singleNoBlock.count > 0)
            {
                blockable_single_move(currentUser, singleNoBlock.pieceNo, singleNoBlock.count);
            }
            else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0)
            {
                blockable_block_move(currentUser, blockNoBlock.pieceNo, blockNoBlock.count);
            }
            else if (findBlockSingle.blockIndex != -1)
            {
                find_blocks_move(currentUser, findBlockSingle.pieceNo, findBlockSingle.blockIndex, dice);
            }
            else if (boxResults.boxIndex != -1)
            {
                find_boxs_move(currentUser, boxResults.pieceNo, boxResults.boxIndex, dice);
            }
            else
            {
                cantMove(currentUser);
            }
        }
        else if (players[currentUser].color == BLUE)
        {
            printf("BLUE player rolled %d", dice);

            if (dice == 6 && players[currentUser].piecesInBase != 0 && standardCells[15].noOfPiece == 0)
            {
                move_to_x(currentUser);
            }
            else if (singleNoBlock.pieceNo != -1 && singleNoBlock.count <= -1)
            {
                standard_single_move(currentUser, singleNoBlock.pieceNo, dice);
            }
            else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count <= -1)
            {
                standard_block_move(currentUser, blockNoBlock.pieceNo, dice);
            }
            else if (boxResults.boxIndex != -1 && players[currentUser].pieces[boxResults.pieceNo].direction == antiClock)
            {
                find_boxs_move(currentUser, boxResults.pieceNo, boxResults.boxIndex, dice);
            }
            else if (blockHunt.huntIndex != -1)
            {
                block_capturing_move(currentUser, blockHunt, dice);
            }
            else if (singleHunt.huntIndex != -1)
            {
                single_capturing_move(currentUser, singleHunt, dice);
            }
            else if (singleNoBlock.pieceNo != -1 && singleNoBlock.count > 0)
            {
                blockable_single_move(currentUser, singleNoBlock.pieceNo, singleNoBlock.count);
            }
            else if (blockNoBlock.pieceNo != -1 && blockNoBlock.count > 0)
            {
                blockable_block_move(currentUser, blockNoBlock.pieceNo, blockNoBlock.count);
            }
            else if (boxResults.boxIndex != -1)
            {
                find_boxs_move(currentUser, boxResults.pieceNo, boxResults.boxIndex, dice);
            }
            else
            {
                cantMove(currentUser);
            }
        }

        if (players[currentUser].piecesInHome == 4)
        {
            print_winner(currentUser);
            winners++;
        }

        rotateCount(&currentUser);

        // if (rounds == 20)
        // {
        //     break;
        // }
    }

    return 0;
}