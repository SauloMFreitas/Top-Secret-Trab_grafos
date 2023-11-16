#include "graph.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tuple>

using namespace std;

// Definição da classe AdjListGraph, que herda de Graph::Graph
class AdjListGraph : public Graph::Graph {
  private:
    map<string, list<tuple<string, int>>> adjList;  // Lista de adjacência para representar o grafo

  public:
    //-----CONSTRUTORES-----//
    // PADRÃO
    AdjListGraph();

    // PARAMETRIZADO (tamanho, direcionamento)
    AdjListGraph(const uint &_size, const bool &_directed);

    // PARAMETRIZADO (tamanho, direcionamento, nomes de vértices)
    AdjListGraph(const uint &_size, const bool &_directed, const vector<string> &names);

    //-----DATA-----//
    /* uint getEdgeCount(){}; */

    //-----EDGE-----//
    // viabilidade, existência, adição e remoção de arestas
    bool viable_edge(const string &from, const string &to) override;
    bool has_edge(const string &from, const string &to) override;
    bool add_edge(const string &from, const string &to, const uint &weight) override;
    bool remove_edge(const string &from, const string &to) override;

    //-----VERTEX-----//
    // existência, adição e remoção de vértices
    bool has_vertex(const string &vertexName) override;
    void add_vertex(const uint &newVertexCount) override;
    bool add_vertex(const uint &newVertexCount, const vector<string> &vertexNames) override;
    bool remove_vertex(const string &vertexName) override;

    //-----OTHER FUNCTIONS-----//
    bool isCyclicUtil() override;
    bool isCyclic() override;

    //-----DEBUG-----//
    void print() override;

    /* vector<string> dijkstra(const string &start);
    vector<string> bellmanFord(const string &start);
    vector<string> bfs(const string &start);
    vector<string> dfs(const string &start);
    Graph stronglyConnectedComponents();
    Graph connectedComponents();
    vector<string> topologicalSort();
    bool isCyclic();
    uint maxFlow(const string &source, const string &sink);
    void invert();
    void transpose();
    Graph *getSubgraph(const vector<string> &vertices); 
    */
};

//-----CONSTRUTORES-----//
// PADRÃO
inline AdjListGraph::AdjListGraph() : Graph() {}

// PARAMETRIZADO (tamanho, direcionamento)
inline AdjListGraph::AdjListGraph(const uint &_size, const bool &_directed) : Graph(_size, _directed, false) {
    for (uint i = 0; i < size; ++i) {
        string key(1, static_cast<char>('A' + i));
        adjList[key] = list<tuple<string, int>>();
    }
}

// PARAMETRIZADO (tamanho, direcionamento, nomes de vértices)
inline AdjListGraph::AdjListGraph(const uint &_size, const bool &_directed, const vector<string> &names) : Graph(_size, _directed, false) {
    if (names.size() != _size) {
        throw invalid_argument("Number of names must match the size of the graph");
    }

    for (uint i = 0; i < size; ++i) {
        adjList[names[i]] = list<tuple<string, int>>();
    }
}

//-----EDGE-----//
inline bool AdjListGraph::viable_edge(const string &from, const string &to) {
    return has_vertex(from) && has_vertex(to);
}

inline bool AdjListGraph::has_edge(const string &from, const string &to) {
    if (!viable_edge(from, to)) {
        return false;
    }

    auto comp = [&to](const auto &t) { return get<0>(t) == to; };
    auto it = find_if(adjList[from].begin(), adjList[from].end(), comp);

    return it != adjList[from].end();
}

inline bool AdjListGraph::add_edge(const string &from, const string &to, const uint &weight) {
    if (viable_edge(from, to) && !has_edge(from, to)) {
        adjList[from].push_back(make_tuple(to, weight));
        if (!directed) {
            adjList[to].push_back(make_tuple(from, weight));
        }
        return true;
    }
    return false;
}

inline bool AdjListGraph::remove_edge(const string &from, const string &to) {
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

//-----VERTEX-----//
inline bool AdjListGraph::has_vertex(const string &vertexName) {
    return adjList.find(vertexName) != adjList.end();
}

inline void AdjListGraph::add_vertex(const uint &newVertexCount) {
    for (uint i = size; i < size + newVertexCount; i++) {
        string key(1, static_cast<char>('A' + i));
        adjList[key] = list<tuple<string, int>>();
    }

    size += newVertexCount;
}

inline bool AdjListGraph::add_vertex(const uint &newVertexCount, const vector<string> &vertexNames) {
    if (vertexNames.size() != newVertexCount) {
        throw invalid_argument("Number of vertexNames must match the size of newVertexCount");
    }

    for (auto &name : vertexNames) {
        if (has_vertex(name)) {
            return false;
        }
    }

    for (uint i = size; i < size + newVertexCount; i++) {
        adjList[vertexNames[i - size]] = list<tuple<string, int>>();
    }

    size += newVertexCount;

    return true;
}

inline bool AdjListGraph::remove_vertex(const string &vertexName) {}


//-----OTHER FUNCTIONS-----//
    bool isCyclicUtil(string v, map<string, bool> &visited, string parent) {
        visited[v] = true;

        for (auto i : adjList[v]) {
            string adjVertex = get<0>(i);

            if (!visited[adjVertex]) {
                if (isCyclicUtil(adjVertex, visited, v)) {
                    return true;
                }
            }
            else if (adjVertex != parent) {
                // Encontrado um vértice visitado que não é pai, ciclo detectado
                return true;
            }
        }

        return false;
    }

    bool isCyclic() {
        map<string, bool> visited;

        for (auto i : adjList) {
            if (!visited[i.first]) {
                if (isCyclicUtil(i.first, visited, "")) {
                    return true;
                }
            }
        }

        return false;
    }

    
//-----DEBUG-----//
inline void AdjListGraph::print() {
    for (auto &[key, edges] : adjList) {
        cout << key << ": ";
        bool isFirst = true;
        for (auto &[vertex, weight] : edges) {
            if (isFirst) {
                cout << "";
                isFirst = false;
            } else {
                cout << ", ";
            }
            cout << "[" << vertex << " " << weight << "]";
        }
        cout << endl;
    }

    cout << endl;
}