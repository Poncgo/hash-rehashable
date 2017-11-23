#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED
#include <limits.h>

#include "list.h"
#define HASH_TB_MIN_NUM 8

#define HASH_TB_MAX_NUM INT_MAX

#define HLIST_SIZE sizeof(struct hlist_head)

//load factor  percent ,if larger than 90 expand table size to 2 * ( size)
#define EXPAND_FACTOR 90
//else shrink to 1/2 * size
#define SHRINK_FACTOR 10

#define KEY_LEN 20

#define VAL_LEN 40

typedef struct hash_table_t
{
    struct hlist_head *table;
    unsigned int size;
    unsigned int used;
}hash_tb;

struct hash_t
{
    hash_tb ht[2];
    unsigned int rehashindex;
}Hash;

typedef struct hash_entity_t
{
    struct hlist_node node; //use hlist_node to link entity while conflict, also by convenience of api of hlist.
    char * key;
    char * val;
}hash_entity;

int hash_add(char *key, char *val);

int hash_del(char *key, char *val);

int hash_find(char *key, char *val);

int rehash_table(void);



#endif // HASH_TABLE_H_INCLUDED
