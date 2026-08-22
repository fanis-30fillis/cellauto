#include "conways.h"

void random_seed(cell** res, size_t rows, size_t cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            cell* cell = (cell*)calloc(1, sizeof(cell));
            cell->alive = (rand() <= ALIVE_CHANCE);
            res[row][col] = cell;
        }
    }
}

void clearCells(cell** res, size_t rows, size_t cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (res[row][col]) {
				free(res[row][col]);
				res[row][col] = NULL;
            }
        }
    }
}

uint8_t get_live_neighbors(cell** res, size_t rows, size_t cols, int current_row, int current_col) {
    int lower_row_limit = current_row > 0 ? current_row - 1 : 0;
    int lower_col_limit = current_col > 0 ? current_col - 1 : 0;
    int upper_row_limit = current_row < rows - 1 ? current_row + 1 : current_row;
    int upper_col_limit = current_col < cols - 1 ? current_col + 1 : current_col;
    uint8_t number_of_live_neighbors = 0;
    for (int row = lower_row_limit; row <= upper_row_limit; row++) {
        for (int col = lower_col_limit; col <= upper_col_limit; col++) {
            number_of_live_neighbors += (current_col != col && current_row != row) && (res[row][col]->alive);
        }
    }
    return number_of_live_neighbors;
}

bool get_next_state(cell** res, size_t rows, size_t cols, int current_row, int current_col) {
    int live_neighbors = get_live_neighbors(res, rows, cols, current_row, current_col);
    bool current_state = res[current_row][current_col]->alive;
    return live_neighbors == 3 || current_state && live_neighbors == 2;
}

void calculate_next_array_state(cell **res, size_t rows, size_t cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            res[row][col]->nextState = get_next_state(res, rows, cols, row, col);
        }
    }
}

bool set_next_state(cell** res, size_t rows, size_t cols) {
    bool has_live_cells = false;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
			res[row][col]->alive = res[row][col]->nextState;
            has_live_cells |= res[row][col]->alive;
        }
    }
    return has_live_cells;
}
