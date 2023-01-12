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

#define I 25//line number
#define J 26//column number



#pragma warning(disable:4996)
/*Example codes.
setcolor(1);
gotoxy(0, 5);
system("cls");
*/
typedef struct gameinfo {
	int xp, level;
	//time:
	int year, month, day, hour, minute, second;

	bool is_active;
}gameinfo;
typedef struct user {
	char username[20];
	char password[20];
	gameinfo first_game;
	gameinfo second_game;
	gameinfo third_game;//oldes game
}user;



void my_callback_on_key_arrival(char c);
void print_frame(int _i, int _j);
void welcome(bool* ocpl);
int l_padding(char str[I], int j);
void ocpl_clean(bool* ocpl);
void account_menu(bool* ocpl, FILE* usr_inf);
void sign_up(bool* ocpl, FILE* usr_inf);
void log_in(bool* ocpl, FILE* usr_inf);
void countdown(int x, int y, int sec);
void print(char* str, char* padding, int y, bool* ocpl, bool a, int j);
void get_pass(char* password);
void get_user(char* username);
bool is_user_unique(FILE* usr_inf, char* username);
bool does_username_exist_and_get_user_struct(FILE* usr_inf, char* username, char* realpass);
void main_menu(bool* ocpl, user* user_strcut);


int main()
{
	bool ocpl[35];//occupied_lines

	//file section
	FILE* usr_inf = NULL;
	usr_inf = fopen("user_info.bin", "ab+");

	if (usr_inf == NULL || fseek(usr_inf, 0, SEEK_SET) != 0) {
		fprintf(stderr, "user_info file didn't open properly.");
		return -1;
	}
	//

	print_frame(I, J);
	welcome(ocpl);
	getch();//wait for any key to be pressed
	ocpl_clean(ocpl);//clean welcome screen
	account_menu(ocpl, usr_inf);


	HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	WaitForSingleObject(thread_id, INFINITE);
	return 0;
}
void my_callback_on_key_arrival(char c) {


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

void print_frame(int _i, int _j) {
	gotoxy(0, 0);
	for (int i = 0; i < _i; i++) {
		for (int j = 0; j < _j; j++) {
			printf((i == 0 || j == 0 || i + 1 == _i || j + 1 == _j) ? "*" : " ");
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

void get_pass(char* password) {
	int p = 0;
	do {
		password[p] = getch();
		if (password[p] == '\b') {
			if (p == 0) {

			}
			else {

				gotoxy(wherex() - 2, 6);
				printf(" ");
				gotoxy(wherex() - 2, 6);
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
}

void get_user(char* username) {
	int p = 0;
	do {
		username[p] = getch();
		if (username[p] == '\b') {
			if (p == 0) {

			}
			else {

				gotoxy(wherex() - 2, 4);
				printf(" ");
				gotoxy(wherex() - 2, 4);
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
}

void account_menu(bool* ocpl, FILE* usr_inf) {

	system("cls");
	print_frame(11, J);


	print("1-Sign Up", "", 4, ocpl, false, J);


	print("2-Log In", "1-Sign In", 5, ocpl, true, J);


	print("3-Exit", "1-Sign In", 6, ocpl, true, J);

	while (1) {
		int ch = getch();
		ch -= '0';
		switch (ch) {
		case 1:
			system("cls");//clears screen completly
			sign_up(ocpl, usr_inf);
			break;

		case 2:
			ocpl_clean(ocpl);
			log_in(ocpl, usr_inf);
			break;

		case 3://exit
			ocpl_clean(ocpl);

			print("Thanks For Your Time", "", 4, ocpl, false, J);

			countdown(l_padding("0", J), wherey() + 1, 3);


			system("cls");
			exit(0);
			break;
		default:

			break;
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
	user temp;
	while (fgetc(usr_inf) != EOF) {
		fseek(usr_inf, -1, SEEK_CUR);
		fread(&temp, sizeof(user), 1, usr_inf);
		if (strcmp(temp.username, username) == 1) {
			return false;
		}
	}
	return true;
}

bool does_username_exist_and_get_user_struct(FILE* usr_inf, char* username, user* user_struct) {
	user temp;
	while (fgetc(usr_inf) != EOF) {
		fseek(usr_inf, -1, SEEK_CUR);
		fread(&temp, sizeof(user), 1, usr_inf);
		if (strcmp(temp.username, username) == 1) {
			*user_struct = temp;
			return true;
		}
	}
	return false;

}

void sign_up(bool* ocpl, FILE* usr_inf) {

	print_frame(11, 48);
	user temp;

	char username[20], password[20];
	while (1) {
		print("SIGN UP", "", 2, ocpl, false, 48);
		print("max length = 19 characters", "", 8, ocpl, false, 49);



		print("choose a username: ", "", 4, ocpl, false, J);
		get_user(username);

		print("                          ", "choose a password: ", 6, ocpl, true, J);//clears username is taken massage if it's there
		print("choose a password: ", "", 6, ocpl, false, J);
		get_pass(password);

		if (is_user_unique(usr_inf, username))
			break;
		else {
			system("cls");
			print_frame(11, 48);
			print("username is already taken", "", 6, ocpl, false, J);
		}
	}
	strcpy(temp.password, password);
	strcpy(temp.username, username);
	fseek(usr_inf, 0, SEEK_END);
	fwrite(&temp, sizeof(user), 1, usr_inf);
	//....................................................................................................
	log_in(ocpl,usr_inf);
}

void log_in(bool* ocpl, FILE* usr_inf) {
	bool is_credentials_right=true;
	print_frame(11, 40);
	user user_struct;

	char username[20], password[20],realpass[20];
	while (1) {
		print("LOG IN", "", 2, ocpl, false, 40);
		print("max length = 19 characters", "", 8, ocpl, false, 41);
		


		print("username: ", "", 4, ocpl, false, J);
		get_user(username);
		
		if (!does_username_exist_and_get_user_struct(usr_inf, username,&user_struct))
			is_credentials_right = false;
			
			
		print("password: ", "", 6, ocpl, false, J);
		get_pass(password);
		if (strcmp(password, user_struct.password) != 0) {
			is_credentials_right = false;
		}
		
		if (is_credentials_right) {
			break;
		}
		else {
			print_frame(11, 40);
			print("wrong credentials", "", 6, ocpl, false, 40);
			print("1-try again 2-return back", "", 8, ocpl, false, 40);

			while (1) {
				int ch = getch();
				ch -= '0';
				switch (ch) {
				case 1:
					system("cls");//clears screen completly
					log_in(ocpl, usr_inf);
					break;

				case 2:
					account_menu(ocpl,usr_inf);
					break;

				default:

					break;
				}
			}

		}
	}
	system("cls");
	main_menu(ocpl,&user_struct);
}
void main_menu(bool* ocpl, user* user_struct) {
	print_frame(15, J);
	print("1-new game", "", 6, ocpl, false, J);
	print("2-load game", "", 6, ocpl, false, J);
	print("3-change password", "", 6, ocpl, false, J);



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
