#ifndef TYPES_H
#define TYPES_H

#define PLAYERS 4
#define PIECES 4
#define BOARD_SIZE 52
#define HOME_STRAIGHT 6
#define WINNING_POSITION 57

typedef struct {
    int pieces[PIECES];
    int pieces_in_home;
    int consecutive_sixes; // Counter for consecutive sixes
    int passed_approach[PIECES]; // Track how many times each piece has passed the Approach cell
} Player;

void initialize_game(Player players[PLAYERS]);
int roll_dice();
void move_piece(Player* player, int piece_index, int roll, Player players[PLAYERS], int current_player);
int can_move_from_base(Player* player, int roll);
void print_board(Player players[PLAYERS]);
int has_won(Player* player);
void capture_piece(Player players[PLAYERS], int current_player, int position);

#endif