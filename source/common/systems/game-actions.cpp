#include"game-actions.hpp"
#include<iostream>
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
    powerupTimer GameActionsSystem::powerupTimers={0,0};
    portalState GameActionsSystem::pS = portalState::off;
    bool GameActionsSystem::portal = false;
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
        pS = portalState::off;
        portal = false;
        powerupTimers = {0,0};
        total_coins = 0;
        total_powerups = 0;
        total_keys = 0;
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
        case powerups::portal:
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
        case powerups::portal:
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
    void GameActionsSystem::portalStateInc()
    {       std::cout<<pS<<std::endl;

       pS = static_cast<portalState>((pS + 1) % 3);
       std::cout<<pS<<std::endl;
    }
    unsigned short int GameActionsSystem::getPortalState()
    {
        return pS;
    }
    bool &GameActionsSystem::getPortal()
    {
        portal = (pS>0);
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
}