#include <cstdlib>
#include <sstream>
#include <numeric>
#include <algorithm>

#include "../include/graph.hpp"

using namespace std;

// ===================================== Edge ===================================== //

Edge::Edge(size_t src, size_t dest, size_t weight = 1) {
    this->src = src;
    this->dest = dest;
    this->weight = weight;
}

Edge::Edge(const Edge& other) {
    this->src = other.src;
    this->dest = other.dest;
    this->weight = other.weight;
}

size_t Edge::get_src() const {
    return this->src;
}

size_t Edge::get_dest() const {
    return this->dest;
}

size_t Edge::get_weight() const {
    return this->weight;
}

// ===================================== Graph ===================================== //

// === Parsing and Representation === //

Graph::Graph(vector<string> lines) {
    istringstream iss(lines[0]);
    size_t vertices, edges, root;
    size_t src, dest;
    size_t weight;

    if(!(iss >> vertices >> edges >> root)) {
        cerr << "Could not parse graph" << endl;
        exit(EXIT_FAILURE);
    }

    this->root = root;
    
    this->vertices = vector<Vertex>(vertices);
    iota(this->vertices.begin(), this->vertices.end(), 0);

    this->edges.reserve(edges);
    
    for(size_t i = 1; i < lines.size(); i++) {
        iss = istringstream(lines[i]);
        
        if(!(iss >> src >> dest >> weight)) {
            cerr << "Could not parse edge" << endl;
            exit(EXIT_FAILURE);
        }
        
        this->edges.push_back(Edge(src, dest, weight));
    }
}

Graph::Graph(const Graph& other) {
    this->vertices = other.vertices;
    this->edges = other.edges;
    this->root = other.root;
}


Graph::Graph(const std::vector<Vertex> vertices, const std::vector<Edge> edges, const size_t root) {
    this->vertices = vertices;
    this->edges = edges;
    this->root = root;
}

string Graph::str() const {
    vector<string> lines(this->vertices.size());

    for(size_t i = 0; i < this->vertices.size(); i++) {
        stringstream ss;
        ss << i << ": ";
        lines[i] = ss.str();
    }
    
    for(const auto& e : this->edges) {
        stringstream ss;
        ss << "(" << e.dest << " " << e.weight << ") ";
        lines[e.src] += ss.str();
    }

    for(auto& l : lines)
        l += "\n";

    return accumulate(lines.begin(), lines.end(), string(""));
}

// === Auxiliary Functions === //

Edge Graph::min_weight_edge(vector<Edge> edges) {
    if(edges.empty()) {
        cerr << "Missing an edge";
        exit(EXIT_FAILURE);
    }
    
    size_t index = 0;
    size_t min_weight = edges[0].weight;

    for(size_t i = 1; i < edges.size(); i++) {
        if(edges[i].weight < min_weight) {
            min_weight = edges[i].weight;
            index = i;
        }
    }

    return Edge(edges[index]);
}

Path Graph::back_traverse_path(Path p, Vertex v) {
    Path traversed_path;
    
    if(!p.empty()) {
        if(p.front().src == v) {
            traversed_path = p;
        } else {
            Edge e(p.front());
            Path p_tail = vector<Edge>(p.begin()+1, p.end());
            traversed_path = Graph::back_traverse_path(p_tail, v);
            traversed_path.insert(traversed_path.begin(), e);
        }
    }

    return traversed_path;
}

pair<Path, Vertex> Graph::traverse_path(Path p1, Path p2, set<Vertex> visited) {
    if(p1.empty()) return { {}, -1 };

    Edge e(p1.front());

    if(visited.count(e.src)) {
        p2.insert(p2.begin(), e);
        return { p2, e.src };
    }

    Path p1_tail = vector<Edge>(p1.begin()+1, p1.end());
    p2.insert(p2.begin(), e);
    visited.insert(e.src);
    
    return Graph::traverse_path(p1_tail, p2, visited);
}

Path Graph::get_cycle(Path p) {
    set<Vertex> s;
    s.insert(p.front().dest);
    pair<Path, Vertex> pv(Graph::traverse_path(p, {}, s));
    return Graph::back_traverse_path(pv.first, pv.second);
}

vector<Path> Graph::dfs(const vector<Edge> es, const Path base, Vertex v) {
    set<size_t> srcs, dests;

    for(const Edge& e : base) {
        srcs.insert(e.src);
        dests.insert(e.dest);
    }

    set<size_t> reached_vertices;
    set_union(
        srcs.begin(), 
        srcs.end(), 
        dests.begin(), 
        dests.end(), 
        inserter(reached_vertices, reached_vertices.begin())
    );

    Vertex start = base.empty() ? v : base.front().dest;

    vector<Edge> new_edges;
    copy_if(es.begin(), es.end(), back_inserter(new_edges), [start, &base](const Edge& e) {
        return e.src == start && find_if(base.begin(), base.end(), [&e](const Edge& base_edge) {
            return base_edge.src == e.src && base_edge.dest == e.dest && base_edge.weight == e.weight;
        }) == base.end();
    });

    auto revisit = [&reached_vertices](const Edge& e) {
        return reached_vertices.count(e.dest);
    };

    vector<Edge> dead_edges, next_edges;
    partition_copy(
        new_edges.begin(), 
        new_edges.end(), 
        back_inserter(dead_edges), 
        back_inserter(next_edges), 
        revisit
    );

    vector<Path> result;

    if(new_edges.empty()) {
        result.push_back(base);
    } else {
        for(const Edge& e : dead_edges) {
            vector<Edge> tmp = base;
            tmp.insert(tmp.begin(), e);
            result.push_back(tmp);
        }

        for(const Edge& e : next_edges) {
            vector<Edge> tmp = base;
            tmp.insert(tmp.begin(), e);

            vector<Path> paths = Graph::dfs(es, tmp, e.dest);
            result.insert(result.end(), paths.begin(), paths.end());
        }
    }

    return result;
}

// === Edmonds Helpers === //

vector<Edge> Graph::remove_multi_edges(vector<Edge> gedges) {
    vector<Edge> result;

    if(!gedges.empty()) {
        Edge e(gedges.front());
        vector<Edge> alike, different;

        partition_copy(
            gedges.begin(),
            gedges.end(),
            back_inserter(alike),
            back_inserter(different),
            [&e](const Edge& edge) {
                return edge.src == e.src && edge.dest == e.dest;
            }
        );

        Edge min_edge(Graph::min_weight_edge(alike));
        result = Graph::remove_multi_edges(different);
        result.insert(result.begin(), min_edge);
    }

    return result;
}

vector<Edge> Graph::cheapest_edges(vector<Edge> gedges) {
    vector<Edge> result;

    if(!gedges.empty()) {
        Edge e(gedges.front());
        vector<Edge> alike, different;

        partition_copy(
            gedges.begin(),
            gedges.end(),
            back_inserter(alike),
            back_inserter(different),
            [&e](const Edge& edge) {
                return edge.dest == e.dest;
            }
        );
        
        Edge cheapest_edge = Graph::min_weight_edge(alike);
        result = Graph::cheapest_edges(different);
        result.insert(result.begin(), cheapest_edge);
    }

    return result;
}

// === Edmonds Branching Algorithm === //
 
Edge Graph::edmonds_edge(Edge edge, vector<Edge> es, set<Vertex> cycle_vertices, Vertex contract_vertex) {
    Vertex u = edge.src;
    Vertex v = edge.dest;
    size_t w = edge.weight;

    bool u_in_c = cycle_vertices.count(u);
    bool v_in_c = cycle_vertices.count(v);
    size_t cheap_w = SIZE_MAX;
    
    for(size_t i = 0; i < es.size(); i++) {
        if(es[i].dest == v && es[i].weight < cheap_w) {
            cheap_w = es[i].weight;
        }
    }

    if(!u_in_c && v_in_c) {
        return Edge(u, contract_vertex, w - cheap_w);
    }

    if(u_in_c && !v_in_c) {
        return Edge(contract_vertex, v, w);
    }

    return edge;
}

Graph Graph::edmonds_cycle(Graph g, Path cy) {
    Vertex new_vertex = 0;

    for(const Vertex& v : g.vertices) {
        if(v > new_vertex) {
            new_vertex = v;
        }
    }
    new_vertex++;
    
    set<Vertex> cycle_vertices;
    for(const Edge& e : cy) {
        cycle_vertices.insert(e.dest);
    }
    cycle_vertices.insert(cy.front().src);

    auto in_cycle = [&cycle_vertices](const Edge& e) {
        return cycle_vertices.count(e.src) && cycle_vertices.count(e.dest);
    };

    vector<Edge> cycle_edges, other_edges;
    partition_copy(
        g.edges.begin(),
        g.edges.end(),
        back_inserter(cycle_edges),
        back_inserter(other_edges),
        in_cycle
    );

    auto map_edge = [&](const Edge& e) {
        return Graph::edmonds_edge(e, g.edges, cycle_vertices, new_vertex);
    };

    vector<pair<Edge, Edge>> associated_edges;
    for(const auto& e : other_edges) {
        associated_edges.push_back(pair<Edge, Edge>(e, map_edge(e)));
    }

    vector<Vertex> new_g_vertices = g.vertices;
    new_g_vertices.insert(new_g_vertices.begin(), new_vertex);

    vector<Edge> mapped_edges;
    for(const pair<Edge, Edge>& ae : associated_edges) {
        mapped_edges.push_back(ae.second);
    }
    
    Graph recurse = Graph::edmonds(Graph(new_g_vertices, mapped_edges, g.root));

    auto unmap_edge = [&associated_edges](const Edge& e) {
        vector<pair<Edge, Edge>> es;
        
        copy_if(associated_edges.begin(), associated_edges.end(), back_inserter(es), [&](const pair<Edge, Edge>& ae) {
            return ae.second.src == e.src && ae.second.dest == e.dest && ae.second.weight == e.weight;
        });

        return es.front().first;
    };

    Edge uvc = *find_if(recurse.edges.begin(), recurse.edges.end(), [&new_vertex](const Edge& e) {
        return e.dest == new_vertex;
    });

    Edge uv = unmap_edge(uvc);

    Edge in_cycle_uv = *find_if(cy.begin(), cy.end(), [&uv](const Edge& e) {
        return e.dest == uv.dest;
    });

    vector<Edge> new_edges;
    
    copy_if(cycle_edges.begin(), cycle_edges.end(), back_inserter(new_edges), [&in_cycle_uv](const Edge& e) {
        return e.src != in_cycle_uv.src || e.dest != in_cycle_uv.dest || e.weight != in_cycle_uv.weight;
    });

    for(const Edge& e : recurse.edges) {
        new_edges.push_back(unmap_edge(e));
    }

    return Graph(g.vertices, new_edges, g.root);
}

Graph Graph::edmonds(Graph g) {
    vector<Edge> not_to_root;

    copy_if(g.edges.begin(), g.edges.end(), back_inserter(not_to_root), [&](const Edge& e) {
        return e.dest != g.root;
    });

    vector<Edge> reduced_edges = Graph::remove_multi_edges(not_to_root);
    vector<Edge> cheap_edges = Graph::cheapest_edges(reduced_edges);
    vector<Path> paths;

    vector<Path> dfss;
    for(const Vertex& v : g.vertices) {
        vector<Path> tmp = Graph::dfs(cheap_edges, {}, v);
        dfss.insert(dfss.end(), tmp.begin(), tmp.end());
    }

    for(const Path& ps : dfss) {
        if(!ps.empty()) {
            paths.push_back(ps);
        }
    }

    vector<Path> cycles;
    for(const Path& p : paths) {
        Path cy = Graph::get_cycle(p);
        if(!cy.empty()) {
            cycles.push_back(cy);
        }
    }

    if(cycles.empty()) {
        return Graph(g.vertices, cheap_edges, g.root);
    }

    return Graph::edmonds_cycle(g, cycles.front());
}
