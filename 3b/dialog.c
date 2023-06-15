#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dialog.h"
#include "table.h"

int getInt(int *a){
    int n;
    do{
        n = scanf("%d", a);
        if (n < 0) {
            return 0;
        }
        if (n == 0){
            printf("%s\n", "Wrong input");
            scanf("%*c", 0);
        }
    } while (n == 0);
    return 1;
}

char *getStr() {
    char *s = (char *)malloc(1);
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
      } else {
        len += strlen(buf);
        s = (char *) realloc(s, len + 1);
        s=strcat(s, buf);
      }
    } while (n > 0);
    return s;
}


const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
const char *errmsgs[] = {"Ok", "Table overflow", "Table is empty", "No these element", "Dublicate key"};

int D_Add(Table *ptab) {
        int rc;
        printf("Enter key:");
	scanf("%*c", NULL);
        char *key = getStr();
        printf("Enter info:");
	char *info = getStr();
        rc = table_insert(ptab, key, info);
	if(rc) {
		free(key); free(info);
	} printf("%s: %s\n", errmsgs[rc], key);
        return 1;
}


int D_Delete(Table *ptab) {
         int rc;
         printf("Enter key:");
	 scanf("%*c", NULL);
         char *key = getStr();
         rc = table_remove(ptab, key);
         printf("%s: %s\n", errmsgs[rc], key); free(key);
         return 1;
}


int D_Show(Table *ptab) {
        int rc = print(ptab);
	printf("%s", errmsgs[rc]);
        return 1;
}

int D_Find(Table *ptab) {
        int rc;
        printf("Enter key: --> ");
	scanf("%*c", NULL);
        char *key = getStr();
	rc = find(ptab, key);
	printf("%s", errmsgs[rc]);
	free(key);
	return 1;
}

int dialog(const char *msgs[], int N) {
        char *errmsg = "";
        int rc;
        int i, n;
        do {
                puts(errmsg);
                errmsg = "You are wrong. Repeate, please!";
                for(i = 0; i < N; ++i)
                        puts(msgs[i]);
                puts("Make your choice: --> ");
                n = getInt(&rc);
                if(n == 0)
                        rc = 0;
                } while(rc < 0 || rc >= N);
        return rc;
}

int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show};

void dialog_menu(Table *t) {
	int rc;
        while(rc = dialog(msgs, NMsgs))
                if(!fptr[rc](t))
                        break;
        printf("That's all. Bye!\n");
}

