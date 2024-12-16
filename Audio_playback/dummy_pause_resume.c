#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 256

Mix_Music *music = NULL; // Global variable to hold the music

// Function to initialize SDL2 and SDL2_mixer
bool init() {
    // Specify SDL audio driver to prevent ALSA errors
    SDL_setenv("SDL_AUDIODRIVER", "dummy", true);

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

// Function to load and play audio file
bool play_audio(const char *filename) {
    music = Mix_LoadMUS(filename);
    if (music == NULL) {
        fprintf(stderr, "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    if (Mix_PlayMusic(music, 1) == -1) {
        fprintf(stderr, "SDL_mixer could not play music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

// Function to pause the music
void pause_audio() {
    if (Mix_PausedMusic() == 0) {
        Mix_PauseMusic();
        printf("Music paused.\n");
    } else {
        printf("Music is already paused.\n");
    }
}

// Function to resume the music
void resume_audio() {
    if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic();
        printf("Music resumed.\n");
    } else {
        printf("Music is already playing.\n");
    }
}

// Function to check if music is playing
bool is_playing() {
    if (Mix_PlayingMusic() != 0) {
        return true;
    }
    return false;
}

// Function to free resources and quit SDL
void cleanup() {
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
}

int main() {
    char filename[MAX_FILENAME_LENGTH];
    bool valid_choice = false;

    if (!init()) {
        fprintf(stderr, "Failed to initialize SDL and SDL_mixer.\n");
        return 1;
    }

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
        printf("Audio file '%s' started playing.\n", filename);
    } else {
        printf("Failed to play audio file '%s'.\n", filename);
        cleanup();
        return 1;
    }

    char option;
    bool quit = false;

    while (!quit) {
        printf("\nOptions:\n");
        printf("1 - Pause\n");
        printf("2 - Resume\n");
        printf("q - Quit\n");
        printf("Choose an option: ");
        scanf(" %c", &option);

        switch (option) {
            case '1':
                if (is_playing()) {
                    pause_audio();
                } else {
                    printf("No music playing.\n");
                }
                break;

            case '2':
                if (is_playing()) {
                    resume_audio();
                } else {
                    printf("No music paused.\n");
                }
                break;

            case 'q':
                quit = true;
                break;

            default:
                printf("Invalid option. Please choose again.\n");
                break;
        }
    }

    // Wait until the music finishes playing
    while (is_playing()) {
        SDL_Delay(100);
    }

    printf("Song completed.\n");

    cleanup();
    return 0;
}

