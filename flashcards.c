#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define RESET 		0
#define BRIGHT		1
#define DIM		2
#define UNDERLINE	3
#define BLINK		4
#define REVERSE		7
#define HIDDENT		8

#define BLACK		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define WHITE		7

typedef struct {
	int a;
	int b;
	int neg_count;
} Problem;

struct {
	int correct;
	int count;
	int table;
	int max;
	int last;
	Problem *incorrect[10];
	char function;
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

void color(int attr, int fg, int bg)
{
	char com[13];
	sprintf(com, "%c[%d;%d;%dm", 0x1B, attr, fg+30, bg+40);
	printf("%s", com);
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
		printf("\nSelect function to practice (+-*/): ");
		scanf("%c", &scores.function);
		while(getchar() != '\n') continue;
	} while (strchr("+-*/", scores.function) == NULL);

	printf("Select the table to practice: ");
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
		printf("%% Correct %10.2f\n\n", percent);
	} else {
		printf("%% Correct: \n\n");
	}

	if(scores.count > 0)
	{
		if(scores.last == 1) { printf("CORRECT!\t");
		} else {
			printf("INCORRECT\t");
			printf("%d %c %d = %d\n\n", last_problem.a, scores.function, last_problem.b, last_problem.answer);
		}
	}

	printf("\n\n");
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
		multiplicand = rand() % (scores.max + 1);
		multiplier   = rand() % (scores.max + 1);
	} else {
		position = rand() % 2;
		printf("Position of Table: %d\n", position);
		if(position == 0)
		{
			multiplicand = scores.table;
			multiplier   = rand() % (scores.max + 1);
		} else {
			multiplier = scores.table;
			multiplicand = rand() % (scores.max + 1);
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
	srand(time(0));
	entrance();
	select_table();
	while(true) { clear_screen(); score_report(); problem(); }
	return 1;
}
