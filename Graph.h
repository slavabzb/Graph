// Файл "Graph.h"
// Заголовочный файл класса Graph (интерфейс)

// Сторожевой define
// Необходим для того, чтобы невозможно было включить этот файл дважды
#ifndef GRAPH_H
#define GRAPH_H

// Команды подключения классов стандартной библиотеки STL
#include <string>   // Строка - последовательность символов
#include <vector>   // Вектор - массив, который может изменять свой размер
#include <set>      // Множество - набор элементов одного типа
#include <map>      // Отображение - набор элементов "ключ-значение"
#include <tuple>    // Кортеж - N элементов, рассматриваемых как единое целое

// Сделать все функции и объекты стандартной библиотеки "видимыми" в текущем коде
using namespace std;

// Класс Graph
// Представляет графы в программе
class Graph
{
// Откытая (публичная) часть класса, его интерфейс
// Функции из этой части могут быть вызваны извне класса
public:
    // Конструктор класса
    Graph();

    // Считывает граф из файла fileName
    void readGraph(string fileName);

    // Сохраняет граф в файл fileName
    void writeGraph(string fileName);

    // Добавляет ребро с весом weight между вершинами from и to
    void addEdge(int from, int to, int weight);

    // Удаляет ребро между вершинами from и to
    void removeEdge(int from, int to);

    // Изменяет вес ребра между вершинами from и to на newWeight
    // Возвращает прежнее значение веса
    int changeEdge(int from, int to, int newWeight);

    // Преобразовывает внутреннее представление графа в ...
    void transformToAdjList();      // ... список смежности
    void transformToAdjMatrix();    // ... матрицу смежности
    void transformToListOfEdges();  // ... список ребер

    // Возвращает флаг взвешенности графа
    bool is_weighted();

// Закрытая (приватная) часть класса
// Эти функции и данные доступны только внутри класса Graph
private:
    // Выводит граф на печать
    void printGraph();

    // Данные-члены (поля) класса
    char type;      // Тип текущего представления
    bool weighted;  // Флаг, true, если граф взвешенный, иначе false
    bool oriented;  // Флаг, true, если граф ориентированный, иначе false
    int N;          // Количество вершин
    int M;          // Количество ребер

    vector< vector< int > > adjMatrix;            // Матрица смежности
    map< int, set< pair< int, int > > > adjList;  // Список смежности
    set< tuple< int, int, int > > edgList;        // Список ребер
};

#endif // GRAPH_H
