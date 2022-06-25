#include <time.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <ncurses.h>

#define WINS_COUNT 2

struct window
{
    WINDOW         *win;
    char            curr_dir[PATH_MAX];
    int             curr_entry_pos;
    int             entries_count;
    struct dirent **entries;
};

struct file_manager
{
    int           active_win;
    struct window wins[WINS_COUNT];

} fm;

int  fm_init(int argc, char *argv[]);
int  fm_exec();
void fm_free();

int main(int argc, char *argv[])
{
    if (fm_init(argc, argv) != 0) {
        exit(EXIT_FAILURE);
    }
    
    const int code = fm_exec();

    fm_free();

    if (code != 0) {
        exit(EXIT_FAILURE);
    }

    return 0;
}

int fm_init(int argc, char *argv[])
{
    if (initscr() == NULL) {
        return -1;
    }
 
    clear();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    memset(&fm, sizeof(struct file_manager), 0);
    
    int win;
    int offset = 1;

    const char *default_dir = argc > 1 ? argv[1] : "/";

    for (win = 0; win < WINS_COUNT; ++win) {
        if ((fm.wins[win].win = subwin(stdscr, LINES - 2, COLS / 2 - 2, 1, offset)) == NULL) {
            fm_free();
            return -1;
        }

        if (realpath(default_dir, fm.wins[win].curr_dir) == NULL) {
            fm_free();
            return -1;
        }

        offset += COLS / 2 - 1;
    }

    return 0;
}

int fm_exec()
{
    int win, key, i, len;
    struct window *pw = NULL;
 
    struct stat st;
    struct dirent *dir;

    char stime[32];

    while (1) {
        for (win = 0; win < WINS_COUNT; ++win) {
            pw = fm.wins + win;

            if (pw->entries != NULL) {
                continue;
            }

            wclear(pw->win);

            box(pw->win, 0, 0);

            pw->entries_count = scandir(pw->curr_dir, &pw->entries, NULL, alphasort);

            if (pw->entries_count < 0) {
                return -1;
            }

            mvwprintw(pw->win, 1, 1, "Name");
            mvwprintw(pw->win, 1, (COLS / 2 - 2) / 3 + 1, "Size");
            mvwprintw(pw->win, 1, (COLS  - 4) / 3 + 1, "Modify time");

            len = strlen(pw->curr_dir);

            for (i = 0; i < pw->entries_count; ++i) {
                mvwprintw(pw->win, i + 2, 1, "%s", pw->entries[i]->d_name);

                if (pw->curr_dir[len - 1] != '/') {
                    sprintf(pw->curr_dir + len, "/%s", pw->entries[i]->d_name);
                } else {
                    sprintf(pw->curr_dir + len, "%s", pw->entries[i]->d_name);
                }

                stat(pw->curr_dir, &st);

                mvwprintw(pw->win, i + 2, (COLS / 2 - 2) / 3 + 1, "%d", st.st_size);
                strftime(stime, sizeof(stime), "%D %T", localtime(&st.st_mtim.tv_sec));
                mvwprintw(pw->win, i + 2, (COLS  - 4) / 3 + 1, "%s", stime);

                pw->curr_dir[len] = '\0';
            }

            wrefresh(pw->win);
        }

        pw = fm.wins + fm.active_win;

        mvwchgat(pw->win, pw->curr_entry_pos + 2, 1, COLS / 2 - 4, A_STANDOUT, 0, NULL);
        wrefresh(pw->win);

        key = getch();

        switch (key) {
            case KEY_LEFT : {
                if (fm.active_win - 1 >= 0) {
                    mvwchgat(pw->win, pw->curr_entry_pos + 2, 1, COLS / 2 - 4, A_NORMAL, 0, NULL);
                    wrefresh(pw->win);
                    --fm.active_win;
                }
                break;
            }
            case KEY_RIGHT : {
                if (fm.active_win + 1 < WINS_COUNT) {
                    mvwchgat(pw->win, pw->curr_entry_pos + 2, 1, COLS / 2 - 4, A_NORMAL, 0, NULL);
                    wrefresh(pw->win);
                    ++fm.active_win;
                }
                break;
            }
            case KEY_DOWN : {
                mvwchgat(pw->win, pw->curr_entry_pos + 2, 1, COLS / 2 - 4, A_NORMAL, 0, NULL);
                if (pw->curr_entry_pos + 1 < pw->entries_count) {
                    ++pw->curr_entry_pos;
                } else {

                    pw->curr_entry_pos = 0;
                }
                break;
            }
            case KEY_UP : {
                mvwchgat(pw->win, pw->curr_entry_pos + 2, 1, COLS / 2 - 4, A_NORMAL, 0, NULL);
                if (pw->curr_entry_pos - 1 >= 0) {
                    --pw->curr_entry_pos;
                } else {
                    pw->curr_entry_pos = pw->entries_count - 1;
                }
                break;
            }
            case 10 : {
                if (pw->entries == NULL) {
                    break;
                }
               
                dir = pw->entries[pw->curr_entry_pos];

                if (strcmp(dir->d_name, ".") == 0) {
                    break;
                }

                if (dir->d_type != DT_DIR) {
                    break;
                }
    
                len = strlen(pw->curr_dir);
            
                if (strcmp(dir->d_name, "..") == 0) {
                    i = len - 1;
                    while (pw->curr_dir[i] != '/') {
                        --i;
                    }
                    if (pw->curr_dir != pw->curr_dir + i) {
                        pw->curr_dir[i] = '\0';
                    } else {
                        pw->curr_dir[i + 1] = '\0';
                    }
                } else {
                    sprintf(pw->curr_dir + len, "/%s", dir->d_name);
                }

                for (i = 0; i < pw->entries_count; ++i) {
                    free(pw->entries[i]);
                }

                free(pw->entries);

                pw->entries = NULL;
                pw->entries_count = pw->curr_entry_pos = 0;

                break;
            }
            case 27 : {
                return 0;
            }
        }
    }

    return 0;
}

void fm_free()
{
    int win, i;

    for (win = 0; win < WINS_COUNT; ++win) {
        delwin(fm.wins[win].win);
        if (fm.wins[win].entries != NULL) {
            for (i = 0; i < fm.wins[win].entries_count; ++i) {
                free(fm.wins[win].entries[i]);
            }
            free(fm.wins[win].entries);
        }
    }
    
    endwin();
}
