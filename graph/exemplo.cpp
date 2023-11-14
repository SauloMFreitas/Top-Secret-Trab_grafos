#include "graphMatrix.hpp"

int main() {
    Graph *g = new MatrixGraph(4, true, {"Plantador", "Mato", "Inseto", "NPC"});
    g->add_edge("Platador", "Mato", 4);
    g->add_edge("Mato", "Inseto", 3);
    g->add_edge("NPC", "Plantador", 2);
    g->add_edge("Inseto", "NPC", 11);
    g->print();

    g->print();

    return 0;
}