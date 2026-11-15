#include "parser.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token_kind get_single_char_kind(char * head, char *c) {
    enum token_kind token;
    enum token_kind prev_kind = null_token;
    if(*c >= 48 && *c <= 57)
        token = num;
    else if(*c == '.')
        token = num;
    else if((*c >= 97 && *c <= 122) || (*c >= 65 && *c <= 90) || *c == 95) 
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
        if(c == head || prev_kind <= lPar) {
            token = neg;
        } else if(prev_kind >= rPar && prev_kind <= num)
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
    temp->msgs_quantity = 0;
    temp->msgs = NULL;
    temp->next = NULL;
    return temp;
}

pToken new_list_token(Token token) {
    pToken token_pointer = new_void_list_token();
    token_pointer->content = malloc(strlen(token.content) + 1);
    strcpy(token_pointer->content, token.content);
    token_pointer->kind = token.kind;
    token_pointer->msgs_quantity = token.msgs_quantity;
    if(token.msgs_quantity > 0) {
        token_pointer->msgs = malloc(sizeof(enum token_status)*token.msgs_quantity);
        memcpy(token_pointer->msgs, token.msgs, sizeof(enum token_status));
    }
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

bool set_token_status(pToken token, enum token_status status) {
    if(token->msgs_quantity == 0) {
        token->msgs = malloc(sizeof(enum token_status));
        if(!token->msgs) return false;
        *(token->msgs) = status;
        token->msgs_quantity++;
    } else {
        enum token_status * ptr;
        token->msgs_quantity++;
        ptr = realloc(token->msgs, token->msgs_quantity * sizeof(enum token_status));
        if(!ptr) return false;
        token->msgs = ptr;
        token->msgs[(token->msgs_quantity)-1] = status;
    }
    return true;
}

void free_pToken(pToken token) {
    free(token->content);
    if(token->msgs_quantity > 0)
        free(token->msgs);
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

char * token_status_to_string(enum token_status status) {
    char * msg = malloc(30);
    switch (status) {
        case too_many_points:
            strcpy(msg, "too_many_points");
            break;
        case no_number:
            strcpy(msg, "no_number");
            break;
        case missing_infix_operator:
            strcpy(msg, "missing_infix_operator");
            break;
        case missing_left_operand:
            strcpy(msg, "missing_left_operand");
            break;
        case missing_right_operand:
            strcpy(msg, "missing_right_operand");
            break;
        case missing_left_parenthesis:
            strcpy(msg, "missing_left_parenthesis");
            break;
        case missing_right_parenthesis:
            strcpy(msg, "missing_right_parenthesis");
            break;
        case empty_expression:
            strcpy(msg, "empty_expression");
            break;
        case left_parenthesis_found:
            strcpy(msg, "left_parenthesis_found");
            break;
        case right_parenthesis_found:
            strcpy(msg, "right_parenthesis_found");
            break;
        case no_error:
            strcpy(msg, "no_error");
            break;
        case null_token_error:
            strcpy(msg, "null_token_error");
            break;
        default:
            strcpy(msg, "unknown");
    }
    return msg;
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

void print_list_status_msgs(pToken to_print) {
    char * status;
    char * kind;
    while (to_print) {
        int status_i = 0;
        kind = token_kind_to_string(to_print->kind);
        printf("Token: %s\nKind: %s\n", to_print->content, kind);
        if(to_print->msgs_quantity > 0) {
            printf("STATUS MSGS: \n");
            while (status_i < to_print->msgs_quantity) {
                status = token_status_to_string(to_print->msgs[status_i]);
                printf("\t%d. %s\n", status_i, status);
                free(status);
                status_i++;
            }
        } else {
            printf("NO STATUS MSGS.\n");
        }
        printf("\n");
        free(kind);
        to_print = to_print->next;
    }
}

void remove_spaces(char * string) {
    char * temp_string = NULL;
    temp_string = malloc(strlen(string) + 1);
    *temp_string = '\0';
    for( char * p = strtok(string, " "); p != NULL; p = strtok(NULL, " "))
        strcat(temp_string, p);
    strcpy(string, temp_string);
    free(temp_string);
}

bool find_token_status(Token token, enum token_status status) {
    if(token.msgs_quantity == 0)
        return false;
    int msg_i = 0;
    while (msg_i < token.msgs_quantity) {
        if(token.msgs[msg_i] == status)
            return true;
        msg_i++;
    }
    return false;
}

void syntax_checker_lPar(pToken token) {
    pToken pList = token->next;
    int balance = 1;
    while(pList) {
        if(pList->kind == lPar)
            balance++;
        if(pList->kind == rPar)
            balance--;
        if(balance == 0)
            break;
        pList = pList->next;
    }
    if(balance != 0){
        if(!set_token_status(token, missing_right_parenthesis))
            printf("\nError agregando error \"missing_right_parenthesis\"\n");
    } else {
        if(!set_token_status(token, right_parenthesis_found))
            printf("\nError agregando status \"rPar_found\"\n");
        if(!set_token_status(pList, left_parenthesis_found))
            printf("\nError agregando status \"rPar_found\"\n");
        if(token->next){
            if(token->next->kind == rPar)
                if(!set_token_status(token, empty_expression))
                    printf("\nError agregando error \"empty_expression\"\n");
        }
    }
    if(token->next){
        if(token->next->kind >= mult && token->next->kind <= exp){
            if(!set_token_status(token, missing_left_operand))
                printf("\nError agregando error \"missing_left_operand\"\n");
        }
    }
}

void syntax_checker_rPar(pToken token) {
    if(!find_token_status(*token, left_parenthesis_found)) {
        if(!set_token_status(token, missing_left_parenthesis))
            printf("\nError agregando status \"missing_left_parenthesis\"\n");
    }
    if(token->next){
        if(token->next->kind >= sin && token->next->kind <= num) {
            if(!set_token_status(token, missing_infix_operator))
                printf("\nError agregando status \"missing_infix_operator\"\n");
    }
    }
}

void syntax_checker(pToken head) {
    pToken pList = head;

    while (pList) {
        if(pList->kind == num) {
            char *p = pList->content;
            int points_q = 0;
            int numbers_q = 0;
            while (*p){
                if(*p == '.')
                    points_q++;
                if(*p >= 48 && *p <= 57)
                    numbers_q++;
                p++;
            }
            if(points_q > 1)
                if(!set_token_status(pList, too_many_points))
                    printf("\nError agregando error \"too_many_points\"\n");
            if(numbers_q == 0)
                if(!set_token_status(pList, no_number))
                    printf("\nError agregando error \"no_number\"\n");
            if(pList->next) {
                if(pList->next->kind >= lPar && pList->next->kind <= num && pList->next->kind != rPar)
                    if(!set_token_status(pList, missing_infix_operator))
                        printf("\nError agregando error \"missing_infix_operator\"\n");
            }
        } else if(pList->kind == var) {
            if(pList->next) {
                if(pList->next->kind >= lPar && pList->next->kind <= num)
                    if(!set_token_status(pList, missing_infix_operator))
                        printf("\nError agregando error \"missing_infix_operator\"\n");
            }
        } else if(pList->kind >= mult && pList->kind <= exp) {
            if(pList == head) {
                if(!set_token_status(pList, missing_left_operand))
                    printf("\nError agregando error \"missing_left_operand\"\n");
            }
            if(pList->next){
                if(pList->next->kind >= mult && pList->next->kind <= exp)
                    if(!set_token_status(pList, missing_right_operand))
                        printf("\nError agregando error \"missing_right_operand\"\n");
            } else {
                if(!set_token_status(pList, missing_right_operand))
                    printf("\nError agregando error \"missing_right_operand\"\n");
            }
        } else if(pList->kind >= sin && pList->kind <= tan) {
            if(pList->next->kind != lPar)
                if(!set_token_status(pList, missing_left_parenthesis))
                        printf("\nError agregando error \"missing_left_parenthesis\"\n");
        } else if(pList->kind == lPar) {
            syntax_checker_lPar(pList);
        } else if(pList->kind == rPar) {
            syntax_checker_rPar(pList);
        }
        pList = pList->next;
    }
}


