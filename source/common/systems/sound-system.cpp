#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include"sound-system.hpp"
#include <cstdio>
#include<string>
namespace our{



bool SoundSystem::global_music_state = false;
ma_engine SoundSystem::engine;
std::unordered_map<std::string, ma_sound*> SoundSystem::Audios;
void SoundSystem::init_engine(){
    ma_result result;
    result = ma_engine_init(NULL, &SoundSystem::engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.\n");
    }
    else{
        printf("Audio engine initialized successfully.\n");
    }
}
void SoundSystem::deinit_engine(){
    ma_engine_uninit(&SoundSystem::engine);
    printf("Audio engine deinitialized.\n");
}
void SoundSystem::init_audio(ma_sound*sound,const char * path){
     ma_sound_init_from_file(&SoundSystem::engine, path, 0, NULL, NULL, sound);
}
void SoundSystem::play_menu_background(){
         ma_sound_start(SoundSystem::Audios["Menu"]);
}
void SoundSystem::stop_menu_background(){
         ma_sound_stop(SoundSystem::Audios["Menu"]);
        //ma_sound_uninit(SoundSystem::Audios["Menu"]);
        }

void SoundSystem::initMap(){
    ma_sound* sound = AssetLoader<ma_sound>::get("Menu");
    SoundSystem::Audios["Menu"] = sound;
}
 void SoundSystem::initMenuSound()
 {
        ma_sound* sound = new ma_sound();
        SoundSystem::init_audio(sound, "assets/audio/Menu.mp3");
        SoundSystem::Audios["Menu"] = sound;
        ma_sound_set_looping(SoundSystem::Audios["Menu"], true);
 }
 void SoundSystem::destroyMenuSound()
 {
     SoundSystem::stop_menu_background();
     ma_sound_uninit(SoundSystem::Audios["Menu"]);
     if(SoundSystem::Audios["Menu"] != nullptr)
       delete SoundSystem::Audios["Menu"];
     SoundSystem::Audios["Menu"] = nullptr;
 }

}

