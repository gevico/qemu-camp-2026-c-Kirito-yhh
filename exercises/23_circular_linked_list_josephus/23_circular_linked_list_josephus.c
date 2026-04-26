#include <stdio.h>
#include <stdlib.h>

#include "circular_linked_list.h"

// 使用环形链表模拟约瑟夫环并打印出列顺序
static void josephus_problem(int n, int k, int m) {
    if (n <= 0 || k <= 0 || m <= 0) {
        printf("参数错误\n");
        return;
    }

    Node* head = create_circular_list(n);
    if (!head) {
        printf("\n");
        return;
    }

    // prev 指向 current 的前一个节点
    Node* current = head;
    Node* prev = head;
    while (prev->next != head) prev = prev->next;

    // 起始位置移动到第 k 个
    for (int i = 1; i < k; ++i) {
        prev = current;
        current = current->next;
    }

    // 循环删除直到只剩一个节点
    while (current->next != current) {
        // 数 m-1 次，移动到第 m 个人
        for (int i = 1; i < m; ++i) {
            prev = current;
            current = current->next;
        }
        // 打印要出列的人
        printf("%d ", current->id);
        // 删除 current 指向的节点
        prev->next = current->next;
        free(current);
        current = prev->next;
    }
    // 打印最后一个人
    printf("%d", current->id);
    
    free_list(current);
    printf("\n");
}

int main(void) {
    josephus_problem(5, 1, 2);  // 输出结果：2 4 1 5 3
    josephus_problem(7, 3, 1);  // 输出结果：3 4 5 6 7 1 2
    josephus_problem(9, 1, 8);  // 输出结果：8 7 9 2 5 4 1 6 3
    return 0;
}
