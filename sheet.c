#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define FILE_DATA_LEN (4096)

void print_usage();
void scan_input();
int (*get_func_pt())();
int run_tests();
int irow();
int arow();
int drow();
int drows();
int icol();
int acol();
int dcol();
int dcols();
int cset();
int to_lower();
int touuper();
int roundup();
int copy();
int swap();
int move();
int csum();
int cavg();
int cmin();
int cmax();
int ccount();
int cseq();
int rseq();
int rsum();
int ravg();
int rmin();
int rmax();
int rcount();
int rows();
int beginswith();
int contains();

/* TODO: Fce budou využívat sdílene struktury, ve které
 * jsou uloženy informace o vstupu, parametrech a datech souboru
 * který nám byl předán */

char commands[31][11] = {
    "irow",        /*0*/ 
    "arow",        /*1*/ 
    "drow",        /*2*/ 
    "drows",       /*4*/
    "icol",        /*5*/ 
    "acol",        /*6*/
    "dcol",        /*7*/ 
    "dcols",       /*8*/
    "cset",        /*9*/
    "tolower",     /*10*/
    "toupper",     /*11*/
    "round",       /*12*/
    "copy",        /*13*/
    "swap",        /*14*/
    "move",        /*15*/
    "csum",        /*16*/
    "cavg",        /*17*/
    "cmin",        /*18*/
    "cmax",        /*19*/
    "ccount",      /*20*/
    "cseq",        /*21*/
    "rseq",        /*22*/
    "rsum",        /*23*/
    "ravg",        /*24*/
    "rmin",        /*25*/
    "rmax",        /*26*/
    "rcount",      /*27*/
    "rows",        /*28*/
    "beginswith",  /*29*/
    "contains"     /*30*/
};

int (*functions[31])() = {
   irow,
   arow,
   drow,
   drows,
   icol,
   acol,
   dcol,
   dcols,
   cset,
   to_lower,
   touuper,
   roundup,
   copy,
   swap,
   move,
   csum,
   cavg,
   cmin,
   cmax,
   ccount,
   cseq,
   rseq,
   rsum,
   ravg,
   rmin,
   rmax,
   rcount,
   rows,
   beginswith,
   contains
};

struct params{
    char delim;
    char command[11];
    char arguments[3];
    int arg_count;
    char file_data[FILE_DATA_LEN];
};

struct params user_params = {
    .delim = ' '
};

// Funkce, která vrací ukazatel na funkci
int 
(*get_func_pt())(){
    for (int i=0; i<31; i++){
        if (strcmp(user_params.command, commands[i]) == 0)
            return functions[i];
    }
    printf("Invalid command\r\n");
    return NULL;
}

int 
run_tests(){
    printf("----TESTING----\r\n");
    int (*chosen_command)();
    scan_input();

    assert(user_params.file_data[FILE_DATA_LEN] == '\0');

    chosen_command = get_func_pt();
    assert(chosen_command  != NULL);

    int result;
    result = chosen_command();
    assert(result == 0);

    printf("----TESTS SUCCESSFUL----\r\n");
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

int
irow(){
    printf("Ty pyco ono to faka!\r\n");
    return -1;
}
int
drow(){
    return -1;
}
int
drows(){
    return -1;
}
int
icol(){
    return -1;
}
int
acol(){
    return -1;
}
int
dcol(){
    return -1;
}
int
dcols(){
    return -1;
}
int
cset(){
    return -1;
}
int
to_lower(){
    return -1;
}
int
touuper(){
    return -1;
}
int
roundup(){
    return -1;
}
int
copy(){
    return -1;
}
int
swap(){
    return -1;
}
int
move(){
    return -1;
}
int
csum(){
    return -1;
}
int
cavg(){
    return -1;
}
int
cmin(){
    return -1;
}
int
cmax(){
    return -1;
}
int
ccount(){
    return -1;
}
int
cseq(){
    return -1;
}
int
rseq(){
    return -1;
}
int
rsum(){
    return -1;
}
int
ravg(){
    return -1;
}
int
rmin(){
    return -1;
}
int
rmax(){
    return -1;
}
int
rcount(){
    return -1;
}
int
rows(){
    return -1;
}
int
beginswith(){
    return -1;
}
int
contains(){
    return -1;
}

int //vlozi novy radek na konec souboru
arow(){ //TODO: zjisti zda je potreba nazacatku printovat \n
    char new_line[2] = {user_params.delim,'\n'};
    strncat(user_params.file_data, new_line, 2); 
    return 0;
}

int
main(int argc, char **argv){
    int opt, perim_chosen = 0, test_flag = 0, result = -1;
    int (*chosen_command)();

    if (argc == 1){
        print_usage();
        return 0;
    }
    
    while ((opt = getopt(argc, argv, "d:t")) != -1){

        switch(opt){
            case 'd':
                if(!perim_chosen){
                    user_params.delim = optarg[0];
                    perim_chosen = 1;
                } else {
                    printf("Perimiter can be set only once,"\
                           " ignoring other -d flags!\r\n");
                }
                break;

            case 't':
                if (!test_flag){
                    test_flag = 1;
                }
                break;

            default:
                print_usage();
                break;
        }

    }

    if (optind < argc)
    {
        int arguments_index = 0;
        strncpy(user_params.command, argv[optind], strlen(argv[optind]));
        for (int i = optind+1; i<argc; i++){
            user_params.arguments[arguments_index] = argv[i][0];
            arguments_index++;
        }
                
    } else {
        printf("Missing command or arguments\r\n");
        return -1;
    }

    if (test_flag){
        result = run_tests();
        return result;
    }
    
    scan_input();

    if ((chosen_command = get_func_pt(user_params.command)) != NULL)
        result = chosen_command();

    if (result == 0)
        printf("%s", user_params.file_data);

    return result;
}
