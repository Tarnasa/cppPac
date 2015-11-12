CPPPAC
This is the source code for a Genetic Programming Co-Evolutionary Algorithm
  which seeks to optimize the controllers behind both Ms. Pacman and the ghosts
  In a simulated world of the Ms. Pacman game.

  
FILE STRUCTURE:
./ - Where you are now
  /config/ - Contains all the default configuration files for use with cppPac
  /source/ - Contains all source code needed to build cppPac
  /dependencies/ - Third-party code required to build cppPac
  /output/ - Contains the results of running the configuration files, along with graphs


COMPILING:
Use CMake to build the executable:

mkdir build
cd build
cmake ..
cmake --build .
	
The executable will be in the /build folder


NOTE:
If running on school computers, CMake does not seem to be set up correctly
  so instead just run this gcc command from the project root directory:
  
g++ -std=c++11 -I dependencies/tclap-1.2.1/include/ source/*.cpp -o cppPac


RUNNING:
To run the default configuration

cppPac @config/default.cfg

This will create default.score and default.world in the /output/ folder
- default.score contains the a list <eval><tab><fitness> pairs for each run
  along with the configuration parameters used to generate this file.
- default.world contains a description of the world and actions taken by
  Pacman and the ghosts.


MORE HELP:
To get more information on how to use cppPac, run cppPac --help


CONFIGURATIONS:
A number of pre-made configuration files have been placed in /config/
name       - width, height, density
small.cfg  - 10, 10, 0.5
medium.cfg - 30, 30, 0.2
large.cfg  - 40, 50, 0.7
huge.cfg   - 80, 80, 0.3
The corresponding output files are located in /output/
The corresponding graphs of these outputs are also located in /output/
