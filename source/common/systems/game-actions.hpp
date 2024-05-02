#pragma once
#include <unordered_map>
#include <string>
#include<asset-loader.hpp>
namespace our
{
      enum class powerups{
            speedUp
        };
        struct powerupTimer{
           float speedup;
        };

    // The sound system is responsible for sound accross the whole game.
   
    class GameActionsSystem {
        private:
        // the player current score
        static int score;
        static unsigned short int coins_collected;
        static unsigned short int powerups_collected;
        static bool speedUp;
        static powerupTimer powerupTimers;
        public:
        // get the player current score
        static int getScore();
        // player collected a coin
        static void collectCoin();
        // reset player score
        static void resetScore();
        static unsigned short int getCoinsCollected();
        static void setSpeedUp();
        static void resetSpeedUp();
        static bool & getSpeedUp();
        static unsigned short int getPowerupsCollected();
        static void collectPowerup();
        static float& getPowerupTimer(powerups powerup);
        static void resetPowerupTimer(powerups powerup);
        static void increasePowerupTimer(powerups powerup,float increase);
    };
}