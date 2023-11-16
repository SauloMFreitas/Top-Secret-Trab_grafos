#include <string>
#include <vector>

using namespace std;
typedef unsigned int uint;

class Graph {
  protected:
    uint size;        // Número de vértices do grafo
    bool directed;    // Indica se o grafo é direcionado
    bool multigraph;  // Indica se o grafo pode ter múltiplas arestas entre os mesmos vértices

  public:
    //-----CONSTRUTORES-----//
    // PADRÃO
    Graph() : size(0), directed(false), multigraph(false) {}
    
    // PARAMETRIZADO (tamanho, direcionamento, é multigrafo)
    Graph(uint _size_, bool _directed_, bool _multigraph_) : size(_size_), directed(_directed_), multigraph(_multigraph_) {}
    
    // DESTRUTOR VIRTUAL
    virtual ~Graph() {}

    //-----DATA-----//
    // get_size RETORNA o número de vértices do grafo
    uint get_size() { return size; }

    // is_directed RETORNA se o grafo é direcionado
    bool is_directed() { return directed; }

    // is_multigraph RETORNA se o grafo pode ter múltiplas arestas entre os mesmos vértices
    bool is_multigraph() { return multigraph; }

    /* virtual uint getEdgeCount() const = 0; */

    //-----EDGE-----//
    // viable_edge VERIFICA se uma aresta entre dois vértices é viável
    virtual bool viable_edge(const string &from, const string &to) = 0;
    
    // has_edge VERIFICA se existe uma aresta entre dois vértices
    virtual bool has_edge(const string &from, const string &to) = 0;

    // add_edge ADICIONA uma aresta entre dois vértices (com um peso opcional)
    virtual bool add_edge(const string &from, const string &to, const uint &weight = 1) = 0;
    
    // remove_edge REMOVE uma aresta entre dois vértices
    virtual bool remove_edge(const string &from, const string &to) = 0;

    //-----VERTEX-----//
    // has_vertex VERIFICA se um vértice específico existe no grafo
    virtual bool has_vertex(const string &vertexName) = 0;
    
    // add_vertex ADICIONA um número específico de vértices ao grafo
    virtual void add_vertex(const uint &newVertexCount) = 0;
    
    // add_vertex ADICIONA um número específico de vértices ao grafo, com nomes dados
    virtual bool add_vertex(const uint &newVertexCount, const vector<string> &vertexNames) = 0;
    
    // remove_vertex REMOVE um vértice específico do grafo
    virtual bool remove_vertex(const string &vertexName) = 0;

    //-----OTHER FUNCTIONS-----//

    virtual bool isCyclicUtil(string v, map<string, bool> &visited, string parent) = 0;
    virtual bool isCyclic() = 0;

    //-----IMPRESSÃO E DEBUG-----//
    // print PRINTA grafo (método virtual puro, deve ser implementado nas classes derivadas)
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
