#include "graph.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

using namespace std;

// Definição da classe MatrixGraph, que herda de Graph::Graph
class MatrixGraph : public Graph::Graph {
  private:
    map<string, uint> nameMap;      // Mapeia os nomes dos vértices para seus índices na matriz
    vector<vector<uint>> matrix;    // Representa a matriz de adjacência do grafo

    bool _has_edge(const string &from, const string &to);

  public:
    //-----CONSTRUTORES-----//
    // PADRÃO
    MatrixGraph();

    // PARAMETRIZADO (tamanho, direcionamento)
    MatrixGraph(const uint &_size, const bool &_directed);

    // PARAMETRIZADO (tamanho, direcionamento, nome de vértices)
    MatrixGraph(const uint &_size, const bool &_directed, const vector<string> &names);

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
    Graph *getSubgraph(const vector<string> &vertices); */
};

//-----CONSTRUTORES-----//
// PADRÃO
inline MatrixGraph::MatrixGraph() : Graph() {}

// PARAMETRIZADO (tamanho, direcionamento)
inline MatrixGraph::MatrixGraph(const uint &_size, const bool &_directed) : Graph(_size, _directed, false) {
    matrix = vector<vector<uint>>(_size, vector<uint>(_size, 0));

    for (uint i = 0; i < size; ++i) {
        string key(1, static_cast<char>('A' + i));
        nameMap[key] = i;
    }
}

// PARAMETRIZADO (tamanho, direcionamento, nome de vértices)
inline MatrixGraph::MatrixGraph(const uint &_size, const bool &_directed, const vector<string> &names) : Graph(_size, _directed, false) {
    if (names.size() != _size) {
        throw invalid_argument("Number of names must match the size of the graph");
    }

    matrix = vector<vector<uint>>(size, vector<uint>(size, 0));
    for (uint i = 0; i < size; ++i) {
        nameMap[names[i]] = i;
    }
}

//-----EDGE-----//
inline bool MatrixGraph::viable_edge(const string &from, const string &to) {
    return has_vertex(from) && has_vertex(to);
}

inline bool MatrixGraph::has_edge(const string &from, const string &to) {
    return viable_edge(from, to) ? matrix[nameMap[from]][nameMap[to]] : false;
}

inline bool MatrixGraph::_has_edge(const string &from, const string &to) {
    return matrix[nameMap[from]][nameMap[to]];
}

inline bool MatrixGraph::add_edge(const string &from, const string &to, const uint &weight = 1) {
    if (viable_edge(from, to) && !_has_edge(from, to)) {
        matrix[nameMap[from]][nameMap[to]] = weight;
        if (!directed) {
            matrix[nameMap[to]][nameMap[from]] = weight;
        }
        return true;
    }
    return false;
}

inline bool MatrixGraph::remove_edge(const string &from, const string &to) {
    if (has_edge(from, to)) {
        matrix[nameMap[from]][nameMap[to]] = 0;
        if (!directed) {
            matrix[nameMap[to]][nameMap[from]] = 0;
        }
        return true;
    }
    return false;
}

//-----VERTEX-----//
inline bool MatrixGraph::has_vertex(const string &vertexName) {
    return nameMap.find(vertexName) != nameMap.end();
}

inline void MatrixGraph::add_vertex(const uint &newVertexCount) {
    matrix.insert(matrix.end(), newVertexCount, vector<uint>(matrix.size(), 0));

    for (auto &row : matrix) {
        row.insert(row.end(), newVertexCount, 0);
    }

    for (uint i = size; i < size + newVertexCount; i++) {
        string key(1, static_cast<char>('A' + i));
        nameMap[key] = i;
    }

    size += newVertexCount;
}

inline bool MatrixGraph::add_vertex(const uint &newVertexCount, const vector<string> &vertexNames) {
    if (vertexNames.size() != newVertexCount) {
        throw invalid_argument("Number of vertexNames must match the size of newVertexCount");
    }

    for (auto &name : vertexNames) {
        if (has_vertex(name)) {
            return false;
        }
    }

    matrix.insert(matrix.end(), newVertexCount, vector<uint>(matrix.size(), 0));

    for (auto &row : matrix) {
        row.insert(row.end(), newVertexCount, 0);
    }

    for (uint i = size; i < size + newVertexCount; i++) {
        nameMap[vertexNames[i - size]] = i;
    }

    size += newVertexCount;

    return true;
}

inline bool MatrixGraph::remove_vertex(const string &vertexName) {
    if (has_vertex(vertexName)) {
        uint index = nameMap[vertexName];
        matrix.erase(matrix.begin() + index);

        for (auto &row : matrix) {
            row.erase(row.begin() + index);
        }

        nameMap.erase(vertexName);

        return true;
    }

    return false;
}

//-----DEBUG-----//
inline void MatrixGraph::print() {
    vector<pair<uint, string>> value_name;
    value_name.reserve(size);
    for (const auto &nv : nameMap) {
        value_name.emplace_back(make_pair(nv.second, nv.first));
    }

    sort(value_name.begin(), value_name.end());

    vector<string> names;
    names.reserve(size);
    for (const auto &vn : value_name) {
        names.push_back(vn.second);
    }

    auto comp = [](const string &a, const string &b) -> bool { return a.size() < b.size(); };
    size_t offset_t = max_element(names.begin(), names.end(), comp)->size();

    offset_t++;
    cout << string(offset_t, ' ');

    int offset = static_cast<int>(offset_t);
    for (auto &name : names) {
        cout << left << setw(offset) << name;
    }
    cout << endl;
    for (size_t i = 0; i < matrix.size(); i++) {
        cout << left << setw(offset) << names[i];
        for (size_t j = 0; j < matrix[i].size(); j++) {
            cout << left << setw(offset) << matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
}
