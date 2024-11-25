#include "../include/graphics.h"

/**
 * @brief Function to print a progress bar with a percentage.
 * 
 * @param percentage The percentage of progress to display (0-100).
 * @param message Optional message to display before the progress bar.
 */
void printProgressBar(const double percentage, const std::string message) {
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

/**
 * @brief Function to display a progress bar with a percentage.
 * 
 * Loading Symbol: =>
 * 
 * @param current The current progress value.
 * @param total The total progress value.
 * @param barWidth The width of the progress bar in characters.
 * @param message Optional message to display before the progress bar.
 * @param startTime The start time of the progress bar.
 * @param barWidth The width of the progress bar in characters.
 */
void displayProgressBar(
  const int current, const int total, const std::string& message, const std::chrono::steady_clock::time_point& startTime, const unsigned int barWidth) {

  float progress = static_cast<float>(current) / total;
  unsigned int position = barWidth * progress;

  // Calculate elapsed time
  auto now = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = now - startTime;

  // Estimate remaining time
  double estimatedTotalTime = progress > 0 ? elapsed.count() / progress : 0;
  double remainingTime = estimatedTotalTime - elapsed.count();

  // Format remaining time as minutes and seconds
  int remainingSeconds = static_cast<int>(remainingTime);
  int minutes = remainingSeconds / 60;
  int seconds = remainingSeconds % 60;

  // Display action message
  std::cout << cyan << "Action: ";
  std::cout << brightCyan << std::setw(15) << std::setfill(' ') << std::left << message << reset;

  // Display progress bar
  std::cout << " | " << green << "Progress: [";
  for (unsigned int i = 0; i < barWidth; i++) {
    if (i < position) std::cout << "=";
    else if (i == position) std::cout << ">";
    else std::cout << black << ".";
  }
  std::cout << "] " << brightGreen << std::setw(6) << std::setfill(' ') << std::right << std::fixed << std::setprecision(2);
  std::cout << float(progress * 100.0) << "%" << reset;

  // Display remaining time
  if (current > 0 && current < total) {
    std::cout << " | " << yellow << "Time remaining: ";
    std::cout << brightYellow << std::setw(2) << std::setfill('0') << minutes << "m ";
    std::cout << std::setw(2) << std::setfill('0') << seconds << "s" << reset;
  }

  std::cout << "\r"; // Return the cursor to the start of the line
  std::cout.flush();

}

/**
 * @brief Function to execute a function with a progress bar.
 * 
 * @param start The starting value of the progress bar.
 * @param end The ending value of the progress bar.
 * @param message The message to display before the progress bar.
 * @param func The function to execute.
 * @param barWidth The width of the progress bar in characters.
 */
void withProgress(
  const unsigned int start, const unsigned int end, const std::string message, std::function<void(int)> func, const unsigned int barWidth) {

  int total = end - start;
  auto startTime = std::chrono::steady_clock::now();

  for (unsigned int i = start; i < end; i++) {
    func(i);
    displayProgressBar(i - start + 1, total, message, startTime, barWidth);
  }

  std::cout << std::endl;

}
