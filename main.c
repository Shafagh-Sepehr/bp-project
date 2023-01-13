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

#define I 25//line number
#define J 26//column number



#pragma warning(disable:4996)
/*Example codes.
setcolor(1);
gotoxy(0, 5);
system("cls");
*/

typedef struct gameinfo {
	int xp, level_difficulty,score;
	//time:
	int year, month, day, hour, minute, second;

	bool is_active;
}gameinfo;

typedef struct user {
	char username[20];
	char password[20];
	//gameinfo first_game;
	//gameinfo second_game;
	//gameinfo third_game;//oldest game
}user;



void my_callback_on_key_arrival(char c);
void print_frame(int _i, int _j);
void print_frame2(int _i, int _j);
void welcome(bool* ocpl);
int l_padding(char str[I], int j);
void ocpl_clean(bool* ocpl);
void account_menu(bool* ocpl, FILE* usr_inf, user* user_struct);
void sign_up(bool* ocpl, FILE* usr_inf);
bool log_in(bool* ocpl, FILE* usr_inf, user* user_struct);
void countdown(int x, int y, int sec);
void print(char* str, char* padding, int y, bool* ocpl, bool a, int j);
void get_pass(char* password);
void get_user(char* username);
bool is_user_unique(FILE* usr_inf, char* username);
bool does_username_exist_and_get_user_struct(FILE* usr_inf, char* username, user* user_struct);
int main_menu(bool* ocpl, FILE* usr_inf, user* user_strcut);
void change_password(bool* ocpl,  FILE* usr_inf ,user* user_struct);

int main()
{
	srand(time(NULL));

	
	printf("\033[;47m");
	setcolor(0);//0-black 1-bit.Dark.Blue 2-dark.green 3-dark.cyan 4-red 5-dark.purple 6-orange 7-white 8-grey 9-blue 10-green 11-cyan 12-light.red 
				//13-purple 14-very.light.orange 
	//printf("hello    world");
	system("cls");

	
	//return 0;
	user user_struct;
	bool ocpl[35];//occupied_lines

	//file section
	FILE* usr_inf = NULL;
	FILE* words[15] = {NULL}; // 0 normal-easy 1 normal-medium 2 normal-hard // 3 long-easy 4 long-medium 5 long-hard 
							// 6 difficult-easy 7 difficult-medium 8 difficult-hard // 9 left-medium 10 right-medium
	//words[] = fopen("words__.bin", "w+");

	usr_inf = fopen("user_info.bin", "rb+");

	if (usr_inf == NULL || fseek(usr_inf, 0, SEEK_SET) != 0) {
		fprintf(stderr, "user_info file didn't open properly.");
		return -1;
	}
	//////

	print_frame(I, J);
	welcome(ocpl);
	getch();//wait for any key to be pressed

	do{
	account_menu(ocpl, usr_inf, &user_struct);
	} while (main_menu(ocpl, usr_inf, &user_struct) == 1);

	

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

void print_frame2(int _i, int _j) {
	gotoxy(0, 0);
	for (int i = 0; i < _i; i++) {
		for (int j = 0; j < _j; j++) {
			//printf((i == 0 || j == 0 || i + 1 == _i || j + 1 == _j) ? "*" : " ");
			if ((i == 0 && j==0)||(i==0 && j+1==_j))
				printf("%c",220);
			else if ( j == 0 ||   j + 1 == _j)
				printf("%c",219);
			else if(i == 0|| i + 1 == _i)
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
			else if (j == 0 && i+1==_i)
				printf("%c", 200);
			else if (j+1 == _j && i + 1 == _i)
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

void get_pass(char* password) {
	int p = 0;
	do {
		password[p] = getch();
		if (password[p] == '\b') {
			if (p == 0) {

			}
			else {

				gotoxy(wherex() - 2, wherey()-1);
				printf(" ");
				gotoxy(wherex() - 2, wherey()-1);
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

				gotoxy(wherex() - 2, wherey()-1);
				printf(" ");
				gotoxy(wherex() - 2, wherey()-1);
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

void account_menu(bool* ocpl, FILE* usr_inf, user* user_struct) {



	while (1) {

		system("cls");
		print_frame(11, J);


		print("1-Sign Up", "", 4, ocpl, false, J);


		print("2-Log In", "1-Sign In", 5, ocpl, true, J);


		print("3-Exit", "1-Sign In", 6, ocpl, true, J);


		int ch = getch();
		ch -= '0';
		switch (ch) {
		case 1:
			system("cls");//clears screen completly
			sign_up(ocpl, usr_inf);
			break;

		case 2:
			ocpl_clean(ocpl);
			if (log_in(ocpl, usr_inf, user_struct))
				return;
			break;

		case 3://exit
			ocpl_clean(ocpl);

			print("Thanks For Your Time", "", 4, ocpl, false, J);

			countdown(l_padding("0", J), wherey() + 1, 3);


			system("cls");
			fclose(usr_inf);
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
		print("Choose A Psername: ", "", 4, ocpl, false, J);
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

	strcpy(temp.password, password);
	strcpy(temp.username, username);
	fseek(usr_inf, 0, SEEK_END);
	fwrite(&temp, sizeof(user), 1, usr_inf);
	fseek(usr_inf, 0, SEEK_SET);

	//....................................................................................................
	return;
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
		if (strcmp(password, user_struct->password) != 0) {
			is_credentials_right = false;
		}

		if (is_credentials_right) {
			break;
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


}

void change_password(bool* ocpl,  FILE* usr_inf, user* user_struct) {
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

		if (strcmp(curPassword, user_struct->password) != 0) {
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

	strcpy(user_struct->password, newPassword);
	

	fwrite(user_struct, sizeof(user),1,usr_inf);
	fseek(usr_inf, -1*sizeof(user), SEEK_CUR);

	return;

}

int main_menu(bool* ocpl,FILE* usr_inf, user* user_struct) {
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

			break;

		case 2:

			break;

		case 3:
			change_password(ocpl,usr_inf, user_struct);
			break;
		case 4:
			return 1;
			break;
		case 5:
			ocpl_clean(ocpl);

			print("Thanks For Your Time", "", 4, ocpl, false, J);

			countdown(l_padding("0", J), wherey() + 1, 3);


			system("cls");
			fclose(usr_inf);
			exit(0);
			break;
		default:

			break;
		}
	}
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
