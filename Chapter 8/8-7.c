//
//  8-7.c
//  algorithm
//
//  Created by 김건아 on 2022/09/25.
//
// 프로그램 8.7 - 디렉토리 용량 계산 프로그램
//

#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode{
    int data;
    struct TreeNode *left, *right;
} TreeNode;

// 디렉토리 크기 구하기
int calc_dir_size(TreeNode* node)
{
    int left_size, right_size;
    if(node == NULL) return 0;
    
    left_size = calc_dir_size(node->left);
    right_size = calc_dir_size(node->right);
    return (node->data + left_size + right_size);
}

int main(void)
{
    TreeNode n4 = {500, NULL, NULL};
    TreeNode n5 = {200, NULL, NULL};
    TreeNode n3 = {100, &n4, &n5};
    TreeNode n2 = {50, NULL, NULL};
    TreeNode n1 = {0, &n2, &n3};
    
    printf("디렉토리의 크기=%d\n", calc_dir_size(&n1));
}
