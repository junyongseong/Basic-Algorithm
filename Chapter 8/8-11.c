//
//  8-11.c
//  algorithm
//
//  Created by 김건아 on 2022/09/25.
//
// 프로그램 8.11 - 이진트리 삽입 프로그램
//

#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode{
    int key;    // 지금 까지는 "data" 였지만, 이제는 레코드 개념으로 "key"사용
    struct TreeNode *left, *right;
    int is_thread;
} TreeNode;

TreeNode* new_node(int item)
{
    TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

TreeNode* insert_node(TreeNode* node, int key)
{
    // 트리가 공백이면 새로운 노드를 반환한다.
    if (node == NULL) return new_node(key);
    
    // 그렇지 않으면 순환적으로 트리를 내려간다.
    if (key < node->key)
        node->left = insert_node(node->left, key);
    else if (key > node->key)
        node->right = insert_node(node->right, key);
    
    // 변경된 루트 포인터를 반환한다.
    return node;
}
