# recursive_graph_collections

## Running unit tests

This project uses [doctest](https://github.com/doctest/doctest) for unit tests.
The library is already present in the `lib/` folder.

```
make unit_tests
./unit_tests
```

## Building

- `make all` - Builds all of the examples
- `make clean` - Removes all output files
- `make recursive_graph_collections_example_1` - Builds 1st example with `LEVEL_MAX = 4`
- `make recursive_graph_collections_example_2_a` - Builds 2nd example with `LEVEL_MAX = 16`
- `make recursive_graph_collections_example_2_b` - Builds 2nd example with `LEVEL_MAX = 16`.
  Uses swapped file order when linking.
- `make unit_tests` - Builds the unit test runner
