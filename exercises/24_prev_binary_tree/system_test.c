#include "simple_tree.h"
#include <stdio.h>
#include <limits.h>

int main() {
    // 测试用例1: 完全二叉树
    int level_order1[] = {1, 2, 3, 4, 5};
    int size1 = sizeof(level_order1) / sizeof(level_order1[0]);
    TreeNode *root1 = build_tree_by_level(level_order1, size1);
    preorder_traversal(root1);
    printf("\n");
    free_tree(root1);
    
    // 测试用例2: 不完全二叉树
    int level_order2[] = {1, 2, 3, INT_MIN, 5, 4};
    int size2 = sizeof(level_order2) / sizeof(level_order2[0]);
    TreeNode *root2 = build_tree_by_level(level_order2, size2);
    preorder_traversal(root2);
    printf("\n");
    free_tree(root2);
    
    return 0;
}