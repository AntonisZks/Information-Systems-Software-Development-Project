#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <string>
#include <iomanip>


/**
 * @brief Function to print a progress bar with a percentage.
 * 
 * @param percentage The percentage of progress to display (0-100).
 * @param message Optional message to display before the progress bar.
 */
void printProgressBar(const double percentage, const std::string message = "");

#endif /* GRAPHICS_H */
