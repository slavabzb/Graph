// ���� "Graph.cpp"
// �������� ���������� ���� �������, ��������� � ����� ���������

#include "Graph.h"      // ���� ��������� ������ Graph
#include <fstream>      // �������� ����-�����
#include <iostream>     // ���������� ����-�����

/*
������� ������ ����� �� �����
���� ����� ���� �� ��������� ��������

1. ������� ���������

C 3 1
0 2 5
2 0 0
5 0 0

���� ������ ��������� ���������� ���� (��������� ����� � ������ ������ - 1;
���� 0, �� ���� ������������), �������������� � ���� ������� ���������
(������ 'C' � ������ ������), ������� ����� 3 �������
(������ ����� � ������ ������)
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

    // �������� ������� ���������� ������� ���������
    adjMatrix.clear();

    // ����������, ����� ��� ����������� ������������� ����� ������ � �����
    char type = ' ';
    file >> type;

    // ���� � ����� ������� ������� ���������
    if(type == 'C')
    {
        // ������� ���������� ������
        int N = 0;
        file >> N;

        // ������� ��������� ������������ �����
        bool W = false;
        file >> W;

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
}

void Graph::addEdge(int from, int to, int weight)
{

}

void Graph::removeEdge(int from, int to)
{

}

int Graph::changeEdge(int from, int to, int newWeight)
{
    return 0;
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

void Graph::writeGraph(string fileName)
{

}
