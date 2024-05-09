#pragma once
#include <unordered_map>
#include <string>
#include<asset-loader.hpp>
#include <glm/glm.hpp>
namespace our
{
      enum class powerups{
            speedUp,
            gravityUp,
            portal,
            door,
            flash
        };
        // only one portal per game in a chosen position
        enum  portalState{
            off = 0,
            inwall = 1,
            outwall = 2 
        };
        enum class endState{
            win,
            lose,
            play
        };
        struct powerupTimer{
           float speedup;
           float gravity;
           float door;
           float flash;
        };

    // The sound system is responsible for sound accross the whole game.
   
    class GameActionsSystem {
        private:
        // the player current score
        static unsigned short int total_coins;
        static unsigned short int total_powerups;
        static unsigned short int total_keys;
        static int score;
        static unsigned short int coins_collected;
        static unsigned short int powerups_collected;
        static unsigned short int keys_collected;
        static bool speedUp;
        static bool cantCollectMasterKey;
        static bool exitKey;
        static bool touchDoor;
        static bool openDoor;
        // gravity powerup
        static bool gravityUp;
        static bool gravityDown;
        static float gravityx;
        static float gravityz;

        // portal powerup
        static portalState pS;
        static powerupTimer powerupTimers;
        static bool portal;
         
        // flashlight control
        static bool flash;
        // flashlight progress
        static float flashProgress;
        

        static endState end;
        public:
        static float flashLightTimeOut;
        // camera parameters
        static glm::vec3 cameraPosition;
        static glm::vec3 cameraRotation;
        static glm::vec3 cameraScale;
        static float doorStartAngle;
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
        static void portalStateInc();
        static unsigned short int getPortalState();
        static bool & getPortal();
        static void setTotalCoins(unsigned short int coins);
        static void setTotalPowerups(unsigned short int powerups);
        static void setTotalKeys(unsigned short int keys);
        static unsigned short int getTotalCoins();
        static unsigned short int getTotalPowerups();
        static unsigned short int getTotalKeys();
        static void setCantCollectMasterKey();
        static void resetCantCollectMasterKey();
        static bool & getCantCollectMasterKey();
        static void collectExitKey();
        static bool & getExitKey();
        static void setTouchDoor();
        static void resetTouchDoor();
        static bool & getTouchDoor();
        static float & getGravityX();
        static float & getGravityZ();
        static void setGravity(float x,float z);
        static void setOpenDoor();
        static void resetOpenDoor();
        static bool & getOpenDoor();
        static bool &getFlash();
        static void toggleFlash();
        static void resetFlash();
        static float & getFlashProgress();
        static void setFlashProgress(float fprogress);
        static void loadFlashLightTimeOut();

        static void setGameOver();
        static void setGameWin();
        static endState getGameState();
    };
}