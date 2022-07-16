#pragma once
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <string>
#include <math.h>
#include <iostream>
#include <memory>

//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/connected_components.hpp>
//#include <boost/graph/strong_components.hpp>

#include "Node.h"

class Graph
{
private:
	std::unordered_map<int, int> dict;
	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<std::weak_ptr<Node>> components;
	bool oriented;
	int countEdge = 0;
	std::shared_ptr<Node>  maxComp;

	double aCL, gCL;

	//std::vector<int> comp;
	//boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> g;

	std::shared_ptr<Node> addNode(int name);

	void DFS(int &i, int comp, std::shared_ptr<Node> node);	
public:
	Graph(bool oriented);
	~Graph();

	std::shared_ptr<Node> operator[](int id);

	int getCountComp();
	int getCountNode();
	int getCountEdge();
	int getCountNodeInMaxComp();

	void clusterCoef(std::string filePath);

	void addConnection(int a, int b);
	void findComponents();
	long long findClique();

	//int boostGetcountComp();
	//int boostGetStrongComp();

	void metaGraph(std::string filePath);

	void removeRandomX(int x);

	void sortByDeg();
	void pop_back(int x);

	friend std::ofstream& operator<< (std::ofstream& ofs, Graph& g);
	friend std::ifstream& operator>> (std::ifstream& ifs, Graph& g);
};

