//
//  8-2.c
//  algorithm
//
//  Created by 김건아 on 2022/09/25.
//
// 프로그램 8.2 - 이진트리의 세가지 순회 방법 (재귀함수)
//

#include <stdio.h>

typedef struct TreeNode{
    int data;
    struct TreeNode *left, *right;
} TreeNode;

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
