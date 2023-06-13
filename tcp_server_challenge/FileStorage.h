#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>


std::string get_time_suffix(void);
void saveFile(const std::string directory, const std::string prefix, std::string data, std::size_t max_file_size_bytes);