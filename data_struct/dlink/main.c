#include "dlinklist.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    // 1. 搞一个空链表
    dlinklist head = init_dlinklist();

    // 2. 用来存储一些随机数
    for(int i=0; i<5; i++)
    {
        // 2.1 创建新节点
        node *new = new_node(rand()%100);

        // 2.2 将新节点添加到链表中
        list_add_tail(head, new);
    }

    // 3. 输出这些随机数
    show(head);


    // 4. 从键盘输入整数，让其从链表删除对应的结点


    // 5. 销毁整个链表

    return 0;
}
