//数独
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>

#define EMPTY 0
#define SOLVE 's'
#define CREATE 'c'

using namespace std;

/**
 * 次の数字を返す
 * @param fp
 * @return
 */
int getNextNumber(FILE *fp) {
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            continue;
        } else if ('1' <= c && c <= '9') {
            c -= '0';
            return c;
        } else {
            return EMPTY;
        }
    }
    return -1;
}

/**
 * 表示
 * @param array
 * @param fp
 */
void printArray(int array[9][9], FILE *fp = stdout) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (array[i][j] == EMPTY)
                fprintf(fp, "_ ");
            else
                fprintf(fp, "%d ", array[i][j]);
        }
        fputc('\n', fp);
    }
    fputc('\n', fp);
}

bool setIJ(int array[9][9], int i, int j, int n, bool row[9][9], bool col[9][9], bool block[9][9]) {
    if (row[i][n - 1] || col[j][n - 1] || block[i / 3 * 3 + j / 3][n - 1])
        return false;
    array[i][j] = n;
    row[i][n - 1] = true;
    col[j][n - 1] = true;
    block[i / 3 * 3 + j / 3][n - 1] = true;
    return true;
}

void canselIJ(int array[9][9], int i, int j, int n, bool row[9][9], bool col[9][9], bool block[9][9]) {
    array[i][j] = EMPTY;
    row[i][n - 1] = false;
    col[j][n - 1] = false;
    block[i / 3 * 3 + j / 3][n - 1] = false;
}

bool init(int array[9][9], bool row[9][9], bool col[9][9], bool block[9][9]) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (array[i][j] != EMPTY)
                if (!setIJ(array, i, j, array[i][j], row, col, block))
                    return false;
        }
    }
    return true;
}

void swap(int &a, int &b) {
    int t = a;
    a = b;
    b = t;
}

void randArray1to9(int randArray[9]) {
    for (int i = 0; i < 9; ++i)
        randArray[i] = i + 1;

    for (int i = 9 - 1; i > 0; --i)
        swap(randArray[i], randArray[rand() % (i+1)]);
}

void randArray0to80(int randArray[81]) {
    for (int i = 0; i < 81; ++i)
        randArray[i] = i;

    for (int i = 81 - 1; i > 0; --i)
        swap(randArray[i], randArray[rand() % i]);
}

bool createRow(int array[9][9], int i, int k, bool col[9][9], bool block[9][9]) {
    if (i == 9)
        return true;

    if (k == 0) {
        randArray1to9(array[i]);
    } else if (k == 9) {
        int j;
        //置けるかどうか
        for (j = 0; j < 9; ++j) {
            if (col[j][array[i][j] - 1] || block[i / 3 * 3 + j / 3][array[i][j] - 1])
                break;
        }
        //置けた時
        if (j == 9) {
            //フラグセット
            for (j = 0; j < 9; ++j) {
                col[j][array[i][j] - 1] = true;
                block[i / 3 * 3 + j / 3][array[i][j] - 1] = true;
            }
            if (createRow(array, i + 1, 0, col, block))
                return true;
            //フラグリセット
            for (j = 0; j < 9; ++j) {
                col[j][array[i][j] - 1] = false;
                block[i / 3 * 3 + j / 3][array[i][j] - 1] = false;
            }
        }
        return false;
    }
    for (int t = k; t < 9; ++t) {
        swap(array[i][t], array[i][k]);
        if (createRow(array, i, k + 1, col, block))
            return true;
        swap(array[i][t], array[i][k]);
    }
    return false;
}

void createResult(int array[9][9]) {
    bool col[9][9] = {0}, block[9][9] = {0};

    randArray1to9(array[0]);
    for (int j = 0; j < 9; ++j) {
        col[j][array[0][j] - 1] = true;
        block[j / 3][array[0][j] - 1] = true;
    }
    for (int i = 1; i < 9; ++i) {
        for (int j = 0; j < 9; ++j)
            array[i][j] = EMPTY;
    }
    createRow(array, 1, 0, col, block);
}

void copy(int a1[9][9], int a2[9][9]) {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            a1[i][j] = a2[i][j];
}

void solve(int array[9][9], int k, bool flag, bool row[9][9], bool col[9][9], bool block[9][9], int &count) {
    int i = k / 9, j = k % 9;
    if (k == 0) {
        count = 0;
        if (!init(array, row, col, block))
            return;
    } else if (k == 81) {
        ++count;
        if (!flag)
            printArray(array);
        return;
    }
    if (array[i][j] != EMPTY) {
        solve(array, k + 1, flag, row, col, block, count);
    } else {
        for (int n = 1; n <= 9; ++n) {
            if (setIJ(array, i, j, n, row, col, block)) {
                solve(array, k + 1, flag, row, col, block, count);
                if (flag && count >= 2)
                    return;
                canselIJ(array, i, j, n, row, col, block);
            }
        }
    }
}

/**
 * 解く
 * @param array
 * @param flag 唯一解であるかどうか調べる時true, 普通に解く時false (default)
 * @return 唯一解かどうか or 解けたかどうか(問題ミス以外は解ける)
 */
bool solve(int array[9][9], bool flag = false) {
    int count;
    bool row[9][9] = {0}, col[9][9] = {0}, block[9][9] = {0};

    solve(array, 0, flag, row, col, block, count);
    if (!flag && count == 0) puts("解なし");
    return ((flag) ? count == 1 : count != 0);
}

void createProbrem(int array[9][9], int ans[9][9]) {
    srand((unsigned) time(NULL));
    int randArray[81];
    createResult(array);
    copy(ans, array);
    int temp[9][9];
    randArray0to80(randArray);
    for (int k = 0; k < 81; ++k) {
        int i = randArray[k] / 9, j = randArray[k] % 9;
        copy(temp, array);
        array[i][j] = EMPTY;
        if (!solve(array, true)) {//唯一解でない時
            copy(array, temp);
        } else {
            copy(array, temp);
            array[i][j] = EMPTY;
        }
    }
}

bool fileImport(char *fileName, int array[9][9]) {
    FILE *fp;
    if ((fp = fopen(fileName, "r")) != NULL) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                array[i][j] = getNextNumber(fp);
            }
        }
        fclose(fp);
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    int array[9][9], ans[9][9];
    char type;
    char fileName[128];

    //引数あるとき
    if (argc > 1) {
        if (!fileImport(argv[1], array)) {
            puts("ファイルがありません");
            puts("End");
            getchar();
            return 0;
        }
        printArray(array);
        solve(array);
        puts("End");
        getchar();
        return 0;
    }
    do {
        puts("c : 問題作成, s : 問題を解く");
        type = getchar();
        fflush(stdin);
    } while ((type != SOLVE) & (type != CREATE));

    switch (type) {
        case SOLVE:
            puts("ファイル名入力");
            scanf("%s", fileName);
            if (!fileImport(fileName, array)) {
                puts("ファイルがありません");
                puts("End");
                getchar();
                getchar();
                return 0;
            }
            printArray(array);
            solve(array);
            break;
        case CREATE:
            createProbrem(array, ans);
            printArray(array);
            printArray(ans);
            FILE *fp = fopen("__sudoku.txt", "w");
            printArray(array, fp);
            fclose(fp);
            break;
    }
    puts("End");
    getchar();
    getchar();
    return 0;
}
