
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
        printf("%s, %d:table mem initial failed\n", __FILE__, __LINE__);
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
 *
 * 
 *    
*/
static inline int rehash_step(struct hash_t *h)
{
    int rehash_num = 8;
    int rehash_step = 3;
    unsigned int idx = 0;
    hash_entity *p_entity = NULL;
    struct hlist_head *he = NULL;
    struct hlist_node *node = NULL;

    if (!is_rehashing(h)) return 0; // no rehash happen & return;

    while (--rehash_step && (0 != h->ht[0].used))
    {

        while(h->ht[0].table[h->rehashindex].first == NULL)
        {
            h->rehashindex++;
            if (--rehash_num == 0) return 1;
        }
        node = h->ht[0].table[h->rehashindex].first;
        while(node)
        {
            p_entity = (hash_entity *)node;
            idx = DJBHash(p_entity->key) % h->ht[1].size;
            hlist_add_head(node, (h->ht[1].table + idx));
            h->ht[0].used--;
            h->ht[1].used++;
        }
        h->ht[0].table[h->rehashindex].first = NULL;
        h->rehashindex++;
    }

    if (0 == h->ht[0].used)
    {
        free(h->ht[0].table);
        h->ht[0] = h->ht[1];

    }
    return 0;
}

static inline int need_expand(struct hash_t *h)
{
    if (is_rehashing(h))
    {
        printf("%s, %d :hash table rehashing\n", __FILE__, __LINE__);
        return -1;
    }
    if (h->ht[0].size >= HASH_TB_MAX_NUM)
    {
        printf("%s, %d:max number reached\n", __FILE__, __LINE__);
        return -1;
    }
    
    if ((h->ht[0].size * 0.9) <= h->ht[0].used) 
    {
        return 1;
    }
    return 0;
}

static inline int need_resize(struct hash_t *h)
{
    if (is_rehashing(h))
    {
        printf("%s, %d:hash table rehashing \n", __FILE__, __LINE__);
        return -1;
    }
    if (h->ht[0].size <= HASH_TB_MIN_NUM)
    {
        printf("%s, %d: min num reached\n", __FILE__, __LINE__);
        return -1;
    }
    if (h->ht[0].size * 0.1 < h->ht[0].used)
    {
        return 1;
    }
    return 0;
}

static inline int hash_expand(struct hash_t *h, unsigned int new_size)
{
    hash_tb      t;
    t.size = new_size;
    t.table = (struct hlist_head *)malloc(new_size * HLIST_SIZE);
    if (NULL == t.table)
    {
        printf("mem alloc failed\n");
        return 0;
    }

    t.used = 0;
    h->ht[1] = t;
    h->rehashindex = 0;
    return 1;
}
/*
    get the idx of hash table by mode tabel size
*/
static inline int get_hash_idx(char * key, unsigned int hash, struct hash_t *h)
{
    unsigned int idx, table_index;
    struct hlist_node *he = NULL;
    hash_entity *p_entity = NULL;

    for (table_index = 0; table_index <2; ++table_index)
    {
        idx = hash % (h->ht[table_index].size);
        he = h->ht[table_index].table[idx].first;
        while (he)
        {
            p_entity = (hash_entity *)he;
            if (0 == strcmp(key, p_entity->key))
            {
                printf("key:%s found in table %d\n", key, table_index);
                return -1;
            }
            he = he->next;
        }
        if (!is_rehashing(h))
        {
            break; //just on table 0, and -1 to return. also no index ;
        }
    }
    return idx;
}


/*
   judge if a key exist, and return hash_table index, else -1
*/
static inline int hash_is_exist(char *key, struct hash_t *hash)
{
    return 0;
}

int hash_add(char *key, char *val)
{
    int idx = 0;
    unsigned int hash = 0;
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
        
    if (is_rehashing(&s_hash)) rehash_step(&s_hash);

    if  ((1 == need_expand(&s_hash)))
    {
        unsigned int new = (s_hash.ht[0].size) << 1;
        hash_expand(&s_hash, new);
    } 

    hash = DJBHash(key);

    if ( 0 > (idx = get_hash_idx(key,hash, &s_hash)))
    {
        printf( "idx get failed\ns");
        return -1;
    }
    // if rehashing ,new entity will put to new table [1]. else table[0] will be choosed.
    tb = is_rehashing(&s_hash) ? (s_hash.ht + 1) : (s_hash.ht); 
    p_entity = malloc(sizeof(hash_entity));
    k = malloc(l_k * sizeof(char));
    v = malloc(l_v * sizeof(char));

    strcpy(k, key);
    strcpy(v, val);
    
    INIT_HASH_ENTITY(p_entity, k, v);
    he = (tb->table + idx);
    hlist_add_head(&(p_entity->node), he);
    tb->used++;
    return idx;
}


int hash_del(char *key, char *val)
{

    int idx = 0;
    unsigned int hash = 0;
    int table_index = 0;
    hash_entity *p_entity = NULL;
    struct hlist_node *node = NULL;
    
    if (is_rehashing(&s_hash)) rehash_step(&s_hash);

    if (need_resize(&s_hash)) 
    {
        unsigned int new = (s_hash.ht[0].size) >> 1;
        hash_expand(&s_hash, new);
    }
    
    hash = DJBHash(key);

    for (table_index = 0; table_index < 2; ++table_index)
    {
        idx = hash % s_hash.ht[0].size;
        if ()



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