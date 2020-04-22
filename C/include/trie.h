#include <stddef.h>

#include "../include/dhcp.h"

typedef struct trie trie_t;
typedef struct node node_t;

trie_t *TrieCreate(size_t num_of_bits);
void TrieDestroy(trie_t *trie);
size_t TrieCount(const trie_t *trie);
int TrieRemove(trie_t *trie, uint32_t key);
enum status TrieInsert(trie_t *trie, uint32_t data, uint32_t *ins_data);