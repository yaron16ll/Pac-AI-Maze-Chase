#pragma once
#include <vector>
#include <queue>
#include "Node.h"
#include "CompareNodes.h"
#include <cmath>
#include <iostream>


using namespace std;
template <size_t MSZ>


class Ghost {
private:
	int row, col;
	Node* parent;
	priority_queue<Node*, vector<Node*>, CompareNodes> grays;
	static  bool isPacmanEaten;

public:
	// Constructors
	Ghost() : row(0), col(0), parent(nullptr) {}
	Ghost(int r, int c, Node* pr) : row(r), col(c), parent(pr) {}

	// Getters
	int GetRow() { return row; }
	int GetCol() { return col; }
	Node* GetParent() { return parent; }
	priority_queue<Node*, vector<Node*>, CompareNodes> GetGrays() { return grays; }
	static bool GetIsPacmanEaten() { return isPacmanEaten; }

	// Setters
	void SetRow(int r) { row = r; }
	void SetCol(int c) { col = c; }
	void SetParent(Node* p) { parent = p; }
	static void SetIsPacmanEaten(bool c) { isPacmanEaten = c; }


	// Maze functions
	void BuildPath(Node* pCurrent, int maze[MSZ][MSZ], int GHOST, int PATH,int PACMAN);
	void ChasePacMan(int maze[MSZ][MSZ], int SPACE, int GRAY, int PACMAN, int GHOST, int BLACK, int PATH, int target_col, int target_row);
	void CheckSolution(int maze[MSZ][MSZ], int SPACE, int GRAY, int PACMAN, int GHOST, int BLACK, int PATH, int target_col, int target_row);
	void ShowResult(Node* pCurrent, int maze[MSZ][MSZ], int GHOST, int PATH, int PACMAN);
	void CheckDirection(int row, int col, Node* pCurrent, int maze[MSZ][MSZ], int SPACE, int GRAY, int PACMAN, int PATH, int GHOST, int target_col, int target_row);
	void AddToGrays(Node* node);

	// Heuristic functions
	double ComputeH(Node* node, int target_col, int target_row);
	double ComputeG(Node* node);
};



template <size_t MSZ>
void Ghost<MSZ>::BuildPath(Node* pCurrent, int maze[MSZ][MSZ], int GHOST, int PATH, int PACMAN) {
	int row, col;

	row = pCurrent->GetRow();
	col = pCurrent->GetCol();

	while (maze[row][col] != GHOST) {
	if (maze[row][col] != PACMAN)
	{
		maze[row][col] = PATH;
	}
	else
	{
		maze[row][col] = PACMAN;
	}
		pCurrent = pCurrent->GetParent();
		row = pCurrent->GetRow();
		col = pCurrent->GetCol();
	}
}

template <size_t MSZ>
void Ghost<MSZ>::ChasePacMan(int maze[MSZ][MSZ], int SPACE, int GRAY, int PACMAN, int GHOST, int BLACK, int PATH, int target_col, int target_row) {
		CheckSolution(maze, SPACE, GRAY, PACMAN, GHOST, BLACK, PATH, target_col, target_row);
}

template <size_t MSZ>
void Ghost<MSZ>::CheckSolution(int maze[MSZ][MSZ], int SPACE, int GRAY, int PACMAN, int GHOST, int BLACK, int PATH, int target_col, int target_row) {
	Node* pCurrent;
	int row, col;


	pCurrent = grays.top();
	grays.pop();	// remove the first element

	row = pCurrent->GetRow();
	col = pCurrent->GetCol();

	// check if it is "the solution"
	if (maze[row][col] == PACMAN) // the solution has been found
	{
		ShowResult(pCurrent, maze, GHOST, PATH, PACMAN);
	}
	else // add WHITE neighbours to gray
	{
		// paint pCurrent to BLACK
		if (maze[row][col] != GHOST)
			maze[row][col] = BLACK;

		CheckDirection(row + 1, col, pCurrent, maze, SPACE, GRAY, PACMAN, PATH, GHOST, target_col, target_row);
		CheckDirection(row - 1, col, pCurrent, maze, SPACE, GRAY, PACMAN, PATH, GHOST, target_col, target_row);
		CheckDirection(row, col - 1, pCurrent, maze, SPACE, GRAY, PACMAN, PATH, GHOST, target_col, target_row);
		CheckDirection(row, col + 1, pCurrent, maze, SPACE, GRAY, PACMAN, PATH, GHOST, target_col, target_row);
	}

}


template <size_t MSZ>
void Ghost<MSZ>::ShowResult(Node* pCurrent, int maze[MSZ][MSZ], int GHOST, int PATH, int PACMAN) {
	cout << "The Pac-Man Has Been Eaten!\nThe Ghosts have won!";
	SetIsPacmanEaten(true);
	BuildPath(pCurrent, maze, GHOST, PATH, PACMAN);
}

template <size_t MSZ>
void Ghost<MSZ>::CheckDirection(int row, int col, Node* pCurrent, int maze[MSZ][MSZ], int SPACE, int GRAY, int PACMAN, int PATH, int GHOST, int target_col, int target_row) {
	Node* tmpNode;

	if (maze[row][col] == SPACE)
	{
		maze[row][col] = GRAY;
		tmpNode = new Node(row, col, pCurrent);
		tmpNode->SetF(ComputeH(tmpNode, target_col, target_row) + ComputeG(tmpNode));
		grays.push(tmpNode);
	}
	else if (maze[row][col] == PACMAN)// this is a target
	{
		ShowResult(pCurrent, maze, GHOST, PATH, PACMAN);
	}
}


template <size_t MSZ>
void Ghost<MSZ>::AddToGrays(Node* node) { grays.push(node); }


template <size_t MSZ>
double Ghost<MSZ>::ComputeH(Node* node, int target_col, int target_row) {
	int row1, col1, row2, col2;

	row1 = node->GetRow();
	col1 = node->GetCol();
	row2 = target_row;
	col2 = target_col;

	return sqrt(pow((row1 - row2), 2) + pow((col1 - col2), 2));
}

template <size_t MSZ>
double Ghost<MSZ>::ComputeG(Node* node) {
	int row1, col1, row2, col2;

	row1 = node->GetRow();
	col1 = node->GetCol();
	row2 = MSZ / 2;
	col2 = MSZ / 2;

	return sqrt(pow((row1 - row2), 2) + pow((col1 - col2), 2));
}
