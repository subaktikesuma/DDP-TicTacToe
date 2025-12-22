#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 3

/**
 * Menginisialisasi papan dengan karaker '-'
 *
 * @param board Array papan untuk dipenuhi
 */
void initBoard(char board[]) {
    for (int i = 0; i < (BOARD_SIZE * BOARD_SIZE); i++) {
        board[i] = ' ';
    }
}

/**
 * Menaruh tanda player ke dalam papan
 *
 * @param board Papan koordinat permainan
 * @param sign Tanda player yang akan digunakan
 * @param signPos Posisi tanda yang ingin ditempatkan
 */
bool place(char board[], char sign, char signPos[]) {
    // Konvert signPos ke numeric melalui ASCII, lalu convert
    // ke 0-indexed dari 1 human-indexed
    int row = (signPos[0] - '0') - 1;
    int col = (signPos[1] - '0') - 1;

    // Hitung posisi akhir pada papan
    int pos = col + (row * BOARD_SIZE);

    if (pos >= 0 && pos < (BOARD_SIZE * BOARD_SIZE)) {
        if (board[pos] == 'x' || board[pos] == 'o')
            return false;

        board[pos] = sign;
        return true;
    }

    return false;
}

/**
 * Mengecek apakah ada 3 tanda yang sama dalam segaris
 *
 * @param board Papan koordinat yang akan dicek
 * @param sign Tanda yang akan digunakan untuk perbandingan
 */
bool check(char board[], char sign) {
    // Mengecek baris
    if (board[0] == sign && board[1] == sign && board[2] == sign)
        return true;
    if (board[3] == sign && board[4] == sign && board[5] == sign)
        return true;
    if (board[6] == sign && board[7] == sign && board[8] == sign)
        return true;

    // Mengecek kolom
    if (board[0] == sign && board[3] == sign && board[6] == sign)
        return true;
    if (board[1] == sign && board[4] == sign && board[7] == sign)
        return true;
    if (board[2] == sign && board[5] == sign && board[8] == sign)
        return true;

    // Mengecek diagonal
    if (board[0] == sign && board[4] == sign && board[8] == sign)
        return true;
    if (board[2] == sign && board[4] == sign && board[6] == sign)
        return true;

    // Jika belum ada pemenang
    return false;
}

/**
 * Mengecek apakah papan penuh untuk menyatakan setara
 *
 * @param board Papan permainan
 */
bool checkTie(char board[]) {
    for (int i = 0; i < (BOARD_SIZE * BOARD_SIZE); i++) {
        if (board[i] == ' ') {
            return false;
        }
    }

    return true;
}