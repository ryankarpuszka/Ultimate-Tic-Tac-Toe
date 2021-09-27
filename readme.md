# Ultimate Tic-Tac-Toe
This is a command line implementation of Ultimate Tic-Tac-Toe. There is a tic-tac-toe board and within each cell is another smaller tic-tac-toe board. The goal is two win three small boards in a row. When a player makes a move, the next player is required to go in the small board of the cell the previous player played in. If a player sends their opponent to a board that is already won or has no spot available, they can move anywhere.

This includes a Monte Carlo Tree Search algorithm for the AI. The AI builds a game tree and randomly plays games to evaluate nodes.