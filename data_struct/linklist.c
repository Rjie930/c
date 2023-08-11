/*
 * @Author: vincent
 * @Date: 2023-08-11 09:31:12
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-11 09:54:02
 * @Description: 
 */

#include "linklist.h"

// 1. 初始化空单循环链表（带头结点）
node *init_linklist()
{
    node *new = calloc(1, sizeof(node));
    if(new != NULL)
    {
        new->next = new;
    }
    return new;
}

node * new_node(int data)
{
    node *new = calloc(1, sizeof(node));
    if(new != NULL)
    {
        new->data = data;
        new->next = new; // 遵循循环链表的规则
    }
    return new;
}


// 2. 在头部插入新节点
bool list_add(node *head, int data)
{
    // a. 创建一个新节点
    node *new = new_node(data);
    if(new == NULL)
        return false;

    // b/c. 将节点插入链表的最开头的地方
    new->next = head->next;
    head->next = new;
    return true;
}


// 3. 在尾部插入新节点
bool list_add_tail(node *head, int data)
{
    
}


// 4. 在指定节点（p指向的节点）的前面插入新节点
void list_add_pos(node *head, node *p)
{

}


// 5. 剔除指定节点p
node * list_del(node *p)
{

}


// 6. 遍历链表

bool is_empty(node *head)
{
    return head == head->next;
}

void show(node *head)
{
    if(is_empty(head))
        return;

    node *p;
    for(p=head->next; p!=head; p=p->next)
    {
        printf("%d\t", p->data);
    }
    printf("\n");
}


// 7. 销毁链表
void detroy_linklist(node *head)
{
    
}