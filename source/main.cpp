/// The main entry point into the EA
///

#include <cstdio>
#include <random>
#include <time.h>
#include <functional>
#include <iostream>
#include <fstream>

#include "tclap/CmdLine.h"

#include "ArgFilePopulator.h"
#include "Game.h"
#include "Helpers.h"
#include "Node.h"
#include "TreePretty.h"
#include "Initializers.h"
#include "ParentSelection.h"

int main(int argc, char** argv)
{
	// Preparse args and check for @configfile
	std::vector<std::string> args(ArgFilePopulator::PopulateArgsFromFiles(argc, argv));

	// Parse augmented args
	try {
		// Setup command line configurations
		TCLAP::CmdLine cmd("A genetic programming EA for co-evolving controllers for Ms. Pacman and ghosts.\n"
			"Use @argfile to include arguments from argfile.", ' ', "0.1");

		TCLAP::MultiArg<int> board_width("w", "width", "The width of the playing board.", false, "Integer");
		TCLAP::MultiArg<int> board_height("g", "height", "The height of the playing board.", false, "Integer");
		TCLAP::MultiArg<double> pill_density("p", "density", "The chance that a cell will contain a pill", false, "Decimal between 0 and 1");
		//TCLAP::MultiArg<int> time_limit("t", "time", "The number of turns that Ms. Pacman has to eat all the dots.", false, 1000, "Integer");

		TCLAP::MultiArg<int> random_seed("s", "seed", "A number to seed the random number generator with, or -1 to seed with current time.", false, "Integer");
		TCLAP::MultiArg<int> runs("r", "runs", "The number of runs.", false, "Integer");
		TCLAP::MultiArg<int> evals("e", "evals", "The maximum number of evaluations that each run can take.", false, "Integer");

		TCLAP::MultiArg<std::string> world_filename("o", "worldfile", "Path to the world log file to be created.", false, "File path");
		TCLAP::MultiArg<std::string> score_filename("c", "scorefile", "Path to score log file to be created.", false, "File path");
		
		cmd.add(board_width);
		cmd.add(board_height);
		cmd.add(pill_density);
		//cmd.add(time_limit);

		cmd.add(random_seed);
		cmd.add(runs);
		cmd.add(evals);

		cmd.add(world_filename);
		cmd.add(score_filename);

		// Parse command line arguments
		cmd.parse(args);

		// If an argument was specified multiple times, grab the last one (This allows easy overriding of a config file)
		// If an argument was NOT specified, use a default value
		int width = board_width.getValue().size() ? board_width.getValue().back() : 30;
		int height = board_height.getValue().size() ? board_height.getValue().back() : 30;
		double density = pill_density.getValue().size() ? pill_density.getValue().back() : 0.5;

		int random_seed_value = random_seed.getValue().size() ? random_seed.getValue().back() : -1;
		int runs_value = runs.getValue().size() ? runs.getValue().back() : 3;
		int evals_value = evals.getValue().size() ? evals.getValue().back() : 2000;
		
		std::string world_filename_value = world_filename.getValue().size() ? world_filename.getValue().back() : "default.world";
		std::string score_filename_value = score_filename.getValue().size() ? score_filename.getValue().back() : "default.score";

		int time_limit = width * height * 2;
		int seed = random_seed_value < 0 ? time(0) : random_seed_value;
		std::mt19937 random(seed);

		// Calculate maximum length of buffer needed to store log file for a single eval
		int length_of_step = get_printf_length(128, "m %i %i\n", width, height) * 4 + get_printf_length(128, "t %i %i\n", time_limit, width * height);
		int buffer_size = 0;
		buffer_size += get_printf_length(128, "%i\n%i\n", width, height);
		buffer_size += get_printf_length(128, "p %i %i\n", width, height) * (width * height);
		buffer_size += length_of_step * time_limit;
		char* best_buffer = nullptr;

		// Send parameters to individual
		Individual::width = width;
		Individual::height = height;
		Individual::density = density;
		Individual::time_limit = time_limit;
		Individual::buffer_size = buffer_size;

		// IOStreams are slow, just use c-style file io
		FILE* world_file = fopen(world_filename_value.c_str(), "w");
		FILE* score_file = fopen(score_filename_value.c_str(), "w");

		// Record and print out configuration options
		fprintf(score_file, "Board width: %i\n"
			"Board height: %i\n"
			"Pill Density: %f\n"
			"Random seed: %i\n"
			"Runs: %i\n"
			"Evals: %i\n"
			"World file: %s\n"
			"Score file: %s\n\nResult Log\n", width, height, density, seed, runs_value, evals_value,
			world_filename_value.c_str(), score_filename_value.c_str());
		printf("Board width: %i\n"
			"Board height: %i\n"
			"Pill Density: %f\n"
			"Random seed: %i\n"
			"Runs: %i\n"
			"Evals: %i\n"
			"World file: %s\n"
			"Score file: %s\n", width, height, density, seed, runs_value, evals_value,
			world_filename_value.c_str(), score_filename_value.c_str());

		GameState::InitializeNeighbors(width, height);

		// Run the EA!
		for (int run = 0; run < runs_value; ++run)
		{
			// Initialize the set of individuals
			auto individuals = Initializers::RampedHalfAndHalf(random, 50, 5);
			for (auto&& i : individuals)
				i.evaluate(random);

			std::sort(individuals.begin(), individuals.end(), [&](Individual& a, Individual& b) { return a.GetFitness(random) > b.GetFitness(random); });
			
			printf("Run %i:\n", run + 1);
			fprintf(score_file, "\nRun %i\n", run + 1);
			for (int eval = 0; eval < evals_value; ++eval)
			{
				auto parent_indices = ParentSelection::overselection(random, individuals, 20);

				auto children = ParentSelection::generate_children(random, individuals, parent_indices);

				for (auto&& i : children)
				{
					std::cout << "Child: " << Brain::ToEquation(&i.pacman_controller.root) << "\n";
					i.evaluate(random);
				}

				if (chance(random, 0.05)) // TODO: config
				{
					for (auto&& child : children)
					{
						Brain::mutate(random, &child.pacman_controller.root);
					}
				}
				// Sort children
				std::sort(children.begin(), children.end(), [&](const Individual& a, const Individual& b) { return a.GetFitness(random) > b.GetFitness(random); });

				individuals.reserve(individuals.size() + children.size());
				std::move(std::begin(children), std::end(children), std::back_inserter(individuals));
				std::inplace_merge(std::begin(individuals), std::begin(individuals) + 50, std::end(individuals), [&](const Individual& a, const Individual& b) { return a.GetFitness(random) > b.GetFitness(random); });
				children.clear();

				ParentSelection::truncate(individuals, 50);
			}
		}

		fprintf(world_file, "%s", best_buffer);

		delete[] best_buffer;
		puts("Done!\n");
	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}

    return 0; // Program ran successfully
}

