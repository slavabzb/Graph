// ���� "Graph.h"
// ������������ ���� ������ Graph (���������)

// ���������� define
// ��������� ��� ����, ����� ���������� ���� �������� ���� ���� ������
#ifndef GRAPH_H
#define GRAPH_H

// ������� ����������� ������� ����������� ���������� STL
#include <string>   // ������
#include <vector>   // ������ - ������, ������� ����� �������� ���� ������

// ������� ��� ������� � ������� ����������� ���������� "��������" � ������� ����
using namespace std;

// ����� Graph
// ������������ ����� � ���������
class Graph
{
// ������� (���������) ����� ������, ��� ���������
// ������� �� ���� ����� ����� ���� ������� ����� ������
public:
    // ��������� ���� �� ����� fileName
    void readGraph(string fileName);

    // ��������� ���� � ���� fileName
    void writeGraph(string fileName);

    // ��������� ����� � ����� weight ����� ��������� from � to
    void addEdge(int from, int to, int weight);

    // ������� ����� ����� ��������� from � to
    void removeEdge(int from, int to);

    // �������� ��� ����� ����� ��������� from � to �� newWeight
    // ���������� ������� �������� ����
    int changeEdge(int from, int to, int newWeight);

    // ��������������� ���������� ������������� ����� � ...
    void transformToAdjList();      // ... ������ ���������
    void transformToAdjMatrix();    // ... ������� ���������
    void transformToListOfEdges();  // ... ������ �����

// �������� (���������) ����� ������
// ��� ������� � ������ �������� ������ ������ ������ Graph
private:
    vector< vector< int > > adjMatrix;  // ������� ���������
};

#endif // GRAPH_H
