#ifndef TYPES_H
#define TYPES_H

#define DICE_SIDES 6

#define PLAYERS 4
#define PIECES 4

#define STANDARD_CELLS 52
#define HOME_STRAIGHT_CELLS 5

#define WINNING_POSITION 58

typedef enum { YELLOW, BLUE,RED, GREEN  } PlayerColor;
typedef struct {
    PlayerColor color;
    int pieces[PIECES];
    int pieces_in_home;
    int consecutive_sixes; // Counter for consecutive sixes
    int passed_approach[PIECES]; // Track how many times each piece has passed the Approach cell
} Player;

typedef struct {
    Player players[PLAYERS];
    int front;
    int rear;
} PlayerQueue;

void pre_message();
void initialize_game(Player players[PLAYERS]);
int roll_dice();
void move_piece(Player* player, int piece_index, int roll, Player players[PLAYERS], int current_player);
int can_move_from_base(Player* player, int roll);
void print_board(Player players[PLAYERS]);
int has_won(Player* player);
void capture_piece(Player players[PLAYERS], int current_player, int position);

#endif