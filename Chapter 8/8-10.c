//
//  8-10.c
//  algorithm
//
//  Created by 김건아 on 2022/09/25.
//
// 프로그램 8.10 - 반복적인 탐색 함수
//

#include <stdio.h>

typedef struct TreeNode{
    int key;    // 지금 까지는 "data" 였지만, 이제는 레코드 개념으로 "key"사용
    struct TreeNode *left, *right;
    int is_thread;
} TreeNode;

// 반복적인 탐색 함수
TreeNode* search(TreeNode* node, int key)
{
    while (node != NULL)
    {
        if (key == node->key)
            return node;
        else if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }
    return NULL;    // 탐색에 실패했을 경우 NULL 반환
}
