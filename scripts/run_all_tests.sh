if [ -d ./bin ]; then
    echo "Testing Graph Node..."
    ./bin/graph_node_test 
    echo "Testing Graph..."
    ./bin/graph_test 
    echo "Testing Eucleidian Distace Between Vectors"
    ./bin/test_distance
    echo "Testing Read Data From Vector Files Functions"
    ./bin/test_data_vectors
    echo "Testing the Recall Evaluation Method"
    ./bin/test_recall
    
else
    echo "Library is not compiled. Please run 'make all' first."
fi
