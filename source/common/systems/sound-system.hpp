#pragma once
#include <miniaudio.h>
#include <unordered_map>
#include <string>
#include<asset-loader.hpp>
namespace our
{

    // The sound system is responsible for sound accross the whole game.
   
    class SoundSystem {
        private:
        // the sound engine
                static ma_engine engine;
                static std::unordered_map<std::string, ma_sound*> Audios;
        public:
                 // background music audio state
                static bool global_music_state;
                // start the audio engine
                static void init_engine();
                // to be deserialized
                static void init_audio(ma_sound*sound,const char * path);
                static void initMap();
                // destroy the audio engine
                static void deinit_engine();
                static void play_menu_background();
                static void stop_menu_background();
                static void initMenuSounds();
                static void destroyMenuSounds();
                static void play_custom_sound(std::string sound_name,bool wait_tell_finish=true);

    };
}