// Файл "main.cpp"
// Основной файл программы

// Подключить описание класса Graph
#include "Graph.h"

// Основная функция программы
int main()
{
    // Создать объект graph класса Graph
    Graph graph;

    // Считать описание графа из файла file.txt
    graph.readGraph("file.txt");

    return 0;
}
