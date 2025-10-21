#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define SIZE 8       /**< Board size. */

/**
 * Knight move offsets
 *
 * Moves that a knight can make relative to the current position.
 * For example,
 * x += MOVES_X[0]; y += MOVES_Y[0]
 * corresponds to one of the eight moves that a knight can make, where `x` and
 * `y` represent the current position.
 */
#define MOVE_COUNT 8 /**< Number of moves that a knight can make */
const int MOVES_X[MOVE_COUNT] = {2, 1, -1, -2, -2, -1, 1, 2};
const int MOVES_Y[MOVE_COUNT] = {1, 2, 2, 1, -1, -2, -2, -1};

/* Board type: 0 means unvisited; positive values indicate visit order. */
typedef int board_t[SIZE][SIZE];

/** Determines whether a move is possible from a starting position.
 *
 * @param move_id One of the 8 moves that the knight wants to make [0, 7]
 * @param x Current horizontal position
 * @param y Current vertical position
 * @param visited A two-dimensional array that represents the squares. If a
 * value is positive, then the corresponding field was visited before.
 * @return True if the move is possible, else false.
 */
bool move_is_possible(size_t move_id, size_t x, size_t y, board_t visited) {
    if (move_id >= MOVE_COUNT) return false;

    /* Use signed ints for intermediate computation to handle negative offsets. */
    int nx = (int)x + MOVES_X[move_id];
    int ny = (int)y + MOVES_Y[move_id];

    if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) {
        return false;
    }
    return visited[ny][nx] == 0;
}

/** Attempts a tour by picking the first accessible square.
 *
 * @param start_x Horizontal starting position on the board
 * @param start_y Vertical starting position on the board
 * @return The number of visited squares
 * @note An array is created for the attempt
 */
unsigned int tour_greedy(size_t start_x, size_t start_y) {
    board_t visited;
    memset(visited, 0, sizeof(visited));

    size_t x = start_x;
    size_t y = start_y;
    unsigned int step = 1;
    visited[y][x] = (int)step;

    /* Keep moving while we can find a first-accessible move. */
    while (1) {
        bool moved = false;
        for (size_t m = 0; m < MOVE_COUNT; ++m) {
            if (move_is_possible(m, x, y, visited)) {
                x = (size_t)((int)x + MOVES_X[m]);
                y = (size_t)((int)y + MOVES_Y[m]);
                visited[y][x] = (int)(++step);
                moved = true;
                break; /* Greedy: take the first valid move. */
            }
        }
        if (!moved) break;
    }

    return step; /* Number of squares visited. */
}

/** Attempts tours beginning from each square available on the board
 * and annotates the number of visited squares like this:
 *
 * 15  8 15 15 
 * 10  6  4 15 
 *  8 10 14 14 
 * 14 14 14 11 
 */
void greedy_tour_from_each_square() {
    for (size_t y = 0; y < SIZE; ++y) {
        for (size_t x = 0; x < SIZE; ++x) {
            unsigned int v = tour_greedy(x, y);
            /* Print with a minimum width so columns align (up to 3 digits for 8x8). */
            printf("%3u ", v);
        }
        printf("\n");
    }
}

int main(void) {
    /* Example: print the greedy tour length from each starting square. */
    greedy_tour_from_each_square();

    /* Optionally, demonstrate a single start position:
       size_t sx = 0, sy = 0;
       printf("\nGreedy tour from (0,0) visits %u squares.\n", tour_greedy(sx, sy));
    */
    return 0;
}
