#include "game.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

SmallBoard::SmallBoard()
{
    // Initialize the board to None everywhere
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            board[i][j] = None;
        }
    }
}

bool SmallBoard::move(bool turn, const Coord& coord)
{
    if (board[coord.row][coord.col] == None)
    {
        board[coord.row][coord.col] = turn ? O : X;
        return true;
    }
    return false;
}

CellState SmallBoard::getWinner() const
{
    std::function<CellState (CellState)> f = [] (CellState cs) { return cs;};
    return winner(board, f);
}

bool SmallBoard::isDone() const
{
    if (getWinner() != None)
    {
        return true;
    }

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (board[row][col] == None)
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<Coord> SmallBoard::getMoves() const
{
    std::vector<Coord> moves;
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (board[row][col] == None)
            {
                moves.push_back({row, col});
            }
        }
    }
    return moves;
}

Game::Game()
{
    turn = false;
    nextBoard = anyBoard;
}

const SmallBoard& Game::getBoard(const Coord& coord) const
{
    return board[coord.row][coord.col];
}

bool Game::move(const CoordPair& coord)
{
    Coord outerCoord = coord.outer;
    Coord innerCoord = coord.inner;

    if (nextBoard == anyBoard || coord.outer == nextBoard)
    {
        bool valid = board[outerCoord.row][outerCoord.col].move(turn, innerCoord);
        if (valid)
        {
            // Set the next board to the inner coordinate if the next player can move there. Otherwise, they can move anywhere
            nextBoard = getBoard(innerCoord).isDone() ? anyBoard : innerCoord;
            turn = !turn;
        }
        return valid;
    }
    return false;
}

CellState Game::getWinner() const
{
    std::function<CellState (SmallBoard)> f = [] (SmallBoard cs) { return cs.getWinner();};
    return winner(board, f);
}

void addMoves(std::vector<CoordPair>& moves, const Coord& outCoord, const SmallBoard& smallBoard)
{
    std::vector<Coord> smallBoardMoves = smallBoard.getMoves();
    for (auto& it : smallBoardMoves)
    {
        moves.push_back({outCoord, it});
    }
}

std::vector<CoordPair> Game::getMoves() const
{
    std::vector<CoordPair> moves;
    if (nextBoard == anyBoard)
    {
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                addMoves(moves, {row, col}, board[row][col]);
            }
        }
    }
    else
    {
        addMoves(moves, nextBoard, getBoard(nextBoard));
    }
    return moves;
}

template<class Cell>
CellState winner(const BoardArray<Cell>& board, std::function<CellState (Cell)> getCellState)
{
    //Check rows
    for (int i = 0; i < 3; ++i)
    {
        bool xWin = true;
        bool oWin = true;

        for (int j = 0; j < 3; ++j)
        {
            if (getCellState(board[i][j]) != X)
            {
                xWin = false;
            }
            if (getCellState(board[i][j]) != O) {
                oWin = false;
            }
        }

        if (xWin)
        {
            return X;
        }
        else if (oWin)
        {
            return O;
        }
    }

    //Check cols
    for (int i = 0; i < 3; ++i)
    {
        bool xWin = true;
        bool oWin = true;

        for (int j = 0; j < 3; ++j)
        {
            if (getCellState(board[j][i]) != X)
            {
                xWin = false;
            }
            if (getCellState(board[j][i]) != O) {
                oWin = false;
            }
        }

        if (xWin)
        {
            return X;
        }
        else if (oWin)
        {
            return O;
        }
    }

    //Check diagonals
    CellState center = getCellState(board[1][1]);

    if (center != None)
    {
        if (center == getCellState(board[0][0]) && center == getCellState(board[2][2]))
        {
            return center;
        }

        if (center == getCellState(board[0][2]) && center == getCellState(board[2][0]))
        {
            return center;
        }
    }

    return None;
}

std::string stateToStr(CellState state, bool showNone)
{
    switch (state)
    {
        case X:
            return "X";
        case O:
            return "O";
        case None:
            return showNone ? "*" : " ";
    }
    return " ";
}

void SmallBoard::printRow(int row, bool showNone) const
{
    using namespace std;
    cout << " ";
    cout << stateToStr(board[row][0], showNone);
    cout << "|";
    cout << stateToStr(board[row][1], showNone);
    cout << "|";
    cout << stateToStr(board[row][2], showNone);
    cout << " ";
}

constexpr std::string_view smallRowSep =   " -+-+- | -+-+- | -+-+- ";
constexpr std::string_view bigRowSepEdge = "       |       |       ";
constexpr std::string_view bigRowSet =     "-------+-------+-------";

void Game::print() const
{
    using namespace std;

    for (int outRow = 0; outRow < 3; ++outRow)
    {
        for (int inRow = 0; inRow < 3; ++inRow)
        {
            for (int outCol = 0; outCol < 3; ++outCol)
            {
                Coord curCoord = {outRow, outCol};
                bool showNone = nextBoard == anyBoard || nextBoard == curCoord;
                board[outRow][outCol].printRow(inRow, showNone);
                if (outCol <= 1)
                {
                    cout << "|";
                }
            }

            cout << endl;

            if (inRow <= 1)
            {
                cout << smallRowSep << endl;
            }
        }

        if (outRow <= 1)
        {
            cout << bigRowSepEdge << endl;
            cout << bigRowSet << endl;
            cout << bigRowSepEdge << endl;
        }
    }
}