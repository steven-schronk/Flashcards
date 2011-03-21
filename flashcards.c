#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RESET 		0
#define BRIGHT		1
#define DIM			2
#define UNDERLINE	3
#define BLINK		4
#define REVERSE		7
#define HIDDENT		8

#define BLACK		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define WHITE		7

struct {
	int correct;	/* number of problems counted correct */
	int count;		/* number of problems completed */
	int table;
	int max;		/* max number to test (all modes) */
	int min;		/* min number to test (some modes) */
	int mode;		/* current mode of testing */
	int swap;		/* computer changes which position table is in */
	int last;		/* was last problem correct? */
	int retry;		/* retry same problem on bad answer */
	int a;			/* first part of problem */
	int b;			/* second part of problem */
	int answer;		/* answer to problem */
	char func;		/* stores function to drill */
} s = { 0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,' ' };

void color(int attr, int fg, int bg){
	printf("%c[%d;%d;%dm", 0x1B, attr, fg+30, bg+40);
}

void clear_screen(){ system("clear"); }

/* generate random number from min to max */
int random_int(int min, int max){
	assert(min > INT_MIN);
	assert(max < INT_MAX / 2);
	assert(max >= min);
	return rand() % max + min;
}

/* seed random number generator */
void random_seed(){ srand(time(0)); }

void get_function(){
	do{
		while(getchar() != '\n'); /* clear stdin buffer */
		printf("\nSelect Function to Drill (");
		color(BRIGHT, GREEN, BLACK);
		printf("+*-/");
		color(RESET, WHITE, BLACK);
		printf("): ");
		scanf("%c", &s.func);
	}while(s.func != '+' && s.func != '*' && s.func != '-' && s.func != '/');
}

void get_table(){
	do{
		while(getchar() != '\n'); /* clear stdin buffer */
		printf("\nEnter Number of Table to Drill: ");
		scanf("%d", &s.table);
	}while(s.table <= 0);
}

void get_swap(){
	do{
		while(getchar() != '\n'); /* clear stdin buffer */
		printf("\nSwap Values in Problem (");
		color(BRIGHT, GREEN, BLACK);
		printf("Yy/Nn");
		color(RESET, WHITE, BLACK);
		printf("): ");
		s.swap = tolower(getchar());
	}while(s.swap != 'y' && s.swap != 'n');
	if(s.swap == 'y') { s.swap = 1; } else { s.swap = 0; }
}

void get_retry(){
	do{
		while(getchar() != '\n'); /* clear stdin buffer */
		printf("\nRetry on Bad Answers (");
		color(BRIGHT, GREEN, BLACK);
		printf("Yy/Nn");
		color(RESET, WHITE, BLACK);
		printf("): ");
		s.retry = tolower(getchar());
	}while(s.retry != 'y' && s.retry != 'n' );
	if(s.retry == 'y') { s.retry = 1; } else { s.retry = 0; }
}

void get_min_max(){
	int count = 0;
	do{
		while(getchar() != '\n'); /* clear stdin buffer */
		printf("\nEnter Minimum Value: ");
		scanf("%d",&s.min);
	}while(s.min <= 0);
	do{
		while(getchar() != '\n'); /* clear stdin buffer */
		if(s.min > s.max && count > 0) { printf("Max must be more than Min.\n"); }
		printf("\nEnter Maximum Value: ");
		scanf("%d",&s.max);
		++count;
	}while(s.max < s.min);
}

void menu_main(){
	clear_screen();
	color(BRIGHT, BLUE, BLACK);
	printf("\n\nWelcome to Flashcards.\n\nPlease select the function you would like to practice.\n");
	printf("To practice a set of tables, please enter the number.\nEntering Zero will use all tables.\n\n");
	color(BRIGHT, RED, BLACK);
	printf("You can press 'Q' or 'q' at any time to quit.\n\n");
	color(RESET, WHITE, BLACK);
	printf("MAIN MENU\n");
	printf("\t1. Drill One Table\n");
	printf("\t2. Drill Multiple Tables\n");
	printf("\t3. Drill All Tables and Functions\n");
	printf("\n Please Enter Your Selection: ");
}

void menu_one_table(){
	get_function();
	get_table();
	get_min_max();
	get_swap();
	get_retry();
}

void menu_mult_table(){
	get_function();
	get_min_max();
	get_retry();
}

void menu_all(){
	get_min_max();
	get_retry();
}

void score_report(){
	float percent = 0.0;

	if(s.count > 0) { percent = (((float)s.correct / (float)s.count) * 100); }
	printf("\n\nCount:   %d\n", s.count);
	printf("Correct: %d\nIncorrect: %d\n", s.correct, s.count-s.correct);
	if(s.count > 0){
		printf("%% Correct:\t%10.2f\n", percent);
	} else {
		printf("%% Correct: \n");
	}

	if(s.count > 0){
		if(s.last == 1){
			color(BRIGHT, GREEN, BLACK);
			printf("CORRECT!\t");
		} else {
			color(BRIGHT, RED, BLACK);
			printf("INCORRECT\t");
			if(s.mode == 1){
				printf("%d %c %d = %d", s.a, s.func, s.table, s.answer);
			} else {
				printf("%d %c %d = %d", s.a, s.func, s.b, s.answer);
			}
		}
	}
	color(RESET, WHITE, BLACK);
	printf("\n");
}

void run_probs()
{
	int cont = 1;
	int temp_ans;
	s.b = s.table; /* this for mode 1 */
	while(cont){
		score_report();
		/* generate random data for problem */
		s.a = random_int(s.min,s.max);
		if(s.mode != 1) { s.b = random_int(s.min,s.max); }

		/* generate new function for mode 3 */
		if(s.mode == 3) {
			temp_ans = random_int(0,5);
			switch(temp_ans){
				case 0:
					s.func = '+';
					break;
				case 1:
					s.func = '-';
					break;
				case 2:
					s.func = '*';
					break;
				case 3:
					s.func = '/';
					break;
			}
		}
		/* calculate answer for problem based on table*/
		switch(s.func){
			case '+':
				s.answer = s.a + s.b;
				break;
			case '-':
				s.answer = s.a - s.b;
				break;
			case '*':
				s.answer = s.a * s.b;
				break;
			case '/':
				s.answer = s.a / s.b;
				break;
		}
		/* print problem on screen with prompt */
		if(s.swap){
			temp_ans = random_int(0,2);
			if(temp_ans == 1){
				printf("%d %c %d = ", s.a, s.func, s.b);
			}else{
				printf("%d %c %d = ", s.b, s.func, s.a);
			}
		}else{
			printf("%d %c %d = ", s.a, s.func, s.b);
		}
		/* get user answer */
		scanf("%d", &temp_ans);
		printf("Answer received:%d\n", temp_ans);
		if(tolower(temp_ans) == 'q'){ break; }
		/* test for correct answer */
		if(temp_ans == s.answer){
			s.correct++;
			s.last = 1;
		} else { s.last = 0; }
		s.count++;
	}
}

int main()
{
	char opt;
	random_seed();
	menu_main();
	opt = getchar();
	switch(opt){
		case '1':
			menu_one_table();
			s.mode = 1;
			break;
		case '2':
			menu_mult_table();
			s.mode = 2;
			break;
		case '3':
			menu_all();
			s.mode = 3;
			break;
		default:
			menu_main();
			break;
	}
	run_probs();
	return EXIT_SUCCESS;
}
