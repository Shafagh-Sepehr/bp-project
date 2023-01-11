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
int l_padding(char str[I]);
void ocpl_clean(bool* ocpl);
void menu(bool* ocpl,FILE* usr_inf);
void sign_up(bool *ocpl);
void log_in(void);
void countdown(int x, int y, int sec);
void print(char* str, char* padding, int y, bool* ocpl, bool a,int j);

int main()
{
	bool ocpl[35];//occupied_lines

	//file section
	FILE *usr_inf = fopen("user_info.bin","rb+");
	//

	print_frame(I,J);
	welcome(ocpl);
	getch();//wait for any key to be pressed
	ocpl_clean(ocpl);//clean welcome screen
	menu(ocpl,usr_inf);






	HANDLE thread_id = start_listening(my_callback_on_key_arrival);
	WaitForSingleObject(thread_id, INFINITE);
	return 0;
}
void my_callback_on_key_arrival(char c) {


}

int l_padding(char str[I],int j) {//middle text
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

void print_frame(int _i,int _j) {
	for (int i = 0; i < _i; i++) {
		for (int j = 0; j < _j; j++) {
			printf((i == 0 || j == 0 || i + 1 == _i || j + 1 == _j) ? "*" : " ");
		}
		printf("\n");
	}
}

void welcome(bool* ocpl) {
	
	print("WELCOME", "", 6, ocpl, false ,J);

	
	print("TYPING GAME", "", 8, ocpl, false, J);

	
	print("press any key...", "", 11, ocpl, false, J);

	
	print("by shafagh sepehr", "", 23, ocpl, false, J);


	gotoxy(0, 30);//reset line to the end
}
void print(char* str, char* padding, int y, bool* ocpl,bool a,int j) {// a determines if padding must be used
	if(a)
		gotoxy(l_padding(padding,j), y);
	else
		gotoxy(l_padding(str,j), y);

	printf("%s",str);
	ocpl[y] = false;
}

void menu(bool* ocpl,FILE* usr_inf) {
	
	system("cls");
	print_frame(11,J);


	print("1-Sign Up", "", 4, ocpl, false, J);

	
	print("2-Log In", "1-Sign In", 5, ocpl, true, J);

	
	print("3-Exit", "1-Sign In", 6, ocpl, true, J);

	while (1) {
		int ch = getch();
		ch -= '0';
		switch (ch) {
		case 1:
			system("cls");//clears screen completly
			sign_up(ocpl);
			break;

		case 2:
			ocpl_clean(ocpl);

			break;

		case 3://exit
			ocpl_clean(ocpl);

			print("Thanks For Your Time", "", 11, ocpl, false, J);

			countdown(l_padding("0",J), wherey()+1,3);

			
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

void sign_up(bool* ocpl) {

	print_frame(9, 48);


	char username[20], password[20];
	print("max length = 19 characters", "", 6, ocpl, false, 49);
	print("choose a username: ", "", 2, ocpl,false,J);

	int p = 0;
	do {
		username[p] = getch();
		if (username[p] == '\b') {
			if (p == 0) {

			}
			else {

				gotoxy(wherex() - 2, 2);
				printf(" ");
				gotoxy(wherex() - 2, 2);
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

	print("choose a password: ","",4,ocpl,false,J);

	
	p = 0;
	do {
		password[p] = getch();
		if (password[p] == '\b') {
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
		else if (password[p] != '\r') {
			printf("*");
			p++;
		}
		
	} while (p==0  ||password[p ] != '\r');
	password[p] = '\0';
	

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
