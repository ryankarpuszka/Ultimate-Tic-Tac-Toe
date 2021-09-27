#include "ai.h"

#include <iostream>
#include <math.h>
#include <memory>
#include <vector>

TreeNode::TreeNode(std::weak_ptr<TreeNode> parent,CellState winner, std::vector<CoordPair> unexpandedMoves)
{
    visits = 0;
    rewardSum = 0;
    this->parent = parent;
    this->winner = winner;
    this->unexpandedMoves = unexpandedMoves;
}

std::shared_ptr<TreeNode> TreeNode::getChild(const CoordPair& move) const
{
    return children.at(move);
}

double TreeNode::ucb(bool maximize) const
{
    if (parent.expired())
    {
        return 0;
    }

    double sum = maximize ? rewardSum : -rewardSum;
    return (sum / visits) + sqrt((2 * log(parent.lock()->visits)) / visits);
}

bool TreeNode::isLeaf() const
{
    return winner != None || !unexpandedMoves.empty();
}

std::shared_ptr<TreeNode> TreeNode::bestChild(Game& game, bool maximize)
{
    double bestUcb = -std::numeric_limits<double>::infinity();
    CoordPair bestMove;
    std::shared_ptr<TreeNode> bestChild;
    for (auto const& [move, child] : children)
    {
        double ucb = child->ucb(maximize);
        if (ucb > bestUcb)
        {
            bestUcb = ucb;
            bestMove = move;
            bestChild = child;
        }
    }

    game.move(bestMove);
    return bestChild;
}

CoordPair TreeNode::bestMove()
{
    int maxVisits = 0;
    CoordPair best;
    for (auto const& [move, child] : children)
    {
        int visits = child->visits;
        if (visits > maxVisits)
        {
            maxVisits = visits;
            best = move;
        }
    }
    return best;
}

std::shared_ptr<TreeNode> TreeNode::expandNode(std::shared_ptr<TreeNode> node, Game& game)
{
    // If there's already a winner, do not expand
    if (node->winner != None)
    {
        return node;
    }

    // Choose a random move that has not been expanded
    int idx = rand() % node->unexpandedMoves.size();
    CoordPair move = node->unexpandedMoves[idx];
    game.move(move);
    node->unexpandedMoves.erase(node->unexpandedMoves.begin() + idx);

    std::shared_ptr<TreeNode> newChild = std::make_shared<TreeNode>(std::weak_ptr<TreeNode>(node), game.getWinner(), game.getMoves());
    node->children.insert({move, newChild});

    return newChild;
}

void TreeNode::addOutcome(int outcome)
{
    ++visits;
    rewardSum += outcome;
}

void TreeNode::backpropogate(int outcome)
{
    addOutcome(outcome);

    if (!parent.expired())
    {
        std::shared_ptr<TreeNode> node = parent.lock();
        node->addOutcome(outcome);
        while (!node->parent.expired())
        {
            node = node->parent.lock();
            node->addOutcome(outcome);
        }
    }
}


AI::AI(std::shared_ptr<Game> game, CellState goal)
{
    this->game = game;
    this->goal = goal;

    root = std::shared_ptr<TreeNode>(new TreeNode(std::weak_ptr<TreeNode>(), game->getWinner(), game->getMoves()));
}

void AI::iterate()
{
    Game gameCopy = *game;

    std::shared_ptr<TreeNode> curNode = root;
    // Travel down the tree choosing the best child until a leaf is reached
    bool maximize = true;
    while (!curNode->isLeaf())
    {
        curNode = curNode->bestChild(gameCopy, maximize);
        maximize = !maximize;   // Switch maximize since turns are alternating
    }

    std::shared_ptr<TreeNode> expandedNode = TreeNode::expandNode(curNode, gameCopy);
    
    // From the new expanded node, simulate a random game until the game ends.
    while (gameCopy.getWinner() == None)
    {
        std::vector<CoordPair> moves = gameCopy.getMoves();
        if (moves.size() == 0)
        {
            break;
        }

        int idx = rand() % moves.size();
        gameCopy.move(moves[idx]);
    }

    // Calculate the outcome of the simulated game and backpropogate it from the expanded node
    int outcome;
    CellState winner = gameCopy.getWinner();
    if (winner == None)
    {
        outcome = 0;
    }
    else
    {
        outcome = winner == goal ? 1 : -1;
    }

    expandedNode->backpropogate(outcome);
}

CoordPair AI::getAIMove()
{
    for (int i = 0; i < AI_ITERATIONS; ++i)
    {
        iterate();
    }

    return root->bestMove();
}

void AI::updateTree(CoordPair move)
{
    std::shared_ptr<TreeNode> newRoot = root->getChild(move);
    root.swap(newRoot);
}