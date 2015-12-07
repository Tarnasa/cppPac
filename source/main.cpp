/// The main entry point into the EA
///

#include <cstdio>
#include <random>
#include <time.h>
#include <iostream>

#include "tclap/CmdLine.h"

#include "MainHelpers.h"
#include "ArgFilePopulator.h"
#include "Game.h"
#include "Helpers.h"
#include "Node.h"
#include "TreePretty.h"
#include "Initializers.h"
#include "Parenting.h"
#include "Survival.h"
#include "GhostIndividual.h"

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

		TCLAP::MultiArg<int> arg_populate_size_pacman("", "pacman_population", "The number of individuals kept after each generation for pacman", false, "Integer");
		TCLAP::MultiArg<int> arg_children_size_pacman("", "pacman_children", "The number of new individuals created during recombination for pacman", false, "Integer");
		TCLAP::MultiArg<double> arg_mutation_chance_pacman("", "pacman_mutation", "The chance that the children will be mutated for pacman", false, "Decimal between 0 and 1");
		TCLAP::MultiArg<int> arg_initialization_height_pacman("", "pacman_levels", "The maximum number of levels of the starting population for pacman", false, "Integer");
		TCLAP::MultiArg<double> arg_parsimony_pressure_pacman("", "pacman_parsimony", "f'(x) = f(x) + maximum_possible_fitness - parsimony_pressure * (# of nodes) for pacman", false, "Decimal");
		//TCLAP::MultiArg<int> arg_maximum_stale_generations_pacman("", "pacman_converge", "The number of non-improving best fitness generations before the run is terminated (-1 to not use) for pacman", false, "Integer");
		TCLAP::MultiArg<int> arg_history_depth_pacman("", "pacman_history", "How many heros of the past each pacman will fight", false, "Integer");

		TCLAP::MultiArg<std::string> arg_parent_selection_pacman("", "pacman_parent", "The method used to select parents for pacman", false, "One of [fps, overselection]");
		TCLAP::MultiArg<std::string> arg_survival_selection_pacman("", "pacman_survival", "The method used to select survivors for pacman", false, "One of [truncation, ktourn]");
		TCLAP::MultiArg<int> arg_tournament_size_pacman("", "pacman_tournamentsize", "The tournament size if kTournament is used for pacman", false, "Integer");

		TCLAP::MultiArg<int> arg_populate_size_ghost("", "ghost_population", "The number of individuals kept after each generation for ghosts", false, "Integer");
		TCLAP::MultiArg<int> arg_children_size_ghost("", "ghost_children", "The number of new individuals created during recombination for ghosts", false, "Integer");
		TCLAP::MultiArg<double> arg_mutation_chance_ghost("", "ghost_mutation", "The chance that the children will be mutated for ghosts", false, "Decimal between 0 and 1");
		TCLAP::MultiArg<int> arg_initialization_height_ghost("", "ghost_levels", "The maximum number of levels of the starting population for ghosts", false, "Integer");
		TCLAP::MultiArg<double> arg_parsimony_pressure_ghost("", "ghost_parsimony", "f'(x) = f(x) + maximum_possible_fitness - parsimony_pressure * (# of nodes) for ghosts", false, "Decimal");
		//TCLAP::MultiArg<int> arg_maximum_stale_generations_ghost("", "ghost_converge", "The number of non-improving best fitness generations before the run is terminated (-1 to not use) for ghosts", false, "Integer");
		TCLAP::MultiArg<int> arg_history_depth_ghost("", "ghost_history", "How many heros of the past each ghost will fight", false, "Integer");

		TCLAP::MultiArg<std::string> arg_parent_selection_ghost("a", "ghost_parent", "The method used to select parents for ghosts", false, "One of [fps, overselection]");
		TCLAP::MultiArg<std::string> arg_survival_selection_ghost("i", "ghost_survival", "The method used to select survivors for ghosts", false, "One of [truncation, ktourn]");
		TCLAP::MultiArg<int> arg_tournament_size_ghost("k", "ghost_tournamentsize", "The tournament size if kTournament is used for ghosts", false, "Integer");

		TCLAP::MultiArg<int> arg_random_seed("s", "seed", "A number to seed the random number generator with, or -1 to seed with current time.", false, "Integer");
		TCLAP::MultiArg<int> arg_runs("r", "runs", "The number of runs.", false, "Integer");
		TCLAP::MultiArg<int> arg_evals("e", "evals", "The maximum number of evaluations that each run can take.", false, "Integer");

		TCLAP::MultiArg<std::string> arg_world_filename("o", "worldfile", "Path to the world log file to be created.", false, "File path");
		TCLAP::MultiArg<std::string> arg_score_filename("c", "scorefile", "Path to score log file to be created.", false, "File path");
		TCLAP::MultiArg<std::string> arg_solution_filename("u", "solutionfile", "Path to solution file to be created.", false, "File path");
		
		cmd.add(arg_board_width);
		cmd.add(arg_board_height);
		cmd.add(arg_pill_density);

		cmd.add(arg_populate_size_pacman);
		cmd.add(arg_children_size_pacman);
		cmd.add(arg_mutation_chance_pacman);
		cmd.add(arg_initialization_height_pacman);
		cmd.add(arg_parsimony_pressure_pacman);
		//cmd.add(arg_maximum_stale_generations_pacman);
		cmd.add(arg_history_depth_pacman);

		cmd.add(arg_parent_selection_pacman);
		cmd.add(arg_survival_selection_pacman);
		cmd.add(arg_tournament_size_pacman);

		cmd.add(arg_populate_size_ghost);
		cmd.add(arg_children_size_ghost);
		cmd.add(arg_mutation_chance_ghost);
		cmd.add(arg_initialization_height_ghost);
		cmd.add(arg_parsimony_pressure_ghost);
		//cmd.add(arg_maximum_stale_generations_ghost);
		cmd.add(arg_history_depth_ghost);

		cmd.add(arg_parent_selection_ghost);
		cmd.add(arg_survival_selection_ghost);
		cmd.add(arg_tournament_size_ghost);

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
		int height = arg_board_height.getValue().size() ? arg_board_height.getValue().back() : 20;
		double density = arg_pill_density.getValue().size() ? arg_pill_density.getValue().back() : 0.5;

		int population_size_pacman = arg_populate_size_pacman.getValue().size() ? arg_populate_size_pacman.getValue().back() : 50;
		int children_size_pacman = arg_children_size_pacman.getValue().size() ? arg_children_size_pacman.getValue().back() : 100;
		double mutation_chance_pacman = arg_mutation_chance_pacman.getValue().size() ? arg_mutation_chance_pacman.getValue().back() : 0.05;
		int initialization_height_pacman = arg_initialization_height_pacman.getValue().size() ? arg_initialization_height_pacman.getValue().back() : 5;
		double parsimony_pressure_pacman = arg_parsimony_pressure_pacman.getValue().size() ? arg_parsimony_pressure_pacman.getValue().back() : 0.15;
		//int maximum_stale_generations_pacman = arg_maximum_stale_generations_pacman.getValue().size() ? arg_maximum_stale_generations_pacman.getValue().back() : -1;
		int history_depth_pacman = arg_history_depth_pacman.getValue().size() ? arg_history_depth_pacman.getValue().back() : 5;
		
		std::string parent_selection_pacman = arg_parent_selection_pacman.getValue().size() ? arg_parent_selection_pacman.getValue().back() : "overselection";
		std::string survival_selection_pacman = arg_survival_selection_pacman.getValue().size() ? arg_survival_selection_pacman.getValue().back() : "truncation";
		int tournament_size_pacman = arg_tournament_size_pacman.getValue().size() ? arg_tournament_size_pacman.getValue().back() : 5;

		int population_size_ghost = arg_populate_size_ghost.getValue().size() ? arg_populate_size_ghost.getValue().back() : 50;
		int children_size_ghost = arg_children_size_ghost.getValue().size() ? arg_children_size_ghost.getValue().back() : 40;
		double mutation_chance_ghost = arg_mutation_chance_ghost.getValue().size() ? arg_mutation_chance_ghost.getValue().back() : 0.05;
		int initialization_height_ghost = arg_initialization_height_ghost.getValue().size() ? arg_initialization_height_ghost.getValue().back() : 5;
		double parsimony_pressure_ghost = arg_parsimony_pressure_ghost.getValue().size() ? arg_parsimony_pressure_ghost.getValue().back() : 0.15;
		//int maximum_stale_generations_ghost = arg_maximum_stale_generations_ghost.getValue().size() ? arg_maximum_stale_generations_ghost.getValue().back() : -1;
		int history_depth_ghost = arg_history_depth_ghost.getValue().size() ? arg_history_depth_ghost.getValue().back() : 5;

		std::string parent_selection_ghost = arg_parent_selection_ghost.getValue().size() ? arg_parent_selection_ghost.getValue().back() : "overselection";
		std::string survival_selection_ghost = arg_survival_selection_ghost.getValue().size() ? arg_survival_selection_ghost.getValue().back() : "ktourn";
		int tournament_size_ghost = arg_tournament_size_ghost.getValue().size() ? arg_tournament_size_ghost.getValue().back() : 5;

		int random_seed_value = arg_random_seed.getValue().size() ? arg_random_seed.getValue().back() : -1;
		int runs_value = arg_runs.getValue().size() ? arg_runs.getValue().back() : 30;
		int evals_value = arg_evals.getValue().size() ? arg_evals.getValue().back() : 2000;
		
		std::string world_filename_value = arg_world_filename.getValue().size() ? arg_world_filename.getValue().back() : "default.world";
		std::string score_filename_value = arg_score_filename.getValue().size() ? arg_score_filename.getValue().back() : "default.score";
		std::string solution_filename_value = arg_solution_filename.getValue().size() ? arg_solution_filename.getValue().back() : "default.solution";

		// Treat negative as no convergence termination criteria
		//if (maximum_stale_generations_pacman <= 0) maximum_stale_generations_pacman = std::numeric_limits<int>::max();
		//if (maximum_stale_generations_ghost <= 0) maximum_stale_generations_ghost = std::numeric_limits<int>::max();
		// Time limit is visiting every square twice
		int time_limit = width * height * 2;
		// Use random seed if given value is negative
		int seed = static_cast<int>(random_seed_value < 0 ? time(nullptr) : random_seed_value);
		std::mt19937 random(seed);
		// Choose selectors
		enum ParentSelector { FPS, OVERSELECTION } parent_selector_pacman = FPS, parent_selector_ghost = FPS;
		enum SurvivalSelector { TRUNCATION, KTOURNAMENT } survival_selector_pacman = TRUNCATION, survival_selector_ghost = TRUNCATION;
		if (parent_selection_pacman == "overselection") parent_selector_pacman = OVERSELECTION;
		if (survival_selection_pacman == "ktourn") survival_selector_pacman = KTOURNAMENT;
		if (parent_selection_ghost == "overselection") parent_selector_ghost = OVERSELECTION;
		if (survival_selection_ghost == "ktourn") survival_selector_ghost = KTOURNAMENT;

		// Calculate maximum length of buffer needed to store log file for a single eval
		int length_of_step = get_printf_length(128, "m %i %i\n", width, height) * 4 + get_printf_length(128, "t %i %i\n", time_limit, width * height);
		int buffer_size = 0;
		buffer_size += get_printf_length(128, "%i\n%i\n", width, height);
		buffer_size += get_printf_length(128, "p %i %i\n", width, height) * (width * height);
		buffer_size += length_of_step * time_limit;

		// Send global parameters to individual TODO: Get rid of these
		Game::width = width;
		Game::height = height;
		Game::density = density;
		Game::time_limit = time_limit;
		Game::buffer_size = buffer_size;

		// Initialize neighbor cache
		GameState::InitializeNeighbors(width, height);

		// IOStreams are slow, just use c-style file io
		FILE* world_file = fopen(world_filename_value.c_str(), "w");
		FILE* score_file = fopen(score_filename_value.c_str(), "w");
		FILE* solution_file = fopen(solution_filename_value.c_str(), "w");

		// Log and print out configuration options
		print_args(score_file, 
			width, height, density, population_size_pacman, children_size_pacman, mutation_chance_pacman,
			initialization_height_pacman, parsimony_pressure_pacman, parent_selection_pacman, survival_selection_pacman,
			tournament_size_pacman, population_size_ghost, children_size_ghost, mutation_chance_ghost,
			initialization_height_ghost, parsimony_pressure_ghost, parent_selection_ghost, survival_selection_ghost,
			tournament_size_ghost, seed, runs_value, evals_value, world_filename_value,
			score_filename_value, solution_filename_value);

		// List of Halls of Fame (one for each run)
		typedef std::pair<PacmanIndividual, GhostIndividual> Pair;
		typedef std::list<Pair> HallOfFame;
		std::list<HallOfFame> halls_of_fame;

		// Keep track of best overall individuals
		PacmanIndividual best_pacman;
		best_pacman.fitness = 0;
		PacmanIndividual best_ghost;
		best_pacman.fitness = 0;

		// --- TEST AREA ---

		//write_match(random, world_filename_value, "-(-(+(-(E, c0.134598), /(c0.931440, D)), /(R(/(R(c0.444404, G), D), G), /(c0.474273, D))), /(R(c0.444404, G), +(D, D)))", "-(c0.107393, p)");
		//return 0;

		// --- TEST AREA ---

		// Run the EA!
		for (int run = 0; run < runs_value; ++run)
		{
			// Create a hall of fame for this run
			halls_of_fame.emplace_back();
			HallOfFame& hall_of_fame = halls_of_fame.back();

			// Keep track of best individuals for the run
			PacmanIndividual best_run_pacman;
			best_run_pacman.fitness = 0;
			PacmanIndividual best_run_ghost;
			best_run_ghost.fitness = 0;

			// Termination checkers
			int evals = 0;

			// Initialize the set of individuals
			auto pacmans = Initializers::RampedHalfAndHalf<PacmanIndividual>(random, population_size_pacman, initialization_height_pacman, parsimony_pressure_pacman);
			auto ghosts = Initializers::RampedHalfAndHalf<GhostIndividual>(random, population_size_ghost, initialization_height_ghost, parsimony_pressure_ghost);
			delete pacmans[0].pacman_controller.root.children[0];
			pacmans[0].pacman_controller.root.children[0] = Brain::from_s_expression(random, "/(D, G)");
			delete ghosts[0].ghost_controller.root.children[0];
			ghosts[0].ghost_controller.root.children[0] = Brain::from_s_expression(random, "R(c0.5, g)");
			// Evaluate them
			int larger = pacmans.size() > ghosts.size() ? pacmans.size() : ghosts.size();
			for (int i = 0; i < larger; ++i)
			{
				int pacman_index = static_cast<int>((i * pacmans.size()) / larger);
				int ghost_index = static_cast<int>((i * ghosts.size()) / larger);
				Game::fight(random, pacmans[pacman_index], ghosts[ghost_index]);
				evals += 1;
			}

			// Sort individuals, highest fitness first
			std::sort(pacmans.begin(), pacmans.end(), [&](const PacmanIndividual& a, const PacmanIndividual& b) { return a.fitness > b.fitness; });
			std::sort(ghosts.begin(), ghosts.end(), [&](const GhostIndividual& a, const GhostIndividual& b) { return a.fitness > b.fitness; });
			
			// Record best for hall of fame
			hall_of_fame.emplace_back(pacmans[0], ghosts[0]);

			printf("Run %i...\n", run + 1);
			fprintf(score_file, "\nRun %i\n", run + 1);
			while (evals < evals_value)
			{
				// Generate parent pairs
				std::vector<std::vector<int>> pacman_parent_indices;
				if (parent_selector_pacman == FPS)
					pacman_parent_indices = Parenting::FPS<PacmanIndividual>(random, pacmans, children_size_pacman / 2);
				else
					pacman_parent_indices = Parenting::overselection<PacmanIndividual>(random, pacmans, children_size_pacman / 2);

				std::vector<std::vector<int>> ghost_parent_indices;
				if (parent_selector_ghost == FPS)
					ghost_parent_indices = Parenting::FPS<GhostIndividual>(random, ghosts, children_size_ghost / 2);
				else
					ghost_parent_indices = Parenting::overselection<GhostIndividual>(random, ghosts, children_size_ghost / 2);

				// Generate children from pairs of parents
				auto pacman_children = Parenting::generate_children(random, pacmans, pacman_parent_indices);
				auto ghost_children = Parenting::generate_children(random, ghosts, ghost_parent_indices);

				// Randomly apply mutation to children
				
				for (auto&& child : pacman_children)
				{
					if (chance(random, mutation_chance_pacman))
						Brain::mutate(random, &child.pacman_controller.root, initialization_height_pacman, false);
				}
				for (auto&& child : ghost_children)
				{
					if (chance(random, mutation_chance_ghost))
						Brain::mutate(random, &child.ghost_controller.root, initialization_height_ghost, true);
				}

				// Evaluate children by fighting them against a random individual from the previous generation
				for (auto&& pacman_child : pacman_children)
				{
					Game::fight(random, pacman_child, choice(random, ghosts));
					evals += 1;

					// Also fight child against hall of famers
					int heros_remaining = history_depth_pacman;
					for (auto&& riter = hall_of_fame.rbegin(); riter != hall_of_fame.rend(); ++riter)
					{
						Game::fight(random, pacman_child, riter->second);
						evals += 1;
						if (--heros_remaining <= 0) break;
					}
				}
				for (auto&& ghost_child : ghost_children)
				{
					Game::fight(random, choice(random, pacmans), ghost_child);
					evals += 1;

					// Also fight child against hall of famers
					int heros_remaining = history_depth_ghost;
					for (auto&& riter = hall_of_fame.rbegin(); riter != hall_of_fame.rend(); ++riter)
					{
						Game::fight(random, riter->first, ghost_child);
						evals += 1;
						if (--heros_remaining <= 0) break;
					}
				}
				// Clear out hero's memories
				for (auto&& pair : hall_of_fame)
				{
					pair.first.games.clear();
					pair.second.games.clear();
				}

				// Sort children, highest fitness first
				std::sort(pacman_children.begin(), pacman_children.end(), [&](const PacmanIndividual& a, const PacmanIndividual& b) { return a.fitness > b.fitness; });
				std::sort(ghost_children.begin(), ghost_children.end(), [&](const GhostIndividual& a, const GhostIndividual& b) { return a.fitness > b.fitness; });

				// Check for improvement, Record best child
				if (pacman_children[0].fitness > best_run_pacman.fitness)
				{
					best_run_pacman = pacman_children[0];
				}

				// Merge children into individuals
				pacmans.reserve(pacmans.size() + pacman_children.size());
				std::move(std::begin(pacman_children), std::end(pacman_children), std::back_inserter(pacmans));
				ghosts.reserve(ghost_children.size() + ghost_children.size());
				std::move(std::begin(ghost_children), std::end(ghost_children), std::back_inserter(ghosts));

				// Sort individuals, highest fitness first
				std::sort(pacmans.begin(), pacmans.end(), [&](const PacmanIndividual& a, const PacmanIndividual& b) { return a.fitness > b.fitness; });
				std::sort(ghosts.begin(), ghosts.end(), [&](const GhostIndividual& a, const GhostIndividual& b) { return a.fitness > b.fitness; });

				// Choose survivors from combined individuals + children pool
				if (survival_selector_pacman == TRUNCATION)
					Survival::truncate<PacmanIndividual>(pacmans, population_size_pacman);
				else
					Survival::kTournament<PacmanIndividual>(random, pacmans, population_size_pacman, tournament_size_pacman);

				if (survival_selector_ghost == TRUNCATION)
					Survival::truncate<GhostIndividual>(ghosts, population_size_ghost);
				else
					Survival::kTournament<GhostIndividual>(random, ghosts, population_size_ghost, tournament_size_ghost);

				// Record best for hall of fame
				hall_of_fame.emplace_back(pacmans[0], ghosts[0]);

				double average_pacman_fitness = average(pacmans, [&](const PacmanIndividual& i) {return i.game_fitness; });
				double average_ghost_fitness = average(ghosts, [&](const GhostIndividual& i) {return i.fitness; });
				fprintf(score_file, "%i\t%f\t%f\n", evals, average_pacman_fitness, best_run_pacman.game_fitness);

				printf("pacman: %f, ghost: %f\n", average_pacman_fitness, average_ghost_fitness);
				std::cout << "Best Pacman: " << individual_string(pacmans[0]) << "\n";
				std::cout << "Best Ghost: " << individual_string(ghosts[0]) << "\n";
			} // Finished with run

			// Ignore best individual of entire run and just find best individual of final population
			if (pacmans[0].fitness > best_pacman.fitness)
			{
				best_pacman = pacmans[0];
			}
		} // Finished with EA

		// Choose most representative game of best_individual
		double best_difference = std::numeric_limits<double>::max();
		std::shared_ptr<Game> best_game;
		for (auto&& game : best_pacman.games)
		{
			double difference = abs(game->fitness - best_pacman.game_fitness);
			if (difference < best_difference)
			{
				difference = best_difference;
				best_game = game;
			}
		}

		fclose(score_file);
		fprintf(world_file, "%s", best_game->buffer.get());
		fclose(world_file);
		fprintf(solution_file, "%f : %s\n", best_pacman.game_fitness, Brain::ToEquation(&best_pacman.pacman_controller.root).c_str());
		fprintf(solution_file, "Most representative game opponent:\n%s\n%s\n",
			Brain::ToEquation(&best_game->ghost_controller.root).c_str(),
			Brain::to_s_expression(&best_game->ghost_controller.root).c_str());
		fclose(solution_file);

		std::cout << "Games: " << best_pacman.games.size() <<
			"\nPacman: " << Brain::ToEquation(&best_pacman.pacman_controller.root) <<
			"\nGhost: " << Brain::ToEquation(&best_game->ghost_controller.root) << "\n";

		puts("Done!\n");
	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}

    return 0; // Program ran successfully
}
