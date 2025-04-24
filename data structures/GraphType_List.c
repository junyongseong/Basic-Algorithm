//
//  GraphType_List.c
//  algorithm
//
//  Created by 김건아 on 2022/10/20.
//
// 그래프 자료구조 (리스트)
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

/*================== 그래프 타입 (리스트) 자료구조 =======================================*/
#define MAX_VERTICES 50

typedef struct GraphNode{
    int vertex;
    struct GraphNode* link;
} GraphNode;
typedef struct {
    int n;  // 정점의 개수
    GraphNode* adj_list[MAX_VERTICES];    // 아마 이게 헤더노드인듯
} GraphType;

// 그래프 초기화
void init_graph(GraphType* g)
{
    g->n = 0;
    for (int v=0; v<MAX_VERTICES; v++)
        g->adj_list[v] = NULL;
}
// 정점 삽입 연산
void insert_vertex(GraphType* g, int v)
{
    if (((g->n) + 1) > MAX_VERTICES){
        fprintf(stderr, "그래프: 정점의 개수 초과");
        return;
    }
    g->n++;
}
// 간선 삽입 연산, v를 u의 인접 리스트에 삽입한다.
void insert_edge(GraphType* g, int u, int v)
{
    GraphNode* node;
    if (u >= g->n || v >= g->n) {
        fprintf(stderr, "그래프: 정점 번호 오류\n");
        return;
    }
    node = (GraphNode*)malloc(sizeof(GraphNode));
    node->vertex = v;
    node->link = g->adj_list[u];
    g->adj_list[u] = node;
}
// 인접 리스트 출력
void print_adj_list(GraphType* g)
{
    for (int i=0; i<g->n; i++) {
        printf("정점 %d의 인접 리스트 ", i);
        GraphNode* p = g->adj_list[i];
        while (p!=NULL) {
            printf("-> %d ", p->vertex);
            p = p->link;
        }
        printf("\n");
    }
}


int visited[MAX_VERTICES];  // 전역변수 자동 0 초기화

// 인접 리스트로 표현된 그래프에 대한 깊이 우선 탐색
void dfs_list(GraphType* g, int v)
{
    /*
        깊이 우선 탐색에서 "작은 값"을 기준으로 차례대로 깊이 우선 탐색을 진행하고 싶다면,
        연결리스트 g->adj_list[v] 가 작은 값으로 정렬되어 있어야 한다.
        연결리스트를 따라가며 정렬하는 방식이라, 연결리스트가 정렬된 상태여야 한다.
     
        아니면,
        애초에 데이터를 삽입할때 정렬된 데이터를 집어넣어 줘야 한다.
     */
    GraphNode* w;
    visited[v] = TRUE;
    printf("정점 %d -> ", v);
    for (w = g->adj_list[v]; w; w = w->link)
        if (!visited[w->vertex])
            dfs_list(g, w->vertex);
}
// 인접 리스트로 표현된 그래프에 대한 너비 우선 탐색
void bfs_list(GraphType* g, int v)
{
    /*
        너비 우선 탐색에서 "작은 값"을 기준으로 차례대로 너비 우선 탐색을 진행하고 싶다면,
        연결리스트 g->adj_list[v] 가 작은 값으로 정렬되어 있어야 한다.
        연결리스트를 따라가며 정렬하는 방식이라, 연결리스트가 정렬된 상태여야 한다.
     
        아니면,
        애초에 데이터를 삽입할때 정렬된 데이터를 집어넣어 줘야 한다.
     */
    GraphNode* w;
    QueueType* q;
    q = (QueueType*)malloc(sizeof(QueueType));
    init_queue(q);          // 큐 초기화
    
    visited[v] = TRUE;      // 정점 v 방문 표시
    printf("%d 방문 -> ", v);
    enqueue(q, v);          // 시작 정점을 큐에 저장
    while (!is_empty(q)) {
        v = dequeue(q);     // 큐에 저장된 정점 선택
        for (w=g->adj_list[v]; w; w=w->link) {      // 인접 정점 탐색
            if (!visited[w->vertex]) {              // 미방문 정점 탐색
                visited[w->vertex] = TRUE;          // 방문 표시
                printf("%d 방문 -> ", w->vertex);
                enqueue(q, w->vertex);              // 정점을 큐에 삽입
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
    bfs_list(g, 0);
    printf("\n");
    free(g);    // 이렇게 하면 메모리 누수 발생.. free_GraphType() 만들어줘야 함.
    
    return 0;
}
