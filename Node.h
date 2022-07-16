#pragma once

#include <vector>
#include <map>
#include <memory>




class Node
{
public:

	struct compare {bool operator() (std::weak_ptr<Node> a, std::weak_ptr<Node> b) const{ return a.lock()->id < b.lock()->id; } };

	Node(int id,int name);
	Node(int id, int pre, int post, int component);
	
	int id;
	int name;
	int pre = -1;
	int post = -1;
	int component = -1;
	int e = 0;
	double CL = 0;

	std::map < std::weak_ptr<Node>, int, compare > connections;

	void addConnection(std::weak_ptr<Node> node);

};

