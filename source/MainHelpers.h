#pragma once
#include <cstdio>
#include <string>

inline void print_args(FILE* score_file,
	int width, int height, double density, int population_size, int children_size, double mutation_chance,
	int initialization_height, double parsimony_pressure, int maximum_stale_generations, std::string parent_selection,
    std::string survival_selection, int tournament_size, int seed, int runs_value, int evals_value,
	std::string world_filename_value, std::string score_filename_value, std::string solution_filename_value)
{
	fprintf(score_file, "Board width: %i\n"
		"Board height: %i\n"
		"Pill Density: %f\n"
		"Population Size: %i\n"
		"Children Size: %i\n"
		"Mutation Chance: %f\n"
		"Initialization Height: %i\n"
		"Parsimony Pressure: %f\n"
		"Maximum Stale Generations: %i\n"
		"Parent Selection Strategy: %s\n"
		"Survival Selection Strategy: %s\n"
		"Tournament Size: %i\n"
		"Random seed: %i\n"
		"Runs: %i\n"
		"Evals: %i\n"
		"World file: %s\n"
		"Score file: %s\n"
		"Solution file: %s\n\nResult Log\n", width, height, density, population_size, children_size, mutation_chance,
		initialization_height, parsimony_pressure, maximum_stale_generations, parent_selection.c_str(), survival_selection.c_str(),
		tournament_size, seed, runs_value, evals_value, world_filename_value.c_str(),
		score_filename_value.c_str(), solution_filename_value.c_str());
	printf("Board width: %i\n"
		"Board height: %i\n"
		"Pill Density: %f\n"
		"Population Size: %i\n"
		"Children Size: %i\n"
		"Mutation Chance: %f\n"
		"Initialization Height: %i\n"
		"Parsimony Pressure: %f\n"
		"Maximum Stale Generations: %i\n"
		"Parent Selection Strategy: %s\n"
		"Survival Selection Strategy: %s\n"
		"Tournament Size: %i\n"
		"Random seed: %i\n"
		"Runs: %i\n"
		"Evals: %i\n"
		"World file: %s\n"
		"Score file: %s\n"
		"Solution file: %s\n\n", width, height, density, population_size, children_size, mutation_chance,
		initialization_height, parsimony_pressure, maximum_stale_generations, parent_selection.c_str(), survival_selection.c_str(),
		tournament_size, seed, runs_value, evals_value, world_filename_value.c_str(),
		score_filename_value.c_str(), solution_filename_value.c_str());
}
