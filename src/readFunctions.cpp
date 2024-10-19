#include <iostream>
#include <fstream>

int main() {
    std::ifstream file("../data/siftsmall/siftsmall_base.fvecs", std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    int nb_vectors = 0;
    while (file) {
        int d;  // dimensionality of the vector (first 4 bytes)

        // Read dimensionality (4 bytes)
        file.read(reinterpret_cast<char*>(&d), sizeof(d));

        if (file.eof()) break; // Exit loop if end of file is reached

        // Display the dimensionality of the current vector
        std::cout << "Vector " << nb_vectors + 1 << ": dimension = " << d << std::endl;

        // Skip the rest of the vector (d * 4 bytes, since it's a float array)
        file.seekg(d * sizeof(float), std::ios::cur);

        nb_vectors++;
    }

    std::cout << "Total number of base vectors: " << nb_vectors << std::endl;

    file.close();
    return 0;
}
