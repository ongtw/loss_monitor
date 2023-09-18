#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


//
// AISG LLM Training Loss Monitor
// dotw 20230916
// - to monitor MLFLow loss/train/total metric for loss spikes
//


//
// Data Structures
//
struct cmp_gtr {
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

// want alertMap in desc order
std::map<float, std::string, cmp_gtr> alertMap = {
    { 2.50, "sounds/warning-sound-6686.mp3" },
    { 2.75, "sounds/uh-ohh-38886.mp3" },
    { 3.00, "sounds/girl_scream-6465.mp3" },
};

std::vector<std::string> alertVec = {
    "warning",
    "getting serious",
    "Aarrrgggghhhhh!!",
};


//
// Helper Functions
//
std::vector<std::string> tokenize(std::string str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}


int main(int argc, char* argv[]) {
    // Init SDL and SDL_mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    // Load MP3
    std::vector<Mix_Music*> sounds(3);
    int i = 2;
    for (const auto &pair : alertMap) {
        float thres = pair.first;
        std::string sound = pair.second;
        std::cout << thres << " -> " << sound << std::endl;

        sounds[i] = Mix_LoadMUS(sound.c_str());
        if (sounds[i] == NULL) {
            printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
            return 1;
        } else {
            std::cout << sound << " loaded into [" << i  << "]" << std::endl;
        }
        i--;
    }

    //
    // Main monitor loop
    //
    std::string line;
    while (std::getline(std::cin, line)) {
        std::vector<std::string> toks = tokenize(line);
        std::string loss_str = toks[1];
        std::string batch = toks[2];

        float loss = std::stof(loss_str);

        std::cout << "batch: " << batch << ", loss: ";
        std::cout << std::setprecision(6) << loss;

        bool alert = false;
        float threshold = 0;
        int k = 2;

        // Check if loss exceeds any threshold value in alertMap
        for (const auto &pair : alertMap) {
            float thres = pair.first;
            if (loss > thres) {
                alert = true;
                threshold = thres;
                break;
            } else {
                k--;
            }
        }

        if (alert) {
            std::cout << " > " << threshold << "  " << alertVec[k];
        }
        std::cout << std::endl;

        if (alert) {
            Mix_PlayMusic(sounds[k], 0);
            // Wait for the music to finish
            while (Mix_PlayingMusic()) {
                SDL_Delay(100);
            }
        }
    }

    // Free music resources and close SDL_mixer
    for (int i = 0; i < sounds.size(); i++) {
        Mix_FreeMusic(sounds[i]);
    }
    Mix_CloseAudio();

    return 0;
}
        
