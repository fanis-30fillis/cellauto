#include "wireworld.h"
#include <stdlib.h>

void changeStatusOfCellInPosition(struct WireworldCell* grid, int rows, int cols, int mouseRow, int mouseCol) {
    struct WireworldCell cell = grid[mouseRow * cols + mouseCol];
    switch (cell.state) {
    case EMPTY:
		cell.state = CONDUCTOR;
        break;
    case HEAD:
		cell.state = TAIL;
        break;
    case TAIL:
		cell.state = EMPTY;
        break; 
    case CONDUCTOR:
		cell.state = HEAD;
        break;
    }

}

void calculateNextStatus(struct WireworldCell* grid, int rows, int cols) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			grid[row * cols + col].nextState = calculateCellNextStatus(grid, rows, cols, row, col);
		}
	}
}

enum WireworldCellState calculateCellNextStatus(struct WireworldCell* grid, int rows, int cols, int currentRow, int currentCol) {
    struct WireworldCell cell = grid[currentRow * cols + currentCol];
    // conductor state is fall through
    switch (cell.state) {
    case EMPTY:
        return EMPTY;
    case HEAD:
        return TAIL;
    case TAIL:
        return CONDUCTOR;
    case CONDUCTOR:
    }
	int numOfHeadNeighbors = 0;
    int lower_row_limit = max(currentRow - 1, 0);
    int lower_col_limit = max(currentCol - 1, 0);
    int upper_row_limit = min(currentRow + 1, (int)rows - 1);
    int upper_col_limit = min(currentCol + 1, (int)cols - 1);
    for (int row = lower_row_limit; row <= upper_row_limit; row++) {
        for (int col = lower_col_limit; col <= upper_col_limit; col++) {
			if (row == currentRow && col == currentCol) {
				continue;
			}
			if (grid[row * cols + col].state == HEAD) {
                numOfHeadNeighbors++;           
			}
        }
    }
	if (numOfHeadNeighbors == 1 || numOfHeadNeighbors == 2) {
		return HEAD;
	}
	return CONDUCTOR;
}