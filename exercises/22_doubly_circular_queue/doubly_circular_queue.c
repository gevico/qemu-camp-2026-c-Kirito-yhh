#include "doubly_circular_queue.h"

#include <stdlib.h>

// 头尾哨兵
static struct node tailsentinel;
static struct node headsentinel = {0, NULL, &tailsentinel};
static struct node tailsentinel = {0, &headsentinel, NULL};

static link head = &headsentinel;
static link tail = &tailsentinel;

link make_node(int data) {
    link p = (link)malloc(sizeof(struct node));
    if (p) {
        p->data = data;
        p->prev = NULL;
        p->next = NULL;
    }
    return p;
}

void free_node(link p) {
    if (p) {
        free(p);
    }
}

link search(int key) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
}

void insert(link p) {
    p->next = head->next;
    p->prev = head;
    head->next->prev = p;
    head->next = p;
}

void delete(link p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
    p->prev = NULL;
    p->next = NULL;
}

void traverse(void (*visit)(link)) {
    link p = head->next;
    while (p != tail) {
        visit(p);
        p = p->next;
    }
}

void destroy(void) {
    link p = head->next;
    while (p != tail) {
        link next = p->next;
        delete(p);
        free_node(p);
        p = next;
    }
}
