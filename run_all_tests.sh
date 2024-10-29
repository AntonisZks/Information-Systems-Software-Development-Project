if [ -d ./bin ]; then
    echo "Testing Graph Node..."
    ./bin/graph_node_test 
    echo "Testing Graph..."
    ./bin/graph_test 
    echo "Testing Eucleidian Distace Between Vectors"
    ./bin/test_distance
else
    echo "Library is not compiled. Please run 'make all' first."
fi
