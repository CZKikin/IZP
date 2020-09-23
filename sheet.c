#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

char delim = ' ';

void
print_usage(){
    printf("Usage: ./sheet -d <delim> <table command> | "\
           "-d <delim> <line select> <data command>\n\r");
}

int
main(int argc, char **argv){
    int opt;
    int flag_chosen = 0;
    while ((opt = getopt(argc, argv, "d:h")) != -1){
        switch(opt){
            case 'd':
                if(!flag_chosen){
                    printf("Parameter -d is %c\n\r", optarg[0]);
                    delim = optarg[0];
                    flag_chosen = 1;
                } else {
                    printf("Perimiter can be set only once!\r\n");
                    exit(-1);
                }
                break;
            default:
                print_usage();
                break;
        }
    }
    return 0;
}
