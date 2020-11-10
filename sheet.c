#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define NUMBER_OF_COMMANDS (28)
#define NUMBER_OF_LINE_SELS (3)
#define NUMBER_OF_ARGUMENTS (10)
#define LINE_DATA_LEN (10240)
#define ARG_LEN (100)

/* We are not allowed to use header files :) / more files... no point of *
 * making static functions.                                              */

int validate_second_command();
int check_for_dash(char *argument);
int find_word_column(int pos_of_str);
int (*get_line_sel_pt(char *line_sel))();
int (*find_line_sel())();
int (*get_command_pt(char *command))();
int run_tests(int(*chosen_command)());
void print_usage();
int scan_input();
int check_for_space(size_t size_needed);
int count_collumns();
int (*find_command())();
void find_arguments(int argc, char **argv);
void separate_line_sel_from_args();
int insert_text(char *text, int start_index, int end_index);
int get_len(char *arr);

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
int to_upper(int last_line);
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
    "roundup",       /*12*/
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
    "rcount"      /*27*/
};

char line_selector_commands[NUMBER_OF_LINE_SELS][11] = {
    "rows",        
    "beginswith", 
    "contains"     
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
   to_upper,
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
   rcount
};

int (*line_sels[NUMBER_OF_LINE_SELS])() = {
   rows,
   beginswith,
   contains
};

struct params{
    char delim;
    char arguments[NUMBER_OF_ARGUMENTS][ARG_LEN];
    char line_selectors[2][ARG_LEN];
    int arg_count;
    char line_data[LINE_DATA_LEN];
    int line_number;
    int delims_count;
    int (*second_command)();
};

struct params user_params = {
    .delim = ' ',
    .second_command = NULL,
};

char checker = '\0';

int //vrati delku pole
get_len(char *arr){
    int q = 0; //pocet znaku
    for(int i = 0; arr[i] != '\0'; i++){
            q++;
    }
    return q;
}
int //vlozi zadany text mezi 2 zadane indexy
insert_text(char *text, int start_index, int end_index){

    int text_len = get_len(text);

    if(check_for_space(text_len) != 0)
        return -1;

    char start[start_index+1], end[get_len(user_params.line_data)-end_index+1];
    memset(start, 0, sizeof start); //vynulovani
    memset(end, 0, sizeof end);

    strncpy(start,user_params.line_data,start_index);
    strcpy(end,user_params.line_data + end_index);

    char full[get_len(user_params.line_data)-(end_index-start_index)+1+text_len];

    memset(full, 0, sizeof full);
    strcat(full, start);
    strcat(full, text);
    strcat(full, end);

    strcpy(user_params.line_data,full);

    return 0;
}
int
validate_second_command(){
    user_params.second_command = find_command();

    if (user_params.second_command == NULL)
        return -1;

    return 0;
}
int
check_for_dash(char *argument){
    if (argument[0] == '-')
        return -1;
    return atoi(argument);
}
int
find_word_column(int pos_of_str){
    int cols = 1; /* because lol:lol are 2 cols.. */
    for(int i=0; i<pos_of_str; i++){
        if(user_params.line_data[i] == user_params.delim)
            cols++;
    }    
    return cols;
}
int
(*get_line_sel_pt(char *line_sel))(){
    for (int i=0; i<NUMBER_OF_LINE_SELS; i++){
    if ((strcmp(line_sel, line_selector_commands[i])) == 0)
        return line_sels[i];
    }
    return NULL;
}
int
(*find_line_sel())(){
    int (*chosen_command)();

        for (int i = 0; i < NUMBER_OF_ARGUMENTS; i++){
            if ((chosen_command = get_line_sel_pt(user_params.arguments[i])) != NULL){
                for (int j=i; j<NUMBER_OF_ARGUMENTS; j++)
                    strncpy(user_params.arguments[j], user_params.arguments[j + 1], ARG_LEN);
                return chosen_command;

            }
        }
        return NULL;

}
int
(*get_command_pt(char *command))(){
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
    int is_checker = 0;

    memset(user_params.line_data, 0, sizeof(user_params.line_data));
    if (checker != '\0'){
        user_params.line_data[0] = checker;
        is_checker = 1;
     }

    for(int i=is_checker, counter=0; ((ch = getchar()) != EOF && ch != '\n') && \
            (i < LINE_DATA_LEN); i++, counter++){
        if (ch == ':')
            counter = 0;
        user_params.line_data[i] = ch;
        if (counter>99){
            printf("Cell too big, max chars in cell is 100\n");
            return -1;
        }
    }
    user_params.line_number++;

    if ((checker = getchar()) == EOF)
        return 1;

    user_params.delims_count = count_collumns()-1;
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
        if ((chosen_command = get_command_pt(user_params.arguments[i])) != NULL){
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
void
separate_line_sel_from_args(){
    for(int i=0; i<2; i++)
        strncpy(user_params.line_selectors[i], user_params.arguments[i], ARG_LEN);

    for(int i=0,j=2; j<NUMBER_OF_ARGUMENTS; i++, j++)
        strncpy(user_params.arguments[i], user_params.arguments[j], ARG_LEN);
}
int
irow(int last_line){
    (void)last_line;
    int selected_row = atoi(user_params.arguments[0]);

    if(user_params.line_number == selected_row){
        int edit_size = user_params.delims_count;

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

    if(user_params.line_number >= selected_row1 && 
        user_params.line_number <= selected_row2){
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
        if(appear >= order-1){
            return i;
        }
    }
    return -1;
}

int //vlozi novy radek na konec souboru
arow(int last_line){
    if (!last_line){
        return 0;
    } else {

        if (check_for_space(user_params.delims_count+1) != 0)
            return -1;

        char str_for_strncat[2] = {user_params.delim, '\0'};
        strncat(user_params.line_data, "\n", 2);
        for (int i=0; i<=user_params.delims_count; i++)
            strncat(user_params.line_data, str_for_strncat, 1);

    }
    return 0;
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

    if(last_line)
        return -1;

    char arr[2];
    arr[0]=user_params.delim;
    insert_text(arr,index,index);
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
    int cols = count_collumns();

    if(selected_col>cols)
        return -1;

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
            int end_index = get_delim_index(selected_col+1);
            insert_text("",index,end_index);
        }
    }
    return 0;
}
int //smaze sloupce mezi N a M
dcols(int last_line){
    (void)last_line;

    if(last_line)
        return -1;

    int selected_col1 = atoi(user_params.arguments[0]);
    int selected_col2 = atoi(user_params.arguments[1]);

    if(selected_col1>selected_col2) //N<=M
        return -1;

    if(selected_col2>count_collumns())
        return -1;

    int index = get_delim_index(selected_col1);
    int end_index = get_delim_index(selected_col2+1);

    insert_text("",index,end_index);

    return 0;
}
int
cset(int last_line){
    (void)last_line;
    return -1;
}
int //vrati text mezi zadanymi indexy
get_text(char* sub_text, int start_index, int end_index){

    for(int i = 0; i<end_index-start_index; i++){
        sub_text[i]=user_params.line_data[start_index+i];
    }
    return 0;
}
int //funkce upravi hodnotu indexu podle toho v jakem sloupci se s indexem pracuje
correct_index(int index, int *p_end_index, int selected_col, int *p_correction){
    //korekce indexu pro sloupce od 2
    *p_correction = 1;
    *p_end_index = 0;
    //osetreni 1 sloupce + pokud je prazdny
    if(index == 0){
        if(user_params.line_data[0] == user_params.delim)
            return -1;
        *p_correction = 0;
    }

    if(selected_col == count_collumns()){
        if((index+1) == get_len(user_params.line_data))
            return -1;
        //osetreni posledniho sloupce
        *p_end_index = get_len(user_params.line_data);
    }
    else{
        *p_end_index = get_delim_index(selected_col+1);
    }
    return 0;
}
int
to_lower(int last_line){
    (void)last_line;

    int selected_col = atoi(user_params.arguments[0]);
    int index = get_delim_index(selected_col);
    int end_index, correction;

    if(correct_index(index, &end_index, selected_col, &correction) == -1)
        return 0;

    char sub_text[end_index-index+1];
    memset(sub_text, 0, sizeof sub_text);
    get_text(sub_text,index+correction,end_index);
    //uprava textu ve sloupci
    for(int i=0; i<get_len(sub_text); i++){
        sub_text[i]=tolower(sub_text[i]);
    }

    insert_text(sub_text,index+correction,end_index);

    return 0;
}
int
to_upper(int last_line){
    (void)last_line;

    int selected_col = atoi(user_params.arguments[0]);
    int index = get_delim_index(selected_col);
    int end_index, correction;

    if(correct_index(index, &end_index, selected_col, &correction) == -1)
        return 0;

    char sub_text[end_index-index+1];
    memset(sub_text, 0, sizeof sub_text);
    get_text(sub_text,index+correction,end_index);
    //uprava textu ve sloupci
    for(int i=0; i<get_len(sub_text); i++){
        sub_text[i]=toupper(sub_text[i]);
    }
    insert_text(sub_text,index+correction,end_index);

    return 0;
}
int
roundup(int last_line){
    (void)last_line;
    if(last_line)
        return -1;

    int selected_col = atoi(user_params.arguments[0]);
    int index = get_delim_index(selected_col);
    int end_index, correction;

    if(correct_index(index, &end_index, selected_col, &correction) == -1)
        return 0;

    char sub_text[end_index-index+1];
    memset(sub_text, 0, sizeof sub_text);
    get_text(sub_text,index+correction,end_index);
    //uprava textu ve sloupci
    char *pend;
    float f1 = strtof(sub_text, &pend);

    //TODO: osetrit pokud ve sloupci neni cislo

    sprintf(sub_text, "%d", (int)round(f1));

    insert_text(sub_text,index+correction,end_index);

    return 0;
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
    int min = -1, value=0;
    int cols = count_collumns();
    int column = atoi(user_params.arguments[0]);
    int col_one = atoi(user_params.arguments[1]);
    int col_last = atoi(user_params.arguments[2]);
    int sel_col = col_one;
    char min_char[ARG_LEN];

    if (col_last > cols || column > cols || col_one > col_last)
        return -1;

    do {
    cols = get_delim_index(sel_col);
    for (int i=sel_col+1; user_params.line_data[i]!=user_params.delim; i++){
            value += (int)user_params.line_data[i];
        }
    
    if (min == -1)
        min = value; 
    if (value<min)
        min = value;
    sel_col++;
    } while (sel_col <= col_last);

    cols = get_delim_index(column);
    sprintf(min_char,":%d:",min);

    check_for_space(strlen(min_char));
    insert_text(min_char, cols, cols+1);

    return 0;
}
int
cmax(int last_line){
    (void)last_line;
    int max = -1, value=0;
    int cols = count_collumns();
    int column = atoi(user_params.arguments[0]);
    int col_one = atoi(user_params.arguments[1]);
    int col_last = atoi(user_params.arguments[2]);
    int sel_col = col_one;
    char max_char[ARG_LEN];

    if (col_last > cols || column > cols || col_one > col_last)
        return -1;

    do {
    cols = get_delim_index(sel_col);
    for (int i=sel_col+1; user_params.line_data[i]!=user_params.delim; i++){
            value += (int)user_params.line_data[i];
        }
    
    if (max == -1)
        max = value; 
    if (value>max)
        max = value;
    sel_col++;
    } while (sel_col <= col_last);

    cols = get_delim_index(column);
    sprintf(max_char,":%d:",max);

    check_for_space(strlen(max_char));
    insert_text(max_char, cols, cols+1);

    return 0;
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
    int n, m;
    n = check_for_dash(user_params.line_selectors[0]);
    m = check_for_dash(user_params.line_selectors[1]);
    if (m == -1)
        m = user_params.line_number + 1;

    if (n==0 || m==0 || m<n){ printf("Invalid lines!\r\n");
        return -1;
    }

    if (n == -1){
        if (last_line)
            return 0;
        return 1;
    }

    if (n<=user_params.line_number && user_params.line_number<=m)
        return 0;

    if (last_line && m>user_params.line_number){
        printf("Last line reached, but you entered more lines!\n\r");
        return -1;
    }

    return -1;
}
int
beginswith(int last_line){
    (void)last_line;
    char *tmp_pt;
    int pos_of_str;

    int selected_col = atoi(user_params.line_selectors[0]);
    int cols = count_collumns();

    if(selected_col>cols)
        return -1;

    tmp_pt = strstr(user_params.line_data, user_params.line_selectors[1]);
    if (tmp_pt == NULL)
        return -1;

    pos_of_str = tmp_pt - user_params.line_data;

    cols = find_word_column(pos_of_str);
    if(cols != selected_col)
        return -1;

    /* Check if there is delimiter before string except the first one */
    if(pos_of_str != 0 && user_params.line_data[pos_of_str - 1] != user_params.delim)
        return -1;

    return 0;
}
int
contains(int last_line){
    (void)last_line;

    char *tmp_pt;
    int pos_of_str;

    int selected_col = atoi(user_params.line_selectors[0]);
    int cols = count_collumns();
    int last_column = cols;

    if(selected_col>cols)
        return -1;

    tmp_pt = strstr(user_params.line_data, user_params.line_selectors[1]);
    if (tmp_pt == NULL)
        return -1;

    pos_of_str = tmp_pt - user_params.line_data;
    cols = find_word_column(pos_of_str);

    if(cols != selected_col)
        return -1;

    /* Check if there is delimiter, with exception of first and last string */
    if((pos_of_str != 0 &&
      user_params.line_data[pos_of_str - 1] != user_params.delim) ||
      ((user_params.line_data[pos_of_str + strlen(user_params.line_selectors[1])]) != user_params.delim && 
      cols != last_column))
        return -1;
    
    if(pos_of_str == 0 && 
        user_params.line_data[pos_of_str + strlen(user_params.line_selectors[1])] != user_params.delim)  
        return -1;

    /* I can check for \0 without seg fault because if the STR given as parameter *
     * would be larger then the word it self, strstr wouldn't find it             */
    if(cols == last_column &&
        user_params.line_data[pos_of_str + strlen(user_params.line_selectors[1])] != '\0' )
        return -1;

    return 0;
}
int
main(int argc, char **argv){
    int opt, perim_chosen = 0, test_flag = 0, result = -1;
    int (*chosen_command)();
    int (*line_sel)();

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

        line_sel = find_line_sel();

        chosen_command = find_command();
        if (chosen_command == NULL){
            printf("Unrecognized command!\n\r");
            return -1;
        }

    } else {
        printf("Missing arguments\r\n");
        return -1;
    }

    if (test_flag){
        result = run_tests(chosen_command);
        return result;
    }

    if (line_sel == NULL){
        while (scan_input() != 1){
            if ((result = chosen_command(0)) != 0){
        printf("Command failed, check your inputs!\n");
                return -1;
        }
            printf("%s\n", user_params.line_data);
        }

        if ((result = chosen_command(1)) != 0){
        printf("Command failed, check your inputs!\n");
            return -1;
    }

    } else {
        separate_line_sel_from_args();
        while (scan_input() != 1){
            if (line_sel(0) == 0){
                if ((result = chosen_command(0)) != 0){
                printf("Command failed, check your inputs!\n");
                    return -1;
        }
            }
            printf("%s\n", user_params.line_data);
        }
        if (line_sel(1) == 0){
            if ((result = chosen_command(1)) != 0){
            printf("Command failed, check your inputs!\n");
                return -1;
        }
        }
    }

    printf("%s\n", user_params.line_data);
    return 0;
}
