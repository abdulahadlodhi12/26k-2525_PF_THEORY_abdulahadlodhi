#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned long long sum;
    unsigned int a;
    unsigned int b;
} pair_sum_t;

typedef struct {
    unsigned long long val;
    unsigned int a1;
    unsigned int b1;
    unsigned int a2;
    unsigned int b2;
} taxicab_t;

static int cmp_pairs(const void *p1, const void *p2) {
    const pair_sum_t *x = (const pair_sum_t *)p1;
    const pair_sum_t *y = (const pair_sum_t *)p2;
    if (x->sum < y->sum) {
        return -1;
    }
    if (x->sum > y->sum) {
        return 1;
    }
    if (x->a != y->a) {
        return (x->a < y->a) ? -1 : 1;
    }
    return (x->b < y->b) ? -1 : (x->b > y->b);
}

static int cmp_taxi(const void *p1, const void *p2) {
    const taxicab_t *x = (const taxicab_t *)p1;
    const taxicab_t *y = (const taxicab_t *)p2;
    if (x->val < y->val) {
        return -1;
    }
    if (x->val > y->val) {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
	    if (argc != 2) {
        printf("Usage: %s taxi_number.txt",argv[0]);
        return 1;
    }
	FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("cant open %s, maybe check if the file is actully there\n", argv[1]);
        return 1;
    }
	long n_in = 0;
    if (fscanf(fp, "%ld", &n_in) != 1 || n_in < 1) {
        printf("n must be 1 or bigger, try agian\n");
        fclose(fp);
        return 1;
    }
    int target_n = (int)n_in;
    taxicab_t *found = NULL;
    int found_cnt = 0;
    unsigned int bound = 20;

    while (found_cnt < target_n) {
        size_t cap = (size_t)bound * ((size_t)bound + 1) / 2;
        pair_sum_t *arr = (pair_sum_t*)malloc(cap * sizeof(pair_sum_t));
        if (!arr) {
            printf("memory ran out while searching cubes\n");
            free(found);
            return 1;
        }

        size_t idx = 0;
        for (unsigned int a = 1; a <= bound; a++) {
            unsigned long long ca = (unsigned long long)a * a * a;
            for (unsigned int b = a; b <= bound; b++) {
                unsigned long long cb = (unsigned long long)b * b * b;
                arr[idx].sum = ca + cb;
                arr[idx].a = a;
                arr[idx].b = b;
                idx++;
            }
        }

        qsort(arr, idx, sizeof(pair_sum_t), cmp_pairs);

        free(found);
        found = NULL;
        found_cnt = 0;

        size_t i = 0;
        while (i < idx) {
            size_t j = i + 1;
            while (j < idx && arr[j].sum == arr[i].sum) {
                j++;
            }
            if (j - i >= 2) {
                taxicab_t *tmp = (taxicab_t*)realloc(found, (size_t)(found_cnt + 1) * sizeof(taxicab_t));
                if (!tmp) {
                    printf("realloc failed on taxicab list\n");
                    free(arr);
                    free(found);
                    return 1;
                }
                found = tmp;
                found[found_cnt].val = arr[i].sum;
                found[found_cnt].a1 = arr[i].a;
                found[found_cnt].b1 = arr[i].b;
                found[found_cnt].a2 = arr[i + 1].a;
                found[found_cnt].b2 = arr[i + 1].b;
                found_cnt++;
            }
            i = j;
        }

        free(arr);

        if (found_cnt < target_n) {
            bound += 20;
        }
    }

    qsort(found, (size_t)found_cnt, sizeof(taxicab_t), cmp_taxi);

    for (int k = 0; k < target_n; k++) {
        printf("%d. %llu = %u^3 + %u^3 = %u^3 + %u^3\n",
               k + 1,
               found[k].val,
               found[k].a1,
               found[k].b1,
               found[k].a2,
               found[k].b2);
    }

    free(found);
    return 0;
}
