#pragma once
#include <stdio.h>
#include <stdbool.h>

enum token_kind {
    mult,
    divs,
    sum,
    res,
    exp,
    neg,
    lPar,
    rPar,
    sin,
    cos,
    tan,
    var,
    num,
    eof_,
    error_,
    null_token
};

enum token_status {
    too_many_points,
    no_number,
    missing_infix_operator,
    missing_left_operand,
    missing_right_operand,
    missing_left_parenthesis,
    missing_right_parenthesis,
    empty_expression,
    left_parenthesis_found,
    right_parenthesis_found,
    no_error,
    null_token_error
};

typedef struct Token {
    enum token_kind kind;
    int msgs_quantity;
    enum token_status * msgs;
    char * content;
    struct Token * next;
} Token;

typedef struct Nodo {
    Token tok;
    struct Nodo * left;
    struct Nodo * right;
} Nodo;

typedef Nodo * pNodo;
typedef Token* pToken;

pToken tokener_string(char *);
pToken new_void_list_token(); //Crea un nodo nuevo pero vacío
pToken new_list_token(Token); //Crea un nodo con los valores de Token
int add_node_token_list(pToken *, pToken); //Agrega un nuevo nodo previamente creado al final de la lista
bool set_token_status(pToken, enum token_status);
void destroy_list_token(pToken);
void free_pToken(pToken);
char * token_kind_to_string(enum token_kind);
char * token_status_to_string(enum token_status);
void print_list_token(pToken);
void print_list_status_msgs(pToken);
void remove_spaces(char *);
bool find_token_status(Token, enum token_status);
void syntax_checker(pToken);
char * error_msg_generator(pToken);

char * token_kind_to_string(enum token_kind);
