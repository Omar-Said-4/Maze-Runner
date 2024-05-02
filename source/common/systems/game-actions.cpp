#include"game-actions.hpp"
namespace our
{
    int GameActionsSystem::score = 0;
    unsigned short int GameActionsSystem::coins_collected = 0;
    unsigned short int GameActionsSystem::powerups_collected = 0;
    unsigned short int GameActionsSystem::keys_collected = 0;
    bool GameActionsSystem::speedUp = false;
    bool GameActionsSystem::gravityUp = false;
    bool GameActionsSystem::gravityDown = false;
    powerupTimer GameActionsSystem::powerupTimers={0,0};
    int GameActionsSystem::getScore()
    {
        return score;
    }
    void GameActionsSystem::collectCoin()
    {
        score+=100;
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
    }
    unsigned short int  GameActionsSystem::getCoinsCollected()
    {
        return coins_collected;
    }
    void GameActionsSystem::setSpeedUp()
    {
        speedUp = true;
    }
    void GameActionsSystem::resetSpeedUp()
    {
        speedUp=false;
    }
    bool & GameActionsSystem::getSpeedUp()
    {
        return speedUp;
    }
    unsigned short int GameActionsSystem::getPowerupsCollected()
    {
        return powerups_collected;
    }
    void GameActionsSystem::collectPowerup()
    {
        score+=500;
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
        default:
            break;
        }
    }
    void GameActionsSystem::increasePowerupTimer(powerups powerup,float increase)
    {
        if (!increase)
        {
            return;
        }
        switch (powerup)
        {
        case powerups::speedUp:
            powerupTimers.speedup+=increase;
            break;
        case powerups::gravityUp:
            powerupTimers.gravity+=increase;
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
        gravityUp=false;
    }
    bool & GameActionsSystem::getGravityUp()
    {
        return gravityUp;
    }
    void GameActionsSystem::setGravityDown()
    {
        gravityDown = true;
    }
    void GameActionsSystem::resetGravityDown()
    {
        gravityDown=false;
    }
    bool & GameActionsSystem::getGravityDown()
    {
        return gravityDown;
    }
    void GameActionsSystem::collectKey()
    {
        score+=200;
        keys_collected++;
    }
    unsigned short int GameActionsSystem::getKeysCollected()
    {
        return keys_collected;
    }
}