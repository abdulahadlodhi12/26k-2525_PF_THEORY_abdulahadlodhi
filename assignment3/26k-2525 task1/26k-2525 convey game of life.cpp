#include <stdio.h>
#include <stdlib.h>

int count_live_neighbors(int **grid, int n, int r, int c) {
    int cnt = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < n && nc >= 0 && nc < n) {
                if (grid[nr][nc] == 1) {
                    cnt++;
                }
            }
        }
    }
    return cnt;
}

int **alloc_grid(int n) {
    int **g = (int**)malloc((size_t)n * sizeof(int *));
    if (!g) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        g[i] = (int*)malloc((size_t)n * sizeof(int));
        if (!g[i]) {
            for (int j = 0; j < i; j++) {
                free(g[j]);
            }
            free(g);
            return NULL;
        }
    }
    return g;
}

void free_grid(int **g, int n) {
    if (!g) {
        return;
    }
    for (int i = 0; i < n; i++) {
        free(g[i]);
    }
    free(g);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage %s convey_game_of_life_input.txt",argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("cant open %s, maybe check if the file is actully there\n", argv[1]);
        return 1;
    }

    int n = 0;
    int g = 0;
    if (fscanf(fp, "%d", &n) != 1 || fscanf(fp, "%d", &g) != 1 || n <= 0 || g < 0) {
        printf("somethng looks off with n or g in the file\n");
        fclose(fp);
        return 1;
    }

    int **curr_grid = alloc_grid(n);
    int **next_grid = alloc_grid(n);
    if (!curr_grid || !next_grid) {
        printf("out of memroy while making grids\n");
        free_grid(curr_grid, n);
        free_grid(next_grid, n);
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(fp, "%d", &curr_grid[i][j]) != 1) {
                printf("ran out of grid numbers too early at row %d\n", i);
                free_grid(curr_grid, n);
                free_grid(next_grid, n);
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);

    for (int gen = 0; gen < g; gen++) {
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                int alive = count_live_neighbors(curr_grid, n, r, c);
                if (curr_grid[r][c] == 1) {
                    if (alive < 2 || alive > 3) {
                        next_grid[r][c] = 0;
                    } else {
                        next_grid[r][c] = 1;
                    }
                } else {
                    if (alive == 3) {
                        next_grid[r][c] = 1;
                    } else {
                        next_grid[r][c] = 0;
                    }
                }
            }
        }
        int **temp = curr_grid;
        curr_grid = next_grid;
        next_grid = temp;
    }

    printf("grid after %d generatons:\n", g);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", curr_grid[i][j]);
        }
        printf("\n");
    }

    free_grid(curr_grid, n);
    free_grid(next_grid, n);
    return 0;
}
