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
#define ONE_HAND_GAME_TIME 20 //one hand game wave time

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
	char username[20], first_name[20], last_name[20];

	int age;
	unsigned long password;
	gameinfo game[3];

}user;

typedef struct Node {
	char* str;
	struct Node* next, * prev;
} Node;

char index[25];
bool is_solved = false;
int index_line, color[25], score;//color[x]=  0->green 1->yellow 2->black
bool* ocpl_copy;
Node* global_node;
bool End = true;

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
void get_pass(char* password);//gets a 19 char long string but dont show it
void get_user(char* username);//gets a 19 char long string
int get_age();
bool is_user_unique(FILE* usr_inf, char* username);
bool does_username_exist_and_get_user_struct(FILE* usr_inf, char* username, user* user_struct);
int  main_menu(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words);
void loadGame_menu(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words);
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
void color_print2(char* str, int y, int j);
Node* create_Node();
void make_linked_list_or_add_nodes(Node** head, Node** tail);
void kill_linked_list(Node** head, Node** tail);
void fill_one_node(Node* head, FILE* words);
void fill_Linked_list(Node* head, int wave, FILE** words);
void fill_linked_list_one_hand(Node* head, FILE* words);
void pop_front(Node** head);
void easy_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
void medium_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
void hard_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
void left_hand_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
void right_hand_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update);
bool do_wave(bool* ocpl, int wave_time, int* f_score, FILE** words, int level_num, float mul, int* wave);
void finish(bool win, int score, FILE* usr_inf, user* user_struct, bool* ocpl, int save_in_slot, bool update, int level_num, int wave);




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

void make_linked_list_or_add_nodes(Node** head, Node** tail) {
	static bool is_head_null = true;
	Node* temp_pointer;
	if (is_head_null) {
		(*head) = create_Node();
		(*tail) = (*head);

		for (int i = 0; i < 9; i++) {
			temp_pointer = create_Node();
			temp_pointer->prev = (*tail);//new node's prev will be the last node
			(*tail)->next = temp_pointer;//last node's next will be new node
			(*tail) = temp_pointer;//new node is now the last node
		}
		is_head_null = false;
	}
	else {
		for (int i = 0; i < 10; i++) {
			temp_pointer = create_Node();
			temp_pointer->prev = (*tail);//new node's prev will be the last node
			(*tail)->next = temp_pointer;//last node's next will be new node
			(*tail) = temp_pointer;//new node is now the last node
		}

	}

	return;
}

void fill_Linked_list(Node* head, int wave, FILE** words) {
	int nrm, lng, hrd, ctr = 0;
	switch (wave)
	{
	case 1:
		nrm = 9; lng = 0; hrd = 1;

		break;
	case 2:
		nrm = 8; lng = 1; hrd = 1;
		break;

	case 3:
		nrm = 7; lng = 2; hrd = 1;
		break;

	case 4:
		nrm = 6; lng = 2; hrd = 2;
		break;

	case 5:
		nrm = 5; lng = 3; hrd = 2;
		break;

	default:
		nrm = 4; lng = 3; hrd = 3;


		break;
	}
	for (int i = 0; i < 10; i++, ctr++) {
		if ((ctr > 3 && hrd && (rand() % (10 - ctr)) == 0) || (!nrm && hrd)) {// if the chance is right
			fill_one_node(head, words[2]);
			hrd--;
		}
		else if ((ctr > 3 && lng && (rand() % (10 - ctr)) == 0) || (!nrm && lng)) {
			fill_one_node(head, words[1]);
			lng--;
		}
		else if (nrm) {
			fill_one_node(head, words[0]);
			nrm--;
		}

	}
}

void kill_linked_list(Node** head, Node** tail) {
	while ((*tail)->prev != NULL) {
		free((*tail)->str);
		(*tail) = (*tail)->prev;
		free((*tail)->next);
	}
	free((*tail)->str);
	free(*tail);
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

void pop_front(Node** head) {
	free((*head)->str);
	(*head) = (*head)->next;
	free((*head)->prev);
	(*head)->prev = NULL;
	return;
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
	char left[] = "N1!qQaAzZwWsSxXeE2@3#dDcC$4rRfFvV%5tTgGbB6^yYhHn~`";
	char a = left[49], b = left[50];

	for (int i = 0; i < word_size; i++) {

		r = rand() % 50;//50 is \0
		
		word[i] = left[r];

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
	char right[] = "yYhHnN7&uUjJmM8*iIkK,<9(oOlL.>0)pP;:/?-_[{\'\"+=]}\\";


	for (int i = 0; i < word_size; i++) {
	a:
		r = rand() % 49;//49 is \0
		

		word[i] = right[r];

	}
	word[word_size] = '\0';

	return word;
}

void fill(FILE** file) {

	for (int i = 0; i < 10000; i++) {
		fprintf(file[0], "%s\n", fill_norm());
	}
	fseek(file[0], 0, SEEK_SET);

	for (int i = 0; i < 10000; i++) {
		fprintf(file[1], "%s\n", fill_long());
	}
	fseek(file[1], 0, SEEK_SET);

	for (int i = 0; i < 10000; i++) {
		fprintf(file[2], "%s\n", fill_hard());
	}
	fseek(file[2], 0, SEEK_SET);

	for (int i = 0; i < 10000; i++) {
		fprintf(file[3], "%s\n", fill_left());
	}
	fseek(file[3], 0, SEEK_SET);

	for (int i = 0; i < 10000; i++) {
		fprintf(file[4], "%s\n", fill_right());
	}
	fseek(file[4], 0, SEEK_SET);

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

int get_age() {
	char username[5];

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
		else if (p >= 3) {

		}
		else if (username[p] != '\r') {
			printf("%c", username[p]);
			p++;
		}

	} while (p == 0 || username[p] != '\r');
	username[p] = '\0';

	switch (p)
	{
	case 1: return username[0] - '0';
	case 2: return (username[1] - '0') + ((username[0] - '0') * 10);
	case 3:return (username[2] - '0') + ((username[1] - '0') * 10) + ((username[0] - '0') * 100);
	default:
		break;
	}



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

	char username[20], password[20], first_name[20], last_name[20];
	int age;

	while (1) {

		system("cls");
		print_frame(17, 50);

		Break = false;
		print("SIGN UP", "", 2, ocpl, false, 50);
		print("max length = 19 characters", "", 14, ocpl, false, 51);



		print("Choose A Username: ", "", 4, ocpl, false, J);
		get_user(username);


		print("Choose A Password: ", "", 6, ocpl, false, J);
		get_pass(password);

		if (is_user_unique(usr_inf, username))
			break;
		else {
			system("cls");
			print_frame(7, 48);
			print("Username Is Already Taken", "", 2, ocpl, false, 50);

			print("  1-Try Again 2-Return Back  ", "", 4, ocpl, false, 50);

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



	print("Enter Your First Name: ", "Choose A Password: ", 8, ocpl, true, J);
	get_user(first_name);

	print("Enter Your Last Name: ", "Choose A Password: ", 10, ocpl, true, J);
	get_user(last_name);

	print("Enter Your Age: ", "Choose A Password: ", 12, ocpl, true, J);
	show_cursor();
	age = get_age();
	hide_cursor();


	strcpy(temp.first_name, first_name);
	strcpy(temp.last_name, last_name);
	temp.age = age;


	temp.password = hash(password, username);
	for (int i = 0; i < 3; i++)
		temp.game[i].is_active = false;

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
			easy_game(ocpl, usr_inf, user_struct, words, 2, false);
			break;

		case 2:
			medium_game(ocpl, usr_inf, user_struct, words, 2, false);
			break;

		case 3:
			hard_game(ocpl, usr_inf, user_struct, words, 2, false);
			break;
		case 4:
			left_hand_game(ocpl, usr_inf, user_struct, words, 2, false);
			break;
		case 5:
			right_hand_game(ocpl, usr_inf, user_struct, words, 2, false);
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
			loadGame_menu(ocpl, usr_inf, user_struct, words);
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
			fcloseall();

			exit(0);
			break;
		default:

			break;
		}
	}
}

void loadGame_menu(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words) {
	char ch;
	int slot, level;
	bool BREAK = false;
	while (1) {


		system("cls");
		print_frame(18, 60);

		for (int i = 0; i < 3 && user_struct->game[i].is_active; i++) {
			gotoxy(3, 2 + (i * 4));
			printf("%d- Level Difficulty: ", i + 1);
			switch (user_struct->game[i].level_difficulty) {
			case 1:printf("Easy"); break;
			case 2:printf("Medium"); break;
			case 3:printf("Hard"); break;
			case 4:printf("Left Hand Only"); break;
			case 5:printf("Right Hand Only"); break;
			default: break;
			}
			gotoxy(3, 3 + (i * 4));
			printf("   XP: %d      High Score: %d", user_struct->game[i].xp, user_struct->game[i].high_score);
			gotoxy(3, 4 + (i * 4));
			printf("   Last Time Played on date: %d/%d/%d", user_struct->game[i].day, user_struct->game[i].month, user_struct->game[i].year);
			gotoxy(3, 5 + (i * 4));
			printf("   Last Time Played at Time: %d:%d:%d", user_struct->game[i].hour, user_struct->game[i].minute, user_struct->game[i].second);
		}
		gotoxy(3, 15);
		printf("4- Back to Main Menu");

		while (1) {
			BREAK = false;
			ch = getch();
			ch -= '0';

			switch (ch) {
			case 1:
				slot = 0;
				level = user_struct->game[0].level_difficulty;
				BREAK = true;
				break;
			case 2:
				slot = 1;
				level = user_struct->game[1].level_difficulty;
				BREAK = true;
				break;
			case 3:
				slot = 2;
				level = user_struct->game[2].level_difficulty;
				BREAK = true;
				break;
			case 4:
				return;
				break;
			default:
				break;
			}

			if (BREAK)
				break;
		}
		system("cls");

		switch (level)
		{
		case 1:
			easy_game(ocpl, usr_inf, user_struct, words, slot, true);

		case 2:
			medium_game(ocpl, usr_inf, user_struct, words, slot, true);

		case 3:
			hard_game(ocpl, usr_inf, user_struct, words, slot, true);

		case 4:
			left_hand_game(ocpl, usr_inf, user_struct, words, slot, true);

		case 5:
			right_hand_game(ocpl, usr_inf, user_struct, words, slot, true);

		default:
			break;
		}

	}
}

int char_index() {//returns index of the letter we're going to type
	int len = strlen(index);
	for (int i = 0; i < len; i++) {
		if (color[i] == 2)
			return i;
	}
}

bool is_word_solved() {//checks if all character  are green or 0 and returns true if so
	int len = strlen(index);
	for (int i = 0; i < len; i++) {
		if (color[i] != 0)
			return false;
	}
	return true;
}

void reset_color_array() {//sets all values to 2
	for (int i = 0; i < 25; i++)
		color[i] = 2;
}

void my_callback_on_key_arrival(char c) {
	if (End)
		return;

	int len = strlen(index);

	int a = char_index();

	if (c == index[a]) {
		color[a] = 0;
		score++;

		if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' || c == ')' || c == '_' || c == '+' || c == '[' || c == ']' || c == '{' || c == '}' || c == '\'' || c == '"' || c == '?' || c == '/' || c == '`' || c == '~' || c == '<' || c == '>' || c == ',' || c == '.' || c == '-' || c == '=' || c == ':' || c == ';')
			score++;
	}
	else if (c == '\b') {
		if (a && color[a - 1] == 0) {
			score--;
			c = index[a - 1];
			if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' || c == ')' || c == '_' || c == '+' || c == '[' || c == ']' || c == '{' || c == '}' || c == '\'' || c == '"' || c == '?' || c == '/' || c == '`' || c == '~' || c == '<' || c == '>' || c == ',' || c == '.' || c == '-' || c == '=')
				score--;
		}

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

		strcpy(index, global_node->next->str);
		index_line--;

		print("                      ", "", index_line, ocpl_copy, false, J);
		color_print2(index, index_line, J);
	}

}

bool do_wave(bool* ocpl, int wave_time, int* f_score, FILE** words, int level_num, float mul, int* wave) {
	Node* head, * tail, * tmp_node, * address_saver[1000] = { NULL };
	bool does_adrs_match = false;
	int w_time, line = 1, final_score = 0, word_ctr = 1, word_ctr2 = 1, adrs_svr_ind = 0;
	*wave = 1;

	w_time = (int)(wave_time * (pow(mul, *wave - 1)) * 1000);

	make_linked_list_or_add_nodes(&head, &tail);

	if (level_num < 4)
		fill_Linked_list(head, *wave, words);
	else if (level_num == 4)
		fill_linked_list_one_hand(head, words[3]);
	else if (level_num == 5)
		fill_linked_list_one_hand(head, words[4]);



	while (1) {

		tmp_node = head;


		for (int i = 0; i < 23; i++) {
			if ((line - i) > 0) {//&& ind <= i + 1
				if (!i) {
					index_line = line - i;
					strcpy(index, tmp_node->str);
					color_print2(index, index_line, J);
					global_node = tmp_node;
				}
				else {
					for (int k = 0; address_saver[k] != NULL; k++)
						if (address_saver[k] == tmp_node) {
							does_adrs_match = true;
							break;
						}
					if (((line - i - 1 == 0) && (word_ctr2 > 8) && (rand() % 6 == 0)) || does_adrs_match) {// if a node is supposed to be hidden we save																						its address so the next time we know we have																						to hide it// the line-i-1 only lets the words																						on the first line to be hidden so there wont																						be a word suddenly getting hidden
						print("*********************", "", line - i, ocpl, false, J);
						does_adrs_match = false;
						address_saver[adrs_svr_ind] = tmp_node;
						adrs_svr_ind++;
					}
					else
						print(tmp_node->str, "", line - i, ocpl, false, J);
				}
			}
			else {
				break;
			}

			tmp_node = tmp_node->next;

		}

		_sleep(w_time / 10);
		line++; word_ctr++; word_ctr2++;
		ocpl_clean(ocpl);
		if (w_time == WAVE_TIME_TO_WIN) {
			*f_score = final_score;
			kill_linked_list(&head, &tail);
			return true;
		}

		if (is_solved) {
			is_solved = false;
			pop_front(&head);
			//ind++;
			final_score += score;
			score = 0;
			line--;
		}
		if (word_ctr >= 9) {
			make_linked_list_or_add_nodes(&head, &tail);
			if (level_num < 4)
				fill_Linked_list(head, *wave, words);
			else if (level_num == 4)
				fill_linked_list_one_hand(head, words[3]);
			else if (level_num == 5)
				fill_linked_list_one_hand(head, words[4]);

			word_ctr -= 10;
		}
		if (word_ctr2 % 10 == 0) {
			(*wave)++;
			w_time = (int)(wave_time * (pow(0.8, *wave - 1)) * 1000);
			if (w_time <= WAVE_TIME_TO_WIN)
				w_time = WAVE_TIME_TO_WIN;
		}

		if (index_line == 23) {
			*f_score = final_score;
			kill_linked_list(&head, &tail);
			return false;
		}
	}


}


void easy_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {

	system("cls");

	End = false;

	HANDLE thread_id = start_listening(my_callback_on_key_arrival);

	int wave, score = 0;

	print_frame(I, J);

	while (1) {

		if (!do_wave(ocpl, EASY_GAME_TIME, &score, words, 1, 0.8, &wave)) {
			End = true;
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 1, wave);
		}
		else {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 1, wave);
		}
	}

}

void medium_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {

	system("cls");

	End = false;

	HANDLE thread_id = start_listening(my_callback_on_key_arrival);

	int wave = 1, score = 0;

	print_frame(I, J);

	while (1) {

		if (!do_wave(ocpl, MEDIUM_GAME_TIME, &score, words, 2, 0.7, &wave)) {
			End = true;
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 2, wave);
		}
		else {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 2, wave);
		}
	}

}

void hard_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {

	system("cls");

	End = false;

	HANDLE thread_id = start_listening(my_callback_on_key_arrival);

	int wave = 1, wave_time, score = 0;

	print_frame(I, J);

	while (1) {

		if (!do_wave(ocpl, HARD_GAME_TIME, &score, words, 3, 0.6, &wave)) {
			End = true;
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 3, wave);
		}
		else {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 3, wave);
		}
	}



}

void left_hand_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {

	system("cls");

	End = false;

	HANDLE thread_id = start_listening(my_callback_on_key_arrival);

	int wave = 1, wave_time, score = 0;

	print_frame(I, J);

	while (1) {

		if (!do_wave(ocpl, ONE_HAND_GAME_TIME, &score, words, 4, 0.8, &wave)) {
			End = true;
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 4, wave);
		}
		else {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 4, wave);
		}
	}



}

void right_hand_game(bool* ocpl, FILE* usr_inf, user* user_struct, FILE** words, int save_in_slot, bool update) {

	system("cls");

	End = false;

	HANDLE thread_id = start_listening(my_callback_on_key_arrival);

	int wave = 1, wave_time, score = 0;

	print_frame(I, J);

	while (1) {

		if (!do_wave(ocpl, ONE_HAND_GAME_TIME, &score, words, 5, 0.8, &wave)) {
			End = true;
			finish(false, score, usr_inf, user_struct, ocpl, save_in_slot, update, 5, wave);
		}
		else {
			finish(true, score, usr_inf, user_struct, ocpl, save_in_slot, update, 5, wave);
		}
	}


}

void finish(bool win, int score, FILE* usr_inf, user* user_struct, bool* ocpl, int save_in_slot, bool update, int level_num, int wave) {
	user new_user_struct = *user_struct, temp;

	time_t t;
	t = time(NULL);
	struct tm tm;
	tm = *localtime(&t);

	if (update) {

		new_user_struct.game[save_in_slot].xp += score;
		if (new_user_struct.game[save_in_slot].high_score < score)
			new_user_struct.game[save_in_slot].high_score = score;
		new_user_struct.game[save_in_slot].second = tm.tm_sec;
		new_user_struct.game[save_in_slot].minute = tm.tm_min;
		new_user_struct.game[save_in_slot].hour = tm.tm_hour;
		new_user_struct.game[save_in_slot].day = tm.tm_mday;
		new_user_struct.game[save_in_slot].month = tm.tm_mon;
		new_user_struct.game[save_in_slot].year = tm.tm_year + 1900;
	}
	else {
		if (new_user_struct.game[0].is_active == false) {//write to slot 0 if its not active
			new_user_struct.game[0].xp = score;
			new_user_struct.game[0].high_score = score;
			new_user_struct.game[0].second = tm.tm_sec;
			new_user_struct.game[0].minute = tm.tm_min;
			new_user_struct.game[0].hour = tm.tm_hour;
			new_user_struct.game[0].day = tm.tm_mday;
			new_user_struct.game[0].month = tm.tm_mon;
			new_user_struct.game[0].year = tm.tm_year + 1900;
			new_user_struct.game[0].level_difficulty = level_num;

			new_user_struct.game[0].is_active = true;
		}
		else if (new_user_struct.game[1].is_active == false) {//write to slot 1 if its not active
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

			new_user_struct.game[1].is_active = true;

		}
		else if (new_user_struct.game[2].is_active == false) {//write to slot 2 if its not active
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

			new_user_struct.game[2].is_active = true;
		}
		else {//////////////////////////////////////////////////over writes slot 2 if anyway
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

	}
	fwrite(&new_user_struct, sizeof(user), 1, usr_inf);

	system("cls");

	if (win) {
		print_frame(8, 45);

		print("You Have Achieved Great Skills in Typing!!", "", 2, ocpl, false, 45);
		print("", "score: 58", 4, ocpl, true, 45); printf("Score: %d", score);

		countdown(22, 6, 5);

	}
	else {
		print_frame(10, 40);

		print("GAME OVER", "", 2, ocpl, false, 40);

		print("", "score: 58", 4, ocpl, true, 40); printf("Score: %d", score);

		print("", "Wave: 58", 5, ocpl, true, 40); printf("Wave: %d", wave);

		switch (level_num)
		{
		case 1:
			print("Level Difficulty: Easy", "", 6, ocpl, false, 40);
			break;
		case 2:
			print("Level Difficulty: Medium", "", 6, ocpl, false, 40);
			break;
		case 3:
			print("Level Difficulty: Hard", "", 6, ocpl, false, 40);
			break;
		case 4:
			print("Level Difficulty: Left Hand Only", "", 6, ocpl, false, 40);
			break;
		case 5:
			print("Level Difficulty: Right Hand Only", "", 6, ocpl, false, 40);
			break;
		default:
			break;
		}
		countdown(20, 8, 5);
	}
	gotoxy(0, 0);
	system("cls");
	fcloseall();
	exit(0);
}

