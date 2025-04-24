//
//  TreeNode_test.c
//  algorithm
//
//  Created by 김건아 on 2022/10/10.
//
// 이진 트리 자료구조 - 삽입, 삭제, 탐색, 그 외...
//

#include <stdio.h>
#include <stdlib.h>

// 자료구조
typedef int element;
typedef struct TreeNode{
    element key;
    struct TreeNode *left, *right;
} TreeNode;

// 에러 처리
void error_print(char* s)   {fprintf(stderr, "%s\n", s);}
void error_exit(char* s)    {fprintf(stderr, "%s\n", s); exit(1);}

/*================== 노드 생성, 삽입 =======================================*/

// 새로운 노드 생성
TreeNode* new_node(element item)
{
    TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

// 노드 삽입 함수(순환)
TreeNode* insert_node_recursion(TreeNode* node, element key)
{
    // 트리가 공백이면 새로운 노드를 반환한다
    if (node == NULL) return new_node(key);
    
    // 그렇지 않으면 순환적으로 트리를 내려간다
    if (key < node->key)
        node->left = insert_node_recursion(node->left, key);
    else if (key > node->key)
        node->right = insert_node_recursion(node->right, key);
    
    // 변경된 루트 포인터를 반환한다.
    return node;
}
// 노드 삽입 함수(반복)
TreeNode* insert_node_repeat(TreeNode* node, element key)
{
    TreeNode* p;    // 부모 노드
    TreeNode* t;    // 자식 노드
    TreeNode* n;    // 새로운 노드
    
    t = node;
    p = NULL;
    
    // 탐색을 먼저 수행
    while (t != NULL) {
        if (key == t->key) return node;  // 동일한 데이터 존재시 삽입 종료.
        p=t;
        if (key < t->key) t = t->left;
        else t = t->right;
    }
    
    // 노드생성, 초기화
    n = (TreeNode*)malloc(sizeof(TreeNode));
    if (n == NULL) {
        error_print("메모리가 부족하여, 노드가 생성되지 않았습니다.");
        return node;
    }
    n->key = key;
    n->left = n->right = NULL;
    
    // 트리에 노드 추가
    if (p == NULL) node = n;   // 트리에 값이 처음 삽입된 경우
    else {
        if (key < p->key) p->left = n;
        else p->right = n;
    }
    
    return node;
}

/*================== 노드 삭제, 이진트리 해제 =======================================*/

// 최소값 노드 구하기
TreeNode* min_value_node(TreeNode* node)
{
    TreeNode* current = node;
    // 맨 왼쪽 단말 노드를 찾으러 내려감
    while (current->left)
        current = current->left;
    return current;
}

// 최대값 노드 구하기
TreeNode* max_value_node(TreeNode* node)
{
    TreeNode* current = node;
    // 맨 오른쪽 단말 노드를 찾으러 내려감
    while (current->right)
        current = current->right;
    return current;
}

// 모든 노드 삭제
TreeNode* delete_all_node(TreeNode* node)
{
    if (node == NULL)   return NULL;
    if (node->left)     node->left = delete_all_node(node->left);
    if (node->right)    node->right = delete_all_node(node->right);
    if (node->left == NULL && node->right == NULL) free(node);
    return NULL;
}

// 노드 삭제
TreeNode* delete_node(TreeNode* node, int key)
{
    if (node == NULL) return node;
    
    // 만약 키가 루트보다 작으면 왼쪽 서브 트리에 있는 것임
    if (key < node->key)
        node->left = delete_node(node->left, key);
    // 만약 키가 루트보다 크면 오른쪽 서브 트리에 있는 것임
    else if (key > node->key)
        node->right = delete_node(node->right, key);
    
    // 키가 루트와 같으면 이 노드를 삭제하면 됨
    else{
        // 첫번째나 두 번째 경우
        if (node->left == NULL){
            TreeNode* temp = node->right;
            free(node);
            return temp;
        }
        else if(node->right == NULL){
            TreeNode* temp = node->left;
            free(node);
            return temp;
        }
        // 세 번째 경우 - 노드 변경x, 데이터 바꿔치기o
        TreeNode* temp_r = min_value_node(node->right);
        TreeNode* temp_l = max_value_node(node->left);
        if (abs(temp_r->key - node->key) < abs(temp_l->key - node->key))
        {
            // 중위 순회시 후계 노드를 복사한다.
            node->key = temp_r->key;
            // 중위 순회시 후계 노드를 삭제한다.
            node->right = delete_node(node->right, temp_r->key);
        }
        else
        {
            // 중위 순회시 후계 노드를 복사한다.
            node->key = temp_l->key;
            // 중위 순회시 후계 노드를 삭제한다.
            node->left = delete_node(node->left, temp_l->key);
        }
    }
    return node;
}

/*================== 탐색 =======================================*/

// 탐색 함수(순환)
TreeNode* search(TreeNode* node, element key)
{
    if (node == NULL) return NULL;
    
    if (key == node->key)
        return node;
    else if (key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

// 탐색 함수(반복)
TreeNode* search_2(TreeNode* node, element key)
{
    if (node == NULL) return NULL;
    
    if (key == node->key)
        return node;
    else if (key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

/*================== 순회 출력 =======================================*/

// 중위 순회 출력(순환)
void print_in_order(TreeNode* node){
    if (node != NULL) {
        print_in_order(node->left);
        printf("[%d] ", node->key);
        print_in_order(node->right);
    }
}
// 전위 순회 출력(순환)
void print_pre_order(TreeNode* node){
    if (node != NULL) {
        printf("[%d] ", node->key);
        print_pre_order(node->left);
        print_pre_order(node->right);
    }
}
// 후위 순회 출력(순환)
void print_post_order(TreeNode* node){
    if (node != NULL) {
        print_post_order(node->left);
        print_post_order(node->right);
        printf("[%d] ", node->key);
    }
}

/*================== 그 외 함수들 =======================================*/

// 최대값 구하기
element get_max_element(TreeNode* node)
{
    while (node->right != NULL) node = node->right;
    return node->key;
}
// 최솟값 구하기
element get_min_element(TreeNode* node)
{
    while (node->left != NULL) node = node->left;
    return node->key;
}

// 노드 개수 구하기
int get_node_count(TreeNode* node)
{
    if (node == NULL) return 0;
    return get_node_count(node->left) + get_node_count(node->right) + 1;
}

// 트리 높이 구하기
int get_tree_height(TreeNode* node)
{
    int left, right;
    if (node == NULL) return 0;
    left = get_tree_height(node->left);
    right = get_tree_height(node->right);
    return (left > right) ? (left + 1) : (right + 1);
}

// 단말 노드 개수 구하기
int get_leaf_node_count(TreeNode* node)
{
    if (node == NULL) return 0;
    if (node->left == NULL && node->right == NULL) return 1;
    return get_leaf_node_count(node->left) + get_leaf_node_count(node->right);
}

/*================== 사용법 =======================================*/

int main(void)
{
    TreeNode* root = NULL;
    element e1 = 3, e2 = 2, e3 = 1;
    element e4 = 4, e5 = 5, e6 = 6;
    
    // 노드 삽입
    root = insert_node_repeat(root, e1);
    root = insert_node_repeat(root, e2);
    root = insert_node_repeat(root, e3);
    root = insert_node_recursion(root, e4);
    root = insert_node_recursion(root, e5);
    root = insert_node_recursion(root, e6);
    
    // 이진트리 출력 (중위 순회)
    print_in_order(root);
    printf("\n");
    
    // 이진트리 삭제
    root = delete_node(root, 5);
    root = delete_node(root, 4);
    root = delete_node(root, 3);
    
    // 이진트리 출력 (중위 순회)
    print_in_order(root);
    printf("\n");
    
    // 이진트리 전체 삭제 (전체 동적할당 해제)
    root = delete_all_node(root);
    
    // 이진트리 출력 (중위 순회)
    print_in_order(root);
    printf("\n");
    
    return 0;
}
