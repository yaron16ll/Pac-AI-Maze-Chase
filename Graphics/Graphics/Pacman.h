#pragma once
#include <vector>
#include <queue>
#include <cmath>
#include <iostream>
#include "Node.h"
#include "Ghost.h"

using namespace std;

template <size_t MSZ>
class Pacman {
private:
	int pacmanRow, pacmanCol;   // Current position of Pac-Man
	int coinAmount;      // Total coins in the maze
	int collectedCoins; // Coins collected so far
 

	// Find the nearest coin using BFS
	Node* FindNearestCoin(int maze[MSZ][MSZ], int WALL, int COIN, int SPACE, int PACMAN);

	// Move in the opposite direction of the closest ghost
	Node* MoveAwayFromGhost(vector<Ghost<MSZ>*> ghosts, int maze[MSZ][MSZ], int WALL);

	void SwapCells(int maze[MSZ][MSZ], int SPACE, int PACMAN, int newRow, int newCol);

public:
	// Constructor
	Pacman(int startRow, int startCo);

	// Getters
	int GetRow() { return pacmanRow; }
	int GetCol() { return pacmanCol; }
	int GetCoinAmount() { return coinAmount; }
	int GetCollectedCoins() { return collectedCoins; }


	// Setters
	void SetRow(int r) { pacmanRow = r; }
	void SetCol(int c) { pacmanCol = c; }
	void SetCoinAmount(int c) { coinAmount = c; }
	void SetCollectedCoins(int c) { collectedCoins = c; }

	// Pac-Man's main movement logic
	void Move(int maze[MSZ][MSZ], vector<Ghost<MSZ>*> ghosts, int WALL, int COIN, int SPACE, int PACMAN);

};







// Function implementations
template <size_t MSZ>
Pacman<MSZ>::Pacman(int startRow, int startCol) : pacmanRow(startRow), pacmanCol(startCol), collectedCoins(0), coinAmount(0) {}


template <size_t MSZ>
Node* Pacman<MSZ>::MoveAwayFromGhost(vector<Ghost<MSZ>*> ghosts, int maze[MSZ][MSZ], int WALL) {
	size_t  i;

	if (ghosts.empty())
		return nullptr;

	Ghost<MSZ>* nearestGhost = ghosts[0];
	double minDistance = sqrt(pow(pacmanRow - nearestGhost->GetRow(), 2) + pow(pacmanCol - nearestGhost->GetCol(), 2));

	// Find the closest ghost
	for (i = 1; i < ghosts.size(); i++)
	{
		double distance = sqrt(pow(pacmanRow - ghosts[i]->GetRow(), 2) + pow(pacmanCol - ghosts[i]->GetCol(), 2));
		if (distance < minDistance) {
			minDistance = distance;
			nearestGhost = ghosts[i];
		}
	}


	if (nearestGhost) {
		int newRow = pacmanRow + (pacmanRow - nearestGhost->GetRow());
		int newCol = pacmanCol + (pacmanCol - nearestGhost->GetCol());

		// Validate the new position
		if (newRow >= 0 && newRow < MSZ && newCol >= 0 && newCol < MSZ && maze[newRow][newCol] != WALL) {
			return new Node(newRow, newCol, nullptr);
		}
	}

	return nullptr; // No valid move found
}


template <size_t MSZ>
void Pacman<MSZ>::Move(int maze[MSZ][MSZ], vector<Ghost<MSZ>*> ghosts, int WALL, int COIN, int SPACE, int PACMAN) {
	Node* oppositeNode = MoveAwayFromGhost(ghosts, maze, WALL);

	if (oppositeNode) {
		int newRow = oppositeNode->GetRow();
		int newCol = oppositeNode->GetCol();

		if (maze[newRow][newCol] == COIN) {
			collectedCoins++;
		}
		SwapCells(maze, SPACE, PACMAN, newRow, newCol);

	}
	else {
		Node* nearestCoinNode = FindNearestCoin(maze, WALL, COIN, SPACE, PACMAN);

		if (nearestCoinNode) {
			int newRow = nearestCoinNode->GetRow();
			int newCol = nearestCoinNode->GetCol();

			// Validate position
			if (maze[newRow][newCol] == SPACE || maze[newRow][newCol] == COIN) {
				SwapCells(maze, SPACE, PACMAN, newRow, newCol);
			}

		}
	}
}


template <size_t MSZ>
void Pacman<MSZ>:: SwapCells(int maze[MSZ][MSZ], int SPACE, int PACMAN, int newRow, int newCol) {

	maze[pacmanRow][pacmanCol] = SPACE;
	pacmanRow = newRow;
	pacmanCol = newCol;
	maze[pacmanRow][pacmanCol] = PACMAN;
}



template <size_t MSZ>
Node* Pacman<MSZ>::FindNearestCoin(int maze[MSZ][MSZ], int WALL, int COIN, int SPACE, int PACMAN) {
	queue<Node*> grays;
	bool visited[MSZ][MSZ] = { false };
	const int dx[] = { -1, 1, 0, 0 }; // Up, Down
	const int dy[] = { 0, 0, -1, 1 }; // Left, Right


	// Add the current position to the queue
	grays.push(new Node(pacmanRow, pacmanCol, nullptr));
	visited[pacmanRow][pacmanCol] = true;

	while (!grays.empty()) {
		Node* currentNode = grays.front();
		grays.pop();

		int row = currentNode->GetRow();
		int col = currentNode->GetCol();

		// If a coin is found
		if (maze[row][col] == COIN) {
			collectedCoins++;

			if (collectedCoins == coinAmount) {
				cout << "The Pac-Man Has Won!";
				Ghost<MSZ>::SetIsPacmanEaten(true);
			}

			return currentNode;
		}

		// Check neighbors
		for (int i = 0; i < 4; i++) {
			int newRow = row + dx[i];
			int newCol = col + dy[i];

			if (newRow >= 0 && newRow < MSZ && newCol >= 0 && newCol < MSZ && !visited[newRow][newCol]&& maze[newRow][newCol] != WALL) {
				grays.push(new Node(newRow, newCol, currentNode));
				visited[newRow][newCol] = true;
			}
		}

	}

	return nullptr; // No coins found in the maze
}

