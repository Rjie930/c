/*
 * @Author: vincent
 * @Date: 2023-08-15 11:20:14
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-15 11:20:16
 * @Description: 
 */
/*
 * @Author: vincent
 * @Date: 2023-08-15 09:54:50
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-15 09:54:51
 * @Description: 
 */
/*
 * @Author: vincent
 * @Date: 2023-08-11 00:57:15
 * @LastEditTime: 2023-08-14 02:40:27
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <strings.h>

#ifndef DATATYPE
#define DATATYPE int 
#endif

typedef DATATYPE datatype;

typedef struct dlistnode
{
    datatype data;

    struct dlistnode *prev;
    struct dlistnode *next;
}dlistnode, *dlinklist;


// 1. 初始化空的双循环链表（带头节点）
dlistnode *dlinklist_init()
{
    dlistnode * head = calloc(1, sizeof(dlistnode));
    if(head != NULL)
    {
        head->next = head;
        head->prev = head;
    }
    return head;
}

dlistnode * dlinklist_newnode(datatype data)
{
    dlistnode *new = calloc(1, sizeof(dlistnode));
    if(new != NULL)
    {
        new->data = data;
        new->prev = new;
        new->next = new;
    }
    return new;
}

// 2. 尾插法添加新节点
bool dlinklist_add_tail(dlinklist head, dlistnode *new)
{
    new->prev = head->prev;
    new->next = head;
    head->prev->next = new;
    head->prev = new;
    return true;
}

bool dlinklist_empty(dlinklist head)
{
    return head->prev == head;
}

// 3. 向后遍历链表的所有结点
void dlinklist_for_each(dlinklist head, void (*handle)(datatype *))
{
    if(dlinklist_empty(head))
        return;
    
    dlistnode *p;
    for(p=head->next; p!=head; p=p->next)
    {
        // printf("%d\t", p->data);
        handle(&p->data);
    }
    printf("\n");
}

dlistnode * dlinklist_find(dlinklist head, datatype data, bool (*compare)(datatype*, datatype*))
{
    if(dlinklist_empty(head))
        return NULL;

    dlistnode *pos;
    for(pos=head->next; pos!=head; pos=pos->next)
    {
        // if(pos->data == data)
        if(compare(&pos->data, &data))
            return pos;
    }
    return NULL;
}

// 将结点p从链表中剔除出去
dlistnode * dlinklist_del(dlistnode *p)
{
    p->prev->next = p->next;
    p->next->prev = p->prev;

    p->prev = NULL;
    p->next = NULL;

    return p;
}

void dlinklist_move_tail(dlinklist head, dlinklist p)
{
    dlinklist_del(p);
    dlinklist_add_tail(head, p);
}

// 释放所有的有效结点，使得head回复初始状态
void dlinklist_destroy_init(dlinklist head, void (*user_free)(datatype*) )
{
    if(dlinklist_empty(head))
        return;

    dlistnode *p;
    for(p=head->next; p!=head; p=head->next)
    {
        p = dlinklist_del(p);

        if(user_free != NULL)
            user_free(&p->data);

        free(p);
    }
}

// 销毁所有的结点（包括头节点）
dlistnode * dlinklist_destroy(dlinklist head, void (*user_free)(datatype*) )
{
    dlinklist_destroy_init(head, user_free);
    free(head);
    return NULL;
}