//
//  8-3.c
//  algorithm
//
//  Created by 김건아 on 2022/09/25.
//
// 프로그램 8.3 - 링크법으로 생성된 이진트리
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct TreeNode{
    int data;
    struct TreeNode *left, *right;
} TreeNode;

//      15
//   4     20
// 1     16  25
TreeNode n1 = {1, NULL, NULL};
TreeNode n2 = {4, &n1, NULL};
TreeNode n3 = {16, NULL, NULL};
TreeNode n4 = {25, NULL, NULL};
TreeNode n5 = {20, &n3, &n4};
TreeNode n6 = {15, &n2, &n5};
TreeNode *root = &n6;

// 중위 순회
void in_order(TreeNode* node){
    if (node != NULL) {
        in_order(node->left);           // 왼쪽서브트리 순회
        printf("[%d] ", node->data);    // 노드 방문
        in_order(node->right);          // 오른쪽서브트리 순회
    }
}
// 전위 순회
void pre_order(TreeNode* node){
    if (node != NULL) {
        printf("[%d] ", node->data);    // 노드 방문
        pre_order(node->left);          // 왼쪽서브트리 순회
        pre_order(node->right);         // 오른쪽서브트리 순회
    }
}
// 후위 순회
void post_order(TreeNode* node){
    if (node != NULL) {
        post_order(node->left);         // 왼쪽서브트리 순회
        post_order(node->right);        // 오른쪽서브트리 순회
        printf("[%d] ", node->data);    // 노드 방문
    }
}

int main(void)
{
    printf("중위 순회=");
    in_order(root);
    printf("\n");
    
    printf("전위 순회=");
    pre_order(root);
    printf("\n");
    
    printf("후위 순회=");
    post_order(root);
    printf("\n");
    
    return 0;
}
