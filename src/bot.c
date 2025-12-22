#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "core.h"
#include "utils.h"

typedef enum { EASY, MEDIUM, HARD } Difficulty;

/**
 * Memeriksa jika sebuah posisi kosong
 *
 * @param board[] Array papan permainan
 * @param row Koordinat baris yang akan dicek
 * @param col Koordinat kolumn yang akan dicek
 */
bool isEmpty(char board[], int row, int col) {
    int idx = (row - 1) * 3 + (col - 1);
    return board[idx] != 'x' && board[idx] != 'o';
}

/**
 * Merubah index menjadi posisi berupa string
 *
 * @param idx index posisi pada board
 * @param pos pointer ke string yang akan di return
 */
void indexToPos(int idx, char *pos) {
    int row = idx / 3 + 1;
    int col = idx % 3 + 1;
    sprintf(pos, "%d%d", row, col);
}

/**
 * Mendapatkan index ke array Papan dari posisi
 * 
 * @param row Baris posisi
 * @param col Kolumn posisi
 */
int getBoardIndex(int row, int col) { return (row - 1) * 3 + (col - 1); }

/**
 * Memeriksa apakah ada pemenang di papan
 * 
 * @param board Papan permainan
 * @return 'o' jika AI menang, 'x' jika pemain menang, '\0' jika tidak ada pemenang
 */
char checkWinner(char board[]) {
    // Kombinasi posisi menang
    int winPos[][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Baris
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Kolom
        {0, 4, 8}, {2, 4, 6}             // Diagonal
    };
    
    for (int i = 0; i < 8; i++) {
        if (board[winPos[i][0]] == board[winPos[i][1]] && 
            board[winPos[i][1]] == board[winPos[i][2]]) {
            if (board[winPos[i][0]] == 'x' || board[winPos[i][0]] == 'o') {
                return board[winPos[i][0]];
            }
        }
    }
    return '\0';
}

/**
 * Memeriksa apakah papan sudah penuh
 * 
 * @param board Papan permainan
 * @return true jika papan penuh, false jika masih ada ruang kosong
 */
bool isBoardFull(char board[]) {
    for (int i = 0; i < 9; i++) {
        if (board[i] != 'x' && board[i] != 'o') {
            return false;
        }
    }
    return true;
}

/**
 * Algoritma minimax untuk menentukan langkah terbaik
 * 
 * @param board Papan permainan
 * @param isMaximizing true jika giliran AI (maksimalisasi), false jika giliran pemain (minimisasi)
 * @return Skor dari posisi papan saat ini
 */
int minimax(char board[], bool isMaximizing) {
    char winner = checkWinner(board);
    
    // Kondisi terminal
    if (winner == 'o') return 10;  // AI menang
    if (winner == 'x') return -10; // Pemain menang
    if (isBoardFull(board)) return 0; // Seri
    
    if (isMaximizing) {
        // Giliran AI - maksimalkan skor
        int maxScore = INT_MIN;
        
        for (int i = 0; i < 9; i++) {
            if (board[i] != 'x' && board[i] != 'o') {
                char original = board[i];
                board[i] = 'o';
                
                int score = minimax(board, false);
                maxScore = (score > maxScore) ? score : maxScore;
                
                board[i] = original; // Undo move
            }
        }
        return maxScore;
    } else {
        // Giliran pemain - minimalkan skor
        int minScore = INT_MAX;
        
        for (int i = 0; i < 9; i++) {
            if (board[i] != 'x' && board[i] != 'o') {
                char original = board[i];
                board[i] = 'x';
                
                int score = minimax(board, true);
                minScore = (score < minScore) ? score : minScore;
                
                board[i] = original; // Undo move
            }
        }
        return minScore;
    }
}

/**
 * Algoritma random untuk memilih posisi (dipakai untuk mode Easy pada AI)
 *
 * @param board Papan permainan
 * @param aiPos Pointer posisi ai yang akan di return
 */
void findEasyMove(char board[], char *aiPos) {
    bool validMove = false;
    while (!validMove) {
        sprintf(aiPos, "%d%d", randint(3) + 1, randint(3) + 1);
        int row = aiPos[0] - '0';
        int col = aiPos[1] - '0';
        validMove = isEmpty(board, row, col);
    }
}

/**
 * Algoritma yang berusaha untuk mengalahkan dan menyusahkan 
 * pemain (dipakai untuk mode Medium)
 * 
 * @param board Papan permainan
 * @param aiPos Pointer posisi ai yang akan di return
 * @param rSeed Random seed untuk error rate algo
 */
void findMediumMove(char board[], char *aiPos, int rSeed) {
    if (rSeed != NULL && rSeed > 100) rSeed = 100;
    
    if (rSeed != NULL) {
        int dSeed = randint(100);

        if (dSeed > rSeed) {
            findEasyMove(board, aiPos);
            return;
        }   
    }
    
    // Periksa jika ada posisi musuh yang bisa diblokir
    int winningEnemyPos[][3][2] = {
        {{1, 1}, {1, 2}, {1, 3}}, {{2, 1}, {2, 2}, {2, 3}},
        {{3, 1}, {3, 2}, {3, 3}}, {{1, 1}, {2, 1}, {3, 1}},
        {{1, 2}, {2, 2}, {3, 2}}, {{1, 3}, {2, 3}, {3, 3}},
        {{1, 1}, {2, 2}, {3, 3}}, {{1, 3}, {2, 2}, {3, 1}},
    };

    int xAmount = 0;
    int oAmount = 0;
    int emptyRow = 0;
    int emptyCol = 0;

    int tempIdx = 0;

    for (int i = 0; i < 8; i++) {
        xAmount = 0;
        oAmount = 0;
        emptyRow = -1;
        emptyCol = -1;

        for (int j = 0; j < 3; j++) {
            tempIdx = getBoardIndex(winningEnemyPos[i][j][0],
                                    winningEnemyPos[i][j][1]);

            if (board[tempIdx] == 'x') {
                xAmount++;
            } else if (board[tempIdx] == 'o') {
                oAmount++;
            } else {
                emptyRow = winningEnemyPos[i][j][0];
                emptyCol = winningEnemyPos[i][j][1];
            }
        }

        if (xAmount == 2 && oAmount <= 0 && emptyRow != -1) {
            indexToPos(getBoardIndex(emptyRow, emptyCol), aiPos);
            return;
        }
    }

    // Ambil posisi yang strategis
    int singlePos[][2] = {{2, 2}, {1, 1}, {1, 3}, {3, 1}, {3, 3}};

    for (int i = 0; i < 5; i++) {
        if (isEmpty(board, singlePos[i][0], singlePos[i][1])) {
            sprintf(aiPos, "%d%d", singlePos[i][0], singlePos[i][1]);
            return;
        }
    }

    findEasyMove(board, aiPos);
}

/**
 * Algoritma yang berusaha mengalahkan lawan dengan prediksi
 * menggunakan algoritma minimax (digunakan untuk mode HARD)
 * 
 * @param board Papan permainan
 * @param aiPos Pointer posisi ai yang akan di return
 */
void findHardMove(char board[], char *aiPos) {
    int bestScore = INT_MIN;
    int bestMove = -1;
    
    // Evaluasi semua langkah yang mungkin
    for (int i = 0; i < 9; i++) {
        if (board[i] != 'x' && board[i] != 'o') {
            char original = board[i];
            board[i] = 'o';
            
            int score = minimax(board, false);
            
            board[i] = original; // Undo move
            
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    
    if (bestMove != -1) {
        indexToPos(bestMove, aiPos);
    } else {
        findEasyMove(board, aiPos);
    }
}

/**
 * Dapatkan posisi dari komputer (AI) berdasarkan tingkatan kesusahan
 *
 * @param board Papan 
 * @param Pointer posisi ai yang akan permainann di return
 * @param difficulty Tingkat kesusahan AI
 */
void makeAIMove(char board[], char *aiPos, Difficulty difficulty) {
    switch (difficulty) {
    case EASY:
        findEasyMove(board, aiPos);
        break;
    case MEDIUM:
        findMediumMove(board, aiPos, 75);
        break;
    case HARD:
        findHardMove(board, aiPos);
        break;
    }
}