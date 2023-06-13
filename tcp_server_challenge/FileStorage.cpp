#include "FileStorage.h"

std::string get_time_suffix(void) {

	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	std::tm timeInfo;
	localtime_s(&timeInfo, &currentTime);

	std::ostringstream oss;
	oss << std::put_time(&timeInfo, "%Y%m%d%H%M%S");
	return oss.str();
}

void saveFile(const std::string directory, const std::string prefix, std::string data, std::size_t max_file_size_bytes)
{
	std::size_t data_size = data.size();
	std::string time_suffix = get_time_suffix();
	std::string filename = prefix + "_" + time_suffix;

	if (data_size > max_file_size_bytes)
	{
		std::size_t n = ceil( double(data_size) / double(max_file_size_bytes));
		std::cout << "Data is too big! must divide it!" << std::endl;
		std::cout << "Must be saved in " << n <<" files."<<std::endl;

		std::size_t index = 0;

		for (std::size_t i = 0; i < n; i++)
		{
			std::string filenamepart = filename + "_part" + std::to_string(i + 1) + ".txt";
			std::ofstream file(directory + filenamepart);

			std::size_t length = max_file_size_bytes;

			if ((index + max_file_size_bytes) > (data_size - 1))
			{
				length = data_size - 1;
			}
			
			std::string chunk = data.substr(index, length);
			index += length;

			if (file.is_open()) {

				file << chunk;
				file.close();
				std::cout << "File created and data saved successfully." << std::endl;
			}
			else {
				std::cout << "Failed to create or open the file." << std::endl;
			}

		}

	}
	else
	{
		filename += ".txt";
		std::ofstream file(directory + filename);
		
		if (file.is_open()) {
			file << data;
			file.close();
			std::cout << "File created and data saved successfully." << std::endl;
		}
		else {
			std::cout << "Failed to create or open the file." << std::endl;
		}

		std::cout << "Saves all of it in 1 file just fine!" << std::endl;

	}
}