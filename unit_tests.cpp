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

    SUBCASE("behaves as expected if collection doesn't exist") {
        // Assuming there isn't a collection with id = LONG_MAX.
        CHECK_NOTHROW(rgc_delete_collection(LONG_MAX));
    }
}

TEST_CASE("rgc_new_graph") {
    SUBCASE("returns -1 on invalid arguments") {
        long collectionId = rgc_new_collection();

        // name = nullptr
        CHECK_FALSE(rgc_new_graph(collectionId, nullptr, 0));

        // name contains invalid chars
        CHECK_FALSE(rgc_new_graph(collectionId, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ąćęłńóśźż", 0));

        // n < 0
        CHECK_FALSE(rgc_new_graph(collectionId, "G1", -1));
        CHECK_FALSE(rgc_new_graph(collectionId, "G1", -20));
    }

    SUBCASE("correctly creates a graph") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 1));
        CHECK(rgc_graph_new_node(collectionId, "G1") == 1);  // Ensure the graph was created.
    }

    SUBCASE("doesn't create a graph if name is duplicate") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK_FALSE(rgc_new_graph(collectionId, "G1", 20));
    }

    SUBCASE("doesn't create a graph if collection doesn't exist") {
        // Assuming there isn't a collection with id = LONG_MAX.
        CHECK_FALSE(rgc_new_graph(LONG_MAX, "G1", 20));
    }
}

TEST_CASE("rgc_delete_graph") {
    SUBCASE("behaves as expected on invalid arguments") {
        long collectionId = rgc_new_collection();
        CHECK_NOTHROW(rgc_delete_graph(collectionId, nullptr));
    }

    SUBCASE("correctly deletes a graph") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 1));
        CHECK(rgc_graph_new_node(collectionId, "G1") == 1);  // Ensure the graph was created.
        rgc_delete_graph(collectionId, "G1");
        CHECK(rgc_graph_new_node(collectionId, "G1") == -1); // Ensure the graph was deleted.
    }

    SUBCASE("behaves as expected if collection doesn't exist") {
        // Assuming there isn't a collection with id = LONG_MAX.
        CHECK_NOTHROW(rgc_delete_graph(LONG_MAX, "G1"));
    }

    SUBCASE("behaves as expected if graph doesn't exist") {
        long collectionId = rgc_new_collection();
        CHECK_NOTHROW(rgc_delete_graph(collectionId, "G1"));
    }
}
