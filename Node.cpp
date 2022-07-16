#include "Node.h"
#include <iostream>
#include <algorithm>

Node::Node(int id, int pre, int post, int component)
{
	this->id = id;
	this->pre = pre;
	this->post = post;
}

Node::Node(int id, int name)
{
	this->id = id;
	this->name = name;
}

void Node::addConnection(std::weak_ptr<Node> node)
{
	++connections[node];
}