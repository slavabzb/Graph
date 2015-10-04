// Файл "Graph.cpp"
// Содержит реализации всех функций, описанных в файле заголовка

#include "Graph.h"      // Файл заголовка класса Graph
#include <fstream>      // Файловый ввод-вывод
#include <iostream>     // Консольный ввод-вывод
#include <sstream>      // Строковый ввод-вывод
#include <limits>       // Диапазоны типов
#include <algorithm>    // Стандартные алгоритмы

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

a)      b)          c)      d)

L 3     L 3         L 3     L 3
0 0     0 1         1 0     1 1
1 2     1 2 2 5     1 2     1 2 2 5
0       0 2                 0
0       0 5                 0

Эти примеры описывают графы, представленные в виде списка смежности
(символ 'L' в первой строке), которые имеют 3 вершины (второе число в первой строке).

Вторая строка определяет параметры ориентированности и взвешенности.

a) Неориентированный невзвешенный,
b) Неориентированный взвешенный (после номеров вершин стоят веса ребер),
c) Ориентированный невзвешенный (обход такого графа возможен только в указанном направлении),
d) Ориентированный взвешенный.

Графы имеют по 2 ребра.

a) Ребро 0-1 и ребро 0-2 (проход в обе стороны),
b) Ребро 0-1 с весом 2 и ребро 0-2 с весом 5 (проход в обе стороны),
c) Ребро 0-1 и ребро 0-2 (проход строго в указанном направлении),
d) Ребро 0-1 с весом 2 и ребро 0-2 с весом 5 (проход строго в указанном направлении).

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
            set< pair<int, int> > row;

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
                row.insert(make_pair(to, weight));

            } while (!iss.eof());

            // Добавить список связей считанной вершины в список смежности
            adjList[from] = row;
        }
    }

    cout << "done\n";

    // Напечатать граф
    printGraph();
}

void Graph::addEdge(int from, int to, int weight)
{
    cout << "Adding edge from " << from << " to " << to << ", weight is " << weight << "...\t";

    // Если текущее представление - матрица смежности
    if(type == 'C')
    {
        // Если значение параметра weight равно 0, это будет означать удаление ребра
        if(weight == 0)
        {
            cout << "Can't add edge from " << from << " to " << to << ", weight is equal to zero\n";
            return;
        }

        // Проверить попадание в размерность матрицы
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
    else    // Если текущее представление - список смежности
    if(type == 'L')
    {
        // Итератор отображения - указывает на список связей вершины в списке смежности
        map< int, set< pair<int, int> > >::iterator fromIt;

        // Найти в списке смежности начало ребра
        fromIt = adjList.find(from);

        // Если не найдено
        if(fromIt==adjList.end())
        {
            // Сообщить об ошибке
            cout << "\nCan't add edge from " << from << " to " << to << ", the node doesn't exists\n";
            return;
        }

        // Итератор множества - указывает на соседние вершины в списке связей множества
        set< pair<int, int> >::iterator toIt;

        // Если граф взвешенный
        if(weighted)
        {
            // Найти соседей текущей вершины, используя указанный вес
            toIt = fromIt->second.find(make_pair(to,weight));
        }
        else    // // Если граф невзвешенный
        {
            // Найти соседей текущей вершины, используя в качестве веса 0
            toIt = fromIt->second.find(make_pair(to,0));
        }

        // Если удалось что-то найти
        if(toIt!=fromIt->second.end())
        {
            // Сообщить об ошибке
            cout << "\nCan't add edge from " << from << " to " << to << ", the edge already exists\n";
            return;
        }

        // Если граф взвешенный
        if(weighted)
        {
            // Вставить новую соседнюю вершину, назначить ребру указанный вес
            fromIt->second.insert(make_pair(to,weight));
        }
        else    // Если граф невзвешенный
        {
            // Вставить новую соседнюю вершину, назначить ребру вес 0
            fromIt->second.insert(make_pair(to,0));
        }

        // Если граф неориентированный, то добавить также ребро в обратном направлении
        if(!oriented)
        {
            // Если граф взвешенный
            if(weighted)
            {
                // Использовать указанный вес
                adjList[to].insert(make_pair(from,weight));
            }
            else    // Если граф невзвешенный
            {
                // Использовать вес 0
                adjList[to].insert(make_pair(from,0));
            }
        }
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
        // Проверить попадание в размерность матрицы
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
    else    // Если текущее представление - список смежности
    if(type == 'L')
    {
        // Итератор отображения - указывает на список связей вершины в списке смежности
        map< int, set< pair<int, int> > >::iterator fromIt;

        // Найти в списке смежности начало ребра
        fromIt = adjList.find(from);

        // Если не найдено
        if(fromIt==adjList.end())
        {
            // Вершина не существует, удалять нечего
            return;
        }

        // Итератор множества - указывает на соседние вершины в списке связей множества
        set< pair<int, int> >::iterator toIt;

        // Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
        toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
            [&to](const pair<int, int>& edge)
            {
                if(edge.first==to) return true;
            });

        // Если удалось что-то найти
        if(toIt==fromIt->second.end())
        {
            // Удалить требуемое ребро
            fromIt->second.erase(toIt);
        }

        // Если граф неориентированный, то удалить также ребро в обратном направлении
        if(!oriented)
        {
            // Найти в списке смежности начало ребра
            fromIt = adjList.find(to);

            // Если не найдено
            if(fromIt==adjList.end())
            {
                // Вершина не существует, удалять нечего
                return;
            }

            // Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
            toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
                [&from](const pair<int, int>& edge)
                {
                    if(edge.first==from) return true;
                });

            // Если не удалось ничего найти
            if(toIt==fromIt->second.end())
            {
                // Сообщить об ошибке (в неориентированном графе ребро должно быть в обоих направлениях)
                cout << "\nCan't remove edge from " << to << " to " << from << ", the edge doesn't exists\n";
                return;
            }

            // Удалить требуемое ребро
            fromIt->second.erase(toIt);
        }
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
        // Проверить попадание в размерность матрицы
        if(max(from,to) > adjMatrix.size() || min(from,to) < 0)
        {
            cout << "\nCan't change edge from " << from << " to " << to << ", the node doesn't exists\n";
            return -1;
        }

        // Сохранить прежнее значение веса
        oldWeight = adjMatrix[from][to];

        // Установить в соответствующем столбце значение веса в newWeight
        adjMatrix[from][to] = newWeight;

        // Установить значение симметричного элемента (свойство матрицы смежности) в newWeight
        adjMatrix[to][from] = newWeight;
    }
    else    // Если текущее представление - список смежности
    if(type == 'L')
    {
        // Итератор отображения - указывает на список связей вершины в списке смежности
        map< int, set< pair<int, int> > >::iterator fromIt;

        // Найти в списке смежности начало ребра
        fromIt = adjList.find(from);

        // Если не найдено
        if(fromIt==adjList.end())
        {
            // Сообщить об ошибке
            cout << "\nCan't change edge from " << from << " to " << to << ", the node doesn't exists\n";
            return -1;
        }

        // Итератор множества - указывает на соседние вершины в списке связей множества
        set< pair<int, int> >::iterator toIt;

        // Найти среди соседей вершину, являющуюся концом ребра, которое необходимо изменить
        toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
            [&to](const pair<int, int>& edge)
            {
                if(edge.first==to) return true;
            });

        // Если не удалось ничего найти
        if(toIt!=fromIt->second.end())
        {
            // Сообщить об ошибке
            cout << "\nCan't change edge from " << from << " to " << to << ", the edge doesn't' exists\n";
            return -1;
        }

        // Сохранить прежнее значение веса
        oldWeight = toIt->second;

        // Если граф взвешенный
        if(weighted)
        {
            // Назначить ребру указанный вес
            toIt->second = newWeight;
        }
        else    // Если граф невзвешенный
        {
            // Назначить ребру вес 0
            toIt->second = 0;
        }

        // Если граф неориентированный, то изменить также ребро в обратном направлении
        if(!oriented)
        {
            // Найти в списке смежности начало ребра
            fromIt = adjList.find(to);

            // Если не найдено
            if(fromIt==adjList.end())
            {
                // Вершина не существует, удалять нечего
                return -1;
            }

            // Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
            toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
                [&from](const pair<int, int>& edge)
                {
                    if(edge.first==from) return true;
                });

            // Если не удалось ничего найти
            if(toIt==fromIt->second.end())
            {
                // Сообщить об ошибке (в неориентированном графе ребро должно быть в обоих направлениях)
                cout << "\nCan't change edge from " << to << " to " << from << ", the edge doesn't exists\n";
                return -1;
            }

            // Если граф взвешенный
            if(weighted)
            {
                // Назначить ребру указанный вес
                toIt->second = newWeight;
            }
            else    // Если граф невзвешенный
            {
                // Назначить ребру вес 0
                toIt->second = 0;
            }
        }
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

bool Graph::is_weighted()
{
    return weighted;
}

void Graph::printGraph()
{
    cout << "Printing graph (";

    // Отобразить флаг ориентированности
    if(oriented)
    {
        cout << "oriented, ";
    }
    else
    {
        cout << "not oriented, ";
    }

    // Отобразить флаг взвешенности
    if(weighted)
    {
        cout << "weighted, ";
    }
    else
    {
        cout << "not weighted, ";
    }

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
        for(map< int, set< pair<int, int> > >::iterator fromIt=adjList.begin(); fromIt!=adjList.end(); fromIt++)
        {
            // Бежать по столбцам
            for(set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt!=fromIt->second.end(); toIt++)
            {
                // Напечатать конец текущего ребра
                cout << toIt->first;

                // Если граф взвешенный
                if(weighted)
                {
                    // Напечатать вес текущего ребра
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
        for(map< int, set< pair<int, int> > >::iterator fromIt=adjList.begin(); fromIt!=adjList.end(); fromIt++)
        {
            // Бежать по столбцам
            for(set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt!=fromIt->second.end(); toIt++)
            {
                // Напечатать конец текущего ребра
                file << toIt->first;

                // Если граф взвешенный
                if(weighted)
                {
                    // Напечатать вес текущего ребра
                    file << " " << toIt->second;
                }

                file << " ";
            }

            file << "\n";
        }
    }

    cout << "done\n";
}
