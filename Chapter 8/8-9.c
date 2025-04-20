//
//  8-9.c
//  algorithm
//
//  Created by 김건아 on 2022/09/25.
//
// 프로그램 8.9 - 순환적인 탐색 함수
//

#include <stdio.h>

typedef struct TreeNode{
    int key;    // 지금 까지는 "data" 였지만, 이제는 레코드 개념으로 "key"사용
    struct TreeNode *left, *right;
    int is_thread;
} TreeNode;

// 순환적인 탐색 함수
TreeNode* search(TreeNode* node, int key)
{
    if (node == NULL) return NULL;
    
    if (key == node->key)
        return node;
    else if (key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}
