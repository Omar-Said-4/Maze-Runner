#include "maze_utils.hpp"

our::Maze *our::maze_utils::loadMaze(const std::string &filename)
{
    //std::cout << "zzzz" << std::endl;

    std::ifstream file_in(filename);
    if (!file_in)
    {
        std::cerr << "Couldn't open file: " << filename << std::endl;
        return nullptr;
    }
    // Read the file into a json object then close the file
    nlohmann::json data = nlohmann::json::parse(file_in, nullptr, true, true);
    file_in.close();
    //std::cout << "yyyy" << std::endl;

    Maze *maze = new Maze();
    maze->deserialize(data);

    return maze;
}