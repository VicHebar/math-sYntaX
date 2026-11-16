#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"

int main(int argc, char * argv[]) {
    Variable * vars = NULL;
    if(argc < 2){
        printf("Error, el programa recibe argumentos! (Al menos una expresion)");
        return 0;
    }
    if(strlen(argv[1]) < 1) {
        printf("La expresion debe tener al menos un caracter");
        return 0;
    }
    if(argc > 2) {
        if(argc %2 != 0) {
            printf("Error, el programa recibio variables sin valor o valores si variable");
            return 0;
        }
        vars = malloc(((argc-2)/2)*sizeof(Variable));
        int item = 2;
        int aloc = 0;
        while (item < argc) {
            char * stop;
            vars[aloc].name = malloc(strlen(argv[item]) + 1);
            strcpy(vars[aloc].name,argv[item]);
            item++;
            vars[aloc].val = strtod(argv[item], &stop);
            item++;
            aloc++;
        }
    }
    char * exp = malloc(strlen(argv[1])+1);
    strcpy(exp,argv[1]);
    remove_spaces(exp);
    pArbol tree = NULL;
    pToken head = NULL;
    pToken tokens = NULL;
    head = tokener_string(exp);
    tokens = head;
    syntax_checker(head);
    //print_list_token(head);
    //print_list_status_msgs(head);
    tree = parse_exp(&tokens);
    print_parse_tree(tree);
    

    double result = eval_parse_tree(tree, vars, (argc-2)/2);
    printf("\n%s = %.2f", argv[1], result);
    destroy_list_token(head);
    return 0;
}