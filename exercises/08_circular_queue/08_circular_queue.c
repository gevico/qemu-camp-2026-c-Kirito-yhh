#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

void initQueue(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

void enqueue(Queue *q, People p) {
    q->data[q->tail] = p;
    q->tail = (q->tail + 1) % MAX_PEOPLE;
    q->count++;
}

People dequeue(Queue *q) {
    People p = q->data[q->head];
    q->head = (q->head + 1) % MAX_PEOPLE;
    q->count--;
    return p;
}

bool isEmpty(Queue *q) {
    return q->count == 0;
}

int main() {
    Queue q;
    int total_people=50;
    int report_interval=5;

    initQueue(&q);
    
    for (int i = 1; i <= total_people; i++) {
        People p;
        p.id = i;
        enqueue(&q, p);
    }
    
    while (q.count > 1) {
        for (int i = 1; i < report_interval; i++) {
            People p = dequeue(&q);
            enqueue(&q, p);
        }
        dequeue(&q);
    }
    
    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}