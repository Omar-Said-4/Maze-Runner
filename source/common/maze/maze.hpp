#pragma once
#include <vector>
#include <json/json.hpp>
#include <iostream>
namespace our
{

    class Maze
    {
        int rows, columns;                         // The no of rows and columns of the maze matrix
        std::vector<std::vector<char>> mazeMatrix; // The matrix that contains each wall in the maze
    public:
        void deserialize(const nlohmann::json &data);
        std::vector<std::vector<char>> getMazeMatrix()
        {
            return mazeMatrix;
        }
    };

}