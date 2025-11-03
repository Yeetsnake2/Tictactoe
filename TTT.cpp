#include <iostream>
#include <cstdlib>
#include <ctime>
#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_Box.h>
#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>
#include <cstdint>
using namespace std;

char tictactoe[9] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0'};
int turn = 0;
int player = 0; // 0 for 'X' goes first, 1 for 'O' goes first
int winner = 2; // 1 for 'X', -1 for 'O', 0 for draw, 2 for ongoing

void random_turn() {
    srand(time(0));
    player = rand() % 2;
}

int move(int position) {
    if (tictactoe[position] == 'X' || tictactoe[position] == 'O') {
        cout << "Invalid move. Try again." << endl;
        return 0;
    }

    if (turn % 2 - player)
    {
        tictactoe[position] = 'O';
        turn++;
        return -1;
    }
    else
    {
        tictactoe[position] = 'X';
        turn++;
        return 1;
    }
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


void go_single(Fl_Widget *w, void *data) {
    Fl_Group **groups = (Fl_Group**)data;
    turn = 0;
    random_turn();
    groups[0]->hide();
    groups[1]->show();
}

void go_two(Fl_Widget *w, void *data) {
    Fl_Group **groups = (Fl_Group**)data;
    turn = 0;
    random_turn();
    groups[0]->hide();
    groups[2]->show();

    (turn % 2 - player) ? groups[2]->child(0)->label("Player O's turn") : groups[2]->child(0)->label("Player X's turn");

}

void go_home(Fl_Widget *w, void *data) {
    Fl_Group **groups = (Fl_Group**)data;
    groups[1]->hide();
    groups[2]->hide();
    groups[0]->show();

    Fl_Group *two_group = groups[2];
    for (int i = 0; i < two_group->children() - 1; ++i) {
        Fl_Widget *child = two_group->child(i);
        Fl_Button *btn = dynamic_cast<Fl_Button*>(child);
        if (btn) btn->label("\0");
    }
    for (int i = 0; i < 9; ++i) {
        tictactoe[i] = '\0';
    }

}

void cell_click(Fl_Widget *w, void *data) {
    void **info = (void**)data;
    int index = (int)(intptr_t)info[0];
    Fl_Box *msg = (Fl_Box*)info[1];
    Fl_Button *btn = (Fl_Button*)w;
    if (move(index)) {
        if (tictactoe[index] == 'X') {
            btn->label("X");
            msg->label("Player O's turn");
            print_board();
        } else {
            btn->label("O");
            msg->label("Player X's turn");
            print_board();
        }
    }
    else
    {
        msg->label("Invalid move!");
        return;
    }
    
    
}

int main() {
    int window_length = 400;
    int window_height = 300;
    Fl_Window *main_win = new Fl_Window(400, 300, "Tic Tac Toe");

    Fl_Group *home_group = new Fl_Group(0, 0, 400, 300);
        Fl_Box *title = new Fl_Box(120, 40, 160, 40, "Tic Tac Toe");
        Fl_Button *single_btn = new Fl_Button(140, 100, 120, 30, "Single Player");
        Fl_Button *two_btn    = new Fl_Button(140, 140, 120, 30, "Two Player");
        Fl_Button *quit_btn   = new Fl_Button(140, 180, 120, 30, "Quit");
    home_group->end();

    Fl_Group *single_group = new Fl_Group(0, 0, 400, 300);
        Fl_Box *single_label = new Fl_Box(120, 100, 160, 40, "Single Player Mode");
        Fl_Button *back_from_single = new Fl_Button(150, 180, 100, 30, "Back");
    single_group->end();
    single_group->hide();

    Fl_Group *two_group = new Fl_Group(0, 0, 400, 300);
        Fl_Box *two_label = new Fl_Box(120, 20, 160, 40, "\0");
        Fl_Button *cells[9];
        int cell_size = 60;
        int start_x = 110, start_y = 60;
        void *void_arr[9][2];
        for (int i = 0; i < 9; ++i) {
            int row = i / 3, col = i % 3;
            void_arr[i][0] = (void*)(intptr_t)i;
            void_arr[i][1] = (void*)two_label;
            cells[i] = new Fl_Button(start_x + col * cell_size, start_y + row * cell_size, cell_size, cell_size, "");
            cells[i]->callback(cell_click, void_arr[i]);
        }
        Fl_Button *back_from_two = new Fl_Button(150, 240, 100, 30, "Back");
    two_group->end();
    two_group->hide();

    Fl_Group *groups[3] = {home_group, single_group, two_group};

    single_btn->callback(go_single, groups);
    two_btn->callback(go_two, groups);
    back_from_single->callback(go_home, groups);
    back_from_two->callback(go_home, groups);
    quit_btn->callback([](Fl_Widget*, void*) { exit(0); });

    main_win->end();
    main_win->show();

    return Fl::run();
}