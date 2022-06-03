#include <stdio.h>

struct aligned_struct
{
    char a;
    int  b;
};

struct packed_struct
{
    char a;
    int  b;
}__attribute__((packed));

int main(void)
{
    printf("sizeof(struct aligned_struct) = %zu\n", sizeof(struct aligned_struct));
    printf("sizeof(struct packed_struct) = %zu\n", sizeof(struct packed_struct));

    const char bytes[] = {'A', 0, 0, 0, 0, 'B', 0, 0, 0, 0};

    const struct aligned_struct *paligned = (const struct aligned_struct *)(bytes);
    const struct packed_struct  *ppacked  = (const struct packed_struct *)(bytes);

    printf("a = %c, b = %d\n", ppacked[0].a, ppacked[0].b); // ok, a = 'A', b = 0
    printf("a = %c, b = %d\n", ppacked[1].a, ppacked[1].b); // ok, a = 'B', b = 0

    printf("a = %c, b = %d\n", paligned[0].a, paligned[0].b); // hmm... but still ok
    printf("a = %c, b = %d\n", paligned[1].a, paligned[1].b); // that's really bad thing, but this can work...

    return 0;
}
