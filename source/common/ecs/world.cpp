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
            if (entityData.contains("isWall") && entityData["isWall"] == true)
            {
                deseralizeWalls(entityData);
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
    void World::deseralizeWalls(const nlohmann::json &wallData)
    {
        Maze *maze = AssetLoader<Maze>::get("maze");
        std::vector<std::vector<char>> mazeMatrix = maze->getMazeMatrix();

        int cellSize = wallData.value("cellSize", 20);
        glm::vec3 initialPosition = wallData.value("position", glm::vec3(0, 0, 0));
        glm::vec3 initialRotation = wallData.value("rotation", glm::vec3(0, 0, 0));
        glm::vec3 scale = wallData.value("scale", glm::vec3(1, 1, 1));

        glm::vec3 position;
        glm::vec3 rotation;

        int rotationY;
        for (int r = 0; r < mazeMatrix.size(); r++)
        {
            for (int c = 0; c < mazeMatrix[r].size(); c++)
            {
                if (mazeMatrix[r][c] == '.')
                    continue;
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
