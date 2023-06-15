#ifndef TABLE_H
#define TABLE_H

typedef struct Item {
	char *key;
	char *info; 
} Item;

typedef struct Table {
        Item *first;
        int csize;
	int msize;
} Table;

int print(const Table *t);
int find(Table *t, const char *key);
int table_insert(Table *t, const char *key, const char *info);
int table_remove(Table *t, const char *key);
#endif /*TABLE_H*/

