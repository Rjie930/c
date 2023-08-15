/*
 * @Author: vincent
 * @Date: 2023-08-15 10:20:37
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-15 10:45:50
 * @Description: 
 */

#include "dlinklist.h"

// 栈的管理结构体
typedef struct stack
{
    dlinklist top;
}stack, *linkstack;

// 1. 初始化一个空栈
static linkstack init_stack(void)
{
    linkstack s = calloc(1, sizeof(stack));
    if(s != NULL)
    {
        s->top = dlinklist_init();
    }
    return s;
}

bool stack_is_empty(linkstack s)
{
    return dlinklist_empty(s->top);
}

// 2. 入栈
static bool push(linkstack s, datatype data)
{
    dlinklist new = dlinklist_newnode(data);
    if(new == NULL)
        return false;

    dlinklist_add(s->top, new);
    return true;
}

// 3. 出栈
static bool pop(linkstack s, datatype *pdata)
{
    if(dlinklist_empty(s->top))
        return false;

    // 将栈顶元素剔除
    dlinklist p = dlinklist_del(s->top->next);

    // 将栈顶元素给到用户
    *pdata = p->data;

    // 释放相关资源
    free(p);
}