#include <getopt.h>
#include <stdio.h>

int main(int argc, char **argv){
    int opt;
    int perim_chosen = 0;
    while ((opt = getopt(argc, argv, "d")) != -1){
        switch(opt){
            case 'd':
                if(!perim_chosen){
                    printf("Parameter d\n\r");
                    perim_chosen = 1;
                } else {
                    printf("Parameter d has already been chosen\r\n");
                }
                break;
            default:
                printf("Error in parsing args\r\n");
        }
    }
    return 0;
}
