#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <string>
#include <iomanip>
#include <functional>
#include <chrono>

const std::string black = "\033[0;30m";
const std::string red = "\033[0;31m";
const std::string green = "\033[0;32m";
const std::string yellow = "\033[0;33m";
const std::string blue = "\033[0;34m";
const std::string magenta = "\033[0;35m";
const std::string cyan = "\033[0;36m";
const std::string white = "\033[0;37m";
const std::string reset = "\033[0m";

// Bright colors
const std::string brightBlack = "\033[1;30m";
const std::string brightRed = "\033[1;31m";
const std::string brightGreen = "\033[1;32m";
const std::string brightYellow = "\033[1;33m";
const std::string brightBlue = "\033[1;34m";
const std::string brightMagenta = "\033[1;35m";
const std::string brightCyan = "\033[1;36m";
const std::string brightWhite = "\033[1;37m";


/**
 * @brief Function to print a progress bar with a percentage.
 * 
 * Loading Symbol: #
 * 
 * @param percentage The percentage of progress to display (0-100).
 * @param message Optional message to display before the progress bar.
 */
void printProgressBar(
  const double percentage, 
  const std::string message = ""
);

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
  const int current, 
  const int total, 
  const std::string& message,
  const std::chrono::steady_clock::time_point& startTime,
  const unsigned int barWidth = 40
);

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
  const unsigned int start, 
  const unsigned int end, 
  const std::string message,
  std::function<void(int)> func, 
  const unsigned int barWidth = 40
);

#endif /* GRAPHICS_H */
