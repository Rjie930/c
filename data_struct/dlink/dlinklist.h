/*
 * @Author: vincent
 * @Date: 2023-08-11 00:57:15
 * @LastEditTime: 2023-08-11 01:12:16
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <strings.h>

typedef struct node
{
    int data;

    struct node *prev;
    struct node *next;
}node, *dlinklist;


// 1. 初始化空的双循环链表（带头节点）
node *init_dlinklist();
bool list_add_tail(dlinklist head, node *new);
bool is_empty(dlinklist head);
void show(dlinklist head);
node * new_node(int data);