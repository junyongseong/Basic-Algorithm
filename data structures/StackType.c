#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100

typedef int elements;
typedef struct {
    elements data[MAX_STACK_SIZE];
    int top;
} StackType;

// 스택 초기화 함수
void init_stack(StackType *s)
{
    s->top = -1;
}






// 공백 상태 확인
int is_empty(StackType *s){
    return (s->top == -1);
}
// 포화 상태 확인
int is_full(StackType *s)
{
    return (s->top >= MAX_STACK_SIZE-1);
}





// 삽입함수
void push(StackType *s, elements item)
{
    if (is_full(s)) {
        fprintf(stderr, "스택 포화 에러\n");
        return;
    }
    else s->data[++(s->top)] = item;
}
// 삭제함수
elements pop(StackType *s)
{
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 에러\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}





// 미리보기함수
elements peek(StackType *s)
{
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 에러\n");
        exit(1);
    }
    else return s->data[s->top];
}




int main(void)
{
    return 0;
}
