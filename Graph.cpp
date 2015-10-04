// ���� "Graph.cpp"
// �������� ���������� ���� �������, ��������� � ����� ���������

#include "Graph.h"      // ���� ��������� ������ Graph
#include <fstream>      // �������� ����-�����
#include <iostream>     // ���������� ����-�����
#include <sstream>      // ��������� ����-�����
#include <limits>       // ��������� �����
#include <algorithm>    // ����������� ���������

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

a)      b)          c)      d)

L 3     L 3         L 3     L 3
0 0     0 1         1 0     1 1
1 2     1 2 2 5     1 2     1 2 2 5
0       0 2                 0
0       0 5                 0

��� ������� ��������� �����, �������������� � ���� ������ ���������
(������ 'L' � ������ ������), ������� ����� 3 ������� (������ ����� � ������ ������).

������ ������ ���������� ��������� ����������������� � ������������.

a) ����������������� ������������,
b) ����������������� ���������� (����� ������� ������ ����� ���� �����),
c) ��������������� ������������ (����� ������ ����� �������� ������ � ��������� �����������),
d) ��������������� ����������.

����� ����� �� 2 �����.

a) ����� 0-1 � ����� 0-2 (������ � ��� �������),
b) ����� 0-1 � ����� 2 � ����� 0-2 � ����� 5 (������ � ��� �������),
c) ����� 0-1 � ����� 0-2 (������ ������ � ��������� �����������),
d) ����� 0-1 � ����� 2 � ����� 0-2 � ����� 5 (������ ������ � ��������� �����������).

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
            set< pair<int, int> > row;

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
                row.insert(make_pair(to, weight));

            } while (!iss.eof());

            // �������� ������ ������ ��������� ������� � ������ ���������
            adjList[from] = row;
        }
    }

    cout << "done\n";

    // ���������� ����
    printGraph();
}

void Graph::addEdge(int from, int to, int weight)
{
    cout << "Adding edge from " << from << " to " << to << ", weight is " << weight << "...\t";

    // ���� ������� ������������� - ������� ���������
    if(type == 'C')
    {
        // ���� �������� ��������� weight ����� 0, ��� ����� �������� �������� �����
        if(weight == 0)
        {
            cout << "Can't add edge from " << from << " to " << to << ", weight is equal to zero\n";
            return;
        }

        // ��������� ��������� � ����������� �������
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
    else    // ���� ������� ������������� - ������ ���������
    if(type == 'L')
    {
        // �������� ����������� - ��������� �� ������ ������ ������� � ������ ���������
        map< int, set< pair<int, int> > >::iterator fromIt;

        // ����� � ������ ��������� ������ �����
        fromIt = adjList.find(from);

        // ���� �� �������
        if(fromIt==adjList.end())
        {
            // �������� �� ������
            cout << "\nCan't add edge from " << from << " to " << to << ", the node doesn't exists\n";
            return;
        }

        // �������� ��������� - ��������� �� �������� ������� � ������ ������ ���������
        set< pair<int, int> >::iterator toIt;

        // ���� ���� ����������
        if(weighted)
        {
            // ����� ������� ������� �������, ��������� ��������� ���
            toIt = fromIt->second.find(make_pair(to,weight));
        }
        else    // // ���� ���� ������������
        {
            // ����� ������� ������� �������, ��������� � �������� ���� 0
            toIt = fromIt->second.find(make_pair(to,0));
        }

        // ���� ������� ���-�� �����
        if(toIt!=fromIt->second.end())
        {
            // �������� �� ������
            cout << "\nCan't add edge from " << from << " to " << to << ", the edge already exists\n";
            return;
        }

        // ���� ���� ����������
        if(weighted)
        {
            // �������� ����� �������� �������, ��������� ����� ��������� ���
            fromIt->second.insert(make_pair(to,weight));
        }
        else    // ���� ���� ������������
        {
            // �������� ����� �������� �������, ��������� ����� ��� 0
            fromIt->second.insert(make_pair(to,0));
        }

        // ���� ���� �����������������, �� �������� ����� ����� � �������� �����������
        if(!oriented)
        {
            // ���� ���� ����������
            if(weighted)
            {
                // ������������ ��������� ���
                adjList[to].insert(make_pair(from,weight));
            }
            else    // ���� ���� ������������
            {
                // ������������ ��� 0
                adjList[to].insert(make_pair(from,0));
            }
        }
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
        // ��������� ��������� � ����������� �������
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
    else    // ���� ������� ������������� - ������ ���������
    if(type == 'L')
    {
        // �������� ����������� - ��������� �� ������ ������ ������� � ������ ���������
        map< int, set< pair<int, int> > >::iterator fromIt;

        // ����� � ������ ��������� ������ �����
        fromIt = adjList.find(from);

        // ���� �� �������
        if(fromIt==adjList.end())
        {
            // ������� �� ����������, ������� ������
            return;
        }

        // �������� ��������� - ��������� �� �������� ������� � ������ ������ ���������
        set< pair<int, int> >::iterator toIt;

        // ����� ����� ������� �������, ���������� ������ �����, ������� ���������� �������
        toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
            [&to](const pair<int, int>& edge)
            {
                if(edge.first==to) return true;
            });

        // ���� ������� ���-�� �����
        if(toIt==fromIt->second.end())
        {
            // ������� ��������� �����
            fromIt->second.erase(toIt);
        }

        // ���� ���� �����������������, �� ������� ����� ����� � �������� �����������
        if(!oriented)
        {
            // ����� � ������ ��������� ������ �����
            fromIt = adjList.find(to);

            // ���� �� �������
            if(fromIt==adjList.end())
            {
                // ������� �� ����������, ������� ������
                return;
            }

            // ����� ����� ������� �������, ���������� ������ �����, ������� ���������� �������
            toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
                [&from](const pair<int, int>& edge)
                {
                    if(edge.first==from) return true;
                });

            // ���� �� ������� ������ �����
            if(toIt==fromIt->second.end())
            {
                // �������� �� ������ (� ����������������� ����� ����� ������ ���� � ����� ������������)
                cout << "\nCan't remove edge from " << to << " to " << from << ", the edge doesn't exists\n";
                return;
            }

            // ������� ��������� �����
            fromIt->second.erase(toIt);
        }
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
        // ��������� ��������� � ����������� �������
        if(max(from,to) > adjMatrix.size() || min(from,to) < 0)
        {
            cout << "\nCan't change edge from " << from << " to " << to << ", the node doesn't exists\n";
            return -1;
        }

        // ��������� ������� �������� ����
        oldWeight = adjMatrix[from][to];

        // ���������� � ��������������� ������� �������� ���� � newWeight
        adjMatrix[from][to] = newWeight;

        // ���������� �������� ������������� �������� (�������� ������� ���������) � newWeight
        adjMatrix[to][from] = newWeight;
    }
    else    // ���� ������� ������������� - ������ ���������
    if(type == 'L')
    {
        // �������� ����������� - ��������� �� ������ ������ ������� � ������ ���������
        map< int, set< pair<int, int> > >::iterator fromIt;

        // ����� � ������ ��������� ������ �����
        fromIt = adjList.find(from);

        // ���� �� �������
        if(fromIt==adjList.end())
        {
            // �������� �� ������
            cout << "\nCan't change edge from " << from << " to " << to << ", the node doesn't exists\n";
            return -1;
        }

        // �������� ��������� - ��������� �� �������� ������� � ������ ������ ���������
        set< pair<int, int> >::iterator toIt;

        // ����� ����� ������� �������, ���������� ������ �����, ������� ���������� ��������
        toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
            [&to](const pair<int, int>& edge)
            {
                if(edge.first==to) return true;
            });

        // ���� �� ������� ������ �����
        if(toIt!=fromIt->second.end())
        {
            // �������� �� ������
            cout << "\nCan't change edge from " << from << " to " << to << ", the edge doesn't' exists\n";
            return -1;
        }

        // ��������� ������� �������� ����
        oldWeight = toIt->second;

        // ���� ���� ����������
        if(weighted)
        {
            // ��������� ����� ��������� ���
            toIt->second = newWeight;
        }
        else    // ���� ���� ������������
        {
            // ��������� ����� ��� 0
            toIt->second = 0;
        }

        // ���� ���� �����������������, �� �������� ����� ����� � �������� �����������
        if(!oriented)
        {
            // ����� � ������ ��������� ������ �����
            fromIt = adjList.find(to);

            // ���� �� �������
            if(fromIt==adjList.end())
            {
                // ������� �� ����������, ������� ������
                return -1;
            }

            // ����� ����� ������� �������, ���������� ������ �����, ������� ���������� �������
            toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
                [&from](const pair<int, int>& edge)
                {
                    if(edge.first==from) return true;
                });

            // ���� �� ������� ������ �����
            if(toIt==fromIt->second.end())
            {
                // �������� �� ������ (� ����������������� ����� ����� ������ ���� � ����� ������������)
                cout << "\nCan't change edge from " << to << " to " << from << ", the edge doesn't exists\n";
                return -1;
            }

            // ���� ���� ����������
            if(weighted)
            {
                // ��������� ����� ��������� ���
                toIt->second = newWeight;
            }
            else    // ���� ���� ������������
            {
                // ��������� ����� ��� 0
                toIt->second = 0;
            }
        }
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

bool Graph::is_weighted()
{
    return weighted;
}

void Graph::printGraph()
{
    cout << "Printing graph (";

    // ���������� ���� �����������������
    if(oriented)
    {
        cout << "oriented, ";
    }
    else
    {
        cout << "not oriented, ";
    }

    // ���������� ���� ������������
    if(weighted)
    {
        cout << "weighted, ";
    }
    else
    {
        cout << "not weighted, ";
    }

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
        for(map< int, set< pair<int, int> > >::iterator fromIt=adjList.begin(); fromIt!=adjList.end(); fromIt++)
        {
            // ������ �� ��������
            for(set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt!=fromIt->second.end(); toIt++)
            {
                // ���������� ����� �������� �����
                cout << toIt->first;

                // ���� ���� ����������
                if(weighted)
                {
                    // ���������� ��� �������� �����
                    cout << " " << toIt->second;
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
        for(map< int, set< pair<int, int> > >::iterator fromIt=adjList.begin(); fromIt!=adjList.end(); fromIt++)
        {
            // ������ �� ��������
            for(set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt!=fromIt->second.end(); toIt++)
            {
                // ���������� ����� �������� �����
                file << toIt->first;

                // ���� ���� ����������
                if(weighted)
                {
                    // ���������� ��� �������� �����
                    file << " " << toIt->second;
                }

                file << " ";
            }

            file << "\n";
        }
    }

    cout << "done\n";
}
