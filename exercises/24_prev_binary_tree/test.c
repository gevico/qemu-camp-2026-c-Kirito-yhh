#include "simple_tree.h"
#include <stdio.h>

int main() {
    int level_order[] = {1, 2, 3, 4, 5, 6, 7};
    int size = sizeof(level_order) / sizeof(level_order[0]);
    
    TreeNode *root = build_tree_by_level(level_order, size);
    
    printf("递归前序遍历: ");
    preorder_traversal(root);
    printf("\n");
    
    printf("迭代前序遍历: ");
    preorder_traversal_iterative(root);
    printf("\n");
    
    free_tree(root);
    
    return 0;
}