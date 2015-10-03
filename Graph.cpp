// Файл "Graph.cpp"
// Содержит реализации всех функций, описанных в файле заголовка

#include "Graph.h"      // Файл заголовка класса Graph
#include <fstream>      // Файловый ввод-вывод
#include <iostream>     // Консольный ввод-вывод

/*
Функция чтения графа из файла
Файл имеет одну из следующих структур

1. Матрица смежности

C 3 1
0 2 5
2 0 0
5 0 0

Этот пример описывает взвешенный граф (последнее число в первой строке - 1;
если 0, то граф невзвешенный), представленный в виде матрицы смежности
(символ 'C' в первой строке), который имеет 3 вершины
(второе число в первой строке)
*/
void Graph::readGraph(string fileName)
{
    // Создать объект файла и попытаться открыть файл с именем "fileName"
    ifstream file(fileName.c_str());

    // Если не удалось открыть файл, кинуть исключение
    if(!file.is_open())
    {
        cout << "Can't open file " << fileName << "\n";
        return;
    }

    // Очистить текущее содержимое матрицы смежности
    adjMatrix.clear();

    // Определить, какой тип внутреннего представления графа описан в файле
    char type = ' ';
    file >> type;

    // Если в файле описана матрица смежности
    if(type == 'C')
    {
        // Считать количество вершин
        int N = 0;
        file >> N;

        // Считать индикатор взвешенности графа
        bool W = false;
        file >> W;

        // Считать всю матрицу смежности

        // Бежать по строкам
        for(int i = 0; i < N; i++)
        {
            // Вектор для хранения текущей строки
            vector<int> row;

            // Бежать по столбцам
            for(int j = 0; j < N; j++)
            {
                // Считать текущее ребро
                int a;
                file >> a;

                // Сохранить считанное ребро в row
                row.push_back(a);
            }

            // Сохранить считанную строку в матрицу смежности
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
