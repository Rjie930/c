#ifndef __LINKLIST_H
#define __LINKLIST_H

/*
 * @Author: vincent
 * @Date: 2023-08-11 09:31:17
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-14 11:29:26
 * @Description: 
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// 带头结点的单向循环链表

#ifndef DATATYPE
#define DATATYPE int // 容器的默认类型
#endif

typedef DATATYPE datatype;

// 节点结构体
typedef struct node
{
    datatype data;
    struct node *next;
}node;


// 1. 初始化空单循环链表（带头结点）
static node *init_linklist()
{
    node *new = calloc(1, sizeof(node));
    if(new != NULL)
    {
        new->next = new;
    }
    return new;
}

static node * new_node(datatype data)
{
    node *new = calloc(1, sizeof(node));
    if(new != NULL)
    {
        new->data = data;
        new->next = new; // 遵循循环链表的规则
    }
    return new;
}


// // 2. 在头部插入新节点
// bool list_add(node *head, int data)
// {
//     // a. 创建一个新节点
//     node *new = new_node(data);
//     if(new == NULL)
//         return false;

//     // b/c. 将节点插入链表的最开头的地方
//     new->next = head->next;
//     head->next = new;
//     return true;
// }

// 5. 判定链表是否为空
static bool is_empty(node *head)
{
    return head == head->next;
}


// 3. 在尾部插入新节点
static void list_add_tail(node *head, node *new)
{
    // a. 
    node *tail;
    for(tail=head; tail->next!=head; tail=tail->next);

    // b.
    new->next  = head;
    tail->next = new;
}

// 3. 找到某个节点
static node * list_find(node *head, datatype data, bool (*compare)(const datatype *, const datatype *) )
{
    if(is_empty(head))
        return NULL;
    
    node *p;
    for(p=head->next; p!=head; p=p->next)
    {
        if(compare(&p->data, &data))
            return p;
    }

    return NULL;
}

// 4. 剔除指定节点p，让其跟链表不再有关联
//    提出后，将节点返回给调用者，让其处理
static node * list_del(node *p)
{
    // 1. 找到p的前一个节点指针
    node *prev;
    for(prev=p->next; prev->next!=p; prev=prev->next);

    // 2. 将12节点的地址，放入3节点里面的next
    prev->next = p->next;

    // 3. 让p节点自己指向自己，确保跟链表不再有任何关系
    p->next = p;
    return p;
}


// 6. 从头到尾遍历链表
static void list_for_each(node *head,  void (*handle)(datatype *)  )
{
    if(is_empty(head))
        return;

    node *p;
    for(p=head->next; p!=head; p=p->next)
    {
        handle(&p->data);
    }
}


// 7. 销毁链表（恢复初始状态）
void destroy_list_init(node *head, void (*free_data)(datatype *))
{
    // 释放所有的有效节点
    while (!is_empty(head))
    {
        node *p = list_del(head->next);

        if(free_data != NULL)
            free_data(&p->data);

        free(p);
    }
}

// 8. 销毁链表（彻底删除，含头结点也删除掉）
void destroy_list(node *head, void (*free_data)(datatype *))
{
    // 释放所有的节点（含头结点）
    destroy_list_init(head, free_data);
    free(head);
}

#endif