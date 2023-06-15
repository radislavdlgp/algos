#ifndef DIALOG_H
#define DIALOG_H
#include "table.h"

int D_Add(Table *ptab);
int D_Delete(Table *ptab);
int D_Show(Table *ptab);
int D_Find(Table *ptab);
int dialog(const char *msgs[], int N);
void dialog_menu(Table *t);

#endif /*DIALOG_H*/
