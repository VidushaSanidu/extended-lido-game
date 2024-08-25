#ifndef TYPES_H
#define TYPES_H

// data types
#define true 1
#define false 0

#define antiClock 1
#define clock 0

// constants
#define DICE_SIDES 6

#define PLAYERS 4
#define PIECES 4

#define STANDARD_CELLS 52
#define HOME_STRAIGHT_CELLS 5
#define BHAWANA_I 9
#define KOTUWA_I 27
#define PITA_KOTUWA_I 46

#define WINNING_POSITION 58

// enums
typedef enum
{
    YELLOW,
    BLUE,
    RED,
    GREEN,
    DEFAULT
} PlayerColor;
typedef enum
{
    BASIC,
    MYSTERY,
    BHAWANA,
    KOTUWA,
    PITA_KOTUWA
} CellType;
typedef enum
{
    BASE,
    HOME,
    ONTRACK
} Status;
typedef enum
{
    SICK,
    POWERED
} Aura;

// structures
typedef struct
{
    int position;
    Status status;
    int direction;
    int capturedPieces;
    int toWin;
    int approchCount;
    int isBlocked;
    int blockDirection;
    Aura auraType;
    int auraDuration;
} Piece;

typedef struct
{
    PlayerColor color;
    Piece pieces[PIECES];
    int piecesInHome;
    int piecesInBase;
    int consecutiveSixes;
} Player;

typedef struct
{
    Player players[PLAYERS];
    int current;
} PlayerQueue;

typedef struct
{
    CellType type;
    int noOfPiece;
    PlayerColor currentColor;
} Cell;

// functions
void pre_message();
void initialize_players(PlayerQueue *q);
void initialize_board();
void initializeQueue(PlayerQueue *q);

int roll_dice();
void move_piece(Player *player, int piece_index, int roll, Player players[PLAYERS], int current_player);
int can_move_from_base(Player *player, int roll);
void print_board(Player players[PLAYERS]);
int has_won(Player *player);
void capture_piece(Player players[PLAYERS], int current_player, int position);

#endif