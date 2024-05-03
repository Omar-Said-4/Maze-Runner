#include "world.hpp"
#include "../deserialize-utils.hpp"
#include "../systems/game-actions.hpp"

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
            if (entityData.contains("isCamera") && entityData["isCamera"] == true)
            {
                deserializeCamera(entityData);
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
    void World::deserializeCamera(const nlohmann::json &cameraData)
    {
        mazeCellSize = cameraData.value("cellSize", mazeCellSize);
        if (mazeObjects.size() == 0)
        {
            loadMazeObjects();
        }
        Entity *entity = add();
        entity->parent = nullptr;
        glm::vec3 rotation = cameraData.value("rotation", glm::vec3(0, 0, 0));
        glm::vec3 scale = cameraData.value("scale", glm::vec3(1, 1, 1));
        glm::vec3 position = glm::vec3(mazeCellSize * mazeObjects['c'][0].second, 2, (-1 * mazeCellSize) * (numOfMazeRows - mazeObjects['c'][0].first - 1));
        entity->deserialize(cameraData, position, rotation, scale);
        
        // init system camera return position
        our::GameActionsSystem::cameraPosition = position;
        our::GameActionsSystem::cameraRotation = rotation;
        our::GameActionsSystem::cameraScale = scale;
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
    void World::loadMazeObjects()
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

    std::pair<int, int> World::getXBordersOfMaze()
    {
        return {-7, mazeCellSize * numOfMazeColumns - 7};
    }

    std::pair<int, int> World::getZBordersOfMaze()
    {
        return {7, -1 * mazeCellSize * numOfMazeRows +7};
    }

    void World::deserializeMaze(const nlohmann::json &data)
    {
        mazeCellSize = data.value("cellSize", mazeCellSize);
        if (mazeObjects.size() == 0)
        {
            loadMazeObjects();
        }

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
            if (symbol == 'p' || symbol == 'r' || symbol == 's')
            {
                our::GameActionsSystem::setTotalPowerups(mazeObjects[symbol].size());
            }
            else if (symbol == 'k')
            {
                our::GameActionsSystem::setTotalKeys(mazeObjects[symbol].size());
            }
            else if (symbol == 'o')
            {
                our::GameActionsSystem::setTotalCoins(mazeObjects[symbol].size());
            }
            else if(symbol =='c')
            {
                // no double cameras
                int size = mazeObjects[symbol].size();
               if(size > 1)
               {
                 
                 // only keep the first camera and get the rest cameras positions
                // for(int i = 1; i < size; i++)
                // {
                //     mazeObjects['.'].push_back({mazeObjects[symbol][i].first,mazeObjects[symbol][i].second});
                // }
                  mazeObjects['.'].insert(mazeObjects['.'].end(), mazeObjects[symbol].begin() + 1, mazeObjects[symbol].end());
                  mazeObjects[symbol].erase(mazeObjects[symbol].begin()+1,mazeObjects[symbol].end());
               }
            }
            for (int i = 0; i < mazeObjects[symbol].size(); i++)
            {
                r = mazeObjects[symbol][i].first;
                c = mazeObjects[symbol][i].second;
                Entity *entity = add();
                entity->parent = nullptr;
                position = initialPosition + glm::vec3(mazeCellSize * c, 0, (-1 * mazeCellSize) * (numOfMazeRows - r - 1));
                entity->deserialize(data, position, rotation, scale);
            }
        }
    }

}
