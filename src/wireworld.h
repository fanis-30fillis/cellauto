enum WireworldCellState {
    EMPTY,
    CONDUCTOR,
    HEAD,
    TAIL
};

struct WireworldCell {
    enum WireworldCellState state;
    enum WireworldCellState nextState;
};

void changeStatusOfCellInPosition(struct WireworldCell *grid, int cols, int mouseRow, int mouseCol);
void calculateNextStatus(struct WireworldCell* grid, int rows, int cols);
enum WireworldCellState calculateCellNextStatus(struct WireworldCell* grid, int rows, int cols, int row, int col);
void wireworldSetNextState(struct WireworldCell* grid, int rows, int cols);
void wireworldDraw(struct WireworldCell* grid, int rows, int cols, int cellSize);
void wireworldResizeGrid(struct WireworldCell* grid, struct WireworldCell* newGrid, int rows, int cols, int cellSize, int newRows, int newCols);