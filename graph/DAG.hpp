#include <iostream>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility> // para pair
using namespace std;

class DAG
{
private:
        int numVertices; // Número de vértices no grafo
        vector<list<pair<int, int>>> adjList; // Lista de adjacência
        bool dfs(int v, std::vector<bool> &visitado, std::vector<bool> &recStack);
public:
    // ------ CONSTRUCTORS ----- //
    DAG(int numVertices);

    // ------ EDGES ------//

    bool viable_edge(const int &from, const int &to);
    bool has_edge(const int &from, const int &to);
    bool add_edge(const int &from, const int &to, const int &weight);
    bool remove_edge(const int &from, const int &to);

    // ------ VERTEX ------ //

    bool has_vertex(const int &vertexName);
    void add_vertex(const int &newVertexCount);
    bool remove_vertex(const int &vertexName);

    // ------ OTHERS ------ //
    bool temCiclo();
    // ------ DEBUG ------ //
    void print();

};

// --- PRIVATE METHODS ---

// ------ metodos de busca ------ //
inline bool DAG::dfs(int v, std::vector<bool> &visitado, std::vector<bool> &recStack) {
    if(!visitado[v]) {
        // Marcar o nó atual como visitado e parte da pilha de recursão
        visitado[v] = true;
        recStack[v] = true;

        // Recorrer aos vértices adjacentes
        for(auto adjacente : adjList[v]) {
            int w = adjacente.first;
            if (!visitado[w] && dfs(w, visitado, recStack))
                return true;
            else if (recStack[w])
                return true;
        }
    }

    // Remover o nó da pilha de recursão
    recStack[v] = false;
    return false;
}


// --- PUBLIC METHODS ---- 

// ------ CONSTRUCTOR ------ //
inline DAG::DAG(int numVertices) {
    this->numVertices = numVertices;
    adjList.resize(numVertices);
}

// ------ EDGES ------//
inline bool DAG::viable_edge(const int &from, const int &to){
    return has_vertex(from) && has_vertex(to);
}


inline bool DAG::has_edge(const int &from, const int &to){
    if (!viable_edge(from, to)) {
        return false;
    }

    auto comp = [&to](const auto &t) { return get<0>(t) == to; };
    auto it = find_if(adjList[from].begin(), adjList[from].end(), comp);

    return it != adjList[from].end();
}


inline bool DAG::add_edge(const int &from, const int &to, const int &weight){
    if (viable_edge(from, to) && !has_edge(from, to)) 
    {
        adjList[from].push_back(std::make_pair(to, weight));
        return true;
    }
    return false;
}


inline bool DAG::remove_edge(const int &from, const int &to){
    if (has_edge(from, to)) {
        for (auto it = adjList[from].begin(); it != adjList[from].end(); ++it) {
            if (get<0>(*it) == to) {
                adjList[from].erase(it);
                it = adjList[from].end();
            }
        }
        return true;
    }
    return false;
}

// ------ VERTEX ------ //

inline bool DAG::has_vertex(const int &vertice){
    return vertice >= 0 && vertice < numVertices;
}

inline void DAG::add_vertex(const int &newVertexCount){
    for (int i = 0; i < newVertexCount; i++)
    {
        adjList.push_back(list<pair<int, int>>());
        numVertices++;
    }
    
}

inline bool DAG::remove_vertex(const int &vertice){
    if (vertice < 0 || vertice >= numVertices) {
        std::cout << "Vértice inválido.\n";
        return false;
    }

    // Etapa 1: Remover todas as arestas que apontam para o vértice
    for (int i = 0; i < numVertices; i++) {
        adjList[i].remove_if([vertice](const std::pair<int, int>& p) { 
            return p.first == vertice; 
        });
    }

    // Etapa 2: Remover o vértice da lista de adjacências
    adjList.erase(adjList.begin() + vertice);
    numVertices--;
    
    // Ajustar os índices dos vértices nas listas de adjacências
    for (auto &lista : adjList) {
        for (auto &par : lista) {
            if (par.first > vertice) {
                par.first--;
            }
        }
    }
    return true;

}

// ------ OTHERS ------ //

inline bool DAG::temCiclo() {
    std::vector<bool> visitado(numVertices, false);
    std::vector<bool> recStack(numVertices, false);

    for(int i = 0; i < numVertices; i++)
        if (dfs(i, visitado, recStack))
            return true;

    return false;
}

// ------ DEBUG ------ //
inline void DAG::print(){
    for (int i = 0; i < numVertices; i++) {
        std::cout << "vértice " << i << ":";
        for (auto it = adjList[i].begin(); it != adjList[i].end(); it++) {
            std::cout << "["<< it->first << ", " << it->second << "]" << " ";
        }
        std::cout << "\n";
    }
}