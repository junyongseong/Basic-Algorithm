//
//  GraphType_Mat.c
//  algorithm
//
//  Created by 김건아 on 2022/10/20.
//
// 그래프 자료구조 (행렬)
//

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/*================== 큐 자료구조 =======================================*/
#define MAX_QUEUE_SIZE 10

typedef int element;
typedef struct {
    int front;
    int rear;
    element data[MAX_QUEUE_SIZE];
} QueueType;

// 오류함수
void error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}
// 원형 큐 초기화
void init_queue(QueueType* q)
{
    q->rear = q->front = 0;
}
// 공백 상태 확인
int is_empty(QueueType* q)
{
    return (q->front == q->rear);
}
// 포화 상태 확인
int is_full(QueueType* q)
{
    return ((q->rear+1) % MAX_QUEUE_SIZE == q->front);
}
// 원형 큐 element 삽입
void enqueue(QueueType* q, element item)
{
    if (is_full(q)) error("큐가 포화상태입니다.");
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->data[q->rear] = item;
}
// 원형 큐 element 제거
element dequeue(QueueType* q)
{
    if (is_empty(q)) error("큐가 공백상태입니다.");
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->data[q->front];
}

/*================== 그래프 타입 (행렬) 자료구조 =======================================*/
#define MAX_VERTICES 50

typedef struct GraphType {
    int n;
    int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;

// 그래프 초기화
void init_graph(GraphType* g)
{
    g->n = 0;
    for (int r=0; r<MAX_VERTICES; r++)
        for (int c=0; c<MAX_VERTICES; c++)
            g->adj_mat[r][c] = 0;
}
// 정점 삽입 연산
void insert_vertex(GraphType* g, int v)  // v는 정점의 이름이었구나.. 하지만 내맘대로 수정..!
{
    if (((g->n) + 1) > MAX_VERTICES){
        fprintf(stderr, "그래프: 정점의 개수 초과\n");
        return;
    }
    g->n++;
}
// 간선 삽입 연산
void insert_edge(GraphType* g, int start, int end)
{
    if (start >= g->n || end >= g->n) {
        fprintf(stderr, "그래프: 정점 번호 오류\n");
        return;
    }
    g->adj_mat[start][end] = 1;
    g->adj_mat[end][start] = 1;
}
// 인접 행렬 출력 함수
void print_adj_mat(GraphType* g)
{
    for (int i=0; i<g->n; i++){
        for (int j=0; j<g->n; j++)
            printf("%2d ", g->adj_mat[i][j]);
        printf("\n");
    }
}


int visited[MAX_VERTICES];  // 전역변수 자동 0 초기화

// 인접 행렬로 표현된 그래프에 대한 깊이 우선 탐색
void dfs_mat(GraphType* g, int v)
{
    visited[v] = TRUE;          // 정점 v의 방문 표시
    printf("정점: %d -> ", v);   // 방문한 정점 출력
    for (int w=0; w<g->n;  w++)
        if (g->adj_mat[v][w] && !visited[w])
            dfs_mat(g, w);      // 정점 w에서 DFS 새로 시작
}
// 인접 행렬로 표현된 그래프에 대한 너비 우선 탐색
void bfs_mat(GraphType* g, int v)
{
    QueueType* q;
    q = (QueueType*)malloc(sizeof(QueueType));
    init_queue(q);      // 큐 초기화
    
    visited[v] = TRUE;  // 정점 v 방문 표시
    printf("%d 방문 -> ", v);
    enqueue(q, v);      // 시작 정점을 큐에 저장
    while (!is_empty(q)) {
        v = dequeue(q);                 // 큐에 정점 추출
        for (int w=0; w<g->n; w++) {    // 인접 정점 탐색
            if (g->adj_mat[v][w] && !visited[w]) {
                visited[w] = TRUE;      // 방문 표시
                printf("%d 방문 -> ", w);
                enqueue(q, w);          // 방문한 정점을 큐에 저장
            }
        }
    }
    free(q);
}

int main(void)
{
    GraphType* g;
    
    g = (GraphType*)malloc(sizeof(GraphType));
    init_graph(g);
    
    for (int i=0; i<6; i++)
        insert_vertex(g, i);
    
    // A B C D E F
    // 0 1 2 3 4 5
    insert_edge(g, 0, 2);
    insert_edge(g, 0, 4);
    insert_edge(g, 2, 1);
    insert_edge(g, 2, 3);
    insert_edge(g, 2, 5);
    insert_edge(g, 1, 5);
    insert_edge(g, 3, 5);
    insert_edge(g, 4, 5);
    
    printf("너비 우선 탐색\n");
    bfs_mat(g, 0);
    printf("\n");
    free(g);
    
    return 0;
}
