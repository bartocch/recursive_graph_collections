#include <climits>
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

using std::map;
using std::max;
using std::move;
using std::nullopt;
using std::optional;
using std::pair;
using std::reference_wrapper;
using std::set;
using std::string;
using std::unordered_map;

template<typename T>
using OptionalReference = optional<reference_wrapper<T>>;

using Level = size_t;
using Node = int;
using NodeSet = set<Node>;
using Edge = pair<Node, Node>;
/** An edge { x, y } is represented by two pairs. The first pair is (x, y)
 *  and the second pair is (y, x). This makes it easier to implement
 *  rgc_first_edge and rgc_next_edge. */
using EdgeMap = map<Edge, Level>;
using Graph = pair<NodeSet, EdgeMap>;
using GraphName = string;
using Collection = map<GraphName, Graph>;
using CollectionId = long;
using CollectionMap = unordered_map<CollectionId, Collection>;

#ifdef __cplusplus
namespace cxx {
#endif

// Namespace for helper functions and global variables.
namespace {

constinit long lastCollectionId = -1;

CollectionMap& collectionMap() {
    // Prevents the static initialization order fiasco.
    static CollectionMap collectionMap;
    return collectionMap;
}

constexpr NodeSet& getNodes(Graph& graph) {
    return get<0>(graph);
}

constexpr EdgeMap& getEdges(Graph& graph) {
    return get<1>(graph);
}

bool verifyGraphNameChar(char ch) {
    return (ch >= 'a' && ch <= 'z')     // lowercase letters
        || (ch >= 'A' && ch <= 'Z')     // uppercase letters
        || (ch >= '0' && ch <= '9')     // digits
        || ch == '_';                   // underscore
}

bool verifyGraphName(const GraphName& name) {
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

/** Finds a collection with the provided id and returns it.
 *  Returns nullopt if a matching collection wasn't found. */
OptionalReference<Collection> findCollection(CollectionId id) {
    CollectionMap::iterator it = collectionMap().find(id);
    if (it == collectionMap().end()) {
        return nullopt;
    }
    return it->second;
}

/** Finds a graph with the provided name within a collection with the
 *  provided id and returns that graph.
 *  Returns nullopt if a matching collection/graph wasn't found. */
OptionalReference<Graph> findGraph(CollectionId id, const GraphName& name) {
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
    NodeSet& nodes = getNodes(graph);
    Node resultNode = 0;

    for (Node node : nodes) {
        if (node == resultNode) {
            resultNode++;
        } else if (node > resultNode) {
            break;  // Found a gap in the node numbers.
        }
    }

    return resultNode;
}

} // namespace

CollectionId rgc_new_collection() {
    if (lastCollectionId == LONG_MAX) {
        return -1;      // Return -1 if no more ids left.
    }

    CollectionId collectionId = ++lastCollectionId;
    collectionMap().insert({ collectionId, Collection() });

    return collectionId;
}

void rgc_delete_collection(CollectionId id) {
    collectionMap().erase(id);
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
    NodeSet& nodes = getNodes(graph);

    // Insert n nodes labeled from 0 to n - 1.
    for (int nodeNumber = 0; nodeNumber < n; nodeNumber++) {
        nodes.insert(nodeNumber);
    }

    // Try to insert the graph while simultaneously checking if another one
    // with the same name exists.
    auto [it, wasInserted] = collection->get().insert({ name, graph });
    return wasInserted;
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

    NodeSet& nodes = getNodes(graph->get());
    if (nodes.size() == INT_MAX) {
        return -1;      // We've run out of node numbers.
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

    NodeSet& nodes = getNodes(graph->get());
    NodeSet::iterator it = nodes.find(x);
    if (it == nodes.end()) {
        return false;   // The node that we're trying to delete doesn't exist.
    }

    EdgeMap& edges = getEdges(graph->get());
    // Remove all edges connected to the node x.
    for (EdgeMap::iterator edgeIt = edges.begin(); edgeIt != edges.end(); ) {
        if (edgeIt->first.first == x || edgeIt->first.second == x) {
            edgeIt = edges.erase(edgeIt);
        } else {
            ++edgeIt;
        }
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

    NodeSet& nodes = getNodes(graph->get());
    EdgeMap& edges = getEdges(graph->get());

    if (nodes.find(x) == nodes.end() || nodes.find(y) == nodes.end()) {
        return false;   // One or more of the nodes doesn't exist.
    }
    if (edges.find({ x, y }) != edges.end()) {
        return false;   // Edge already exists.
    }

    edges.insert({ { x, y }, 0 });
    edges.insert({ { y, x }, 0 });
    return true;
}

bool rgc_graph_delete_edge(CollectionId id, const char *name, Node x, Node y) {
    if (name == nullptr || x == y) {
        return false;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return false;
    }

    EdgeMap& edges = getEdges(graph->get());
    EdgeMap::iterator it = edges.find({ x, y });
    if (it == edges.end()) {
        return false;   // The edge we're trying to delete doesn't exist.
    }

    edges.erase(it);
    edges.erase({ y, x });
    return true;
}

const char *rgc_first_graph(CollectionId id) {
    OptionalReference<Collection> collection = findCollection(id);
    if (!collection || collection->get().empty()) {
        return nullptr;
    }

    return collection->get().begin()->first.c_str();
}

const char *rgc_next_graph(CollectionId id, const char *name) {
    if (name == nullptr) {
        return nullptr;
    }

    OptionalReference<Collection> collection = findCollection(id);
    if (!collection) {
        return nullptr;
    }

    Collection::iterator it = collection->get().find(name);
    if (it == collection->get().end()) {
        return nullptr; // The graph with this name doesn't exist.
    }

    ++it;
    if (it == collection->get().end()) {
        return nullptr; // The graph with this name was the last one.
    }

    return it->first.c_str();
}

Node rgc_first_node(CollectionId id, const char *name) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph || getNodes(graph->get()).empty()) {
        return -1;
    }

    return *getNodes(graph->get()).begin();
}

Node rgc_next_node(CollectionId id, const char *name, Node x) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return -1;
    }

    NodeSet& nodes = getNodes(graph->get());
    NodeSet::iterator it = nodes.upper_bound(x);
    if (it == nodes.end()) {
        return -1;   // The node x was the last one.
    }

    return *it;
}

Node rgc_first_edge(CollectionId id, const char *name, Node x) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return -1;
    }

    NodeSet& nodes = getNodes(graph->get());
    EdgeMap& edges = getEdges(graph->get());

    if (nodes.find(x) == nodes.end()) {
        return -1;   // The node x doesn't exist.
    }

    EdgeMap::iterator it = edges.lower_bound({ x, 0 });
    if (it == edges.end() || it->first.first != x) {
        return -1;   // The node x has no edges.
    }

    return it->first.second;
}

Node rgc_next_edge(CollectionId id, const char *name, Node x, Node y) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return -1;
    }

    NodeSet& nodes = getNodes(graph->get());
    EdgeMap& edges = getEdges(graph->get());

    if (nodes.find(x) == nodes.end()) {
        return -1;   // The node x doesn't exist.
    }

    EdgeMap::iterator it = edges.upper_bound({ x, y });
    if (it == edges.end() || it->first.first != x) {
        return -1;   // No edges exist to nodes greater than y for node x.
    }

    return it->first.second;
}

bool rgc_replace_edge(CollectionId id,
                      const char *result_name,
                      const char *recipient_name,
                      Node w_1,
                      Node w_2,
                      const char *donor_name,
                      Node v_1,
                      Node v_2) {

    if (result_name == nullptr || recipient_name == nullptr ||
       donor_name == nullptr || verifyGraphName(result_name) == false) {
        return false;
    }

    OptionalReference<Collection> collection = findCollection(id);
    if (!collection || collection->get().contains(result_name)) {
        return false;
    }

    OptionalReference<Graph> recipientGraph = findGraph(id, recipient_name);
    OptionalReference<Graph> donorGraph = findGraph(id, donor_name);

    if (!recipientGraph || !donorGraph) {
        return false;
    }

    NodeSet& recipientNodes = getNodes(recipientGraph->get());
    NodeSet& donorNodes = getNodes(donorGraph->get());
    EdgeMap& recipientEdges = getEdges(recipientGraph->get());
    EdgeMap& donorEdges = getEdges(donorGraph->get());

    if (!recipientNodes.contains(w_1) || !recipientNodes.contains(w_2) ||
        !donorNodes.contains(v_1) || !donorNodes.contains(v_2)) {
        return false;   // One or more of the nodes doesn't exist.
    }

    EdgeMap::iterator recipientEdgeIt = recipientEdges.find({ w_1, w_2 });

    if (v_1 == v_2 || recipientEdgeIt == recipientEdges.end()) {
        return false;   // Invalid edge replacement.
    }

    if (donorNodes.size() - 2 > INT_MAX - recipientNodes.size()) {
        return false;   // Not enough space for new nodes.
    }

    Level newLevel = 0;

    if (!donorEdges.empty()) {
        Level donorLevel = rgc_graph_level(id, donor_name);
        Level replaceLevel = recipientEdgeIt->second;

        newLevel = max(donorLevel, replaceLevel) + 1;

        if (newLevel >= LEVEL_MAX) {
            return false;   // Level limit exceeded.
        }
    }

    Graph resultGraph = recipientGraph->get(); // Creating result graph as a copy of the recipient graph.
    NodeSet& resultNodes = getNodes(resultGraph);
    EdgeMap& resultEdges = getEdges(resultGraph);

    resultEdges.erase({ w_1, w_2 }); // Remove from the result graph the edge { w_1, w_2 }.
    resultEdges.erase({ w_2, w_1 }); // Remove from the result graph the edge { w_2, w_1 }.

    map<Node, Node> nodeMapping; // Mapping donor nodes to result nodes.

    nodeMapping[v_1] = w_1;
    nodeMapping[v_2] = w_2;

    Node nextFreeNode = 0;

    for (Node donorNode : donorNodes) {
        if (donorNode == v_1 || donorNode == v_2) {
            continue; // Skip nodes v_1 and v_2
        }

        while (resultNodes.contains(nextFreeNode)) {
            ++nextFreeNode; // Find the next free node number in the result graph.
        }

        resultNodes.insert(nextFreeNode); // Add the new node to the result graph.
        nodeMapping[donorNode] = nextFreeNode;
        ++nextFreeNode;
    }

    for (EdgeMap::iterator it = donorEdges.begin(); it != donorEdges.end(); ++it) {
        Node donorX = it->first.first;
        Node donorY = it->first.second;

        Node resultX = nodeMapping[donorX];
        Node resultY = nodeMapping[donorY];

        resultEdges[{ resultX, resultY }] = newLevel;
    }

    collection->get().emplace(result_name, move(resultGraph)); // Add the result graph to the collection.
    return true;
}

ssize_t rgc_edge_level(CollectionId id, const char *name, Node x, Node y) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return -1;
    }

    EdgeMap& edges = getEdges(graph->get());
    EdgeMap::iterator it = edges.find({ x, y });

    if (it == edges.end()) {
        return -1;   // The edge { x, y } doesn't exist.
    }

    return it->second;
}

ssize_t rgc_graph_level(CollectionId id, const char *name) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return -1;
    }

    EdgeMap& edges = getEdges(graph->get());
    Level maxLevel = 0;

    for (auto& edge : edges) {
        maxLevel = max(maxLevel, edge.second);
    }

    return maxLevel;
}

ssize_t rgc_max_level() {
    return LEVEL_MAX - 1;
}

ssize_t rgc_count_nodes(CollectionId id, const char *name) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return -1;
    }

    return getNodes(graph->get()).size();
}

ssize_t rgc_count_edges(CollectionId id, const char *name) {
    if (name == nullptr) {
        return -1;
    }

    OptionalReference<Graph> graph = findGraph(id, name);
    if (!graph) {
        return -1;
    }

    return getEdges(graph->get()).size() / 2;  // Each edge is stored twice.
}

ssize_t rgc_size(CollectionId id) {
    OptionalReference<Collection> collection = findCollection(id);
    if (!collection) {
        return -1;
    }

    return collection->get().size();
}

#ifdef __cplusplus
} // namespace
#endif
