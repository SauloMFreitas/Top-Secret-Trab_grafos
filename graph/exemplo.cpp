#include "graphAdjList.hpp"

int main() {
    Graph *g = new AdjListGraph(4, true);
    g->add_edge("A", "B", 4);
    g->add_edge("B", "C", 3);
    g->add_edge("D", "A", 2);
    g->add_edge("C", "D", 11);
    g->print();

    return 0;
}