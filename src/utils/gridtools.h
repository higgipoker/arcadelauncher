#pragma once

#include <SDL.h>
#include <vector>

class Grid {
  public:
    Grid();
    void init(int w, int h, int r, int c, int x_offs = 0, int y_offs = 0);
    SDL_Rect getIconPosition(unsigned int index);

    int cell_width;
    int cell_height;
    std::vector<SDL_Rect> cells;

  private:
    int total_width, total_height, rows, cols, x_offset, y_offset;
};

class GridManager {
  public:
    GridManager();
    void init(int screen_width, int screen_height, unsigned int r,
              unsigned int c);
    SDL_Rect getIconPosition(unsigned int index);
    void addGame();
    int getGrid(int index);
    bool onShiftRight();
    bool onShiftLeft();
    int displayed_grid;
    int plusGrid(int index);
    int minusGrid(int index);

    int toBaseIndex(int idx);
    int firstIndex();
    int numberGrids() { return grids.size(); }

  private:
    std::vector<Grid> grids;
    int width, height, rows, cols;
    int games;
    unsigned int total_games;
};