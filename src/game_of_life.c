#include <stdio.h>
#include <unistd.h>
#define y 25
#define x 80

int input(int board[y][x], int boardNew[y][x]);
void fileInput(int method, int board[y][x], int boardNew[y][x]);
void draw(int board[y][x]);
int count(int board[y][x], int boardNew[y][x]);

int main() {
    int first[y][x], second[y][x], q = 0, end = 0, speed = -1;
    // two matrices for copying them, q = flag for 1st(=0) or 2nd(=1)
    while ((speed < 1 || speed > 10) && end == 0) {
        printf("\e[1;1H\e[2J");  // clear the terminal
        printf("Введите скорость от 1 до 10 (чем выше, тем быстрее):");
        if (scanf("%d", &speed) != 1)
            end = 1;
    }
    if (end != 1)
        end = input(first, second);  // input sequence until -1 in i or j
    if (end == 1) printf("Ошибка в вводе, в следующий раз будь аккуратнее!\n");
    while (end != 1) {
        if (q == 0) {
            if (count(first, second) == 0)
                end = 1;
            draw(second);
            q = 1;
        } else {
            if (count(second, first) == 0)
            end = 1;
            draw(first);
            q = 0;
        }
        usleep(1500000 / speed);
    }
    return 0;
}

int input(int board[y][x], int boardNew[y][x]) {
    int method = -1, q = 0, end = 0;
    for (int i = 1; i < y; i++)  // set all board cells to zeroes
        for (int j = 1; j < x; j++) {
            board[i][j] = 0;
            boardNew[i][j] = 0;
        }
    int i = -1, j = -1;
    while (q == 0 && end == 0) {
        draw(board);  // draw after one correct input
        printf("Введите:\n1 для ввода с клавиатуры\n2 для загрузки вертолёта\n");
        printf("3 для загрузки ключей\n4 для загрузки часов\n5 для загрузки креста\n6 для загрузки квадро\n");
        if (scanf("%d", &method) != 1) end = 1;
        if (method == 1 && end == 0) {
            while (i != 0 && j != 0 && end == 0) {
                draw(board);  // draw after one correct input
                printf("Введите строку и столбец изменяемой клетки или введите 0 для прекращения ввода: ");
                if (scanf("%d", &i) != 1) end = 1;
                if (i != 0 && end == 0)
                    if (scanf("%d", &j) != 1) end = 1;
                if (i > 0 && i < y && j > 0 && j < x && end == 0) {
                    if (board[i][j] == 0) {  // inversion of desired cell
                        board[i][j] = 1;
                        boardNew[i][j] = 1;
                    } else {
                        board[i][j] = 0;
                        boardNew[i][j] = 0;
                    }
                }
            }
            q = 1;
        }
        if (method > 1 && method < 7) {
            fileInput(method, board, boardNew);
            draw(board);  // draw after input
            q = 1;
        }
    }
    return end;
}

void fileInput(int method, int board[y][x], int boardNew[y][x]) {
    int i, j;
    FILE *file;
    if (method == 2)
        file = fopen("helicopter.txt", "r");
    else if (method == 3)
        file = fopen("keys.txt", "r");
    else if (method == 4)
        file = fopen("clock.txt", "r");
    else if (method == 5)
        file = fopen("cross.txt", "r");
    else if (method == 6)
        file = fopen("quad.txt", "r");
    while (fscanf(file, "%d %d", &i, &j) == 2) {
        if (i != -1 && j != -1) {
                board[i][j] = 1;
                boardNew[i][j] = 1;
        }
    }
    fclose(file);
}

void draw(int board[y][x]) {
    printf("\e[1;1H\e[2J");  // clear the terminal
    for (int i = 0; i <= y; i++) {  // set all board cells to zeroes
        for (int j = 0; j <= x; j++)
            if (i == 0 || i == y || j == 0 || j == x) {
                    printf("· ");
            } else {
                if (board[i][j] == 0)
                    printf("  ");
                if (board[i][j] == 1) printf("o ");
            }
        printf("\n");
    }
}

int count(int board[y][x], int boardNew[y][x]) {
    int amount = 0;
    for (int i = 1; i < y; i++) {
        for (int j = 1; j < x; j++) {
            int i1 = i - 1, i2 = i + 1, j1 = j - 1, j2 = j + 1, counter;
            if (i1 < 1)
                i1 = y - 1;
            if (i2 > y - 1)
                i2 = 1;
            if (j1 < 1)
                j1 = x - 1;
            if (j2 > x - 1)
                j2 = 1;
            counter = board[i1][j1] + board[i1][j] + board[i1][j2] + board[i][j1] +
            board[i][j2] + board[i2][j1] + board[i2][j] + board[i2][j2];
            if (board[i][j] == 0) {
                if (counter == 3) {
                    boardNew[i][j] = 1;
                    amount++;
                } else {
                    boardNew[i][j] = 0;
                }
            }
            if (board[i][j] == 1) {
                if (counter != 2 && counter != 3) {
                    boardNew[i][j] = 0;
                    amount++;
                } else {
                    boardNew[i][j] = 1;
                }
            }
        }
    }
    return amount;
}
