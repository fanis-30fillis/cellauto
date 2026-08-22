#include "conways.h"
#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Seeds the grid with random live cells.
 * @param res The grid to seed.
 * @param rows The number of rows in the grid.
 * @param cols The number of columns in the grid.
 * @param aliveChance The probability that a cell will be alive.
 */
void random_seed(struct cell* res, size_t rows, size_t cols, double aliveChance) {
    srand((unsigned)time(NULL));

    for (int row = 0; row < rows; row++) {
        const int currentIndex = row * cols;
        for (int col = 0; col < cols; col++) {
            const bool willBeAlive = (rand() / (double)RAND_MAX) < aliveChance;
            res[currentIndex + col].alive = willBeAlive;
            res[currentIndex + col].nextState = false;
        }
    }
}

/**
 * Gets the number of live neighbors for a cell.
 * @param res The grid containing the cells.
 * @param rows The number of rows in the grid.
 * @param cols The number of columns in the grid.
 * @param current_row The row index of the cell.
 * @param current_col The column index of the cell.
 * @return The number of live neighbors.
 */
uint8_t get_live_neighbors(struct cell* res, size_t rows, size_t cols, int current_row, int current_col) {
    int lower_row_limit = max(current_row - 1, 0);
    int lower_col_limit = max(current_col - 1, 0);
    int upper_row_limit = min(current_row + 1, (int)rows - 1);
    int upper_col_limit = min(current_col + 1, (int)cols - 1);
    uint8_t number_of_live_neighbors = 0;
    for (int row = lower_row_limit; row <= upper_row_limit; row++) {
        for (int col = lower_col_limit; col <= upper_col_limit; col++) {
            if ((current_col != col || current_row != row) && (res[row * cols + col].alive)) {
                number_of_live_neighbors++;
            }
        }
    }
    return number_of_live_neighbors;
}

/**
 * Determines the next state of a cell based on its current state and the number of live neighbors.
 * @param res The grid containing the cells.
 * @param rows The number of rows in the grid.
 * @param cols The number of columns in the grid.
 * @param current_row The row index of the cell.
 * @param current_col The column index of the cell.
 * @return The next state of the cell.
 */
bool get_next_state(struct cell* res, size_t rows, size_t cols, int current_row, int current_col) {
    int live_neighbors = get_live_neighbors(res, rows, cols, current_row, current_col);
    bool current_state = res[current_row * cols + current_col].alive;
    return live_neighbors == 3 || current_state && live_neighbors == 2;
}

/**
 * Calculates the next state for all cells in the grid.
 * @param res The grid containing the cells.
 * @param rows The number of rows in the grid.
 * @param cols The number of columns in the grid.
 */
void calculate_next_array_state(struct cell* res, size_t rows, size_t cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            res[row * cols + col].nextState = get_next_state(res, rows, cols, row, col);
        }
    }
}

/**
 * Sets the next state of all cells in the grid.
 * @param res The grid containing the cells.
 * @param rows The number of rows in the grid.
 * @param cols The number of columns in the grid.
 * @return True if there are any live cells, false otherwise.
 */
bool set_next_state(struct cell* res, size_t rows, size_t cols) {
    bool has_live_cells = false;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            res[row * cols + col].alive = res[row * cols + col].nextState;
            res[row * cols + col].nextState = false;
            has_live_cells |= res[row * cols + col].alive;
        }
    }
    return has_live_cells;
}

/**
 * Draws the current state of the grid.
 * @param res The grid containing the cells.
 * @param rows The number of rows in the grid.
 * @param cols The number of columns in the grid.
 * @param cellSize The size of each cell.
 */
void drawCurrentState(struct cell* res, size_t rows, size_t cols, int cellSize) {
    int physicalRow = 0;
    for (int row = 0; row < rows; row++) {
        int physicalCol = 0;
        for (int col = 0; col < cols; col++) {
            // the cell size must be twice the offset so it's dead center
            if (res[row * cols + col].alive) {
                DrawRectangle(physicalCol + 1, physicalRow + 1, cellSize - 2, cellSize - 2, GREEN);
            }
            else {
                DrawRectangle(physicalCol + 1, physicalRow + 1, cellSize - 2, cellSize - 2, BLUE);
            }
            physicalCol += cellSize;
        }
        physicalRow += cellSize;
    }
}

void fixAfterResize(struct cell* oldGrid, struct cell* newGrid, int oldRows, int oldCols, int newRows, int newCols) {
    for (int oldRow = 0; oldRow < oldRows; oldRow++) {
        for (int oldCol = 0; oldCol < oldCols; oldCol++) {
            if (oldRow < newRows && oldCol < newCols) {
                const int oldSpace = oldRow * oldCols + oldCol;
                const int newSpace = oldRow * newCols + oldCol;
                newGrid[newSpace] = oldGrid[oldSpace];
            }
        }
    }
}
