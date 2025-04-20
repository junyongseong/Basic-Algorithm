//
//  8-4.c
//  algorithm
//
//  Created by 김건아 on 2022/09/25.
//
// 프로그램 8.4 - 반복적인 중위 순회
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct TreeNode{
    int data;
    struct TreeNode *left, *right;
} TreeNode;

#define SIZE 100
int top = -1;
TreeNode* stack[SIZE];

void push(TreeNode *p)
{
    if (top < SIZE - 1) stack[++top] = p;
}
TreeNode* pop(void)
{
    TreeNode* p = NULL;
    if (top >= 0) p = stack[top--];
    return p;
}
void in_order_iter(TreeNode* node)
{
    while (1) {
        for (; node; node = node->left) push(node);
        node = pop();
        if (!node) break;
        printf("[%d] ", node->data);
        node = node->right;
    }
}

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

int main(void)
{
    printf("중위 순회=");
    in_order_iter(root);
    printf("\n");
    
    return 0;
}
