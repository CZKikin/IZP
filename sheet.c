#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NUMBER_OF_COMMANDS (31)
#define NUMBER_OF_ARGUMENTS (5)
#define LINE_DATA_LEN (10240)
#define ARG_LEN (100)

int delims;

void print_usage();
int scan_input();
int (*get_func_pt(char *command))();
int run_tests();
int check_for_space(size_t size_needed);
int count_collumns();
int get_delim_index(int order);
//int get_row_end(int cbsr);
int (*find_command())();
void find_arguments(int argc, char **argv);
int irow(int last_line);
int arow(int last_line);
int drow(int last_line);
int drows(int last_line);
int icol(int last_line);
int acol(int last_line);
int dcol(int last_line);
int dcols(int last_line);
int cset(int last_line);
int to_lower(int last_line);
int touuper(int last_line);
int roundup(int last_line);
int copy(int last_line);
int swap(int last_line);
int move(int last_line);
int csum(int last_line);
int cavg(int last_line);
int cmin(int last_line);
int cmax(int last_line);
int ccount(int last_line);
int cseq(int last_line);
int rseq(int last_line);
int rsum(int last_line);
int ravg(int last_line);
int rmin(int last_line);
int rmax(int last_line);
int rcount(int last_line);
int rows(int last_line);
int beginswith(int last_line);
int contains(int last_line);

/* TODO: Fce budou využívat sdílene struktury, ve které
 * jsou uloženy informace o vstupu, parametrech a datech souboru
 * který nám byl předán */

char commands[NUMBER_OF_COMMANDS][11] = {
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

int (*functions[NUMBER_OF_COMMANDS])() = {
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
    char arguments[NUMBER_OF_ARGUMENTS][ARG_LEN];
    int arg_count;
    char line_data[LINE_DATA_LEN];
    int line_number;
};

struct params user_params = {
    .delim = ' '
};

int
irow(int last_line){
    (void)last_line; //pičuje compiler a ja potřebuju testovat :)
    int selected_row = atoi(user_params.arguments[0]);

    if(user_params.line_number == selected_row){
        int edit_size = count_collumns()-1;
        if (check_for_space(edit_size) != 0)
            return -1;

        for(int i = 0; i<edit_size; i++){
            printf("%c",user_params.delim);
        }

        printf("\n");


    }

    return 0;
}
int
drow(int last_line){
    (void)last_line;

    int selected_row = atoi(user_params.arguments[0]);

    if(user_params.line_number == selected_row){

        user_params.line_data[0] = '\0';

    }

    return 0;
}
int
drows(int last_line){
    (void)last_line;

    int selected_row1 = atoi(user_params.arguments[0]);
    int selected_row2 = atoi(user_params.arguments[1]);

    if(selected_row1>selected_row2) //N<=M
        return -1;

    if(user_params.line_number >= selected_row1 && user_params.line_number <= selected_row2){

        user_params.line_data[0] = '\0';

    }

    return 0;
}

//vrati index n-teho delimu
int get_delim_index(int order){
    int appear = 0; //vyskyt delimu v radku
    for(int i = 0; i<LINE_DATA_LEN; i++){
        if(user_params.line_data[i] == user_params.delim){
            appear++;
        }
        //pokud je nalezeny delim tolikaty kolikaty chceme, vratime index
        if(appear == order-1){
            return i;
        }
    }
    return -1;
}

int //vrati delku pole
get_len(char *arr){
    int q = 0; //pocet znaku
    for(int i = 0; arr[i] != '\0'; i++){
            q++;
    }
    return q;
}

int //vlozi prazdny sloupec pred sloupec C
icol(int last_line){
    (void)last_line;

    int edit_size = 1; //delka delim

    if (check_for_space(edit_size) != 0)
        return -1;

    //ziska index n-teho delimu
    int index = get_delim_index(atoi(user_params.arguments[0]));

    if(index == -1)
        return -1;

    //vsude +1 nez je potreba pro \0
    char start[index+2], end[get_len(user_params.line_data)-index+1];
    memset(start, 0, sizeof start); //vynulovani
    memset(end, 0, sizeof end);

    strncpy(start,user_params.line_data,index);
    strcpy(end,user_params.line_data + index);

    start[index]=user_params.delim;

    char full[get_len(user_params.line_data)+edit_size+1];

    memset(full, 0, sizeof full);
    strcat(full,start);
    strcat(full,end);

    strcpy(user_params.line_data,full);
    return 0;
}

int
acol(int last_line){
    (void)last_line;
    int edit_size = 1; //delka delim

    if (check_for_space(edit_size) != 0)
        return -1;

    if (last_line)
        return -1;

    user_params.line_data[get_len(user_params.line_data)]=user_params.delim;
    return 0;
}

int //odstrani sloupec N
dcol(int last_line){
    (void)last_line;
    int selected_col = atoi(user_params.arguments[0]);

    if(selected_col>count_collumns())
        return -1;

    int cols = count_collumns();
    int index = get_delim_index(selected_col);
    if(cols == 1){
        memset(user_params.line_data, 0, sizeof LINE_DATA_LEN);
        return 0;
    }
    else {
        if(cols == selected_col){
            char start[index+1];
            memset(start, 0, sizeof start); //vynulovani
            strncpy(start,user_params.line_data,index);
            memset(user_params.line_data, 0, sizeof LINE_DATA_LEN);
            strcpy(user_params.line_data,start);
        }

        else{
            int afterIndex = get_delim_index(selected_col+1);
            //smazani textu mezi <index a afterindex)

            char start[index+1], end[get_len(user_params.line_data)-index+1];
            memset(start, 0, sizeof start); //vynulovani
            memset(end, 0, sizeof end);

            strncpy(start,user_params.line_data,index);
            strcpy(end,user_params.line_data + afterIndex);

            char full[get_len(user_params.line_data)-(afterIndex-index)+1];

            memset(full, 0, sizeof full);
            strcat(full,start);
            strcat(full,end);

            strcpy(user_params.line_data,full);
        }
    }
    return 0;
}

int //smaze sloupce mezi N a M
dcols(int last_line){
    (void)last_line;

    if(!last_line){

        int selected_col1 = atoi(user_params.arguments[0]);
        int selected_col2 = atoi(user_params.arguments[1]);

        if(selected_col1>selected_col2) //N<=M
            return -1;

        if(selected_col2>count_collumns())
            return -1;

        int index = get_delim_index(selected_col1);
        int afterIndex = get_delim_index(selected_col2+1);
        printf("i%da%d",index,afterIndex);
        //smazani textu mezi <index a afterindex)

        if(index!=0){
            char start[index+1], end[get_len(user_params.line_data)-index+1];

            memset(start, 0, sizeof start); //vynulovani
            memset(end, 0, sizeof end);


            strncpy(start,user_params.line_data,index);

            strcpy(end,user_params.line_data + afterIndex);

            char full[get_len(user_params.line_data)-(afterIndex-index)+1];

            memset(full, 0, sizeof full);

            strcat(full,start);
            strcat(full,end);

            strcpy(user_params.line_data,full);
        }
        else{

        }
        return 0;
    }
    return -1;
}
int
cset(int last_line){
    (void)last_line;
    //int selected_cell = atoi(user_params.arguments[0]);
    char str[ARG_LEN];
    int collumns;
    strncpy(str,user_params.arguments[1],ARG_LEN);
    //printf("cell %d%s\n", selected_cell, str);
    collumns = count_collumns();
    printf("collumns %d\n", collumns);
    return -1;
}
int
to_lower(int last_line){
    (void)last_line;
    return -1;
}
int
touuper(int last_line){
    (void)last_line;
    return -1;
}
int
roundup(int last_line){
    (void)last_line;
    return -1;
}
int
copy(int last_line){
    (void)last_line;
    return -1;
}
int
swap(int last_line){
    (void)last_line;
    return -1;
}
int
move(int last_line){
    (void)last_line;
    return -1;
}
int
csum(int last_line){
    (void)last_line;
    return -1;
}
int
cavg(int last_line){
    (void)last_line;
    return -1;
}
int
cmin(int last_line){
    (void)last_line;
    return -1;
}
int
cmax(int last_line){
    (void)last_line;
    return -1;
}
int
ccount(int last_line){
    (void)last_line;
    return -1;
}
int
cseq(int last_line){
    (void)last_line;
    return -1;
}
int
rseq(int last_line){
    (void)last_line;
    return -1;
}
int
rsum(int last_line){
    (void)last_line;
    return -1;
}
int
ravg(int last_line){
    (void)last_line;
    return -1;
}
int
rmin(int last_line){
    (void)last_line;
    return -1;
}
int
rmax(int last_line){
    (void)last_line;
    return -1;
}
int
rcount(int last_line){
    (void)last_line;
    return -1;
}
int
rows(int last_line){
    (void)last_line;
    return -1;
}
int
beginswith(int last_line){
    (void)last_line;
    return -1;
}
int
contains(int last_line){
    (void)last_line;
    return -1;
}

int //vlozi novy radek na konec souboru
arow(int last_line){
    if (!last_line){
        return 0;
    } else {

        if (check_for_space(delims) != 0)
            return -1;

        for(int i = 0; i<delims; i++){
            printf("%c", user_params.delim);
        }

    }
    return 0;
}

int
(*get_func_pt(char *command))(){
    for (int i=0; i<NUMBER_OF_COMMANDS; i++){
        if (strcmp(command, commands[i]) == 0)
            return functions[i];
    }
    return NULL;
}

int
run_tests(int(*chosen_command)()){
    int result;

    printf("----TESTING----\r\n");
    result = scan_input();

    assert(result == 0);
    assert(user_params.line_data[LINE_DATA_LEN] == '\0');

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


int /* To prevent buffer overflow */
scan_input(){
    char ch;

    memset(user_params.line_data, 0, sizeof(user_params.line_data));
    for(int i=0; ((ch = getchar()) != EOF && ch != '\n') && \
            (i < LINE_DATA_LEN); i++){
      user_params.line_data[i] = ch;

    }
    user_params.line_number++;
    if (ch == EOF)
        return 1;
    delims = count_collumns()-1;
    return 0;
}

int
check_for_space(size_t size_needed){     /* -1 because indexing from 0 */
    if (user_params.line_data[(LINE_DATA_LEN - 1) - size_needed] != '\0')
        return -1;
    return 0;
}

int //vrati pocet sloupcu v souboru
count_collumns(){
    int count = 0;

    for(int i = 0; user_params.line_data[i] != '\0'; i++){
        if(user_params.line_data[i] == user_params.delim)
        {
            count++; //spocita pocet rozdělovačů
        }
    }
    return count+1;
}

int
(*find_command())(){
    int (*chosen_command)();

    for (int i = 0; i < NUMBER_OF_ARGUMENTS; i++){
        if ((chosen_command = get_func_pt(user_params.arguments[i])) != NULL){
            for (int j=i; j<NUMBER_OF_ARGUMENTS; j++)
                strncpy(user_params.arguments[j], user_params.arguments[j + 1], ARG_LEN);
            return chosen_command;

        }
    }
    return NULL;
}

void
find_arguments(int argc, char **argv){
    for (int i = optind, j = 0; i<argc; i++,j++){
        strncpy(user_params.arguments[j], argv[i], ARG_LEN);
    }
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
                    user_params.line_number = 0;
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
        find_arguments(argc, argv);

        chosen_command = find_command();
        if (chosen_command == NULL)
            return -1;

    } else {
        printf("Missing arguments\r\n");
        return -1;
    }

    if (test_flag){
        result = run_tests(chosen_command);
        return result;
    }

    while (scan_input() != 1){
        if ((result = chosen_command(0)) != 0)
            return -1;
        printf("%s\n", user_params.line_data);
    }

    if ((result = chosen_command(1)) != 0)
        return -1;
    printf("%s\n", user_params.line_data);

    return 0;
}
