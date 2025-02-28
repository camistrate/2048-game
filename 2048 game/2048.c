#include <ctype.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define COUNT 10
#define MAX 50
#define FOREVER 1
#define INIT_ROW 20
#define INIT_COL 20

// alocare memorie matrice
int **mat() {
    int i;
    int **p = malloc(4 * sizeof(int *));
    for (i = 0; i < 4; i++) p[i] = (int *)malloc(4 * sizeof(int));
    return p;
}

// initializarea perechilor de culori
void init_colors() {
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_GREEN, COLOR_BLACK);
}

// afisare 2048
void draw_2048(WINDOW *game, int i, int ncols) {
    wattron(game, COLOR_PAIR(i % 7));
    mvwprintw(game, 7, ncols / 2 - 18, " _______ ");
    mvwprintw(game, 8, ncols / 2 - 18, "|       |");
    mvwprintw(game, 9, ncols / 2 - 18, "|____   |");
    mvwprintw(game, 10, ncols / 2 - 18, " ____|  |");
    mvwprintw(game, 11, ncols / 2 - 18, "| ______|");
    mvwprintw(game, 12, ncols / 2 - 18, "| |_____ ");
    mvwprintw(game, 13, ncols / 2 - 18, "|_______|");
    wattroff(game, COLOR_PAIR(i % 7));

    wattron(game, COLOR_PAIR((i + 1) % 7));
    mvwprintw(game, 7, ncols / 2 - 9, " _______ ");
    mvwprintw(game, 8, ncols / 2 - 9, "|  _    |");
    mvwprintw(game, 9, ncols / 2 - 9, "| | |   |");
    mvwprintw(game, 10, ncols / 2 - 9, "| | |   |");
    mvwprintw(game, 11, ncols / 2 - 9, "| |_|   |");
    mvwprintw(game, 12, ncols / 2 - 9, "|       |");
    mvwprintw(game, 13, ncols / 2 - 9, "|_______|");
    wattroff(game, COLOR_PAIR((i + 1) % 7));

    wattron(game, COLOR_PAIR((i + 2) % 7));
    mvwprintw(game, 7, ncols / 2, " _   ___ ");
    mvwprintw(game, 8, ncols / 2, "| | |   |");
    mvwprintw(game, 9, ncols / 2, "| |_|   |");
    mvwprintw(game, 10, ncols / 2, "|       |");
    mvwprintw(game, 11, ncols / 2, "|___    |");
    mvwprintw(game, 12, ncols / 2, "    |   |");
    mvwprintw(game, 13, ncols / 2, "    |___|");
    wattroff(game, COLOR_PAIR((i + 2) % 7));

    wattron(game, COLOR_PAIR((i + 3) % 7));
    mvwprintw(game, 7, ncols / 2 + 9, "  _____  ");
    mvwprintw(game, 8, ncols / 2 + 9, " |  _  | ");
    mvwprintw(game, 9, ncols / 2 + 9, " | |_| | ");
    mvwprintw(game, 10, ncols / 2 + 9, "|   _   |");
    mvwprintw(game, 11, ncols / 2 + 9, "|  | |  |");
    mvwprintw(game, 12, ncols / 2 + 9, "|  |_|  |");
    mvwprintw(game, 13, ncols / 2 + 9, "|_______|");
    wattroff(game, COLOR_PAIR((i + 3) % 7));
}

// grafica meniu
char draw_menu(WINDOW *game) {
    const char *variante[] = {"New Game", "Resume", "Top Players", "Quit"};
    int alegere, highlight = 0, i, j = 0, ok = 1, nrows, ncols;

    // nu asteapta input
    nodelay(game, TRUE);

    init_colors();
    wclear(game);
    box(game, 0, 0);
    getmaxyx(game, nrows, ncols);
    keypad(game, true);

    // crearea meniului
    while (ok) {
        draw_2048(game, j, ncols);
        for (i = 0; i < 4; i++) {
            // evidentierea elementului selectat din meniu prin inversarea
            // culorilor
            if (i == highlight) wattron(game, A_REVERSE);
            mvwprintw(game, nrows / 2 + i - 3, ncols / 2 - 4, "%s",
                      variante[i]);
            // revenirea la formatarea normala (fara evidentiere)
            wattroff(game, A_REVERSE);
        }
        wrefresh(game);

        alegere = wgetch(game);
        // verificare tip alegere si returnarea ei
        if (alegere == KEY_ENTER || alegere == '\n') {
            if (highlight == 0) {
                wclear(game);
                ok = 0;
                return 'n';
            }
            if (highlight == 1) {
                wclear(game);
                ok = 0;
                return 'r';
            }
            if (highlight == 2) {
                wclear(game);
                ok = 0;
                return 't';
            }
            if (highlight == 3) {
                wclear(game);
                ok = 0;
                return 'q';
            }
        }
        // mutarea highlight-ului
        else {
            switch (alegere) {
                case KEY_UP:
                    highlight--;
                    if (highlight == -1) highlight = 0;
                    break;
                case KEY_DOWN:
                    highlight++;
                    if (highlight == 4) highlight = 3;
                    break;
            }
        }
        j++;
        napms(200);
    }
    return 'q';
}

// adaugarea unui nou jucator
void add_player(WINDOW *game, char name[], int nrows, int ncols) {
    int ch;
    int pos = 0;
    while (FOREVER) {
        ch = wgetch(game);

        if (ch != ERR) {
            // break daca s-a apasat enter
            if (ch == '\n' || ch == KEY_ENTER) {
                name[pos] = '\0';
                break;
            } else if (ch == KEY_BACKSPACE) {
                // stergerea caracterului daca s-a apasat backspace
                if (pos > 0) {
                    pos--;
                    name[pos] = '\0';
                    // inlocuirea caracterului de pe ecran cu un spatiu
                    mvwaddch(game, nrows / 2, ncols / 4 + pos - 5, ' ');
                    // mutarea cursorului inapoi pe pozitia corecta
                    wmove(game, nrows / 2, ncols / 4 + pos - 5);
                }
            } else if (pos < 20 && isprint(ch)) {
                // adaugarea caracterului la sir si afisarea pe ecran
                name[pos] = ch;
                pos++;
                mvwprintw(game, nrows / 2, ncols / 4 + pos - 6, "%c", ch);
            }
        }
    }
}

// desenarea unui camp
void draw_camp(WINDOW *game, int yc, int xc, int width_camp, int height_camp,
               int nr) {
    int i = -1, cop = nr;
    WINDOW *camp = newwin(height_camp, width_camp, yc, xc);

    // pentru elementul 0 alegem perechea 0 de culori
    if (cop == 0) i++;
    // aflarea perechii de culori in functie de numar
    else {
        while (cop != 1) {
            cop = cop / 2;
            i++;
            if (i == 7) {
                wattron(camp, A_BOLD);
                i = 0;
            }
        }
    }
    // colorarea box-ului si a numarului din interior
    wattron(camp, COLOR_PAIR(i));
    box(camp, 0, 0);
    if (nr == 0)
        mvwprintw(camp, height_camp / 2, (width_camp - 1) / 2, " ");
    else
        mvwprintw(camp, height_camp / 2, (width_camp - 1) / 2, "%d", nr);
    wrefresh(camp);
    wattroff(camp, COLOR_PAIR(i));
    wattroff(camp, A_BOLD);
}

// desenarea tablei de joc
void draw_board(int **matr, WINDOW *game) {
    int nrows, ncols, width_camp, height_camp;
    getmaxyx(game, nrows, ncols);
    box(game, 0, 0);
    wrefresh(game);
    width_camp = (ncols - 5) / 4;
    height_camp = (nrows - 5) / 4;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int xc = width_camp * x + (x + 1) + ncols / 2;
            int yc = height_camp * y + (y + 1);
            draw_camp(game, yc, xc, width_camp, height_camp, matr[y][x]);
        }
    }
    wrefresh(game);
}

// grafica game over
void draw_game_over(int **matr, WINDOW *game) {
    int nrows, ncols, width_camp, height_camp, i = 0, x, y, xc, yc;
    char s[9] = "GAMEOVER";

    init_pair(1, COLOR_RED, COLOR_BLACK);

    getmaxyx(game, nrows, ncols);
    box(game, 0, 0);
    wrefresh(game);
    width_camp = (ncols - 5) / 4;
    height_camp = (nrows - 5) / 4;

    // redesenarea tablei de joc
    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            xc = width_camp * x + (x + 1) + ncols / 2;
            yc = height_camp * y + (y + 1);
            WINDOW *camp = newwin(height_camp, width_camp, yc, xc);
            wattron(camp, COLOR_PAIR(1));
            box(camp, 0, 0);
            wrefresh(camp);
            wattroff(camp, COLOR_PAIR(1));

            wattron(camp, COLOR_PAIR(1));
            if (y == 0 || y == 3) {
                mvwprintw(camp, height_camp / 2, (width_camp - 1) / 2, "*");
            } else if (y == 1 || y == 2) {
                mvwprintw(camp, height_camp / 2, (width_camp - 1) / 2, "%c",
                          s[i]);
                i++;
            }
            wrefresh(camp);
            wattroff(camp, COLOR_PAIR(1));
        }
    }
    mvwprintw(game, nrows - 2, ncols / 2 - 17,
              "Press ENTER to return to the menu.");
    wrefresh(game);
}

// grafica win
void draw_win(int **matr, WINDOW *game) {
    int nrows, ncols, width_camp, height_camp, x, y, xc, yc;

    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    getmaxyx(game, nrows, ncols);
    box(game, 0, 0);
    wrefresh(game);
    width_camp = (ncols - 5) / 4;
    height_camp = (nrows - 5) / 4;

    // redesenarea tablei de joc
    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            xc = width_camp * x + (x + 1) + ncols / 2;
            yc = height_camp * y + (y + 1);

            WINDOW *camp = newwin(height_camp, width_camp, yc, xc);
            wattron(camp, COLOR_PAIR(1));
            box(camp, 0, 0);
            wrefresh(camp);
            wattroff(camp, COLOR_PAIR(1));

            wattron(camp, COLOR_PAIR(1));
            mvwprintw(camp, height_camp / 2, (width_camp - 1) / 2, "WIN");

            wrefresh(camp);
            wattroff(camp, COLOR_PAIR(1));
        }
    }
    mvwprintw(game, nrows - 2, ncols / 2 - 17,
              "Press ENTER to return to the menu.");
    wrefresh(game);
}

// afisarea timpului si datei
void print_time_and_date(WINDOW *date_time, int nrows, int *x) {
    time_t t;
    struct tm *timeinfo;

    time(&t);
    timeinfo = localtime(&t);

    // afisarea datei
    mvwprintw(date_time, 3, 4, "Date: %02d-%02d-%d", timeinfo->tm_mday,
              timeinfo->tm_mon + 1, 1900 + timeinfo->tm_year);
    // afisarea orei
    mvwprintw(date_time, 4, 4, "Time: %02d:%02d:%02d", timeinfo->tm_hour,
              timeinfo->tm_min, timeinfo->tm_sec);
    (*x)++;
    wrefresh(date_time);
}

// alegerea random a numerelor
void randomnr(int n, int nr, int *random_number) {
    int i;
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        random_number[i] = rand() % nr;
    }
}

// imbinarea elementelor de pe o linie/coloana si calcularea scorului
int unite(int v[], int *scor, int *nr) {
    int i, j, ok = 0;
    for (i = 0; i < 3; i++) {
        // daca gasim doua campuri alaturate cu aceeasi valoare
        if ((v[i] == v[i + 1]) && (v[i] != 0)) {
            for (j = i; j < 3; j++) v[j] = v[j + 1];
            v[3] = 0;
            v[i] = v[i] * 2;
            (*scor) = (*scor) + v[i];
            (*nr) = (*nr) + 1;
            if (v[i] == 2048) ok = 1;
        }
    }
    if (ok == 1)
        return 1;
    else
        return 0;
}

// deplasarea in stanga a celulelor
int left(int **matr, int *scor, int *nr, int *win) {
    int v[4], ok = 0, i, j, k;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            v[j] = 0;
        }
        k = 0;
        // trecerea elementelor nenule in ordine in vectorul v
        for (j = 0; j < 4; j++) {
            if (matr[i][j] != 0) {
                v[k] = matr[i][j];
                k++;
            }
        }
        // imbinare elemente din v
        (*win) = unite(v, scor, nr);
        // trecerea elementelor din v inapoi in matrice
        for (j = 0; j < 4; j++) {
            // verificare daca s-a facut vreo modificare
            if (v[j] != matr[i][j]) {
                ok = 1;
            }
            matr[i][j] = v[j];
        }
    }
    if (ok == 0)
        return 0;
    else
        return 1;
}

// deplasarea in sus a celulelor
int up(int **matr, int *scor, int *nr, int *win) {
    int v[4], ok = 0, i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            v[j] = 0;
        }

        k = 0;
        // trecerea elementelor nenule in ordine in vectorul v
        for (j = 0; j < 4; j++) {
            if (matr[j][i] != 0) {
                v[k] = matr[j][i];
                k++;
            }
        }
        // imbinare elemente din v
        (*win) = unite(v, scor, nr);
        // trecerea elementelor din v inapoi in matrice
        for (j = 0; j < 4; j++) {
            // verificare daca s-a facut vreo modificare
            if (v[j] != matr[j][i]) {
                ok = 1;
            }
            matr[j][i] = v[j];
        }
    }
    if (ok == 0)
        return 0;
    else
        return 1;
}

// deplasarea in jos a celulelor
int down(int **matr, int *scor, int *nr, int *win) {
    int v[4], ok = 0, i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            v[j] = 0;
        }

        k = 0;
        // trecerea elementelor nenule in ordine in vectorul v
        for (j = 3; j >= 0; j--) {
            if (matr[j][i] != 0) {
                v[k] = matr[j][i];
                k++;
            }
        }
        // imbinare elemente din v
        (*win) = unite(v, scor, nr);
        // trecerea elementelor din v inapoi in matrice
        for (j = 3; j >= 0; j--) {
            // verificare daca s-a facut vreo modificare
            if (v[3 - j] != matr[j][i]) {
                ok = 1;
            }
            matr[j][i] = v[3 - j];
        }
    }
    if (ok == 0)
        return 0;
    else
        return 1;
}

// deplasarea in dreapta a celulelor
int right(int **matr, int *scor, int *nr, int *win) {
    int v[4], ok = 0, i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            v[j] = 0;
        }

        k = 0;
        // trecerea elementelor nenule in ordine in vectorul v
        for (j = 3; j >= 0; j--) {
            if (matr[i][j] != 0) {
                v[k] = matr[i][j];
                k++;
            }
        }
        // imbinare elemente din v
        (*win) = unite(v, scor, nr);
        // trecerea elementelor din v inapoi in matrice
        for (j = 3; j >= 0; j--) {
            // verificare daca s-a facut vreo modificare
            if (v[3 - j] != matr[i][j]) {
                ok = 1;
            }
            matr[i][j] = v[3 - j];
        }
    }
    if (ok == 0)
        return 0;
    else
        return 1;
}

// verificare caz game over
int Game_over(int **matr, int *scor, int nr, WINDOW *wnd, char *c) {
    int **copie, copie_scor, i, j, k = 0, ok = 0, cop_nr, maxi = nr,
                                   cop_win = 0;
    copie = mat();
    copie_scor = *scor;

    wclear(wnd);
    box(wnd, 0, 0);
    wrefresh(wnd);
    mvwprintw(wnd, 4, 4, "VALID MOVES: ");

    // trecerea elementelor din matr in copie
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) copie[i][j] = matr[i][j];
    cop_nr = nr;
    // verificare mutare valida si afisarea ei - DOWN
    if (down(copie, &copie_scor, &cop_nr, &cop_win)) {
        ok = 1;
        k++;
        mvwprintw(wnd, 4 + k, 4, "DOWN -> S");
        if (cop_nr >= maxi) {
            maxi = cop_nr;
            (*c) = 's';
        }
    }

    // trecerea elementelor din matr in copie
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) copie[i][j] = matr[i][j];
    cop_nr = nr;
    // verificare mutare valida si afisarea ei - LEFT
    if (left(copie, &copie_scor, &cop_nr, &cop_win)) {
        ok = 1;
        k++;
        mvwprintw(wnd, 4 + k, 4, "LEFT -> A");
        if (cop_nr >= maxi) {
            maxi = cop_nr;
            (*c) = 'a';
        }
    }

    // trecerea elementelor din matr in copie
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) copie[i][j] = matr[i][j];
    cop_nr = nr;
    // verificare mutare valida si afisarea ei - RIGHT
    if (right(copie, &copie_scor, &cop_nr, &cop_win)) {
        ok = 1;
        k++;
        mvwprintw(wnd, 4 + k, 4, "RIGHT -> D");
        if (cop_nr >= maxi) {
            maxi = cop_nr;
            (*c) = 'd';
        }
    }

    // trecerea elementelor din matr in copie
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) copie[i][j] = matr[i][j];
    cop_nr = nr;
    // verificare mutare valida si afisarea ei - UP
    if (up(copie, &copie_scor, &cop_nr, &cop_win)) {
        ok = 1;
        k++;
        mvwprintw(wnd, 4 + k, 4, "UP -> W");
        if (cop_nr >= maxi) {
            maxi = cop_nr;
            (*c) = 'w';
        }
    }

    wrefresh(wnd);
    free(copie);
    return ok;
}

// jocul
void Game(int **matr, int *scor, int *scor_max, int *nr, WINDOW *game, int *win,
          WINDOW *wnd) {
    int ok = 0, *camp, *valoare, i, j, k, game_over, nrows, ncols, x = 0;
    char c, a;

    getmaxyx(wnd, nrows, ncols);
    WINDOW *date_time =
        newwin(nrows / 6 - 1, ncols / 4 - 1, 5 * nrows / 6 - 1, 1);
    nodelay(wnd, TRUE);

    game_over = Game_over(matr, scor, *nr, wnd, &a);
    // verificare caz in care avem game over
    if (game_over) {
        wrefresh(wnd);
        draw_board(matr, game);
        wrefresh(game);
    }

    while (FOREVER) {
        // afisarea scorului
        mvwprintw(wnd, 4, (ncols / 4) * 3 + 4, "HIGH SCORE: %d", *scor_max);
        mvwprintw(wnd, 5, (ncols / 4) * 3 + 4, "SCORE: %d", *scor);
        wrefresh(game);

        if (game_over) {
            print_time_and_date(date_time, nrows, &x);
            // Setare timeout la 0.1 secunde
            napms(100);

            c = wgetch(wnd);
            ok = 0;
            // verificare daca a fost introdus un caracter sau nu
            if (c == ERR) {
                // daca au  trecut 10 secunde fara sa se primeasca input
                if (x == 100) {
                    c = a;
                    x = 0;
                }
            } else
                x = 0;

            // caz quit
            if (tolower(c) == 'q') {
                break;
            }
            // verificarea tipului de deplasare
            switch (tolower(c)) {
                case 'a':
                    if (left(matr, scor, nr, win) == 1) ok = 1;
                    break;

                case 'd':
                    if (right(matr, scor, nr, win) == 1) ok = 1;
                    break;

                case 'w':
                    if (up(matr, scor, nr, win) == 1) ok = 1;
                    break;

                case 's':
                    if (down(matr, scor, nr, win) == 1) ok = 1;
                    break;
            }
            // verificam daca scorul este mai mare decat highscore-ul
            if ((*scor_max) < (*scor)) (*scor_max) = (*scor);

            // verificare caz in care player-ul castiga
            if ((*win) == 1) {
                wclear(wnd);
                box(wnd, 0, 0);
                wrefresh(wnd);
                draw_win(matr, game);
                wrefresh(game);
                // asteptare enter pentru revenirea la meniu
                c = wgetch(game);
                while (FOREVER)
                    if (c == KEY_ENTER || c == '\n')
                        break;
                    else
                        c = wgetch(game);
                break;
                wclear(game);
            }

            // adaugarea unei valori random pe o pozitie random
            if (ok) {
                camp = (int *)malloc(2 * sizeof(int));
                valoare = (int *)malloc(2 * sizeof(int));

                randomnr(1, *nr, camp);
                k = -1;
                for (i = 0; i < 4; i++)
                    for (j = 0; j < 4; j++) {
                        if (matr[i][j] == 0) {
                            k++;
                            if (camp[0] == k) {
                                randomnr(1, 2, valoare);
                                if (valoare[0] == 0)
                                    valoare[0] = 2;
                                else
                                    valoare[0] = 4;
                                matr[i][j] = valoare[0];
                                (*nr)--;
                            }
                        }
                    }
                free(camp);
                free(valoare);
                // verificare daca este game over
                game_over = Game_over(matr, scor, *nr, wnd, &a);
            }

            if (x == 0) {
                box(game, 0, 0);
                draw_board(matr, game);
            }
        }
        // daca este game over
        else {
            wclear(wnd);
            box(wnd, 0, 0);
            wrefresh(wnd);
            draw_game_over(matr, game);
            wrefresh(game);
            // asteptare enter pentru revenirea la meniu
            c = wgetch(game);
            while (FOREVER)
                if (c == KEY_ENTER || c == '\n')
                    break;
                else
                    c = wgetch(game);
            break;
            wclear(game);
            wrefresh(game);
            if (ok) {
                for (i = 0; i < 4; i++)
                    for (j = 0; j < 4; j++) matr[i][j] = 0;
            }
            break;
        }
    }
}

// meniul
void menu(int **matr) {
    int *camp1, *camp2, *valori, nr, win, ok = 0, scor[30], cop_scor[30], i, j,
                                          players = -1;
    int nrows, ncols, aux, scor_max = 0, maxplayers = 0, cop_players;
    char a, s[30][21], auxs[21], cop_s[30][21];

    WINDOW *wnd = initscr();

    // se va reține in nrows si ncols numarul maxim de linii si coloane
    getmaxyx(wnd, nrows, ncols);
    WINDOW *game = newwin(nrows, ncols / 2, 0, ncols / 4);
    // se sterge ecranul
    clear();
    // se inhiba afisarea caracterelor introduse de la tastatura
    noecho();
    // caracterele introduse sunt citite imediat - fara 'buffering'
    cbreak();
    // se ascunde cursorul
    curs_set(0);

    while (FOREVER) {
        wclear(wnd);
        box(wnd, 0, 0);
        wrefresh(wnd);
        a = draw_menu(game);
        wclear(game);
        box(game, 0, 0);
        wrefresh(game);
        if (a == 'q') {
            break;
        }
        switch (a) {
            case 'n':
                win = 0;
                players++;
                if (players == 30) {
                    players = 0;
                    maxplayers = 1;
                    cop_players = 29;
                } else if (maxplayers == 0)
                    cop_players = players;

                // determinarea celui mai mare scor
                for (i = 0; i < cop_players; i++) {
                    if (scor_max < scor[i]) {
                        scor_max = scor[i];
                    }
                }
                mvwprintw(game, nrows / 2 - 2, ncols / 4 - 6,
                          "ENTER YOUR NAME");
                add_player(game, s[players], nrows, ncols);
                wrefresh(game);
                wclear(game);
                box(game, 0, 0);
                wrefresh(game);
                camp1 = (int *)malloc(2 * sizeof(int));
                camp2 = (int *)malloc(2 * sizeof(int));
                valori = (int *)malloc(2 * sizeof(int));

                // reinitializam matricea
                if (ok) {
                    for (i = 0; i < 4; i++)
                        for (j = 0; j < 4; j++) matr[i][j] = 0;
                }
                scor[players] = 0;

                // alegem random 2 campuri si 2 valori pentru acestea
                randomnr(2, 4, camp1);
                randomnr(2, 4, camp2);
                randomnr(2, 2, valori);

                // cazul in care cele 2 campuri coincid
                while (camp1[0] == camp2[0] && camp1[1] == camp2[1]) {
                    randomnr(2, 4, camp2);
                }

                // valorile de 0 si 1 le schimbam in valori de 2 si 4
                if (valori[0] == 0)
                    valori[0] = 2;
                else
                    valori[0] = 4;

                if (valori[1] == 0)
                    valori[1] = 2;
                else
                    valori[1] = 4;

                // dam valori campurilor
                matr[camp1[0]][camp1[1]] = valori[0];
                matr[camp2[0]][camp2[1]] = valori[1];

                // numarul de campuri libere
                nr = 14;

                // inceperea jocului
                ok = 1;
                Game(matr, &scor[players], &scor_max, &nr, game, &win, wnd);

                // eliberam spatiul de stocare
                free(camp1);
                free(camp2);
                free(valori);
                break;
            case 'r':
                // continuarea jocului
                if (ok == 0) {
                    mvwprintw(game, nrows / 2 - 2, ncols / 4 - 14,
                              "You haven't started a game yet.");
                    mvwprintw(game, nrows / 2, ncols / 4 - 15,
                              "Press ENTER to return to the menu.");
                    a = wgetch(game);
                    while (FOREVER)
                        if (a == KEY_ENTER || a == '\n')
                            break;
                        else
                            a = wgetch(game);
                    break;
                } else {
                    Game(matr, &scor[players], &scor_max, &nr, game, &win, wnd);
                }
                break;
            case 't':
                // top players
                wclear(game);
                box(game, 0, 0);
                wrefresh(game);

                if (maxplayers == 0) cop_players = players;

                for (i = 0; i <= cop_players; i++) {
                    cop_scor[i] = scor[i];
                    strcpy(cop_s[i], s[i]);
                }

                for (i = 0; i < cop_players; i++) {
                    for (j = i + 1; j <= cop_players; j++) {
                        if (cop_scor[j] > cop_scor[i]) {
                            aux = cop_scor[j];
                            cop_scor[j] = cop_scor[i];
                            cop_scor[i] = aux;

                            strcpy(auxs, cop_s[j]);
                            strcpy(cop_s[j], cop_s[i]);
                            strcpy(cop_s[i], auxs);
                        }
                    }
                }

                mvwprintw(game, 4, ncols / 4 - 20, "TOP PLAYERS:");
                for (i = 0; i <= cop_players; i++) {
                    mvwprintw(game, 5 + i, ncols / 4 - 22, "%d. %s", i + 1,
                              cop_s[i]);
                    mvwprintw(game, 5 + i, ncols / 4 + 16, "SCOR: %d",
                              cop_scor[i]);
                }
                mvwprintw(game, nrows - 4, ncols / 4 - 15,
                          "Press ENTER to return to the menu.");
                a = wgetch(game);
                while (FOREVER)
                    if (a == KEY_ENTER || a == '\n')
                        break;
                    else
                        a = wgetch(game);
                break;
        }
    }
}

int main() {
    int **matr;
    matr = mat();

    menu(matr);

    // se inchide fereastra ncurses
    endwin();

    free(matr);
    return 0;
}
