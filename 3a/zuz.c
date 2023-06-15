

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char* key;
    char* info;
} ITEM;

typedef struct {
    ITEM* arr[100];
    int msize;
    int csize;
} TABLE;

char* getstr() {
    char* s = (char*)malloc(1);
    char buf[81];
    int n, len = 0;
    *s = '\0';
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            free(s);
            s = NULL;
            continue;
        }
        if (n == 0) {
            scanf("%*c");
        }
        else {
            len += strlen(buf);
            s = (char*)realloc(s, len + 1);
            s = strcat(s, buf);
        }
    } while (n > 0);
    return s;
}

TABLE* initTable() {
    TABLE* table = (TABLE*)malloc(sizeof(TABLE));
    return table;
}







void add_Item(TABLE* table, char* key, char* info) {
    ITEM* new_element = (ITEM*)malloc(sizeof(ITEM));
    printf("Input key's value: ");
    scanf("%*c", NULL);
    new_element->key = getstr();
    printf("\n");
    printf("Input element's value: ");
    new_element->info = getstr();
    printf("\n");
    table->msize = 6;
    int i = table->csize;
    if (i >= table->msize)

        printf("There is no space for new element!\n");
    else
    {
        table->arr[i] = new_element;
        table->csize += 1;
    }
}


void fItem(TABLE* table, char* key)
{
    ITEM* new_element = (ITEM*)malloc(sizeof(ITEM));
    printf("Input key's value: ");
    scanf("%*c", NULL);
    new_element->key = getstr();
    printf("\n");
    int i = 0;
    int c = 0;
    for (i; i < table->csize; i++)
    {
        if (strcmp(new_element->key, table->arr[i]->key) == 0) {
            printf("Success,the key exists: %s\n", new_element->key);
            c++;
            break;
        }
    }
    if (c == 0)
        printf("FAILED!\n");
}

void new_item(TABLE* table, char* key, char* info)
{
    ITEM* new_element = (ITEM*)malloc(sizeof(ITEM));
    printf("Input key's value: ");
    new_element->key = getstr();
    printf("\n");
    printf("Input element's value: ");
    new_element->info = getstr();
    printf("\n");
    if (table->csize >= table->msize)
    {
        printf("There is no space in the table \n");
    }
    else
    {
        int i = table->csize;
        table->arr[i] = new_element;
        table->csize += 1;
    }
}
void deleteItem(TABLE* table, char* key) {
    int i, j;
    ITEM* new_element = (ITEM*)malloc(sizeof(ITEM));
    printf("Input key's value: ");
    scanf("%*c", NULL);
    new_element->key = getstr();
    printf("\n");
    for (i = 0; i < table->csize; i++) {
        if (strcmp(new_element->key, table->arr[i]->key) == 0) {
            free(table->arr[i]->key);
            free(table->arr[i]->info);
            free(table->arr[i]);
            for (j = i; j < table->csize - 1; j++) {
                table->arr[j] = table->arr[j + 1];
            }
            table->csize--;
        }
    }
}

void sort_elems(TABLE* table)
{

    int N = table->csize;
    ITEM* new_element = (ITEM*)malloc(sizeof(ITEM));
    for (int i = N - 1; i >= 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (strcmp(table->arr[j + 1]->key, table->arr[j]->key) < 0)
            {
                new_element = table->arr[j];
                table->arr[j] = table->arr[j + 1];
                table->arr[j + 1] = new_element;
            }
        }
    }
}


void printTable(TABLE* table)
{
    int i;
    printf("Table:\n");
    sort_elems(table);
    if (table->msize > 0)
    {
        for (i = 0; i < table->csize; i++) {
            printf("%s %s\n", table->arr[i]->key, table->arr[i]->info);
        }
    }
}


int dialog(const char* msgs[], int N)
{
    char* errmsg = "";
    int rc;
    int i, n;
    do {
        puts(errmsg);
        errmsg = "You are wrong. Repeate, please!";
        for (i = 0; i < N; ++i)
            puts(msgs[i]);
        puts("Make your choice: --> ");
        n = scanf("%d", &rc);
        if (n == 0)
            rc = 0;
    } while (rc < 0 || rc >= N);
    return rc;
}


void main()
{
    int (*fptr[])(TABLE*) = { NULL, add_Item, fItem, deleteItem, printTable };
    const char* msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show" };
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    TABLE* table = initTable();
    table->msize = 6;
    table->csize = 0;
    int rc;
    while (rc = dialog(msgs, NMsgs))
    {
        if (!fptr[rc](&table))
        {
            printf("error");
            break;
        }
    }
    printf("That's all. Bye!\n");
    printTable(table);
    free(table);
}