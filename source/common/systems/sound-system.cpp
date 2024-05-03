#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include"sound-system.hpp"
#include"game-actions.hpp"
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


void SoundSystem::initMap(){
    std::unordered_map<std::string, ma_sound*> tempMap = AssetLoader<ma_sound>::getMap();
    for(auto& [name, sound] : tempMap){
        SoundSystem::Audios[name] = sound;
    }
}
 void SoundSystem::initMenuSounds()
 {
        ma_sound* sound = new ma_sound();
        SoundSystem::init_audio(sound, "assets/audio/Menu.mp3");
        SoundSystem::Audios["Menu"] = sound;
        ma_sound* sound2 = new ma_sound();
        SoundSystem::init_audio(sound2, "assets/audio/button.wav");
        SoundSystem::Audios["Button"] = sound2;
        ma_sound* sound3 = new ma_sound();
        SoundSystem::init_audio(sound3, "assets/audio/hover.wav");
        SoundSystem::Audios["Hover"] = sound3;
 }
  void SoundSystem::initScoreSounds()
 {
        ma_sound* sound = new ma_sound();
        if(our::GameActionsSystem::getGameState()==endState::win){
            SoundSystem::init_audio(sound, "assets/audio/win.mp3");
        }
        else
        {
            SoundSystem::init_audio(sound, "assets/audio/lose.mp3");
        }
        SoundSystem::Audios["Score"] = sound;
        ma_sound* sound2 = new ma_sound();
        SoundSystem::init_audio(sound2, "assets/audio/button.wav");
        SoundSystem::Audios["Button"] = sound2;
        ma_sound* sound3 = new ma_sound();
        SoundSystem::init_audio(sound3, "assets/audio/hover.wav");
        SoundSystem::Audios["Hover"] = sound3;
 }
 void SoundSystem::destroyScoreSounds()
 {
    // delete score audio
    our::SoundSystem::stop_custom_sound("Score");
     ma_sound_uninit(SoundSystem::Audios["Score"]);
     if(SoundSystem::Audios["Score"] != nullptr)
       delete SoundSystem::Audios["Score"];
     SoundSystem::Audios["Score"] = nullptr;
    // delete button audio
    ma_sound_uninit(SoundSystem::Audios["Button"]);
     if(SoundSystem::Audios["Button"] != nullptr)
       delete SoundSystem::Audios["Button"];
     SoundSystem::Audios["Button"] = nullptr;
     // delete hover audio
    ma_sound_uninit(SoundSystem::Audios["Hover"]);
     if(SoundSystem::Audios["Hover"] != nullptr)
       delete SoundSystem::Audios["Hover"];
     SoundSystem::Audios["Hover"] = nullptr;
 }
 void SoundSystem::destroyMenuSounds()
 {
    // delete mmenu audio
    our::SoundSystem::stop_custom_sound("Menu");
     ma_sound_uninit(SoundSystem::Audios["Menu"]);
     if(SoundSystem::Audios["Menu"] != nullptr)
       delete SoundSystem::Audios["Menu"];
     SoundSystem::Audios["Menu"] = nullptr;
    // delete button audio
    ma_sound_uninit(SoundSystem::Audios["Button"]);
     if(SoundSystem::Audios["Button"] != nullptr)
       delete SoundSystem::Audios["Button"];
     SoundSystem::Audios["Button"] = nullptr;
     // delete hover audio
    ma_sound_uninit(SoundSystem::Audios["Hover"]);
     if(SoundSystem::Audios["Hover"] != nullptr)
       delete SoundSystem::Audios["Hover"];
     SoundSystem::Audios["Hover"] = nullptr;
 }
    void SoundSystem::play_custom_sound(std::string sound_name, bool wait_tell_finish,bool loop){
        if (ma_sound_is_playing(SoundSystem::Audios[sound_name])&& !wait_tell_finish && ! sound_name.compare("Hover")==0) {
        return; // Exit the function if the sound is already playing
    }
        ma_sound_seek_to_pcm_frame(SoundSystem::Audios[sound_name], 0);
        ma_sound_start(SoundSystem::Audios[sound_name]);
        if(loop){
            ma_sound_set_looping(SoundSystem::Audios[sound_name], true);
        }
        else{
            ma_sound_set_looping(SoundSystem::Audios[sound_name], false);
        }
        if(wait_tell_finish){
            while (ma_sound_is_playing(SoundSystem::Audios[sound_name]));
        }

    }
    void SoundSystem::stop_custom_sound(std::string sound_name){
        ma_sound_stop(SoundSystem::Audios[sound_name]);
    }
    void SoundSystem::destroy_sounds(){
        for(auto& [name, sound] : SoundSystem::Audios){
            ma_sound_uninit(sound);
        }
    }

}

