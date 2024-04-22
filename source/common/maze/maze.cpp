#include "maze.hpp"

void our::Maze::deserialize(const nlohmann::json &data)
{
    if (!data.is_object())
        return;
    rows = data.value("rows", rows);
    columns = data.value("columns", columns);
    if (data.find("matrix") != data.end())
    {
        mazeMatrix.clear();
        for (const auto &row : data["matrix"])
        {
            std::vector<char> temp_row;
            for (const auto &cell : row)
            {
                temp_row.push_back(cell.get<std::string>()[0]); // Convert string to char
            }
            mazeMatrix.push_back(temp_row);
        }
    }
}