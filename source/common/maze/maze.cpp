#include "maze.hpp"

void our::Maze::deserialize(const nlohmann::json &data)
{
    if (!data.is_object())
        return;
    rows = data.value("rows", rows);
    columns = data.value("columns", columns);
    mazeTimeInSec = data.value("mazeTimeInSec", 120);
    flashLightTimeOut = data.value("flashLightTimeOut", 40.0f);
    if (data.find("matrix") != data.end())
    {
        std::vector<std::vector<char>> tmpMazeMatrix;
        mazeMatrix.clear();

        // if there is no camera or exit key add them
        bool hasCamera=false;
        bool hasExitKey=false;
        for (const auto &row : data["matrix"])
        {
            std::vector<char> temp_row;
            for (const auto &cell : row)
            {
                temp_row.push_back(cell.get<std::string>()[0]); // Convert string to char
                if (cell.get<std::string>()[0] == 'c') // Check if cell is 'c'
                    hasCamera = true;
                if (cell.get<std::string>()[0] == 'm') // Check if cell is 'm'
                    hasExitKey = true;
            }
            tmpMazeMatrix.push_back(temp_row);
        }
        // pad with walls
        int paddedRows = rows + 2;
        int paddedColumns = columns + 2;
        std::vector<std::vector<char>> paddedMatrix(paddedRows, std::vector<char>(paddedColumns, ' '));
        for (int j = 1; j < paddedColumns-1; ++j) {
        paddedMatrix[0][j] = '-';
        paddedMatrix[paddedRows - 1][j] = '-';
       }
        for (int i = 0; i < paddedRows; ++i) {
        paddedMatrix[i][0] = '|';
        paddedMatrix[i][paddedColumns - 1] = '|';
      }
        for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
        paddedMatrix[i + 1][j + 1] = tmpMazeMatrix[i][j];
      }
        }
       // door place is constant
       paddedMatrix[1][0]='d';
       paddedMatrix[1][1]='.';
       mazeMatrix=paddedMatrix;

       // default positions if don't exist
       if(!hasCamera)
       {
           mazeMatrix[1][2]='c';
       }
        if(!hasExitKey)
        {
            mazeMatrix[2][2]='m';
        }



    }
}