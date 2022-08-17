#include "mem_ctl.h"
#include "main.h"

typedef struct Memory_Pool_Node
{
    int size;
    char* data;
    struct Memory_Pool_Node *next;
}memory_pool_node;

typedef struct{
    int total_count;
    memory_pool_node* next;
}memory_pool_head;

int main(void)
{
    memory_pool_node a;
    memory_pool_head b;
}

