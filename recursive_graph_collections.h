#include <sys/types.h>

#ifndef RECURSIVE_GRAPH_COLLECTIONS_H
#define RECURSIVE_GRAPH_COLLECTIONS_H

#ifdef __cplusplus

namespace cxx {
extern "C" {

#endif

long rgc_new_collection();
void rgc_delete_collection(long id);
bool rgc_new_graph(long id, const char *name, int n);
void rgc_delete_graph(long id, const char *name);
int rgc_graph_new_node(long id, const char *name);
bool rgc_graph_delete_node(long id, const char *name, int x);
bool rgc_graph_add_edge(long id, const char *name, int x, int y);
bool rgc_graph_delete_edge(long id, const char *name, int x, int y);
const char *rgc_first_graph(long id);
const char *rgc_next_graph(long id, const char *name);
int rgc_first_node(long id, const char *name);
int rgc_next_node(long id, const char *name, int x);
int rgc_first_edge(long id, const char *name, int x);
int rgc_next_edge(long id, const char *name, int x, int y);
bool rgc_replace_edge(long id,
                      const char *result_name,
                      const char *recipient_name,
                      int w_1,
                      int w_2,
                      const char *donor_name,
                      int v_1,
                      int v_2);
ssize_t rgc_edge_level(long id, const char *name, int x, int y);
ssize_t rgc_graph_level(long id, const char *name);
ssize_t rgc_max_level();
ssize_t rgc_count_nodes(long id, const char *name);
ssize_t rgc_count_edges(long id, const char *name);
ssize_t rgc_size(long id);

#ifdef __cplusplus

} // extern
} // namespace

#endif

#endif