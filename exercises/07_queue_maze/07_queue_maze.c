#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_QUEUE 100

typedef struct {
    int row, col;
} Position;

Position queue[MAX_QUEUE];
int front = 0;
int rear = 0;

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

int visited[MAX_ROW][MAX_COL] = {0};
Position prev[MAX_ROW][MAX_COL];

// 方向：上、左、下、右（按此顺序探索以获得预期路径）
int dirs[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

void enqueue(Position pos) {
    queue[rear++] = pos;
}

Position dequeue() {
    return queue[front++];
}

int isEmpty() {
    return front == rear;
}

int main(void)
{
    Position start = {0, 0};
    Position end = {MAX_ROW - 1, MAX_COL - 1};
    
    enqueue(start);
    visited[start.row][start.col] = 1;
    prev[start.row][start.col] = (Position){-1, -1};
    
    while (!isEmpty()) {
        Position current = dequeue();
        
        if (current.row == end.row && current.col == end.col) {
            Position path[MAX_QUEUE];
            int pathLen = 0;
            Position p = current;
            while (p.row != -1 && p.col != -1) {
                path[pathLen++] = p;
                p = prev[p.row][p.col];
            }
            for (int i = pathLen - 1; i >= 0; i--) {
                printf("(%d, %d)\n", path[i].row, path[i].col);
            }
            return 0;
        }
        
        for (int i = 0; i < 4; i++) {
            int newRow = current.row + dirs[i][0];
            int newCol = current.col + dirs[i][1];
            
            if (newRow >= 0 && newRow < MAX_ROW && 
                newCol >= 0 && newCol < MAX_COL && 
                maze[newRow][newCol] == 0 && 
                visited[newRow][newCol] == 0) {
                Position next = {newRow, newCol};
                enqueue(next);
                visited[newRow][newCol] = 1;
                prev[newRow][newCol] = current;
            }
        }
    }
    
    printf("No path!\n");
	return 0;
}
