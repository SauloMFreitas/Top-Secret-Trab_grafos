#include "DAG.hpp"
int main() {
    DAG *grafo = new DAG(4);

    grafo->add_edge(0, 1, 1);
    grafo->add_edge(0, 2, 3);
    grafo->add_edge(1, 2, 1);
    grafo->add_edge(2, 3, 1);

    grafo->print();

    if (grafo->temCiclo())
        std::cout << "O grafo tem pelo menos um ciclo.\n";
    else
        std::cout << "O grafo não tem ciclo.\n";

    return 0;
}