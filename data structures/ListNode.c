#include <stdio.h>
#include <stdlib.h>

typedef int element;
typedef struct ListNode{
    element data;
    struct ListNode* link;
}ListNode;

// 오류 처리 함수
void error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}




// 리스트 삽입 : 맨 처음
ListNode* insert_first(ListNode* head, element value)
{
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));
    p->data = value;
    p->link = head;
    head = p;
    return head;
}
// 리스트 삽입 : pre 노드 뒤에
ListNode* insert(ListNode* head, ListNode* pre, element value)
{
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));
    p->data = value;
    p->link = pre->link;
    pre->link = p;
    return head;
}
// 리스트 제거 : 맨 처음
ListNode* delete_first(ListNode* head)
{
    ListNode* removed;
    removed = head;
    head = removed->link;
    free(removed);
    return head;
}
// 리스트 제거 : pre 노드 뒤에
ListNode* delete(ListNode* head, ListNode* pre)
{
    ListNode* removed;
    removed = pre->link;
    pre->link = removed->link;
    free(removed);
    return head;
}




// 리스트 출력
void print_list(ListNode* head)
{
    ListNode* p;
    for (p = head; p != NULL; p = p->link) {
        printf("%d->", p->data);
    }
    printf("NULL \n");
}
// 리스트의 모든 요소 제거
ListNode* clear_list(ListNode* head)
{
    ListNode* removed;
    ListNode* pre = head;
    
    while (pre != NULL) {
        removed = pre;
        pre = pre->link;
        free(removed);
    }
    head = pre;
    return head;
}
/*=== LAB =====================================================================*/
// 특정한 값을 탐색하는 함수
ListNode* search_list(ListNode* head, element x)
{
    ListNode* p = head;
    
    while (p != NULL) {
        if(p->data == x) return p;
        p = p->list;
    }
    return NULL;
}
// 두개의 리스트를 하나로 합치는 함수 작성
ListNode* concat_list(ListNode* head1, ListNode* head2)
{
    if (head1 == NULL) return head2;
    else if (head2 == NULL) return head1;
    else{
        ListNode* p;
        p = head1;
        while (p->link != NULL)
            p = p->link;
        p->link = head2;
        return head1;
    }
}
// 리스트를 역순으로 만드는 연산
ListNode* reverse(ListNode* head)
{
    // 순회 포인터로 p q, r을 사용
    ListNode *p, *q, *r;
    
    p = head;   // p는 역순으로 만들 리스트
    q = NULL;   // q는 역순으로 만들 노드
    while (p != NULL) {
        r = q;  // r은 역순으로 된 리스트.
                // r는 q, q는 p를 차례로 따라간다.
        q = p;
        p = p->link;
        q->link = r;    // q의 링크 방향을 바꾼다.
    }
    return q;
}





int main(void)
{
    // 사용하려면
    // ListNode* head = NULL; 해서 사용해야함
    
    ListNode* head = NULL;
        
    for (int i=0; i<5; i++) {
        head = insert_first(head, i);
        print_list(head);
    }
    for(int i=0; i<5; i++){
        head = delete_first(head);
        print_list(head);
    }

    return 0;
}

