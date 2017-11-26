#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"
#include <time.h>

#define NUM 1 << 20


int main (int argc, char *args[])
{
    int i = 0;
    char k[10] = {'0'};
    char v[10] = {'0'};
    HASH_INIT();
    time_t s1, s2, s3, s4;

    time(&s1);
    for (i = 0; i < NUM; ++i)
    {
        sprintf(k, "%d\0", i);
        hash_add(k, k);
    }
    time(&s2);

	for (i = 0; i < NUM; ++i)
	{
		sprintf(k, "%d\0", i);
		if (1 == hash_find(k, v))
		{
			printf("find Key %s val %s\n", k, v);
		}
	}
	time(&s3);

	for (i = 0; i < NUM; i ++)
	{
		sprintf(k, "%d\0", i);
		sprintf(k, "%d\0", i);
		if (1 == hash_del(k, v))
		{
			printf("del key %s val%s\n", k, v);
		}
	}

    time(&s4);

    printf("add interval = %d\n", s2 - s1);
	printf("find interval = %d\n", s3 - s2);
    printf("del interval = %d\n", s4 - s3);
	HASH_DEL();
}
