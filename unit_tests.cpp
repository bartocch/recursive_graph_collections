#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "recursive_graph_collections.h"

using cxx::rgc_new_collection;
using cxx::rgc_delete_collection;
using cxx::rgc_new_graph;
using cxx::rgc_delete_graph;
using cxx::rgc_graph_new_node;
using cxx::rgc_graph_delete_node;
using cxx::rgc_graph_add_edge;
using cxx::rgc_graph_delete_edge;

TEST_CASE("rgc_new_collection") {
    SUBCASE("returns correct ids upon successful creation") {
        CHECK(rgc_new_collection() == 0);
        CHECK(rgc_new_collection() == 1);
        CHECK(rgc_new_collection() == 2);
        CHECK(rgc_new_collection() == 3);
    }
}

TEST_CASE("rgc_delete_collection") {
    SUBCASE("correctly deletes a collection") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 1));        // Ensure the collection was created.
        rgc_delete_collection(collectionId);
        CHECK_FALSE(rgc_new_graph(collectionId, "G2", 1));  // Ensure the collection was deleted.
    }
}

TEST_CASE("rgc_new_graph") {
    SUBCASE("returns -1 on invalid arguments") {
        // TODO
    }

    SUBCASE("correctly creates a graph") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 1));
        CHECK(rgc_graph_new_node(collectionId, "G1") == 1);  // Ensure the graph was created.
    }
}

TEST_CASE("rgc_delete_graph") {
    SUBCASE("returns -1 on invalid arguments") {
        // TODO
    }

    SUBCASE("correctly deletes a graph") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 1));
        CHECK(rgc_graph_new_node(collectionId, "G1") == 1);  // Ensure the graph was created.
        rgc_delete_graph(collectionId, "G1");
        CHECK(rgc_graph_new_node(collectionId, "G1") == -1); // Ensure the graph was deleted.
    }
}
