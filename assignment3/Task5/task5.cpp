#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 0;
    printf("type an odd number n for the star pattrn: ");
    if (scanf("%d", &n) != 1 || n < 3 || n % 2 == 0) {
        printf("pls give an odd integer like 7 or 21\n");
        return 1;
    }

    int arm_len = n;
    int total_rows = n * 3 + 2;
    int mid = n * 2 + 1;
    int width = mid * 2 + 1;
    int bar_row = n + 1;

    for (int r = 0; r < total_rows; r++) {
        for (int c = 0; c < width; c++) {
            int draw = 0;

            if (r < bar_row) {
                int left_slope = mid - r * 2;
                int right_slope = mid + r * 2;
                if (c == left_slope || c == right_slope) {
                    draw = 1;
                }
            } else if (r == bar_row) {
                if ((c >= 0 && c <= mid - n) || (c >= mid + n && c < width)) {
                    if (c % 2 == 0) {
                        draw = 1;
                    }
                }
            } else {
                int dr = r - bar_row;
                int outer_l = (mid - n) - dr;
                int outer_r = (mid + n) + dr;
                int inner_l = (mid - n / 2) + dr;
                int inner_r = (mid + n / 2) - dr;

                if (outer_l >= 0 && c == outer_l) {
                    draw = 1;
                }
                if (outer_r < width && c == outer_r) {
                    draw = 1;
                }
                if (inner_l < inner_r && (c == inner_l || c == inner_r)) {
                    draw = 1;
                }
            }

            if (draw && arm_len > 0) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}
