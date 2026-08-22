#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct cell {
	bool alive;
	bool nextState;
};

void random_seed(struct cell** res, size_t rows, size_t cols, double aliveChance);
void clearCells(struct cell** res, size_t rows, size_t cols);
uint8_t get_live_neighbors(struct cell** res, size_t rows, size_t cols, int current_row, int current_col);
bool set_next_state(struct cell** res, size_t rows, size_t cols);
void calculate_next_array_state(struct cell** res, size_t rows, size_t cols);