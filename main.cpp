#include <iostream>
#include <memory>
#include "game.h"
#include "ai.h"

int main(int, char**) {
    std::cout << "Hello, world!\n";

    SmallBoard board = SmallBoard();

    std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game());
    AI ai = AI(game, X);

    CoordPair aiMove = ai.getAIMove();
    game->move(aiMove);
    ai.updateTree(aiMove);
    
    while (game->getWinner() == None)
    {
        
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
            ai.updateTree(playerMove);
            CoordPair aiMove = ai.getAIMove();
            game->move(aiMove);
            ai.updateTree(aiMove);
        }
    }
}
