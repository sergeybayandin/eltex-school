#include <stdio.h>

void print_as_bytes(int num)
{
    int i;
    const unsigned char *pbyte = (const unsigned char *)(&num);

    for (i = 0; i < sizeof(num); ++i) {
        printf("%x\t", pbyte[i]);
    }
}

int main(void)
{
    int num = 0xaabbccdd;

    print_as_bytes(num);  puts("");

    unsigned char *pbyte = (unsigned char *)(&num);

    pbyte[2] = 0xee;

    print_as_bytes(num);  puts("");

    return 0;
}
