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

		TCLAP::MultiArg<int> arg_board_width("w", "width", "The width of the playing board.", false, "Integer");
		TCLAP::MultiArg<int> arg_board_height("g", "height", "The height of the playing board.", false, "Integer");
		TCLAP::MultiArg<double> arg_pill_density("p", "density", "The chance that a cell will contain a pill", false, "Decimal between 0 and 1");

		TCLAP::MultiArg<int> arg_populate_size("z", "population", "The number of individuals kept after each generation", false, "Integer");
		TCLAP::MultiArg<int> arg_children_size("l", "children", "The number of new individuals created during recombination", false, "Integer");
		TCLAP::MultiArg<double> arg_mutation_chance("m", "mutation", "The chance that the children will be mutated", false, "Decimal between 0 and 1");
		TCLAP::MultiArg<int> arg_initialization_height("v", "levels", "The maximum number of levels of the starting population", false, "Integer");
		TCLAP::MultiArg<double> arg_parsimony_pressure("y", "parsimony", "f'(x) = f(x) + maximum_possible_fitness - parsimony_pressure * (# of nodes)", false, "Decimal");
		TCLAP::MultiArg<int> arg_maximum_stale_generations("t", "converge", "The number of non-improving best fitness generations before the run is terminated (-1 to not use)", false, "Integer");

		TCLAP::MultiArg<int> arg_random_seed("s", "seed", "A number to seed the random number generator with, or -1 to seed with current time.", false, "Integer");
		TCLAP::MultiArg<int> arg_runs("r", "runs", "The number of runs.", false, "Integer");
		TCLAP::MultiArg<int> arg_evals("e", "evals", "The maximum number of evaluations that each run can take.", false, "Integer");

		TCLAP::MultiArg<std::string> arg_world_filename("o", "worldfile", "Path to the world log file to be created.", false, "File path");
		TCLAP::MultiArg<std::string> arg_score_filename("c", "scorefile", "Path to score log file to be created.", false, "File path");
		TCLAP::MultiArg<std::string> arg_solution_filename("u", "solutionfile", "Path to solution file to be created.", false, "File path");
		
		cmd.add(arg_board_width);
		cmd.add(arg_board_height);
		cmd.add(arg_pill_density);

		cmd.add(arg_populate_size);
		cmd.add(arg_children_size);
		cmd.add(arg_mutation_chance);
		cmd.add(arg_initialization_height);
		cmd.add(arg_parsimony_pressure);

		cmd.add(arg_random_seed);
		cmd.add(arg_runs);
		cmd.add(arg_evals);

		cmd.add(arg_world_filename);
		cmd.add(arg_score_filename);
		cmd.add(arg_solution_filename);

		// Parse command line arguments
		cmd.parse(args);

		// If an argument was specified multiple times, grab the last one (This allows easy overriding of a config file)
		// If an argument was NOT specified, use a default value
		int width = arg_board_width.getValue().size() ? arg_board_width.getValue().back() : 30;
		int height = arg_board_height.getValue().size() ? arg_board_height.getValue().back() : 30;
		double density = arg_pill_density.getValue().size() ? arg_pill_density.getValue().back() : 0.5;

		int population_size = arg_populate_size.getValue().size() ? arg_populate_size.getValue().back() : 50;
		int children_size = arg_children_size.getValue().size() ? arg_children_size.getValue().back() : 40;
		double mutation_chance = arg_mutation_chance.getValue().size() ? arg_mutation_chance.getValue().back() : 0.05;
		int initialization_height = arg_initialization_height.getValue().size() ? arg_initialization_height.getValue().back() : 5;
		double parsimony_pressure = arg_parsimony_pressure.getValue().size() ? arg_parsimony_pressure.getValue().back() : 0.5;
		int maximum_stale_generations = arg_maximum_stale_generations.getValue().size() ? arg_maximum_stale_generations.getValue().back() : -1;
		
		int random_seed_value = arg_random_seed.getValue().size() ? arg_random_seed.getValue().back() : -1;
		int runs_value = arg_runs.getValue().size() ? arg_runs.getValue().back() : 3;
		int evals_value = arg_evals.getValue().size() ? arg_evals.getValue().back() : 2000;
		
		std::string world_filename_value = arg_world_filename.getValue().size() ? arg_world_filename.getValue().back() : "default.world";
		std::string score_filename_value = arg_score_filename.getValue().size() ? arg_score_filename.getValue().back() : "default.score";
		std::string solution_filename_value = arg_solution_filename.getValue().size() ? arg_solution_filename.getValue().back() : "default.solution";

		// More config logic
		int time_limit = width * height * 2;
		int seed = random_seed_value < 0 ? time(0) : random_seed_value;
		// Treat negative as no convergence termination criteria
		if (maximum_stale_generations <= 0) maximum_stale_generations = std::numeric_limits<int>::max();
		std::mt19937 random(seed);

		// Calculate maximum length of buffer needed to store log file for a single eval
		int length_of_step = get_printf_length(128, "m %i %i\n", width, height) * 4 + get_printf_length(128, "t %i %i\n", time_limit, width * height);
		int buffer_size = 0;
		buffer_size += get_printf_length(128, "%i\n%i\n", width, height);
		buffer_size += get_printf_length(128, "p %i %i\n", width, height) * (width * height);
		buffer_size += length_of_step * time_limit;

		// Send parameters to individual
		Individual::width = width;
		Individual::height = height;
		Individual::density = density;
		Individual::time_limit = time_limit;
		Individual::buffer_size = buffer_size;
		Individual::parsimony_pressure = parsimony_pressure;

		// IOStreams are slow, just use c-style file io
		FILE* world_file = fopen(world_filename_value.c_str(), "w");
		FILE* score_file = fopen(score_filename_value.c_str(), "w");
		FILE* solution_file = fopen(solution_filename_value.c_str(), "w");

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

		Individual best_individual(random);
		best_individual.fitness = 0;
		best_individual.valid_fitness = true;

		// Run the EA!
		for (int run = 0; run < runs_value; ++run)
		{
			// Keep track of best individual for the run
			Individual best_run_individual(random);
			best_run_individual.fitness = 0;
			best_run_individual.valid_fitness = true;

			// Termination checkers
			int evals = 0;
			int generations_since_improvement = 0;

			// Initialize the set of individuals
			auto individuals = Initializers::RampedHalfAndHalf(random, population_size, initialization_height);
			for (auto&& i : individuals)
				i.evaluate(random);
			evals += individuals.size();

			std::sort(individuals.begin(), individuals.end(), [&](Individual& a, Individual& b) { return a.GetFitness(random) > b.GetFitness(random); });
			
			printf("Run %i:\n", run + 1);
			fprintf(score_file, "\nRun %i\n", run + 1);
			while (evals < evals_value && generations_since_improvement < maximum_stale_generations)
			{
				auto parent_indices = ParentSelection::overselection(random, individuals, children_size / 2);

				auto children = ParentSelection::generate_children(random, individuals, parent_indices);

				for (auto&& i : children)
				{
					i.evaluate(random);
				}
				evals += children.size();

				if (chance(random, mutation_chance))
				{
					for (auto&& child : children)
					{
						Brain::mutate(random, &child.pacman_controller.root);
					}
				}
				// Sort children
				std::sort(children.begin(), children.end(), [&](const Individual& a, const Individual& b) { return a.GetFitness(random) > b.GetFitness(random); });
				if (children[0].GetFitness(random) > best_run_individual.GetFitness(random))
				{
					best_run_individual.StealBuffer(children[0]);
					generations_since_improvement = 0;
					fprintf(score_file, "%i\t%f\t%i\n", evals, average(individuals, [&](const Individual& i) {return i.game_fitness; }), best_run_individual.game_fitness);
				}
				else
				{
					generations_since_improvement += 1;
				}

				individuals.reserve(individuals.size() + children.size());
				std::move(std::begin(children), std::end(children), std::back_inserter(individuals));
				std::inplace_merge(std::begin(individuals), std::begin(individuals) + population_size, std::end(individuals), [&](const Individual& a, const Individual& b) { return a.GetFitness(random) > b.GetFitness(random); });
				children.clear();

				ParentSelection::kTournament(random, individuals, population_size, 5);
			} // Finished with run

			if (best_run_individual.GetFitness(random) > best_individual.GetFitness(random))
			{
				best_individual.StealBuffer(best_run_individual);
			}
		} // Finished with EA

		fclose(score_file);
		fprintf(world_file, "%s", best_individual.buffer.get());
		fclose(world_file);
		fprintf(solution_file, "%i : %s\n", best_individual.game_fitness, Brain::ToEquation(&best_individual.pacman_controller.root).c_str());
		fclose(solution_file);

		std::cout << Brain::ToEquation(&best_individual.pacman_controller.root) << "\n";

		puts("Done!\n");
	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}

    return 0; // Program ran successfully
}

