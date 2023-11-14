#include "graph.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tuple>

using namespace std;

class AdjListGraph : public Graph::Graph {
  private:
    map<string, list<tuple<string, int>>> adjList;

  public:
    AdjListGraph();
    AdjListGraph(const uint &_size, const bool &_directed);
    AdjListGraph(const uint &_size, const bool &_directed, const vector<string> &names);

    // Data
    /* uint getEdgeCount(){}; */

    // Edge
    bool viableEdge(const string &from, const string &to) override;
    bool hasEdge(const string &from, const string &to) override;
    bool addEdge(const string &from, const string &to, const uint &weight) override;
    bool removeEdge(const string &from, const string &to) override;

    // Vertex
    bool hasVertex(const string &vertexName) override;
    void addVertex(const uint &newVertexCount) override;
    bool addVertex(const uint &newVertexCount, const vector<string> &vertexNames) override;
    bool removeVertex(const string &vertexName) override;

    // Debug
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

inline AdjListGraph::AdjListGraph() : Graph() {}

inline AdjListGraph::AdjListGraph(const uint &_size, const bool &_directed) : Graph(_size, _directed, false) {
    for (uint i = 0; i < size; ++i) {
        string key(1, static_cast<char>('A' + i));
        adjList[key] = list<tuple<string, int>>();
    }
}

inline AdjListGraph::AdjListGraph(const uint &_size, const bool &_directed, const vector<string> &names) : Graph(_size, _directed, false) {
    if (names.size() != _size) {
        throw invalid_argument("Number of names must match the size of the graph");
    }

    for (uint i = 0; i < size; ++i) {
        adjList[names[i]] = list<tuple<string, int>>();
    }
}

// Edge
inline bool AdjListGraph::viableEdge(const string &from, const string &to) {
    return hasVertex(from) && hasVertex(to);
}

inline bool AdjListGraph::hasEdge(const string &from, const string &to) {
    if (!viableEdge(from, to)) {
        return false;
    }

    auto comp = [&to](const auto &t) { return get<0>(t) == to; };
    auto it = find_if(adjList[from].begin(), adjList[from].end(), comp);

    return it != adjList[from].end();
}

inline bool AdjListGraph::addEdge(const string &from, const string &to, const uint &weight) {
    if (viableEdge(from, to) && !hasEdge(from, to)) {
        adjList[from].push_back(make_tuple(to, weight));
        if (!directed) {
            adjList[to].push_back(make_tuple(from, weight));
        }
        return true;
    }
    return false;
}

inline bool AdjListGraph::removeEdge(const string &from, const string &to) {
    if (hasEdge(from, to)) {
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

// Vertex
inline bool AdjListGraph::hasVertex(const string &vertexName) {
    return adjList.find(vertexName) != adjList.end();
}

inline void AdjListGraph::addVertex(const uint &newVertexCount) {
    for (uint i = size; i < size + newVertexCount; i++) {
        string key(1, static_cast<char>('A' + i));
        adjList[key] = list<tuple<string, int>>();
    }

    size += newVertexCount;
}

inline bool AdjListGraph::addVertex(const uint &newVertexCount, const vector<string> &vertexNames) {
    if (vertexNames.size() != newVertexCount) {
        throw invalid_argument("Number of vertexNames must match the size of newVertexCount");
    }

    for (auto &name : vertexNames) {
        if (hasVertex(name)) {
            return false;
        }
    }

    for (uint i = size; i < size + newVertexCount; i++) {
        adjList[vertexNames[i - size]] = list<tuple<string, int>>();
    }

    size += newVertexCount;

    return true;
}

// Debug
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

inline bool AdjListGraph::removeVertex(const string &vertexName) {}
