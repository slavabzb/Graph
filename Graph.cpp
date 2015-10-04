// Файл "Graph.cpp"
// Содержит реализации всех функций, описанных в файле заголовка

#include "Graph.h"      // Файл заголовка класса Graph
#include <fstream>      // Файловый ввод-вывод
#include <iostream>     // Консольный ввод-вывод
#include <sstream>      // Строковый ввод-вывод
#include <limits>       // Диапазоны типов

/*
Конструктор класса Graph. Инициализирует все данные-члены (поля) класса
*/
Graph::Graph()
{
    // Установить значения по умолчанию для всех полей класса
    type = ' ';
    weighted = false;
    oriented = false;
}

/*
Функция чтения графа из файла.
Во всех представлениях вершины нумеруются с 0.

Файл имеет одну из следующих структур.



1. Матрица смежности

C 3 1
0 2 5
2 0 0
5 0 0

Этот пример описывает взвешенный граф (последнее число в первой строке - 1;
если 0, то граф невзвешенный), представленный в виде матрицы смежности
(символ 'C' в первой строке), который имеет 3 вершины
(второе число в первой строке).

Граф имеет 2 ребра:
* ребро с весом 2 между вершинами 0 и 1;
* ребро с весом 5 между вершинами 0 и 2.



2. Список смежности

L 3
0 0
1 2
0
0

Этот пример описывает неориентированный (первый 0) невзвешенный (второй 0) граф,
представленный в виде списка смежности (символ 'L' в первой строке), который
имеет 3 вершины (второе число в первой строке).

Граф имеет 2 ребра:
* ребро между вершинами 0 и 1;
* ребро между вершинами 0 и 2.

Другой вариант списка смежности

L 3
1 1
1 2 2 5
0 2
0 5


Этот пример описывает ориентированный (первая 1) взвешенный (вторая 1) граф,
представленный в виде списка смежности (символ 'L' в первой строке), который
имеет 3 вершины (второе число в первой строке).

Граф имеет 2 ребра:
* ребро с весом 2 между вершинами 0 и 1;
* ребро с весом 5 между вершинами 0 и 2.

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

    cout << "Reading graph from file " << fileName << "...\t";

    // Определить, какой тип внутреннего представления графа описан в файле
    file >> type;

    // Если в файле описан граф, представленный в виде матрицы смежности
    if(type == 'C')
    {
        // Очистить текущее содержимое матрицы смежности
        adjMatrix.clear();

        // Считать количество вершин
        int N = 0;
        file >> N;

        // Считать индикатор взвешенности графа
        file >> weighted;

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
    else    // Если в файле описан граф, представленный в виде списка смежности
    if(type == 'L')
    {
        // Очистить текущее содержимое списка смежности
        adjList.clear();

        // Считать количество вершин
        int N = 0;
        file >> N;

        // Считать индикатор ориентированности графа
        file >> oriented;

        // Считать индикатор взвешенности графа
        file >> weighted;

        // Установить курсор чтения файла на новую строку
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        // Считать N связей вершин
        for(int from = 0; from < N; from++)
        {
            // Строка для хранения списка связей текущей вершины
            string rowStr;

            // Считать текущую строку
            getline(file, rowStr);

            // Строковый поток ввода (для разбиения row по отдельным числам)
            istringstream iss(rowStr);

            // Переменная для хранения связей текущей вершины
            set< tuple< int, int, int > > row;

            // Бежать по rowStr
            do
            {
                // Переменная для хранения номера соседней вершины
                int to = 0;

                // Считать соседнюю вершину
                iss >> to;

                // Переменная для хранения веса ребра
                int weight = 0;

                // Если описан взвешенный граф
                if(weighted)
                {
                    // Считать вес ребра
                    iss >> weight;
                }

                // Сохранить считанные связи в список связей
                row.insert(make_tuple(from, to, weight));

            } while (!iss.eof());

            // Добавить список связей считанной вершины в список смежности
            adjList.push_back(row);
        }
    }

    cout << "done\n";

    // Напечатать граф
    printGraph();
}

void Graph::addEdge(int from, int to, int weight)
{
    cout << "Adding edge from " << from << " to " << to << ", weight is " << weight << "...\t";

    // Если значение параметра weight равно 0, это будет означать удаление ребра
    if(weight == 0)
    {
        cout << "Can't add edge from " << from << " to " << to << ", weight is equal to zero\n";
        return;
    }

    // Если текущее представление - матрица смежности
    if(type == 'C')
    {
        if(max(from,to) > adjMatrix.size() || min(from,to) < 0)
        {
            cout << "\nCan't add edge from " << from << " to " << to << ", the node doesn't exists\n";
            return;
        }

        // Установить в соответствующем столбце значение веса
        adjMatrix[from][to] = weight;

        // Установить значение симметричного элемента (свойство матрицы смежности)
        adjMatrix[to][from] = weight;
    }

    cout << "done\n";

    // Напечатать граф
    printGraph();
}

void Graph::removeEdge(int from, int to)
{
    cout << "Removing edge from " << from << " to " << to << "...\t";

    // Если текущее представление - матрица смежности
    if(type == 'C')
    {
        if(max(from,to) > adjMatrix.size() || min(from,to) < 0)
        {
            cout << "\nCan't remove edge from " << from << " to " << to << ", the node doesn't exists\n";
            return;
        }

        // Установить в соответствующем столбце значение веса в 0
        adjMatrix[from][to] = 0;

        // Установить значение симметричного элемента (свойство матрицы смежности) в 0
        adjMatrix[to][from] = 0;
    }

    cout << "done\n";

    // Напечатать граф
    printGraph();
}

int Graph::changeEdge(int from, int to, int newWeight)
{
    cout << "Changing edge from " << from << " to " << to << ", new weight is " << newWeight << "...\t";

    int oldWeight = 0;

    // Если текущее представление - матрица смежности
    if(type == 'C')
    {
        // Сохранить прежнее значение веса
        oldWeight = adjMatrix[from][to];

        // Установить в соответствующем столбце значение веса в newWeight
        adjMatrix[from][to] = newWeight;

        // Установить значение симметричного элемента (свойство матрицы смежности) в newWeight
        adjMatrix[to][from] = newWeight;
    }

    cout << "done\n";

    // Напечатать граф
    printGraph();

    // Вернуть прежнее значение веса
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

    // Если текущее представление - матрица смежности
    if(type == 'C')
    {
        cout << "adjacency matrix)...\t\n";

        // Бежать по строкам
        for(size_t i=0; i<adjMatrix.size(); i++)
        {
            // Бежать по столбцам
            for(size_t j=0; j<adjMatrix[i].size(); j++)
            {
                // Напечатать текущий элемент
                cout << adjMatrix[i][j] << " ";
            }
            cout << "\n";
        }
    }
    else    // Если текущее представление - список смежности
    if(type == 'L')
    {
        cout << "adjacency list)...\t\n";

        // Бежать по строкам
        for(size_t i=0; i<adjList.size(); i++)
        {
            // Бежать по столбцам
            for(set< tuple< int, int, int > >::iterator it = adjList[i].begin(); it!=adjList[i].end(); it++)
            {
                // Напечатать конец текущего ребра
                cout << get<1>(*it);

                // Если граф взвешенный
                if(weighted)
                {
                    // Напечатать вес текущего ребра
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
    // Создать объект файла и попытаться открыть файл с именем "fileName"
    ofstream file(fileName.c_str());

    // Если не удалось открыть файл, кинуть исключение
    if(!file.is_open())
    {
        cout << "Can't open file " << fileName << "\n";
        return;
    }

    cout << "Writing graph to file " << fileName << "...\t";

    // Сохранить тип текущего представления
    file << type << " ";

    // Если текущее представление - матрица смежности
    if(type == 'C')
    {
        // Сохранить количество вершин
        file << adjMatrix.size() << " ";

        // Сохранить флаг взвешенности
        file << weighted << "\n";

        // Бежать по строкам
        for(size_t i=0; i<adjMatrix.size(); i++)
        {
            // Бежать по столбцам
            for(size_t j=0; j<adjMatrix[i].size(); j++)
            {
                // Сохранить текущий элемент
                file << adjMatrix[i][j] << " ";
            }
            file << "\n";
        }
    }
    else    // Если текущее представление - список смежности
    if(type == 'L')
    {
        // Сохранить количество вершин
        file << adjList.size() << "\n";

        // Сохранить флаг ориентированности графа
        file << oriented << " ";

        // Сохранить флаг взвешенности графа
        file << weighted << "\n";

        // Бежать по строкам
        for(size_t i=0; i<adjList.size(); i++)
        {
            // Бежать по столбцам
            for(set< tuple< int, int, int > >::iterator it = adjList[i].begin(); it!=adjList[i].end(); it++)
            {
                // Сохранить конец текущего ребра
                file << get<1>(*it);

                // Если граф взвешенный
                if(weighted)
                {
                    // Сохранить вес текущего ребра
                    file << " " << get<2>(*it);
                }

                file << " ";
            }

            file << "\n";
        }
    }

    cout << "done\n";
}
