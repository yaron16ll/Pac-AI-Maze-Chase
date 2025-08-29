#include "Node.h"

Node::Node() : row(0), col(0), parent(nullptr), f(0.0), myIndex(-1)
{
}

Node::Node(int r, int c, Node* pr) : row(r), col(c), parent(pr), f(0.0), myIndex(-1)
{

}


int Node::GetRow()
{
	return row;
}

int Node::GetCol()
{
	return col;
}

Node* Node::GetParent()
{
	return parent;
}

double Node::GetF()
{
	return f;
}

void Node::SetRow(int r)
{
	row = r;
}

void Node::SetCol(int c)
{
	col = c;
}

void Node::SetParent(Node* p)
{
	parent = p;
}

void Node::SetF(double dist)
{
	f = dist;
}

void Node::AddNeighbour(int index)
{
	neighbour.push_back(index);
}

vector<int> Node::GetNeighbours()
{
	return neighbour;
}

int Node::GetIndex()
{
	return myIndex;
}

void Node::SetIndex(int index)
{
	myIndex = index;
}
