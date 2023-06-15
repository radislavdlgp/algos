#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Node {
    int release;
    char* key;
    unsigned info;
    struct Node* next;
} Node;

typedef struct Item {
    Node *node;
} Item;

typedef struct Table {
    Item* data;
    int size;
} Table;

unsigned long long hash(const char* key, int size) {
    unsigned long long hash = INT_MAX;
    for(int i = 0; i < strlen(key); ++i) {
        hash = 37 * hash + key[i];
    }
    return abs((long long)hash) % size;
}

int findLastRelease(const Table* table, const char* key) {
    int j = hash(key, table->size);
    Node *ptr = table->data[j].node;
    if(ptr == NULL)
        return 0;

    while(ptr) {
        if(strcmp(key, ptr->key) == 0)
            return ptr->release;
        ptr = ptr->next;
    }
    return 0;
}

Node* newNode(const Table* table, const char* key, unsigned data) {
    Node* new = (Node *)calloc(1, sizeof(Node));
    new->key = (char *)calloc(strlen(key) + 1, sizeof(char));
    strcpy(new->key, key);
    new->info = data;
    new->release = findLastRelease(table, key) + 1;
    new->next = NULL;
    return new;
}

int insert(Table* table, const char* key, unsigned data) {
    if(!table)
        return 1;
    int j = hash(key, table->size);
    Node *new = newNode(table, key, data);
    new->next = table->data[j].node;
    table->data[j].node = new;
    return 0;
}


int delete(Table* table, const char* key) {
    int j = hash(key, table->size);
    Node *prev = NULL;
    Node *cur = table->data[j].node;
    while(cur) {
        if(strcmp(key, cur->key) == 0) {
            if(prev == NULL) {
                table->data[j].node = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur->key);
            free(cur);
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return 1;
}

int find(Table* table, const char* key, int version) {
    if(!table)
        return 0;
    int j = hash(key, table->size);
    Node *ptr = table->data[j].node;
    while(ptr) {
        if(strcmp(key, ptr->key) == 0 && (ptr->release == version || !version)) {
            if(ptr->next && !version) {
                printf("%s(%d):%u ->", ptr->key, ptr->release, ptr->info);
            } else {
                printf("%s(%d):%u\n", ptr->key, ptr->release, ptr->info);
            }
        }
        ptr = ptr->next;
    }
    return 0;
}

void printNode(Node *ptr) {
    if(!ptr) {
        printf("NULL\n");
    } else {
        for(; ptr; ptr = ptr->next) {
            if(ptr->next) {
                printf("%s(%d):%u ->", ptr->key, ptr->release, ptr->info);
            } else {
                printf("%s(%d):%u\n", ptr->key, ptr->release, ptr->info);
            }
        }
    }

}

void printTable(Table *table) {
    if(!table)
        return;
    for(int i = 0; i < table->size; ++i)
        printNode(table->data[i].node);
}

int deleteAllExceptLast(Table* table, const char* key) {
    int j = hash(key, table->size);
    Node *prev = NULL;
    Node *cur = table->data[j].node;
    int lastRelease = findLastRelease(table, key);
    while(cur) {
        if(strcmp(key, cur->key) == 0 && cur->release != lastRelease) {
            if(prev == NULL) {
                table->data[j].node = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur->key);
            free(cur);
        } else {
            if(strcmp(key, cur->key) == 0 && cur->release == lastRelease) {
                cur->release = 1;
            }
            prev = cur;
        }
        cur = cur->next;
    }
    return 0;
}


int main() {
    int size;
    printf("Enter size of table: ");
    scanf("%d", &size);

    Item *data = (Item *)calloc(size, sizeof(Item));
    Table table = {data, size};

    while(1) {
        printf("\n1. Insert\n2. Delete\n3. Print table\n4. Find\n5. Clear\n6. Exit\nEnter your choice: ");
        int choice;
        scanf("%d", &choice);
        switch(choice) {
            case 1: {
                char key[100];
                unsigned info;
                printf("Enter key: ");
                scanf("%s", key);
                printf("Enter info: ");
                scanf("%u", &info);
                insert(&table, key, info);
                break;
            }
            case 2: {
                char key[100];
                printf("Enter key to delete: ");
                scanf("%s", key);
                delete(&table, key);
                break;
            }
            case 3: {
                printTable(&table);
                break;
            }
            case 4: {
                char key[100];
                int version;
                printf("Enter key: ");
                scanf("%s", key);
                printf("Enter version, or 0 to find all versions: ");
                scanf("%d", &version);
                find(&table, key, version);
                break;

            }
            case 5: {
                char key[100];
                printf("Enter key to keep only last version: ");
                scanf("%s", key);
                deleteAllExceptLast(&table, key);
                break;
            }
            case 6: {
                exit(0);
                break;
            }
            default: {
                printf("Invalid choice\n");
            }
        }
    }
    return 0;
}
