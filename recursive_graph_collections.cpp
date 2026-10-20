#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include "recursive_graph_collections.h"

// Set LEVEL_MAX to default value if not set using -DLEVEL_MAX.
#ifndef LEVEL_MAX
#define LEVEL_MAX 4
#endif

using std::ignore;  // For ignoring unused function params (temporary)
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;

using Vertex = int;
using Edge = pair<Vertex, Vertex>;
using Graph = pair<set<Vertex>, set<Edge>>;
using GraphName = string;
using Level = size_t;
using Collection = map<GraphName, set<Graph>>;
using CollectionId = long;
using CollectionMap = unordered_map<CollectionId, Collection>;

#ifdef __cplusplus
namespace cxx {
#endif

CollectionId rgc_new_collection() {
    // TODO
    return -1;
}

void rgc_delete_collection(CollectionId id) {
    ignore = id;
    // TODO
}

bool rgc_new_graph(CollectionId id, const char *name, int n) {
    ignore = id;
    ignore = name;
    ignore = n;
    // TODO
    return false;
}

void rgc_delete_graph(CollectionId id, const char *name) {
    ignore = id;
    ignore = name;
    // TODO
}

Vertex rgc_graph_new_node(CollectionId id, const char *name) {
    ignore = id;
    ignore = name;
    // TODO
    return -1;
}

bool rgc_graph_delete_node(CollectionId id, const char *name, Vertex x) {
    ignore = id;
    ignore = name;
    ignore = x;
    // TODO
    return false;
}

bool rgc_graph_add_edge(CollectionId id, const char *name, Vertex x, Vertex y) {
    ignore = id;
    ignore = name;
    ignore = x;
    ignore = y;
    // TODO
    return false;
}

bool rgc_graph_delete_edge(CollectionId id, const char *name, Vertex x, Vertex y) {
    ignore = id;
    ignore = name;
    ignore = x;
    ignore = y;
    // TODO
    return false;
}

const char *rgc_first_graph(CollectionId id) {
    ignore = id;
    // TODO
    return "";
}

const char *rgc_next_graph(CollectionId id, const char *name) {
    ignore = id;
    ignore = name;
    // TODO
    return "";
}

Vertex rgc_first_node(CollectionId id, const char *name) {
    ignore = id;
    ignore = name;
    // TODO
    return -1;
}

Vertex rgc_next_node(CollectionId id, const char *name, Vertex x) {
    ignore = id;
    ignore = name;
    ignore = x;
    // TODO
    return -1;
}

Vertex rgc_first_edge(CollectionId id, const char *name, Vertex x) {
    ignore = id;
    ignore = name;
    ignore = x;
    // TODO
    return -1;
}

Vertex rgc_next_edge(CollectionId id, const char *name, Vertex x, Vertex y) {
    ignore = id;
    ignore = name;
    ignore = x;
    ignore = y;
    // TODO
    return -1;
}

bool rgc_replace_edge(CollectionId id,
                      const char *result_name,
                      const char *recipient_name,
                      Vertex w_1,
                      Vertex w_2,
                      const char *donor_name,
                      Vertex v_1,
                      Vertex v_2) {
    ignore = id;
    ignore = result_name;
    ignore = recipient_name;
    ignore = w_1;
    ignore = w_2;
    ignore = donor_name;
    ignore = v_1;
    ignore = v_2;
    // TODO
    return false;
}

ssize_t rgc_edge_level(CollectionId id, const char *name, Vertex x, Vertex y) {
    ignore = id;
    ignore = name;
    ignore = x;
    ignore = y;
    // TODO
    return -1;
}

ssize_t rgc_graph_level(CollectionId id, const char *name) {
    ignore = id;
    ignore = name;
    // TODO
    return -1;
}

ssize_t rgc_max_level() {
    // TODO
    return -1;
}

ssize_t rgc_count_nodes(CollectionId id, const char *name) {
    ignore = id;
    ignore = name;
    // TODO
    return -1;
}

ssize_t rgc_count_edges(CollectionId id, const char *name) {
    ignore = id;
    ignore = name;
    // TODO
    return -1;
}

ssize_t rgc_size(CollectionId id) {
    ignore = id;
    // TODO
    return -1;
}


#ifdef __cplusplus
} // namespace
#endif