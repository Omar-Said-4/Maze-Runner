#pragma once

#include "maze.hpp"
#include <string>
#include <iostream>
#include <fstream>

namespace our::maze_utils
{
    Maze *loadMaze(const std::string &filename);
}