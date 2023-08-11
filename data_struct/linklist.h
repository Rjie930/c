/*
 * @Author: vincent
 * @Date: 2023-08-11 09:31:17
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-11 09:50:10
 * @Description: 
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// 带头结点的单向循环链表

// 节点结构体
typedef struct node
{
    int data;
    struct node *next;
}node;



// 1. 初始化空单循环链表（带头结点）
node *init_linklist();
node * new_node(int data);
bool list_add(node *head, int data);
bool list_add_tail(node *head, int data);
bool is_empty(node *head);
void show(node *head);
