// Файл "main.cpp"
// Основной файл программы

#include "Graph.h"  // Подключить описание класса Graph
#include <iostream> // Ввод-вывод
#include <fstream>  // Файловый ввод-вывод

// Сделать все функции и объекты стандартной библиотеки "видимыми" в текущем коде
using namespace std;

// Основная функция программы
int main()
{
    // Создать объект graph класса Graph
    Graph graph;

    // Поприветствовать пользователя
    cout << "Graph operations and algorithms.\n";
    cout << "(C) Dremov V., SUSU, 2015.\n";

    // Переменная для хранения пользовательского выбора
    string choice;

    // Опрашивать пользователя и выполнять требуемое
    do
    {
        // Показать основное меню
        cout << "\n\nWhat would you like to do?\n";
        cout << "[1] Read graph from file\n";
        cout << "[2] Save graph as\n";
        cout << "[3] Add an edge\n";
        cout << "[4] Change an edge\n";
        cout << "[5] Remove an edge\n";
        cout << "[6] Transform to adjacency list\n";
        cout << "[7] Transform to adjacency matrix\n";
        cout << "[8] Transform to edge list\n";
        cout << "[9] Create examples\n";
        cout << "[0] Exit\n";

        // Получить выбор пользователя
        cin >> choice;

        if(choice == "1")
        {
            cout << "Reading graph...\n";

            // Переменная для хранения имени файла
            string fileName;

            // Получить имя файла от пользователя
            cout << "\tEnter file name > ";
            cin >> fileName;

            // Считать граф из указанного файла
            graph.readGraph(fileName);
        }
        else if(choice == "2")
        {
            cout << "Saving graph...\n";

            // Переменная для хранения имени файла
            string fileName;

            // Получить имя файла от пользователя
            cout << "\tEnter file name > ";
            cin >> fileName;

            // Сохранить граф в указанный файл
            graph.writeGraph(fileName);
        }
        else if(choice == "3")
        {
            cout << "Adding an edge...\n";

            // Переменные для хранения номеров вершин и веса ребра
            int from = 0;
            int to = 0;
            int weight = 0;

            // Получить номер первой вершины от пользователя
            cout << "\tEnter \"from\" vertex > ";
            cin >> from;

            // Получить номер второй вершины от пользователя
            cout << "\tEnter \"to\" vertex > ";
            cin >> to;

            // Если граф взвешенный
            if(graph.is_weighted())
            {
                // Получить значение веса нового ребра от пользователя
                cout << "\tEnter a weight > ";
                cin >> weight;
            }

            // Добавить ребро с указанными параметрами
            graph.addEdge(from,to,weight);
        }
        else if(choice == "4")
        {
            cout << "Changing an edge...\n";

            // Переменные для хранения номеров вершин и веса ребра
            int from = 0;
            int to = 0;
            int newWeight = 0;
            int oldWeight = 0;

            // Получить номер первой вершины от пользователя
            cout << "\tEnter \"from\" vertex > ";
            cin >> from;

            // Получить номер второй вершины от пользователя
            cout << "\tEnter \"to\" vertex > ";
            cin >> to;

            // Получить новое значение веса редактируемого ребра от пользователя
            cout << "\tEnter a new weight > ";
            cin >> newWeight;

            // Редактировать ребро с указанными параметрами
            oldWeight = graph.changeEdge(from,to,newWeight);

            // Напечатать прежнее значение веса
            cout << "\tThe previous weight of " << from << "-" << to << " edge was " << oldWeight << "\n";
        }
        else if(choice == "5")
        {
            cout << "Removing an edge...\n";

            // Переменные для хранения номеров вершин
            int from = 0;
            int to = 0;

            // Получить номер первой вершины от пользователя
            cout << "\tEnter \"from\" vertex > ";
            cin >> from;

            // Получить номер второй вершины от пользователя
            cout << "\tEnter \"to\" vertex > ";
            cin >> to;

            // Удалить указанное ребро
            graph.removeEdge(from, to);
        }
        else if(choice == "6")
        {
            graph.transformToAdjList();
        }
        else if(choice == "7")
        {
            graph.transformToAdjMatrix();
        }
        else if(choice == "8")
        {
            graph.transformToListOfEdges();
        }
        else if(choice == "9")
        {
            cout << "Generating examples...\n";

            {
                string name = "adjmatrix.txt";

                cout << "\tfile " << name;

                ofstream file(name);
                file << "C 3\n"
                        "0 1\n"
                        "0 2 0\n"
                        "2 0 5\n"
                        "0 5 0\n";

                cout << "\tok\n";
            }

            {
                string name = "adjlist.txt";

                cout << "\tfile " << name;

                ofstream file(name);
                file << "L 3\n"
                        "1 1\n"
                        "2 5\n"
                        "0 2\n"
                        "\n";

                cout << "\tok\n";
            }

            {
                string name = "edglist.txt";

                cout << "\tfile " << name;

                ofstream file(name);
                file << "E 3 2\n"
                        "1 1\n"
                        "0 1 2\n"
                        "0 2 5\n";

                cout << "\tok\n";
            }

            cout << "done\n";
        }
    } while(choice != "0");

    return 0;
}
