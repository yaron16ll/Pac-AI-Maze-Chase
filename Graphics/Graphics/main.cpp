#define _USE_MATH_DEFINES
#include "glut.h"
#include <math.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <queue>
#include "Node.h"
#include "Ghost.h"
#include "Pacman.h"
#include "CompareNodes.h"
#include <string>

using namespace std;

const int W = 500; // window width
const int H = 500; // window height
const int MSZ = 120;
const int WALL = 1;
const int SPACE = 2;
const int GHOST = 3;
const int PATH = 4;
const int GRAY = 5;
const int BLACK = 6;
const int PACMAN = 7;
const int COIN = 8;
 
int maze[MSZ][MSZ];
int target_row, target_col;

vector<Ghost<MSZ>*> ghosts = { new Ghost<MSZ>() ,new Ghost<MSZ>(),new Ghost<MSZ>() };
Pacman<MSZ>* pacman = new Pacman<MSZ>(0, 0);

bool Ghost<MSZ>::isPacmanEaten = true;

void SetUpMaze();
void init();
void DrawMaze();
void display();
void idle();
void ShowText(string text, float x, float y);
void RunGhosts();
void RunPacman();



void init() {
	glClearColor(0.8f, 0.0f, 0.0f, 0.0f);
	srand(static_cast<unsigned int>(time(0)));
	glOrtho(-1, 1, -1, 1, -1, 1);

	SetUpMaze();
}

bool IsPositionValid(int row, int col) {
	return (maze[row][col] == SPACE && maze[row - 1][col] != WALL && maze[row + 1][col] != WALL && maze[row][col - 1] != WALL && maze[row][col + 1] != WALL);
}

void SetUpMaze() {
	int coinAmount = 0;

	for (int i = 0; i < MSZ; i++) {
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
		maze[0][i] = WALL;
		maze[MSZ - 1][i] = WALL;
	}


	for (int i = 1; i < MSZ - 1; i++) {
		for (int j = 1; j < MSZ - 1; j++) {
			// Generate walls and spaces
			if (rand() % 100 < 10) { // 10% chance for a wall
				maze[i][j] = WALL;
			}
			else {
				maze[i][j] = SPACE;
			}

			// Place a coin only if the cell is SPACE and the neighbors allow accessibility
			if (maze[i][j] == SPACE && rand() % 100 < 10) { // 10% chance for a coin
				// Check if there's at least one accessible neighbor
				bool hasAccessibleNeighbor =
					(i > 1 && maze[i - 1][j] == SPACE) ||
					(i < MSZ - 2 && maze[i + 1][j] == SPACE) ||
					(j > 1 && maze[i][j - 1] == SPACE) ||
					(j < MSZ - 2 && maze[i][j + 1] == SPACE);

				if (hasAccessibleNeighbor) {
					maze[i][j] = COIN;
					coinAmount++;
					pacman->SetCoinAmount(coinAmount);
				}
			}
		}
	}


	do {
		target_row = 1 + rand() % (MSZ - 2);
		target_col = 1 + rand() % (MSZ - 2);
	} while (!IsPositionValid(target_row, target_col));

	maze[target_row][target_col] = PACMAN;
	pacman->SetCol(target_col);
	pacman->SetRow(target_row);


	int start_positions[3][2];
	for (int k = 0; k <3; k++) {
		int row, col;
		do {
			row = 1 + rand() % (MSZ - 2);
			col = 1 + rand() % (MSZ - 2);
		} while (!IsPositionValid(row, col));

		maze[row][col] = GHOST;
		start_positions[k][0] = row;
		start_positions[k][1] = col;
		ghosts[k]->SetCol(col);
		ghosts[k]->SetRow(row);
		ghosts[k]->AddToGrays(new Node(row, col, nullptr));


	}
}




void DrawMaze() {
	double dx = 2.0 / MSZ, dy = 2.0 / MSZ;
	double x, y = -1;

	int ghostCounter = 0;

	for (int i = 0; i < MSZ; i++, y += dy) {
		x = -1;
		for (int j = 0; j < MSZ; j++, x += dx) {
			if (maze[i][j] != COIN) {
				switch (maze[i][j]) {
				case WALL:
					glColor3d(0, 0, 1);
					break;
				case SPACE:
					glColor3d(0, 0, 0);
					break;
				case GHOST:
					if (ghostCounter == 0) glColor3d(0, 1, 0);
					else if (ghostCounter == 1) glColor3d(1.0, 0.078, 0.576);
					else glColor3d(1, 0.5, 0);
					ghostCounter++;
					break;
				case PACMAN:
					glColor3d(1, 1, 0);
					break;
				case BLACK:
					glColor3d(1, 1, 1);
					break;
				case GRAY:
					glColor3d(0.5, 0.5, 0.5);
					break;
				case PATH:
					glColor3d(1, 0.5, 1);
					break;
				}
				glBegin(GL_POLYGON);
				glVertex2d(x, y);
				glVertex2d(x, y + dy);
				glVertex2d(x + dx, y + dy);
				glVertex2d(x + dx, y);
				glEnd();
			}

			if (maze[i][j] == COIN) {
				glColor3d(0, 0, 0);
				glBegin(GL_POLYGON);
				glVertex2d(x, y);
				glVertex2d(x, y + dy);
				glVertex2d(x + dx, y + dy);
				glVertex2d(x + dx, y);
				glEnd();

				glColor3d(1, 1, 1);
				glBegin(GL_POLYGON);
				for (int k = 0; k < 360; k += 10) {
					double angle = k * M_PI / 180;
					double radius = dx / 4;
					glVertex2d(x + dx / 2 + radius * cos(angle), y + dy / 2 + radius * sin(angle));
				}
				glEnd();
			}
		}
	}
}

void DrawCoin(int x, int y, int dx, int dy) {
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x, y);
	glVertex2d(x, y + dy);
	glVertex2d(x + dx, y + dy);
	glVertex2d(x + dx, y);
	glEnd();

	glColor3d(1, 1, 1);
	glBegin(GL_POLYGON);
	for (int k = 0; k < 360; k += 10) {
		double angle = k * M_PI / 180;
		double radius = dx / 4;
		glVertex2d(x + dx / 2 + radius * cos(angle), y + dy / 2 + radius * sin(angle));
	}
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	DrawMaze();

	string text = "Coins: " + to_string(pacman->GetCollectedCoins()) + " / " + to_string(pacman->GetCoinAmount());
	ShowText(text, -0.95f, 0.95f);

	glutSwapBuffers();
}

void ShowText(string text, float x, float y) {
	glColor3d(1, 1, 1);
	glRasterPos2f(x, y);
	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

void idle() {
	if (!Ghost<MSZ>::GetIsPacmanEaten()) {
		RunPacman();
	 	RunGhosts();       
			 
	}
	glutPostRedisplay();
}

void RunPacman() {
	pacman->Move(maze, ghosts, WALL, COIN, SPACE, PACMAN);
}


void RunGhosts() {
	size_t  i;

	for (i = 0; i < ghosts.size(); i++) {
		ghosts[i]->ChasePacMan(maze, SPACE, GRAY, PACMAN, GHOST, BLACK, PATH, pacman->GetCol(), pacman->GetRow());
	}
}


void menu(int choice)
{
	if (choice == 1)
	{
		Ghost<MSZ>::SetIsPacmanEaten(false);
	}

}


void CreateGraphicalWindow(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Pac-Man");
}


void CreateSmallMenu() {
	glutCreateMenu(menu);
	glutAddMenuEntry("Play", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}


int main(int argc, char* argv[]) {

	CreateGraphicalWindow(argc, argv);

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	CreateSmallMenu();

	init();
	glutMainLoop();
	return 0;
}

