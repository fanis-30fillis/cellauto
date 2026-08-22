#include "wireworld.h"
#include <stdlib.h>
#include <raylib.h>

void changeStatusOfCellInPosition(struct WireworldCell* grid, int cols, int mouseRow, int mouseCol) {
    switch (grid[mouseRow * cols + mouseCol].state) {
    case EMPTY:
		grid[mouseRow * cols + mouseCol].state = CONDUCTOR;
        break;
    case HEAD:
		grid[mouseRow * cols + mouseCol].state = TAIL;
        break;
    case TAIL:
		grid[mouseRow * cols + mouseCol].state = EMPTY;
        break; 
    case CONDUCTOR:
		grid[mouseRow * cols + mouseCol].state = HEAD;
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

void wireworldSetNextState(struct WireworldCell* grid, int rows, int cols) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			grid[row * cols + col].state = grid[row * cols + col].nextState;
			grid[row * cols + col].nextState = EMPTY;
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

void wireworldDraw(struct WireworldCell* grid, int rows, int cols, int cellSize) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			struct WireworldCell cell = grid[row * cols + col];
			switch (cell.state) {
			case EMPTY:
				DrawRectangle(col * cellSize, row * cellSize + 1, cellSize - 2, cellSize - 2, BLUE);
				break;
			case CONDUCTOR:
				DrawRectangle(col * cellSize, row * cellSize + 1, cellSize - 2, cellSize - 2, YELLOW);
				break;
			case HEAD:
				DrawRectangle(col * cellSize, row * cellSize + 1, cellSize - 2, cellSize - 2, GREEN);
				break;
			case TAIL:
				DrawRectangle(col * cellSize, row * cellSize + 1, cellSize - 2, cellSize - 2, RED);
				break;
			}
		}
	}
}

void wireworldResizeGrid(struct WireworldCell* grid, struct WireworldCell* newGrid, int rows, int cols, int cellSize, int newRows, int newCols) {
	for (int oldRow = 0; oldRow < cols; oldRow++) {
		for (int oldCol = 0; oldCol < cols; oldCol++) {
			if (oldRow < newRows && oldCol < newCols) {
				const int oldSpace = oldRow * cols + oldCol;
				const int newSpace = oldRow * newCols + oldCol;
				newGrid[newSpace] = grid[oldSpace];
			}
		}
	}

}