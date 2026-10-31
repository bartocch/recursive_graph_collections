#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "lib/doctest.h"
#include "recursive_graph_collections.h"
#include <climits>
#include <cstring>

using cxx::rgc_new_collection;
using cxx::rgc_delete_collection;
using cxx::rgc_new_graph;
using cxx::rgc_delete_graph;
using cxx::rgc_graph_new_node;
using cxx::rgc_graph_delete_node;
using cxx::rgc_graph_add_edge;
using cxx::rgc_graph_delete_edge;
using cxx::rgc_next_graph;

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

    SUBCASE("does nothing if collection doesn't exist") {
        // Assuming there isn't a collection with id = LONG_MAX.
        CHECK_NOTHROW(rgc_delete_collection(LONG_MAX));
    }
}

TEST_CASE("rgc_new_graph") {
    SUBCASE("returns false on invalid arguments") {
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

    SUBCASE("returns false if name is duplicate") {
        long collectionId = rgc_new_collection();
        CHECK(rgc_new_graph(collectionId, "G1", 10));
        CHECK_FALSE(rgc_new_graph(collectionId, "G1", 20));
    }

    SUBCASE("returns false if collection doesn't exist") {
        // Assuming there isn't a collection with id = LONG_MAX.
        CHECK_FALSE(rgc_new_graph(LONG_MAX, "G1", 20));
    }
}

TEST_CASE("rgc_delete_graph") {
    SUBCASE("does nothing on invalid arguments") {
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

    SUBCASE("does nothing if collection doesn't exist") {
        // Assuming there isn't a collection with id = LONG_MAX.
        CHECK_NOTHROW(rgc_delete_graph(LONG_MAX, "G1"));
    }

    SUBCASE("does nothing if graph doesn't exist") {
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

TEST_CASE("rgc_next_graph") {
    SUBCASE("returns name of next graph even if there is no graph with the provided name") {
        long collectionId = rgc_new_collection();
        REQUIRE(rgc_new_graph(collectionId, "A", 10));
        REQUIRE(rgc_new_graph(collectionId, "B", 10));
        REQUIRE(rgc_new_graph(collectionId, "C", 10));
        REQUIRE(rgc_new_graph(collectionId, "D", 10));
        rgc_delete_graph(collectionId, "B");

        CHECK(strcmp(rgc_next_graph(collectionId, "B"), "C") == 0);
    }

    SUBCASE("returns nullptr if there is no next graph") {
        long collectionId = rgc_new_collection();
        REQUIRE(rgc_new_graph(collectionId, "A", 10));
        REQUIRE(rgc_new_graph(collectionId, "B", 10));

        CHECK(rgc_next_graph(collectionId, "B") == nullptr);
    }
}

TEST_CASE("iterators with missing node numbers") {
    long id = rgc_new_collection();
    REQUIRE(rgc_new_graph(id, "G", 5));

    REQUIRE(rgc_graph_delete_node(id, "G", 2));

    CHECK(cxx::rgc_first_node(id, "G") == 0);
    CHECK(cxx::rgc_next_node(id, "G", 2) == 3);
    CHECK(cxx::rgc_next_node(id, "G", -1) == 0);
    CHECK(cxx::rgc_next_node(id, "G", INT_MAX) == -1);

    REQUIRE(rgc_graph_add_edge(id, "G", 0, 1));
    REQUIRE(rgc_graph_add_edge(id, "G", 0, 4));

    CHECK(cxx::rgc_first_edge(id, "G", 0) == 1);
    CHECK(cxx::rgc_next_edge(id, "G", 0, 2) == 4);
    CHECK(cxx::rgc_next_edge(id, "G", 0, 4) == -1);
    CHECK(cxx::rgc_next_edge(id, "G", 0, INT_MAX) == -1);
    CHECK(cxx::rgc_first_edge(id, "G", 3) == -1);

    rgc_delete_collection(id);
}


TEST_CASE("deleting node removes all incident edges") {
    long id = rgc_new_collection();
    REQUIRE(rgc_new_graph(id, "G", 4));

    REQUIRE(rgc_graph_add_edge(id, "G", 0, 1));
    REQUIRE(rgc_graph_add_edge(id, "G", 1, 2));
    REQUIRE(rgc_graph_add_edge(id, "G", 1, 3));
    REQUIRE(rgc_graph_add_edge(id, "G", 0, 3));

    REQUIRE(rgc_graph_delete_node(id, "G", 1));

    CHECK(cxx::rgc_count_nodes(id, "G") == 3);
    CHECK(cxx::rgc_count_edges(id, "G") == 1);
    CHECK(cxx::rgc_edge_level(id, "G", 0, 1) == -1);
    CHECK(cxx::rgc_edge_level(id, "G", 1, 2) == -1);
    CHECK(cxx::rgc_edge_level(id, "G", 0, 3) == 0);

    rgc_delete_collection(id);
}


TEST_CASE("replace_edge with gaps in numbering") {
    long id = rgc_new_collection();

    REQUIRE(rgc_new_graph(id, "R", 5));
    REQUIRE(rgc_graph_delete_node(id, "R", 2));
    REQUIRE(rgc_graph_delete_node(id, "R", 3));
    REQUIRE(rgc_graph_add_edge(id, "R", 0, 1));

    REQUIRE(rgc_new_graph(id, "D", 4));
    REQUIRE(rgc_graph_add_edge(id, "D", 0, 1));
    REQUIRE(rgc_graph_add_edge(id, "D", 0, 2));
    REQUIRE(rgc_graph_add_edge(id, "D", 1, 3));

    if (cxx::rgc_max_level() > 0) {
        REQUIRE(cxx::rgc_replace_edge(
            id, "X", "R", 0, 1, "D", 2, 3
        ));

        CHECK(cxx::rgc_count_nodes(id, "X") == 5);
        CHECK(cxx::rgc_count_edges(id, "X") == 3);

        // Donor nodes 0 and 1 receive numbers 2 and 3.
        CHECK(cxx::rgc_edge_level(id, "X", 2, 3) == 1);
        CHECK(cxx::rgc_edge_level(id, "X", 0, 2) == 1);
        CHECK(cxx::rgc_edge_level(id, "X", 1, 3) == 1);
        CHECK(cxx::rgc_edge_level(id, "X", 0, 1) == -1);

        CHECK(cxx::rgc_count_edges(id, "R") == 1);
        CHECK(cxx::rgc_count_edges(id, "D") == 3);
    }

    rgc_delete_collection(id);
}


TEST_CASE("replace_edge with edgeless donor") {
    long id = rgc_new_collection();

    REQUIRE(rgc_new_graph(id, "R", 3));
    REQUIRE(rgc_graph_add_edge(id, "R", 0, 1));
    REQUIRE(rgc_graph_add_edge(id, "R", 1, 2));

    REQUIRE(rgc_new_graph(id, "D", 3));

    REQUIRE(cxx::rgc_replace_edge(
        id, "X", "R", 0, 1, "D", 0, 1
    ));

    CHECK(cxx::rgc_count_nodes(id, "X") == 4);
    CHECK(cxx::rgc_count_edges(id, "X") == 1);
    CHECK(cxx::rgc_graph_level(id, "X") == 0);

    CHECK(cxx::rgc_edge_level(id, "X", 0, 1) == -1);
    CHECK(cxx::rgc_edge_level(id, "X", 1, 2) == 0);
    CHECK(cxx::rgc_first_edge(id, "X", 3) == -1);

    CHECK(cxx::rgc_count_edges(id, "R") == 2);

    rgc_delete_collection(id);
}


TEST_CASE("replace_edge with same donor and recipient") {
    long id = rgc_new_collection();

    REQUIRE(rgc_new_graph(id, "G", 3));
    REQUIRE(rgc_graph_add_edge(id, "G", 0, 1));
    REQUIRE(rgc_graph_add_edge(id, "G", 1, 2));

    if (cxx::rgc_max_level() > 0) {
        REQUIRE(cxx::rgc_replace_edge(
            id, "X", "G", 0, 1, "G", 0, 2
        ));

        CHECK(cxx::rgc_count_nodes(id, "X") == 4);
        CHECK(cxx::rgc_count_edges(id, "X") == 3);
        CHECK(cxx::rgc_graph_level(id, "X") == 1);

        CHECK(cxx::rgc_edge_level(id, "X", 0, 3) == 1);
        CHECK(cxx::rgc_edge_level(id, "X", 3, 1) == 1);
        CHECK(cxx::rgc_edge_level(id, "X", 1, 2) == 0);

        // Original graph must remain unchanged.
        CHECK(cxx::rgc_count_nodes(id, "G") == 3);
        CHECK(cxx::rgc_count_edges(id, "G") == 2);
    }

    rgc_delete_collection(id);
}


TEST_CASE("replace_edge level boundary") {
    long id = rgc_new_collection();

    REQUIRE(rgc_new_graph(id, "L0", 2));
    REQUIRE(rgc_graph_add_edge(id, "L0", 0, 1));

    std::string current = "L0";
    auto max_level = cxx::rgc_max_level();

    for (decltype(max_level) level = 1;
         level <= max_level; ++level) {

        std::string next = "L" + std::to_string(level);

        REQUIRE(cxx::rgc_replace_edge(
            id, next.c_str(), current.c_str(),
            0, 1, "L0", 0, 1
        ));

        CHECK(cxx::rgc_edge_level(
            id, next.c_str(), 0, 1
        ) == level);

        current = next;
    }

    // Creating an edge above LEVEL_MAX - 1 must fail.
    CHECK_FALSE(cxx::rgc_replace_edge(
        id, "Overflow", current.c_str(),
        0, 1, "L0", 0, 1
    ));

    CHECK(cxx::rgc_count_nodes(id, "Overflow") == -1);

    // An edgeless donor is still allowed.
    REQUIRE(rgc_new_graph(id, "Empty", 2));

    REQUIRE(cxx::rgc_replace_edge(
        id, "Gone", current.c_str(),
        0, 1, "Empty", 0, 1
    ));

    CHECK(cxx::rgc_count_edges(id, "Gone") == 0);
    CHECK(cxx::rgc_graph_level(id, "Gone") == 0);

    rgc_delete_collection(id);
}
