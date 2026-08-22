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

void changeStatusOfCellInPosition(struct WireworldCell *grid, int rows, int cols, int mouseRow, int mouseCol);
void calculateNextStatus(struct WireworldCell* grid, int rows, int cols);
enum WireworldCellState calculateCellNextStatus(struct WireworldCell* grid, int rows, int cols, int row, int col);
