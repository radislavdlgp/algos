#include <stdio.h>
#include <stdlib.h>
#include "dialog.h"
#include "table.h"

void table_destroy(Table *t) {
	for(int i = 0; i < t->csize; ++i) {
		free(t->first[i].key); free(t->first[i].info);
	}
}

int main() {
	Item first[10] = {NULL, NULL};
	Table t = {first, 0, 10};
        dialog_menu(&t);
	table_destroy(&t);
        return 0;
}

