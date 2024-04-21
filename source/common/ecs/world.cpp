#include "world.hpp"

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
            if (entityData.contains("is_wall") && entityData["is_wall"] == true)
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
        std::vector<std::vector<int>> maze = {{'|', '.', '-'},
                                              {'T', '-', 'T'},
                                              {'|', '.', '|'}};

        glm::vec3 initialPosition = glm::vec3(0, -1, 0);
        glm::vec3 initialRotation = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(0.1, 0.15, 0.1);

        glm::vec3 position;
        glm::vec3 rotation;

        int rotationY;
        for (int r = 0; r < maze.size(); r++)
        {
            for (int c = 0; c < maze[r].size(); c++)
            {
                if (maze[r][c] == '.')
                    continue;
                if (maze[r][c] != '|' && maze[r][c] != 'T' && maze[r][c] != '-')
                    continue;

                Entity *entity = add();
                entity->parent = nullptr;
                if (maze[r][c] == '-')
                    rotationY = 0;
                else if (maze[r][c] == '|' || maze[r][c] == 'T')
                    rotationY = 90;
                position = initialPosition + glm::vec3(20 * c, 0, (-20) * ((int)maze.size() - r - 1));
                std::cout << "position " << position.x << " ," << position.y << "," << position.z << std::endl;
                rotation = initialRotation + glm::vec3(0, rotationY, 0);
                entity->deserialize(wallData, position, rotation, scale);
                if (maze[r][c] == 'T')
                {
                    Entity *entity = add();
                    entity->parent = nullptr;
                    entity->deserialize(wallData, position, initialRotation, scale);
                }
            }
        }
    }

}
