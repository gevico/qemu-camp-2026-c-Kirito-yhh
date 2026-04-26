#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    return (unsigned)key;
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    unsigned hash = hash_int(key) % c->bucket_count;
    HashEntry* entry = c->buckets[hash];
    HashEntry** prev_next = &c->buckets[hash];
    
    while (entry) {
        if (entry->key == key) {
            if (pprev_next) {
                *pprev_next = prev_next;
            }
            return entry;
        }
        prev_next = &entry->next;
        entry = entry->next;
    }
    
    if (pprev_next) {
        *pprev_next = prev_next;
    }
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    if (!c->head) {
        // 链表为空
        c->head = c->tail = node;
        node->prev = node->next = NULL;
    } else {
        // 链表不为空，添加到头部
        node->next = c->head;
        node->prev = NULL;
        c->head->prev = node;
        c->head = node;
    }
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (!node) return;
    
    // 处理前一个节点
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        // 节点是头节点
        c->head = node->next;
    }
    
    // 处理后一个节点
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        // 节点是尾节点
        c->tail = node->prev;
    }
    
    // 清空节点的指针
    node->prev = node->next = NULL;
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    if (!node || node == c->head) return;
    
    // 先移除节点
    list_remove(c, node);
    // 再添加到头部
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    if (!c->tail) return NULL;
    
    LRUNode* tail = c->tail;
    list_remove(c, tail);
    return tail;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    LRUCache* c = (LRUCache*)malloc(sizeof(LRUCache));
    if (!c) return NULL;
    
    c->capacity = capacity;
    c->size = 0;
    c->head = c->tail = NULL;
    
    // 简单设置桶的数量为容量的2倍
    c->bucket_count = capacity * 2;
    c->buckets = (HashEntry**)calloc(c->bucket_count, sizeof(HashEntry*));
    if (!c->buckets) {
        free(c);
        return NULL;
    }
    
    return c;
}

static void lru_free(LRUCache* c) {
    if (!c) return;
    
    // 释放链表中的所有节点
    LRUNode* node = c->head;
    while (node) {
        LRUNode* next = node->next;
        free(node);
        node = next;
    }
    
    // 释放哈希表中的所有条目
    for (size_t i = 0; i < c->bucket_count; ++i) {
        HashEntry* entry = c->buckets[i];
        while (entry) {
            HashEntry* next = entry->next;
            free(entry);
            entry = next;
        }
    }
    
    // 释放哈希表
    free(c->buckets);
    // 释放缓存本身
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    if (!c || !out_value) return 0;
    
    // 在哈希表中查找
    HashEntry* entry = hash_find(c, key, NULL);
    if (!entry) return 0;
    
    // 找到，将节点移动到链表头部
    list_move_to_head(c, entry->node);
    // 输出值
    *out_value = entry->node->value;
    
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    if (!c) return;
    
    // 查找键是否已存在
    HashEntry** pprev_next;
    HashEntry* entry = hash_find(c, key, &pprev_next);
    
    if (entry) {
        // 键已存在，更新值并移动到链表头部
        entry->node->value = value;
        list_move_to_head(c, entry->node);
    } else {
        // 键不存在，创建新节点
        LRUNode* node = (LRUNode*)malloc(sizeof(LRUNode));
        if (!node) return;
        
        node->key = key;
        node->value = value;
        node->prev = node->next = NULL;
        
        // 添加到链表头部
        list_add_to_head(c, node);
        
        // 添加到哈希表
        HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
        if (!new_entry) {
            list_remove(c, node);
            free(node);
            return;
        }
        
        new_entry->key = key;
        new_entry->node = node;
        new_entry->next = *pprev_next;
        *pprev_next = new_entry;
        
        // 增加大小
        c->size++;
        
        // 如果超过容量，淘汰最久未使用的节点
        if (c->size > c->capacity) {
            LRUNode* tail = list_pop_tail(c);
            if (tail) {
                // 从哈希表中删除
                HashEntry** pprev_next_tail;
                HashEntry* entry_tail = hash_find(c, tail->key, &pprev_next_tail);
                if (entry_tail) {
                    *pprev_next_tail = entry_tail->next;
                    free(entry_tail);
                }
                free(tail);
                c->size--;
            }
        }
    }
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
