#include "world.hpp"
#include "../deserialize-utils.hpp"

namespace our
{

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json &data, Entity *parent)
    {
        if (!data.is_array())
            return;
        for (const auto &entityData : data)
        {
            if (entityData.contains("isGround") && entityData["isGround"] == true)
            {
                deserializeGround(entityData);
                continue;
            }
            if (entityData.contains("isMaze") && entityData["isMaze"] == true)
            {
                deserializeMaze(entityData);
                continue;
            }
            // TODO: (Req 8) Create an entity, make its parent "parent" and call its deserialize with "entityData".
            Entity *entity = add();
            entity->parent = parent;
            entity->deserialize(entityData);
            if (entityData.contains("children"))
            {
                // TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                //  and the current entity as the parent
                deserialize(entityData["children"], entity);
            }
        }
    }
    void World::deserializeGround(const nlohmann::json &groundData)
    {
        int rows = groundData.value("rows", 10);
        int columns = groundData.value("columns", 10);
        int cellSize = groundData.value("cellSize", 20);
        glm::vec3 initialPosition = groundData.value("position", glm::vec3(0, 0, 0));
        glm::vec3 rotation = groundData.value("rotation", glm::vec3(0, 0, 0));
        glm::vec3 scale = groundData.value("scale", glm::vec3(1, 1, 1));
        glm::vec3 position;
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < columns; c++)
            {
                Entity *entity = add();
                entity->parent = nullptr;
                position = initialPosition + glm::vec3(cellSize * c, 0, (-1 * cellSize) * (rows - r - 1));
                entity->deserialize(groundData, position, rotation, scale);
            }
        }
    }
    void World::loadMazeObjects(const nlohmann::json &data)
    {
        Maze *maze = AssetLoader<Maze>::get("maze");
        std::vector<std::vector<char>> mazeMatrix = maze->getMazeMatrix();
        numOfMazeRows = mazeMatrix.size();
        numOfMazeColumns = mazeMatrix[0].size();
        for (int r = 0; r < mazeMatrix.size(); r++)
        {
            for (int c = 0; c < mazeMatrix[r].size(); c++)
            {
                mazeObjects[mazeMatrix[r][c]].push_back({r, c});
            }
        }
    }

    void World::deserializeMaze(const nlohmann::json &data)
    {
        if (mazeObjects.size() == 0)
        {
            loadMazeObjects(data);
        }

        int cellSize = data.value("cellSize", 20);
        std::unordered_set<char> objectSymbols;
        if (data.contains("objectSymbol"))
        {
            for (const auto &cell : data["objectSymbol"])
                objectSymbols.insert(cell.get<std::string>()[0]);
        }
        glm::vec3 initialPosition = data.value("position", glm::vec3(0, 0, 0));
        glm::vec3 rotation = data.value("rotation", glm::vec3(0, 0, 0));
        glm::vec3 scale = data.value("scale", glm::vec3(1, 1, 1));

        glm::vec3 position;
        int r, c;
        for (const auto &symbol : objectSymbols)
        {
            for (int i = 0; i < mazeObjects[symbol].size(); i++)
            {
                r = mazeObjects[symbol][i].first;
                c = mazeObjects[symbol][i].second;
                Entity *entity = add();
                entity->parent = nullptr;
                position = initialPosition + glm::vec3(cellSize * c, 0, (-1 * cellSize) * (numOfMazeRows - r - 1));
                entity->deserialize(data, position, rotation, scale);
            }
        }
    }

}
