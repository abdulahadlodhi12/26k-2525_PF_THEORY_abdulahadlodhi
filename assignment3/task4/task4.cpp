#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 0;
    printf("enter an odd number n for the pentagon: ");
    if (scanf("%d", &n) != 1 || n < 3 || n % 2 == 0) {
        printf("bro it needs to be a positive odd numbr >= 3\n");
        return 1;
    }

    int half = n / 2;
    int top_rows = half + 1;
    int bot_rows = half;
    int total_rows = top_rows + bot_rows;
    int mid_col = n + half;
    int width = 2 * mid_col + 1;

    for (int r = 0; r < total_rows; r++) {
        for (int c = 0; c < width; c++) {
            int draw = 0;

            if (r < top_rows) {
                int left_edge = mid_col - r * 2;
                int right_edge = mid_col + r * 2;
                if (c == left_edge || c == right_edge) {
                    draw = 1;
                }
            } else {
                int br = r - top_rows;
                if (r == total_rows - 1) {
                    if (c >= half && c <= width - 1 - half && ((c - half) % 2 == 0)) {
                        draw = 1;
                    }
                } else {
                    int left_wall = half;
                    int right_wall = width - 1 - half;
                    if (br == 0) {
                        left_wall = mid_col - (top_rows - 1) * 2;
                        right_wall = mid_col + (top_rows - 1) * 2;
                    }
                    if (c == left_wall || c == right_wall) {
                        draw = 1;
                    }
                }
            }

            if (draw) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}
