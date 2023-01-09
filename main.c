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

#define I 30//line number
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
void print_frame(void);
void welcome(bool* ocpl);
int l_padding(char str[I]);
void ocpl_clean(bool* ocpl);
void menu(bool* ocpl);
void sign_up(void);
void log_in(void);
void show_counter(int x, int y, int sec);

int main()
{
	bool ocpl[35];//occupied_lines
	print_frame();
	welcome(ocpl);
	getch();
	ocpl_clean(ocpl);
	menu(ocpl);






	HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	WaitForSingleObject(thread_id, INFINITE);
	return 0;
}
void my_callback_on_key_arrival(char c) {


}

int l_padding(char str[I]) {//middle text
	return (J - strlen(str)) / 2;
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

void print_frame(void) {
	for (int i = 0; i < I; i++) {
		for (int j = 0; j < J; j++) {
			printf((i == 0 || j == 0 || i + 1 == I || j + 1 == J) ? "*" : " ");
		}
		printf("\n");
	}
}

void welcome(bool* ocpl) {
	gotoxy(l_padding("welcome"), 6);
	printf("WELCOME");
	ocpl[6] = false;

	gotoxy(l_padding("TYPING GAME"), 8);
	printf("TYPING GAME");
	ocpl[8] = false;

	gotoxy(l_padding("press any key..."), 11);
	printf("press any key...");
	ocpl[11] = false;

	gotoxy(l_padding("by shafagh sepehr"), 27);
	printf("by shafagh sepehr");
	ocpl[27] = false;





	gotoxy(0, 30);//reset line to the end
}




void menu(bool* ocpl) {
	gotoxy(l_padding("1-Sign In"), 9);
	printf("1-Sign In");
	ocpl[9] = false;

	gotoxy(l_padding("1-Sign In"), 10);
	printf("2-Log In");
	ocpl[10] = false;

	gotoxy(l_padding("1-Sign In"), 11);
	printf("3-Exit");
	ocpl[11] = false;

	while (1) {
		int ch = getch();
		ch -= '0';
		switch (ch) {
		case 1:
			ocpl_clean(ocpl);

			break;

		case 2:
			ocpl_clean(ocpl);

			break;

		case 3://exit
			ocpl_clean(ocpl);
			gotoxy(l_padding("Thanks For Your Time"), 10);
			printf("Thanks For Your Time");
			show_counter(l_padding("0"), wherey()+1,3);

			
			system("cls");
			exit(0);
			break;
		default:

			break;
		}
	}
}

void show_counter(int x, int y, int sec) {
	for (int i = sec; i >= 0; i--) {
		gotoxy(x, y);
		printf("%d", i);
		_sleep(1000);
	}
}

void sign_up(void) {
	
}

/*
char password[55];

	printf("password:\n");
	int p = 0;
	do {
		password[p] = getch();
		if (password[p] != '\r') {
			printf("");
		}
		p++;
	} while (password[p - 1] != '\r');
	password[p - 1] = '\0';
	printf("press any key to see your password\n");
	getch();
	printf("\nYou have entered %s as password.", password);
*/

/*
this is for time and date
time_t t;
	t = time(NULL);
	struct tm tm;
	tm = *localtime(&t);
	printf("Current Time: %d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	printf("Current Date: %d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
*/