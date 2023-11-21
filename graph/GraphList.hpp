#include <iostream>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility> // para pair
using namespace std;

class GraphList
{
private:
    int numVertices; // Número de vértices no grafo
    vector<list<pair<int, int>>> adjList; // Lista de adjacência
    bool dfsCiclo(int v, vector<bool> &visitado, vector<bool> &recStack, vector<int> &caminho);

public:
    // ------ CONSTRUCTORS ----- //
    GraphList(int numVertices);

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
    vector<int> encontrarCiclo();
    void contrairCiclo(const vector<int>& ciclo);
    // ------ DEBUG ------ //
    void print();

};

// --- PRIVATE METHODS ---

// ------ metodos de busca ------ //
inline bool GraphList::dfsCiclo(int v, vector<bool> &visitado, vector<bool> &recStack, vector<int> &caminho) {
    if(!visitado[v]) {
        visitado[v] = true;
        recStack[v] = true;
        caminho.push_back(v);

        for(auto& aresta : adjList[v]) {
            int vizinho = aresta.first;
            if (!visitado[vizinho]) {
                if (dfsCiclo(vizinho, visitado, recStack, caminho)) {
                    return true;
                }
            } else if (recStack[vizinho]) {
                // Encontrou um ciclo, agora identifica os nós do ciclo
                std::vector<int> ciclo;
                bool encontrouCiclo = false;
                for (int no : caminho) {
                    if (no == vizinho || encontrouCiclo) {
                        ciclo.push_back(no);
                        encontrouCiclo = true;
                    }
                }
                ciclo.push_back(vizinho); // Adiciona o nó inicial do ciclo ao final para fechá-lo
                caminho = ciclo; // Atualiza o caminho para conter apenas os nós do ciclo
                return true;
            }
        }
    }

    caminho.pop_back();
    recStack[v] = false;
    return false;
}


// --- PUBLIC METHODS ---- 

// ------ CONSTRUCTOR ------ //
inline GraphList::GraphList(int numVertices) {
    this->numVertices = numVertices;
    adjList.resize(numVertices);
}

// ------ EDGES ------//
inline bool GraphList::viable_edge(const int &from, const int &to){
    return has_vertex(from) && has_vertex(to);
}


inline bool GraphList::has_edge(const int &from, const int &to){
    if (!viable_edge(from, to)) {
        return false;
    }

    auto comp = [&to](const auto &t) { return get<0>(t) == to; };
    auto it = find_if(adjList[from].begin(), adjList[from].end(), comp);

    return it != adjList[from].end();
}


inline bool GraphList::add_edge(const int &from, const int &to, const int &weight){
    if (viable_edge(from, to) && !has_edge(from, to)) 
    {
        adjList[from].push_back(make_pair(to, weight));
        return true;
    }
    return false;
}


inline bool GraphList::remove_edge(const int &from, const int &to){
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

inline bool GraphList::has_vertex(const int &vertice){
    return vertice >= 0 && vertice < numVertices;
}

inline void GraphList::add_vertex(const int &newVertexCount){
    for (int i = 0; i < newVertexCount; i++)
    {
        adjList.push_back(list<pair<int, int>>());
        numVertices++;
    }
    
}

inline bool GraphList::remove_vertex(const int &vertice){
    if (vertice < 0 || vertice >= numVertices) {
        cout << "Vértice inválido.\n";
        return false;
    }

    // Etapa 1: Remover todas as arestas que apontam para o vértice
    for (int i = 0; i < numVertices; i++) {
        adjList[i].remove_if([vertice](const pair<int, int>& p) { 
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

inline vector<int> GraphList::encontrarCiclo() {
    vector<bool> visitado(numVertices, false);
    vector<bool> recStack(numVertices, false);
    vector<int> caminho;

    for(int i = 0; i < numVertices; i++) {
        if (dfsCiclo(i, visitado, recStack, caminho)) {
            // Encontrou ciclo, retornar caminho
            return caminho;
        }
    }

    return {}; // Retorna vetor vazio se não encontrar ciclo
}

inline void GraphList::contrairCiclo(const vector<int>& ciclo) {
    int representante = ciclo[0];

    // Para cada nó no ciclo
    for (int no : ciclo) {
        if (no != representante) {
            // Reatribuir arestas de entrada e saída para o nó representante
            for (int i = 0; i < numVertices; ++i) {
                if (i != no) {
                    for (auto &aresta : adjList[i]) {
                        if (aresta.first == no) {
                            aresta.first = representante;
                            // Atualizar peso se necessário
                        }
                    }
                }
            }

            // Mover as arestas de saída do nó atual para o representante
            for (auto &aresta : adjList[no]) {
                if (find(ciclo.begin(), ciclo.end(), aresta.first) == ciclo.end()) {
                    adjList[representante].push_back(aresta);
                }
            }

            // Remover o nó do ciclo da lista de adjacência
            adjList[no].clear();
        }
    }
}

// ------ DEBUG ------ //
inline void GraphList::print(){
    for (int i = 0; i < numVertices; i++) {
        cout << "vértice " << i << ":";
        for (auto it = adjList[i].begin(); it != adjList[i].end(); it++) {
            cout << "["<< it->first << ", " << it->second << "]" << " ";
        }
        cout << "\n";
    }
}