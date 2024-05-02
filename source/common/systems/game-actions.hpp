#pragma once
#include <unordered_map>
#include <string>
#include<asset-loader.hpp>
namespace our
{
      enum class powerups{
            speedUp,
            gravityUp
        };
        struct powerupTimer{
           float speedup;
           float gravity;
        };

    // The sound system is responsible for sound accross the whole game.
   
    class GameActionsSystem {
        private:
        // the player current score
        static int score;
        static unsigned short int coins_collected;
        static unsigned short int powerups_collected;
        static unsigned short int keys_collected;
        static bool speedUp;

        // gravity powerup
        static bool gravityUp;
        static bool gravityDown;
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
        static void setGravityUp();
        static void resetGravityUp();
        static bool & getGravityUp();
        static void setGravityDown();
        static void resetGravityDown();
        static bool & getGravityDown();
        static void collectKey();
        static unsigned short int getKeysCollected();
    };
}