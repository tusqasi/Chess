#include <stdbool.h>
#include <stdio.h>
bool canCastleQueenSide(int board[64] ){
	for (int file = 4;file <7;file++) {
		for (int rank = 0; rank<8; rank++) {
			if(board[file+8*rank]  == 21 //   black rook
			  ||board[file+8*rank] == 22){ // black queen
				return false;
			} 
		}
	}
	int file = 1;
	for(int rank = 0; rank < 5; rank++){
		if(board[ file+8*rank]  == 20 //   black bishop
		  ||board[file+8*rank] == 22){ // black queen
			return false;
		} 
	}
	file = 2;
	for(int rank = 0; rank < 5; rank++){
		if(board[ file+8*rank]  == 20 //   black bishop
		  ||board[file+8*rank] == 22){ // black queen
			return false;
		} 
	}
	file = 3;
	for(int rank = 0; rank < 5; rank++){
		if(board[ file+8*rank]  == 20 //   black bishop
		  ||board[file+8*rank] == 22){ // black queen
			return false;
		} 
	}

	return true;
}
bool canCastleKingSide(int board[64] ){
	for (int rank = 0; rank<8; rank++) {
		if(board[1+8*rank]  == 21 //   black rook
		  ||board[1+8*rank] == 22){ // black queen
			return false;
		} 
	}
	for (int rank = 0; rank<8; rank++) {
		if(board[2+8*rank]  == 21 //   black rook
		  ||board[2+8*rank] == 22){ // black queen
			return false;
		} 
	}
	for (int rank = 0; rank<8; rank++) {
		if(board[3+8*rank]  == 21 //   black rook
		  ||board[3+8*rank] == 22){ // black queen
			return false;
		} 
	}
	for (int rank = 0; rank<8; rank++) {
		if(board[3+8*rank]  == 21 //   black rook
		  ||board[3+8*rank] == 22){ // black queen
			return false;
		} 
	}
	int file = 1;
	for(int rank = 0; rank < 5; rank++){
		if(board[ file+8*rank]  == 20 //   black bishop
		  ||board[file+8*rank] == 22){ // black queen
			return false;
		} 
	}
	file = 2;
	for(int rank = 0; rank < 5; rank++){
		if(board[ file+8*rank]  == 20 //   black bishop
		  ||board[file+8*rank] == 22){ // black queen
			return false;
		} 
	}
	file = 3;
	for(int rank = 0; rank < 5; rank++){
		if(board[ file+8*rank]  == 20 //   black bishop
		  ||board[file+8*rank] == 22){ // black queen
			return false;
		} 
	}

	return true;
}
int* validateKing(int* all_possible_moves, int board[64], int moved[6], int rank, int file) {

    int k_elem_in = 0;

    for(int i = rank - 1; i <= rank + 1; i++) {
        if(i <= 7 && i >= 0) {
            for(int j = file - 1; j <= file + 1; j++) {
                if(j <= 7 && j >= 0) {
                    all_possible_moves[k_elem_in] = (8 * i + j);
                    k_elem_in++;
                }
            }
        }
    }

    for(int i = k_elem_in; i < 32; i++) {
        all_possible_moves[i] = -1;
    }

	if(!moved[0]){	
	if(canCastleKingSide(board)){
		printf("King side castle possible\n");
		all_possible_moves[k_elem_in++] = 2 + 8*7;
		all_possible_moves[k_elem_in++] = 1 + 8*7;
	}
	if(canCastleQueenSide(board)){
		printf("Queen side castle possible\n");
		all_possible_moves[k_elem_in++] = 4 + 8*7;
		all_possible_moves[k_elem_in++] = 5 + 8*7;
	}
	}

    k_elem_in = 0;
    return all_possible_moves;

}

