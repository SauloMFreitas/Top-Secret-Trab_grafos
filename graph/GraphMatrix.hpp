#include <iostream>
#include <vector>
#include <climits>
#include <stack>
#include <map>  
#include <set>
#include <algorithm>

using namespace std;    


class GraphMatrix
{
private:

    // ----- estrutura para armazenar as informações das arestas
    struct Edge
    {
        int source, target, score;
        vector<Edge> kicksOut;
        Edge(int s, int t, int score):source(s), target(t), score(score){}
        Edge() : source(-1), target(-1), score(INT_MAX){}
    };

    // ----- variáveis privadas

    vector<vector<Edge>> adjMatrix;
    vector<Edge> BestInEdge;
    int numVertices;

    // ---------- Private Methods ---------- //
    // ----- EDGE
    bool _hasEdge(const int &from, const int &to);

    // ----- métodos auxiliares de Edmonds Algorithm
    void preprocess_edmonds(const int &root);
    void contracting_stage(const int &root);
    void expanding_stage(const int &root);
    pair<int, int> BestEdge(const int &vertex);
    void contract_cycle(vector<int>& cycle);
    bool dfs(int v, const vector<vector<int>>& graph, vector<bool>& visited, vector<bool>& stack, vector<int>& cycle);
public:
    // ------ CONSTRUCTORS ----- //
    GraphMatrix(int numVertices);

    // ------ EDGES ------//

    bool viable_edge(const int &from, const int &to);
    bool has_edge(const int &from, const int &to);
    bool add_edge(const int &from, const int &to, const int &weight);
    bool remove_edge(const int &from, const int &to);

    // ------ VERTEX ------ //

    bool has_vertex(const int &vertex);
    void add_vertex(const int &newVertexCount);
    bool remove_vertex(const int &vertex);

    // ------ OTHERS ------ //
    // --- find cycles
    vector<int> findCycles();
    // ------ ALGORITMOS DE ARBORENCÊNCIA ------ //
    void edmonds_algorithm(const int &root);

    // ------ DEBUG ------ //
    void print();


};

// ------ CONSTRUCTORS ----- //

inline GraphMatrix::GraphMatrix(int numVertices){
    this->numVertices = numVertices;
    this->adjMatrix.resize(numVertices, vector<Edge>(numVertices, Edge()));
    this->BestInEdge.resize(numVertices, Edge());
}

// ------ EDGES ------//

inline bool GraphMatrix::viable_edge(const int &from, const int &to){
    return has_vertex(from) && has_vertex(to);
}
inline bool GraphMatrix::has_edge(const int &from, const int &to){
    return viable_edge(from, to) ? adjMatrix[from][to].score != INT_MAX : false;
}
inline bool GraphMatrix::add_edge(const int &from, const int &to, const int &weight){
    if (viable_edge(from, to) && !_hasEdge(from, to)) {
        adjMatrix[from][to].source = from;
        adjMatrix[from][to].target = to;
        adjMatrix[from][to].score = weight;
        return true;
    }
    return false;
}
inline bool GraphMatrix::remove_edge(const int &from, const int &to){
    if (has_edge(from, to)) {
        adjMatrix[from][to].source = -1;
        adjMatrix[from][to].target = -1;
        adjMatrix[from][to].score = INT_MAX;
        return true;
    }
    return false;
    
}

// ------ VERTEX ------ //
inline bool GraphMatrix::has_vertex(const int &vertex){
    return vertex >= 0 && vertex < numVertices;
}

inline void GraphMatrix::add_vertex(const int &newVertexCount){
    numVertices += newVertexCount;
    for(auto &row : adjMatrix){
        row.resize(numVertices, Edge());
    }
    for (int i = 0; i < newVertexCount; ++i) {
        adjMatrix.push_back(vector<Edge>(numVertices, Edge()));
    }
}

inline bool GraphMatrix::remove_vertex(const int &vertex){
    if (!has_vertex(vertex)) {
        cout << "Vertice invalido!" << endl;
        return false;
    }

    // Remove a linha x 
    adjMatrix.erase(adjMatrix.begin() + vertex);    

    // Remove a coluna x de cada linha restante
    for (auto& row : adjMatrix) {
        row.erase(row.begin() + vertex);
    }
    // Diminui o número de vértices
    numVertices--;
    return true;
}
// ------ ALGORITMOS DE ARBORENCÊNCIA ------ //

// algoritmo implementado baseado na apresentação de Sam Thompson
// pela universidade de Washington, CSE 490u em 22 de fevereiro de 2017
//segue o link da apresentação: https://courses.cs.washington.edu/courses/cse490u/17wi/slides/CLE.pdf
inline void GraphMatrix::edmonds_algorithm(const int &root){
    preprocess_edmonds(root);
    contracting_stage(root);
}

// ------ DEBUG ------ //
inline void GraphMatrix::print(){
    cout << "  ";
    for (int i = 0; i < numVertices; i++)
    {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < numVertices; ++i) {
        cout << i << " ";
        for (int j = 0; j < numVertices; ++j) {
            if (adjMatrix[i][j].score == INT_MAX)
            {
                cout << "0 ";
            }
            else cout << adjMatrix[i][j].score << " ";
        }
        cout << endl;
    }
}



// ---------- Private Methods ---------- //
// ----- EDGES
inline bool GraphMatrix::_hasEdge(const int &from, const int &to) {
    return adjMatrix[from][to].score != INT_MAX;
}

// ----- métodos auxiliares de Edmonds Algorithm
inline void GraphMatrix::preprocess_edmonds(const int &root){
    //remove todas as arestas de entrada do vértice raiz (root)
    for (int u = 0; u < numVertices; u++)
    {
        if(adjMatrix[u][root].score == INT_MAX) continue;
        adjMatrix[u][root].score = INT_MAX;
    }
}

inline void GraphMatrix::contracting_stage(const int &root){
    pair<int, int > bestEdges;

    for (int v = 0; v < numVertices; v++)
    {
        // 1. para cada vértice v que não seja a raiz(root), 
        //selecione a aresta de menor peso e coloque em BestInEdge[v]
        if(v == root) continue;
        bestEdges = BestEdge(v);
        BestInEdge[v].source = bestEdges.first;
        BestInEdge[v].target = v;
        BestInEdge[v].score = bestEdges.second;

        // 2. a cada seleção, verifique se não gerou ciclos
        vector<int> cycle = findCycles();
        if (!cycle.empty())// 3. se gerar ciclo, contraia, o nó para Vc
        {
            cout << "ciclo encontrado" << endl;
            for(const auto &u : cycle) cout << u << " ";
            cout << endl;

            contract_cycle(cycle);
        }
    }
    
}

inline void GraphMatrix::expanding_stage(const int &root){

}

inline pair<int, int> GraphMatrix::BestEdge(const int &vertex){

    pair<int, int> bestEdge = {-1, INT_MAX};
    for (int v = 0; v < numVertices; v++)
    {
        if (adjMatrix[v][vertex].score < bestEdge.second){
            bestEdge.first = v;
            bestEdge.second = adjMatrix[v][vertex].score;
        }
        
    }

    return bestEdge;
    
}
inline void GraphMatrix::contract_cycle(vector<int>& cycle) {
    int n = numVertices++;
    int superVertex = n; // Escolhendo o primeiro vértice do ciclo como supervértice
    adjMatrix.resize(numVertices, vector<Edge>(numVertices, Edge()));
    BestInEdge.resize(numVertices, Edge());

    // Inicializando um vetor para marcar os vértices do ciclo
    vector<bool> isInCycle(n, false);
    for (int v : cycle) {
        isInCycle[v] = true;
    }

    // Para cada vertice u em C, e para cada aresta e de entrada de fora do ciclo para u
    for(const auto &u : cycle){
        for (int v = 0; v < numVertices; v++)
        {
            if (adjMatrix[v][u].score != INT_MAX)
            { 
                // coloque e.kicksOut = BestInEdge[u]
                adjMatrix[v][u].kicksOut.push_back(BestInEdge[u]);
                // atualize e.score para ser e.score - e.kicksOut.score
                adjMatrix[v][u].score -= adjMatrix[v][u].kicksOut.back().score;
            }
            
        }
        
    }

    for (int i = 0; i < n; ++i) {
        if (!isInCycle[i]) {
            // Para cada vértice fora do ciclo, redirecione as arestas para o supervértice
            for (int v : cycle) {
                if (adjMatrix[i][v].score != INT_MAX) {
                    adjMatrix[i][superVertex].score = adjMatrix[i][v].score;
                    adjMatrix[i][v].score = INT_MAX;
                }
                if (adjMatrix[v][i].score != INT_MAX) {
                    adjMatrix[superVertex][i].score = adjMatrix[v][i].score;
                    adjMatrix[v][i].score = INT_MAX;
                }
            }
        }
    }
    

    // Removendo arestas internas do ciclo
    for (int i = 0; i < (int)cycle.size(); ++i) {
        for (int j = 0; j < (int)cycle.size(); ++j) {
            adjMatrix[cycle[i]][cycle[j]].score = INT_MAX;
        }
    }
}


inline vector<int> GraphMatrix::findCycles() {
    vector<vector<int>> graph;
    graph.resize(numVertices);
    cout << "aa"<<endl;
    for (const auto& edge : BestInEdge) {
        cout << edge.source << " " << edge.target  << " " << edge.score << endl;
        if (edge.source >= 0 && edge.target >= 0 && adjMatrix[edge.source][edge.target].score != INT_MAX)
        {
            graph[edge.target].push_back(edge.source);
        }
        
    }

    vector<bool> visited(numVertices, false), stack(numVertices, false);
    vector<int> cycle;

    for (int i = 0; i < numVertices; ++i) {
        if (!visited[i] && dfs(i, graph, visited, stack, cycle)) {
            break;
        }
    }

    return cycle;
}

inline bool GraphMatrix::dfs(int v, const vector<vector<int>>& graph, vector<bool>& visited, vector<bool>& stack, vector<int>& cycle) {
    if (!visited[v]) {
        visited[v] = true;
        stack[v] = true;

        for (int u : graph[v]) {
            if (!visited[u] && dfs(u, graph, visited, stack, cycle)) {
                if (cycle.empty() || stack[cycle.back()]) { // Checa se ainda estamos no ciclo
                    cycle.push_back(u); // Adiciona o vértice ao ciclo
                }
                return true;
            } else if (stack[u]) {
                cycle.push_back(u); // Adiciona o vértice ao ciclo
                return true;
            }
        }
    }

    stack[v] = false;
    return false;
}