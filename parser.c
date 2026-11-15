#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token_kind get_single_char_kind(char * head, char *c) {
    enum token_kind token;
    enum token_kind prev_kind = null_token;
    enum token_kind next_kind = null_token;
    if(*c >= 48 && *c <= 57)
        token = num;
    else if(*c == '.') {
        if(c != head)
            prev_kind = get_single_char_kind(head, c-1);
        if(*c != '\0')
            next_kind = get_single_char_kind(head, c+1);
        if(prev_kind != num || next_kind != num)
            token = error_;
        else
            token = num;
    }
    else if(*c >= 97 && *c <= 122) 
        token = var;
    else if(*c == '*')
        token = mult;
    else if(*c == '/')
        token = divs;
    else if(*c == '+')
        token = sum;
    else if(*c == '-') {
        if(c != head)
            prev_kind = get_single_char_kind(head, c-1);
        if(c == head || prev_kind == mult || prev_kind == divs || 
                prev_kind == sum || prev_kind == res || prev_kind == neg ||
                prev_kind == exp || prev_kind == lPar) {
            token = neg;
        } else if(prev_kind == num || prev_kind == var || prev_kind == rPar)
            token = res;
    } else if(*c == '^')
        token = exp;
    else if(*c == '(')
        token = lPar;
    else if(*c == ')')
        token = rPar;
    else if(*c == '\0')
        token = eof_;
    else
        token = error_;
    return token;
}

pToken tokener_string(char * string) {
    enum token_kind initial_token;
    pToken list_token = NULL;
    char * p;
    p = string;
    while(*p != '\0') {
        initial_token = get_single_char_kind(string, p);
        if(initial_token == error_) {
            printf("Error al parsear la expresion %s, "
                "en el caracter: %c", string, *p);
            if(list_token != NULL)
                destroy_list_token(list_token);
            return NULL;
        }
        pToken temp;
        char * q = p;
        unsigned int size = 0;
        unsigned int i = 0;
        temp = new_void_list_token();
        if(initial_token == num || initial_token == var) {
            while (get_single_char_kind(string, q) == initial_token) {
                size++;
                q++;
            }
            temp->content = malloc(size + 1);
            q = p;
            while (i < size) {
                temp->content[i] = *q;
                i++;
                q++;
            }
            temp->content[i] = '\0';
            p = q;
        }
        else {
            temp->content = malloc(2);
            temp->content[0] = *q;
            temp->content[1] = '\0';
            p++;
        }
        temp->kind = initial_token;
        if(add_node_token_list(&list_token, temp) == 0){
            printf("Error agregando nuevo token");
            if(list_token != NULL)
                destroy_list_token(list_token);
            return NULL;
        }
    }
    return list_token;
}

pToken new_void_list_token() {
    pToken temp = malloc(sizeof(Token));
    temp->content = NULL;
    temp->kind = null_token;
    temp->next = NULL;
    return temp;
}

pToken new_list_token(Token token) {
    pToken token_pointer = new_void_list_token();
    token_pointer->content = malloc(strlen(token.content) + 1);
    strcpy(token_pointer->content, token.content);
    token_pointer->kind = token.kind;
    token_pointer->next = NULL;
    return token_pointer;
}

int add_node_token_list(pToken *head,  pToken node) {
    pToken token_pointer;
    if(!(*head)) {
        *head = new_list_token(*node);
        return 1;
    }
    token_pointer = *head;
    while (token_pointer->next)
        token_pointer = token_pointer->next;
    token_pointer->next = new_list_token(*node);
    return 1;
}

void free_pToken(pToken token) {
    free(token->content);
    free(token);
}

void destroy_list_token(pToken to_delete) {
    pToken next_to;
    while (to_delete) {
        next_to = to_delete->next;
        free_pToken(to_delete);
        to_delete = next_to;
    }
}

char * token_kind_to_string(enum token_kind kind) {
    char *string_kind = malloc(20);
    switch (kind) {
        case mult:
            strcpy(string_kind, "mult");
            break;
        case divs:
            strcpy(string_kind, "divs");
            break;
        case sum:
            strcpy(string_kind, "sum");
            break;
        case res:
            strcpy(string_kind, "res");
            break;
        case neg:
            strcpy(string_kind, "neg");
            break;
        case var:
            strcpy(string_kind, "var");
            break;
        case num:
            strcpy(string_kind, "num");
            break;
        case exp:
            strcpy(string_kind, "exp");
            break;
        case sin:
            strcpy(string_kind, "sin");
            break;
        case cos:
            strcpy(string_kind, "cos");
            break;
        case tan:
            strcpy(string_kind, "tan");
            break;
        case lPar:
            strcpy(string_kind, "lPar");
            break;
        case rPar:
            strcpy(string_kind, "rPar");
            break;
        case eof_:
            strcpy(string_kind, "eof_");
            break;
        case error_:
            strcpy(string_kind, "error_");
            break;
        case null_token:
            strcpy(string_kind, "null_token");
            break;
        default:
            strcpy(string_kind, "unknown");
            break;
    }
    return string_kind;
}

void print_list_token(pToken to_print) {
    char * kind;
    while (to_print) {
        kind = token_kind_to_string(to_print->kind);
        printf("[\'%s\', %s] . ", to_print->content, kind);
        free(kind);
        to_print = to_print->next;
    }
}