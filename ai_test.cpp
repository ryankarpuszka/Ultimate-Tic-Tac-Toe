#include <gtest/gtest.h>
#include <memory>

#define private public

#include "game.h"
#include "ai.h"

TEST(AITest, TreeNodeTest)
{
    Game game = Game();
    std::shared_ptr<TreeNode> node = std::shared_ptr<TreeNode>(new TreeNode(std::weak_ptr<TreeNode>(), game.getWinner(), game.getMoves()));

    ASSERT_TRUE(node->isLeaf());
    ASSERT_EQ(node->winner, None);
    

    for (int i = 0; i < 81; ++i)
    {
        ASSERT_EQ(node->unexpandedMoves.size(), 81 - i);
        ASSERT_EQ(node->visits, i);
        std::shared_ptr<TreeNode> child = TreeNode::expandNode(node, game);
        child->backpropogate(1);
        
    }

    ASSERT_FALSE(node->isLeaf());

    std::shared_ptr<TreeNode> child = node->bestChild(game, true);
    ASSERT_FALSE(node == child);
    ASSERT_EQ(child->visits, 1);
    ASSERT_EQ(child->rewardSum, 1);
}