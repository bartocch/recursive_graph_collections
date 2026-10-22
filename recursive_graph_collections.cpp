#include <limits.h>
#include <map>
#include <optional>
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
using std::nullopt;
using std::pair;
using std::optional;
using std::reference_wrapper;
using std::set;
using std::string;
using std::unordered_map;

template<typename T>
using OptionalReference = optional<reference_wrapper<T>>;

using Vertex = int;
using Edge = pair<Vertex, Vertex>;
using Graph = pair<set<Vertex>, set<Edge>>;
using GraphName = string;
using Level = size_t;
using Collection = map<GraphName, Graph>;
using CollectionId = long;
using CollectionMap = unordered_map<CollectionId, Collection>;

#ifdef __cplusplus
namespace cxx {
#endif

// Namespace for helper functions and global variables.
namespace {

long lastCollectionId = -1;
CollectionMap collectionMap;

bool verifyName(string name) {
    // TODO
}

OptionalReference<Collection> findCollection(CollectionId id) {
    CollectionMap::iterator it = collectionMap.find(id);
    if (it == collectionMap.end()) {
        return nullopt;
    }
    return it->second;
}

OptionalReference<Graph> findGraph(CollectionId id, GraphName name) {
    OptionalReference<Collection> collection = findCollection(id);
    if (!collection) {
        return nullopt;
    }

    Collection::iterator it = collection->get().find(name);
    if (it == collection->get().end()) {
        return nullopt;
    }
    return it->second;
}

Vertex smallestFreeNodeNumber(Graph& graph) {
    auto& [nodes, edges] = graph;

    // TODO: Temporary solution
    for (int nodeNumber = 0; nodeNumber <= INT_MAX; nodeNumber++) {
        if (nodes.find(nodeNumber) != nodes.end()) {
            return nodeNumber;
        }
    }

    return -1;
}

} // namespace

CollectionId rgc_new_collection() {
    if (lastCollectionId == LONG_MAX) {
        return -1;      // Return -1 if no more ids left.
    }

    int collectionId = ++lastCollectionId;
    collectionMap.insert({ collectionId, Collection() });

    return collectionId;
}

void rgc_delete_collection(CollectionId id) {
    collectionMap.erase(id);
}

bool rgc_new_graph(CollectionId id, const char *name, int n) {
    if (name == nullptr || !verifyName(name) || n < 0) {
        return false;
    }

    OptionalReference<Collection> collection = findCollection(id);
    if (!collection) {
        return false;
    }

    Graph graph;
    auto& [vertices, edges] = graph;

    // Insert n nodes labeled from 0 to n - 1.
    for (int vertexNo = 0; vertexNo < n; vertexNo++) {
        vertices.insert(vertexNo);
    }

    collection->get().insert({ name, graph });

    return true;
}

void rgc_delete_graph(CollectionId id, const char *name) {
    if (name == nullptr) {
        return;
    }

    OptionalReference<Collection> collection = findCollection(id);
    if (!collection) {
        return;
    }

    collection->get().erase(name);
}

Vertex rgc_graph_new_node(CollectionId id, const char *name) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return -1;
    }

    auto& [nodes, edges] = graph->get();
    if (nodes.size() == INT_MAX) {
        return -1;
    }

    Vertex nodeNumber = smallestFreeNodeNumber(graph->get());
    nodes.insert(nodeNumber);

    return nodeNumber;
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