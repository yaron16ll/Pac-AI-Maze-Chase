#pragma once
#include <vector>

using namespace std;

class Node
{
private:
	int row, col;
	Node* parent;
	vector <int> neighbour; // list of adjacent Edges by their indices
	double f; // f = h + g
	int myIndex;

public:
	Node();
	Node(int r, int c, Node* pr);

	int GetRow();
	int GetCol();
	Node* GetParent();
	double GetF();
	vector <int> GetNeighbours();
	int GetIndex();

 	void SetRow(int r);
	void SetCol(int c);
	void SetParent(Node* p);
	void SetF(double dist);
	void AddNeighbour(int index);
	void SetIndex(int index);
};

