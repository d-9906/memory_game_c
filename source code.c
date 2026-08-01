/* Memory Game (4x4) - Arrow-key navigation
   Turbo C compatible C code */

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>

#define SIZE 4
#define CARDS (SIZE * SIZE)

int grid[SIZE][SIZE];
int revealed[SIZE][SIZE];
int matched[SIZE][SIZE];
int cursorRow = 0;
int cursorCol = 0;

void drawCard(int row, int col, int show);
void drawBoard(void);
int gameWon(void);

/* draw one card at row,col; show=1 forces face-up */
void drawCard(int row, int col, int show)
{
    int x, y, value;
    char str[4];

    x = 100 + col * 70;
    y = 100 + row * 70;
    value = grid[row][col];

    setcolor(WHITE);
    rectangle(x, y, x + 60, y + 60);

    if (show || revealed[row][col] || matched[row][col]) {
        sprintf(str, "%d", value);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(x + 20, y + 20, str);
    } else {
        setfillstyle(SOLID_FILL, LIGHTBLUE);
        bar(x + 1, y + 1, x + 59, y + 59);
    }

    /* highlight the cursor */
    if (row == cursorRow && col == cursorCol) {
        setcolor(YELLOW);
        rectangle(x - 2, y - 2, x + 62, y + 62);
    }
}

/* draw whole board */
void drawBoard(void)
{
    int i, j;
    cleardevice();
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            drawCard(i, j, 0);
        }
    }
}

/* check if all matched */
int gameWon(void)
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (!matched[i][j]) return 0;
        }
    }
    return 1;
}

int main(void)
{
    int gd, gm;
    int i, j, k, tmp;
    int values[CARDS];
    int firstRow, firstCol;
    int ch;

    gd = DETECT;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI"); /* change path if needed */

    srand((unsigned)time(NULL));

    /* prepare pairs (1..CARDS/2) */
    for (i = 0; i < CARDS / 2; i++) {
        values[2 * i] = i + 1;
        values[2 * i + 1] = i + 1;
    }

    /* shuffle */
    for (i = 0; i < CARDS; i++) {
        j = rand() % CARDS;
        tmp = values[i];
        values[i] = values[j];
        values[j] = tmp;
    }

    /* fill grid and reset flags */
    k = 0;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            grid[i][j] = values[k++];
            revealed[i][j] = 0;
            matched[i][j] = 0;
        }
    }

    firstRow = -1;
    firstCol = -1;

    /* brief on-screen instructions (will be cleared by drawBoard) */
    cleardevice();
setcolor(WHITE);
settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
outtextxy(50, 100, "MEMORY GAME (4x4)");
outtextxy(50, 150, "Use ARROW KEYS to choose a card.");
outtextxy(50, 180, "Press ENTER or SPACE to flip a card.");
outtextxy(50, 210, "Match all pairs to win!");
outtextxy(50, 250, "Press ESC anytime to quit.");
outtextxy(50, 300, "Press any key to start...");
getch();   // waits for player

    /* main loop */
    while (1) {
	drawBoard();

	if (gameWon()) {
	    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	    outtextxy(150, 30, "ALL PAIRS MATCHED!");
	    outtextxy(150, 50, "Press any key to exit");
	    getch();
	    break;
        }

        ch = getch();
        if (ch == 0) {
            ch = getch(); /* extended key: arrow keys come as 0 then code */
        }

        if (ch == 27) { /* ESC to quit */
            break;
        }

        switch (ch) {
            case 72: /* up */
                if (cursorRow > 0) cursorRow--;
                break;
            case 80: /* down */
                if (cursorRow < SIZE - 1) cursorRow++;
                break;
            case 75: /* left */
                if (cursorCol > 0) cursorCol--;
                break;
            case 77: /* right */
                if (cursorCol < SIZE - 1) cursorCol++;
                break;
            case 13: /* Enter */
            case 32: /* Space */
                if (!matched[cursorRow][cursorCol] && !revealed[cursorRow][cursorCol]) {
                    revealed[cursorRow][cursorCol] = 1;
                    drawBoard();
                    if (firstRow == -1) {
                        /* first selection */
                        firstRow = cursorRow;
                        firstCol = cursorCol;
                    } else {
                        /* second selection — show briefly, then check */
                        delay(800);
                        if (grid[firstRow][firstCol] == grid[cursorRow][cursorCol]) {
                            matched[firstRow][firstCol] = 1;
                            matched[cursorRow][cursorCol] = 1;
                        } else {
                            revealed[firstRow][firstCol] = 0;
                            revealed[cursorRow][cursorCol] = 0;
                        }
                        firstRow = -1;
                        firstCol = -1;
                    }
                }
                break;
            default:
                break;
        }
    }

    closegraph();
    return 0;
}


