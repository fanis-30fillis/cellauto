#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "grid.h"	// utility header for drawGrid
#include <stdlib.h>
#include <conways.h>
#include <errno.h>
#include <stdio.h>
#include "time.h"
#include <wireworld.h>

void changeCellInPosition(struct cell* grid, int rows, int cols, int physicalRow, int physicalCol, int cellSize, bool newState) {
	int actualRow = physicalRow / cellSize;
	int actualCol = physicalCol / cellSize;
	grid[actualRow * cols + actualCol].alive = newState;
}

void enableCellInPosition(struct cell* grid, int rows, int cols, int physicalRow, int physicalCol, int cellSize) {
	changeCellInPosition(grid, rows, cols, physicalRow, physicalCol, cellSize, true);
}

void disableCellInPosition(struct cell* grid, int rows, int cols, int physicalRow, int physicalCol, int cellSize) {
	changeCellInPosition(grid, rows, cols, physicalRow, physicalCol, cellSize, false);
}

void runConwaysGame() {
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "Conways Game of Life in RayLib");

	int targetFps = 10;
	const uint64_t delay = 1000000000 / targetFps;
	int cellSize = 10;
	// Load a texture from the resources directory
	int screenWidth, screenHeight;
	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();
	struct cell* grid = (struct cell*)calloc((screenHeight * screenWidth) / cellSize, sizeof * grid);
	if (grid == NULL) {
		printf("Failed to allocate memory for grid\n");
		exit(ENOMEM);
	}
	random_seed(grid, screenHeight / cellSize, screenWidth / cellSize, 0.1);
	SetTargetFPS(60);	// set our game to run at 60 frames per second
	Vector2 mousePoint = { 0.0f, 0.0f };
	bool manualChange = false;         // Button action should be activated
	bool paused = false;
	uint64_t lastTime = 0;
	bool singleStep = false;
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	while (!WindowShouldClose())
	{
		// drawing
		BeginDrawing();
		ClearBackground(BLUE);
		mousePoint = GetMousePosition();
		manualChange = false;
		// We only care whether the mouse is down, that means that the user can click and drag to enable/disable cells
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			enableCellInPosition(grid, screenHeight / cellSize, screenWidth / cellSize, mousePoint.y, mousePoint.x, cellSize);
			manualChange = true;
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			disableCellInPosition(grid, screenHeight / cellSize, screenWidth / cellSize, mousePoint.y, mousePoint.x, cellSize);
			manualChange = true;
		}
		if (IsKeyPressed(KEY_SPACE)) {
			paused = !paused;
		}
		if (IsKeyReleased(KEY_RIGHT) && paused) {
			singleStep = true;
		}
		// detect resizes and change accordintly
		if (IsWindowResized()) { // ||&& !IsWindowFullscreen()
			int oldRows = screenHeight / cellSize;
			int oldCols = screenWidth / cellSize;
			const long oldSize = oldRows * oldCols;
			screenWidth = GetScreenWidth();
			screenHeight = GetScreenHeight();
			int newRows = screenHeight / cellSize;
			int newCols = screenWidth / cellSize;
			const long newSize = newRows * newCols;
			struct cell* newGrid = (struct cell*)calloc((screenHeight * screenWidth) / cellSize, sizeof * grid);

			if (newGrid == NULL) {
				printf("Failed to reallocate memory for grid\n");
				free(grid);
				exit(ENOMEM);
			}

			fixAfterResize(grid, newGrid, oldRows, oldCols, newRows, newCols);
			free(grid);
			grid = newGrid;
		}
		drawGrid(screenWidth, screenHeight, cellSize);
		drawCurrentState(grid, screenHeight / cellSize, screenWidth / cellSize, cellSize);
		uint64_t currentTime = monotonic_ns();
		// we won't draw if the game is paused, if the user is currently changing stuff and if the delay has not passed yet
		if ((!paused && !manualChange && currentTime - lastTime >= delay) || singleStep) {
			calculate_next_array_state(grid, screenHeight / cellSize, screenWidth / cellSize);
			set_next_state(grid, screenHeight / cellSize, screenWidth / cellSize);
			lastTime = currentTime;
			singleStep = false;
		}

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	free(grid);
	CloseWindow();
}

void runWireworldGame() {
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "Wireworld in RayLib");

	int targetFps = 10;
	const uint64_t delay = 1000000000 / targetFps;
	int cellSize = 10;
	// Load a texture from the resources directory
	int screenWidth, screenHeight;
	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();
	struct WireworldCell* grid = (struct WireworldCell*)calloc((screenHeight * screenWidth) / cellSize, sizeof * WireworldCell);
	if (grid == NULL) {
		printf("Failed to allocate memory for wireworld grid\n");
		exit(ENOMEM);
	}
	SetTargetFPS(60);	// set our game to run at 60 frames per second
	Vector2 mousePoint = { 0.0f, 0.0f };
	bool manualChange = false;         // Button action should be activated
	bool paused = false;
	uint64_t lastTime = 0;
	bool singleStep = false;
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	while (!WindowShouldClose())
	{
		// drawing
		BeginDrawing();
		ClearBackground(BLUE);
		mousePoint = GetMousePosition();
		manualChange = false;
		// We only care whether the mouse is down, that means that the user can click and drag to enable/disable cells
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			enableCellInPosition(grid, screenHeight / cellSize, screenWidth / cellSize, mousePoint.y, mousePoint.x, cellSize);
			manualChange = true;
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			disableCellInPosition(grid, screenHeight / cellSize, screenWidth / cellSize, mousePoint.y, mousePoint.x, cellSize);
			manualChange = true;
		}
		if (IsKeyPressed(KEY_SPACE)) {
			paused = !paused;
		}
		if (IsKeyReleased(KEY_RIGHT) && paused) {
			singleStep = true;
		}
		// detect resizes and change accordintly
		if (IsWindowResized()) { // ||&& !IsWindowFullscreen()
			int oldRows = screenHeight / cellSize;
			int oldCols = screenWidth / cellSize;
			const long oldSize = oldRows * oldCols;
			screenWidth = GetScreenWidth();
			screenHeight = GetScreenHeight();
			int newRows = screenHeight / cellSize;
			int newCols = screenWidth / cellSize;
			const long newSize = newRows * newCols;
			struct cell* newGrid = (struct cell*)calloc((screenHeight * screenWidth) / cellSize, sizeof * grid);

			if (newGrid == NULL) {
				printf("Failed to reallocate memory for grid\n");
				free(grid);
				exit(ENOMEM);
			}

			fixAfterResize(grid, newGrid, oldRows, oldCols, newRows, newCols);
			free(grid);
			grid = newGrid;
		}
		drawGrid(screenWidth, screenHeight, cellSize);
		drawCurrentState(grid, screenHeight / cellSize, screenWidth / cellSize, cellSize);
		uint64_t currentTime = monotonic_ns();
		// we won't draw if the game is paused, if the user is currently changing stuff and if the delay has not passed yet
		if ((!paused && !manualChange && currentTime - lastTime >= delay) || singleStep) {
			calculate_next_array_state(grid, screenHeight / cellSize, screenWidth / cellSize);
			set_next_state(grid, screenHeight / cellSize, screenWidth / cellSize);
			lastTime = currentTime;
			singleStep = false;
		}

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	free(grid);
	CloseWindow();
}

int main () {
	runConwaysGame();
	return 0;
}
