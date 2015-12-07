#pragma once

#include <cstdio>

#include "TreePretty.h"
#include "PacmanIndividual.h"
#include "GhostIndividual.h"

inline void print_args(FILE* score_file,
	int width, int height, double density,
	int population_size_pacman, int children_size_pacman, double mutation_chance_pacman, int initialization_height_pacman,
	double parsimony_pressure_pacman, std::string parent_selection_pacman, std::string survival_selection_pacman, int tournament_size_pacman,
	int population_size_ghost, int children_size_ghost, double mutation_chance_ghost, int initialization_height_ghost,
	double parsimony_pressure_ghost, std::string parent_selection_ghost, std::string survival_selection_ghost, int tournament_size_ghost,
	int seed, int runs_value, int evals_value,
	std::string world_filename_value, std::string score_filename_value, std::string solution_filename_value)
{
	fprintf(score_file, "Board width: %i\n"
		"Board height: %i\n"
		"Pill Density: %f\n"
		"Population Size Pacman: %i\n"
		"Children Size Pacman: %i\n"
		"Mutation Chance Pacman: %f\n"
		"Initialization Height Pacman: %i\n"
		"Parsimony Pressure Pacman: %f\n"
		"Parent Selection Strategy Pacman: %s\n"
		"Survival Selection Strategy Pacman: %s\n"
		"Tournament Size Pacman: %i\n"
		"Population Size Ghost: %i\n"
		"Children Size Ghost: %i\n"
		"Mutation Chance Ghost: %f\n"
		"Initialization Height Ghost: %i\n"
		"Parsimony Pressure Ghost: %f\n"
		"Parent Selection Strategy Ghost: %s\n"
		"Survival Selection Strategy Ghost: %s\n"
		"Tournament Size Ghost: %i\n"
		"Random seed: %i\n"
		"Runs: %i\n"
		"Evals: %i\n"
		"World file: %s\n"
		"Score file: %s\n"
		"Solution file: %s\n\nResult Log\n", width, height, density,
		population_size_pacman, children_size_pacman, mutation_chance_pacman, initialization_height_pacman, parsimony_pressure_pacman,
		parent_selection_pacman.c_str(), survival_selection_pacman.c_str(), tournament_size_pacman,
		population_size_ghost, children_size_ghost, mutation_chance_ghost, initialization_height_ghost, parsimony_pressure_ghost,
		parent_selection_ghost.c_str(), survival_selection_ghost.c_str(), tournament_size_ghost,
		seed, runs_value, evals_value, world_filename_value.c_str(), score_filename_value.c_str(), solution_filename_value.c_str());
	printf("Board width: %i\n"
		"Board height: %i\n"
		"Pill Density: %f\n"
		"Population Size Pacman: %i\n"
		"Children Size Pacman: %i\n"
		"Mutation Chance Pacman: %f\n"
		"Initialization Height Pacman: %i\n"
		"Parsimony Pressure Pacman: %f\n"
		"Parent Selection Strategy Pacman: %s\n"
		"Survival Selection Strategy Pacman: %s\n"
		"Tournament Size Pacman: %i\n"
		"Population Size Ghost: %i\n"
		"Children Size Ghost: %i\n"
		"Mutation Chance Ghost: %f\n"
		"Initialization Height Ghost: %i\n"
		"Parsimony Pressure Ghost: %f\n"
		"Parent Selection Strategy Ghost: %s\n"
		"Survival Selection Strategy Ghost: %s\n"
		"Tournament Size Ghost: %i\n"
		"Random seed: %i\n"
		"Runs: %i\n"
		"Evals: %i\n"
		"World file: %s\n"
		"Score file: %s\n"
		"Solution file: %s\n\n", width, height, density,
		population_size_pacman, children_size_pacman, mutation_chance_pacman, initialization_height_pacman, parsimony_pressure_pacman,
		parent_selection_pacman.c_str(), survival_selection_pacman.c_str(), tournament_size_pacman,
		population_size_ghost, children_size_ghost, mutation_chance_ghost, initialization_height_ghost, parsimony_pressure_ghost,
		parent_selection_ghost.c_str(), survival_selection_ghost.c_str(), tournament_size_ghost,
		seed, runs_value, evals_value, world_filename_value.c_str(), score_filename_value.c_str(), solution_filename_value.c_str());
}

inline std::string individual_string(const PacmanIndividual& pacman)
{
	return Brain::ToEquation(&pacman.pacman_controller.root) + " {" + Brain::to_s_expression(&pacman.pacman_controller.root) + "}";
}

inline std::string individual_string(const GhostIndividual& pacman)
{
	return Brain::ToEquation(&pacman.ghost_controller.root) + " {" + Brain::to_s_expression(&pacman.ghost_controller.root) + "}";
}

inline void write_match(std::mt19937& random, std::string filename, const std::string& pacman_string, const std::string& ghost_string)
{
	PacmanIndividual pacman;
	GhostIndividual ghost;
	pacman.pacman_controller.root.children.emplace_back(Brain::from_s_expression(random, pacman_string));
	ghost.ghost_controller.root.children.emplace_back(Brain::from_s_expression(random, ghost_string));
	Game::fight(random, pacman, ghost);
	FILE* file = fopen(filename.c_str(), "w");
	fprintf(file, "%s", pacman.games[0]->buffer.get());
	fclose(file);
}
