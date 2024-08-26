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
    POWERED,
    DEFAULT
} Aura;

// structures
typedef struct
{
    int position;
    Status status;
    int direction;
    int capturedPieces;
    int straightCount;
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
    CellType type;
    int noOfPiece;
    PlayerColor currentColor;
} Cell;

typedef struct {
    int hunt;
    int pieceIndex;
} HuntResult;

typedef struct {
    Piece result;
    int blockedPiece;
    int blockedCell;
} BlockedResult;

// functions
void pre_message();
void initialize_players(Player *q[]);
void initialize_board();
void initializeQueue(Player *q[]);

int roll_dice();
void move_piece(Player *player, int piece_index, int roll, Player players[PLAYERS], int current_player);
void capture_piece(Player players[PLAYERS], int current_player, int position);

void move_to_x (Player player);
HuntResult nearest_hunt(Player player, int max);


#endif