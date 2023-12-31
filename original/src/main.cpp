#define _MATRIX_DEBUG_

#include "../include/graphy.hpp"

int main() {
#ifndef _MATRIX_DEBUG_
    Graphy::Graph *g = new Graphy::AdjList(7, true);
    g->addEdge("A", "B", 1);
    g->addEdge("B", "C", 3);
    g->addEdge("C", "D", 4);
    g->addEdge("D", "E", 5);
    g->addEdge("E", "C", 1);
    g->addEdge("D", "B", 9);
    g->addEdge("B", "F", 2);
    g->addEdge("D", "G", 1);
    g->addEdge("G", "A", 2);
    g->print();

    Graphy::Graph *a = g->arborescence("E");
    a->print();

#else
    Graphy::Graph *g = new Graphy::Matrix(3, true);
    g->addEdge("A", "B", 1);
    g->addEdge("B", "C", 2);
    g->addEdge("C", "A", 3);
    g->print();

#endif

    return 0;
}
