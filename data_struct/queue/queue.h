/*
 * @Author: vincent
 * @Date: 2023-08-15 11:20:28
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-15 11:55:42
 * @Description: 
 */
#include "dlinklist.h"

// 设计队列管理结构体
typedef struct queue
{
    dlinklist front; // 队头指针
    dlinklist tail;  // 队尾指针

    int number;
}queue, *linkqueue;


// 初始化空队列
static linkqueue init_queue()
{
    linkqueue q = calloc(1, sizeof(queue));
    if(q != NULL)
    {
        q->front = dlinklist_init();
        q->tail = q->front;
        q->number = 0;
    }
    return q;
}


// 入队
static bool en_queue(linkqueue q, datatype data)
{
    dlinklist new = dlinklist_newnode(data);
    if(new == NULL)
        return false;

    // 用尾插法将节点插入到尾部（排队）
    dlinklist_add_tail(q->front, new);
    q->tail = new;
    q->number++;
    return true;
}

// 判定队列是否为空
static bool queue_is_empty(linkqueue q)
{
    return q->number == 0;
}

// 出队
static bool out_queue(linkqueue q, datatype *pdata)
{
    if(queue_is_empty(q))
        return false;

    // 将队头节点剔除
    // 将数据返回给用户指定的变量
    dlistnode *p = dlinklist_del(q->front->next);
    *pdata = p->data;

    // 如果出队的元素是队列中唯一的最后一个节点，那么要更新队尾指针
    if(p == q->tail)
        q->tail = q->front;

    q->number--;

    // 释放相应的资源
    free(p);
    return true;
}

// 取队头数据
static bool front_queue(linkqueue q, datatype *pdata)
{
    if(queue_is_empty(q))
        return false;

    // 将数据返回给用户指定的变量
    *pdata = q->front->next->data;
    return true;
}

// 销毁队列
void linkqueue_destroy(linkqueue q, void (*user_free)(datatype *))
{
    dlinklist_destroy(q->front, user_free);
    free(q);
}