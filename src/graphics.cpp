#include "../include/graphics.h"

/**
 * @brief Function to print a progress bar with a percentage.
 * 
 * @param percentage The percentage of progress to display (0-100).
 * @param message Optional message to display before the progress bar.
 */
void printProgressBar(const double percentage, const std::string message = "") {
  const int barWidth = 50; // Width of the progress bar in characters
  unsigned int pos = static_cast<unsigned int>((percentage * barWidth) / 100); // Calculate position in bar

  // Print progress bar with optional message
  std::cout << "\r" << message << " [";
  for (unsigned int i = 0; i < barWidth; i++) {
    if (i < pos) {
      std::cout << "#";
    } else {
      std::cout << " ";
    }
  }

  // Print percentage with two decimal places
  std::cout << "] " << std::fixed << std::setprecision(2) << percentage << "%" << std::flush;

  // Print newline when the progress reaches 100%
  if (percentage >= 100.0) {
    std::cout << std::endl;
  }

}