#include <gtest/gtest.h>
#include "game.h"

TEST(GameTest, SmallBoardTest)
{
    SmallBoard board = SmallBoard();
    ASSERT_EQ(board.getWinner(), None);
    ASSERT_FALSE(board.isDone());

    ASSERT_TRUE(board.move(false, {0, 0}));
    ASSERT_TRUE(board.move(false, {0, 1}));
    ASSERT_TRUE(board.move(false, {0, 2}));

    ASSERT_EQ(board.getWinner(), X);
    ASSERT_TRUE(board.isDone());
    ASSERT_EQ(board.getMoves().size(), 6);

    ASSERT_FALSE(board.move(true, {0, 1}));
}

TEST(GameTest, SmallBoardWinnerTest)
{
    SmallBoard board = SmallBoard();

    ASSERT_TRUE(board.move(true, {0, 1}));
    ASSERT_TRUE(board.move(true, {1, 1}));
    ASSERT_TRUE(board.move(true, {2, 1}));

    ASSERT_EQ(board.getWinner(), O);
    ASSERT_TRUE(board.isDone());

    board = SmallBoard();

    ASSERT_TRUE(board.move(true, {0, 0}));
    ASSERT_TRUE(board.move(true, {1, 1}));
    ASSERT_TRUE(board.move(true, {2, 2}));

    ASSERT_EQ(board.getWinner(), O);
    ASSERT_TRUE(board.isDone());

    board = SmallBoard();

    ASSERT_TRUE(board.move(true, {0, 2}));
    ASSERT_TRUE(board.move(true, {1, 1}));
    ASSERT_TRUE(board.move(true, {2, 0}));

    ASSERT_EQ(board.getWinner(), O);
    ASSERT_TRUE(board.isDone());
}

TEST(GameTest, SmallBoardFull)
{
    SmallBoard board = SmallBoard();
    bool turn = false;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            board.move(turn, {row, col});
            turn = !turn;
        }
    }

    ASSERT_TRUE(board.isDone());
}

TEST(GameTest, GameTest)
{
    Game game = Game();
    ASSERT_EQ(game.getWinner(), None);
    ASSERT_EQ(game.getMoves().size(), 81);

    ASSERT_TRUE(game.move({{0, 0}, {1, 1}}));
    ASSERT_EQ(game.getMoves().size(), 9);

    ASSERT_FALSE(game.move({{2, 2}, {2, 2}}));
}

TEST(GameTest, Coord)
{
    Coord c1 = {1, 2};
    Coord c2 = {1, 2};

    ASSERT_EQ(c1, c2);
}