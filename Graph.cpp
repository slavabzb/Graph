// ���� "Graph.cpp"
// �������� ���������� ���� �������, ��������� � ����� ���������

#include "Graph.h"      // ���� ��������� ������ Graph
#include <fstream>      // �������� ����-�����
#include <iostream>     // ���������� ����-�����
#include <sstream>      // ��������� ����-�����
#include <limits>       // ��������� �����

/*
����������� ������ Graph. �������������� ��� ������-����� (����) ������
*/
Graph::Graph()
{
    // ���������� �������� �� ��������� ��� ���� ����� ������
    type = ' ';
    weighted = false;
    oriented = false;
}

/*
������� ������ ����� �� �����.
�� ���� �������������� ������� ���������� � 0.

���� ����� ���� �� ��������� ��������.



1. ������� ���������

C 3 1
0 2 5
2 0 0
5 0 0

���� ������ ��������� ���������� ���� (��������� ����� � ������ ������ - 1;
���� 0, �� ���� ������������), �������������� � ���� ������� ���������
(������ 'C' � ������ ������), ������� ����� 3 �������
(������ ����� � ������ ������).

���� ����� 2 �����:
* ����� � ����� 2 ����� ��������� 0 � 1;
* ����� � ����� 5 ����� ��������� 0 � 2.



2. ������ ���������

L 3
0 0
1 2
0
0

���� ������ ��������� ����������������� (������ 0) ������������ (������ 0) ����,
�������������� � ���� ������ ��������� (������ 'L' � ������ ������), �������
����� 3 ������� (������ ����� � ������ ������).

���� ����� 2 �����:
* ����� ����� ��������� 0 � 1;
* ����� ����� ��������� 0 � 2.

������ ������� ������ ���������

L 3
1 1
1 2 2 5
0 2
0 5


���� ������ ��������� ��������������� (������ 1) ���������� (������ 1) ����,
�������������� � ���� ������ ��������� (������ 'L' � ������ ������), �������
����� 3 ������� (������ ����� � ������ ������).

���� ����� 2 �����:
* ����� � ����� 2 ����� ��������� 0 � 1;
* ����� � ����� 5 ����� ��������� 0 � 2.

*/
void Graph::readGraph(string fileName)
{
    // ������� ������ ����� � ���������� ������� ���� � ������ "fileName"
    ifstream file(fileName.c_str());

    // ���� �� ������� ������� ����, ������ ����������
    if(!file.is_open())
    {
        cout << "Can't open file " << fileName << "\n";
        return;
    }

    cout << "Reading graph from file " << fileName << "...\t";

    // ����������, ����� ��� ����������� ������������� ����� ������ � �����
    file >> type;

    // ���� � ����� ������ ����, �������������� � ���� ������� ���������
    if(type == 'C')
    {
        // �������� ������� ���������� ������� ���������
        adjMatrix.clear();

        // ������� ���������� ������
        int N = 0;
        file >> N;

        // ������� ��������� ������������ �����
        file >> weighted;

        // ������� ��� ������� ���������

        // ������ �� �������
        for(int i = 0; i < N; i++)
        {
            // ������ ��� �������� ������� ������
            vector<int> row;

            // ������ �� ��������
            for(int j = 0; j < N; j++)
            {
                // ������� ������� �����
                int a;
                file >> a;

                // ��������� ��������� ����� � row
                row.push_back(a);
            }

            // ��������� ��������� ������ � ������� ���������
            adjMatrix.push_back(row);
        }
    }
    else    // ���� � ����� ������ ����, �������������� � ���� ������ ���������
    if(type == 'L')
    {
        // �������� ������� ���������� ������ ���������
        adjList.clear();

        // ������� ���������� ������
        int N = 0;
        file >> N;

        // ������� ��������� ����������������� �����
        file >> oriented;

        // ������� ��������� ������������ �����
        file >> weighted;

        // ���������� ������ ������ ����� �� ����� ������
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        // ������� N ������ ������
        for(int from = 0; from < N; from++)
        {
            // ������ ��� �������� ������ ������ ������� �������
            string rowStr;

            // ������� ������� ������
            getline(file, rowStr);

            // ��������� ����� ����� (��� ��������� row �� ��������� ������)
            istringstream iss(rowStr);

            // ���������� ��� �������� ������ ������� �������
            set< tuple< int, int, int > > row;

            // ������ �� rowStr
            do
            {
                // ���������� ��� �������� ������ �������� �������
                int to = 0;

                // ������� �������� �������
                iss >> to;

                // ���������� ��� �������� ���� �����
                int weight = 0;

                // ���� ������ ���������� ����
                if(weighted)
                {
                    // ������� ��� �����
                    iss >> weight;
                }

                // ��������� ��������� ����� � ������ ������
                row.insert(make_tuple(from, to, weight));

            } while (!iss.eof());

            // �������� ������ ������ ��������� ������� � ������ ���������
            adjList.push_back(row);
        }
    }

    cout << "done\n";

    // ���������� ����
    printGraph();
}

void Graph::addEdge(int from, int to, int weight)
{
    cout << "Adding edge from " << from << " to " << to << ", weight is " << weight << "...\t";

    // ���� �������� ��������� weight ����� 0, ��� ����� �������� �������� �����
    if(weight == 0)
    {
        cout << "Can't add edge from " << from << " to " << to << ", weight is equal to zero\n";
        return;
    }

    // ���� ������� ������������� - ������� ���������
    if(type == 'C')
    {
        if(max(from,to) > adjMatrix.size() || min(from,to) < 0)
        {
            cout << "\nCan't add edge from " << from << " to " << to << ", the node doesn't exists\n";
            return;
        }

        // ���������� � ��������������� ������� �������� ����
        adjMatrix[from][to] = weight;

        // ���������� �������� ������������� �������� (�������� ������� ���������)
        adjMatrix[to][from] = weight;
    }

    cout << "done\n";

    // ���������� ����
    printGraph();
}

void Graph::removeEdge(int from, int to)
{
    cout << "Removing edge from " << from << " to " << to << "...\t";

    // ���� ������� ������������� - ������� ���������
    if(type == 'C')
    {
        if(max(from,to) > adjMatrix.size() || min(from,to) < 0)
        {
            cout << "\nCan't remove edge from " << from << " to " << to << ", the node doesn't exists\n";
            return;
        }

        // ���������� � ��������������� ������� �������� ���� � 0
        adjMatrix[from][to] = 0;

        // ���������� �������� ������������� �������� (�������� ������� ���������) � 0
        adjMatrix[to][from] = 0;
    }

    cout << "done\n";

    // ���������� ����
    printGraph();
}

int Graph::changeEdge(int from, int to, int newWeight)
{
    cout << "Changing edge from " << from << " to " << to << ", new weight is " << newWeight << "...\t";

    int oldWeight = 0;

    // ���� ������� ������������� - ������� ���������
    if(type == 'C')
    {
        // ��������� ������� �������� ����
        oldWeight = adjMatrix[from][to];

        // ���������� � ��������������� ������� �������� ���� � newWeight
        adjMatrix[from][to] = newWeight;

        // ���������� �������� ������������� �������� (�������� ������� ���������) � newWeight
        adjMatrix[to][from] = newWeight;
    }

    cout << "done\n";

    // ���������� ����
    printGraph();

    // ������� ������� �������� ����
    return oldWeight;
}

void Graph::transformToAdjList()
{

}

void Graph::transformToAdjMatrix()
{

}

void Graph::transformToListOfEdges()
{

}

void Graph::printGraph()
{
    cout << "Printing graph (";

    // ���� ������� ������������� - ������� ���������
    if(type == 'C')
    {
        cout << "adjacency matrix)...\t\n";

        // ������ �� �������
        for(size_t i=0; i<adjMatrix.size(); i++)
        {
            // ������ �� ��������
            for(size_t j=0; j<adjMatrix[i].size(); j++)
            {
                // ���������� ������� �������
                cout << adjMatrix[i][j] << " ";
            }
            cout << "\n";
        }
    }
    else    // ���� ������� ������������� - ������ ���������
    if(type == 'L')
    {
        cout << "adjacency list)...\t\n";

        // ������ �� �������
        for(size_t i=0; i<adjList.size(); i++)
        {
            // ������ �� ��������
            for(set< tuple< int, int, int > >::iterator it = adjList[i].begin(); it!=adjList[i].end(); it++)
            {
                // ���������� ����� �������� �����
                cout << get<1>(*it);

                // ���� ���� ����������
                if(weighted)
                {
                    // ���������� ��� �������� �����
                    cout << " " << get<2>(*it);
                }

                cout << " ";
            }

            cout << "\n";
        }
    }

}

void Graph::writeGraph(string fileName)
{
    // ������� ������ ����� � ���������� ������� ���� � ������ "fileName"
    ofstream file(fileName.c_str());

    // ���� �� ������� ������� ����, ������ ����������
    if(!file.is_open())
    {
        cout << "Can't open file " << fileName << "\n";
        return;
    }

    cout << "Writing graph to file " << fileName << "...\t";

    // ��������� ��� �������� �������������
    file << type << " ";

    // ���� ������� ������������� - ������� ���������
    if(type == 'C')
    {
        // ��������� ���������� ������
        file << adjMatrix.size() << " ";

        // ��������� ���� ������������
        file << weighted << "\n";

        // ������ �� �������
        for(size_t i=0; i<adjMatrix.size(); i++)
        {
            // ������ �� ��������
            for(size_t j=0; j<adjMatrix[i].size(); j++)
            {
                // ��������� ������� �������
                file << adjMatrix[i][j] << " ";
            }
            file << "\n";
        }
    }
    else    // ���� ������� ������������� - ������ ���������
    if(type == 'L')
    {
        // ��������� ���������� ������
        file << adjList.size() << "\n";

        // ��������� ���� ����������������� �����
        file << oriented << " ";

        // ��������� ���� ������������ �����
        file << weighted << "\n";

        // ������ �� �������
        for(size_t i=0; i<adjList.size(); i++)
        {
            // ������ �� ��������
            for(set< tuple< int, int, int > >::iterator it = adjList[i].begin(); it!=adjList[i].end(); it++)
            {
                // ��������� ����� �������� �����
                file << get<1>(*it);

                // ���� ���� ����������
                if(weighted)
                {
                    // ��������� ��� �������� �����
                    file << " " << get<2>(*it);
                }

                file << " ";
            }

            file << "\n";
        }
    }

    cout << "done\n";
}
