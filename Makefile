.PHONY: clean

clean:
	rm *.o
	rm recursive_graph_collections_example_1 recursive_graph_collections_example_2_a recursive_graph_collections_example_2_b

recursive_graph_collections_4.o: recursive_graph_collections.cpp
	g++ -c -Wall -Wextra -O2 -std=c++23 recursive_graph_collections.cpp -o recursive_graph_collections_4.o

recursive_graph_collections_16.o: recursive_graph_collections.cpp
	g++ -c -Wall -Wextra -O2 -std=c++23 -DLEVEL_MAX=16 recursive_graph_collections.cpp -o recursive_graph_collections_16.o

recursive_graph_collections_example_1.o: recursive_graph_collections_example_1.c
	gcc -c -Wall -Wextra -O2 -std=c23 recursive_graph_collections_example_1.c -o recursive_graph_collections_example_1.o

recursive_graph_collections_example_2.o: recursive_graph_collections_example_2.cpp
	g++ -c -Wall -Wextra -O2 -std=c++23 recursive_graph_collections_example_2.cpp -o recursive_graph_collections_example_2.o

recursive_graph_collections_example_1: recursive_graph_collections_example_1.o recursive_graph_collections_4.o
	g++ recursive_graph_collections_example_1.o recursive_graph_collections_4.o -o recursive_graph_collections_example_1

recursive_graph_collections_example_2_a: recursive_graph_collections_example_2.o recursive_graph_collections_16.o
	g++ recursive_graph_collections_example_2.o recursive_graph_collections_16.o -o recursive_graph_collections_example_2_a

recursive_graph_collections_example_2_b: recursive_graph_collections_16.o recursive_graph_collections_example_2.o
	g++ recursive_graph_collections_16.o recursive_graph_collections_example_2.o -o recursive_graph_collections_example_2_b