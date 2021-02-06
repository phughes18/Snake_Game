#include <cstdlib>
#include <curses.h>

bool gameEnd;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
enum Direction{STOP = 0, LEFT, RIGHT, UP, DOWN};
Direction dir;

int tailX[100], tailY[100];
int nTail = 0;

// Initializes game conditions, fruit location, board size
void Begin()
{
	initscr();
	clear();
	noecho();
	cbreak();
	curs_set(0);

	gameEnd = false;
	dir = STOP;
	x = width/2;
	y = height/2;
	fruitX = ((rand() % width) + 1);
	fruitY = ((rand() % width) + 1);
	score = 0;
}

// Sets parameters for board size, prints fruit location and tail
void Draw()
{
	clear();

	for (int i=0; i< width + 2; i++)
		mvprintw(0, i, "+");

	for (int i=0; i < height + 2; i++)
	{
		for (int j=0; j < width + 2; j++)
		{
			if (i==0 | i==21)
				mvprintw(i, j, "#");
			else if (j==0 | j==21)
				mvprintw(i, j, "+");
			else if (i==y && j==x)
				mvprintw(i, j, "0");
			else if (i==fruitY && j==fruitX)
				mvprintw(i, j, "0");
			else
				for(int k=0; k < nTail; k++)
				{
					if (tailX[k]==j && tailY[k] == i)
						mvprintw(i, j, "o");
				}
		}
	}

	mvprintw(23, 0, "Score %d", score);
	refresh();
}

void Input()
{
	keypad(stdscr, TRUE);
	halfdelay(1);
	int n = getch();

	// associates keystrokes with direction of snake
	switch(n)
	{
		case KEY_LEFT:
			dir = LEFT;
			break;
		case KEY_RIGHT:
			dir = RIGHT;
			break;
		case KEY_UP:
			dir = UP;
			break;
		case KEY_DOWN:
			dir = DOWN;
			break;
		case 113:
			gameEnd = true;
			break;
	}
}

void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;

	tailX[0] = x;
	tailY[0] = y;

	for (int i=1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	// associates keystroke with movement in an x or y direction
	switch (dir)
	{
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		default:
			break;
	}

	// if snake head intersects border then game over
	if (x > width || x < 1 || y > height)
	{
		gameEnd = true;
	}

	// when snake intersects fruit add to score and 
	// randomly generate new fruit location
	if (x == fruitX && y == fruitY)
	{
		score++;
		fruitX = (rand() % width) + 1;
		fruitY = (rand() % height) + 1;
		nTail++;
	}

	// if snake head intersects tail then game over
	for (int i=0; i < nTail; i++)
	{
		if (tailX[i]==x && tailY[i]==y)
			gameEnd = true;
	}

}


int main()
{

	Begin();

	while (!gameEnd)
	{
		Draw();
		Input();
		Logic();
	}

	getch();
	endwin();


	return 0;
}











