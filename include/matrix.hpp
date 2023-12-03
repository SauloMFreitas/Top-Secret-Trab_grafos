#ifndef GRAPHY_MATRIX_H
#define GRAPHY_MATRIX_H

#include "graph.hpp"
#include <map>
#include <set>

namespace Graphy {
    class Matrix : public Graph {
      private:
        std::map<std::string, uint> nameMap;
        std::vector<std::vector<uint>> matrix;

        bool _hasEdge(const std::string &from, const std::string &to);
        void dfs(const std::string &vertex, std::set<std::string> &visited, int &count);

      public:
        Matrix();
        Matrix(const uint &_size, const bool &_directed);
        Matrix(const uint &_size, const bool &_directed, const std::vector<std::string> &names);

        // Edge
        bool viableEdge(const std::string &from, const std::string &to) const override;
        bool hasEdge(const std::string &from, const std::string &to) override;
        bool addEdge(const std::string &from, const std::string &to, const uint &weight) override;
        bool removeEdge(const std::string &from, const std::string &to) override;
        void removeEdgesTo(const std::string &to) override;
        void removeEdgesFrom(const std::string &from) override;

        // Vertex
        bool hasVertex(const std::string &vertexName) const override;
        void addVertex(const uint &newVertexCount) override;
        bool addVertex(const std::vector<std::string> &vertexNames) override;
        bool removeVertex(const std::string &vertexName) override;
        bool removeVertex(const std::vector<std::string> &vertexNames) override;

        bool editEdge(const std::string &from, const std::string &to, const uint &weight) override;
        Graph *clone() const override;
        Graph *reverse() override;
        Graph *minIncomingEdgeGraph() override;

        // Data
        std::vector<std::string> vertexNames() const override;

        // Debug
        void print() const override;

        // Search
        virtual Matrix *transitiveClosure(const std::string &start) override;

        // Minimum spanning tree
        Graph *arborescence(const std::string &vertexName) override;
    };
}

#endif // GRAPHY_MATRIX_H
