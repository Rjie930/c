/*
 * @Author: vincent
 * @Date: 2023-08-11 09:31:07
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-11 09:47:30
 * @Description: 
 */
#include "linklist.h"

int main(int argc, char const *argv[])
{
    // 1. 搞一个空链表
    node *head = init_linklist();

    // 2. 键盘输入任意节点
    int n;
    while(1)
    {
        scanf("%d", &n);

        if(n > 0)
            list_add(head, n);

        // else if(n < 0)
        //     list_del(head, -n);

        else
            break;

        show(head);
    }

    // 3. 销毁链表

    return 0;
}
