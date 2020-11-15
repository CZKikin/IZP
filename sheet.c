#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define NUMBER_OF_COMMANDS (29)
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
int correct_index(int index, int *p_end_index, int selected_col, int *p_correction);
int get_text(char* sub_text, int start_index, int end_index);


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
int to_int(int last_line);

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
    "rcount",      /*27*/
    "int"	 /*28*/
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
   rcount,
   to_int
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

/*
 * Function: get_len
 * --------------------
 *  Vrátí délku pole
 *
 *  arr: Pole
 *
 *  returns: Délku pole
 *
 */
int 
get_len(char *arr){
    int char_count = 0;
    for(int i = 0; arr[i] != '\0'; i++){
            char_count++;
    }
    return char_count;
}
/*
 * Function: insert_text
 * --------------------
 *  Vloží zadaný text do user_params.line_data mezi 2 zadané indexy
 *
 *  text: Text který se má vložit
 *
 *  start_index: Počátek
 *
 *  end_index: Konec
 *
 *  returns: 0, při chybě -1
 *
 */
int
insert_text(char *text, int start_index, int end_index){

    int text_len = get_len(text);

    if(check_for_space(text_len) != 0)
        return -1;

    char start[start_index+1], end[get_len(user_params.line_data)-end_index+1];
    memset(start, 0, sizeof start);
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
/*
 * Function: validate_second_command
 * --------------------
 *  Zkontroluje, zda je přítomen druhý příkaz
 *
 *  returns: 0 pokud je přítomen, jinak -1
 *
 */
int
validate_second_command(){
    user_params.second_command = find_command();

    if (user_params.second_command == NULL)
        return -1;

    return 0;
}
/*
 * Function: check_for_dash
 * --------------------
 *  Zjistí, zda je v argumentu -
 *
 *  argument: String argumentu
 *
 *  returns: Číselnou hodnotu stringu, při nalezení pomlčky -1
 *
 */
int
check_for_dash(char *argument){
    if (argument[0] == '-')
        return -1;
    return atoi(argument);
}
/*
 * Function: find_word_column
 * --------------------
 *  Najde sloupec slova
 *
 *  pos_of_str: Index slova, jenž sloupec hledáme
 *
 *  returns: Sloupec ve kterém se slovo nachází
 *
 */
int
find_word_column(int pos_of_str){
    int cols = 1; /* because lol:lol are 2 cols.. */
    for(int i=0; i<pos_of_str; i++){
        if(user_params.line_data[i] == user_params.delim)
            cols++;
    }    
    return cols;
}
/*
 * Function: get_line_sel_pt
 * --------------------
 *  Vrátí pointer na fci line_selectoru
 *
 *  line_sel: String line select commandu
 *
 *  returns: Když najde line_selector, tak na něj vrátí pointer
 *           Když ne, vrátí NULL
 *
 */
int
(*get_line_sel_pt(char *line_sel))(){
    for (int i=0; i<NUMBER_OF_LINE_SELS; i++){
    if ((strcmp(line_sel, line_selector_commands[i])) == 0)
        return line_sels[i];
    }
    return NULL;
}
/*
 * Function: find_line_sel
 * --------------------
 *  Prohledá argumenty zadané při spuštění s cílem najít line_selector
 *
 *  returns: Pointer na fci line_selectoru, při nenalezení NULL
 *
 */
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
/*
 * Function: get_command_pt
 * --------------------
 *  Vrátí pointer na fci commandu
 *
 *  command: String commandu
 *
 *  returns: Když najde command, tak na něj vrátí pointer
 *           Když ne, vrátí NULL
 *
 */
int
(*get_command_pt(char *command))(){
    for (int i=0; i<NUMBER_OF_COMMANDS; i++){
        if (strcmp(command, commands[i]) == 0)
            return functions[i];
    }
    return NULL;
}
/*
 * Function: run_tests
 * --------------------
 *  Funkce sloužící k testování 
 *
 *  chosen_command: Zvolený příkaz 
 *
 *  returns: 0
 */
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
/*
 * Function: print_usage
 * --------------------
 *  Vypíše syntaxy spouštění
 */
void
print_usage(){
    printf("Usage: ./sheet -d <delim> <table command> | "\
           "-d <delim> <line select> <data command>\n\rFor testing: -t\r\n");
}
/*
 * Function:  scan_input
 * --------------------
 *  Načte soubor, zkontroluje buňky, sečte sloupce
 *  Zabraňuje přetečení bufferu při načítání
 *
 *  returns: 0, při posledním řádku 1, při chybě -1
 *
 */
int
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
/*
 * Function: check_for_space
 * --------------------
 *  Zjistí, zda je místo pro přidání stringu do user_params.line_data
 *
 *  size_needed: Potřebné místo pro uložení stringu
 *
 *  returns: při chybě -1
 *
 */
int
check_for_space(size_t size_needed){     /* -1 because indexing from 0 */
    if (user_params.line_data[(LINE_DATA_LEN - 1) - size_needed] != '\0')
        return -1;
    return 0;
}
/*
 * Function: count_columns
 * --------------------
 *  Vrátí počet sloupců v řádku
 *
 *  returns: počet sloupců
 */
int
count_collumns(){
    int count = 0;

    for(int i = 0; user_params.line_data[i] != '\0'; i++){
        if(user_params.line_data[i] == user_params.delim)
        {
            count++;
        }
    }
    return count+1;
}
/*
 * Function: find_command
 * --------------------
 *  Prohledá argumenty zadané při spuštění s cílem najít command
 *
 *  returns: Pointer na fci commandu, při nenalezení NULL
 *
 */
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
/*
 * Function: find_arguments
 * --------------------
 *  Prohledá argumenty v argv a zapíše je user_params.arguments
 */
void
find_arguments(int argc, char **argv){
    for (int i = optind, j = 0; i<argc; i++,j++){
        strncpy(user_params.arguments[j], argv[i], ARG_LEN);
    }
}
/*
 * Function: separate_line_sel_from_args
 * --------------------
 *  Prohledá argumenty a odseparuje argumenty pro line_selecor od argumentů
 */
void
separate_line_sel_from_args(){
    for(int i=0; i<2; i++)
        strncpy(user_params.line_selectors[i], user_params.arguments[i], ARG_LEN);

    for(int i=0,j=2; j<NUMBER_OF_ARGUMENTS; i++, j++)
        strncpy(user_params.arguments[i], user_params.arguments[j], ARG_LEN);
}
/*
 * Function: get_delim_index
 * --------------------
 *  Vrátí index n-tého delimiteru
 *
 *  order: číslo delimu
 *
 *  returns: Při chybě -1, jinak index n-tého delimu
 */
int
get_delim_index(int order){
    int appear = 0; 
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
/*
 * Function: get_text
 * --------------------
 *  Vrátí text mezi zadanými indexi
 *
 *  sub_text: String do kterého se uloží text
 *
 *  start_index: Začátek
 *
 *  end_index: Konec
 *
 *  returns: 0
 */
int
get_text(char* sub_text, int start_index, int end_index){

    for(int i = 0; i<end_index-start_index; i++){
        sub_text[i]=user_params.line_data[start_index+i];
    }
    return 0;
}
/*
 * Function: correct_index
 * --------------------
 *  Funkce upraví hodnotu indexu podle toho v jakém sloupci se s indexem pracuje
 *
 *  index: 
 *
 *  p_end_index:  
 *
 *  selected_col:
 *
 *  p_correction:
 *
 *  returns: Při chybě -1
 */
int
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
/*
 * Function: irow
 * --------------------
 *  Vloží řádek tabulky před určený řádek
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
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
/*
 * Function: drow
 * --------------------
 *  Odstraní řádek
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int //TODO: CHECK VSTUP DOPIČI
drow(int last_line){
    (void)last_line;

    int selected_row = atoi(user_params.arguments[0]);

    if(user_params.line_number == selected_row){

        user_params.line_data[0] = '\0';

    }

    return 0;
}
/*
 * Function: drows
 * --------------------
 *  Odstraní řádky
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
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
/*
 * Function: arow
 * --------------------
 *  Přidá řádek na konec souboru
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
arow(int last_line){
    if (!last_line){
        return 0;
    } else {

        if (check_for_space(user_params.delims_count+1) != 0)
            return -1;

        char str_for_strncat[2] = {user_params.delim, '\0'};
        strncat(user_params.line_data, "\n", 2);
        for (int i=0; i<user_params.delims_count; i++)
            strncat(user_params.line_data, str_for_strncat, 1);

    }
    return 0;
}
/*
 * Function: icol
 * --------------------
 *  Přidá sloupec před určený sloupec
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
icol(int last_line){
    (void)last_line;

    int edit_size = 1;

    if (check_for_space(edit_size) != 0)
        return -1;

    int index = get_delim_index(atoi(user_params.arguments[0]));

    if(index == -1)
        return -1;


    char arr[2];
    arr[0]=user_params.delim;
    insert_text(arr,index,index);
    return 0;
}
/*
 * Function: acol
 * --------------------
 *  Přidá prázdný sloupec za poslední sloupec
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
acol(int last_line){
    (void)last_line;
    int edit_size = 1;

    if (check_for_space(edit_size) != 0)
        return -1;

    if (last_line)
        return -1;

    user_params.line_data[get_len(user_params.line_data)]=user_params.delim;
    return 0;
}
/*
 * Function: dcol
 * --------------------
 *  Odstraní sloupec
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
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
/*
 * Function: dcols
 * --------------------
 *  Odstraní vybrané sloupce
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
dcols(int last_line){
    (void)last_line;

    if(last_line)
        return -1;

    int selected_col1 = atoi(user_params.arguments[0]);
    int selected_col2 = atoi(user_params.arguments[1]);

    if(selected_col1>selected_col2)
        return -1;

    if(selected_col2>count_collumns())
        return -1;

    int index = get_delim_index(selected_col1);
    int end_index = get_delim_index(selected_col2+1);

    insert_text("",index,end_index);

    return 0;
}
/*
 * Function: cset
 * --------------------
 *  Do vybrané buňky bude nastaven vybraný řetězec
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
cset(int last_line){
    (void)last_line;
    int selected_cell = atoi(user_params.arguments[0]);
    char str[ARG_LEN];
    int collumns;

    collumns = count_collumns();
    if((selected_cell > collumns)||(selected_cell < 1)){
	return -1; //kontrola vstupu
    }
    strncpy(str,user_params.arguments[1],ARG_LEN);
    //printf("cell %d string  %s\n", selected_cell, str);
    //printf("collumns %d\n", collumns);
    int start_index = get_delim_index(selected_cell) + 1; //od ktereho indexu zacina bunka kam chcu zapsat
    if(selected_cell == 1)
	    start_index --;
    //printf("start index %d\n", start_index);
    int end_index = get_delim_index(selected_cell+1);
    //printf("end index %d\n", end_index);
    insert_text(str, start_index, end_index);
    return 0;

}
/*
 * Function: to_lower
 * --------------------
 *  Řetězec ve vybraném sloupci bude převeden na malá písmena
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
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

    for(int i=0; i<get_len(sub_text); i++){
        sub_text[i]=tolower(sub_text[i]);
    }

    insert_text(sub_text,index+correction,end_index);

    return 0;
}
/*
 * Function: to_upper
 * --------------------
 *  Řetězec ve vybraném sloupci bude převeden na velká písmena
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
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

    for(int i=0; i<get_len(sub_text); i++){
        sub_text[i]=toupper(sub_text[i]);
    }
    insert_text(sub_text,index+correction,end_index);

    return 0;
}
/*
 * Function: roundup
 * --------------------
 *  Ve vybraném sloupci se zaokrouhlí číslo na celé číslo
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
roundup(int last_line){
    (void)last_line;

    int selected_col = atoi(user_params.arguments[0]);
    int index = get_delim_index(selected_col);
    int end_index;
    if(selected_col>count_collumns())
	return -1;
    if(selected_col==count_collumns()){
    	
	int end_of_line_index = 0; //kdeje /n
	for(int i = 0; i<LINE_DATA_LEN; i++){
	    if(user_params.line_data[i] == 0){
		 end_of_line_index=i;
	  	 break;
		 }	
        }
    	end_index=end_of_line_index;

    }else{
    	end_index=get_delim_index(selected_col+1);
    }
    char sub_text[end_index-index+1];
    memset(sub_text, 0, sizeof sub_text);
    
    if(selected_col==1){
	    get_text(sub_text,index,end_index);
    }else{
    get_text(sub_text,index+1,end_index);
    }
    char *pend;
    float f1 = strtof(sub_text, &pend);
    //osetreni: neni cislo nebo prazdny sloupec
    
    if(get_len(pend)!=0)
	return 0;

    if(get_len(sub_text)==0)
	return 0;

    sprintf(sub_text, "%d", (int)(f1+0.5));
    if(selected_col==1)
	index--;
    insert_text(sub_text,index+1,end_index);
    return 0;
}
/*
 * Function: copy
 * --------------------
 *  Přepíše obsah buněk ve vybraném sloupci hodnotami ze jiného sloupce
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
copy(int last_line){ 
    (void)last_line;
    int n_col = atoi(user_params.arguments[0]);
    int m_col = atoi(user_params.arguments[1]);
    int collumns = count_collumns();
    if((n_col == m_col) || (n_col == 0) || (m_col == 0) || (collumns < n_col) || (collumns < m_col))
	return -1;
    //printf("delimindex %d\n", get_delim_index(n_col));
    
    if(n_col == collumns){ //osetreni pokud N==collums 

	int end_of_line_index = 0; //kdeje /n
	for(int i = 0; i<LINE_DATA_LEN; i++){
	    if(user_params.line_data[i] == 0){
		 end_of_line_index=i;
	  	 break;
		 }	
        }

    	int n_end_index = end_of_line_index;
	
        int n_start_index = get_delim_index(n_col) + 1;
        if(n_col ==1)
   	    n_start_index = 0;
    
        int m_start_index = get_delim_index(m_col) + 1;
        if(m_col ==1)
	    m_start_index = 0;

        int m_end_index = get_delim_index(m_col+1) + 1;
    
        char n_text[n_end_index-n_start_index+1];
        memset(n_text, 0, sizeof n_text);
        get_text(n_text,n_start_index,n_end_index);
        char m_text[m_end_index-m_start_index + 1];
        memset(m_text, 0, sizeof m_text);
        get_text(m_text,m_start_index,m_end_index-1);
        //printf("NTEXT %s MTEXT %s\n", n_text, m_text);
        insert_text(n_text, m_start_index, m_end_index-1);
        return 0;
    }else if(m_col == collumns){ //osetreni pokud je M==collumns

	int end_of_line_index = 0; //kdeje /n
        for(int i = 0; i<LINE_DATA_LEN; i++){
    	    if(user_params.line_data[i] == 0){
	   	 end_of_line_index=i;
	   	 break;
	   	 }	
	}

    	int m_end_index = end_of_line_index;
	
        int n_start_index = get_delim_index(n_col) + 1;
        if(n_col ==1)
	    n_start_index = 0;

        int n_end_index = get_delim_index(n_col+1) + 1;
        int m_start_index = get_delim_index(m_col) + 1;
        if(m_col ==1)
	    m_start_index = 0;
	
        char n_text[n_end_index-n_start_index+1];
        memset(n_text, 0, sizeof n_text);
        get_text(n_text,n_start_index,n_end_index-1);
        char m_text[m_end_index-m_start_index + 1];
        memset(m_text, 0, sizeof m_text);
        get_text(m_text,m_start_index,m_end_index);
        //printf("NTEXT %s MTEXT %s\n", n_text, m_text);
        insert_text(n_text, m_start_index, m_end_index);
        return 0;
    }else {
    	int n_start_index = get_delim_index(n_col) + 1;
    	if(n_col ==1)
	    n_start_index = 0;

    	int n_end_index = get_delim_index(n_col+1) + 1;
    	int m_start_index = get_delim_index(m_col) + 1;
    	if(m_col ==1)
	    m_start_index = 0;

    	int m_end_index = get_delim_index(m_col+1) + 1;
    	char n_text[n_end_index-n_start_index+1];
    	memset(n_text, 0, sizeof n_text);
    	get_text(n_text,n_start_index,n_end_index-1);
    	char m_text[m_end_index-m_start_index + 1];
    	memset(m_text, 0, sizeof m_text);
    	get_text(m_text,m_start_index,m_end_index-1);
    	//printf("NTEXT %s MTEXT %s\n", n_text, m_text);
    	insert_text(n_text, m_start_index, m_end_index-1);
    	return 0;
    }
}
/*
 * Function: swap
 * --------------------
 *  Zamění hodnoty buněk ve vybraných sloupcích
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
swap(int last_line){
    (void)last_line;
    
    int n_col = atoi(user_params.arguments[0]);
    int m_col = atoi(user_params.arguments[1]);
    int collumns = count_collumns();
    if((n_col == m_col) || (n_col == 0) || (m_col == 0) || (collumns < n_col) || (collumns < m_col))
	return -1;
    //printf("delimindex %d\n", get_delim_index(n_col));
    
    if(n_col == collumns){ //osetreni pokud N==collums 

	int end_of_line_index = 0; //kdeje /n
	for(int i = 0; i<LINE_DATA_LEN; i++){
	    if(user_params.line_data[i] == 0){
		 end_of_line_index=i;
	  	 break;
		 }	
        }

    	int n_end_index = end_of_line_index;
	
        int n_start_index = get_delim_index(n_col) + 1;
        if(n_col ==1)
   	    n_start_index = 0;
    
        int m_start_index = get_delim_index(m_col) + 1;
        if(m_col ==1)
	    m_start_index = 0;

        int m_end_index = get_delim_index(m_col+1) + 1;
    
        char n_text[n_end_index-n_start_index+1];
        memset(n_text, 0, sizeof n_text);
        get_text(n_text,n_start_index,n_end_index);
        char m_text[m_end_index-m_start_index + 1];
        memset(m_text, 0, sizeof m_text);
        get_text(m_text,m_start_index,m_end_index-1);
        insert_text(n_text, m_start_index, m_end_index-1);
	//osetreni pokud je N>M ...nove indexy
        int n_n_start_index = get_delim_index(n_col) + 1;
        if(n_col ==1)
   	    n_n_start_index = 0;
	int new_end_of_line_index = 0; //kde je /n (zisk indexu konce radku)
	for(int i = 0; i<LINE_DATA_LEN; i++){
	    if(user_params.line_data[i] == 0){
		 new_end_of_line_index=i;
	  	 break;
		 }	
        }
	int n_n_end_index = new_end_of_line_index;

	
        insert_text(m_text, n_n_start_index, n_n_end_index);
	return 0;

    }else if(m_col == collumns){ //osetreni pokud je M==collumns

	int end_of_line_index = 0; //kde je /n (zisk indexu konce radku)
        for(int i = 0; i<LINE_DATA_LEN; i++){
    	    if(user_params.line_data[i] == 0){
	   	 end_of_line_index=i;
	   	 break;
	   	 }	
	}

    	int m_end_index = end_of_line_index;
	
        int n_start_index = get_delim_index(n_col) + 1;
        if(n_col ==1) //pohlidani prvnich bunek radku
	    n_start_index = 0;

        int n_end_index = get_delim_index(n_col+1) + 1;
        int m_start_index = get_delim_index(m_col) + 1;
        if(m_col ==1)
	    m_start_index = 0;
	
        char n_text[n_end_index-n_start_index+1];
        memset(n_text, 0, sizeof n_text);
        get_text(n_text,n_start_index,n_end_index-1);
        char m_text[m_end_index-m_start_index + 1];
        memset(m_text, 0, sizeof m_text);
        get_text(m_text,m_start_index,m_end_index);
        insert_text(n_text, m_start_index, m_end_index);
        insert_text(m_text, n_start_index, n_end_index-1);
        return 0;
    }else if(n_col > m_col){ //pokud je N>M indexy
    	int n_start_index = get_delim_index(n_col) + 1;
    	if(n_col ==1)
	    n_start_index = 0;

    	int n_end_index = get_delim_index(n_col+1) + 1;
    	int m_start_index = get_delim_index(m_col) + 1;
    	if(m_col ==1)
	    m_start_index = 0;

    	int m_end_index = get_delim_index(m_col+1) + 1;
    	char n_text[n_end_index-n_start_index+1];
    	memset(n_text, 0, sizeof n_text);
    	get_text(n_text,n_start_index,n_end_index-1);
    	char m_text[m_end_index-m_start_index + 1];
    	memset(m_text, 0, sizeof m_text);
    	get_text(m_text,m_start_index,m_end_index-1);
        insert_text(n_text, m_start_index, m_end_index-1);
	
	//osetreni pokud je N>M ...nove indexy
        int n_n_start_index = get_delim_index(n_col) + 1;
        if(n_col ==1)
   	    n_n_start_index = 0;
	int n_n_end_index = n_n_start_index + get_len(n_text);
        insert_text(m_text, n_n_start_index, n_n_end_index);
    	return 0;
    }else {
    	int n_start_index = get_delim_index(n_col) + 1;
    	if(n_col ==1)
	    n_start_index = 0;

    	int n_end_index = get_delim_index(n_col+1) + 1;
    	int m_start_index = get_delim_index(m_col) + 1;
    	if(m_col ==1)
	    m_start_index = 0;

    	int m_end_index = get_delim_index(m_col+1) + 1;
    	char n_text[n_end_index-n_start_index+1];
    	memset(n_text, 0, sizeof n_text);
    	get_text(n_text,n_start_index,n_end_index-1);
    	char m_text[m_end_index-m_start_index + 1];
    	memset(m_text, 0, sizeof m_text);
    	get_text(m_text,m_start_index,m_end_index-1);
      	insert_text(n_text, m_start_index, m_end_index-1);
        insert_text(m_text, n_start_index, n_end_index-1);
    	return 0;
    }

}
/*
 * Function: move
 * --------------------
 *  Přesune vybraná sloupec před druhý sloupec
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
move(int last_line){
    (void)last_line;
    
    int n_col = atoi(user_params.arguments[0]);
    int m_col = atoi(user_params.arguments[1]);
    int collumns = count_collumns();
    char delim_str[2]={user_params.delim,'\0'};
   
   
    if((n_col == m_col) || (n_col == 0) || (m_col == 0) || (collumns < n_col) || (collumns < m_col)) //osetreni vstupu
	return -1;
    int n_start_index = get_delim_index(n_col) + 1;
    if(n_col ==1)
	n_start_index = 0;

    if(n_col==collumns){ //osetreni pokud je n_coll na konci radku
	int end_of_line_index = 0; //kde je \n (index konce radku)
        for(int i = 0; i<LINE_DATA_LEN; i++){
    	    if(user_params.line_data[i] == 0){
	   	 end_of_line_index=i;
	   	 break;
	   	 }	
	}
    	
    	int m_index = get_delim_index(m_col) + 1;
    	if(m_col ==1) //osetreni indexu pokud je m_col 1.bunka
	    m_index = 0;
    	char n_text[end_of_line_index-n_start_index+1];
    	memset(n_text, 0, sizeof n_text);
    	get_text(n_text,n_start_index,end_of_line_index); //zisk textu z bunky
        insert_text(delim_str, m_index, m_index); //vkladani textu do bunky
    	insert_text(n_text, m_index, m_index);

    	int n_text_size = get_len(n_text); //delka textu
	int new_end_of_line_index = 0; //kde je /n (novy index konce radku)
        for(int i = 0; i<LINE_DATA_LEN; i++){
    	    if(user_params.line_data[i] == 0){
	   	 new_end_of_line_index=i;
	   	 break;
	   	 }	
	}
    	insert_text("", (new_end_of_line_index-n_text_size)-1, new_end_of_line_index);//prepis stare bunky
	return 0;
    }else{
    	int n_end_index = get_delim_index(n_col+1) + 1;
    	int m_index = get_delim_index(m_col) + 1;
    	if(m_col ==1)
	    m_index = 0;
    	char n_text[n_end_index-n_start_index+1];
    	memset(n_text, 0, sizeof n_text);
    	get_text(n_text,n_start_index,n_end_index);
    	insert_text(n_text, m_index, m_index);
    	int n_text_size = get_len(n_text);
    	int delim_index = n_start_index + n_text_size;
   
    	if(n_col<m_col){ //pohlidani indexu
    	    insert_text("", n_start_index, n_end_index);
	    return 0;
    	}else{
    	    insert_text("", delim_index, (delim_index + n_text_size));
   	    return 0;
   	}
    }	
}
/*
 * Function: csum 
 * --------------------
 *  Do vybrané buňky C bude uložena suma čísel od sloupce N po sloupec M na stejném řádku
 *  
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
csum(int last_line){
    (void)last_line;
    
    int c_col = atoi(user_params.arguments[0]);
    int n_par = atoi(user_params.arguments[1]);
    int m_par = atoi(user_params.arguments[2]);
    int collumns = count_collumns();
    int n_to_m = 0;
    int cell_start_index;
    int cell_end_index;
    int step_add=0;
    double final_number=0;
    char char_final_number[ARG_LEN];
   
    int end_of_line_index=0;
    if((n_par==m_par)||(n_par==c_col)||(m_par==c_col)||(n_par==0)||(m_par==0)||(c_col==0)) //osetreni vstupu
	return -1;
    if((collumns<n_par)||(collumns<m_par)||(collumns<c_col)||(n_par>m_par))
	return -1;
    if((c_col<m_par)&&(c_col>n_par))
	return -1;

    int c_col_start_index = get_delim_index(c_col) + 1; //zisk indexu
        if(c_col ==1)
   	    c_col_start_index = 0;

    int c_col_end_index = get_delim_index(c_col+1) + 1;

    for(int i=n_par;i<m_par;i++){ //pocet skoku
	n_to_m ++;
    }
    for(int i=-1;i<n_to_m;i++){
    	
        cell_start_index = get_delim_index(n_par+step_add) + 1;
        if(n_par ==1 && i==-1)
   	    cell_start_index = 0;
	
	if(m_par==collumns && i+1==n_to_m){ //osetreni pro posledni bunku, pokud je na konci radku
	    end_of_line_index = 0; //kde je /n (zjisteni end indexu radku)
            for(int i = 0; i<LINE_DATA_LEN; i++){
    	        if(user_params.line_data[i] == 0){
	   	     end_of_line_index=i;
	   	     break;
	   	 }	
	    }
   	    char cell_text[end_of_line_index-cell_start_index+1];
    	    memset(cell_text, 0, sizeof cell_text);
    	    get_text(cell_text,cell_start_index,end_of_line_index); //zisk textu z bunky
	    step_add++;
	    if(get_len(cell_text)==0) //osetreni prazdneho cellu
	        continue;
	    
	    char *end_ptr;
	    double cell_number=strtod(cell_text,&end_ptr);
	    if (get_len(end_ptr)==0)
	    	final_number=final_number+cell_number;
    	}else{
	    cell_end_index = get_delim_index(n_par+1+step_add) + 1;
    	    char cell_text[cell_end_index-cell_start_index+1];
    	    memset(cell_text, 0, sizeof cell_text);
    	    get_text(cell_text,cell_start_index,cell_end_index-1);
	    step_add++;
	    if(get_len(cell_text)==0) //osetreni prazdneho cellu
	        continue;
	    char *end_ptr;
	    double cell_number=strtod(cell_text,&end_ptr);
	    if(get_len(end_ptr)==0)
		 final_number=final_number+cell_number;
    	}
    }
	
	if(final_number==0)
	   return 0;
	sprintf(char_final_number, "%f", final_number); //prevod na char
	insert_text(char_final_number, c_col_start_index, c_col_end_index-1);

    return 0;
}
/*
 * Function: cavg 
 * --------------------
 *  Do vybrané buňky C bude uložena aritmetický průměr čísel od sloupce N po sloupec M na stejném řádku
 *  
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
cavg(int last_line){
    (void)last_line;
    int c_col = atoi(user_params.arguments[0]);
    int n_par = atoi(user_params.arguments[1]);
    int m_par = atoi(user_params.arguments[2]);
    int collumns = count_collumns();
    int n_to_m = 0;
    int cell_start_index;
    int cell_end_index;
    int step_add=0;
    double final_number=0;
    char char_final_number[ARG_LEN];
    int devider =0;
    int end_of_line_index=0;
    if((n_par==m_par)||(n_par==c_col)||(m_par==c_col)||(n_par==0)||(m_par==0)||(c_col==0)) //osetreni vstupu
	return -1;
    if((collumns<n_par)||(collumns<m_par)||(collumns<c_col)||(n_par>m_par))
	return -1;
    if((c_col<m_par)&&(c_col>n_par))
	return -1;

    int c_col_start_index = get_delim_index(c_col) + 1; //zisk indexu
        if(c_col ==1)
   	    c_col_start_index = 0;

    int c_col_end_index = get_delim_index(c_col+1) + 1;

    for(int i=n_par;i<m_par;i++){ //pocet skoku
	n_to_m ++;
    }
    for(int i=-1;i<n_to_m;i++){ //poslupne prochazeni bunek od N do M
    	
        cell_start_index = get_delim_index(n_par+step_add) + 1;
        if(n_par ==1 && i==-1) //osetreni pro index 1.bunky
   	    cell_start_index = 0;
	
	if(m_par==collumns && i+1==n_to_m){ //osetreni pro posledni bunku, pokud je na konci radku
	    end_of_line_index = 0; //kde je /n (zjisteni indexu konce radku)
            for(int i = 0; i<LINE_DATA_LEN; i++){
    	        if(user_params.line_data[i] == 0){
	   	     end_of_line_index=i;
	   	     break;
	   	 }	
	    }
   	    char cell_text[end_of_line_index-cell_start_index+1];
    	    memset(cell_text, 0, sizeof cell_text);
    	    get_text(cell_text,cell_start_index,end_of_line_index);
	    step_add++;
	    if(get_len(cell_text)==0) //osetreni prazdneho cellu
	        continue;
	    
	    char *end_ptr;
	    double cell_number=strtod(cell_text,&end_ptr);
	    if (get_len(end_ptr)==0){
	    	final_number=final_number+cell_number; // pricitani bunky k vysledku
	        devider++;
	    }
    	}else{
	    cell_end_index = get_delim_index(n_par+1+step_add) + 1;
    	    char cell_text[cell_end_index-cell_start_index+1];
    	    memset(cell_text, 0, sizeof cell_text);
    	    get_text(cell_text,cell_start_index,cell_end_index-1);
	    step_add++;
	    if(get_len(cell_text)==0) //osetreni prazdneho cellu
	        continue;
	    char *end_ptr;
	    double cell_number=strtod(cell_text,&end_ptr);
	    if(get_len(end_ptr)==0){
		 final_number=final_number+cell_number; //pricitani bunky k vysledku
	         devider++;
	    }
    	}
    }
	if(final_number==0) //pokud je vysledek 0 tak se nezapise
	   return 0;
	final_number=final_number/devider; //deleni vysledku poctem prictenych bunek
	sprintf(char_final_number, "%f", final_number); //prevod na char
	insert_text(char_final_number, c_col_start_index, c_col_end_index-1); //vypis do C bunky

    return 0;

 

}
/*
 * Function: cmin 
 * --------------------
 *  Do vynbrané buňky bude uloženo číslo reprezentující min hodnotu buněk na stejném řádku
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
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
/*
 * Function: cmax 
 * --------------------
 *  Do vynbrané buňky bude uloženo číslo reprezentující max hodnotu buněk na stejném řádku
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
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
/*
 * Function: to_int
 * --------------------
 *  Odstraní desetinnou část čísla z vybraného sloupce
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Při chybě -1
 */
int
to_int(int last_line){
    
    (void)last_line;

    int selected_col = atoi(user_params.arguments[0]);
    int index = get_delim_index(selected_col);
    int end_index;
    if(selected_col>count_collumns())
	return -1;
    if(selected_col==count_collumns()){
    	
	int end_of_line_index = 0; //kdeje /n
	for(int i = 0; i<LINE_DATA_LEN; i++){
	    if(user_params.line_data[i] == 0){
		 end_of_line_index=i;
	  	 break;
		 }	
        }
    	end_index=end_of_line_index;

    }else{
    	end_index=get_delim_index(selected_col+1);
    }
    char sub_text[end_index-index+1];
    memset(sub_text, 0, sizeof sub_text);
    if(selected_col==1){
	    get_text(sub_text,index,end_index);
    }else{
    get_text(sub_text,index+1,end_index);
    }

    char *pend;
    float f1 = strtof(sub_text, &pend);
    int ffinal = ((int)(f1*10))/10.0;
    //osetreni: neni cislo nebo prazdny sloupec
    
   
    if(get_len(pend)!=0)
	return 0;

    if(get_len(sub_text)==0)
	return 0;

    sprintf(sub_text, "%d", ffinal);
    if(selected_col==1)
	index--;
    insert_text(sub_text,index+1,end_index);
    return 0;
}
/*
 * Function: rows 
 * --------------------
 *  Upřesní které řádky budou upraveny 
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Vrací 0 když se má řádek upravit, jinak -1 
 */
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
        return -1;
    }

    if (n<=user_params.line_number && user_params.line_number<=m)
        return 0;

    if (last_line && m>user_params.line_number){
        printf("Last line reached, but you entered more lines!\n\r");
        return -1;
    }

    return -1;
}
/*
 * Function: beginswith 
 * --------------------
 *  Procesor bude zpracovávat pouze ty řádky, jejichž obsah buňky ve vybraném sloupci začíná vybraným řetězcem
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Vrací 0 když se má řádek upravit, jinak -1 
 */
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
/*
 * Function: contains 
 * --------------------
 *  Procesor bude zpracovávat pouze ty řádky, jejichž buňky ve vybraném sloupci obsahují vybraný řetězec
 *
 *  last_line: Indikátor posledního řádku
 *
 *  returns: Vrací 0 když se má řádek upravit, jinak -1 
 */
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
        if (validate_second_command() == 0){
            if(validate_second_command() == 0){
                printf("This version supports only 2 commands\n\r"\
                        "For sequential edits make a scipt...");
            }
        }
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
            if(user_params.second_command != NULL){
                if((result = user_params.second_command(0)) != 0){
                    printf("Second command failed, check your inputs!\r\n");
                    return -1;
                }
            }
            printf("%s\n", user_params.line_data);
        }

        if ((result = chosen_command(1)) != 0){
        printf("Command failed, check your inputs!\n");
            return -1;
        }

        if(user_params.second_command != NULL){
                if((result = user_params.second_command(0)) != 0){
                    printf("Second command failed, check your inputs!\r\n");
                    return -1;
                }
        }
        //No time to edit... worst code I have written in my 4 years of programming
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
