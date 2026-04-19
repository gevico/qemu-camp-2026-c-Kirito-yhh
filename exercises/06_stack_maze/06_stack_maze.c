#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_STACK 100

typedef struct {
    int row, col;
} Position;

Position stack[MAX_STACK];
int top = -1;

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

int visited[MAX_ROW][MAX_COL] = {0};

// 方向：右、下、左、上
int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

void push(Position pos) {
    stack[++top] = pos;
}

Position pop() {
    return stack[top--];
}

int isEmpty() {
    return top == -1;
}

void printMaze() {
    printf("迷宫路径：\n");
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            if (visited[i][j] == 2) {
                printf("* ");
            } else if (maze[i][j] == 1) {
                printf("# ");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}

int main(void)
{
    Position start = {0, 0};
    Position end = {MAX_ROW - 1, MAX_COL - 1};
    
    push(start);
    visited[start.row][start.col] = 1;
    
    while (!isEmpty()) {
        Position current = stack[top];
        
        if (current.row == end.row && current.col == end.col) {
            for (int i = 0; i <= top; i++) {
                visited[stack[i].row][stack[i].col] = 2;
            }
            printMaze();
            printf("\n找到路径！\n");
            return 0;
        }
        
        int found = 0;
        for (int i = 0; i < 4; i++) {
            int newRow = current.row + dirs[i][0];
            int newCol = current.col + dirs[i][1];
            
            if (newRow >= 0 && newRow < MAX_ROW && 
                newCol >= 0 && newCol < MAX_COL && 
                maze[newRow][newCol] == 0 && 
                visited[newRow][newCol] == 0) {
                Position next = {newRow, newCol};
                push(next);
                visited[newRow][newCol] = 1;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            pop();
        }
    }
    
    printf("未找到路径！\n");
	return 0;
}