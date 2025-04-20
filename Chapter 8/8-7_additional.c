//
//  8-7_additional.c
//  algorithm
//
//  Created by 김건아 on 2022/09/25.
//

#include <stdio.h>

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

typedef struct TreeNode{
    int data;
    struct TreeNode *left, *right;
} TreeNode;

// 노드의 개수 구하기
int get_node_count(TreeNode* node)
{
    int count = 0;
    if (node != NULL)
        count = 1 + get_node_count(node->left) + get_node_count(node->right);
    return count;
}
// 단말 노드 개수 구하기
int get_leaf_count(TreeNode* node)
{
    int count = 0;
    if (node != NULL){
        if (node->left == NULL && node->right == NULL) return 1;
        else count = get_leaf_count(node->left) + get_leaf_count(node->right);
    }
    return count;
}
// 이진 탐색 트리에서 높이 구하기
int get_height(TreeNode* node)
{
    int height = 0;
    if (node != NULL)
        height = 1 + max(get_height(node->left), get_height(node->right));
    return height;
}
