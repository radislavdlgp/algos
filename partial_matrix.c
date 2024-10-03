#include <stdio.h>
#include <stdlib.h>

typedef struct Line {
    int n;
    int* mas;
}Line;

typedef struct Matrix {
    int lines;
    Line* matr;
}Matrix;

int geti(int* a) {
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0) {
            return 0;
        }
        if (n == 0) {
            printf("%s\n", "Error! Repeat input");
            scanf("%*c", 0);
        }
    } while (n == 0);
    return 1;
}

void erase(Matrix* a) {
    int i;
    for (i = 0; i < a->lines; ++i)
        free(a->matr[i].mas);
    free(a->matr);
    a->lines = 0;
    a->matr = NULL;
}

int getm(Matrix* rm) {
    const char* pr = "";
    int m;
    int* p;
    do {
        printf("%s\n", pr);
        printf("Enter number of the lines :");
        pr = "You are wrong, repeat";
        if (geti(&m) == 0) {
            return 0;
        }
    } while (m < 1);
    rm->lines = m;
    rm->matr = (Line*)calloc(m, sizeof(Line));
    for (int i = 0; i < rm->lines; ++i) {
        pr = "";
        do {
            printf("%s\n", pr);
            printf("Enter number of items in line %d:", i + 1);
            pr = "You are wrong; repeat, please!";
            if (geti(&m) == 0) {
                rm->lines = i;
                erase(rm);
                return 0;
            }
        } while (m < 1);
        rm->matr[i].n = m;
        p = (int*)malloc(sizeof(int) * m);
        rm->matr[i].mas = p;
        printf("Enter items for matrix line %d:\n", i + 1);
        for (int j = 0; j < m; ++j, ++p)
            if (geti(p) == 0) {
                rm->lines = i + 1;
                erase(rm);
                return 0;
            }
    }
    return 1;
}

int ind(Matrix* pt) {
    int* vectb = malloc(sizeof(int) * pt->lines);
    int sr, summ;
    for (int i = 0; i < pt->lines; ++i) {
        summ = 0;
        int c = 0;
        for (int j = 0; j < pt->matr[i].n; ++j) {
            c++;
            summ += pt->matr[i].mas[j];
        }
        sr = summ / c;
        int sumstr = 0;
        for (int j = 0; j < pt->matr[i].n; ++j) {
            if (pt->matr[i].mas[j] > sr) {
                sumstr += pt->matr[i].mas[j];
            }
        }
        vectb[i] = sumstr;
    }
    printf("Result vector :\n[ ");
    for (int i = 0; i < pt->lines; ++i) {
        printf("%d ", vectb[i]);
    }
    printf("]");
    printf("\n");
    return *vectb;
}

void output(const char* msg, Matrix* a) {
    Line* w;
    int len;
    int* p;
    printf("%s:\n", msg);
    for (int i = 0; i < a->lines; ++i) {
        w = a->matr + i;
        p = w->mas;
        len = w->n;
        for (int j = 0; j < len; ++j, ++p)
            printf("%d ", *p);
        printf("\n");
    }
}

int main() {
    Matrix matr = { 0,NULL };
    int* mas = malloc(sizeof(char) * 20);
    if (getm(&matr) == 0) {
        printf("%s\n", "End of file occured");
        return 1;
    }
    ind(&matr);
    output("Source matrix", &matr);
    erase(&matr);
    return(0);
}
