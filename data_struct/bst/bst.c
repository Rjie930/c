#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

// 设计树节点
typedef struct node
{
    int data;

    struct node *lchild;
    struct node *rchild;
} node, *linktree;

#define TREENODE struct node
#include "drawtree.h"

linktree new_node(int n)
{
    linktree new = calloc(1, sizeof(node));
    if (new != NULL)
    {
        new->data = n;
        new->lchild = NULL;
        new->rchild = NULL;
    }
    return new;
}

linktree bst_add_node(linktree root, linktree new)
{
    if (root == NULL)
        return new;

    if (new->data < root->data)
        root->lchild = bst_add_node(root->lchild, new);
    if (new->data >= root->data)
        root->rchild = bst_add_node(root->rchild, new);

    return root;
}

// 在以root为根的二叉树中删除结点n
// 且返回新的树的根
linktree bst_remove(linktree root, int n)
{
    if (n < root->data)
        root->lchild = bst_remove(root->lchild, n);
    else if (n > root->data)
        root->rchild = bst_remove(root->rchild, n);

    else
    {
        if (root->lchild != NULL)
        {
            linktree max;
            for (max = root->lchild; max->rchild != NULL; max = max->rchild)
                ;
            root->data = max->data;
            root->lchild = bst_remove(root->lchild, max->data);
        }

        else if (root->rchild != NULL)
        {
            linktree min;
            for (min = root->rchild; min->lchild != NULL; min = min->lchild)
                ;
            root->data = min->data;
            root->rchild = bst_remove(root->rchild, min->data);
        }

        else
        {
            free(root);
            return NULL;
        }
    }
    return root;
}

void pre_travel2(linktree root)
{
    if (root == NULL)
        return;

    // 根 - 左 - 右
    printf("%d ", root->data);
    pre_travel2(root->lchild);
    pre_travel2(root->rchild);
}

void mid_travel2(linktree root)
{
    if (root == NULL)
        return;

    //  左 - 根 - 右
    mid_travel2(root->lchild);
    printf("%d ", root->data);
    mid_travel2(root->rchild);
}

void pos_travel2(linktree root)
{
    if (root == NULL)
        return;

    //  左 - 右 - 根
    pos_travel2(root->lchild);
    pos_travel2(root->rchild);
    printf("%d ", root->data);
}

int main(int argc, char const *argv[])
{
    // 1. 搞一个空树
    linktree root = NULL;

    // 2. 按照小-中-大的逻辑逐个添加节点
    int n;
    while (1)
    {
        scanf("%d", &n);

        if (n == 0)
            break;

        if (n > 0)
        {
            linktree new = new_node(n);
            root = bst_add_node(root, new); // 在树root中添加新节点new
        }
        else if (n < 0)
        {
            root = bst_remove(root, -n);
        }

        // 以网页形式直观地观察树是否正确
        draw(root);
    }

    // 使用各种遍历算法打印节点
    pre_travel2(root);
    printf("\n"); // 前
    mid_travel2(root);
    printf("\n"); // 中
    pos_travel2(root);
    printf("\n"); // 后

    return 0;
}
