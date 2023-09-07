int b_elem_in = 0;

void NE(int* all_possible_moves, int rank, int file) {
    for(int i = rank; i >= 0; i--) {
        if(file >= 0 && file <= 7) {
            all_possible_moves[b_elem_in] = (8 * i + file);
            file++;
            b_elem_in++;
        }
    }
}

void SW(int* all_possible_moves, int rank, int file) {
    for(int i = rank; i <= 7; i++) {
        if(file >= 0 && file <= 7) {
            all_possible_moves[b_elem_in] = (8 * i + file);
            file--;
            b_elem_in++;
        }
    }
}

void NW(int* all_possible_moves, int rank, int file) {
    for(int i = rank; i >= 0; i--) {
        if(file >= 0 && file <= 7) {
            all_possible_moves[b_elem_in] = (8 * i + file);
            file--;
            b_elem_in++;
        }
    }
}

void SE(int* all_possible_moves, int rank, int file) {
    for(int i = rank; i <= 7; i++) {
        if(file >= 0 && file <= 7) {
            all_possible_moves[b_elem_in] = (8 * i + file);
            file++;
            b_elem_in++;
        }
    }
}

int* validateBishop(int* all_possible_moves, int rank, int file) {

    SW(all_possible_moves, rank, file);
    NW(all_possible_moves, rank, file);
    SE(all_possible_moves, rank, file);
    NE(all_possible_moves, rank, file);

    for(int i = b_elem_in; i < 40; i++) {
        all_possible_moves[i] = -1;
    }

    b_elem_in = 0;

    return all_possible_moves;

}
