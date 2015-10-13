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
    N = 0;
    M = 0;
}

/*
Функция чтения графа из файла.
Во всех представлениях вершины нумеруются с 0.
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
        file >> N;

        // Считать индикатор ориентированности графа
        file >> oriented;

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
            while (!iss.eof() && !iss.str().empty())
            {
                // Переменная для хранения номера соседней вершины
                int to = 0;

                // Считать соседнюю вершину
                iss >> ws >> to;

                // Переменная для хранения веса ребра
                int weight = 0;

                // Если описан взвешенный граф
                if(weighted)
                {
                    // Считать вес ребра
                    iss >> ws >> weight;
                }

                // Сохранить считанные связи в список связей
                row.insert(make_pair(to, weight));

                // Игнорировать оставшиеся символы в строке
                iss.ignore(128);
            };

            // Добавить список связей считанной вершины в список смежности
            adjList[from] = row;
        }
    }
    else    // Если в файле описан граф, представленный в виде списка ребер
    if(type == 'E')
    {
        // Очистить текущее содержимое списка ребер
        edgList.clear();

        // Считать количество вершин
        file >> N;

        // Считать количество ребер
        file >> M;

        // Считать индикатор ориентированности графа
        file >> oriented;

        // Считать индикатор взвешенности графа
        file >> weighted;

        // Установить курсор чтения файла на новую строку
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        // Считать M ребер
        for(int i = 0; i < M; i++)
        {
            // Строка для хранения текущего ребра
            string rowStr;

            // Считать текущую строку
            getline(file, rowStr);

            // Строковый поток ввода (для разбиения row по отдельным числам)
            istringstream iss(rowStr);

            // Переменная для хранения текущего ребра
            tuple<int, int, int> row;

            // Бежать по rowStr
            while (!iss.eof() && !rowStr.empty())
            {
                // Считать начало ребра
                int from = 0;
                iss >> from;

                // Считать конец ребра
                int to = 0;
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
                get<0>(row) = from;
                get<1>(row) = to;
                get<2>(row) = weight;

            }

            // Добавить ребро в список ребер
            edgList.insert(row);
        }
    }
    else    // Если в файле что-то иное
    {
        // Сообщить об ошибке
        cout << "Can't recognize type of graph representation\n";
        return;
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
    else    // Если текущее представление - спискок ребер
    if(type == 'E')
    {
        // Итератор множества - указывает на ребро в списке ребер
        set< tuple<int, int, int> >::iterator itForward = edgList.end();
        set< tuple<int, int, int> >::iterator itBackward = edgList.end();

        // Проверить, существует ли добавляемое ребро
        itForward = find_if(edgList.begin(), edgList.end(),
            [from](const tuple<int, int, int>& edge)
            {
                if(get<0>(edge)==from) return true;
                return false;
            });

        if(!oriented)
        {
            itBackward = find_if(edgList.begin(), edgList.end(),
                [to](const tuple<int, int, int>& edge)
                {
                    if(get<0>(edge)==to) return true;
                    return false;
                });
        }

        // Если что-то нашли
        if(itForward != edgList.end() || itBackward != edgList.end())
        {
            // Сообщить об ошибке
            cout << "\nCan't add edge from " << from << " to " << to << ", the edge already exists\n";
            return;
        }

        // Добавить требуемое ребро
        if(weighted)
        {
            edgList.insert(make_tuple(from,to,weight));
        }
        else
        {
            edgList.insert(make_tuple(from,to,0));
        }
    }
    else    // Если другой тип
    {
        // Сообщить об ошибке
        cout << "\nCan't recognize type of graph representation\n";
        return;
    }

    // Увеличить счетчик ребер
    M++;

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

        // Итератор множества - указывает на соседние вершины в списке связей множества
        set< pair<int, int> >::iterator toIt;

        // Найти в списке смежности начало ребра
        fromIt = adjList.find(from);

        // Если найдено
        if(fromIt!=adjList.end())
        {            
            // Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
            toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
                [&to](const pair<int, int>& edge)
                {
                    if(edge.first==to) return true;
                    return false;
                });

            // Если удалось что-то найти
            if(toIt!=fromIt->second.end())
            {
                // Удалить требуемое ребро
                fromIt->second.erase(toIt);
            }
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
                    return false;
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
    else    // Если текущее представление - спискок ребер
    if(type == 'E')
    {
        // Итератор множества - указывает на ребро в списке ребер
        set< tuple<int, int, int> >::iterator itForward = edgList.end();

        // Проверить, существует ли удаляемое ребро
        itForward = find_if(edgList.begin(), edgList.end(),
        [&from, &to](const tuple<int, int, int>& edge){
            if(get<0>(edge) == from && get<1>(edge) == to) return true;
            return false;
        });

        // Если что-то нашли, то удалить требуемое ребро
        if(itForward != edgList.end())
        {
            edgList.erase(itForward);
        }

        // Если совсем ничего нет
        if(itForward == edgList.end())
        {
            // Сообщить об ошибке
            cout << "\nCan't remove edge from " << to << " to " << from << ", the edge doesn't exists\n";
            return;
        }
    }
    else    // Если другой тип
    {
        // Сообщить об ошибке
        cout << "\nCan't recognize type of graph representation\n";
        return;
    }

    // Уменьшить счетчик ребер
    M--;

    cout << "done\n";

    // Напечатать граф
    printGraph();
}



int Graph::changeEdge(int from, int to, int newWeight)
{
    cout << "Changing edge from " << from << " to " << to << ", new weight is " << newWeight << "...\t";

    // Если граф невзвешенный
    if(!weighted)
    {
        // Сообщить об ошибке (у ребер нет весов, модифицировать нечего)
        cout << "\tSorry, you can't modify an edge weight of non-weighted graph\n";
        return -1;
    }

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

        fromIt->second.insert(make_pair(1,2));

        // Итератор множества - указывает на соседние вершины в списке связей множества
        set< pair<int, int> >::iterator toIt;

        // Найти среди соседей вершину, являющуюся концом ребра, которое необходимо изменить
        toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
            [&to](const pair<int, int>& edge)
            {
                if(edge.first==to) return true;
                return false;
            });

        // Если не удалось ничего найти
        if(toIt==fromIt->second.end())
        {
            // Сообщить об ошибке
            cout << "\nCan't change edge from " << from << " to " << to << ", the edge doesn't' exists\n";
            return -1;
        }

        // Сохранить прежнее значение веса
        oldWeight = toIt->second;

        // Назначить ребру указанный вес
        pair<int, int> copy = *toIt;
        copy.second = newWeight;
        fromIt->second.erase(toIt);
        fromIt->second.insert(copy);

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
                    return false;
                });

            // Если не удалось ничего найти
            if(toIt==fromIt->second.end())
            {
                // Сообщить об ошибке (в неориентированном графе ребро должно быть в обоих направлениях)
                cout << "\nCan't change edge from " << to << " to " << from << ", the edge doesn't exists\n";
                return -1;
            }

            // Назначить ребру указанный вес
            pair<int, int> copy = *toIt;
            copy.second = newWeight;
            fromIt->second.erase(toIt);
            fromIt->second.insert(copy);
        }
    }
    else    // Если текущее представление - спискок ребер
    if(type == 'E')
    {
        // Итератор множества - указывает на ребро в списке ребер
        set< tuple<int, int, int> >::iterator itForward = edgList.end();
        set< tuple<int, int, int> >::iterator itBackward = edgList.end();

        // Проверить, существует ли изменяемое ребро
        itForward = find_if(edgList.begin(), edgList.end(),
        [&from, &to](const tuple<int, int, int>& edge){
            if(get<0>(edge) == from && get<1>(edge) == to) return true;
            return false;
        });

        // Для неориентированного проверить также ребро в обратном направлении
        if(!oriented)
        {
            itBackward = find_if(edgList.begin(), edgList.end(),
            [&from, &to](const tuple<int, int, int>& edge){
                if(get<0>(edge) == to && get<1>(edge) == from) return true;
                return false;
            });
        }

        // Если что-то нашли, то изменить требуемое ребро
        if(itForward != edgList.end())
        {
            tuple<int, int, int> copy = *itForward;
            oldWeight = get<2>(copy);
            get<2>(copy) = newWeight;
            edgList.erase(itForward);
            edgList.insert(copy);
        }

        if(itBackward != edgList.end())
        {
            tuple<int, int, int> copy = *itBackward;
            oldWeight = get<2>(copy);
            get<2>(copy) = newWeight;
            edgList.erase(itBackward);
            edgList.insert(copy);
        }

        // Если совсем ничего нет
        if(itForward == edgList.end() && itBackward == edgList.end())
        {
            // Сообщить об ошибке
            cout << "\nCan't change edge from " << to << " to " << from << ", the edge doesn't exists\n";
            return -1;
        }
    }
    else    // Если другой тип
    {
        // Сообщить об ошибке
        cout << "\nCan't recognize type of graph representation\n";
        return -1;
    }

    cout << "done\n";

    // Напечатать граф
    printGraph();

    // Вернуть прежнее значение веса
    return oldWeight;
}



void Graph::transformToAdjList()
{
    cout << "Transforming graph from ";

    // Конвертировать в зависимости от типа
    if(type == 'C')
    {
        cout << " adjacency matrix to adjcency list...\t";

        adjList.clear();

        for(size_t i=0; i<adjMatrix.size(); i++)
        {
            for(size_t j=0; j<adjMatrix[i].size(); j++)
            {
                if(adjMatrix[i][j] != 0)
                {
                    adjList[i].insert(make_pair(j,adjMatrix[i][j]));
                }
            }
        }

        adjMatrix.clear();
    }
    else if(type == 'L')
    {
        cout << " adjacency list to adjcency list...\t";
    }
    else if(type == 'E')
    {
        cout << " edge list to adjcency list...\t";

        adjList.clear();

        for(set< tuple<int, int, int> >::iterator it = edgList.begin(); it != edgList.end(); it++)
        {
            adjList[get<0>(*it)].insert(make_pair(get<1>(*it), get<2>(*it)));
        }

        edgList.clear();
    }
    else    // Если другой тип
    {
        // Сообщить об ошибке
        cout << "\nCan't recognize type of graph representation\n";
        return;
    }

    type = 'L';

    cout << "done\n";

    printGraph();
}



void Graph::transformToAdjMatrix()
{
    cout << "Transforming graph from ";

    // Конвертировать в зависимости от типа
    if(type == 'C')
    {
        cout << " adjacency matrix to adjcency matrix...\t";
    }
    else if(type == 'L')
    {
        cout << " adjacency list to adjcency matrix...\t";

        adjMatrix.clear();
        adjMatrix.resize(adjList.size(), vector<int>(adjList.size(), 0));

        for(map< int, set< pair< int, int > > >::iterator i = adjList.begin(); i != adjList.end(); i++)
        {
            for(set< pair< int, int > >::iterator j = i->second.begin(); j != i->second.end(); j++)
            {
                adjMatrix[i->first][j->first] = j->second;
            }
        }

        adjList.clear();
    }
    else if(type == 'E')
    {
        cout << " edge list to adjcency matrix...\t";

        adjMatrix.clear();
        adjMatrix.resize(N, vector<int>(N, 0));

        for(set< tuple< int, int, int > >::iterator it = edgList.begin(); it != edgList.end(); it++)
        {
            adjMatrix[get<0>(*it)][get<1>(*it)] = get<2>(*it);

            if(!oriented)
            {
                adjMatrix[get<1>(*it)][get<0>(*it)] = get<2>(*it);
            }
        }

        edgList.clear();
    }
    else    // Если другой тип
    {
        // Сообщить об ошибке
        cout << "\nCan't recognize type of graph representation\n";
        return;
    }

    type = 'C';

    cout << "done\n";

    printGraph();
}



void Graph::transformToListOfEdges()
{
    cout << "Transforming graph from ";

    // Конвертировать в зависимости от типа
    if(type == 'C')
    {
        cout << " adjacency matrix to edge list...\t";

        edgList.clear();

        for(int i=0; i<adjMatrix.size(); i++)
        {
            for(int j=0; j<adjMatrix[i].size(); j++)
            {
                if(adjMatrix[i][j] != 0)
                {
                    set< tuple< int, int, int > >::iterator it =
                        find_if(edgList.begin(), edgList.end(), [this,i,j](const tuple< int, int, int >& elem)
                        {
                            if(get<0>(elem) == i && get<1>(elem) == j) return true;

                            if(!oriented)
                            {
                                if(get<0>(elem) == j || get<1>(elem) == i) return true;
                            }

                            return false;
                        });

                    if(it == edgList.end())
                    {
                        edgList.insert(make_tuple(i,j,adjMatrix[i][j]));
                    }
                }
            }
        }

        adjMatrix.clear();
    }
    else if(type == 'L')
    {
        cout << " adjacency list to edge list...\t";

        edgList.clear();

        for(map< int, set< pair< int, int > > >::iterator i = adjList.begin(); i != adjList.end(); i++)
        {
            for(set< pair< int, int > >::iterator j = i->second.begin(); j != i->second.end(); j++)
            {
                edgList.insert(make_tuple(i->first, j->first, j->second));
            }
        }

        adjList.clear();
    }
    else if(type == 'E')
    {
        cout << " edge list to edge list...\t";
    }
    else    // Если другой тип
    {
        // Сообщить об ошибке
        cout << "\nCan't recognize type of graph representation\n";
        return;
    }

    type = 'E';

    cout << "done\n";

    printGraph();
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
    else    // Если в файле описан граф, представленный в виде списка ребер
    if(type == 'E')
    {
        cout << "edge list)...\t\n";

        // Бежать по строкам
        for(set< tuple<int, int, int> >::iterator it=edgList.begin(); it!=edgList.end(); it++)
        {
            // Напечатать начало текущего ребра
            cout << get<0>(*it) << " ";

            // Напечатать конец текущего ребра
            cout << get<1>(*it);

            // Если граф взвешенный
            if(weighted)
            {
                // Напечатать вес текущего ребра
                cout << " " << get<2>(*it);
            }

            cout << "\n";
        }
    }
    else    // Если в файле что-то иное
    {
        // Сообщить об ошибке
        cout << "Can't recognize type of graph representation\n";
        return;
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
        file << adjMatrix.size() << "\n";

        // Сохранить флаг ориентированности
        file << oriented << " ";

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
        file << N << "\n";

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
    else    // Если текущее представление - список ребер
    if(type == 'E')
    {
        // Сохранить количество вершин
        file << N << " ";

        // Сохранить количество ребер
        file << M << "\n";

        // Сохранить флаг ориентированности графа
        file << oriented << " ";

        // Сохранить флаг взвешенности графа
        file << weighted << "\n";

        // Бежать по строкам
        for(set< tuple<int, int, int> >::iterator it=edgList.begin(); it!=edgList.end(); it++)
        {
            // Сохранить начало текущего ребра
            file << get<0>(*it) << " ";

            // Сохранить конец текущего ребра
            file << get<1>(*it);

            // Если граф взвешенный
            if(weighted)
            {
                // Сохранить вес текущего ребра
                file << " " << get<2>(*it);
            }

            file << "\n";
        }
    }
    else    // Если в файле что-то иное
    {
        // Сообщить об ошибке
        cout << "Can't recognize type of graph representation\n";
        return;
    }

    cout << "done\n";
}
