#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include <vector>
#include <string>
#include <set>
#include <utility>
#include <iostream>
#include <cstdlib>

class Edge {
    std::size_t src;
    std::size_t dest;
    std::size_t weight;

public:
    Edge(std::size_t src, std::size_t dest, std::size_t weight);
    Edge(const Edge& other);

    std::size_t get_src() const;
    std::size_t get_dest() const;
    std::size_t get_weight() const;

    friend class Graph;
};

using Vertex = std::size_t;
using Path = std::vector<Edge>;

class Graph {
    std::vector<std::size_t> vertices;
    std::vector<Edge> edges;
    std::size_t root;

    static Edge min_weight_edge(std::vector<Edge> edges);
    static Path back_traverse_path(Path p, Vertex v);
    static std::pair<Path, Vertex> traverse_path(Path p1, Path p2, std::set<Vertex> visited);
    static Path get_cycle(Path p);
    static std::vector<Path> dfs(const std::vector<Edge> es, const Path base, Vertex v);

    static std::vector<Edge> remove_multi_edges(std::vector<Edge> gedges);
    static std::vector<Edge> cheapest_edges(std::vector<Edge> gedges);

    static Edge edmonds_edge(Edge edge, std::vector<Edge> es, std::set<Vertex> cycle_vertices, Vertex contract_vertex);
    static Graph edmonds_cycle(Graph g, Path cy);
    
public:
    explicit Graph(std::vector<std::string> lines);
    Graph(const Graph& other);
    Graph(const std::vector<Vertex> vertices, const std::vector<Edge> edges, const size_t root);

    std::string str() const;

    static Graph edmonds(Graph g);
};

#endif
