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
#include <limits>
using namespace std;

char tictactoe[9] = {'1','2','3','4','5','6','7','8','9'};
int turn = 0;
int player = 0; // 0 for 'X' goes first, 1 for 'O' goes first
int winner = 2; // 1 for 'X', -1 for 'O', 0 for draw, 2 for ongoing
int global_depth = 10;

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

bool win_check() {
    for (int i = 0; i < 9; i += 3) {
        if (tictactoe[i] == tictactoe[i + 1] && tictactoe[i + 1] == tictactoe[i + 2]) {
            winner = (tictactoe[i] == 'X') ? 1 : -1;
            return true;
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (tictactoe[i] == tictactoe[i + 3] && tictactoe[i + 3] == tictactoe[i + 6]) {
            winner = (tictactoe[i] == 'X') ? 1 : -1;
            return true;
        }
    }
    
    if (tictactoe[0] == tictactoe[4] && tictactoe[4] == tictactoe[8]) {
        winner = (tictactoe[0] == 'X') ? 1 : -1;
        return true;
    }
    if (tictactoe[2] == tictactoe[4] && tictactoe[4] == tictactoe[6]) {
        winner = (tictactoe[2] == 'X') ? 1 : -1;
        return true;
    }

    if (turn == 9) {
        winner = 0;
        return false;
    }
    return false;
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

int minimax(int max_depth = global_depth, bool maximizing = true) {
    int best_score;
    if (max_depth == 0) return rand() % 3 - 1;
    if (win_check()) return winner * -1;
    if (turn == 9) return 0;
    if (maximizing) {
        best_score = numeric_limits<int>::min();
        int current_score;
        for (int i = 0; i < 9; ++i) {
            if (tictactoe[i] != 'X' && tictactoe[i] != 'O') {
                tictactoe[i] = 'O';
                turn++;
                current_score = minimax(max_depth - 1, false);
                tictactoe[i] = '1' + i;
                turn--;
                winner = 2;
                best_score = max(best_score, current_score);
            }
        }
    }
    else {
        best_score = numeric_limits<int>::max();
        int current_score;
        for (int i = 0; i < 9; ++i) {
            if (tictactoe[i] != 'X' && tictactoe[i] != 'O') {
                tictactoe[i] = 'X';
                turn++;
                current_score = minimax(max_depth - 1, true);
                tictactoe[i] = '1' + i;
                turn--;
                winner = 2;
                best_score = min(best_score, current_score);
            }
        }
    }
    return best_score;
}

int get_best_move(bool maximizing = true, int depth = global_depth) {
    int best_score;
    int best_move = -1;
    int current_score;
    if (depth < 0){
        best_move = rand() % 9;
        while (tictactoe[best_move] == 'X' || tictactoe[best_move] == 'O') {
            best_move = rand() % 9;
        }
        return best_move;
    }
    if (maximizing) {
        best_score = numeric_limits<int>::min();
        for (int i = 0; i < 9; ++i) {
            if (tictactoe[i] != 'X' && tictactoe[i] != 'O') {
                tictactoe[i] = 'O';
                turn++;
                current_score = minimax(depth, false);
                tictactoe[i] = '1' + i;
                turn--;
                winner = 2;
                if (current_score > best_score) {
                    best_score = current_score;
                    best_move = i;
                }
            
            }
        }
    }
    else {
        best_score = numeric_limits<int>::max();
        for (int i = 0; i < 9; ++i) {
            if (tictactoe[i] != 'X' && tictactoe[i] != 'O') {
                tictactoe[i] = 'X';
                turn++;
                current_score = minimax(depth, true);
                tictactoe[i] = '1' + i;
                turn--;
                winner = 2;
                if (current_score < best_score) {
                    best_score = current_score;
                    best_move = i;
                }
            
            }
        }
    }
    return best_move;
}

void go_single(Fl_Widget *w, void *data) {
    Fl_Group **groups = (Fl_Group**)data;
    global_depth = fl_choice("Choose Difficulty", "Easy", "Medium", "Hard") - 1;
    global_depth = global_depth ? global_depth * 10 : 1;
    turn = 0;
    winner = 2;
    random_turn();
    groups[0]->hide();
    groups[1]->show();

    if (turn % 2 - player) {
        int best_move = get_best_move();
        move(best_move);
        groups[1]->child(best_move + 1)->label("O");
        print_board();
        cout << "Best move: " << get_best_move(false) << endl;
    }

    groups[1]->child(0)->label("Player X's turn");

}

void go_two(Fl_Widget *w, void *data) {
    Fl_Group **groups = (Fl_Group**)data;
    turn = 0;
    winner = 2;
    global_depth = 10;
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

    Fl_Group *single_group = groups[1];
    for (int i = 0; i < single_group->children() - 1; ++i) {
        Fl_Widget *child = single_group->child(i);
        Fl_Button *btn = dynamic_cast<Fl_Button*>(child);
        if (btn) btn->label("\0");
    }

    Fl_Group *two_group = groups[2];
    for (int i = 0; i < two_group->children() - 1; ++i) {
        Fl_Widget *child = two_group->child(i);
        Fl_Button *btn = dynamic_cast<Fl_Button*>(child);
        if (btn) btn->label("\0");
    }
    for (int i = 0; i < 9; ++i) {
        tictactoe[i] = '1' + i;
    }

}

void cell_click(Fl_Widget *w, void *data) {
    void **info = (void**)data;
    int index = (int)(intptr_t)info[0];
    Fl_Box *msg = (Fl_Box*)info[1];
    Fl_Button *btn = (Fl_Button*)w;
    Fl_Group *two_group = (Fl_Group*)msg->parent();
    Fl_Button *back_button = (Fl_Button*)two_group->child(two_group->children() - 1);
    if (winner == 2 && move(index)) {
        if (tictactoe[index] == 'X') {
            btn->label("X");
            if (win_check())
            {
                declare_winner();
                (winner + 1) ? msg->label("Player X wins!") : msg->label("Player O wins!");
                fl_message("Game Over!");
                return;
            }
            else if (winner == 0)
            {
                declare_winner();
                msg->label("It's a draw!");
                fl_message("Game Over!");
                return;
            }
            
            msg->label("Player O's turn");
            print_board();
            cout << "Best move: " << get_best_move() << endl;
        } else {
            btn->label("O");
            if (win_check())
            {
                declare_winner();
                (winner + 1) ? msg->label("Player X wins!") : msg->label("Player O wins!");
                fl_message("Game Over!");
                return;
            }
            else if (winner == 0)
            {
                declare_winner();
                msg->label("It's a draw!");
                fl_message("Game Over!");
                return;
            }
            msg->label("Player X's turn");
            print_board();
            cout << "Best move: " << get_best_move(false) << endl;
        }
    }
    else if (winner != 2)
    {
        back_button->do_callback();
        return;
    }
    
    else
    {
        msg->label("Invalid move!");
        return;
    }
    
    
}

void cell_click2(Fl_Widget *w, void *data) {
    void **info = (void**)data;
    int index = (int)(intptr_t)info[0];
    Fl_Box *msg = (Fl_Box*)info[1];
    Fl_Button *btn = (Fl_Button*)w;
    Fl_Group *two_group = (Fl_Group*)msg->parent();
    Fl_Button *back_button = (Fl_Button*)two_group->child(two_group->children() - 1);
    if (winner == 2 && move(index)) {
        if (tictactoe[index] == 'X') {
            btn->label("X");
            if (win_check())
            {
                declare_winner();
                (winner + 1) ? msg->label("Player X wins!") : msg->label("Player O wins!");
                fl_message("Game Over!");
                return;
            }
            else if (winner == 0)
            {
                declare_winner();
                msg->label("It's a draw!");
                fl_message("Game Over!");
                return;
            }

            int best_move = get_best_move();
            move(best_move);
            two_group->child(best_move + 1)->label("O");
            if (win_check())
            {
                declare_winner();
                (winner + 1) ? msg->label("Player X wins!") : msg->label("The Computer wins!");
                fl_message("Game Over!");
                return;
            }
            else if (winner == 0)
            {
                declare_winner();
                msg->label("It's a draw!");
                fl_message("Game Over!");
                return;
            }

            msg->label("Player X's turn");

            print_board();
            cout << "Best move: " << get_best_move(false) << endl;
        }
    }
    else if (winner != 2)
    {
        back_button->do_callback();
        return;
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
        Fl_Box *single_label = new Fl_Box(120, 20, 160, 40, "\0");
        Fl_Button *cells2[9];
        int cell_size2 = 60;
        int start_x2 = 110, start_y2 = 60;
        void *void_arr2[9][2];
        for (int i = 0; i < 9; ++i) {
            int row = i / 3, col = i % 3;
            void_arr2[i][0] = (void*)(intptr_t)i;
            void_arr2[i][1] = (void*)single_label;
            cells2[i] = new Fl_Button(start_x2 + col * cell_size2, start_y2 + row * cell_size2, cell_size2, cell_size2, "");
            cells2[i]->callback(cell_click2, void_arr2[i]);
        }
        Fl_Button *back_from_single = new Fl_Button(150, 240, 100, 30, "Back");
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