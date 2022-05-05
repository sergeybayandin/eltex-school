#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;

    printf("n = ");
    scanf("%d", &n);

    int *mtx;
    int nn = n * n;

    if ((mtx = malloc(nn * sizeof(int))) == NULL) {
        fprintf(stderr, "failed to allocate memory\n");
        return EXIT_FAILURE;
    }
    
    int i, j;

    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            mtx[i * n + j] = j < n - i - 1 ? 1 : 0;
        }
    }

    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            printf("%d\t", mtx[i * n + j]);
        }
        printf("\n");
    }  

    free(mtx);

    return EXIT_SUCCESS;
}
