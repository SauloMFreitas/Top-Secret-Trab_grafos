#include <iostream>
#include <vector>
#include <climits>
#include <stack>


using namespace std;    

class GraphMatrix
{
private:
    vector<vector<int>> adjMatrix;
    int numVertices;
    bool _hasEdge(const int &from, const int &to);
    bool dfs(int v, vector<bool>& visitado, vector<bool>& pilhaRecursiva, vector<int>& parent, stack<int>& ciclo);

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
    vector<int> findCycles();
    void contract(const vector<int>& ciclo);
    // ------ DEBUG ------ //
    void print();

};

// --- PRIVATE FUNCTIONS

inline bool GraphMatrix::dfs(int v, vector<bool>& visitado, vector<bool>& pilhaRecursiva, vector<int>& parent, stack<int>& ciclo){
    visitado[v] = true;
    pilhaRecursiva[v] = true;

    for (int i = 0; i < numVertices; ++i) {
        if (adjMatrix[v][i] != INT_MAX) {
            if (!visitado[i]) {
                parent[i] = v;
                if (dfs(i, visitado, pilhaRecursiva, parent, ciclo)) return true;
            } else if (pilhaRecursiva[i]) {
                ciclo.push(i);
                int u = v;
                while (u != i) {
                    ciclo.push(u);
                    u = parent[u];
                }
                ciclo.push(i);
                return true;
            }
        }
    }

    pilhaRecursiva[v] = false;
    return false;
}

// ------ CONSTRUCTORS ----- //
inline GraphMatrix::GraphMatrix(int numVertices){
    this->numVertices = numVertices;
    adjMatrix.resize(numVertices, vector<int>(numVertices, INT_MAX));
}

// ------ EDGES ------//
inline bool GraphMatrix::viable_edge(const int &from, const int &to) {
    return has_vertex(from) && has_vertex(to);
}

inline bool GraphMatrix::has_edge(const int &from, const int &to) {
    return viable_edge(from, to) ? adjMatrix[from][to] != INT_MAX : false;
}

inline bool GraphMatrix::_hasEdge(const int &from, const int &to) {
    return adjMatrix[from][to] != INT_MAX;
}

inline bool GraphMatrix::add_edge(const int &from, const int &to, const int &weight = 1) {
    if (viable_edge(from, to) && !_hasEdge(from, to)) {
        adjMatrix[from][to] = weight;
        return true;
    }
    return false;
}

inline bool GraphMatrix::remove_edge(const int &from, const int &to) {
    if (has_edge(from, to)) {
        adjMatrix[from][to] = INT_MAX;
        return true;
    }
    return false;
}

// ------ VERTEX ------ //

inline bool GraphMatrix::has_vertex(const int &vertex) {
    return vertex >= 0 && vertex < numVertices;
}

inline void GraphMatrix::add_vertex(const int &newVertexCount) {
        numVertices += newVertexCount;
        for (auto& row : adjMatrix) {
            row.resize(numVertices, INT_MAX);
        }
        for (int i = 0; i < newVertexCount; ++i) {
            adjMatrix.push_back(vector<int>(numVertices, INT_MAX));
        }
}

inline bool GraphMatrix::remove_vertex(const int &vertex) {
    if (vertex < 0 || vertex >= numVertices) {
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

// ------ OTHER FUNCTIONS ------ //

inline vector<int> GraphMatrix::findCycles(){
    vector<bool> visitado(numVertices, false);
    vector<bool> pilhaRecursiva(numVertices, false);
    vector<int> parent(numVertices, -1);
    stack<int> ciclo;

    for (int i = 0; i < numVertices; ++i) {
        if (!visitado[i]) {
            if (dfs(i, visitado, pilhaRecursiva, parent, ciclo)) {
                vector<int> cicloVec;
                while (!ciclo.empty()) {
                    cicloVec.push_back(ciclo.top());
                    ciclo.pop();
                }
                return cicloVec;
            }
        }
    }
    return {};
}


inline void GraphMatrix::contract(const vector<int>& ciclo){
    
    if (ciclo.empty())
    {
        return;
    }
    
    // Criando super-vértice
    int superVertice = numVertices++;
    adjMatrix.resize(numVertices, vector<int>(numVertices, INT_MAX));
    for (auto& row : adjMatrix) {
        row.resize(numVertices, INT_MAX);
    }

    // Redirecionando arestas de/para o ciclo para o super-vértice
    for (int i = 0; i < numVertices; ++i) {
        for (int v : ciclo) {
            if (i != superVertice && adjMatrix[i][v] != INT_MAX) {
                adjMatrix[i][superVertice] = adjMatrix[i][v];
                adjMatrix[i][v] = INT_MAX;
            }
            if (i != superVertice && adjMatrix[v][i] != INT_MAX) {
                adjMatrix[superVertice][i] = adjMatrix[v][i];
                adjMatrix[v][i] = INT_MAX;
            }
        }
    }

    // Removendo as arestas internas do ciclo
    for (int i = (int)ciclo.size()-2; i >= 0 ; --i) {
        remove_vertex(ciclo.at(i));
    }
    cout << endl;
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
            if (adjMatrix[i][j] == INT_MAX)
                cout << "0 ";
            else
                cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}