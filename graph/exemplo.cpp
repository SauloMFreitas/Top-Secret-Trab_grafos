#include "graphMatrix.hpp"

int main() {
    Graph *g = new MatrixGraph(4, true, {"Plantador", "Mato", "Inseto", "NPC"});
    g->addEdge("Platador", "Mato", 4);
    g->addEdge("Mato", "Inseto", 3);
    g->addEdge("NPC", "Plantador", 2);
    g->addEdge("Inseto", "NPC", 11);
    g->print();

    g->print();

    return 0;
}
