#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "lib/doctest.h"
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

TEST_CASE("rgc_graph_new_node") {
    SUBCASE("returns -1 on invalid arguments") {
        long collectionId = rgc_new_collection();

        // name = nullptr
        CHECK(rgc_graph_new_node(collectionId, nullptr) == -1);
    }

    SUBCASE("correctly creates nodes") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK(rgc_graph_new_node(collectionId, "G1") == 10);
        CHECK(rgc_graph_new_node(collectionId, "G1") == 11);
        CHECK(rgc_graph_new_node(collectionId, "G1") == 12);
        CHECK(rgc_graph_delete_node(collectionId, "G1", 10));  // Ensure nodes were created
        CHECK(rgc_graph_delete_node(collectionId, "G1", 11));
        CHECK(rgc_graph_delete_node(collectionId, "G1", 12));
    }

    SUBCASE("returns -1 if collection doesn't exist") {
        // Assuming that a collection with id LONG_MAX doesn't exist.
        CHECK(rgc_graph_new_node(LONG_MAX, "G1") == -1);
    }

    SUBCASE("returns -1 if graph doesn't exist") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_graph_new_node(collectionId, "G1") == -1);
    }
}

TEST_CASE("rgc_graph_delete_node") {
    SUBCASE("returns false on invalid arguments") {
        long collectionId = rgc_new_collection();

        // name = nullptr
        CHECK_FALSE(rgc_graph_delete_node(collectionId, nullptr, 10));
    }

    SUBCASE("correctly deletes nodes") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK(rgc_graph_delete_node(collectionId, "G1", 5));
        CHECK(rgc_graph_delete_node(collectionId, "G1", 6));
        CHECK(rgc_graph_delete_node(collectionId, "G1", 8));
        CHECK(rgc_graph_new_node(collectionId, "G1") == 5);
        CHECK(rgc_graph_new_node(collectionId, "G1") == 6);
        CHECK(rgc_graph_new_node(collectionId, "G1") == 8);
    }

    SUBCASE("returns false if collection doesn't exist") {
        // Assuming that a collection with id LONG_MAX doesn't exist.
        CHECK_FALSE(rgc_graph_delete_node(LONG_MAX, "G1", 10));
    }

    SUBCASE("returns false if graph doesn't exist") {
        long collectionId = rgc_new_collection();
        CHECK_FALSE(rgc_graph_delete_node(collectionId, "G1", 10));
    }

    SUBCASE("returns false if node doesn't exist") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK_FALSE(rgc_graph_delete_node(collectionId, "G1", 10));
    }
}

TEST_CASE("rgc_graph_add_edge") {
    SUBCASE("returns false on invalid arguments") {
        long collectionId = rgc_new_collection();

        // name = nullptr
        CHECK_FALSE(rgc_graph_add_edge(collectionId, nullptr, 10, 20));

        // x = y
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK_FALSE(rgc_graph_add_edge(collectionId, "G1", 5, 5));
    }

    SUBCASE("correctly creates edges") {
        long collectionId = rgc_new_collection();

        CHECK(rgc_new_graph(collectionId, "G1", 10));

        // Test all arg orders
        CHECK(rgc_graph_add_edge(collectionId, "G1", 1, 2));
        CHECK(rgc_graph_delete_edge(collectionId, "G1", 1, 2)); // Ensure that edges were created
        CHECK(rgc_graph_add_edge(collectionId, "G1", 2, 1));
        CHECK(rgc_graph_delete_edge(collectionId, "G1", 1, 2)); // Ensure that edges were created
        CHECK(rgc_graph_add_edge(collectionId, "G1", 1, 2));
        CHECK(rgc_graph_delete_edge(collectionId, "G1", 2, 1)); // Ensure that edges were created
        CHECK(rgc_graph_add_edge(collectionId, "G1", 2, 1));
        CHECK(rgc_graph_delete_edge(collectionId, "G1", 2, 1)); // Ensure that edges were created
    }

    SUBCASE("returns false if edge already exists") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK(rgc_graph_add_edge(collectionId, "G1", 2, 8));
        CHECK_FALSE(rgc_graph_add_edge(collectionId, "G1", 2, 8));
    }

    SUBCASE("returns false if collection doesn't exist") {
        // Assuming that a collection with id LONG_MAX doesn't exist.
        CHECK_FALSE(rgc_graph_add_edge(LONG_MAX, "G1", 2, 5));
    }

    SUBCASE("returns false if graph doesn't exist") {
        long collectionId = rgc_new_collection();
        CHECK_FALSE(rgc_graph_add_edge(collectionId, "G1", 2, 5));
    }

    SUBCASE("returns false if node doesn't exist") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK_FALSE(rgc_graph_add_edge(collectionId, "G1", 10, 5));
        CHECK_FALSE(rgc_graph_add_edge(collectionId, "G1", 5, 10));
        CHECK_FALSE(rgc_graph_add_edge(collectionId, "G1", 10, 11));
    }
}

TEST_CASE("rgc_graph_delete_edge") {
    SUBCASE("returns false on invalid arguments") {
        long collectionId = rgc_new_collection();

        // name = nullptr
        CHECK_FALSE(rgc_graph_delete_edge(collectionId, nullptr, 10, 20));
    }

    SUBCASE("correctly deletes edges") {
        long collectionId = rgc_new_collection();

        CHECK(rgc_new_graph(collectionId, "G1", 10));

        // Test all arg orders
        CHECK(rgc_graph_add_edge(collectionId, "G1", 1, 2));
        CHECK(rgc_graph_delete_edge(collectionId, "G1", 1, 2));
        CHECK(rgc_graph_add_edge(collectionId, "G1", 2, 1));    // Ensure that edges were deleted
        CHECK(rgc_graph_delete_edge(collectionId, "G1", 1, 2));
        CHECK(rgc_graph_add_edge(collectionId, "G1", 1, 2));    // Ensure that edges were deleted
        CHECK(rgc_graph_delete_edge(collectionId, "G1", 2, 1));
        CHECK(rgc_graph_add_edge(collectionId, "G1", 2, 1));    // Ensure that edges were deleted
        CHECK(rgc_graph_delete_edge(collectionId, "G1", 2, 1));
        CHECK(rgc_graph_add_edge(collectionId, "G1", 1, 2));    // Ensure that edges were deleted
    }

    SUBCASE("returns false if collection doesn't exist") {
        // Assuming that a collection with id LONG_MAX doesn't exist.
        CHECK_FALSE(rgc_graph_delete_edge(LONG_MAX, "G1", 10, 20));
    }

    SUBCASE("returns false if graph doesn't exist") {
        long collectionId = rgc_new_collection();
        CHECK_FALSE(rgc_graph_delete_edge(collectionId, "G1", 10, 20));
    }

    SUBCASE("returns false if edge doesn't exist") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK_FALSE(rgc_graph_delete_edge(collectionId, "G1", 2, 8));
        CHECK_FALSE(rgc_graph_delete_edge(collectionId, "G1", 4, 5));
    }
}
