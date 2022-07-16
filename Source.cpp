#include <iostream> 
#include <string>

#include "Graph.h"
#include "Node.h"

using namespace::std;


int main()
{
	setlocale(0, "");

	srand(time(0));

	string path[8];
	path[0] = "datasets\\CA-AstroPh.txt";
	path[1] = "datasets\\web-Google.txt";
	path[2] = "datasets\\vk-output.txt";
	path[3] = "datasets\\CA-GrQc.txt";
	path[4] = "datasets\\email-Eu-core.txt";
	path[5] = "datasets\\socfb-Middlebury45.mtx";
	path[6] = "datasets\\socfb-Reed98.mtx";
	path[7] = "datasets\\soc-wiki-Vote.mtx";



	cout << "�������� �������: \n0: CA-AstroPh\n1: web-Google\n2: vk\n3: CA-GrQc\n4: email-Eu-core\n5: socfb-Middlebury45\n6: socfb-Reed98\n7: soc-wiki-Vote\n";

	int dataset;
	cin >> dataset;

	cout << "���� ������...\n";
	ifstream fin(path[dataset]);

	int start = clock();
	Graph graph(false);
	Graph orGraph(true);

	fin >> graph;
	fin.close();

	if (dataset == 1 || dataset == 7 || dataset == 4)
	{
		fin.open(path[dataset]);
		fin >> orGraph;
		fin.close();
	}

	cout << "������ ����������, ���� " << (clock() - start) / 1000. << " ���.\n\n";

	cout << "�������� ������:\n0: A1\n1: A3\n2: A4\n3: B1\n4: B2\n";

	int z;
	cin >> z;

	double proc = 1;
	int x = graph.getCountNode() / 100 * proc;

	start = clock();
	switch (z)
	{
	case 0:
		start = clock();
		graph.findComponents();
		cout << "time - " << clock() - start << endl;

		cout << "1) ������ - " << graph.getCountNode() << endl;
		cout << "2) ����� - " << graph.getCountEdge() << endl;
		cout << "3) �����/�������� ����� - " << (double)graph.getCountEdge() / (graph.getCountNode() / 2. * (graph.getCountNode() - 1)) << endl;
		cout << "4) ��������� ������ ��������� - " << graph.getCountComp() << endl;
		cout << "5) ������ � ������������ ����������� ������ ��������� - " << graph.getCountNodeInMaxComp() << endl;
		cout << "6) 5) / 1) - " << (double)graph.getCountNodeInMaxComp() / graph.getCountNode() << endl;

		start = clock();
		//cout << "boost - " << graph.boostGetcountComp() << endl;
		cout << "time - " << (clock() - start) / 1000. << endl << endl;


		if (dataset == 1 || dataset == 7 || dataset == 4)
		{
			start = clock();
			orGraph.findComponents();
			cout << "time - " << (clock() - start) / 1000. << endl;


			cout << "7) ����� ��������� ������� ��������� - " << orGraph.getCountComp() << endl;
			cout << "8) ������ � ������������ ���������� ������� ��������� - " << orGraph.getCountNodeInMaxComp() << endl;
			cout << "9) 8) / 1) - " << (double)orGraph.getCountNodeInMaxComp() / orGraph.getCountNode() << endl;


			start = clock();
			//cout << "boost - " << graph.boostGetStrongComp() << endl;
			cout << "time - " << (clock() - start) / 1000. << endl << endl;
		}

		break;
	case 1:
		if (dataset != 1)
		{
			cout << "�������� ������� 1";
			break;
		}
		orGraph.findComponents();
		orGraph.metaGraph("metaGraph.txt");
		cout << "��������� ������� � ���� metaGraph.txt\n";
		cout << "1 ������ - n - ���-�� ��������� ������� ���������\n"
			<< "������ n �����.i ������ - ������ ������ � i ����������. ��������� � 0\n"
			<< "������ ����� � ��������� � ������ ������ � �������\n";

		break;
	case 2:
		cout << "����� ������������� - " << graph.findClique() << endl;
		cout << endl << "������������� �������: " << (clock() - start) / 1000. << endl;
		graph.clusterCoef("clusterCoef.txt");
		cout << "��������� ������� � ���� clusterCoef.txt\n";
		cout << "1 ������ - ������� ���������� ����\n"
			<< "2 ������ - ���������� ���������� ����\n"
			<< "������ ������ ���. �������� ������� � ��������� ���������� ����\n";

		break;
	case 3:
	{
		ofstream f1("B1.txt");

		for (int i = 1; i < 100; i++)
		{
			graph.removeRandomX(x);
			graph.findComponents();

			f1 << proc * i << " " << (double)graph.getCountNodeInMaxComp() / graph.getCountNode() << endl;
			cout << proc * i << " " << (double)graph.getCountNodeInMaxComp() / graph.getCountNode() << endl;
		}

		f1.close();
		cout << "��������� � ����� B1.txt\n";
	}

	break;
	case 4:
	{
		ofstream f2("B2.txt");

		graph.sortByDeg();
		for (int i = 1; i < 100; i++)
		{
			graph.pop_back(x);
			graph.findComponents();

			f2 << proc * i << " " << (double)graph.getCountNodeInMaxComp() / graph.getCountNode() << endl;
			cout << proc * i << " " << (double)graph.getCountNodeInMaxComp() / graph.getCountNode() << endl;

			if (i == 85)
				cout << " ";
		}
		f2.close();
		cout << "��������� � ����� B2.txt\n";
	}

	break;
	}

	cout << endl << "������������� �������: " << (clock() - start) / 1000. << endl;

	system("pause");
	return 0;
}