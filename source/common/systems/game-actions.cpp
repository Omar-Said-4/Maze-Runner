#include "game-actions.hpp"
#include "../asset-loader.hpp"
#include "../maze/maze.hpp"
#include <iostream>
namespace our
{
    int GameActionsSystem::score = 0;
    unsigned short int GameActionsSystem::coins_collected = 0;
    unsigned short int GameActionsSystem::powerups_collected = 0;
    unsigned short int GameActionsSystem::keys_collected = 0;
    unsigned short int GameActionsSystem::total_coins = 0;
    unsigned short int GameActionsSystem::total_powerups = 0;
    unsigned short int GameActionsSystem::total_keys = 0;
    bool GameActionsSystem::speedUp = false;
    bool GameActionsSystem::gravityUp = false;
    bool GameActionsSystem::gravityDown = false;
    powerupTimer GameActionsSystem::powerupTimers = {0, 0, 0, 0};
    portalState GameActionsSystem::pS = portalState::off;
    bool GameActionsSystem::portal = false;
    bool GameActionsSystem::cantCollectMasterKey = false;
    bool GameActionsSystem::exitKey = false;
    bool GameActionsSystem::touchDoor = false;
    bool GameActionsSystem::openDoor = false;
    bool GameActionsSystem::flash = false;
    float GameActionsSystem::flashProgress = 1.0f;
    float GameActionsSystem::gravityx = 0;
    float GameActionsSystem::gravityz = 0;
    float GameActionsSystem::doorStartAngle = 90.0f;
    float GameActionsSystem::flashLightTimeOut;
    endState GameActionsSystem::end = endState::play;
    glm::vec3 GameActionsSystem::cameraPosition = glm::vec3(0, 0, 0);
    glm::vec3 GameActionsSystem::cameraRotation = glm::vec3(0, 0, 0);
    glm::vec3 GameActionsSystem::cameraScale = glm::vec3(1, 1, 1);
    void GameActionsSystem::loadFlashLightTimeOut()
    {
        flashLightTimeOut = our::AssetLoader<our::Maze>::get("maze")->getFlashLightTimeOut();
    }
    int GameActionsSystem::getScore()
    {
        return score;
    }
    void GameActionsSystem::collectCoin()
    {
        score += 100;
        coins_collected++;
    }
    void GameActionsSystem::resetScore()
    {
        score = 0;
        coins_collected = 0;
        speedUp = false;
        powerups_collected = 0;
        keys_collected = 0;
        gravityUp = false;
        gravityDown = false;
        pS = portalState::off;
        portal = false;
        powerupTimers = {0, 0, 0, 0};
        total_coins = 0;
        total_powerups = 0;
        total_keys = 0;
        cantCollectMasterKey = false;
        end = endState::play;
        exitKey = false;
        doorStartAngle = 90.0f;
        openDoor = false;
        flash = false;
        flashProgress = 1.0f;
    }
    unsigned short int GameActionsSystem::getCoinsCollected()
    {
        return coins_collected;
    }
    void GameActionsSystem::setSpeedUp()
    {
        speedUp = true;
    }
    void GameActionsSystem::resetSpeedUp()
    {
        speedUp = false;
    }
    bool &GameActionsSystem::getSpeedUp()
    {
        return speedUp;
    }
    unsigned short int GameActionsSystem::getPowerupsCollected()
    {
        return powerups_collected;
    }
    void GameActionsSystem::collectPowerup()
    {
        score += 500;
        powerups_collected++;
    }
    float &GameActionsSystem::getPowerupTimer(powerups powerup)
    {
        switch (powerup)
        {
        case powerups::speedUp:
            return powerupTimers.speedup;
            break;
        case powerups::gravityUp:
            return powerupTimers.gravity;
            break;
        case powerups::portal:
            break;
        case powerups::door:
            return powerupTimers.door;
            break;
        case powerups::flash:
            return powerupTimers.flash;
            break;
        default:
            break;
        }
    }
    void GameActionsSystem::resetPowerupTimer(powerups powerup)
    {
        switch (powerup)
        {
        case powerups::speedUp:
            powerupTimers.speedup = 0;
            break;
        case powerups::gravityUp:
            powerupTimers.gravity = 0;
            break;
        case powerups::portal:
            break;
        case powerups::door:
            powerupTimers.door = 0;
            break;
        case powerups::flash:
            powerupTimers.flash = 0;
            break;
        default:
            break;
        }
    }
    void GameActionsSystem::increasePowerupTimer(powerups powerup, float increase)
    {
        if (!increase)
        {
            return;
        }
        switch (powerup)
        {
        case powerups::speedUp:
            powerupTimers.speedup += increase;
            break;
        case powerups::gravityUp:
            powerupTimers.gravity += increase;
            break;
        case powerups::portal:
            break;
        case powerups::door:
            powerupTimers.door += increase;
            break;
        case powerups::flash:
            powerupTimers.flash += increase;
            break;
        default:
            break;
        }
    }
    void GameActionsSystem::setGravityUp()
    {
        gravityUp = true;
    }
    void GameActionsSystem::resetGravityUp()
    {
        gravityUp = false;
    }
    bool &GameActionsSystem::getGravityUp()
    {
        return gravityUp;
    }
    void GameActionsSystem::setGravityDown()
    {
        gravityDown = true;
    }
    void GameActionsSystem::resetGravityDown()
    {
        gravityDown = false;
    }
    bool &GameActionsSystem::getGravityDown()
    {
        return gravityDown;
    }
    void GameActionsSystem::collectKey()
    {
        score += 200;
        keys_collected++;
    }
    unsigned short int GameActionsSystem::getKeysCollected()
    {
        return keys_collected;
    }
    void GameActionsSystem::portalStateInc()
    {
        std::cout << pS << std::endl;

        pS = static_cast<portalState>((pS + 1) % 3);
        std::cout << pS << std::endl;
    }
    unsigned short int GameActionsSystem::getPortalState()
    {
        return pS;
    }
    bool &GameActionsSystem::getPortal()
    {
        portal = (pS > 0);
        return portal;
    }
    void GameActionsSystem::setTotalCoins(unsigned short int coins)
    {
        total_coins += coins;
    }
    void GameActionsSystem::setTotalPowerups(unsigned short int powerups)
    {
        total_powerups += powerups;
    }
    void GameActionsSystem::setTotalKeys(unsigned short int keys)
    {
        total_keys += keys;
    }
    unsigned short int GameActionsSystem::getTotalCoins()
    {
        return total_coins;
    }
    unsigned short int GameActionsSystem::getTotalPowerups()
    {
        return total_powerups;
    }
    unsigned short int GameActionsSystem::getTotalKeys()
    {
        return total_keys;
    }
    void GameActionsSystem::setCantCollectMasterKey()
    {
        cantCollectMasterKey = true;
    }
    void GameActionsSystem::resetCantCollectMasterKey()
    {
        cantCollectMasterKey = false;
    }
    bool &GameActionsSystem::getCantCollectMasterKey()
    {
        return cantCollectMasterKey;
    }
    void GameActionsSystem::collectExitKey()
    {
        score += 1000;
        exitKey = true;
    }
    bool &GameActionsSystem::getExitKey()
    {
        return exitKey;
    }
    bool &GameActionsSystem::getTouchDoor()
    {
        return touchDoor;
    }
    void GameActionsSystem::setTouchDoor()
    {
        touchDoor = true;
    }
    void GameActionsSystem::resetTouchDoor()
    {
        touchDoor = false;
    }
    bool &GameActionsSystem::getOpenDoor()
    {
        return openDoor;
    }
    void GameActionsSystem::setOpenDoor()
    {
        openDoor = true;
    }
    void GameActionsSystem::resetOpenDoor()
    {
        openDoor = false;
    }
    void GameActionsSystem::setGravity(float x, float z)
    {
        gravityx = x;
        gravityz = z;
    }
    float &GameActionsSystem::getGravityX()
    {
        return gravityx;
    }
    float &GameActionsSystem::getGravityZ()
    {
        return gravityz;
    }
    float &GameActionsSystem::getFlashProgress()
    {
        return flashProgress;
    }
    void GameActionsSystem::setFlashProgress(float fprogress)
    {
        flashProgress = fprogress;
    }

    bool &GameActionsSystem::getFlash()
    {
        return flash;
    }
    void GameActionsSystem::toggleFlash()
    {
        flash = !flash;
    }
    void GameActionsSystem::resetFlash()
    {
        flash = false;
    }
    void GameActionsSystem::setGameOver()
    {
        end = endState::lose;
    }
    void GameActionsSystem::setGameWin()
    {
        end = endState::win;
        score += 2000;
    }
    endState GameActionsSystem::getGameState()
    {
        return end;
    }
}