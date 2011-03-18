#include <assert.h>
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
	int correct;
	int count;		/* number of problems completed */
	int table;
	int max;
	int min;		/* min number to test (some modes) */
	int mode;		/* current mode of testing */
	int swap;		/* computer changes which position table is in */
	int last;
	int retry;		/* retry same problem on bad answer */
	char function;	/* stores function to drill */
} scores;

struct {
	int a;
	int b;
	int answer;
} last_problem;

void clear_screen()
{
	system("clear");
}

/* generate random number from min to max */
int random_int(int min, int max){
	assert(min > INT_MIN);
	assert(max < INT_MAX / 2);
	assert(max >= min);
	return rand() % max + min;
}

/* seed random number generator */
void random_seed(){ srand(time(0)); }

void color(int attr, int fg, int bg)
{
	printf("%c[%d;%d;%dm", 0x1B, attr, fg+30, bg+40);
}

void entrance()
{
	clear_screen();
	color(BRIGHT, RED, BLACK);
	printf("\n\nWelcome to Flashcards.\n\nPlease select the function you would like to practice.\n");
	printf("To practice a set of tables, please enter the number.\nEntering Zero will use all tables.\n");
	color(RESET, WHITE, BLACK);
}

void select_table()
{
	do {
		printf("\nSelect function to practice (");
		color(BRIGHT, GREEN, BLACK);
		printf("+-*/");
		color(RESET, WHITE, BLACK);
		printf("): ");
		scanf("%c", &scores.function);
		while(getchar() != '\n') continue;
	} while (strchr("+-*/", scores.function) == NULL);

	printf("Select the table to practice (0 for all):");
	scanf("%d", &scores.table);

	printf("Enter max number in set: ");
	scanf("%d", &scores.max);
}

void score_report()
{
	float percent = 0.0;

	if(scores.count > 0) { percent = (((float)scores.correct / (float)scores.count) * 100); }
	printf("\n\nCount:   %d\n", scores.count);
	printf(    "Correct: %d\t\tIncorrect: %d\n", scores.correct, scores.count-scores.correct);
	if(scores.count > 0)
	{
		printf("%% Correct %10.2f\n", percent);
	} else {
		printf("%% Correct: \n");
	}

	if(scores.count > 0)
	{
		if(scores.last == 1) {
			color(BRIGHT, GREEN, BLACK);
			printf("CORRECT!\t");
			
		} else {
			color(BRIGHT, RED, BLACK);
			printf("INCORRECT\t");
			printf("%d %c %d = %d", last_problem.a, scores.function, last_problem.b, last_problem.answer);
		}
	}
	color(RESET, WHITE, BLACK);
	printf("\n");
}

void problem()
{
	int answer = 0;
	int multiplier = 0;
	int multiplicand = 0;
	int position = 0;
	int response;
	int correct;

	while(getchar() != '\n') continue;

	if(scores.table == 0)
	{
		multiplicand = random_int(scores.min, scores.max);
		multiplier   = random_int(scores.min, scores.max);
	} else {
		position = rand() % 2;
		if(position == 0)
		{
			multiplicand = scores.table;
			multiplier   = random_int(scores.min, scores.max);
		} else {
			multiplier = scores.table;
			multiplier   = random_int(scores.min, scores.max);
		}
	}

	printf("%d %c %d = ", multiplicand, scores.function, multiplier);
	scanf("%d", &response);

	switch(scores.function)
	{
		case '+':
			correct = multiplier + multiplicand;
			if(response == correct) { answer = true; }
			break;
		case '-':
			correct = multiplier - multiplicand;
			if(response == correct) { answer = true; }
			break;
		case '/':
			correct = multiplier / multiplicand;
			if(response == correct) { answer = true; }
			break;
		case '*':
			correct = multiplier * multiplicand;
			if(response == correct) { answer = true; }
			break;
	}

	last_problem.a = multiplicand;
	last_problem.b = multiplier;
	last_problem.answer = correct;

	if(answer == true)
	{
		scores.correct++;
		scores.last = 1;
	} else {
		scores.last = 0;
	}
	scores.count++;
}

int main()
{
	random_seed();
	entrance();
	select_table();
	while(true) { clear_screen(); score_report(); problem(); }
	return 1;
}
