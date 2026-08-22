#pragma once
#include <stdint.h>

struct cell {
	bool alive;
	bool nextState;
};

void random_seed(struct cell* res, size_t rows, size_t cols, double aliveChance);
uint8_t get_live_neighbors(struct cell* res, size_t rows, size_t cols, int current_row, int current_col);