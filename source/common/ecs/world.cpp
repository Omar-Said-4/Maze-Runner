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
            if (entityData.contains("isMaze") && entityData["isMaze"] == true)
            {
                deserializeMaze(entityData);
                continue;
            }
            if (entityData.contains("isGround") && entityData["isGround"] == true)
            {
                deserializeGround(entityData);
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
    void World::deserializeMaze(const nlohmann::json &data)
    {
        Maze *maze = AssetLoader<Maze>::get("maze");
        std::vector<std::vector<char>> mazeMatrix = maze->getMazeMatrix();

        int cellSize = data.value("cellSize", 20);
        glm::vec3 initialPosition = data.value("position", glm::vec3(0, 0, 0));
        glm::vec3 initialRotation = data.value("rotation", glm::vec3(0, 0, 0));
        glm::vec3 scale = data.value("scale", glm::vec3(1, 1, 1));

        if (data.contains("isWall") && data["isWall"] == true)
            deserializeMazeWalls(data, initialPosition, initialRotation, scale, cellSize, mazeMatrix);
        else
            deserializeMazeObjects(data, initialPosition, initialRotation, scale, cellSize, mazeMatrix, 'o');
    }

    void World::deserializeMazeObjects(const nlohmann::json &objectData, glm::vec3 &initialPosition,
                                       glm::vec3 &rotation, glm::vec3 &scale, int &cellSize,
                                       std::vector<std::vector<char>> &mazeMatrix, char objectSymbol)
    {
        glm::vec3 position;
        for (int r = 0; r < mazeMatrix.size(); r++)
        {
            for (int c = 0; c < mazeMatrix[r].size(); c++)
            {
                if (mazeMatrix[r][c] != objectSymbol)
                    continue;

                Entity *entity = add();
                entity->parent = nullptr;
                position = initialPosition + glm::vec3(cellSize * c, 0, (-1 * cellSize) * ((int)mazeMatrix.size() - r - 1));
                entity->deserialize(objectData, position, rotation, scale);
            }
        }
    }

    void World::deserializeMazeWalls(const nlohmann::json &wallData, glm::vec3 &initialPosition,
                                     glm::vec3 &initialRotation, glm::vec3 &scale, int &cellSize,
                                     std::vector<std::vector<char>> &mazeMatrix)
    {
        glm::vec3 position;
        glm::vec3 rotation;
        int rotationY;
        for (int r = 0; r < mazeMatrix.size(); r++)
        {
            for (int c = 0; c < mazeMatrix[r].size(); c++)
            {
                if (mazeMatrix[r][c] != '|' && mazeMatrix[r][c] != 'T' && mazeMatrix[r][c] != '-')
                    continue;

                Entity *entity = add();
                entity->parent = nullptr;
                if (mazeMatrix[r][c] == '-')
                    rotationY = 0;
                else if (mazeMatrix[r][c] == '|' || mazeMatrix[r][c] == 'T')
                    rotationY = 90;
                position = initialPosition + glm::vec3(cellSize * c, 0, (-1 * cellSize) * ((int)mazeMatrix.size() - r - 1));
                rotation = initialRotation + glm::vec3(0, rotationY, 0);
                entity->deserialize(wallData, position, rotation, scale);
                if (mazeMatrix[r][c] == 'T')
                {
                    Entity *entity = add();
                    entity->parent = nullptr;
                    entity->deserialize(wallData, position, initialRotation, scale);
                }
            }
        }
    }

}
