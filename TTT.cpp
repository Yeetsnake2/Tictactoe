#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

char tictactoe[9] = {'1','2','3','4','5','6','7','8','9'};
int turn = 0;
int player = 0;
int winner = 2; // 1 for 'X', -1 for 'O', 0 for draw, 2 for ongoing

void random_turn() {
    srand(time(0));
    player = rand() % 2;
}

int move(int position) {
    if (tictactoe[position] == 'X' || tictactoe[position] == 'O') {
        cout << "Invalid move. Try again." << endl;
        return 1;
    }

    if (turn % 2 - player)
    {
        tictactoe[position] = 'O';
        turn++;
    }
    else
    {
        tictactoe[position] = 'X';
        turn++;
    }
    return 0;
}

void win_check() {
    for (int i = 0; i < 9; i += 3) {
        if (tictactoe[i] == tictactoe[i + 1] && tictactoe[i + 1] == tictactoe[i + 2]) {
            winner = (tictactoe[i] == 'X') ? 1 : -1;
            return;
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (tictactoe[i] == tictactoe[i + 3] && tictactoe[i + 3] == tictactoe[i + 6]) {
            winner = (tictactoe[i] == 'X') ? 1 : -1;
            return;
        }
    }
    
    if (tictactoe[0] == tictactoe[4] && tictactoe[4] == tictactoe[8]) {
        winner = (tictactoe[0] == 'X') ? 1 : -1;
        return;
    }
    if (tictactoe[2] == tictactoe[4] && tictactoe[4] == tictactoe[6]) {
        winner = (tictactoe[2] == 'X') ? 1 : -1;
        return;
    }

    if (turn == 9) {
        winner = 0;
    }
}

void declare_winner() {
    if (winner == 1) {
        cout << "X wins!" << endl;
    } else if (winner == -1) {
        cout << "O wins!" << endl;
    } else if (winner == 0) {
        cout << "It's a draw!" << endl;
    }
}   

void print_board() {
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << tictactoe[i * 3 + j] << " ";
        }

        cout << endl;
    }
    
}


int main() {
    int pos;
    while (turn < 9 && winner == 2)
    {
        print_board();
        cin >> pos;
        move(pos - 1);
        win_check();
        if (winner != 2) {
            print_board();
            declare_winner();
            break;
        }

    }
    
    return 0;
}