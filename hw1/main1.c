#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, i;
    
    printf("n = ");
    scanf("%d", &n);

    int *mtx;
    int nn = n * n;

    if ((mtx = malloc(sizeof(int) * nn)) == NULL) {
        fprintf(stderr, "failed to allocate memory\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < nn; ++i) {
        mtx[i] = i + 1;
    }

    for (i = 0; i < nn; ++i) {
        if (i % n == 0) {
            printf("\n");
        }
        printf("%d\t", mtx[i]);
    }

    printf("\n");

    free(mtx);

    return EXIT_SUCCESS;
}
