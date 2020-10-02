#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_DATA_LEN (4096)

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

void
print_usage(){
    printf("Usage: ./sheet -d <delim> <table command> | "\
           "-d <delim> <line select> <data command>\n\r");
}

void /* To prevent buffer overflow */
scan_input(){
    char char_read;

    for(int i=0; ((char_read = getchar()) != EOF) && (i < FILE_DATA_LEN); i++){
      user_params.file_data[i] = char_read;
    }
}

int //vrati pocet sloupcu v souboru
count_collumns(){
    int count = 0;

    for(int i = 0; user_params.file_data[i] != '\n'; i++){
        if(user_params.file_data[i] == ':')
        {
            count++; //spocita pocet dvojtecek(:)
        }
    }
    return count+1;
}

int //vlozi novy radek na konec souboru
arow(){
    //printf("%d\n", count_collumns());
    return 0;
}

int
main(int argc, char **argv){
    int opt;
    int flag_chosen = 0;
    scan_input();
    printf("%s", user_params.file_data);
    if (argc == 1){
        print_usage();
        return 0;
    }


    while ((opt = getopt(argc, argv, "d:")) != -1){
        switch(opt){
            case 'd':
                if(!flag_chosen){
                    printf("Parameter -d is %c\n\r", optarg[0]);
                    user_params.delim = optarg[0];
                    flag_chosen = 1;
                } else {
                    printf("Perimiter can be set only once,"\
                           " ignoring other -d flags!\r\n");
                }
                break;

            default:
                print_usage();
                break;
        }
    }
    return 0;
}
