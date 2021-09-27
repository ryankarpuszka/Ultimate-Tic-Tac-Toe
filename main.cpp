#include <iostream>
#include <memory>
#include <string>
#include "game.h"
#include "ai.h"

int main(int, char**) {
    SmallBoard board = SmallBoard();

    std::shared_ptr<Game> game = std::make_shared<Game>();
    AI ai(game, X);

    CoordPair aiMove = ai.getAIMove();
    game->move(aiMove);
    ai.updateTree(aiMove);
    
    while (game->getWinner() == None)
    {

        if (game->getMoves().empty())
        {
            break;
        }
        
        game->print();

        int outRow, outCol, inRow, inCol;

        Coord nextBoard = game->getNextBoard();
        if (nextBoard == anyBoard)
        {
            std::cout << "Outer row number: ";
            std::cin >> outRow;
            std::cout << "Outer column number: ";
            std::cin >> outCol;
        }
        else
        {
            outRow = nextBoard.row;
            outCol = nextBoard.col;
        }

        std::cout << "Inner row number: ";
        std::cin >> inRow;
        std::cout << "Inner column number: ";
        std::cin >> inCol;

        CoordPair playerMove = {{outRow, outCol}, {inRow, inCol}};
        if (game->move(playerMove))
        {
            if (game->getMoves().empty())
            {
                break;
            }

            ai.updateTree(playerMove);
            CoordPair aiMove = ai.getAIMove();
            game->move(aiMove);
            ai.updateTree(aiMove);
        }
    }

    game->print();

    CellState winner = game->getWinner();
    if (winner == None)
    {
        std::cout << "Tie Game" << std::endl;
    }
    else
    {
        std::string winLetter = winner == X ? "X" : "O";
        std::cout << "The winner is " + winLetter << std::endl;
    }
}
