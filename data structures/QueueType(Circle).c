#include <stdio.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 11

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
    if(is_full(q)) error("포화상태입니다");
    q->rear = (q->rear +  1) % MAX_QUEUE_SIZE;
    q->data[q->rear] = item;
}
// 원형 큐 element 빼기
element dequeue(QueueType* q)
{
    if(is_empty(q)) error("큐가 공백상태입니다.");
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->data[q->front];
}
// 살짝 꺼내 보기
element peeek(QueueType* q){
    if(is_empty(q)) error("큐가 공백상태입니다");
    return q->data[(q->front + 1) % MAX_QUEUE_SIZE];
}





// 원형 큐 출력
void queue_print(QueueType* q)
{
    printf("QUEUE(front=%d rear=%d) = ", q->front, q->rear);
    if (!is_empty(q)) {
        int i = q->front;
        do{
            i = (i+1) % MAX_QUEUE_SIZE;
            printf("%2d | ", q->data[i]);
            if(i == q->rear) break;
        } while(i != q->front);
    }
    printf("\n");
}




int main(void)
{
    return 0;
}

