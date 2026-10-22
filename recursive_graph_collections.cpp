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

using Node = int;
using Edge = set<Node, Node>;
using Graph = pair<set<Node>, set<Edge>>;
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

bool verifyGraphNameChar(char ch) {
    return (ch >= 'a' && ch <= 'z')     // lowercase letters
        || (ch >= 'A' && ch <= 'Z')     // uppercase letters
        || (ch >= '0' && ch <= '9')     // digits
        || ch == '_';                   // underscore
}

bool verifyGraphName(string name) {
    if (name.empty()) {
        return false;   // Graph names cannot be empty.
    }

    for (char ch : name) {
        if (!verifyGraphNameChar(ch)) {
            return false;
        }
    }
    return true;
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

Node smallestFreeNodeNumber(Graph& graph) {
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
    if (name == nullptr || !verifyGraphName(name) || n < 0) {
        return false;
    }

    OptionalReference<Collection> collection = findCollection(id);
    if (!collection) {
        return false;
    }

    Graph graph;
    auto& [nodes, edges] = graph;

    // Insert n nodes labeled from 0 to n - 1.
    for (int nodeNumber = 0; nodeNumber < n; nodeNumber++) {
        nodes.insert(nodeNumber);
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

Node rgc_graph_new_node(CollectionId id, const char *name) {
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

    Node nodeNumber = smallestFreeNodeNumber(graph->get());
    nodes.insert(nodeNumber);

    return nodeNumber;
}

bool rgc_graph_delete_node(CollectionId id, const char *name, Node x) {
    if (name == nullptr) {
        return false;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return false;
    }

    auto& [nodes, edges] = graph->get();

    set<Node>::iterator it = nodes.find(x);
    if (it == nodes.end()) {
        return false;   // The node that we're trying to delete doesn't exist.
    }

    nodes.erase(it);
    return true;
}

bool rgc_graph_add_edge(CollectionId id, const char *name, Node x, Node y) {
    if (name == nullptr || x == y) {
        return false;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return false;
    }

    auto& [nodes, edges] = graph->get();

    // Try to insert the edge while simultaneously checking if it exists.
    // (wasInserted = true if the edge didn't exist before the insertion)
    auto [it, wasInserted] = edges.insert({ x, y });
    return wasInserted;
}

bool rgc_graph_delete_edge(CollectionId id, const char *name, Node x, Node y) {
    if (name == nullptr || x == y) {
        return false;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return false;
    }

    auto& [nodes, edges] = graph->get();

    set<Edge>::iterator it = edges.find({ x, y });
    if (it == edges.end()) {
        return false;   // The edge we're trying to delete doesn't exist.
    }

    edges.erase(it);
    return true;
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

Node rgc_first_node(CollectionId id, const char *name) {
    ignore = id;
    ignore = name;
    // TODO
    return -1;
}

Node rgc_next_node(CollectionId id, const char *name, Node x) {
    ignore = id;
    ignore = name;
    ignore = x;
    // TODO
    return -1;
}

Node rgc_first_edge(CollectionId id, const char *name, Node x) {
    ignore = id;
    ignore = name;
    ignore = x;
    // TODO
    return -1;
}

Node rgc_next_edge(CollectionId id, const char *name, Node x, Node y) {
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
                      Node w_1,
                      Node w_2,
                      const char *donor_name,
                      Node v_1,
                      Node v_2) {
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

ssize_t rgc_edge_level(CollectionId id, const char *name, Node x, Node y) {
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