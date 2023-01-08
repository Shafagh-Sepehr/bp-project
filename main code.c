#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "helper_windows.h"
#include "colorize.h"
#pragma warning(disable:4996)
/*Example codes.
setcolor(1);
gotoxy(0, 5);
*/
void my_callback_on_key_arrival(char c);

int main()
{


	HANDLE thread_id = start_listening(my_callback_on_key_arrival);


	WaitForSingleObject(thread_id, INFINITE);
	return 0;
}
void my_callback_on_key_arrival(char c) {
	if (c = 'a') {
		printf("Hello world");
	}
}
