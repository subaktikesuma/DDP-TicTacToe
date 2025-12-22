#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "utils.h"
#include "bot.h"

void printLogo();
void gameMenu();
void helpMenu();
void secretMenu();
void drawBoard(char board[]);

/**
 * Main menu for the game
 */
void mainMenu() {
    bool running = true;

    while (running) {
        clear_console();
        printLogo();
        printf("\n[1] Play Game\n");
        printf("[2] Help\n");
        printf("[3] Quit\n\n");

        char *choice = input(">> ", 10);

        if (choice == NULL) {
            printf("Error reading input!\n");
            continue;
        }

        int menu = atoi(choice);
        free(choice);

        switch (menu) {
        case 1:
            gameMenu();
            break;
        case 2:
            helpMenu();
            break;
        case 3:
            running = false;
            break;
        case 1998:
            secretMenu();
            break;
        default:
            printf("Menu code %d doesn't exist!", menu);
            char *temp = input("\nPress Enter to continue...", 2);
            free(temp);
            break;
        }
    }
}

/**
 * Game menu for the game
 */
void gameMenu() {
    clear_console();

    char board[BOARD_SIZE * BOARD_SIZE];
    char sign = 'x';

    bool ai = false;
    Difficulty difficulty = EASY;

    int points[2] = {0, 0};

    char *player1 = input("Masukkan nama Player 1 (X): ", 64);
    player1[strcspn(player1, "\n")] = '\0';

    char *player2 = input("Masukkan nama Player 2 (O): ", 64);
    player2[strcspn(player2, "\n")] = '\0';

    clear_console();

    char temp = 'n';

    printf("Apakah ingin bermain dengan Komputer (O)? (y/n): ");
    scanf(" %c", &temp);
    getchar();

    if (temp == 'y' || temp == 'Y') {
        ai = true;
        
        int diff;
        printf("\nPilih tingkat kesulitan:\n");
        printf("1. Mudah (Easy)\n");
        printf("2. Sedang (Medium)\n");
        printf("3. Sulit (Hard - Tidak Terkalahkan!)\n");
        printf("\nPilihan (1-3)? ");
        scanf("%d", &diff);
        getchar();
        
        switch (diff) {
            case 1:
                difficulty = EASY;
                printf("\n\033[1;32mMode: MUDAH - AI bermain acak\033[0m\n");
                break;
            case 2:
                difficulty = MEDIUM;
                printf("\n\033[1;33mMode: SEDANG - AI akan memblokir dan menyerang\033[0m\n");
                break;
            case 3:
                difficulty = HARD;
                printf("\n\033[1;31mMode: SULIT - AI tidak bisa dikalahkan!\033[0m\n");
                break;
            default:
                difficulty = MEDIUM;
                printf("\n\033[1;33mMode: SEDANG (default)\033[0m\n");
        }
        
        printf("\nPress Enter to start...");
        getchar();
    }

    bool loopRun = true;
    char repeat;

    while (loopRun) {
        initBoard(board);
        sign = 'x';

        while (true) {
            clear_console();

            printf("Pilih antara 1 dan 3, contoh: 11, 12, 13, 21, 22, 23, 31, "
                   "32, 33\n\n");

            drawBoard(board);

            if (sign == 'x') {
                printf("Skor saat ini: \033[1;32m> (X) %s = \033[0;37m%d | "
                       "\033[1;31m(O) %s = \033[0;37m%d\n",
                       player1, points[0], player2, points[1]);

            } else if (sign == 'o') {
                printf("Skor saat ini: \033[1;32m(X) %s = \033[0;37m%d | "
                       "\033[1;31m> (O) %s = \033[0;37m%d\n",
                       player1, points[0], player2, points[1]);

            } else {
                printf("Skor saat ini: \033[1;32m(X) %s = \033[0;37m%d | "
                       "\033[1;31m(O) %s = \033[0;37m%d\n",
                       player1, points[0], player2, points[1]);
            }

            char *pos = input("\nMasukkan posisi kamu (xy, contoh: 11): ", 64);

            if (place(board, sign, pos)) {
                if (check(board, sign)) {
                    clear_console();
                    drawBoard(board);
                    printf("-----------------------------\n");

                    if (sign == 'x') {
                        printf("%s (X) menang!\n", player1);
                        points[0]++;
                    } else {
                        printf("%s (O) menang!\n", player2);
                        points[1]++;
                    }

                    printf("-----------------------------\n");
                    free(pos);
                    break;
                }

                if (checkTie(board)) {
                    clear_console();
                    drawBoard(board);
                    printf("-----------------------------\n");
                    printf("Setara!\n");
                    printf("-----------------------------\n");
                    free(pos);
                    break;
                }

                if (ai && sign == 'x') {
                    char aiPos[3];
                    makeAIMove(board, aiPos, difficulty);
                    place(board, 'o', aiPos);

                    if (check(board, 'o')) {
                        clear_console();
                        drawBoard(board);
                        printf("-----------------------------\n");
                        printf("%s (O) menang!\n", player2);
                        points[1]++;
                        printf("-----------------------------\n");
                        free(pos);
                        break;
                    }

                    if (checkTie(board)) {
                        clear_console();
                        drawBoard(board);
                        printf("-----------------------------\n");
                        printf("Setara!\n");
                        printf("-----------------------------\n");
                        free(pos);
                        break;
                    }
                } else {
                    sign = (sign == 'x') ? 'o' : 'x';
                }

            } else {
                printf("Posisi tidak valid atau sudah terisi, coba lagi!\n");
                system("pause");
            }

            free(pos);
        }

        printf("\nIngin bermain lagi? (y/n): ");
        scanf(" %c", &repeat);
        getchar(); // Clear the newline left by scanf

        if (repeat != 'y' && repeat != 'Y') {
            clear_console();

            loopRun = false;
            printf("-----------------------------\n");
            printf("Skor akhir");
            printf("\n-----------------------------\n\n");
            printf("(X) %s : %d poin\n", player1, points[0]);
            printf("(O) %s : %d poin\n", player2, points[1]);

            printf("\n-----------------------------\n");

            if (points[0] > points[1]) {
                printf("Pemenang keseluruhan: %s!\n", player1);
            } else if (points[1] > points[0]) {
                printf("Pemenang keseluruhan: %s!\n", player2);
            } else {
                printf("\033[1;32mPermainan berakhir seri!\033[0m\n");
            }
            printf("-----------------------------\n");

            char *temp = input("\nPress Enter to continue...", 2);
            free(temp);
        }
    }

    free(player1);
    free(player2);
}

/**
 * Draw the board
 */
void drawBoard(char board[]) {
    printf("=================\n");
    printf("| # | 1 | 2 | 3 |\n");
    printf("-----------------\n");

    // Baris
    for (int i = 0, n = 0; i < BOARD_SIZE; i++) {
        // Kolom
        printf("| %d |", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c |", board[n]);
            n++;
        }
        printf("\n");
    }
    printf("=================\n");
}

/**
 * Help menu for the game
 */
void helpMenu() {
    clear_console();
    printf("===============================================================\n");
    printf("                     BANTUAN PERMAINAN                         \n");
    printf(
        "===============================================================\n\n");

    printf("CARA BERMAIN\n");
    printf("---------------------------------------------------------------\n");
    printf("1. Tic-Tac-Toe dimainkan oleh 2 pemain secara bergantian\n");
    printf("2. Player 1 menggunakan simbol X, Player 2 menggunakan simbol O\n");
    printf(
        "3. Tujuan: Susun 3 simbol yang sama secara horizontal, vertikal,\n");
    printf("   atau diagonal untuk memenangkan permainan\n\n");

    printf("MEMASUKKAN POSISI\n");
    printf("---------------------------------------------------------------\n");
    printf("Masukkan 2 angka untuk menentukan posisi:\n");
    printf("  * Angka pertama = baris (1-3)\n");
    printf("  * Angka kedua = kolom (1-3)\n\n");
    printf("Contoh:\n");
    printf("  11 = Baris 1, Kolom 1 (pojok kiri atas)\n");
    printf("  22 = Baris 2, Kolom 2 (tengah)\n");
    printf("  33 = Baris 3, Kolom 3 (pojok kanan bawah)\n\n");

    printf("SISTEM POIN\n");
    printf("---------------------------------------------------------------\n");
    printf("  * Setiap kemenangan mendapat 1 poin\n");
    printf("  * Permainan seri tidak menambah poin\n");
    printf("  * Pemain dengan poin terbanyak menjadi pemenang keseluruhan\n\n");

    printf("===============================================================\n");
    char *temp = input("\nTekan Enter untuk kembali...", 2);
    free(temp);
}

void printLogo() {
    printf("\033[0;32m$$$$$$$$\\ $$\\        $$$$$$$$\\               $$$$$$$$\\ "
           "        "
           "         \n");
    printf(
        "\\__$$  __|\\_|        \\__$$  __|              \\__$$  __|         "
        "        \n");
    printf("   $$ |   $$\\  $$$$$$$\\ $$ | $$$$$$\\   $$$$$$$\\ $$ | $$$$$$\\  "
           " $$$$$$\\  \n");
    printf("   $$ |   $$ |$$  _____|$$ | \\____$$\\ $$  _____|$$ |$$  __$$\\ "
           "$$  __$$\\ \n");
    printf("   $$ |   $$ |$$ /      $$ | $$$$$$$ |$$ /      $$ |$$ /  $$ "
           "|$$$$$$$$ |\n");
    printf("   $$ |   $$ |$$ |      $$ |$$  __$$ |$$ |      $$ |$$ |  $$ |$$   "
           "____|\n");
    printf("   $$ |   $$ |\\$$$$$$$\\ $$ |\\$$$$$$$ |\\$$$$$$$\\ $$ |\\$$$$$$  "
           "|\\$$$$$$$\\ \n");
    printf("   \\__|   \\__| \\_______|\\_| \\_______| \\_______|\\__| "
           "\\______/  \\_______|\033[0;37m\n");
}

void secretMenu() {
    clear_console();
    slow_print("WELLCOME TO THE SECRET MENU\n", 100);
    getchar();
}