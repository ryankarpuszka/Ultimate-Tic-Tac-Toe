#pragma once

#include <array>
#include <functional>
#include <vector>

template <typename Cell>
using BoardArray = std::array<std::array<Cell, 3>, 3>;

enum CellState {X, O, None};

/**
 * @brief A coordinate in a board
 */
struct Coord
{
    int row, col;

    bool operator==(const Coord& other) const
    {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Coord& other) const
    {
        return !(*this == other);
    }

    bool operator<(const Coord& other) const
    {
        if (row == other.row)
        {
            return col < other.col;
        }
        return row < other.row;
    }
};

constexpr Coord anyBoard = {-1, -1};

class SmallBoard
{
private:
    BoardArray<CellState> board;

public:
    SmallBoard();

    CellState getCell(int row, int col) const {return board[row][col];}

    /**
     * @brief Makes a move on the board. Returns whether the move was valid or not.
     * @param turn False for X, True for O
     * @param coord The coordinate to move at
    */
    bool move(bool turn, const Coord& coord);

    /// Return the current winner of the board
    CellState getWinner() const;

    /// Returns whether the board has a winner or all spaces are filled
    bool isDone() const;

    /// Returns all the coordinates where a player can move
    std::vector<Coord> getMoves() const;

    /**
     * @brief Prints a row of this board to standard output
     * @param row The row to print
     * @param showNone Whether to print '*' in blank spaces
     */
    void printRow(int row, bool showNone) const;
};

/**
 * @brief A pair of Coords that describes a single cell in the game.
 * 
 */
struct CoordPair
{
    Coord outer, inner;

    bool operator==(const CoordPair& other) const
    {
        return outer == other.outer && inner == other.inner;
    }

    bool operator<(const CoordPair& other) const
    {
        if (outer == other.outer)
        {
            return inner < other.inner;
        }
        return outer < other.outer;
    }
};

class Game
{
private:
    bool turn;
    Coord nextBoard;
    BoardArray<SmallBoard> board;

public:
    Game();

    /**
     * @brief Get the current turn
     * @return False for X, True for O
     */
    bool getTurn() const {return turn;};

    /**
     * @brief Get the coordinate of the large board the next player has to go in.
     * If the player can go in any board, then this will be equal to anyBoard
     */
    Coord getNextBoard() const {return nextBoard;};

    const SmallBoard& getBoard(const Coord& coord) const;

    /**
     * @brief Make a move at the specified coordinate pair
     * 
     * @return True if the move was valid.
     */
    bool move(const CoordPair& coord);

    /// Returns all the coordinate pairs where a player can move
    std::vector<CoordPair> getMoves() const;

    /// Return the current winner of the game
    CellState getWinner() const;

    /// Prints the whole game board to standart out
    void print() const;
};


template<class Cell>
CellState winner(const BoardArray<Cell>& board, std::function<CellState (Cell)> getCellState);
