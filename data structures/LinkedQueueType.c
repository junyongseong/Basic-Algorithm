#include <stdio.h>
#include <stdlib.h>

typedef int element;        // 요소의 타입
typedef struct QueueNode{   // 큐의 노드의 타입
    element data;
    struct QueueNode* link;
}QueueNode;
typedef struct {            // 큐 ADT 구현
    QueueNode *front, *rear;
} LinkedQueueType;

// 큐 초기화 함수
void init(LinkedQueueType* q)
{
    q->front = q->rear = 0;
}
// 공백 상태 검출 함수
int is_empty(LinkedQueueType* q)
{
    return (q->front == NULL);
}
// 포화 상태 검출 함수
int is_full(LinkedQueueType* q, element data)
{
    return 0;
}
// 삽입 함수
void enqueue(LinkedQueueType* q, element data)
{
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->data = data;
    temp->link = NULL;
    if(is_empty(q)){
        q->front = temp;
        q->rear = temp;
    }
    else{
        q->rear->link = temp;
        q->rear = temp;
    }
}
// 삭제 함수
element dequeue(LinkedQueueType* q)
{
    QueueNode* temp = q->front;
    element data;
    if (is_empty(q)) {
        fprintf(stderr, "큐가 비어있음\n");
        exit(1);
    }
    else{
        data = temp->data;
        q->front = q->front->link;
        if (q->front == NULL)
            q->rear = NULL;
        free(temp);
        return data;
    }
}

void print_queue(LinkedQueueType* q)
{
    QueueNode* p;
    for(p = q->front; p != NULL; p = p->link)
        printf("%d->", p->data);
    printf("NULL\n");
}

// 연결된 큐 테스트 함수
int main(void)
{
    LinkedQueueType queue;
    
    init(&queue);           // 큐 초기화
    
    enqueue(&queue, 1);     print_queue(&queue);
    enqueue(&queue, 2);     print_queue(&queue);
    enqueue(&queue, 3);     print_queue(&queue);
    dequeue(&queue);        print_queue(&queue);
    dequeue(&queue);        print_queue(&queue);
    dequeue(&queue);        print_queue(&queue);
    
    return 0;
}

