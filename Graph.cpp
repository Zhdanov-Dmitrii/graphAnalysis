#include "Graph.h"


Graph::Graph(bool oriented)
{
	this->oriented = oriented;
}

Graph::~Graph()
{
}

std::shared_ptr<Node>  Graph::operator[](int id)
{
	return nodes[id];
}

int Graph::getCountComp()
{
	return components.size();
}

int Graph::getCountNode()
{
	return nodes.size();
}

int Graph::getCountEdge()
{
	return countEdge;
}

int Graph::getCountNodeInMaxComp()
{
	int res = 0;
	int comp = maxComp->component;

	for (auto n : nodes)
		if (n->component == comp)
			res++;

	return res;
}

void Graph::clusterCoef(std::string filePath)
{
	for (auto n : nodes)
	{
		double k = n->connections.size();
		/*for (auto e : n->connections)
			k += e.second;*/

		if (k >= 2)
			n->CL = 2. * n->e / (k * (k - 1));
		else
			n->CL = 0;
	}

	aCL = 0;
	for (auto n : nodes)
		aCL += n->CL;
	aCL /= nodes.size();

	double CnCL = 0, Cn = 0;
	for (auto n : nodes)
	{
		double Cnv = n->connections.size() * (n->connections.size() - 1) / 2;
		CnCL += n->CL * Cnv;
		Cn += Cnv;
	}

	gCL = CnCL / Cn;

	std::ofstream fout(filePath);
	
	fout << aCL << std::endl;
	fout << gCL << std::endl;

	for (auto n : nodes)
		fout << n->name << " " << n->CL << std::endl;

	fout.close();
}

void Graph::addConnection(int a, int b)
{
	auto f = addNode(a);
	auto s = addNode(b);
	
	f->addConnection(s);
	
	if (!oriented && f != s)
		s->addConnection(f);
		
	//boost::add_edge(f->id, s->id, g);
	++countEdge;
}

std::shared_ptr<Node> Graph::addNode(int name)
{
	std::unordered_map<int,int>::iterator it = dict.find(name);
	if (it == dict.end())
	{
		it = dict.emplace(name, nodes.size()).first;
		
		nodes.push_back(std::make_shared<Node>(nodes.size(),name));
	}

	return nodes[(*it).second];
}

void Graph::DFS(int &i, int comp, std::shared_ptr<Node> node)
{
	node->pre = i;
	node->component = comp;
	for (auto &[n,val] : node->connections)
		if (n.lock()->pre == -1)
			DFS(++i, comp, n.lock());

	node->post = ++i;
}


void Graph::findComponents()
{
	int max = 0;
	if (!oriented)
	{
		
		int comp = 0;
		int i = 0;

		for (auto n : nodes)
			if (n->component == -1)
			{
				components.push_back(n);
				DFS(i, comp, n);

				int size = components.back().lock()->post - components.back().lock()->pre + 1;
				if (size > max)
				{
					maxComp = n;
					max = size;
				}
				++comp;
			}
	}
	else
	{
		Graph rg(false); // считаем его как неорграф, но по списку смежности он орграф

		for (auto n : nodes)
			rg.nodes.push_back(std::make_shared<Node>(n->id, n->name));
		

		for (auto n : nodes)
			for (auto &[edge,val] : n->connections)
				rg[edge.lock()->id]->connections.insert({ rg[n->id], 1 });

		rg.findComponents();

		std::sort(rg.nodes.begin(), rg.nodes.end(),
			[](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {return a->post >= b->post; });

		int comp = 0;
		int i = 0;		
		for (auto n : rg.nodes)
		{
			if (nodes[n->id]->component == -1)
			{
				components.push_back(nodes[n->id]);
				DFS(i, comp, nodes[n->id]);

				int size = components.back().lock()->post - components.back().lock()->pre + 1;
				if (size > max)
				{
					maxComp = nodes[n->id];
					max = size;
				}

				++comp;
			}
		}
	}
}

long long Graph::findClique()
{
	if (oriented)
		return -1;
	long long res = 0;

	for (auto n1 : nodes)
	{
		if (n1->connections.size() < 2)
			continue;
		for (auto &[wn2, edc2] : n1->connections)
		{
			auto n2 = wn2.lock();
			if (n2->connections.size() < 2 || n2->id <= n1->id)
				continue;

			for (auto &[wn3, edc3] : n1->connections)
			{
				auto n3 = wn3.lock();

				if (n3->id < n2->id || n3->id < n1->id)
					continue;

				if (n3->connections.find(n2) != n3->connections.end())
				{
					++n1->e;
					++n2->e;
					++n3->e;
					++res;
				}
			}
		}
	}

	return res;	
}

//int Graph::boostGetcountComp()
//{
//	comp.resize(boost::num_vertices(g));
//
//	return boost::strong_components(g, &comp[0]);
//	//return boost::connected_components(g, &comp[0]);
//}
//
//int Graph::boostGetStrongComp()
//{
//	comp.resize(boost::num_vertices(g));
//
//	return boost::strong_components(g, &comp[0]) ;
//}

void Graph::metaGraph(std::string filePath)
{
	Graph res(oriented);

	for (int i = 0; i < components.size(); i++)
		res.addNode(i);

	for (auto n : nodes)
		for (auto &[e, val] : n->connections)
			if (n->component != e.lock()->component)
				res[n->component]->connections[res[e.lock()->component]] += val;

	std::ofstream fout(filePath);

	auto t(nodes);
	std::sort(t.begin(), t.end(),
		[](std::shared_ptr<Node>  a, std::shared_ptr<Node>  b) {return a->component < b->component; });

	fout << res.nodes.size() << std::endl; 

	for (int i = 0; i < t.size() - 1; i++)
	{
		fout << t[i]->name << " ";
		if (t[i]->component != t[i + 1]->component)
			fout << std::endl;
	}
	if(t.size() != 0)
		fout << t.back()->name << std::endl;

	for(auto n : res.nodes)
		for (auto &[e,val] : n->connections)
			for (int i = 0; i < val; i++)
				fout << n->name << " " << e.lock()->name << std::endl;
		


	fout.close();
}

void Graph::removeRandomX(int x)
{

	for (int k = 0; k < x; k++)
	{
		int i = rand() % nodes.size();

		std::swap(nodes[i], nodes[nodes.size()-1]);
		nodes[i]->id = i;


		if (oriented)
			for (auto n : nodes)
				n->connections.erase(nodes.back());
		else
			for (auto &[n,val] : nodes.back()->connections)
					nodes[n.lock()->id]->connections.erase(nodes.back());

		auto t = nodes.back();
		nodes.pop_back();
	}

	for (auto n : nodes)
	{
		n->pre = -1;
		n->post = -1;
		n->component = -1;
	}

}

void Graph::sortByDeg()
{
	std::sort(nodes.begin(), nodes.end(),
		[](std::shared_ptr<Node>  a, std::shared_ptr<Node>  b) {return a->connections.size() < b->connections.size(); });

	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i]->id == i;
		dict[nodes.at(i)->name] = i;
	}
}

void Graph::pop_back(int x)
{
	for (int k = 0; k < x; k++)
	{
		if (oriented)
			for (auto n : nodes)
				n->connections.erase(nodes.back());
		else
			for (auto &[n,val] : nodes.back()->connections)
				if (n.lock()->id < nodes.size())
					nodes[n.lock()->id]->connections.erase(nodes.back());

		auto t = nodes.back();
		nodes.pop_back();
	}

	for (auto n : nodes)
	{
		n->pre = -1;
		n->post = -1;
		n->component = -1;
	}
}

std::ofstream& operator<<(std::ofstream& ofs, Graph& g)
{
	ofs << g.nodes.size() << std::endl;
	for (auto n : g.nodes)
		ofs << n->name << " " << n->pre << " " << n->post << std::endl;

	for (auto n : g.nodes)
	{
		ofs << n->name << " " << n->connections.size() << std::endl;
		for (auto& [edg, val] : n->connections)
			ofs << edg.lock()->name << " " << val << " ";
		ofs << std::endl;
	}
	return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, Graph& g)
{
	while (!ifs.eof())
	{
		int ida, idb;
		ifs >> ida >> idb;

		g.addConnection(ida, idb);
	}
	
	return ifs;
}
