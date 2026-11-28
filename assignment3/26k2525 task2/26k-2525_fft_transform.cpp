#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float *data_real;
    float *data_imag;
    float ***real_vol;
    float ***imag_vol;
    float **real_rows;
    float **imag_rows;
    int n;
} vol3d;

void fft1d(float *real, float *imag, int n) {
    if (n <= 1) {
        return;
    }
    float *out_r = (float*)malloc((size_t)n * sizeof(float));
    float *out_i = (float*)malloc((size_t)n * sizeof(float));
    for (int k = 0; k < n; k++) {
        double sum_r = 0.0;
        double sum_i = 0.0;
        for (int t = 0; t < n; t++) {
            double angle = -2.0 * M_PI * k * t / n;
            double ca = cos(angle);
            double sa = sin(angle);
            sum_r += real[t] * ca - imag[t] * sa;
            sum_i += real[t] * sa + imag[t] * ca;
        }
        out_r[k] = (float)sum_r;
        out_i[k] = (float)sum_i;
    }
    for (int i = 0; i < n; i++) {
        real[i] = out_r[i];
        imag[i] = out_i[i];
    }
    free(out_r);
    free(out_i);
}

void fft2d(float **real_sheet, float **imag_sheet, int n) {
    for (int i = 0; i < n; i++) {
        fft1d(real_sheet[i], imag_sheet[i], n);
    }

    float *col_r = (float*)malloc((size_t)n * sizeof(float));
    float *col_i = (float*)malloc((size_t)n * sizeof(float));

    for (int c = 0; c < n; c++) {
        for (int r = 0; r < n; r++) {
            col_r[r] = real_sheet[r][c];
            col_i[r] = imag_sheet[r][c];
        }
        fft1d(col_r, col_i, n);
        for (int r = 0; r < n; r++) {
            real_sheet[r][c] = col_r[r];
            imag_sheet[r][c] = col_i[r];
        }
    }

    free(col_r);
    free(col_i);
}

vol3d *create_vol3d(int n) {
    vol3d *v = (vol3d*)malloc(sizeof(vol3d));
    v->n = n;
    size_t total = (size_t)n * (size_t)n * (size_t)n;
    size_t total_rows = (size_t)n * (size_t)n;

    v->data_real = (float*)calloc(total, sizeof(float));
    v->data_imag = (float*)calloc(total, sizeof(float));
    v->real_rows = (float**)malloc(total_rows * sizeof(float **));
    v->imag_rows = (float**)malloc(total_rows * sizeof(float **));
    v->real_vol = (float***)malloc((size_t)n * sizeof(float ***));
    v->imag_vol = (float***)malloc((size_t)n * sizeof(float ***));

    for (int s = 0; s < n; s++) {
        v->real_vol[s] = v->real_rows + (size_t)s * n;
        v->imag_vol[s] = v->imag_rows + (size_t)s * n;
        for (int r = 0; r < n; r++) {
            size_t offset = ((size_t)s * n + (size_t)r) * n;
            v->real_vol[s][r] = &v->data_real[offset];
            v->imag_vol[s][r] = &v->data_imag[offset];
        }
    }
    return v;
}

void destroy_vol3d(vol3d *v) {
    if (!v) {
        return;
    }
    free(v->data_real);
    free(v->data_imag);
    free(v->real_rows);
    free(v->imag_rows);
    free(v->real_vol);
    free(v->imag_vol);
    free(v);
}

int main(void) {
    FILE *fp = fopen("grid3d.txt", "r");
    if (!fp) {
        printf("couldnt open grid3d.txt, check the folder\n");
        return 1;
    }

    int n = 0;
    if (fscanf(fp, "%d", &n) != 1 || n <= 0) {
        printf("bad grid size in grid3d.txt\n");
        fclose(fp);
        return 1;
    }

    vol3d *orig = create_vol3d(n);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            for (int z = 0; z < n; z++) {
                if (fscanf(fp, "%f", &orig->real_vol[x][y][z]) != 1) {
                    printf("not enugh values in grid3d.txt\n");
                    destroy_vol3d(orig);
                    fclose(fp);
                    return 1;
                }
            }
        }
    }
    fclose(fp);

    for (int z = 0; z < n; z++) {
        fft2d(orig->real_vol[z], orig->imag_vol[z], n);
    }

    vol3d *trans = create_vol3d(n);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            for (int z = 0; z < n; z++) {
                trans->real_vol[z][y][x] = orig->real_vol[x][y][z];
                trans->imag_vol[z][y][x] = orig->imag_vol[x][y][z];
            }
        }
    }

    for (int s = 0; s < n; s++) {
        fft2d(trans->real_vol[s], trans->imag_vol[s], n);
    }

    printf("magnitude slice at z = 0 after full 3d fft:\n");
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            float re = trans->real_vol[0][y][x];
            float im = trans->imag_vol[0][y][x];
            float mag = sqrtf(re * re + im * im);
            printf("%.3f ", mag);
        }
        printf("\n");
    }

    destroy_vol3d(orig);
    destroy_vol3d(trans);
    return 0;
}
