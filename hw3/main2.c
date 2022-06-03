#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LEN     16
#define MAX_LASTNAME_LEN 16
#define PHONE_NUMBER_LEN 11

struct sub
{
    char name[MAX_NAME_LEN + 2];
    char lastname[MAX_LASTNAME_LEN + 2];
    char phone_number[PHONE_NUMBER_LEN + 2];   
};

struct subdir
{
    int capacity, count;
    struct sub *storage;
};

int subdir_reserve(struct subdir *pdir, int new_capacity)
{
    if (pdir == NULL) {
        return -1;
    }

    if (new_capacity < pdir->capacity) {
        return 0;
    }

    if ((pdir->storage = realloc(pdir->storage, sizeof(struct sub) * new_capacity)) == NULL) {
        return -1;
    }

    pdir->capacity = new_capacity;

    return 0;
}

int subdir_add(struct subdir *pdir, const struct sub *pnew_sub)
{
    if (pdir->count == pdir->capacity && 
        subdir_reserve(pdir, pdir->capacity == 0 ? 1 : pdir->capacity * 2) != 0) {

        return -1;
    }

    memcpy(pdir->storage + pdir->count, pnew_sub, sizeof(struct sub));

    ++pdir->count;

    return 0;
}

struct sub *subdir_search(const struct subdir *pdir, void *pvalue, int (*cmp)(const struct sub *, void *))
{
    if (pdir == NULL || cmp == NULL) {
        return NULL;
    }

    struct sub *pfirst_sub = pdir->storage;
    const struct sub *plast_sub  = pdir->storage + pdir->count;
    
    for (; pfirst_sub < plast_sub; ++pfirst_sub) {
        if (cmp(pfirst_sub, pvalue) == 0) {
            break;
        }
    }

    if (pfirst_sub == plast_sub) {
        return NULL;
    }

    return pfirst_sub;
}

int subdir_rm(struct subdir *pdir, int index)
{
    if (pdir == NULL || index < 0 || index >= pdir->count) {
        return -1;
    }
    
    if (pdir->count > 1) {
        memcpy(pdir->storage + index, pdir->storage + pdir->count - 1, sizeof(struct sub));
    }

    --pdir->count;

    return 0;
}

void subdir_print(const struct subdir *pdir)
{
    if (pdir == NULL) {
        return;
    }
    
    int i;

    for (i = 0; i < pdir->count; ++i) {
        printf("%zu. %s %s %s\n", i + 1, 
                                  pdir->storage[i].lastname, 
                                  pdir->storage[i].name,
                                  pdir->storage[i].phone_number);
    }
}

void subdir_free(struct subdir *dir)
{
    if (dir == NULL) {
        return;
    }

    free(dir->storage);
}

int read_int(int *pnum)
{
    if (pnum == NULL) {
        return -1;
    }
    
    int i;
    char buf[16];

    for (i = 0; i < sizeof(buf) - 1; ++i) {
        buf[i] = getchar();

        if (buf[i] == '\n' || buf[i] == EOF) {
            break;
        }
    }   

    buf[i] = 0;

    if (isdigit(*buf) == 0) {
        return -1;
    }

    *pnum = atoi(buf);

    return 0;
}

int search_name(const struct sub *psub, const char *name)
{
    return strcmp(psub->name, name);
}

int search_lastname(const struct sub *psub, const char *lastname)
{
    return strcmp(psub->lastname, lastname);
}

int search_phone_number(const struct sub *psub, const char *phone_number)
{
    return strcmp(psub->phone_number, phone_number);
}

int main(void)
{
    struct subdir dir;
    struct subdir *pdir = &dir;

    memset(pdir, sizeof(struct subdir), 0);

    struct sub sub;
    struct sub *psub = &sub, *psub1 = NULL;

    memset(psub, sizeof(struct sub), 0);
    
    int index   = -1;
    int choice  = 0;
    int running = 1;

    char *pch = NULL;

    for ( ; running != 0; ) {
        puts("1. Add");
        puts("2. Rm");
        puts("3. Search");
        puts("4. Print");
        puts("5. Exit");

        if (read_int(&choice) != 0) {
            continue;
        }

        switch (choice) {
            case 1: {
                puts("Enter name: ");
                
                if (fgets(sub.name, sizeof(sub.name), stdin) == NULL && ferror(stdin) != 0) {
                    perror("Failed to read name");
                    break;
                }

                if ((pch = strchr(sub.name, '\n')) != NULL) {
                    *pch = '\0';
                }

                puts("Enter lastname: ");
                
                if (fgets(sub.lastname, sizeof(sub.lastname), stdin) == NULL && ferror(stdin) != 0) {
                    perror("Failed to read lastname");
                    break;
                }

                if ((pch = strchr(sub.lastname, '\n')) != NULL) {
                    *pch = '\0';
                }

                puts("Enter phone number: ");
                
                if (fgets(sub.phone_number, sizeof(sub.phone_number), stdin) == NULL && ferror(stdin) != 0) {
                    perror("Failed to read phone number");
                    break;
                }

                if ((pch = strchr(sub.phone_number, '\n')) != NULL) {
                    *pch = '\0';
                }

                if (subdir_add(pdir, psub) != 0) {
                    fprintf(stderr, "Failed to add sub\n");
                }

                break;
            }
    
            case 2: {
                if (pdir->count == 0) {
                    puts("Subdir is empty");
                    break;
                }

                printf("Enter number from 1 to %d: ", pdir->count);

                if (read_int(&index) != 0 || index < 1 || index > pdir->count) {
                    fprintf(stderr, "Failed to read number of sub\n");
                    break;
                }

                --index;

                if (subdir_rm(pdir, index) != 0) {
                    fprintf(stderr, "Failed to rm sub\n");
                }

                break;
            }

            case 3: {
                puts("1. Search name");
                puts("2. Search lastname");
                puts("3. Search phone number");
        
                if (read_int(&choice) != 0) {
                    break;
                }

                switch (choice) {
                    case 1: {
                        if (fgets(psub->name, sizeof(psub->name), stdin) == NULL && ferror(stdin) != 0) {
                            perror("Failed to read name");
                            break;
                        }

                        pch = strchr(psub->name, '\n');

                        if (pch != NULL) {
                            *pch = '\0';
                        }

                        psub1 = subdir_search(pdir, psub->name, (int (*)(const struct sub *, void *))(search_name));

                        if (psub1 == NULL) {
                            break;
                        }

                        printf("%s %s %s\n", psub1->name, psub1->lastname, psub2->phone_number);

                        break;
                    }

                    case 2: {
                        if (fgets(psub->lastname, sizeof(psub->lastname), stdin) == NULL && ferror(stdin) != 0) {
                            perror("Failed to read name");
                            break;
                        }

                        pch = strchr(psub->lastname, '\n');

                        if (pch != NULL) {
                            *pch = '\0';
                        }

                        psub1 = subdir_search(pdir, psub->lastname, (int (*)(const struct sub *, void *))(search_lastname));

                        if (psub1 == NULL) {
                            break;
                        }

                        printf("%s %s %s\n", psub1->name, psub1->lastname, psub2->phone_number);

                        break;
                    }

                    case 3: {
                        if (fgets(psub->phone_number, sizeof(psub->phone_number), stdin) == NULL && ferror(stdin) != 0) {
                            perror("Failed to read name");
                            break;
                        }

                        pch = strchr(psub->phone_number, '\n');

                        if (pch != NULL) {
                            *pch = '\0';
                        }

                        psub1 = subdir_search(pdir, psub->phone_number, (int (*)(const struct sub *, void *))(search_phone_number));

                        if (psub1 == NULL) {
                            break;
                        }

                        printf("%s %s %s\n", psub1->name, psub1->lastname, psub2->phone_number);

                        break;
                    }
                }

                break;
            }

            case 4: {
                subdir_print(pdir);
                break;
            }

            case 5: {
                running = 0;
                break;
            }
        }
    }

    subdir_free(pdir);

    return EXIT_SUCCESS;
}
