#include "SDL.h"
#include "SDL_image.h"
#include "headers/loadimage.h"
#include <string>
#include <iostream>
using namespace std;

SDL_Texture *loadImage(string path, SDL_Renderer *renderer) {
    SDL_Texture *finalImage = NULL;
    SDL_Surface *image = IMG_Load(path.c_str());
    if (image == NULL) {
        cout << "Error loading image: [" << path << "]" << endl;
        return NULL;
    }

    finalImage = SDL_CreateTextureFromSurface(renderer, image);
    if (finalImage == NULL) {
        cout << "Error OPTIMIZING image: [" << path << "]" << endl;
        return NULL;
    }

    SDL_FreeSurface(image);

    return finalImage;
}