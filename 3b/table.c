#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "table.h"

void print_element(const Item *item) {
	printf("{key: %s info: %s}\n", item->key, item->info);
}

int print(const Table *t) {
	if(t->csize == 0)
		return 2;
	for(int i = 0; i < t->csize; i++)
		print_element(t->first + i);
	return 0;
}


int bin_search(const Table *t, const char *key) {
    int low = 0, high = t->csize - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(key, t->first[mid].key);
        if (cmp < 0) {
            high = mid - 1;
        } else if (cmp > 0) {
            low = mid + 1;
        } else {
            return mid;
        }
    }
    return -1;
}

int cmp(const void *s1, const void *s2) {
    return strcmp(*(char **)s1, *(char **)s2);
}


int find(Table *t, const char *key) {
    if (t->csize == 0)
        return 2;
    int i = bin_search(t, key);
    if(i == -1)
	    return 3;
    print_element(t->first + i);
    return 0;
}

int table_insert(Table *t, const char *key, const char *info) {
	if(t->msize == t->csize)
		return 1;
	if(bin_search(t, key) != -1)
		return 4;
	int i = t->csize - 1;
        for( ; i > 0 && strcmp(t->first[i].key, key) > 0; --i)
		t->first[i + 1] = t->first[i];
	t->first[i + 1].key = key;
	t->first[i + 1].info = info;
	++t->csize;
        return 0;
}

int table_remove(Table *t, const char *key) {
	if(t->csize == 0)
		return 2;
	int i = bin_search(t, key);
	free(t->first[i].key); free(t->first[i].info);
	if(i == -1)
		return 3;
	for( ; i < t->csize; ++i)
		t->first[i] = t->first[i + 1];
	--t->csize; 
	return 0;
}

