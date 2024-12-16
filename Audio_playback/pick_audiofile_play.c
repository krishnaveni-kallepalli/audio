#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 256

// Function to play audio file
bool play_audio(const char *filename) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    Mix_Music *music = Mix_LoadMUS(filename);
    if (music == NULL) {
        fprintf(stderr, "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    if (Mix_PlayMusic(music, 1) == -1) {
        fprintf(stderr, "SDL_mixer could not play music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    while (Mix_PlayingMusic()) {
        SDL_Delay(100);
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return true;
}

int main() {
    char filename[MAX_FILENAME_LENGTH];
    char choice;
    bool valid_choice = false;

    do {
        printf("Choose an audio file to play (mp3 or wav):\n");
        scanf("%s", filename);

        // Check if the file has a valid extension
        if (strstr(filename, ".mp3") || strstr(filename, ".wav")) {
            valid_choice = true;
        } else {
            printf("Invalid file format. Please enter a valid file name with .mp3 or .wav extension.\n");
        }
    } while (!valid_choice);

    if (play_audio(filename)) {
        printf("Audio file '%s' played successfully.\n", filename);
    } else {
        printf("Failed to play audio file '%s'.\n", filename);
    }

    return 0;
}

