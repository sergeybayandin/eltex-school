#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, i;

    printf("n = ");
    scanf("%d", &n);

    int *arr;

    if ((arr = malloc(n * sizeof(int))) == NULL) {
        fprintf(stderr, "failed to allocate memory\n");
        return EXIT_FAILURE;
    }

    printf("array = ");

    for (i = 0; i < n; ++i) {
        scanf("%d", arr + i);
    }

    int t;
    int *pl = arr;
    int *pr = arr + (n - 1);

    for (; pl < pr; ++pl, --pr) {
        t = *pl;
        *pl = *pr;
        *pr = t;
    }

    printf("array(reversed) = ");

    for (i = 0; i < n; ++i) {
        printf("%d\t", arr[i]);
    }
    printf("\n");

    free(arr);

    return EXIT_SUCCESS;
}
