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
#include <map>      // ����������� - ����� ��������� "����-��������"
#include <tuple>    // ������ - N ���������, ��������������� ��� ������ �����

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

    // ���������� ���� ������������ �����
    bool is_weighted();

// �������� (���������) ����� ������
// ��� ������� � ������ �������� ������ ������ ������ Graph
private:
    // ������� ���� �� ������
    void printGraph();

    // ������-����� (����) ������
    char type;      // ��� �������� �������������
    bool weighted;  // ����, true, ���� ���� ����������, ����� false
    bool oriented;  // ����, true, ���� ���� ���������������, ����� false
    int N;          // ���������� ������
    int M;          // ���������� �����

    vector< vector< int > > adjMatrix;            // ������� ���������
    map< int, set< pair< int, int > > > adjList;  // ������ ���������
    set< tuple< int, int, int > > edgList;        // ������ �����
};

#endif // GRAPH_H
