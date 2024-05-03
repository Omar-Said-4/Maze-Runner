#include "application.hpp"
#include<systems/game-actions.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <queue>
#include <tuple>
#include <filesystem>
#include<stb_image.h>
#include <flags/flags.h>
#include<texture/texture2d.hpp>
// Include the Dear ImGui implementation headers
#define IMGUI_IMPL_OPENGL_LOADER_GLAD2
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>
#include<systems/sound-system.hpp>
#if !defined(NDEBUG)
// If NDEBUG (no debug) is not defined, enable OpenGL debug messages
#define ENABLE_OPENGL_DEBUG_MESSAGES
#define GAME_OVER_TIME_SECS 120
#endif

#include "texture/screenshot.hpp"

std::string default_screenshot_filepath() {
    std::stringstream stream;
    auto time = std::time(nullptr);
    
    struct tm localtime;
    localtime_s(&localtime, &time);
    stream << "screenshots/screenshot-" << std::put_time(&localtime, "%Y-%m-%d-%H-%M-%S") << ".png";
    return stream.str();
}

// This function will be used to log errors thrown by GLFW
void glfw_error_callback(int error, const char* description){
    std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
}

// This function will be used to log OpenGL debug messages
void GLAPIENTRY opengl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::string _source;
    std::string _type;
    std::string _severity;

    // What is the source of the message
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY"; break;
        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION"; break;
        case GL_DEBUG_SOURCE_OTHER: default:
            _source = "UNKNOWN"; break;
    }

    // What is the type of the message (error, warning, etc).
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR"; break;
        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE"; break;
        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER"; break;
        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER"; break;
        default:
            _type = "UNKNOWN"; break;
    }

    // How severe is the message
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM"; break;
        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION"; break;
        default:
            _severity = "UNKNOWN"; break;
    }

    std::cout << "OpenGL Debug Message " << id << " (type: " << _type << ") of " << _severity
    << " raised from " << _source << ": " << message << std::endl;
}

void our::Application::configureOpenGL() {
    // Request that OpenGL is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Only enable core functionalities (disable features from older OpenGL versions that were removed in 3.3)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Enable forward compatibility with newer OpenGL versions by removing deprecated functionalities
    // This is necessary for some platforms
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Make window size fixed (User can't resize it)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    //Set Number of sample used in MSAA (0 = Disabled)
    glfwWindowHint(GLFW_SAMPLES, 8);

    //Enable Double Buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    //Set the bit-depths of the frame buffer
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    //Set Bits for Depth Buffer
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    //Set Bits for Stencil Buffer
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    //Set the refresh rate of the window (GLFW_DONT_CARE = Run as fast as possible)
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    // disable window bar
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // make it start at the center of the screen
    glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
}

our::WindowConfiguration our::Application::getWindowConfiguration() {
    auto window_config = app_config["window"];
    std::string title = window_config["title"].get<std::string>();
    
    int width = window_config["size"]["width"].get<int>();
    int height = window_config["size"]["height"].get<int>();

    bool isFullScreen = window_config["fullscreen"].get<bool>();

    return {title, {width, height}, isFullScreen};
}

// This is the main class function that run the whole application (Initialize, Game loop, House cleaning).
// run_for_frames decides how many frames should be run before the application automatically closes.
// if run_for_frames == 0, the application runs indefinitely till manually closed.
int our::Application::run(int run_for_frames) {
    // Initialize the audio engine
    our::SoundSystem::init_engine();
    // Set the function to call when an error occurs.
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW and exit if it failed
    if(!glfwInit()){
        std::cerr << "Failed to Initialize GLFW" << std::endl;
        return -1;
    }
  
    configureOpenGL(); // This function sets OpenGL window hints.
    
    auto win_config = getWindowConfiguration();             // Returns the WindowConfiguration current struct instance.

    // Create a window with the given "WindowConfiguration" attributes.
    // If it should be fullscreen, monitor should point to one of the monitors (e.g. primary monitor), otherwise it should be null
    GLFWmonitor* monitor = win_config.isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
    // The last parameter "share" can be used to share the resources (OpenGL objects) between multiple windows.
    window = glfwCreateWindow(win_config.size.x, win_config.size.y, win_config.title.c_str(), monitor, nullptr);
    if(!window) {
        std::cerr << "Failed to Create Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    GLFWimage images[1]; 

    // change window icon
    images[0].pixels = stbi_load("assets/images/icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(window, 1, images); 
    stbi_image_free(images[0].pixels);
    images[0].pixels = stbi_load("assets/images/cursor.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    

    // Create a custom cursor from the image data
    GLFWcursor* cursor = glfwCreateCursor(&images[0], 23, 23);
if (!cursor) {
    // Handle error, e.g., log the error or use a default cursor
    std::cerr << "Failed to create cursor from image" << std::endl;
}

    // Set the cursor
    glfwSetCursor(window, cursor);

    // Free the image data
    stbi_image_free(images[0].pixels);
    glfwMakeContextCurrent(window);         // Tell GLFW to make the context of our window the main context on the current thread.

   
    // Calculate the window position to center it on the screen
    
    // Set the window position
   // glfwSetWindowPos(window, 100, 100);

    gladLoadGL(glfwGetProcAddress);         // Load the OpenGL functions from the driver

    // Print information about the OpenGL context
    std::cout << "VENDOR          : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "RENDERER        : " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "VERSION         : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL VERSION    : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


#if defined(ENABLE_OPENGL_DEBUG_MESSAGES)
    // if we have OpenGL debug messages enabled, set the message callback
    glDebugMessageCallback(opengl_callback, nullptr);
    // Then enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    // Then make the output synchronized to the OpenGL commands.
    // This will make sure that OpenGL and the main thread are synchronized such that message callback is called as soon
    // as the command causing it is called. This is useful for debugging but slows down the code execution.
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

    setupCallbacks();
    keyboard.enable(window);
    mouse.enable(window);

    // Start the ImGui context and set dark style (just my preference :D)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImFont *playFont = io.Fonts->AddFontFromFileTTF("assets/fonts/chary.ttf", 30.0f);
    ImFont *powerupFont = io.Fonts->AddFontFromFileTTF("assets/fonts/HalloweenSlimePersonalUse.otf", 40.0f);

    // for hover effect
    ImVec4 textColorCollect = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    ImVec4 textColorTimer = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    ImGui::StyleColorsDark();

    // Initialize ImGui for GLFW and OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // This part of the code extracts the list of requested screenshots and puts them into a priority queue
    using ScreenshotRequest = std::pair<int, std::string>;
    std::priority_queue<
        ScreenshotRequest, 
        std::vector<ScreenshotRequest>, 
        std::greater<ScreenshotRequest>> requested_screenshots;
    if(auto& screenshots = app_config["screenshots"]; screenshots.is_object()) {
        auto base_path = std::filesystem::path(screenshots.value("directory", "screenshots"));
        if(auto& requests = screenshots["requests"]; requests.is_array()) {
            for(auto& item : requests){
                auto path = base_path / item.value("file", "");
                int frame = item.value("frame", 0);
                requested_screenshots.push({ frame, path.string() });
            }
        }
    }

    // If a scene change was requested, apply it
    if(nextState) {
        currentState = nextState;
        nextState = nullptr;
    }
    // Call onInitialize if the scene needs to do some custom initialization (such as file loading, object creation, etc).
    if(currentState) currentState->onInitialize();
    // The time at which the last frame started. But there was no frames yet, so we'll just pick the current time.
    double last_frame_time = glfwGetTime();
    int current_frame = 0;

    //Game loop
    while(!glfwWindowShouldClose(window)){
        // game over timer
        static float game_over_timer=0;
        // time to display in any state
        unsigned char display_time=GAME_OVER_TIME_SECS + 1-game_over_timer;
        std::ostringstream mins_stream, secs_stream;
        mins_stream << std::setw(2) << std::setfill('0') << std::to_string(display_time/60);
        secs_stream << std::setw(2) << std::setfill('0') << std::to_string(display_time%60);
        std::string timer_display = mins_stream.str() + ":" + secs_stream.str();
        if(run_for_frames != 0 && current_frame >= run_for_frames) break;
        glfwPollEvents(); // Read all the user events and call relevant callbacks.

        // Start a new ImGui frame
                ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();    

        if(currentState) currentState->onImmediateGui(); // Call to run any required Immediate GUI.

        // If ImGui is using the mouse or keyboard, then we don't want the captured events to affect our keyboard and mouse objects.
        // For example, if you're focusing on an input and writing "W", the keyboard object shouldn't record this event.
        keyboard.setEnabled(!io.WantCaptureKeyboard, window);
        mouse.setEnabled(!io.WantCaptureMouse, window);

        if(currentState==states["play"]){
            // increment the timer
            game_over_timer+=ImGui::GetIO().DeltaTime;
           // collectable sub window
            {
            
            ImGui::SetNextWindowSize(ImVec2(300, 200));
            ImGui::PushStyleColor(ImGuiCol_Text, textColorCollect);
            // start window GUI
            ImGui::Begin("COLLECTED", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
            ImGui::SetWindowPos("COLLECTED", ImVec2(10, 10));
            ImVec2 collectWindowPos = ImGui::GetWindowPos();
            ImVec2 collectWindowSize = ImGui::GetWindowSize();
            float collectWindowLeft = collectWindowPos.x;
            float collectWindowRight = collectWindowPos.x + collectWindowSize.x;
            float collectWindowTop = collectWindowPos.y;
            float collectWindowBottom = collectWindowPos.y + collectWindowSize.y;
            
            // Mouse position
            ImVec2 mousePos = ImGui::GetIO().MousePos;

            // Check if mouse is within CollectWindow boundaries
            if (mousePos.x >= collectWindowLeft && mousePos.x <= collectWindowRight &&
                mousePos.y >= collectWindowTop && mousePos.y <= collectWindowBottom) {
                // Increase the opacity of the text when the mouse is hovering over TimerWindow
                textColorCollect.w = 1.0f; // Set alpha component to 1.0 for full opacity
            } else {
                // Decrease the opacity of the text when the mouse is not hovering over TimerWindow
                textColorCollect.w = 0.5f; // Set alpha component to 0.5 for half opacity
            }
            // set the position of the Mission mini sub window

            ImGuiStyle *style = &ImGui::GetStyle();

            ImVec4 *colors = style->Colors;

            // set the color for window background
            colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ScrollbarBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);

            ImGui::PushFont(playFont);
            // write collectables
            std::string string1 = "COINS: ";
            std::string coins = std::to_string(our::GameActionsSystem::getCoinsCollected());
            std::string stringLine1 = string1 + coins + "/" + std::to_string(our::GameActionsSystem::getTotalCoins());

            std::string string2 = "KEYS: ";
            std::string keys = std::to_string(our::GameActionsSystem::getKeysCollected());
            std::string stringLine2 = string2 + keys + "/" + std::to_string(our::GameActionsSystem::getTotalKeys());


            std::string string3 = "EXIT KEY: ";
            std::string exit_key;//
            if(our::GameActionsSystem::getExitKey())
            {
                exit_key = "COLLECTED";
            }
            else
            {
               exit_key = "NOT YET";
            }
            std::string stringLine3 = string3 + exit_key;
            // write it in the window
            ImGui::Text(stringLine1.c_str());
            ImGui::Text(stringLine2.c_str());
            ImGui::Text(stringLine3.c_str());

            // pop the font after writing
            ImGui::PopFont();            
            ImGui::PopStyleColor();
            ImGui::End();
            }
            
            // timer sub window
            {
            ImGui::SetNextWindowSize(ImVec2(300, 200));
            ImGui::PushStyleColor(ImGuiCol_Text, textColorTimer);
            // start window GUI
            ImGui::Begin("TIMER", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
            // set the position of the Mission mini sub window
            ImGui::SetWindowPos("TIMER", ImVec2(win_config.size.x-100, 10));
            ImVec2 timerWindowPos = ImGui::GetWindowPos();
            ImVec2 timerWindowSize = ImGui::GetWindowSize();
            float timerWindowLeft = timerWindowPos.x;
            float timerWindowRight = timerWindowPos.x + timerWindowSize.x;
            float timerWindowTop = timerWindowPos.y;
            float timerWindowBottom = timerWindowPos.y + timerWindowSize.y;

            // Mouse position
            ImVec2 mousePos = ImGui::GetIO().MousePos;

            // Check if mouse is within TimerWindow boundaries
            if (mousePos.x >= timerWindowLeft && mousePos.x <= timerWindowRight &&
                mousePos.y >= timerWindowTop && mousePos.y <= timerWindowBottom) {
                // Increase the opacity of the text when the mouse is hovering over TimerWindow
                textColorTimer.w = 1.0f; // Set alpha component to 1.0 for full opacity
            } else {
                // Decrease the opacity of the text when the mouse is not hovering over TimerWindow
                textColorTimer.w = 0.5f; // Set alpha component to 0.5 for half opacity
            }
            

            ImGui::PushFont(playFont);
            // write collectables
            // Time is Up
            if(display_time==0){
                our::GameActionsSystem::setGameOver();
                currentState->getApp()->changeState("score");
            }
            if(our::GameActionsSystem::getGameState()==endState::win){
                currentState->getApp()->changeState("score");
            }
           
            
            
            // write it in the window
            ImGui::Text(timer_display.c_str());

            // pop the font after writing
            ImGui::PopFont();                      
            ImGui::PopStyleColor();
            ImGui::End();
            }
            // powerup popup message
            const float messageDuration1 = 8.0f; // Duration of the message in seconds
            if (our::GameActionsSystem::getSpeedUp()) {
                ImGui::PushFont(powerupFont);
                ImGui::Begin("POWERUP", &(our::GameActionsSystem::getSpeedUp()), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
                ImGui::SetWindowPos("POWERUP", ImVec2(win_config.size.x/2-250, 30));
                ImGui::Text("SPEED UP POWERUP ACQUIRED !");
                ImGui::PopFont();                      
                ImGui::End();

                // Update message timer
                our::GameActionsSystem::increasePowerupTimer(our::powerups::speedUp,ImGui::GetIO().DeltaTime);

                // If message duration elapsed, hide the message
                if (our::GameActionsSystem::getPowerupTimer(our::powerups::speedUp) >= messageDuration1) {
                    our::GameActionsSystem::resetSpeedUp();
                    our::GameActionsSystem::resetPowerupTimer(our::powerups::speedUp); // Reset the timer for future messages
                }
            }
            const float messageDuration2 = 5.0f; // Duration of the message in seconds
            if(our::GameActionsSystem::getGravityUp())
            {
                ImGui::PushFont(powerupFont);
                ImGui::Begin("POWERUP2", &(our::GameActionsSystem::getGravityUp()), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
                ImGui::SetWindowPos("POWERUP2", ImVec2(win_config.size.x/2-250, 200));
                ImGui::Text("GRAVITY POWERUP ACQUIRED !");
                ImGui::PopFont();                      
                ImGui::End();

                // Update message timer
                our::GameActionsSystem::increasePowerupTimer(our::powerups::gravityUp,ImGui::GetIO().DeltaTime);

                // If message duration elapsed, hide the message
                if (our::GameActionsSystem::getPowerupTimer(our::powerups::gravityUp) >= messageDuration2) {
                    our::GameActionsSystem::resetGravityUp();
                    our::GameActionsSystem::setGravityDown();
                    our::GameActionsSystem::resetPowerupTimer(our::powerups::gravityUp); 
                }
            }
            if(our::GameActionsSystem::getGravityDown())
            {
                our::GameActionsSystem::increasePowerupTimer(our::powerups::gravityUp,ImGui::GetIO().DeltaTime);

                // If message duration elapsed, hide the message
                if (our::GameActionsSystem::getPowerupTimer(our::powerups::gravityUp) >= messageDuration2) {
                    our::GameActionsSystem::resetGravityDown();
                    our::GameActionsSystem::resetPowerupTimer(our::powerups::gravityUp); 
                }
            }
            if(our::GameActionsSystem::getPortal())
            {
                ImGui::PushFont(powerupFont);
                ImGui::Begin("POWERUP3", &(our::GameActionsSystem::getPortal()), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
                ImGui::SetWindowPos("POWERUP3", ImVec2(win_config.size.x/2-250, 80));
                ImGui::Text("PORTAL POWERUP ACQUIRED");
                ImGui::PopFont();                      
                ImGui::End();
            }
            
            const float messageDuration3 = 2.0f; // Duration of the message in seconds
            static float timer_alert=0;
            if(our::GameActionsSystem::getCantCollectMasterKey())
            {
                timer_alert+=ImGui::GetIO().DeltaTime;
                ImGui::PushFont(powerupFont);
                ImGui::Begin("ALERT", &(our::GameActionsSystem::getCantCollectMasterKey()), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
                ImGui::SetWindowPos("ALERT", ImVec2(win_config.size.x/2-250, 500));
                ImGui::Text("Get THE REMAINING KEYS FIRST");
                ImGui::PopFont();                      
                ImGui::End();
                 if (timer_alert >= messageDuration3) {
                    timer_alert=0;
                    our::GameActionsSystem::resetCantCollectMasterKey();
                }
            }
            static float timer_alert2=0;
            if(our::GameActionsSystem::getTouchDoor())
            {
                timer_alert2+=ImGui::GetIO().DeltaTime;
                ImGui::PushFont(powerupFont);
                ImGui::Begin("ALERT2", &(our::GameActionsSystem::getTouchDoor()), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
                ImGui::SetWindowPos("ALERT2", ImVec2(win_config.size.x/2-250, 400));
                ImGui::Text("Get THE EXIT KEY FIRST");
                ImGui::PopFont();                      
                ImGui::End();
                 if (timer_alert2 >= messageDuration3) {
                    timer_alert2=0;
                    our::GameActionsSystem::resetTouchDoor();
                }
            }
            const float openDoorTimer=5.0f;
            if(our::GameActionsSystem::getOpenDoor())
            {
                our::GameActionsSystem::increasePowerupTimer(powerups::door,ImGui::GetIO().DeltaTime);
                if(our::GameActionsSystem::getPowerupTimer(powerups::door)>openDoorTimer)
                {
                    our::GameActionsSystem::resetOpenDoor();
                    our::GameActionsSystem::setGameWin();
                }

            }



        }
        else if(currentState==states["score"])
        {
  {
            
            ImGui::SetNextWindowSize(ImVec2(1000, 1000));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            // start window GUI
            ImGui::Begin("EXIT", nullptr, ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
            ImGui::SetWindowPos("EXIT", ImVec2(win_config.size.x/2-180, win_config.size.y/2-150));
            ImVec2 collectWindowPos = ImGui::GetWindowPos();
            ImVec2 collectWindowSize = ImGui::GetWindowSize();
            float collectWindowLeft = collectWindowPos.x;
            float collectWindowRight = collectWindowPos.x + collectWindowSize.x;
            float collectWindowTop = collectWindowPos.y;
            float collectWindowBottom = collectWindowPos.y + collectWindowSize.y;
            
        
           

            ImGuiStyle *style = &ImGui::GetStyle();

            ImVec4 *colors = style->Colors;

            // set the color for window background
            colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            colors[ImGuiCol_ScrollbarBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);

            ImGui::PushFont(powerupFont);
            std::string string1;
            if (our::GameActionsSystem::getGameState()==endState::lose){
              string1= "GAME OVER :(";
            }
            else if(our::GameActionsSystem::getGameState()==endState::play)
            {
               string1="YOU QUITTED ;(";
            }   
            else
            {
                string1= "CONGRATULATIONS :)";
            }
            std::string string2 = "SCORE: ";
            string2+=" " + std::to_string(our::GameActionsSystem::getScore());
            std::string string3 ="COINS COLLECTED: ";
            string3+= std::to_string(our::GameActionsSystem::getCoinsCollected()) + "/" +  std::to_string(our::GameActionsSystem::getTotalCoins());
            std::string string4 ="KEYS COLLECTED: ";
            string4+=" "+std::to_string(our::GameActionsSystem::getKeysCollected())+"/" + std::to_string(our::GameActionsSystem::getTotalKeys());
            std::string string5 ="POWERUPS COLLECTED: ";
            string5+=" " + std::to_string(our::GameActionsSystem::getPowerupsCollected())+"/" + std::to_string(our::GameActionsSystem::getTotalPowerups());
            std::string string6 ="REMAINING TIME: ";
            string6+=timer_display;

            ImGui::Text(string1.c_str());
            ImGui::Text(string2.c_str());
            ImGui::Text(string3.c_str());
            ImGui::Text(string4.c_str());
            ImGui::Text(string5.c_str());
            ImGui::Text(string6.c_str());
            ImGui::PopFont();            
            ImGui::PopStyleColor();
            ImGui::End();
            }
            
        }
        // Render the ImGui commands we called (this doesn't actually draw to the screen yet.
        ImGui::Render();

        // Just in case ImGui changed the OpenGL viewport (the portion of the window to which we render the geometry),
        // we set it back to cover the whole window
        auto frame_buffer_size = getFrameBufferSize();
        glViewport(0, 0, frame_buffer_size.x, frame_buffer_size.y);

        // Get the current time (the time at which we are starting the current frame).
        double current_frame_time = glfwGetTime();

        // Call onDraw, in which we will draw the current frame, and send to it the time difference between the last and current frame
        if(currentState) currentState->onDraw(current_frame_time - last_frame_time);
        last_frame_time = current_frame_time; // Then update the last frame start time (this frame is now the last frame)
#if defined(ENABLE_OPENGL_DEBUG_MESSAGES)
        // Since ImGui causes many messages to be thrown, we are temporarily disabling the debug messages till we render the ImGui
        glDisable(GL_DEBUG_OUTPUT);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Render the ImGui to the framebuffer

#if defined(ENABLE_OPENGL_DEBUG_MESSAGES)
        // Re-enable the debug messages
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

        // If F12 is pressed, take a screenshot
        if(keyboard.justPressed(GLFW_KEY_F12)){
            glViewport(0, 0, frame_buffer_size.x, frame_buffer_size.y);
            std::string path = default_screenshot_filepath();
            if(our::screenshot_png(path)){
                std::cout << "Screenshot saved to: " << path << std::endl;
            } else {
                std::cerr << "Failed to save a Screenshot" << std::endl;
            }
        }
        // There are any requested screenshots, take them
        while(requested_screenshots.size()){ 
            if(const auto& request = requested_screenshots.top(); request.first == current_frame){
                if(our::screenshot_png(request.second)){
                    std::cout << "Screenshot saved to: " << request.second << std::endl;
                } else {
                    std::cerr << "Failed to save a screenshot to: " << request.second << std::endl;
                }
                requested_screenshots.pop();
            } else break;
        }

        // Swap the frame buffers
        
        glfwSwapBuffers(window);

        // Update the keyboard and mouse data
        keyboard.update();
        mouse.update();
        // If a scene change was requested, apply it
        while(nextState){
            // If a scene was already running, destroy it (not delete since we can go back to it later)
            if(currentState) currentState->onDestroy();
            // Switch scenes
            currentState = nextState;

            // reset the gameover timer
            if(currentState==states["play"])
            {
               game_over_timer=0;
            }
            nextState = nullptr;
            // Initialize the new scene
            currentState->onInitialize();
        }

        ++current_frame;
    }

    // Call for cleaning up
    if(currentState) currentState->onDestroy();

    // Shutdown ImGui & destroy the context
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // destroy cursor
    glfwDestroyCursor(cursor);
    // Destroy the window
    glfwDestroyWindow(window);

    // And finally terminate GLFW
    glfwTerminate();

    // destroy the audio engine
    our::SoundSystem::deinit_engine();
    return 0; // Good bye
}

// Sets-up the window callback functions from GLFW to our (Mouse/Keyboard) classes.
void our::Application::setupCallbacks() {

    // We use GLFW to store a pointer to "this" window instance.
    glfwSetWindowUserPointer(window, this);
    // The pointer is then retrieved in the callback function.

    // The second parameter to "glfwSet---Callback" is a function pointer.
    // It is replaced by an inline function -lambda expression- as it is not needed to create
    // a seperate function for it.
    // In the inline function we retrieve the window instance and use it to set our (Mouse/Keyboard) classes values.

    // Keyboard callbacks
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            app->getKeyboard().keyEvent(key, scancode, action, mods);
            if(app->currentState) app->currentState->onKeyEvent(key, scancode, action, mods);
        }
    });

    // mouse position callbacks
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x_position, double y_position){
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            app->getMouse().CursorMoveEvent(x_position, y_position);
            if(app->currentState) app->currentState->onCursorMoveEvent(x_position, y_position);
        }
    });

    // mouse position callbacks
    glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered){
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            if(app->currentState) app->currentState->onCursorEnterEvent(entered);
        }
    });

    // mouse button position callbacks
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            app->getMouse().MouseButtonEvent(button, action, mods);
            if(app->currentState) app->currentState->onMouseButtonEvent(button, action, mods);
        }
    });

    // mouse scroll callbacks
    glfwSetScrollCallback(window, [](GLFWwindow* window, double x_offset, double y_offset){
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            app->getMouse().ScrollEvent(x_offset, y_offset);
            if(app->currentState) app->currentState->onScrollEvent(x_offset, y_offset);
        }
    });
}
