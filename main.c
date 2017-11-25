#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"

int main (int argc, char *args[])
{
    int i = 0;
    char k[10] = {'0'};
    char v[10] = {'0'};
    HASH_INIT();

    for (i = 0; i < 32; ++i)
    {
        sprintf(k, "%d\n", i);
        hash_add(k, k);
    }
}
