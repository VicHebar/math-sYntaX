#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"

int main(int argc, char * argv[]) {
    if(argc != 2){
        printf("Error, el programa recibe un unico argumento!");
        return 0;
    }
    if(strlen(argv[1]) < 1) {
        printf("La expresion debe tener al menos un caracter");
        return 0;
    }
    char * exp = malloc(strlen(argv[1])+1);
    strcpy(exp,argv[1]);
    remove_spaces(exp);
    pToken head = NULL;
    head = tokener_string(exp);
    syntax_checker(head);
    //print_list_token(head);
    print_list_status_msgs(head);
    destroy_list_token(head);
    return 0;
}