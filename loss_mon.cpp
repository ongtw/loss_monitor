#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
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
struct cmp_gtr
{
    template <class T>
    auto operator()(T const &a, T const &b) const { return a > b; }
};

// want alertMap in desc order
std::map<float, std::string, cmp_gtr> alertMap = {
    {2.60, "sounds/warning-sound-6686.mp3"},
    {3.00, "sounds/uh-ohh-38886.mp3"},
    {4.00, "sounds/girl_scream-6465.mp3"},
};

std::vector<std::string> alertVec = {
    "warning",
    "getting serious",
    "Aarrrgggghhhhh!!",
};

//
// Helper Functions
//
std::vector<std::string> tokenize(std::string str)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, ' '))
    {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char *argv[])
{
    // Init SDL and SDL_mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL could not init music! SDL error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer could not init! SDL_mixer error: " << Mix_GetError() << std::endl;
        return 1;
    }

    // Load MP3
    std::vector<Mix_Music *> sounds(3);
    std::map<float, int, cmp_gtr> threshold_to_index;
    auto i = 2;
    for (const auto &pair : alertMap)
    {
        auto thres = pair.first;
        auto sound = pair.second;
        std::cout << thres << " -> " << sound << std::endl;

        sounds[i] = Mix_LoadMUS(sound.c_str());
        if (sounds[i] == NULL)
        {
            std::cout << "Failed to load music! SDL_mixer error: " << Mix_GetError() << std::endl;
            return 1;
        }
        else
        {
            std::cout << sound << " loaded into [" << i << "]" << std::endl;
        }

        auto p = std::pair<float, int>(thres, i);
        std::cout << "gen map index [" << p.first << " -> " << p.second << "]" << std::endl;
        threshold_to_index.insert(p);

        i--;
    }

    //
    // Main monitor loop
    //
    std::string line;
    while (std::getline(std::cin, line))
    {
        auto toks = tokenize(line);
        auto loss_str = toks[1];
        auto batch = toks[2];
        auto loss = std::stof(loss_str);

        std::cout << "batch: " << batch << ", loss: ";
        std::cout << std::setprecision(6) << loss;

        auto it = std::find_if(
            begin(threshold_to_index), end(threshold_to_index), [loss](std::pair<float, int> p)
            { return p.first < loss; });

        if (it != end(threshold_to_index))
        {
            auto threshold = it->first;
            auto k = it->second;
            std::cout << " > " << threshold << "  " << alertVec[k] << std::endl;

            Mix_PlayMusic(sounds[k], 0);
            // Wait for the music to finish
            while (Mix_PlayingMusic())
            {
                SDL_Delay(100);
            }
        }
        else
        {
            std::cout << std::endl;
        }
    }

    // Free music resources and close SDL_mixer
    for (auto i = 0; i < sounds.size(); i++)
    {
        Mix_FreeMusic(sounds[i]);
    }
    Mix_CloseAudio();

    return 0;
}
