#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_QUEUE_SIZE 11

typedef int element;
typedef struct {
    int front, rear;
    element data[MAX_QUEUE_SIZE];
} DequeType;

// 오류함수
void error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}
// 원형 큐 초기화
void init_queue(DequeType* q)
{
    q->rear = q->front = 0;
    
}





// 공백 상태 확인
int is_empty(DequeType* q)
{
    return (q->front == q->rear);
}
// 포화 상태 확인
int is_full(DequeType* q)
{
    return ((q->rear+1) % MAX_QUEUE_SIZE == q->front);
}






// 삽입 함수
void add_rear(DequeType* q, element item)
{
    if(is_full(q)) error("큐가 포화상태입니다.");
    q->rear = (q->rear +  1) % MAX_QUEUE_SIZE;
    q->data[q->rear] = item;
}
void add_front(DequeType* q, element item)
{
    if (is_full(q)) error("큐가 포화상태입니다.");
    q->data[q->front] = item;   // front 가 가리키고 있는 것은 비어있는 공간 : 그래서 바로 삽입
    q->front = (q->front -1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
}
// 삭제 함수
element delete_front(DequeType* q)
{
    if(is_empty(q)) error("큐가 공백상태입니다.");
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->data[q->front];
}
element delete_rear(DequeType* q){
    if (is_empty(q)) error("큐가 공백상태입니다.");
    int index = q->rear;
    q->rear = (q->rear -1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    return q->data[index];
}
// 살짝 꺼내 보기 함수 - 앞
element get_front(DequeType* q, element val)
{
    if (is_empty(q)) error("큐가 공백상태입니다.");
    return q->data[(q->front + 1) % MAX_QUEUE_SIZE];
}
// 살짝 꺼내 보기 함수 - 뒤
element get_rear(DequeType* q)
{
    if (is_empty(q)) error("큐가 공백상티입니다.");
    return q->data[q->rear];
}



// 큐 내용 출력
void deque_print(DequeType* q)
{
    printf("DEQUE(front=%d rear=%d) = ", q->front, q->rear);
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

