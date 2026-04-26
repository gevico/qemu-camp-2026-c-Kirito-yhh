#include "simple_tree.h"
#include <stdio.h>
#include <limits.h>

void print_tree(TreeNode *root, int level) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%d\n", root->val);
    print_tree(root->left, level + 1);
    print_tree(root->right, level + 1);
}

int main() {
    // 测试用例2: 不完全二叉树
    int level_order2[] = {1, 2, 3, INT_MIN, 5, 4};
    int size2 = sizeof(level_order2) / sizeof(level_order2[0]);
    TreeNode *root2 = build_tree_by_level(level_order2, size2);
    printf("树的结构:\n");
    print_tree(root2, 0);
    printf("前序遍历: ");
    preorder_traversal(root2);
    printf("\n");
    free_tree(root2);
    
    return 0;
}