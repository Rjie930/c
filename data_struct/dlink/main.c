/*
 * @Author: vincent
 * @Date: 2023-08-14 02:04:59
 * @LastEditTime: 2023-08-14 02:41:35
 */

#include "dlinklist.h"

void show(int *pdata)
{
    printf("%d ", *pdata);
}

int main(int argc, char const *argv[])
{
    // 1. 搞一个空链表
    dlinklist head = dlinklist_init();

    // 2. 存储系列自然数
    int n;
    scanf("%d", &n);
    for(int i=8; i>=1; i--)
    {
        // 2.1 创建新节点
        dlistnode *new = dlinklist_newnode(i);

        // 2.2 将新节点添加到链表中
        dlinklist_add_tail(head, new);
    }

    // 3. 输出这些数
    dlinklist_for_each(head, show);

    // 4. 重整数据，奇偶重排
    dlistnode *p, *q;
    for(p=q=head->prev; p!=head; p=p->prev)
    {
        // 4.1 偶数就移动到末尾
        if(p->data % 2 == 0)
        {
            dlinklist_move_tail(head, p);
            p = q;
        }

        // 4.2 奇数记下位置
        else
            q = p;
    }

    dlinklist_for_each(head, show);

    // 5. 销毁整个链表
    dlinklist_destroy(head, NULL);

    return 0;
}
