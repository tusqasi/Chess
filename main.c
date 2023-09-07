#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "./include/raygui.h"
#include "./include/terminal.h"

#include "rook.c"
#include "bishop.c"
#include "knight.c"
#include "queen.c"
#include "king.c"
#include "pawn.c"

#define board_width 480 
#define board_height 480
#define window_width board_width * 2
#define window_height 480
#define sqSize 60
#define all_possible_moves_len 40

static char *pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
//static char *pos = "/ppp///3BQ//3pppp";

bool drawNumber = false;

struct _sprites {
	Image white_pawn;
	Image white_knight;
	Image white_queen;
	Image white_king;
	Image white_bishop;
	Image white_rook;
	Image black_pawn;
	Image black_knight;
	Image black_queen;
	Image black_king;
	Image black_bishop;
	Image black_rook;
};

enum piece { 
    None = 0, 
    King = 1, 
    Pawn = 2, 
    Knight = 3, 
    Bishop = 4, 
    Rook = 5, 
    Queen = 6, 
    White = 8, 
    Black = 16,
};

static int piece_info = 0;
static int pos_piece_info = -1;
static int all_possible_moves[all_possible_moves_len] = { 0 };

void drawChessBoard() {
    for(int file = 0; file < 8; file++) {
        for(int rank = 0; rank < 8; rank++) {
            bool isLightSquared = (file + rank) % 2 != 0;
            Color color = isLightSquared ? DARKPURPLE : WHITE;
            DrawRectangle(file * sqSize, rank * sqSize, sqSize, sqSize, color);
        }
    }
}

void fixHighlightedSquares() {
    for(int i = 0; i < all_possible_moves_len; i++) {
        if(all_possible_moves[i] != -1) {
            int rank = all_possible_moves[i] / 8;
            int file = all_possible_moves[i] % 8;
            bool isLightSquared = (file + rank) % 2 != 0;
            Color color = isLightSquared ? DARKPURPLE : WHITE;
            DrawRectangle(file * sqSize, rank * sqSize, sqSize, sqSize, color);
        }
    }
}

void show() {
    printf("+++++++++++++++++++++++\n");
    for(int i = 0; i < all_possible_moves_len; i++) {
        if(all_possible_moves[i] != -1)
            printf("%d ", all_possible_moves[i]);
    }
    printf("\n+++++++++++++++++++++++\n");
}

void drawAllPossibleSquares() {
    for(int i = 0; i < all_possible_moves_len; i++) {
        if(all_possible_moves[i] != -1) {
            int rank = all_possible_moves[i] / 8;
            int file = all_possible_moves[i] % 8;
            DrawRectangle(file * sqSize, rank * sqSize, sqSize, sqSize, ColorAlpha(RED, 0.6));
        }
    }
}

int whereIsTargetOnDiagonal(int targetPos) {
    
    int tRank = targetPos / 8;
    int tFile = targetPos % 8;

    int sRank = pos_piece_info / 8;
    int sFile = pos_piece_info % 8;

    if(tFile < sFile) {

        if(tRank < sRank) {
            return  1;
        }

        if(tRank > sRank) {
            return  3;
        }

    } else {

        if(tRank < sRank) {
            return  2;
        }

        if(tRank > sRank) {
            return  4;
        }

    }

    return 0;

}

bool diagonalPiecesValidMoveCheck(int targetPos, int* squareBoard) {
    
    int whereTo = whereIsTargetOnDiagonal(targetPos);
    int pos_piece_info_copy = pos_piece_info;

    while(pos_piece_info_copy != targetPos) {

        if(whereTo == 1) {
            pos_piece_info_copy -= 9;

        } else if (whereTo == 2) {
            pos_piece_info_copy -= 7;

        } else if (whereTo == 3) {
            pos_piece_info_copy += 7;

        } else if (whereTo == 4) {
            pos_piece_info_copy += 9;

        }

        if(squareBoard[pos_piece_info_copy] != 0) {
            printf("PIECE IN THE WAY\n");
            return false;
        }

    }

    return true;

}

int whereIsTargetOnStraightLine(int targetPos) {
    
    int tRank = targetPos / 8;
    int tFile = targetPos % 8;

    int sRank = pos_piece_info / 8;
    int sFile = pos_piece_info % 8;

    if(tFile < sFile) {
        return 1;
    }

    if(tRank < sRank) {
        return 2;
    }

    if(tFile > sFile) {
        return 3;
    }

    if(tRank > sRank) {
        return 4;
    }

    return 0;

}

bool straightPiecesValidMoveCheck(int targetPos, int* squareBoard) {
    
    int whereTo = whereIsTargetOnStraightLine(targetPos);
    int pos_piece_info_copy = pos_piece_info;

    while(pos_piece_info_copy != targetPos) {

        if(whereTo == 1) {
            pos_piece_info_copy -= 1;

        } else if (whereTo == 2) {
            pos_piece_info_copy -= 8;

        } else if (whereTo == 3) {
            pos_piece_info_copy += 1;

        } else if (whereTo == 4) {
            pos_piece_info_copy += 8;

        }

        if(squareBoard[pos_piece_info_copy] != 0) {
            printf("PIECE IN THE WAY\n");
            return false;
        }

    }

    return true;

}

bool isMoveInPossible(int targetPos) {

    int first = 0;
    int last = all_possible_moves_len - 1;

    while(first <= last){

        int mid = (first + last) / 2;

        if(targetPos == all_possible_moves[mid]){

            return true;

        } else if(all_possible_moves[mid] > targetPos){
            last = mid - 1;
        } else{
            first = mid + 1;
        }

    }

    return false;

}

void sortList() {
    for(int i = 0; i < all_possible_moves_len; i++) {
        for(int j = 0; j < all_possible_moves_len; j++) {
            if(all_possible_moves[i] < all_possible_moves[j]) {
                int temp = all_possible_moves[i];
                all_possible_moves[i] = all_possible_moves[j];
                all_possible_moves[j] = temp;
            }
        }
    }
}

void removeDupFromList() {
    for(int i = 0; i < all_possible_moves_len; i++) {
        if(all_possible_moves[i] == all_possible_moves[i + 1]) {
            all_possible_moves[i] = -1;
        }
    }
}

void initChessBoard(int *squareBoard) {
	printf("Loading FEN: %s\n",pos);
    for(int file = 0; file < 8; file++) {
        for(int rank = 0; rank < 8; rank++) {
            bool isLightSquared = (file + rank) % 2 != 0;
            Color color = isLightSquared ? DARKPURPLE : WHITE;
            DrawRectangle(file * sqSize, rank * sqSize, sqSize, sqSize, color);
        }
    }

    int file = 0;
    int rank = 0;
	// make sure board is clear
	for (int i=0; i<64; i++) {
		squareBoard[i] = -1;	
	}
    for(int i = 0; i < strlen(pos); i++) {

        char *c = &pos[i];

        if(isdigit(*c)) {

            file += atoi(c);

        } else if (strncmp("/", c, 1) == 0) {

            rank++;
            file = 0;

        } else {

            if (strncmp("k", c, 1) == 0) {
                squareBoard[8 * rank + file] = 17;
            }

            if (strncmp("p", c, 1) == 0) {
                squareBoard[8 * rank + file] = 18;
            }

            if (strncmp("n", c, 1) == 0) {
                squareBoard[8 * rank + file] = 19;
            }

            if (strncmp("b", c, 1) == 0) {
                squareBoard[8 * rank + file] = 20;
            } 

            if (strncmp("r", c, 1) == 0) {
                squareBoard[8 * rank + file] = 21;
            } 

            if (strncmp("q", c, 1) == 0) {
                squareBoard[8 * rank + file] = 22;
            } 

            if(strncmp("K", c, 1) == 0) {
                squareBoard[8 * rank + file] = 9;
            } 

            if (strncmp("P", c, 1) == 0) {
                squareBoard[8 * rank + file] = 10;
            } 

            if (strncmp("N", c, 1) == 0) {
                squareBoard[8 * rank + file] = 11;
            } 

            if (strncmp("B", c, 1) == 0) {
                squareBoard[8 * rank + file] = 12;
            } 

            if (strncmp("R", c, 1) == 0) {
                squareBoard[8 * rank + file] = 13;
            } 

            if (strncmp("Q", c, 1) == 0) {
                squareBoard[8 * rank + file] = 14;
            }

            file++;

        }

    }
}

void updateChessBoard( 
	int *squareBoard,
	struct _sprites sprites
) {
	ClearBackground(BLACK);
	Image white_pawn = sprites.white_pawn; 
	Image white_knight = sprites.white_knight; 
	Image white_queen = sprites.white_queen; 
	Image white_king = sprites.white_king; 
	Image white_bishop = sprites.white_bishop; 
	Image white_rook = sprites.white_rook; 
	Image black_pawn = sprites.black_pawn; 
	Image black_knight = sprites.black_knight; 
	Image black_queen = sprites.black_queen; 
	Image black_king = sprites.black_king; 
	Image black_bishop = sprites.black_bishop; 
	Image black_rook = sprites.black_rook; 

	ClearBackground(BLACK);
	char buf[2];
    for(int file = 0; file < 8; file++) {
        for(int rank = 0; rank < 8; rank++) {
            bool isLightSquared = (file + rank) % 2 != 0;
            Color color = isLightSquared ? DARKPURPLE : WHITE;
            DrawRectangle(file * sqSize, rank * sqSize, sqSize, sqSize, color);

            if(squareBoard[8 * rank + file] == None) {
                if((rank + file) % 2 != 0) {
                    DrawRectangle(file * sqSize, rank * sqSize, sqSize, sqSize, DARKPURPLE);
                } else {
                    DrawRectangle(file * sqSize, rank * sqSize, sqSize, sqSize, WHITE);
                }
            }

            if(squareBoard[8 * rank + file] == (King | White)) {
                DrawTexture(LoadTextureFromImage(white_king), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Pawn | White)) {
                DrawTexture(LoadTextureFromImage(white_pawn), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Knight | White)) {
                DrawTexture(LoadTextureFromImage(white_knight), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Bishop | White)) {
                DrawTexture(LoadTextureFromImage(white_bishop), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Rook | White)) {
                DrawTexture(LoadTextureFromImage(white_rook), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Queen | White)) {
                DrawTexture(LoadTextureFromImage(white_queen), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (King | Black)) {
                DrawTexture(LoadTextureFromImage(black_king), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Pawn | Black)) {
                DrawTexture(LoadTextureFromImage(black_pawn), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Knight | Black)) {
                DrawTexture(LoadTextureFromImage(black_knight), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Bishop | Black)) {
                DrawTexture(LoadTextureFromImage(black_bishop), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Rook | Black)) {
                DrawTexture(LoadTextureFromImage(black_rook), file * sqSize, rank * sqSize, RAYWHITE);
            }

            if(squareBoard[8 * rank + file] == (Queen | Black)) {
                DrawTexture(LoadTextureFromImage(black_queen), file * sqSize, rank * sqSize, RAYWHITE);
            }
			if(drawNumber){
				sprintf(buf, "%d", rank*8+file);	
				DrawText(buf, file*sqSize, rank*sqSize, 19, GREEN);
			}
        }
    }

}

char * piece_name(int p){
	switch (p) {
		case(None   |White):return "Selected: White None";
		case(King   |White):return "Selected: White King";
		case(Pawn   |White):return "Selected: White Pawn";
		case(Knight |White):return "Selected: White Knight";
		case(Bishop |White):return "Selected: White Bishop";
		case(Rook   |White):return "Selected: White Rook";
		case(Queen  |White):return "Selected: White Queen";
		case(None   |Black):return "Selected: Black None";
		case(King   |Black):return "Selected: Black King";
		case(Pawn   |Black):return "Selected: Black Pawn";
		case(Knight |Black):return "Selected: Black Knight";
		case(Bishop |Black):return "Selected: Black Bishop";
		case(Rook   |Black):return "Selected: Black Rook";
		case(Queen  |Black):return "Selected: Black Queen";
		default            :return "Not Selected";
	}
}

int main(void) {

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(window_width, window_height, "CHESS BOARD");

	GuiLoadStyleTerminal();

    SetTargetFPS(60);

    // ------------------- LOADING IMAGES -------------------
	struct _sprites sprites;
    sprites.black_king= LoadImage("./Chess Pieces/black_king.png");
    sprites.black_pawn = LoadImage("./Chess Pieces/black_pawn.png");
    sprites.black_knight = LoadImage("./Chess Pieces/black_knight.png");
    sprites.black_bishop = LoadImage("./Chess Pieces/black_bishop.png");
    sprites.black_rook = LoadImage("./Chess Pieces/black_rook.png");
    sprites.black_queen = LoadImage("./Chess Pieces/black_queen.png");

    sprites.white_king= LoadImage("./Chess Pieces/white_king.png");
    sprites.white_pawn = LoadImage("./Chess Pieces/white_pawn.png");
    sprites.white_knight = LoadImage("./Chess Pieces/white_knight.png");
    sprites.white_bishop = LoadImage("./Chess Pieces/white_bishop.png");
    sprites.white_rook = LoadImage("./Chess Pieces/white_rook.png");
    sprites.white_queen = LoadImage("./Chess Pieces/white_queen.png");

    // ------------------- DRAWING BOARD -------------------

    int squareBoard[64] = { 0 };

    initChessBoard(squareBoard);
    updateChessBoard(squareBoard, sprites);

	int keyPressed ;
    while(!WindowShouldClose()) {
        // ------------------- TRACKING MOUSE -------------------

        int mouseX = GetMouseX() / sqSize;
        int mouseY = GetMouseY() / sqSize;
        int mouseOnBoard = 8 * mouseY + mouseX;
		keyPressed = GetKeyPressed();	
		
		if(keyPressed == KEY_N){
			drawNumber = !drawNumber;
			updateChessBoard(squareBoard, sprites);
		}
		GuiCheckBox((Rectangle){board_width+10,50,30,30}, "Draw Numbers", &drawNumber);
		if(drawNumber){
			updateChessBoard(squareBoard, sprites);
		}
		if(keyPressed == KEY_R 
			|| GuiButton((Rectangle){board_width+10,10,120,30}, "RESET")
		  ){
			initChessBoard(squareBoard);
			updateChessBoard(squareBoard, sprites);
		}
        if(IsMouseButtonPressed(0) 
			&& mouseX <=7
			&& mouseY <=7 
			&& mouseX >=0 
			&& mouseY >= 0) {
            if(squareBoard[mouseOnBoard] != 0 && piece_info == 0) {
                piece_info = squareBoard[mouseOnBoard];
                pos_piece_info = mouseOnBoard;
                squareBoard[mouseOnBoard] = 0;
				DrawText(piece_name(piece_info),board_width+10,90,20,GREEN);
                switch (piece_info) {

                    case (White | Pawn):
                        validateWhitePawn(all_possible_moves, mouseY, mouseX);
                        break;

                    case (White | Rook):
                        validateRook(all_possible_moves, mouseY, mouseX);
                        break;

                    case (White | Bishop):
                        validateBishop(all_possible_moves, mouseY, mouseX);
                        break;

                    case (White | Knight):
                        validateKnight(all_possible_moves, mouseY, mouseX);
                        break;

                    case (White | Queen):
                        validateQueen(all_possible_moves, mouseY, mouseX);
                        break;

                    case (White | King):
                        validateKing(all_possible_moves, mouseY, mouseX);
                        break;

                    case (Black | Pawn):
                        validateBlackPawn(all_possible_moves, mouseY, mouseX);
                        break;

                    case (Black | Rook):
                        validateRook(all_possible_moves, mouseY, mouseX);
                        break;

                    case (Black | Bishop):
                        validateBishop(all_possible_moves, mouseY, mouseX);
                        break;

                    case (Black | Knight):
                        validateKnight(all_possible_moves, mouseY, mouseX);
                        break;

                    case (Black | Queen):
                        validateQueen(all_possible_moves, mouseY, mouseX);
                        break;

                    case (Black | King):
                        validateKing(all_possible_moves, mouseY, mouseX);
                        break;

                }

                sortList();
                removeDupFromList();
                sortList();
                show();

                drawAllPossibleSquares();

            } else {

                if(isMoveInPossible(mouseOnBoard)) {

                    bool canGoThrought = false;

                    updateChessBoard(squareBoard, sprites);
                    switch (piece_info) {
                        case (White | Rook):
                            if(straightPiecesValidMoveCheck(mouseOnBoard, squareBoard) == true) {
                                canGoThrought = true;
                            }
                            break;

                        case (White | Bishop):
                            if(diagonalPiecesValidMoveCheck(mouseOnBoard, squareBoard)) {
                                canGoThrought = true;
                            }
                            break;

                        case (White | Queen):
                            if(straightPiecesValidMoveCheck(mouseOnBoard, squareBoard) || diagonalPiecesValidMoveCheck(mouseOnBoard, squareBoard)) {
                                canGoThrought = true;
                            }
                            break;

                        case (White | Pawn):
                            canGoThrought = true;
                            break;

                        case (White | King):
                            canGoThrought = true;
                            break;

                        case (White | Knight):
                            canGoThrought = true;
                            break;

                        case (Black | Rook):
                            if(straightPiecesValidMoveCheck(mouseOnBoard, squareBoard)) {
                                canGoThrought = true;
                            }
                            break;

                        case (Black | Bishop):
                            if(diagonalPiecesValidMoveCheck(mouseOnBoard, squareBoard)) {
                                canGoThrought = true;
                            }
                            break;

                        case (Black | Queen):
                            if(straightPiecesValidMoveCheck(mouseOnBoard, squareBoard) || diagonalPiecesValidMoveCheck(mouseOnBoard, squareBoard)) {
                                canGoThrought = true;
                            }
                            break;
                            
                        case (Black | Pawn):
                            canGoThrought = true;
                            break;
                            
                        case (Black | Knight):
                            canGoThrought = true;
                            break;
                            
                        case (Black | King):
                            canGoThrought = true;
                            break;

                    }

                    if(canGoThrought == true) {

                        squareBoard[mouseOnBoard] = 0;

                        updateChessBoard(squareBoard, sprites);

                        squareBoard[mouseOnBoard] = piece_info;

                        updateChessBoard(squareBoard, sprites);

                        fixHighlightedSquares();

                        updateChessBoard(squareBoard, sprites);

                    } else {

                    squareBoard[pos_piece_info] = piece_info;

                    fixHighlightedSquares();

                    updateChessBoard(squareBoard, sprites);

                    }

                } else {

                    squareBoard[pos_piece_info] = piece_info;
                }
                    fixHighlightedSquares();

                    updateChessBoard(squareBoard, sprites);

                piece_info = 0;
                pos_piece_info = -1;

            }
        }
        BeginDrawing();

        EndDrawing();

    }
	
	printf("Closing...\n");
    UnloadImage(sprites.black_king);
    UnloadImage(sprites.black_pawn);
    UnloadImage(sprites.black_rook);
    UnloadImage(sprites.black_bishop);
    UnloadImage(sprites.black_knight);
    UnloadImage(sprites.black_queen);

    UnloadImage(sprites.white_king);
    UnloadImage(sprites.white_pawn);
    UnloadImage(sprites.white_rook);
    UnloadImage(sprites.white_bishop);
    UnloadImage(sprites.white_knight);
    UnloadImage(sprites.white_queen);

    CloseWindow();

    return 0;

}
