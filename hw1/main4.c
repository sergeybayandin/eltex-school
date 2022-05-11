#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;

    printf("n = ");
    scanf("%d", &n);

    if (n % 2 == 0) {
        fprintf(stderr, "n must be odd\n");
        return EXIT_FAILURE;
    }

    int *mtx;
    int nn = n * n;

    if ((mtx = malloc(nn * sizeof(int))) == NULL) {
        fprintf(stderr, "failed to allocate memory\n");
        return EXIT_FAILURE;
    }
    
    int left_top = 0;
    int origin = 0, width = n, i;

    while (width > 1) {
        for (i = 0; i < width - 1; ++i) {
            mtx[left_top + i] = origin + i + 1;
            mtx[left_top + width - 1 + i * n] = origin + i + width;
            mtx[left_top + (width - 1) * (n + 1) - i] = origin + i + 2 * width - 1;
            mtx[left_top + n * (width - i - 1)] = origin + i + 3 * width - 2;
        }

        width -= 2;
        left_top += n + 1;
        origin = mtx[left_top - 1];
    }

    mtx[(n / 2) * n + n / 2] = nn;

    for (i = 0; i < n; ++i) {
        for (k = 0; k < n; ++k) {
            printf("%d\t", mtx[i * n + k]);
        }
        printf("\n");
    }  

    free(mtx);

    return EXIT_SUCCESS;
}
