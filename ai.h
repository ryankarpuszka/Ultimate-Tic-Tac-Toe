#pragma once

#include "game.h"

#include <map>
#include <memory>
#include <vector>

#define AI_ITERATIONS 20000

/**
 * @brief A node in the search tree
 * 
 */
class TreeNode
{
private:
    /// Number of times this node has been visited
    int visits;
    /**The sum of the rewards from all times this node has been visited. Positive indicates a win for the AI.
     * Negative indicates a win for the opponent.
     */
    int rewardSum;
    /// The current outcome of the game at this node
    CellState winner;

    std::weak_ptr<TreeNode> parent;
    std::map<CoordPair, std::shared_ptr<TreeNode>> children;
    std::vector<CoordPair> unexpandedMoves;

public:
    TreeNode(std::weak_ptr<TreeNode> parent, CellState winner, std::vector<CoordPair> unexpandedMoves);
    
    /// Returns the child from the corresponding move
    std::shared_ptr<TreeNode> getChild(const CoordPair& move) const;
    
    /**
     * @brief Calculates the upper confidence bound at this node.
     * 
     * @param maximize Whether or not to maximize rewardSum. Set to true if this is an AI node, and false if this is an opponent node.
     * @return double The upper confidence bound
     */
    double ucb(bool maximize) const;

    /// Returns whether this node has a winner or has unexpanded moves
    bool isLeaf() const;

    /**
     * @brief Finds the best child to explore according to the ucb.
     * 
     * @param game The game matching the state at this node. This method will add the move to the best child to the given game.
     * @param maximize Whether or not to maximize rewardSum. Set to true if this is an AI node, and false if this is an opponent node.
     * @return std::shared_ptr<TreeNode> The best child
     */
    std::shared_ptr<TreeNode> bestChild(Game& game, bool maximize);

    /**
     * @brief Adds a new child to the given node that has not been expanded yet. If the game at this node already has a winner
     * then no child will be created an the same node will be returned.
     * 
     * @param node The node to expand
     * @param game The game matching the state at this node.
     * @return std::shared_ptr<TreeNode> The new child node
     */
    static std::shared_ptr<TreeNode> expandNode(std::shared_ptr<TreeNode> node, Game& game);

    /// Adds a new outcome from a simulation to this node
    void addOutcome(int outcome);

    /// Add a new outcome and propogate this outcome to all parent nodes
    void backpropogate(int outcome);

    /// Finds the move from this node that has been explored the most
    CoordPair bestMove();
};

class AI
{
private:
    std::shared_ptr<Game> game;
    CellState goal;

    std::shared_ptr<TreeNode> root;

    void iterate();

public:
    AI(std::shared_ptr<Game> game, CellState goal);

    /// Get the optimal AI move
    CoordPair getAIMove();

    /**
     * @brief Call this whenever a move is made in the game in order to keep the game tree up to date.
     * 
     * @param move The move that was just played.
     */
    void updateTree(CoordPair move);
};