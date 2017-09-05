#include "gridtools.h"

Grid::Grid() {
    total_width = total_height = rows = cols = x_offset = y_offset =
        cell_width = cell_height = 0;
}

void Grid::init(int w, int h, int r, int c, int x_offs, int y_offs) {
    total_width = w;
    total_height = h;
    rows = r;
    cols = c;
    x_offset = x_offs;
    y_offset = y_offs;

    cell_width = total_width / cols;
    cell_height = (total_height - y_offset) / rows;

    //                 int icon_width = cell_width * BORDER_PERCENT;
    //                 int icon_height = cell_heihgt * BORDER_PERCENT;

    for (int r = 0; r < rows; r++) {

        for (int c = 0; c < cols; ++c) {

            SDL_Rect rect;
            rect.x = c * cell_width;
            rect.y = (r * cell_height) + y_offset;
            rect.w = cell_width;
            rect.h = cell_height;

            cells.push_back(rect);
        }
    }
}

SDL_Rect Grid::getIconPosition(unsigned int index) {
    if (index < cells.size())
        return cells[index];

    return SDL_Rect();
}

GridManager::GridManager() {
    games = 0;
    width = height = rows = cols = 0;
    displayed_grid = 0;
    total_games = 0;
}

void GridManager::init(int screen_width, int screen_height, unsigned int r,
                       unsigned int c) {
    width = screen_width;
    height = screen_height;
    rows = r;
    cols = c;
    // create a defauld grid
    Grid g;
    g.init(width, height, rows, cols);
    grids.push_back(g);
}

SDL_Rect GridManager::getIconPosition(unsigned int index) {
    int which_grid = index / (rows * cols);

    int index_in_grid = index - ((which_grid)*rows * cols);

    return grids[which_grid].cells[index_in_grid];
}

int GridManager::getGrid(int index) { return index / (rows * cols); }

void GridManager::addGame() {
    if (++games > rows * cols) {

        // add a grid
        Grid g;
        g.init(width, height, rows, cols);
        grids.push_back(g);
        games = 1;
    }
    total_games++;
}

bool GridManager::onShiftRight() {
    if (displayed_grid + 1 >= (int)grids.size())
        return false;

    return true;
}

bool GridManager::onShiftLeft() {
    if (displayed_grid - 1 < 0)
        return false;

    return true;
}

int GridManager::plusGrid(int index) {
    return ((index + (rows * cols)) - cols) + 1;
}

int GridManager::minusGrid(int index) {
    return ((index - (rows * cols)) + cols - 1);
}

int GridManager::toBaseIndex(int idx) {
    return idx - (rows * cols) * getGrid(idx);
}

int GridManager::firstIndex() { return (rows * cols) * displayed_grid; }