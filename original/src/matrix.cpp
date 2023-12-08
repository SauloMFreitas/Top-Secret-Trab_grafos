#include "../include/matrix.hpp"
#include <algorithm>
#include <iomanip>
#include <set>
#include <iostream>
#include <vector>

using namespace Graphy;
using namespace std;

Matrix::Matrix() : Graph() {}

Matrix::Matrix(const uint &_size, const bool &_directed) : Graph(_size, _directed, false) {
    matrix = vector<vector<uint>>(_size, vector<uint>(_size, 0));

    for (uint i = 0; i < size; ++i) {
        string key(1, static_cast<char>('A' + i));
        nameMap[key] = i;
    }
}

Matrix::Matrix(const uint &_size, const bool &_directed, const vector<string> &names) : Graph(_size, _directed, false) {
    if (names.size() != _size) {
        throw invalid_argument("Number of names must match the size of the graph");
    }

    matrix = vector<vector<uint>>(size, vector<uint>(size, 0));
    for (uint i = 0; i < size; ++i) {
        nameMap[names[i]] = i;
    }
}

// Edge

inline bool Matrix::viableEdge(const string &from, const string &to) const {
    return hasVertex(from) && hasVertex(to);
}

inline bool Matrix::hasEdge(const string &from, const string &to) {
    return viableEdge(from, to) ? matrix[nameMap[from]][nameMap[to]] : false;
}

inline bool Matrix::_hasEdge(const string &from, const string &to) {
    return matrix[nameMap[from]][nameMap[to]];
}

inline bool Matrix::addEdge(const string &from, const string &to, const uint &weight = 1) {
    if (viableEdge(from, to) && !_hasEdge(from, to)) {
        matrix[nameMap[from]][nameMap[to]] = weight;
        if (!directed) {
            matrix[nameMap[to]][nameMap[from]] = weight;
        }
        return true;
    }
    return false;
}

inline bool Matrix::removeEdge(const string &from, const string &to) {
    if (hasEdge(from, to)) {
        matrix[nameMap[from]][nameMap[to]] = 0;
        if (!directed) {
            matrix[nameMap[to]][nameMap[from]] = 0;
        }
        return true;
    }
    return false;
}

// TODO
void Matrix::removeEdgesTo(const string &to) { return; }

// TODO
void Matrix::removeEdgesFrom(const string &from) { return; }

// Vertex

inline bool Matrix::hasVertex(const string &vertexName) const {
    return nameMap.find(vertexName) != nameMap.end();
}

inline void Matrix::addVertex(const uint &newVertexCount) {
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

inline bool Matrix::addVertex(const vector<string> &vertexNames) {
    int newVertexCount = vertexNames.size();

    for (auto &name : vertexNames) {
        if (hasVertex(name)) {
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

inline bool Matrix::removeVertex(const string &vertexName) {
    if (hasVertex(vertexName)) {
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

bool Matrix::removeVertex(const vector<string> &vertexNames) {
    bool b = true;
    for(auto v: vertexNames) b &= removeVertex(v);
    return b;
}

vector<string> Matrix::vertexNames() const {
    vector<string> names;
    for(const auto &pair : nameMap) names.push_back(pair.first);
    return names;
}

// TODO
bool Matrix::editEdge(const std::string &from, const std::string &to, const uint &weight) {
    return false;
}

// TODO
Graph* Matrix::clone() const {
    return nullptr;
}

// TODO
Graph* Matrix::reverse() {
    return nullptr;
}

// TODO
Graph* Matrix::minIncomingEdgeGraph() {
    return nullptr;
}

// TODO
Matrix* Matrix::transitiveClosure(const string &start) {
    return nullptr;
}

inline void Matrix::print() const {
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

// Arborescence
Graph *Matrix::arborescence(const string &vertexName) {
    return NULL;
}

/*
 * =============================================================================
 * =                                PRIVATE                                    =
 * =============================================================================
 */

void Matrix::dfs(const string &vertex, set<string> &visited, int &count) {
    uint index = nameMap[vertex];
    visited.insert(vertex);
    count++;

    for(const auto &edge : this->matrix[index]) {

    }
}
