/// This utility function checks if the argv contains any @configfile strings,
///  and inserts command line options from that file into the argc and argv

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace ArgFilePopulator
{
	std::vector<std::string> PopulateArgsFromFiles(int argc, char** argv)
	{
		// Convert args to a vector of strings
		std::vector<std::string> args;
		for (int i = 0; i < argc; ++i)
		{
			std::string v(argv[i]);
			if (i != 0) // Ignore cmdline path
			{
				if (v[0] == '@')
				{
					std::ifstream config_file(v.substr(1));
					if (config_file.good())
					{
						while (!config_file.eof())
						{
							std::string line;
							std::getline(config_file, line);
							args.emplace_back(line);
						}
					}
					else
					{
						std::cerr << "Error opening config file: " << v << std::endl;
					}
				}
				else
				{
					args.emplace_back(v); // Non @configfile
				}
			}
			else
			{
				args.emplace_back(v); // First arg
			}
		}
		return args;
	}
}
