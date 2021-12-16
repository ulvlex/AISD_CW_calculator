#pragma once
#include <string>
#include <math.h>
#include "Stack.h"

using namespace std;

const string delimiters = " ()+/*-^"; //separators
// {"cos", "sin", "tg", "ctg", "ln", "log", "sqrt", "arctg"} added more arctangent
int num_brack = 0;
int cnt = 0;
int cnt_res = 0;

//token types: variable - var, action - op, function - func, function argument - func_arg,
//brackets - brak, constants - constant, unary minus - opposite

// Структура токена
class token {
private:
    class node //doubly linked list
    {
    public:
        string info;
        string type;
        node* next; //pointer to the next element
        node* prev; //pointer to the previous element

    };
    node* head; //pointer to the first element
    node* tail; //pointer to the last element

    class node_res //doubly linked list
    {
    public:
        string info_res;
        string type_res;
        node_res* next_res; //pointer to the next element
        node_res* prev_res; //pointer to the previous element

    };
    node_res* head_res; //pointer to the first element
    node_res* tail_res; //pointer to the last element

     //checking for a separator
    bool isDelimiter(char sym) {
        bool check = false;
        for (int i = 0; i < delimiters.size(); i++) {
            if (sym == delimiters[i]) check = true;
        }
        return check;
    }

    int get_prior(string info, string type) { //priority
        if (type == "opposite" || info == "^") return 3;
        if (info == "*" || info == "/") return 2;
        if (info == "+" || info == "-") return 1;
        return 0;
    }

    void add(string info, string type) { //adding an item to the list
        node* cur = new node;
        cur->info = info;
        cur->type = type;
        if (head == NULL) {
            tail = head = cur;
            cur->next = NULL;
            cnt++;
        }
        else
        {
            cur->prev = tail;
            tail->next = cur;
            cur->next = NULL;
            tail = cur;
            cnt++;
        }
    }

    void add_res(string info, string type) { //adding an item to the list
        node_res* cur = new node_res;
        cur->info_res = info;
        cur->type_res = type;
        cur->next_res = NULL;
        cur->prev_res = NULL;
        if (head_res == NULL) {
            tail_res = head_res = cur;
            cur->next_res = NULL;
            cnt_res++;
        }
        else
        {
            cur->next_res = head_res;
            head_res->prev_res = cur;
            head_res = cur;
            cnt_res++;
        }
    }

    void for_func(node_res* cur) {
        for (int i = 0; i < 3; i++) {
            node_res* del = cur->next_res;
            (del->next_res)->prev_res = del->prev_res;
            (del->prev_res)->next_res = del->next_res;
            delete del;
        }
    }

public:

    token() { //constructor
        head = NULL;
        head_res = NULL;
        tail = NULL;
        tail_res = NULL;
        cnt = 0;
        cnt_res = 0;
        num_brack = 0;
    }

    ~token() { //destructor
        clear();
    }

    void clear() {
        node* del = new node;
        while (cnt) {
            if (head != NULL) {
                if (head == tail) {
                    head = tail = NULL;
                    cnt--;
                }
                else {
                    del = head;
                    head = head->next;
                    delete del;
                    head->prev = NULL;
                    cnt--;
                }
            }
        }

        node_res* del_res = new node_res;
        while (cnt_res) {
            if (head_res != NULL) {
                if (head_res == tail_res) {
                    head_res = tail_res = NULL;
                    cnt_res--;
                }
                else {
                    del_res = head_res;
                    head_res = head_res->next_res;
                    delete del_res;
                    head_res->prev_res = NULL;
                    cnt_res--;
                }
            }
        }
    }

    //splitting into expression tokens
    bool create_tokens(string& expr) {
        int i = 0;
        bool check = true;
        string info;
        string type;
        while (i < expr.size()) {
            if (isDelimiter(expr[i]) == true) //if it is a separator
            {
                if (expr[i] == ' ') {//jump over
                    i++;
                    continue;
                }
                else { //we write down other separators
                    if (expr[i] == '(' || expr[i] == ')') { //brackets
                        type = "brak";
                        info = expr[i];
                        add(info, type);
                        i++;
                        num_brack++;
                    }
                    else {
                        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '/' || expr[i] == '*' || (i < expr.size() - 4 && expr[i] == 's' && expr[i + 1] == 'q' && expr[i + 2] == 'r' && expr[i + 3] == 't')) { //operation
                            if (expr[i] == '-' && (i == 0 || expr[i - 1] == '(')) { //unary minus
                                type = "opposite";
                                info = expr[i];
                                add(info, type);
                                i++;
                            }
                            else {
                                if (i < expr.size() - 4 && expr[i] == 's' && expr[i + 1] == 'q' && expr[i + 2] == 'r' && expr[i + 3] == 't') {
                                    type = "op";
                                    info = "sqrt";
                                    add(info, type);
                                    i+=4;
                                }
                                else {
                                    type = "op";
                                    info = expr[i];
                                    add(info, type);
                                    i++;
                                }
                            }
                        }
                        else {
                            if (expr[i] == '^') {
                                type = "func";
                                info = "^";
                                add(info, type);
                                i++;
                            }
                        }
                    }
                }
            }
            else {
                if (expr[i] >= 'a' && expr[i] <= 'z') //function
                {
                    string buf_func;
                    while (expr[i] >= 'a' && expr[i] <= 'z') {
                        buf_func += expr[i];
                        i++;
                    }
                    if (buf_func == "cos" || buf_func == "sin" || buf_func == "tg" || buf_func == "ctg" ||
                        buf_func == "ln" || buf_func == "log" || buf_func == "sqrt" || buf_func == "arctg")
                    {
                        type = "func";
                        info = buf_func;
                        add(info, type);
                    }
                    else {
                        cout << "Error in " << i - 1 << endl;
                        check = false;
                    }
                    buf_func.clear();
                }
                else {
                    if (expr[i] >= '0' && expr[i] <= '9') { //if the number
                        string buf_func_arg;
                        if (i > 1 && expr[i - 1] == '(' && expr[i - 2] > 'a' && expr[i] <= 'z') //function argument
                        {
                            while (expr[i] >= '0' && expr[i] <= '9') {
                                buf_func_arg += expr[i];
                                i++;
                            }
                            if (expr[i] == ',' || expr[i] == '.') {
                                i++;
                                buf_func_arg += '.';
                                while (expr[i] >= '0' && expr[i] <= '9') {
                                    buf_func_arg += expr[i];
                                    i++;
                                }
                            }
                            add(buf_func_arg, "func_arg");
                        }
                        else {
                            while (expr[i] >= '0' && expr[i] <= '9') { //just a variable 
                                buf_func_arg += expr[i];
                                i++;
                            }
                            if (expr[i] == ',' || expr[i] == '.') {
                                buf_func_arg += '.';
                                i++;
                                while (expr[i] >= '0' && expr[i] <= '9') {
                                    buf_func_arg += expr[i];
                                    i++;
                                }
                            }
                            add(buf_func_arg, "var");
                        }
                        buf_func_arg.clear();
                    }
                    else {
                        if ((expr[i] == 'e') || (i < expr.size() - 1 && expr[i] == 'p' && expr[i + 1] == 'i')) { //constant
                            if (expr[i] == 'e') {
                                type = "constant";
                                info = "2.72";
                                add(info, type);
                                i++;
                            }
                            else {
                                type = "constant";
                                info = "3.14";
                                add(info, type);
                                i += 2;
                            }
                        }
                        else{
                            cout << "Error in " << i << endl; //nothing came up, so it was entered incorrectly
                            check = false;
                        }
                    }
                }
            }
        }
        return check;
    }

    //balancing brackets
    bool balancing_brackets(string expr) {
        node* cur = new node;
        stack stack;
        cur = head;
        while (cur != tail->next) {
            if (num_brack != 0) {
                if (cur->info == "(") { //if the opening parenthesis
                    stack.add(cur->info, cur->type);
                }
                else {
                    if (cur->info == ")") { //if the closing parenthesis
                        if (stack.isEmpty() == false) stack.take_last();
                        else return false;
                    }
                }
                cur = cur->next;
            }
            else return true;
        }
        if (stack.isEmpty() == true) return true;
        else return false;
    }

    void prefix_form() { //translation into the prefix form of the record
        stack stack;
        node* cur = new node;
        cur = tail; //we read from right to left
        do {
            if (cur->info == ")") { //if the right bracket
                if ((cur->prev->prev->prev)->type == "func") { //checking the function
                    for (int i = 0; i < 4; i++) {
                        add_res(cur->info, cur->type);
                        cur = cur->prev;
                    }
                }
                else {
                    stack.add(cur->info, cur->type);
                    cur = cur->prev;
                }
            }
            else {
                if (cur->type == "var" || cur->type == "constant") { //if a variable or constant, then just write to the string
                    add_res(cur->info, cur->type);
                    cur = cur->prev;
                }
                else {
                    if (cur->info == "(") { //if the left parenthesis
                        do {
                            add_res(stack.get_info(), stack.get_type());
                            stack.take_last();
                        } while (stack.take_last() != ")");
                        cur = cur->prev;
                    }
                    else {
                        if (cur->type == "op" || cur->type == "opposite") {
                            if (stack.isEmpty() == false) {
                                while (stack.get_size() >= 1 && get_prior(cur->info, cur->type) < get_prior(stack.get_info(), stack.get_type())) {
                                    add_res(stack.get_info(), stack.get_type());
                                    stack.take_last();
                                }
                                stack.add(cur->info, cur->type);
                                cur = cur->prev;
                            }
                            else {
                                stack.add(cur->info, cur->type);
                                cur = cur->prev;
                            }
                        }
                    }
                }
            }
        } while (cur != head->prev);
        if (stack.isEmpty() == false) {
            while (stack.isEmpty() != true) {
                add_res(stack.get_info(), stack.get_type());
                stack.take_last();
            }
        }
    }

    double answer() {
        stack stack;
        node_res* cur = new node_res;
        cur = head_res; //we read from right to left
        double buf;
        while (cur->next_res != NULL) {
            if (cur->type_res == "func") {
                if (cur->info_res == "cos") {
                    buf = double(stod((cur->next_res->next_res)->info_res) * 3.14 / 180.0);
                    cur->info_res = to_string(cos(buf));
                    cur->type_res = "var";
                    for_func(cur);
                }
                if (cur->info_res == "sin") {
                    buf = double(stod((cur->next_res->next_res)->info_res) * 3.14 / 180.0);
                    cur->info_res = to_string(sin(buf));
                    cur->type_res = "var";
                    for_func(cur);
                }
                if (cur->info_res == "tg") {
                    buf = double(stod((cur->next_res->next_res)->info_res) * 3.14 / 180.0);
                    cur->info_res = to_string(tan(buf));
                    cur->type_res = "var";
                    for_func(cur);
                }
                if (cur->info_res == "ctg") {
                    buf = double(stod((cur->next_res->next_res)->info_res) * 3.14 / 180.0);
                    cur->info_res = to_string(double(1.0 / tan(buf)));
                    cur->type_res = "var";
                    for_func(cur);
                }
                if (cur->info_res == "arctg") {
                    buf = double(stod((cur->next_res->next_res)->info_res) * 3.14 / 180.0);
                    cur->info_res = to_string(atan(buf));
                    cur->type_res = "var";
                    for_func(cur);
                }
                if (cur->info_res == "ln") {
                    buf = stod((cur->next_res->next_res)->info_res);
                    cur->info_res = to_string(log(buf));
                    cur->type_res = "var";
                    for_func(cur);
                }
                if (cur->info_res == "log") {
                    buf = stod((cur->next_res->next_res)->info_res);
                    cur->info_res = to_string(log10(buf));
                    cur->type_res = "var";
                    for_func(cur);
                }
                if (cur->info_res == "^") {
                    double buf1;
                    double buf2;
                    buf2 = stod((cur->next_res->next_res)->info_res);
                    buf1 = stod((cur->prev_res)->info_res);
                    cur->info_res = to_string(pow(buf1, buf2));
                    cur->type_res = "var";
                    node_res* del = cur->prev_res;
                    (del->next_res)->prev_res = del->prev_res;
                    (del->prev_res)->next_res = del->next_res;
                    delete del;
                    for_func(cur);
                    
                }
                if (cur->info_res == "sqrt") {
                    buf = stod((cur->next_res->next_res)->info_res);
                    cur->info_res = to_string(sqrt(buf));
                    cur->type_res = "var";
                    for_func(cur);
                }
            }
            else {
                if (cur->type_res == "opposite") {
                    if ((cur->next_res)->info_res == "(") {
                        cur = cur->next_res;
                        buf = -stod((cur->next_res)->info_res);
                        cur->info_res = to_string(buf);
                        cur->type_res = "opposite";
                        for_func(cur);
                    }
                    else {
                        buf = -stod((cur->next_res)->info_res);
                        cur->info_res = to_string(buf);
                        cur->type_res = "opposite";
                        node_res* del = cur->next_res;
                        (del->next_res)->prev_res = del->prev_res;
                        (del->prev_res)->next_res = del->next_res;
                        delete del;
                    }
                }
            }
            cur = cur->next_res;
        }

        cur = tail_res;
        while (cur != head_res->prev_res) {
            if (cur->type_res == "var" || cur->type_res == "constant" || cur->type_res == "opposite" || cur->type_res == "func" || cur->info_res == "^" || cur->info_res == "sqrt") stack.add(cur->info_res, cur->type_res);
            else {
                if (cur->type_res == "op") {
                    if (cur->info_res == "+") {
                        buf = double(stod(stack.take_last())) + double(stod(stack.take_last()));
                        stack.add(to_string(buf), cur->type_res);
                    }
                    if (cur->info_res == "-") {
                        buf = stod(stack.take_last()) - stod(stack.take_last());
                        stack.add(to_string(buf), cur->type_res);
                    }
                    if (cur->info_res == "*") {
                        buf = stod(stack.take_last()) * stod(stack.take_last());
                        stack.add(to_string(buf), cur->type_res);
                    }
                    if (cur->info_res == "/") {
                        buf = stod(stack.take_last()) / stod(stack.take_last());
                        stack.add(to_string(buf), cur->type_res);
                    }
                }
            }
            cur = cur->prev_res;
        }
        double res = stod(stack.take_last());
        return res;
    }

    int size() {
        return cnt;
    }

    string out() {
        node* cur = new node;
        string out;
        cur = head;
        while (cur != NULL) {
            out += cur->info;
            cur = cur->next;
        }
        return out;
    }

    string out_res() {
        node_res* cur = new node_res;
        string out_res;
        cur = head_res;
        while (cur != NULL) {
            out_res += cur->info_res;
            cur = cur->next_res;
        }
        return out_res;
    }
};
