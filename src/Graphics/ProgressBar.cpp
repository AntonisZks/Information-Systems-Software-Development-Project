#include "../include/graphics.h"

bool firstTime = true;


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

  if (firstTime) {
    std::cout << brightMagenta << "Action" << std::setw(22) << reset << " | ";
    std::cout << brightMagenta << "Progress" << std::setw(36) << reset << " | ";
    std::cout << brightMagenta << "Time Remaining" << reset << " | ";
    std::cout << brightMagenta << "Time Elapsed" << reset << std::endl;

    for (unsigned int i = 0; i < 24; i++) { std::cout << "-"; } std::cout << " | ";
    for (unsigned int i = 0; i < 40; i++) { std::cout << "-"; } std::cout << " | ";
    for (unsigned int i = 0; i < 14; i++) { std::cout << "-"; } std::cout << " | ";
    for (unsigned int i = 0; i < 13; i++) { std::cout << "-"; }
    std::cout << std::endl;

    firstTime = false;
  }

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
  std::cout << brightYellow << std::setw(24) << std::setfill(' ') << std::left << message << reset;

  // Display progress bar
  std::cout << " | " << green << "[";
  for (unsigned int i = 0; i < barWidth; i++) {
    if (i < position) std::cout << "=";
    else if (i == position) std::cout << ">";
    else std::cout << black << ".";
  }
  std::cout << green << "] " << brightGreen << std::setw(6) << std::setfill(' ') << std::right << std::fixed << std::setprecision(2);
  std::cout << float(progress * 100.0) << "%" << reset;

  // Display remaining time
  if (current > 0 && current < total) {
    std::cout << " | " << yellow;
    std::cout << yellow << std::setw(2) << std::setfill('0') << minutes << "m ";
    std::cout << std::setw(2) << std::setfill('0') << seconds << "s" << std::setw(11) << std::setfill(' ') << reset;
  }
  else if (current == total) {
    std::cout << " | " << yellow;
    std::cout << brightGreen << "Done" << std::setw(14) << std::setfill(' ') << reset;
  }

  // Display elapsed time
  int elapsedSeconds = static_cast<int>(elapsed.count());
  int elapsedMinutes = elapsedSeconds / 60;
  elapsedSeconds = elapsedSeconds % 60;

  std::cout << " | " << yellow;
  std::cout << cyan << std::setw(2) << std::setfill('0') << elapsedMinutes << "m ";
  std::cout << std::setw(2) << std::setfill('0') << elapsedSeconds << "s" << reset;

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
    displayProgressBar(i - start + 1, total, message, startTime, barWidth);
    func(i);
  }

  std::cout << std::endl;

}
