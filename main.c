#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "helper_windows.h"
#include "colorize.h"
#include "conio_wherexy.h"
#include <time.h>
#pragma warning(disable:4996)//disables visual studio errors about scanf and some other functions being unsafe

#define I 25//line number
#define J 26//column number

#define EASY_GAME_TIME 20// easy game wave time
#define MEDIUM_GAME_TIME 15// medium game wave time
#define HARD_GAME_TIME 10 // hard game wave time
#define ONE_HAND_GAME_TIME 15 //one hand game wave time

#define WAVE_TIME_TO_WIN 5000

#define MIN_LENGTH_OF_NORMAL_WORD 4
#define MAX_LENGTH_OF_NORMAL_WORD 7

#define MIN_LENGTH_OF_LONG_WORD 9
#define MAX_LENGTH_OF_LONG_WORD 17

#define MIN_LENGTH_OF_HARD_WORD 6
#define MAX_LENGTH_OF_HARD_WORD 12

#define MIN_LENGTH_OF_ONE_HAND_WORD 6
#define MAX_LENGTH_OF_ONE_HAND_WORD 10



typedef struct gameinfo {
	int xp, level_difficulty, high_score;
	//time:
	int year, month, day, hour, minute, second;

	bool is_active;
}gameinfo;

typedef struct user {
	char username[20];
	unsigned long password;
	gameinfo game[3];
	
}user;

typedef struct Node {
	char* str;
	struct Node* next, * prev;
} Node;

char index[25];
bool is_solved = false;
int index_line, color[25], score, ind = 1;//color[x]=  0->green 1->yellow 2->black
bool* ocpl_copy;
Node* global_node;
bool End;

/*Example codes.
setcolor(1);
gotoxy(0, 5);
system("cls");
*/




void my_callback_on_key_arrival(char c);
int char_index();
bool is_word_solved();
void reset_color_array();
void print_frame(int _i, int _j);
void print_frame2(int _i, int _j);
void welcome(bool* ocpl);
int	 l_padding(char str[I], int j);
void ocpl_clean(bool* ocpl);
void ocpl_purge_all();
void account_menu(bool* ocpl, FILE* usr_inf, user* user_struct);
void sign_up(bool* ocpl, FILE* usr_inf);
bool log_in(bool* ocpl, FILE* usr_inf, user* user_struct);
void countdown(int x, int y, int sec);
void print(char* str, char* padding, int y, bool* ocpl, bool a, int j);
void get_pass(char* password);
void get_user(char* username);
bool is_user_unique(FILE* usr_inf, char* username);
bool does_username_exist_and_get_user_struct(FILE* usr_inf, char* username, user* user_struct);
int  main_menu(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words);
void change_password(bool* ocpl, FILE* usr_inf, user* user_struct);
void hide_cursor(void);
void show_cursor(void);
unsigned long hash(char* s, char* username);
void fill(FILE** file);
char* fill_norm(void);
char* fill_long(void);
char* fill_hard(void);
char* fill_left(void);
char* fill_right(void);
void newGame_menu(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words);
void color_print(char* str, char* padding, int y, bool* ocpl, bool a, int j, int sabz, int zard_begin, int zard_end);
void color_print2(char* str, int y, int j);
Node* create_Node();
void make_linked_list(Node** head, Node** tail);
void kill_linked_list(Node** head, Node** tail);
void fill_one_node(Node* head, FILE* words);
void fill_Linked_list(Node* head, int wave, FILE** words);
void fill_linked_list_one_hand(Node* head, FILE* words);
void easy_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
void medium_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
void hard_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
void left_hand_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
void right_hand_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
bool do_wave(bool* ocpl, int wave_time, Node* head, int* f_score);
void finish(bool win, int score, FILE* usr_inf, user* user_struct, bool* ocpl, int save_in_slot, bool update, int level_num,int wave);



int main()
{
	srand(time(NULL));
	hide_cursor();
	printf("\033[;47m");//change background color to white
	setcolor(0);//0-black 1-bit.Dark.Blue 2-dark.green 3-dark.cyan 4-red 5-dark.purple 6-orange 7-white 8-grey 9-blue 10-green 11-cyan 12-light.red 
	//13-purple 14-very.light.orange 

	system("cls");




	reset_color_array();


	user user_struct;
	bool ocpl[35];//occupied_lines
	ocpl_copy = ocpl;
	//file section

	FILE* words[5]; // 0 normal 1 long 2 hard 3 left 4 right

	words[0] = fopen("normal_words.txt", "w+");
	words[1] = fopen("long_words.txt", "w+");
	words[2] = fopen("hard_words.txt", "w+");
	words[3] = fopen("left_words.txt", "w+");
	words[4] = fopen("right_words.txt", "w+");

	if (words[0] == NULL || words[1] == NULL || words[2] == NULL || words[3] == NULL || words[4] == NULL) {
		fprintf(stderr, "word files didn't open properly.");
		fcloseall();
		return 1;
	}

	FILE* usr_inf = fopen("user_info.bin", "rb+");

	if (usr_inf == NULL || fseek(usr_inf, 0, SEEK_SET) != 0) {
		fprintf(stderr, "user_info file didn't open properly.");
		fcloseall();
		return 1;
	}
	//////file section end

	fill(words);



	print_frame(I, J);

	welcome(ocpl);//welcome page

	getch();//wait for any key to be pressed
	//HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	do {
		account_menu(ocpl, usr_inf, &user_struct);
	} while (main_menu(ocpl, usr_inf, &user_struct, words) == 1);




	//WaitForSingleObject(thread_id, INFINITE);
	return 0;
}


Node* create_Node() {
	Node* x;
	x = (Node*)malloc(sizeof(Node));
	if (x == NULL) {
		gotoxy(0, 0);
		system("cls");
		fcloseall();
		fprintf(stderr, "node memory allocation failed!");
		exit(1);
	}

	x->str = NULL;
	x->next = NULL;
	x->prev = NULL;
	return x;//returns address to this node
}

void make_linked_list(Node** head, Node** tail) {
	Node* temp_pointer;

	(*head) = create_Node();
	(*tail) = (*head);

	for (int i = 0; i < 10; i++) {
		temp_pointer = create_Node();
		temp_pointer->prev = (*tail);//new node's prev will be the last node
		(*tail)->next = temp_pointer;//last node's next will be new node
		(*tail) = temp_pointer;//new node is now the last node
	}
	return;
}

void kill_linked_list(Node** head, Node** tail) {
	for (int i = 0; i < 10; i++) {
		free((*tail)->str);
		(*tail) = (*tail)->prev;
		free((*tail)->next);
	}
	tail = head = NULL;
	return;
}

void fill_one_node(Node* head, FILE* words) {
	char buffer[25];
	Node* temp_node = head;

	while (temp_node->str != NULL)
		temp_node = temp_node->next;


	fgets(buffer, 25, words);//getting a word from file



	buffer[strlen(buffer) - 1] = '\0';//remove \n from end of buffer

	temp_node->str = (char*)malloc(strlen(buffer) + 1);

	if (temp_node->str == NULL) {
		gotoxy(0, 0);
		system("cls");
		fcloseall();
		fprintf(stderr, "node str memory allocation failed!");
		exit(1);
	}

	strcpy(temp_node->str, buffer);//copying the word in a node



	return;

}

void fill_linked_list_one_hand(Node* head, FILE* words) {
	
	for (int i = 0; i < 10; i++)
		fill_one_node(head, words);

}

void fill_Linked_list(Node* head, int wave, FILE** words) {
	int nrm, lng, hrd, ctr = 0;
	switch (wave)
	{
	case 1:
		nrm = 9; lng = 0; hrd = 1;

		for (int i = 0; i < 10; i++, ctr++) {
			if (hrd && ctr >= 5 && (rand() % (10 - ctr)) == 0) {
				fill_one_node(head, words[2]);
				hrd--;
			}
			else {
				fill_one_node(head, words[0]);
			}

		}

		break;
	case 2:
		nrm = 8; lng = 1; hrd = 1;

		for (int i = 0; i < 10; i++, ctr++) {
			if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && hrd)) {// if the chance is right
				fill_one_node(head, words[2]);
				hrd--;
			}
			else if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && lng)) {
				fill_one_node(head, words[1]);
				lng--;
			}
			else if (nrm) {
				fill_one_node(head, words[0]);
			}

		}

		break;
	case 3:
		nrm = 7; lng = 2; hrd = 1;

		for (int i = 0; i < 10; i++, ctr++) {
			if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && hrd)) {// if the chance is right
				fill_one_node(head, words[2]);
				hrd--;
			}
			else if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && lng)) {
				fill_one_node(head, words[1]);
				lng--;
			}
			else if (nrm) {
				fill_one_node(head, words[0]);
			}

		}

		break;
	case 4:
		nrm = 6; lng = 2; hrd = 2;

		for (int i = 0; i < 10; i++, ctr++) {
			if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && hrd)) {// if the chance is right
				fill_one_node(head, words[2]);
				hrd--;
			}
			else if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && lng)) {
				fill_one_node(head, words[1]);
				lng--;
			}
			else if (nrm) {
				fill_one_node(head, words[0]);
			}

		}

		break;
	case 5:
		nrm = 5; lng = 3; hrd = 2;

		for (int i = 0; i < 10; i++, ctr++) {
			if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && hrd)) {// if the chance is right
				fill_one_node(head, words[2]);
				hrd--;
			}
			else if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && lng)) {
				fill_one_node(head, words[1]);
				lng--;
			}
			else if (nrm) {
				fill_one_node(head, words[0]);
			}

		}

		break;

	default:
		nrm = 4; lng = 3; hrd = 3;

		for (int i = 0; i < 10; i++, ctr++) {
			if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && hrd)) {// if the chance is right
				fill_one_node(head, words[2]);
				hrd--;
			}
			else if ((hrd && (rand() % (10 - ctr)) == 0) || (!nrm && lng)) {
				fill_one_node(head, words[1]);
				lng--;
			}
			else if (nrm) {
				fill_one_node(head, words[0]);
			}

		}
		break;
	}
}


char* fill_norm(void) {
	char* word;
	int max_size = MAX_LENGTH_OF_NORMAL_WORD;
	int min_size = MIN_LENGTH_OF_NORMAL_WORD;
	int word_size = rand() % (max_size + 1);
	while (word_size < min_size) {
		word_size += rand() % (max_size - min_size + 2);
	}
	word = (char*)malloc(sizeof(char) * word_size);
	if (word == NULL) {
		fprintf(stderr, "word alloc failed");
		exit(1);
	}

	int r;
	for (int i = 0; i < word_size; i++) {
		do {
			r = rand() % 127;
		} while (!((r >= 48 && r <= 57) || (r >= 65 && r <= 90) || (r >= 97 && r <= 122)));
		word[i] = r;

	}
	word[word_size] = '\0';

	return word;
}



char* fill_long(void) {
	char* word;
	int max_size = MAX_LENGTH_OF_LONG_WORD;
	int min_size = MIN_LENGTH_OF_LONG_WORD;
	int word_size = rand() % (max_size + 1);
	while (word_size < min_size) {
		word_size += rand() % (max_size - min_size + 2);
	}
	word = (char*)malloc(sizeof(char) * word_size);

	if (word == NULL) {
		fprintf(stderr, "word alloc failed");
		exit(1);
	}

	char r;
	for (int i = 0; i < word_size; i++) {
		do {
			r = rand() % 125;
		} while (!((r >= 48 && r <= 57) || (r >= 65 && r <= 90) || (r >= 97 && r <= 122)));
		word[i] = r;

	}
	word[word_size] = '\0';

	return word;
}

char* fill_hard(void) {
	char* word;
	int max_size = MAX_LENGTH_OF_HARD_WORD;
	int min_size = MIN_LENGTH_OF_HARD_WORD;
	int word_size = rand() % (max_size + 1);
	while (word_size < min_size) {
		word_size += rand() % (max_size - min_size + 2);
	}
	word = (char*)malloc(sizeof(char) * word_size);
	if (word == NULL) {
		fprintf(stderr, "word alloc failed");
		exit(1);
	}

	int r;
	for (int i = 0; i < word_size; i++) {
		do {
			r = rand() % 128;
		} while (!(r >= 33 && r <= 126));
		word[i] = r;

	}
	word[word_size] = '\0';

	return word;
}

char* fill_left(void) {
	char* word;
	int max_size = MAX_LENGTH_OF_ONE_HAND_WORD;
	int min_size = MIN_LENGTH_OF_ONE_HAND_WORD;
	int word_size = rand() % (max_size + 1);
	while (word_size < min_size) {
		word_size += rand() % (max_size - min_size + 2);
	}
	word = (char*)malloc(sizeof(char) * word_size);
	if (word == NULL) {
		fprintf(stderr, "word alloc failed");
		exit(1);
	}

	int r;
	for (int i = 0; i < word_size; i++) {

		r = rand() % 48;

		switch (r)
		{
		case 0:		r = 'N'; break;
		case 1:		r = '1'; break;
		case 2:		r = '!'; break;
		case 3:		r = 'q'; break;
		case 4:		r = 'Q'; break;
		case 5:		r = 'a'; break;
		case 6:		r = 'A'; break;
		case 7:		r = 'z'; break;
		case 8:		r = 'Z'; break;
		case 9:		r = 'w'; break;
		case 10:	r = 'W'; break;
		case 11:	r = 's'; break;
		case 12:	r = 'S'; break;
		case 13:	r = 'x'; break;
		case 14:	r = 'X'; break;
		case 15:	r = 'e'; break;
		case 16:	r = 'E'; break;
		case 17:	r = '2'; break;
		case 18:	r = '@'; break;
		case 19:	r = '3'; break;
		case 20:	r = '#'; break;
		case 21:	r = 'd'; break;
		case 22:	r = 'D'; break;
		case 23:	r = 'c'; break;
		case 24:	r = 'C'; break;
		case 25:	r = '4'; break;
		case 26:	r = '$'; break;
		case 27:	r = 'r'; break;
		case 28:	r = 'R'; break;
		case 29:	r = 'f'; break;
		case 30:	r = 'F'; break;
		case 31:	r = 'v'; break;
		case 32:	r = 'V'; break;
		case 33:	r = '%'; break;
		case 34:	r = '5'; break;
		case 35:	r = 't'; break;
		case 36:	r = 'T'; break;
		case 37:	r = 'g'; break;
		case 38:	r = 'G'; break;
		case 39:	r = 'b'; break;
		case 40:	r = 'B'; break;
		case 41:	r = '6'; break;
		case 42:	r = '^'; break;
		case 43:	r = 'y'; break;
		case 44:	r = 'Y'; break;
		case 45:	r = 'h'; break;
		case 46:	r = 'H'; break;
		case 47:	r = 'n'; break;
		case 48:	r = '~'; break;


		default:
			break;
		}

		word[i] = r;

	}
	word[word_size] = '\0';

	return word;
}

char* fill_right(void) {
	char* word;
	int max_size = MAX_LENGTH_OF_ONE_HAND_WORD;
	int min_size = MIN_LENGTH_OF_ONE_HAND_WORD;
	int word_size = rand() % (max_size + 1);
	while (word_size < min_size) {
		word_size += rand() % (max_size - min_size + 2);
	}
	word = (char*)malloc(sizeof(char) * word_size);
	if (word == NULL) {
		fprintf(stderr, "word alloc failed");
		exit(1);
	}

	int r;
	for (int i = 0; i < word_size; i++) {

		r = rand() % 48;

		switch (r)
		{
		case 1:		r = 'y'; break;
		case 2:		r = 'Y'; break;
		case 3:		r = 'h'; break;
		case 4:		r = 'H'; break;
		case 5:		r = 'n'; break;
		case 6:		r = 'N'; break;
		case 7:		r = '7'; break;
		case 8:		r = '&'; break;
		case 9:		r = 'u'; break;
		case 10:	r = 'U'; break;
		case 11:	r = 'j'; break;
		case 12:	r = 'J'; break;
		case 13:	r = 'm'; break;
		case 14:	r = 'M'; break;
		case 15:	r = '8'; break;
		case 16:	r = '*'; break;
		case 17:	r = 'i'; break;
		case 18:	r = 'I'; break;
		case 19:	r = 'k'; break;
		case 20:	r = 'K'; break;
		case 21:	r = ','; break;
		case 22:	r = '<'; break;
		case 23:	r = '9'; break;
		case 24:	r = '('; break;
		case 25:	r = 'o'; break;
		case 26:	r = 'O'; break;
		case 27:	r = 'l'; break;
		case 28:	r = 'L'; break;
		case 29:	r = '.'; break;
		case 30:	r = '>'; break;
		case 31:	r = '0'; break;
		case 32:	r = ')'; break;
		case 33:	r = 'p'; break;
		case 34:	r = 'P'; break;
		case 35:	r = ';'; break;
		case 36:	r = ':'; break;
		case 37:	r = '/'; break;
		case 38:	r = '?'; break;
		case 39:	r = '-'; break;
		case 40:	r = '_'; break;
		case 41:	r = '['; break;
		case 42:	r = '{'; break;
		case 43:	r = "'"; break;
		case 44:	r = '"'; break;
		case 45:	r = '+'; break;
		case 46:	r = '='; break;
		case 47:	r = ']'; break;
		case 48:	r = '}'; break;
		case 49:	r = '\\'; break;
		case 50:	r = '|'; break;


		default:
			break;
		}

		word[i] = r;

	}
	word[word_size] = '\0';

	return word;
}

void fill(FILE** file) {

	for (int i = 0; i < 10000; i++) {
		fprintf(file[0], "%s\n", fill_norm());
	}

	for (int i = 0; i < 10000; i++) {
		fprintf(file[1], "%s\n", fill_long());
	}

	for (int i = 0; i < 10000; i++) {
		fprintf(file[2], "%s\n", fill_hard());
	}

	for (int i = 0; i < 10000; i++) {
		fprintf(file[3], "%s\n", fill_left());
	}

	for (int i = 0; i < 10000; i++) {
		fprintf(file[4], "%s\n", fill_right());
	}

}

int l_padding(char str[I], int j) {//middle text
	return (j - strlen(str)) / 2;
}

void ocpl_clean(bool* ocpl) {//line cleaner
	char space[] = "                        ";//25 spaces
	for (int i = 0; i < I; i++) {
		if (!ocpl[i]) {
			gotoxy(1, i);//goes to the line in which text is written
			printf("%s", space);//over writes the line with space
			ocpl[i] = true;
		}
	}
}

void ocpl_purge_all() {
	char space[] = "                        ";//25 spaces
	for (int i = 0; i < I; i++) {

		gotoxy(1, i);//goes to the line in which text is written
		printf("%s", space);//over writes the line with space

	}
	ocpl_clean(ocpl_copy);
}

void print_frame2(int _i, int _j) {
	gotoxy(0, 0);
	for (int i = 0; i < _i; i++) {
		for (int j = 0; j < _j; j++) {
			//printf((i == 0 || j == 0 || i + 1 == _i || j + 1 == _j) ? "*" : " ");
			if ((i == 0 && j == 0) || (i == 0 && j + 1 == _j))
				printf("%c", 220);
			else if (j == 0 || j + 1 == _j)
				printf("%c", 219);
			else if (i == 0 || i + 1 == _i)
				printf("%c", 220);

			else
				printf(" ");
		}
		printf("\n");
	}
}

void print_frame(int _i, int _j) {
	gotoxy(0, 0);
	for (int i = 0; i < _i; i++) {
		for (int j = 0; j < _j; j++) {

			if (i == 0 && j == 0)
				printf("%c", 201);
			else if (i == 0 && j + 1 == _j)
				printf("%c", 187);
			else if (j == 0 && i + 1 == _i)
				printf("%c", 200);
			else if (j + 1 == _j && i + 1 == _i)
				printf("%c", 188);
			else if (j == 0 || j + 1 == _j)
				printf("%c", 186);
			else if (i == 0 || i + 1 == _i)
				printf("%c", 205);

			else
				printf(" ");
		}
		printf("\n");
	}
}

void welcome(bool* ocpl) {

	print("WELCOME", "", 6, ocpl, false, J);


	print("TYPING GAME", "", 8, ocpl, false, J);


	print("press any key...", "", 11, ocpl, false, J);


	print("by shafagh sepehr", "", 23, ocpl, false, J);


	gotoxy(0, 30);//reset line to the end
}

void print(char* str, char* padding, int y, bool* ocpl, bool a, int j) {// a determines if padding must be used
	if (a)
		gotoxy(l_padding(padding, j), y);
	else
		gotoxy(l_padding(str, j), y);

	printf("%s", str);
	ocpl[y] = false;
}

void color_print2(char* str, int y, int j) {
	int len = strlen(str);
	gotoxy(l_padding(str, j), y);
	ocpl_copy[y] = false;
	for (int i = 0; i < len; i++) {
		if (color[i] == 0) {
			setcolor(10); printf("%c", str[i]); setcolor(0);
		}

		else if (color[i] == 1) {
			setcolor(6); printf("%c", str[i]); setcolor(0);
		}

		else if (color[i] == 2) {
			printf("%c", str[i]);

		}
	}
	return;
}


void color_print(char* str, char* padding, int y, bool* ocpl, bool a, int j, int green, int yellow, int black) {//dead function
	if (a)
		gotoxy(l_padding(padding, j), y);
	else
		gotoxy(l_padding(str, j), y);


	int len = strlen(str);
	int k = 0;
	if (green) {
		setcolor(10);//green

		for (int i = 0; i < green; i++, k++) {
			printf("%c", str[k]);
		}

	}

	if (yellow) {
		setcolor(6);//orange

		for (int i = 0; i < yellow; i++, k++) {
			printf("%c", str[k]);
		}

	}

	if (black) {

		setcolor(0);//black

		for (int i = 0; i < black; i++, k++) {
			printf("%c", str[k]);
		}
	}

	ocpl[y] = false;
}

void get_pass(char* password) {
	show_cursor();

	int p = 0;
	do {
		password[p] = getch();
		if (password[p] == '\b') {
			if (p == 0) {

			}
			else {

				gotoxy(wherex() - 2, wherey() - 1);
				printf(" ");
				gotoxy(wherex() - 2, wherey() - 1);
				p--;
			}
		}
		else if (p >= 19) {

		}
		else if (password[p] != '\r') {
			printf("*");
			p++;
		}

	} while (p == 0 || password[p] != '\r');
	password[p] = '\0';
	hide_cursor();
}

void get_user(char* username) {
	show_cursor();

	int p = 0;
	do {
		username[p] = getch();
		if (username[p] == '\b') {
			if (p == 0) {

			}
			else {

				gotoxy(wherex() - 2, wherey() - 1);
				printf(" ");
				gotoxy(wherex() - 2, wherey() - 1);
				p--;
			}
		}
		else if (p >= 19) {

		}
		else if (username[p] != '\r') {
			printf("%c", username[p]);
			p++;
		}

	} while (p == 0 || username[p] != '\r');
	username[p] = '\0';

	hide_cursor();
}

void account_menu(bool* ocpl, FILE* usr_inf, user* user_struct) {

	bool BREAK;

	while (1) {

		system("cls");
		print_frame(11, J);


		print("1-Sign Up", "", 4, ocpl, false, J);


		print("2-Log In", "1-Sign In", 5, ocpl, true, J);


		print("3-Exit", "1-Sign In", 6, ocpl, true, J);

		while (1) {
			BREAK = false;
			int ch = getch();
			ch -= '0';
			switch (ch) {
			case 1:
				system("cls");//clears screen completly
				sign_up(ocpl, usr_inf);
				BREAK = true;
				break;

			case 2:
				ocpl_clean(ocpl);
				if (log_in(ocpl, usr_inf, user_struct))
					return;
				BREAK = true;
				break;

			case 3://exit
				ocpl_clean(ocpl);

				print("Thanks For Your Time", "", 4, ocpl, false, J);

				countdown(l_padding("0", J), wherey() + 1, 3);


				system("cls");
				_fcloseall();
				exit(0);
				break;
			default:

				break;
			}
			if (BREAK) {
				break;
			}
		}
	}
}

void countdown(int x, int y, int sec) {
	for (int i = sec; i >= 0; i--) {
		gotoxy(x, y);
		printf("%d", i);
		_sleep(1000);
	}
}

bool is_user_unique(FILE* usr_inf, char* username) {
	fseek(usr_inf, 0, SEEK_SET);
	user temp;
	while (fgetc(usr_inf) != EOF) {
		fseek(usr_inf, -1, SEEK_CUR);
		fread(&temp, sizeof(user), 1, usr_inf);
		if (strcmp(temp.username, username) == 0) {
			return false;
		}
	}
	fseek(usr_inf, 0, SEEK_SET);
	return true;
}

bool does_username_exist_and_get_user_struct(FILE* usr_inf, char* username, user* user_struct) {
	fseek(usr_inf, 0, SEEK_SET);
	user temp;
	while (fgetc(usr_inf) != EOF) {
		fseek(usr_inf, -1, SEEK_CUR);
		fread(&temp, sizeof(user), 1, usr_inf);
		if (strcmp(temp.username, username) == 0) {
			(*user_struct) = temp;
			fseek(usr_inf, -1 * sizeof(user), SEEK_CUR);
			return true;
		}
	}

	return false;

}

void sign_up(bool* ocpl, FILE* usr_inf) {

	print_frame(11, 48);
	user temp;
	char ch;
	bool Break;

	char username[20], password[20];
	while (1) {
		Break = false;
		print("SIGN UP", "", 2, ocpl, false, 48);
		print("max length = 19 characters", "", 8, ocpl, false, 49);


		print("                                 ", "", 6, ocpl, false, 48);//clears username is taken massage if it's there
		print("Choose A Username: ", "", 4, ocpl, false, J);
		get_user(username);

		print("                                 ", "choose a password: ", 6, ocpl, true, 48);//clears username is taken massage if it's there
		print("Choose A Password: ", "", 6, ocpl, false, J);
		get_pass(password);

		if (is_user_unique(usr_inf, username))
			break;
		else {
			system("cls");
			print_frame(11, 48);
			print("Username Is Already Taken", "", 6, ocpl, false, 48);

			print("  1-Try Again 2-Return Back  ", "", 8, ocpl, false, 48);

			while (1) {
				ch = getch();
				ch -= '0';
				switch (ch) {
				case 1:
					//outer loop continues
					Break = true;
					break;

				case 2:
					return;//to the account_menu
					break;

				default:

					break;
				}
				if (Break)
					break;
			}

		}
	}

	//strcpy(temp.password, password);
	temp.password = hash(password, username);

	strcpy(temp.username, username);
	fseek(usr_inf, 0, SEEK_END);
	fwrite(&temp, sizeof(user), 1, usr_inf);
	fseek(usr_inf, 0, SEEK_SET);

	//....................................................................................................
	return;
}

void hide_cursor(void) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void show_cursor(void)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 10;
	info.bVisible = TRUE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

unsigned long hash(char* password, char* username) {
	char s[45];
	strcpy(s, password);
	strcat(s, username);

	unsigned long h;
	unsigned const char* us;
	int MULTIPLIER = 37;
	us = (unsigned const char*)s;
	int i = 255;
	h = 0;
	while (*us != '\0') {
		h = h * MULTIPLIER + *us * i;
		us++;
		i = i * *us - i;
	}

	return h;
}

bool log_in(bool* ocpl, FILE* usr_inf, user* user_struct) {

	bool is_credentials_right, Break;



	char username[20], password[20], realpass[20], ch;
	while (1) {
		is_credentials_right = true;
		Break = false;
		system("cls");//clears screen completly

		print_frame(11, 40);

		print("LOG IN", "", 2, ocpl, false, 40);
		print("max length = 19 characters", "", 8, ocpl, false, 41);



		print("Username: ", "", 4, ocpl, false, J);
		get_user(username);

		if (!does_username_exist_and_get_user_struct(usr_inf, username, user_struct))
			is_credentials_right = false;


		print("Password: ", "", 6, ocpl, false, J);
		get_pass(password);
		if (hash(password, username) != user_struct->password) {
			is_credentials_right = false;
		}

		if (is_credentials_right) {
			break;//to the menu
		}
		else {
			print_frame(11, 40);
			print("Wrong Credentials", "", 6, ocpl, false, 40);
			print("1-Try Again 2-Return Back", "", 8, ocpl, false, 40);

			while (1) {
				ch = getch();
				ch -= '0';
				switch (ch) {
				case 1:
					//outer loop continues
					Break = true;
					break;

				case 2:
					return false;//to the account_menu
					break;

				default:

					break;
				}
				if (Break)
					break;
			}

		}
	}
	return true;

}

void change_password(bool* ocpl, FILE* usr_inf, user* user_struct) {
	bool Break;


	char newPassword[20], curPassword[20];

	while (1) {
		Break = false;

		system("cls");
		print_frame(11, 50);

		print("CHANGE PASSWORD", "", 2, ocpl, false, 50);
		print("max length = 19 characters", "", 8, ocpl, false, 51);


		print("Enter Current Password: ", "", 4, ocpl, false, J + 2);
		get_pass(curPassword);

		if (hash(curPassword, user_struct->username) != user_struct->password) {
			system("cls");
			print_frame(7, 50);


			print("Password Is Incorrect", "", 2, ocpl, false, 50);
			print("1-Try Again 2-Return Back", "", 4, ocpl, false, 50);


			char num;
			while (1) {
				num = getch();
				num -= '0';
				switch (num) {
				case 1:
					Break = true;
					break;
				case 2:
					return;
					break;
				default:
					break;
				}
				if (Break)
					break;
			}
		}
		else {
			break;
		}
	}
	print("Enter   New   Password: ", "", 6, ocpl, false, J + 2);
	get_user(newPassword);

	//strcpy(user_struct->password, newPassword);
	user_struct->password = hash(newPassword, user_struct->username);

	fwrite(user_struct, sizeof(user), 1, usr_inf);
	fseek(usr_inf, -1 * sizeof(user), SEEK_CUR);

	return;

}



void newGame_menu(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words) {

	char ch;

	while (1) {

		system("cls");

		print_frame(15, J);

		print("CHOOSE LEVEL DIFFICULTY", "", 3, ocpl, false, J);
		print("1-Easy", "3-Change Password", 6, ocpl, true, J);
		print("2-Medium", "3-Change Password", 7, ocpl, true, J);
		print("3-Hard", "3-Change Password", 8, ocpl, true, J);
		print("4-Left Hand Only", "3-Change Password", 9, ocpl, true, J);
		print("5-Right Hand Only", "3-Change Password", 10, ocpl, true, J);
		print("6-Back To Main Menu", "3-Change Password", 11, ocpl, true, J);



		ch = getch();
		ch -= '0';
		switch (ch) {
		case 1:
			easy_game(ocpl, usr_inf, user_struct, words, 2,false);
			break;

		case 2:
			medium_game(ocpl, usr_inf, user_struct, words, 2,false);
			break;

		case 3:
			hard_game(ocpl, usr_inf, user_struct, words, 2,false);
			break;
		case 4:
			left_hand_game(ocpl, usr_inf, user_struct, words, 2,false);
			break;
		case 5:
			right_hand_game(ocpl, usr_inf, user_struct, words, 2,false);
			break;
		case 6:
			return;
			break;
		default:

			break;
		}
	}

}



int main_menu(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words) {
	char ch;

	while (1) {

		system("cls");
		print_frame(15, J);
		print("1-New Game", "3-Change Password", 5, ocpl, true, J);
		print("2-Load Game", "3-Change Password", 6, ocpl, true, J);
		print("3-Change Password", "", 7, ocpl, false, J);
		print("4-Log Out", "3-Change Password", 8, ocpl, true, J);
		print("5-Exit", "3-Change Password", 9, ocpl, true, J);


		ch = getch();
		ch -= '0';
		switch (ch) {
		case 1:
			newGame_menu(ocpl, usr_inf, user_struct, words);
			break;

		case 2:

			break;

		case 3:
			change_password(ocpl, usr_inf, user_struct);
			break;
		case 4:
			return 1;
			break;
		case 5:
			ocpl_clean(ocpl);

			print("Thanks For Your Time", "", 4, ocpl, false, J);

			countdown(l_padding("0", J), wherey() + 1, 3);


			system("cls");
			//fcloseall();
			_fcloseall();
			exit(0);
			break;
		default:

			break;
		}
	}
}

int char_index() {
	int len = strlen(index);
	for (int i = 0; i < len; i++) {
		if (color[i] == 2)
			return i;
	}
}

bool is_word_solved() {
	int len = strlen(index);
	for (int i = 0; i < len; i++) {
		if (color[i] != 0)
			return false;
	}
	return true;
}

void reset_color_array() {
	for (int i = 0; i < 25; i++)
		color[i] = 2;
}

void my_callback_on_key_arrival(char c) {//when a word is removed, you need to wait for words to move a line down to be able to enter the next word
	if (End)
		return;

	int len = strlen(index);

	int a = char_index();

	if (c == index[a]) {
		color[a] = 0;
		score++;
	}
	else if (c == '\b') {
		if (a && color[a - 1] == 0)
			score--;
		color[a - 1] = 2;

	}
	else {
		color[a] = 1;

	}
	color_print2(index, index_line, J);
	if (is_word_solved()) {
		print("                      ", "", index_line, ocpl_copy, false, J);
		is_solved = true;

		reset_color_array();
		if (ind < 10) {
			strcpy(index, global_node->next->str);
			index_line--;
		}
	}

}

bool do_wave(bool* ocpl, int wave_time, Node* head, int* f_score) {
	reset_color_array();
	Node* tmp_node;
	int line = 1, final_score = 0;
	ind = 1;
	while (1) {
		tmp_node = head;
		
		for (int i = 0; i < 10; i++) {
			if ((line - i) > 0 && ind <= i+1) {
				if (ind == i+1) {
					index_line = line - i;
					strcpy(index, tmp_node->str);
					color_print2(index, index_line, J);
					global_node = tmp_node;
				}
				else {
					print(tmp_node->str, "", line - i, ocpl, false, J);

				}	
			}
			if(i<9)
			tmp_node = tmp_node->next;
		}

		_sleep(wave_time / 10);
		line++;
		ocpl_clean(ocpl);
		if (is_solved) {
			is_solved = false;

			ind++;
			final_score += score;
			score = 0;
			if (ind > 10) {
				*f_score += final_score;
				return true;
			}
		}
		if (index_line == 23) {
			*f_score += final_score;
			return false;
		}


	}
}


void easy_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {
	HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	Node* head, * tail;
	int wave = 1, wave_time, score = 0;

	print_frame(I, J);

	while (1) {
		make_linked_list(&head, &tail);
		fill_Linked_list(head, wave, words);
		wave_time = (int)(EASY_GAME_TIME * (pow(0.8, wave - 1)) * 1000);
		if (wave_time < WAVE_TIME_TO_WIN)
			wave_time = WAVE_TIME_TO_WIN;

		if (!do_wave(ocpl, wave_time, head, &score)) {
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 1, wave);
		}
		wave++;
		if (wave_time == WAVE_TIME_TO_WIN) {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 1, wave);
		}
		kill_linked_list(&head, &tail);
	}
}

void medium_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {
	HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	Node* head, * tail;
	int wave = 1, wave_time, score = 0;

	print_frame(I, J);

	while (1) {
		make_linked_list(&head, &tail);
		fill_Linked_list(head, wave, words);
		wave_time = (int)(MEDIUM_GAME_TIME * (pow(0.8, wave - 1)) * 1000);
		if (wave_time < WAVE_TIME_TO_WIN)
			wave_time = WAVE_TIME_TO_WIN;

		if (!do_wave(ocpl, wave_time, head, &score)) {
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 2, wave);
		}
		wave++;
		if (wave_time == WAVE_TIME_TO_WIN) {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 2, wave);
		}
		kill_linked_list(&head, &tail);
	}
}

void hard_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {
	HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	Node* head, * tail;
	int wave = 1, wave_time, score = 0;

	print_frame(I, J);

	while (1) {
		make_linked_list(&head, &tail);
		fill_Linked_list(head, wave, words);
		wave_time = (int)(HARD_GAME_TIME * (pow(0.8, wave - 1)) * 1000);
		if (wave_time < WAVE_TIME_TO_WIN)
			wave_time = WAVE_TIME_TO_WIN;

		if (!do_wave(ocpl, wave_time, head, &score)) {
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 3, wave);
		}
		wave++;
		if (wave_time == WAVE_TIME_TO_WIN) {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 3, wave);
		}
		kill_linked_list(&head, &tail);
	}
}

void left_hand_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {
	HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	Node* head, * tail;
	int wave = 1, wave_time, score = 0;

	print_frame(I, J);

	while (1) {
		make_linked_list(&head, &tail);
		fill_linked_list_one_hand(head, words[3]);

		wave_time = (int)(ONE_HAND_GAME_TIME * (pow(0.8, wave - 1)) * 1000);
		if (wave_time < WAVE_TIME_TO_WIN)
			wave_time = WAVE_TIME_TO_WIN;

		if (!do_wave(ocpl, wave_time, head, &score)) {
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 3, wave);
		}
		wave++;
		if (wave_time == WAVE_TIME_TO_WIN) {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 3, wave);
		}
		kill_linked_list(&head, &tail);
	}
}

void right_hand_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {
	HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	Node* head, * tail;
	int wave = 1, wave_time, score = 0;

	print_frame(I, J);

	while (1) {
		make_linked_list(&head, &tail);
		fill_linked_list_one_hand(head, words[4]);
		wave_time = (int)(ONE_HAND_GAME_TIME * (pow(0.8, wave - 1)) * 1000);
		if (wave_time < WAVE_TIME_TO_WIN)
			wave_time = WAVE_TIME_TO_WIN;

		if (!do_wave(ocpl, wave_time, head, &score)) {
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 3, wave);
		}
		wave++;
		if (wave_time == WAVE_TIME_TO_WIN) {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 3, wave);
		}
		kill_linked_list(&head, &tail);
	}
}

void finish(bool win, int score, FILE* usr_inf, user* user_struct, bool* ocpl, int save_in_slot, bool update,int level_num,int wave) {
	user new_user_struct = *user_struct,temp;
	
	time_t t;
	t = time(NULL);
	struct tm tm;
	tm = *localtime(&t);

	if (update) {
		
		new_user_struct.game[save_in_slot].xp += score;
		new_user_struct.game[save_in_slot].high_score += score;
		new_user_struct.game[save_in_slot].second = tm.tm_sec;
		new_user_struct.game[save_in_slot].minute = tm.tm_min;
		new_user_struct.game[save_in_slot].hour = tm.tm_hour;
		new_user_struct.game[save_in_slot].day = tm.tm_mday;
		new_user_struct.game[save_in_slot].month = tm.tm_mon;
		new_user_struct.game[save_in_slot].year = tm.tm_year + 1900;
	}
	else {
		new_user_struct.game[2] = new_user_struct.game[1];
		new_user_struct.game[1] = new_user_struct.game[0];

		new_user_struct.game[0].xp = score;
		new_user_struct.game[0].high_score = score;
		new_user_struct.game[0].second = tm.tm_sec;
		new_user_struct.game[0].minute = tm.tm_min;
		new_user_struct.game[0].hour = tm.tm_hour;
		new_user_struct.game[0].day = tm.tm_mday;
		new_user_struct.game[0].month = tm.tm_mon;
		new_user_struct.game[0].year = tm.tm_year + 1900;
		new_user_struct.game[0].level_difficulty = level_num;
	}
	fwrite(&new_user_struct, sizeof(user), 1, usr_inf);

	system("cls");
	
	if (win) {
		print_frame(8, 45);

		print("You Have Achieved Great Skills in Typing!!", "",2, ocpl, false, 45);
		print("", "score: 58", 4, ocpl, true, 45); printf("Score: %d", score);

		countdown(22, 6, 5);

	}
	else {
		print_frame(10, 30);

		print("GAME OVER", "", 2, ocpl, false, 30);

		print("", "score: 58", 4, ocpl, true, 30); printf("Score: %d", score);

		print("", "Wave: 58", 5, ocpl, true, 30); printf("Wave: %d", wave);

		switch (level_num)
		{
		case 1:
			print("Level Difficulty: Easy", "", 6, ocpl, false, 30);
			break;
		case 2:
			print("Level Difficulty: Medium", "", 6, ocpl, false, 30);
			break;
		case 3:
			print("Level Difficulty: Hard", "", 6, ocpl, false, 30);
			break;
		case 4:
			print("Level Difficulty: Left Hand Only", "", 6, ocpl, false, 30);
			break;
		case 5:
			print("Level Difficulty: Right Hand Only", "", 6, ocpl, false, 30);
			break;
		default:
			break;
		}
		countdown(15, 8, 5);
	}
	gotoxy(0, 0);
	system("cls");
	fcloseall();
	exit(0);
}


/*
this is for time and date
time_t t;
	t = time(NULL);
	struct tm tm;
	tm = *localtime(&t);
	printf("Current Time: %d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	printf("Current Date: %d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
*/


/*
		if (line > 0 && ind <= 1) {
			if (ind == 1) {
				index_line = line;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 1) > 0 && ind <= 2) {
			if (ind == 2) {
				index_line = line - 1;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 1, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 2) > 0 && ind <= 3) {
			if (ind == 3) {
				index_line = line - 2;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 2, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 3) > 0 && ind <= 4) {
			if (ind == 4) {
				index_line = line - 3;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 3, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 4) > 0 && ind <= 5) {
			if (ind == 5) {
				index_line = line - 4;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 4, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 5) > 0 && ind <= 6) {
			if (ind == 6) {
				index_line = line - 5;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 5, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 6) > 0 && ind <= 7) {
			if (ind == 7) {
				index_line = line - 6;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 6, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 7) > 0 && ind <= 8) {
			if (ind == 8) {
				index_line = line - 7;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 7, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 8) > 0 && ind <= 9) {
			if (ind == 9) {
				index_line = line - 8;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 8, ocpl, false, J);

			}
		}

		tmp_node = tmp_node->next;
		if ((line - 9) > 0 && ind <= 10) {
			if (ind == 10) {
				index_line = line - 9;
				strcpy(index, tmp_node->str);
				color_print2(index, index_line, J);
				global_node = tmp_node;
			}
			else {
				print(tmp_node->str, "", line - 9, ocpl, false, J);

			}
		}
		*/