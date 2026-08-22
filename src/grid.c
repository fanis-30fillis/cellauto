#include "raylib.h"
#include "grid.h"	// utility header for drawGrid

void drawGrid(int width, int height, int cellSize) {
	DrawLine(0, 0, width, 0, BLACK); // Top border
	DrawLine(0, 0, 0, height, BLACK); // Left border
	DrawLine(width, 0, width, height, BLACK); // Right border
	DrawLine(0, height, width, height, BLACK); // Bottom border
	for (int x = cellSize; x < width; x += cellSize) {
		DrawLine(x, 0, x, height, LIGHTGRAY); // Vertical lines
	}
	for (int y = cellSize; y < height; y += cellSize) {
		DrawLine(0, y, width, y, LIGHTGRAY); // Horizontal lines
	}
}