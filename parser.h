#pragma once
#include <stdio.h>

enum token_kind {
    mult,
    divs,
    sum,
    res,
    neg,
    exp,
    sin,
    cos,
    tan,
    lPar,
    rPar,
    var,
    num,
    eof_,
    error_,
    null_token
};

typedef struct Token {
    enum token_kind kind;
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
void destroy_list_token(pToken);
void free_pToken(pToken);
void print_list_token(pToken);

char * token_kind_to_string(enum token_kind);
