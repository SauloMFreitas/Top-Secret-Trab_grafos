#include <string>
#include <vector>

using namespace std;
typedef unsigned int uint;

class Graph {
  protected:
    uint size;
    bool directed;
    bool multigraph;

  public:
    Graph() : size(0), directed(false), multigraph(false) {}
    Graph(uint _size_, bool _directed_, bool _multigraph_) : size(_size_), directed(_directed_), multigraph(_multigraph_) {}
    virtual ~Graph() {}

    // Data
    uint getSize() { return size; }
    bool isDirected() { return directed; }
    bool isMultigraph() { return multigraph; }
    /* virtual uint getEdgeCount() const = 0; */

    // Edge
    virtual bool viableEdge(const string &from, const string &to) = 0;
    virtual bool hasEdge(const string &from, const string &to) = 0;
    virtual bool addEdge(const string &from, const string &to, const uint &weight = 1) = 0;
    virtual bool removeEdge(const string &from, const string &to) = 0;

    // Vertex
    virtual bool hasVertex(const string &vertexName) = 0;
    virtual void addVertex(const uint &newVertexCount) = 0;
    virtual bool addVertex(const uint &newVertexCount, const vector<string> &vertexNames) = 0;
    virtual bool removeVertex(const string &vertexName) = 0;

    // Impressão e Debug
    virtual void print() = 0;

    /* // Algoritmos de Caminho Mínimo
    virtual vector<string> dijkstra(const string &start) = 0;
    virtual vector<string> bellmanFord(const string &start) = 0;

    // Algoritmos de Busca
    virtual vector<string> bfs(const string &start) = 0;
    virtual vector<string> dfs(const string &start) = 0;

    // Algoritmos de Componentes
    virtual Graph stronglyConnectedComponents() = 0;
    virtual Graph connectedComponents() = 0;

    // Outros Algoritmos
    virtual vector<string> topologicalSort() = 0;
    virtual bool isCyclic() = 0;

    // Algoritmos de Fluxo
    virtual uint maxFlow(const string &source, const string &sink) = 0;

    // Utilitários
    virtual void invert() = 0;
    virtual void transpose() = 0;
    virtual Graph *getSubgraph(const vector<string> &vertices) = 0; */
};
