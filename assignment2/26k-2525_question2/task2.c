#include <stdio.h>

int sumofdivisors(int num) {
    int sum = 0;
    for (int i = 1; i <= num / 2; i++) {
        if (num % i == 0) {
            sum += i;
        }
    }
    return sum;
}

int main() {
    int n;
    printf("Enter a postive number n: ");
    scanf("%d", &n);

    int count = 0;
    printf("\nAmicable pairs found:\n");

    for (int i = 1; i <= n; i++) {
        int j = sumofdivisors(i);
        if (j > i && j <= n) {
            if (sumofdivisors(j) == i) {
                printf("(%d, %d)\n", i, j);
                count++;
            }
        }
    }

    if (count == 0) {
        printf("No amicable pairs found up to %d.\n", n);
    } else {
        printf("Total amicable pairs found: %d\n", count);
    }

    return 0;
}