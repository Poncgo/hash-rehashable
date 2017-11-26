#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED
#include <limits.h>

#include "list.h"


#define HASH_TB_MIN_NUM 8

#define HASH_TB_MAX_NUM 0x80000000

#define HLIST_SIZE sizeof(struct hlist_head)

//load factor  percent ,if larger than 90 expand table size to 2 * ( size)
#define EXPAND_FACTOR 90
//else shrink to 1/2 * size
#define SHRINK_FACTOR 10

#define KEY_LEN 20

#define VAL_LEN 40

/*
    @struct hlist_head *table :hash table.
    @unsigned int size : current size of table;
    @unsigned int used : number of entities in the table
*/
typedef struct hash_table_t
{
    struct hlist_head *table; // table to store
    unsigned int size;        // size of hash table
    unsigned int used;        // number of entity in table
}hash_tb;

/*
    @hash_tb ht[2]: two hash_tb defined, [0] used for store entity as normal conditions,
                    [1] used for store entity while rehashing such as expand or decrease.
    @rehashindex :  normally, it equal to -1 , but when rehashing happened, it may the value of (0 ~ ht[0].size)
                    to indicate current rehashing table index, index small than it has rehased into ht[0],
                    use as ht[0].table[rehashingindex],
*/
struct hash_t
{

    hash_tb ht[2];
    int rehashindex;
}Hash;

/*
    @struct hlist_node: node to index the entity in table,
    @char *key : key of entity
    @char *val : value of entity
*/
typedef struct hash_entity_t
{
    struct hlist_node node; //use hlist_node to link entity while conflict, also by convenience of api of hlist.
    char * key;
    char * val;
}hash_entity;

/*
    @Function Name : hash_add
    @param  inout :key ,key of val
    @param  output:val  value to store
    @return    : -1, when add vall failed if key exist, or integer >= 0
    @Description : add  a string <key , val> into a hash table
*/
int hash_add(char *key, char *val);


/*
    @function name : hash_del
    @param input :key, key of value to be delete
    @param output :val , :value to be delete
    @return : -1 if key not find, or integer >= 0
    @
    @description: delete a string from hash table
*/
int hash_del(char *key, char *val);

/*
    @function name : hash_find
    @param   input: key of value to be find.
    @param  output: value to find
    @return        : -1 i
*/
int hash_find(char *key, char *val);


void HASH_INIT(void);

void HASH_DEL(void);


#endif // HASH_TABLE_H_INCLUDED
