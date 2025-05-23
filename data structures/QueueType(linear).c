#include <stdio.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 5

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
// 큐 초기화
void init_queue(QueueType* q)
{
    q->rear = -1;
    q->front = -1;
}





// 포화 상태 확인
int is_full(QueueType* q)
{
    if (q ->rear == MAX_QUEUE_SIZE -1) return 1;
    else return 0;
}
// 공백 상태 확인
int is_empty(QueueType* q)
{
    if (q->front == q->rear) return 1;
    else return 0;
}






// 큐에 element 삽입
void enqueue(QueueType* q, element item)
{
    if (is_full(q)){
        error("큐가 포화상태입니다.");
        return;
    }
    q->data[++(q->rear)] = item;
}
// 큐에서 element 제거
element dequeue(QueueType* q)
{
    if(is_empty(q)){
        error("큐가 공백상태입니다.");
        return -1;
    }
    element item = q->data[++(q->front)];
    return item;
}





// 큐 내용 출력
void queue_print(QueueType* q)
{
    for (int i=0; i<MAX_QUEUE_SIZE; i++) {
        if (i <= q->front || i > q->rear) {
            printf("   | ");
        }
        else printf("%2d | ", q->data[i]);
    }
    printf("\n");
}






int main(void)
{
    return 0;
}

