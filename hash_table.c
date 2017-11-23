
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"

#define is_rehashing(h) ((h)->rehashindex != -1)

static struct hash_t s_hash;

static inline unsigned int hash_size(unsigned int size)
{
    unsigned int i = HASH_TB_MIN_NUM;

    if (size > HASH_TB_MAX_NUM)
    {
        size = HASH_TB_MAX_NUM;
    }
    while(1) {
        if (i >= size)
            return i;
        i *= 2;
    }
}

static inline void INIT_HASH_TB(hash_tb *tb, unsigned int size)
{
    unsigned int i = 0;
    tb->table = (struct hlist_head *)malloc(size * HLIST_SIZE);
    if (NULL == tb->table)
    {
        printf("table mem initial failed\n");
        return;
    }
    tb->size = size;
    tb->used = 0;
    for (i = 0; i < size; ++i)
    {
        INIT_HLIST_HEAD(tb->table + i);
    }
}

static inline void INIT_HASH(void)
{
    s_hash.rehashindex = -1;
    INIT_HASH_TB(&s_hash.ht[0], HASH_TB_MIN_NUM);

    INIT_HASH_TB(&s_hash.ht[1], HASH_TB_MIN_NUM * 2);    
}

static inline void INIT_HASH_ENTITY(hash_entity *n, char *k, char *v)
{
    INIT_HLIST_NODE(&(n->node));
    n->key = k;
    n->val = v;
}

/*
    hash code generate
*/
static inline unsigned int hash_code(char* str, unsigned int len)
{
    unsigned int hash = len;

    while (*str != '\n')
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str++);
    }
    return hash;
}

unsigned int DJBHash(char *str)
{
    unsigned int hash = 5381;
 
    while (*str)
    {
        hash = ((hash << 5) + hash) + (*str++); /* times 33 */
    }
    hash &= ~(1 << 31); /* strip the highest bit */
    return hash;
}

/*
    get the idx of hash table by mode tabel size
*/
static inline unsigned int get_hash_idx(char * key, struct hash_t *hash)
{

    return -1;
}


/*
   judge if a key exist, and return hash_table index, else -1
*/
static inline hash_entity *hash_is_exist(char *key, struct hash_t *hash)
{

    if (NULL == key)
    {
        printf( "key is  null\n");
        return NULL;
    }

    unsigned int idx      = get_hash_idx(key, &s_hash);
    struct hlist_node *he = ht.hash_table[idx].first;
    hash_entity *p_entity = NULL;

    while(he)
    {
        p_entity = (hash_entity *) he;
        if (0 == strcmp(p_entity->key, key))
        {
            return p_entity;
        }
        he = he->next;
    }
    return NULL;
}

static inline void _RehashStep()
{

}
int hash_add(char *key, char *val)
{
    int idx = 0;
    struct hlist_head *he = NULL;
    hash_tb *tb = NULL;
    int l_k = strlen(key);
    int l_v = strlen(val);
    char *k = NULL;
    char *v = NULL;
    hash_entity *p_entity = NULL;

    if  ((NULL == val) || (NULL == key))
    {
        printf( "NULL POINTER of key or val\n");
        return -1;
    }
    if (is_rehashing(&s_hash)) 
    {

    }
       
    if ( 0 > (idx = get_hash_idx(key, &s_hash)))
    {
        printf( "idx get failed\ns");
        return -1;
    }
    tb =  is_rehashing(&s_hash) ? (s_hash.ht + 1) : (s_hash.ht); 

    p_entity = malloc(sizeof(hash_entity));
    k = malloc(l_k * sizeof(char));
    v = malloc(l_v * sizeof(char));

    strcpy(k, key);
    strcpy(v, val);
    
    INIT_HASH_ENTITY(p_entity, k, v);
    he = (ht.hash_table + idx);
    hlist_add_head(&(p_entity->node), he);
    ht.used++;
    return idx;
}


int hash_del(char *key, char *val)
{
    int idx = hash_idx(key);
    hash_entity *p_entity = NULL;
    struct hlist_node *node = NULL;
    if (0 > idx)
    {
        printf( "key error\n");
        return -1;
    }

    p_entity = hash_is_exist(key);
    if (NULL == p_entity)
    {
        printf( "no key found while del\n");
        return -1;
    }

    node = &(p_entity->node);
    hlist_del(node);
    ht.used--;

    if (NULL != val)
    {
        strcpy(val, p_entity->val);
    }

    free(p_entity->key);
    free(p_entity->val);
    free(p_entity);

    return 0;
}

int hash_find(char *key, char *val)
{
    int idx = hash_idx(key);
    hash_entity *p_entity = NULL;

    if (0 > idx)
    {
        printf( "key error\n");
        return -1;
    }

    p_entity = hash_is_exist(key);
    if (NULL == p_entity)
    {
        printf( "no key found while del\n");
        return -1;
    }

    if (NULL != val)
    {
        strcpy(val, p_entity->val);
    }
    return 0;
}


/*
    init hash table by min size HASH_TB_MIN_NUM
*/
static inline void HASH_INIT(void)
{
    INIT_HASH();
}


void HASH_DEL(void)
{
    if ((0 == ht.size) && ( NULL == ht.hash_table))
    {
        return;
    }

    ht.size = 0;
    //del element
}