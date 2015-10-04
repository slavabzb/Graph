// ���� "Graph.h"
// ������������ ���� ������ Graph (���������)

// ���������� define
// ��������� ��� ����, ����� ���������� ���� �������� ���� ���� ������
#ifndef GRAPH_H
#define GRAPH_H

// ������� ����������� ������� ����������� ���������� STL
#include <string>   // ������ - ������������������ ��������
#include <vector>   // ������ - ������, ������� ����� �������� ���� ������
#include <set>      // ��������� - ����� ��������� ������ ����
#include <tuple>    // ������ - N ���������, ��������������� ������

// ������� ��� ������� � ������� ����������� ���������� "��������" � ������� ����
using namespace std;

// ����� Graph
// ������������ ����� � ���������
class Graph
{
// ������� (���������) ����� ������, ��� ���������
// ������� �� ���� ����� ����� ���� ������� ����� ������
public:
    // ����������� ������
    Graph();

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
    // ������� ���� �� ������
    void printGraph();

    // ������-����� (����) ������
    char type;      // ��� �������� �������������
    bool weighted;  // ����, true, ���� ���� ����������, ����� false
    bool oriented;  // ����, true, ���� ���� ���������������, ����� false

    vector< vector< int > > adjMatrix;                  // ������� ���������
    vector< set< tuple< int, int, int > > > adjList;    // ������ ���������
};

#endif // GRAPH_H
