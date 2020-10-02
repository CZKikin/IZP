#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_DATA_LEN (4096)

void print_usage();
void scan_input();
int run_tests();

/* TODO: Fce budou využívat sdílene struktury, ve které
 * jsou uloženy informace o vstupu, parametrech a datech souboru
 * který nám byl předán */

struct params{
    char delim;
    char command[10];
    char arguments[3];
    int arg_count;
    char file_data[FILE_DATA_LEN];
};

struct params user_params = { 
    .delim = ' '
};

int run_tests(){
    scan_input();
    if (user_params.file_data[FILE_DATA_LEN] != '\0'){
        printf("Buffer overflow!\r\n");
        return -1;
    }

    return 0;
}

void
print_usage(){
    printf("Usage: ./sheet -d <delim> <table command> | "\
           "-d <delim> <line select> <data command>\n\rFor testing: -t\r\n");
}

void /* To prevent buffer overflow */
scan_input(){
    char char_read;

    for(int i=0; ((char_read = getchar()) != EOF) && (i < FILE_DATA_LEN); i++){
      user_params.file_data[i] = char_read;
    }
}

int
main(int argc, char **argv){
    int opt, perim_chosen = 0, test_flag = 0;

    if (argc == 1){
        print_usage();
        return 0;
    }
    
    while ((opt = getopt(argc, argv, "d:t")) != -1){
        switch(opt){
            case 'd':
                if(!perim_chosen){
                    printf("Parameter -d is %c\n\r", optarg[0]);
                    user_params.delim = optarg[0];
                    perim_chosen = 1;
                } else {
                    printf("Perimiter can be set only once,"\
                           " ignoring other -d flags!\r\n");
                }
                break;

            case 't':
                if (!test_flag){
                    if (run_tests() == 0)
                        printf("Test successful\r\n");
                    else
                        return -1;
                    return 0;
                }
                break;

            default:
                print_usage();
                break;
        }

    }

    scan_input();
    printf("%s", user_params.file_data);

    return 0;
}
