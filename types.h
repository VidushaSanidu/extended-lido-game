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
#define HOME_STRAIGHT_CELLS 6
#define BHAWANA_I 9
#define KOTUWA_I 27
#define PITA_KOTUWA_I 46


#define START_POINTS_V {2, 15, 28, 41}
#define APPROACH_POSITIONS_V {0, 13, 26, 39}

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
    STRAIGHT,
    ONTRACK
} Status;
typedef enum
{
    SICK,
    POWERED,
    NORMAL,
    FREEZE
} Aura;

// structures
typedef struct
{
    int position;
    Status status;
    int direction;
    int capturedPieces;
    int straightCount;
    int approchCount;
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
    int pieceNo;
    int huntIndex;
} HuntResult;

typedef struct {
    int pieceNo;
    int prevIndex;
} BlockedResult;

typedef struct {
    int pieceNo;
    int blockIndex;
} FindBlockResult;

typedef struct {
    int pieceNo;
    int boxIndex;
} BoxResult;

// global variables
extern Player players[4];

// functions
void pre_message();
void initialize_players(int *current);
void initialize_board();
void initialize_queue();

int roll_dice();
int coin_toss();
int find_gap(Piece piece);
void reset_piece(PlayerColor color, int peiceNo);
int get_board_count(Player player);

void single_move(PlayerColor index, int pieceIndex, int roll);
void block_move(PlayerColor index, int pieceIndex, int roll);
void handle_mystery(PlayerColor index, int pieceIndex, int option);

HuntResult get_nearest_hunt_for_single(Player player, int max);
HuntResult get_nearest_hunt_for_block(Player player, int max);
BlockedResult find_non_blockable_single(Player player, int max);
BlockedResult find_non_blockable_block(Player player, int max);
BoxResult find_mystory_box(Player player, int max);
FindBlockResult find_blocks(Player player, int max);

void move_to_x(Player player);
void single_capturing_move(int cUser, HuntResult hunt,int roll);
void block_capturing_move(int cUser, HuntResult hunt,int roll);
void standard_single_move(int cUser,int pieceNo, int roll);


#endif