#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "headers/loadimage.h"
#include "headers/tamagotchi.h"
#include "headers/SDL_FontCache.h"

using namespace std;

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void rectangle(SDL_Renderer *renderer, SDL_Rect rect) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void updateIcons(SDL_Renderer *renderer, SDL_Texture *image, SDL_Rect iconRect, SDL_Rect iconPos, int menuCounter) {
    for (int i=0; i<8; i++) {    
        if (i < 4) {
            iconPos.x = 56 * i;
            iconPos.y = 0;
        } else {
            iconPos.x = (56 * i) - 224;
            iconPos.y = 168;
        }
            


        if (menuCounter == i) {
            iconRect.x = 32;
            iconRect.y = 32 * i;
        } else {
            iconRect.x = 0;
            iconRect.y = 32 * i;
        }

        SDL_RenderCopy(renderer, image, &iconRect, &iconPos);
    }
}

/*
0 - Eating
1 - Lights ON / OFF
2 - Play
3 - Medicine
4 - Toilet
5 - Stat meter
6 - SCOLD
7 - SLEEP STAT
*/

struct {
    bool showClock = false;
    int statScreenMenu = 0;
    int foodChoice = 0;
    bool proceedToEat = false;
    bool dontLeavePage = false;
    bool lightChoice = true;
    int lightMode = 1; //1 on 0 off
    int hungerCounter = 0;
    //play
    bool leftClick = false;
    bool rightClick = false;
} tamagotchiVals;

void gameMain() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Failed to initialize the SDL2 library\n" << endl;
        return;
    }

    if (IMG_Init(IMG_INIT_JPG) < 0) {
        cout << "Failed to Inititalize image" << endl;
        return;
    }

    window = SDL_CreateWindow("TAMAGOTCHI", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetWindowBordered(window, SDL_FALSE); //window border


    if(window == NULL) {
        cout << "Failed to create window\n";
        return;
    }

    SDL_Texture *icons = loadImage("images/iconpack3.png", renderer);
    SDL_Rect iconPos;
    iconPos.w = 32;
    iconPos.h = 32;
    iconPos.x = 0;
    iconPos.y = 0;

    SDL_Rect iconRect;
    iconRect.w = 32;
    iconRect.h = 32;

    SDL_Rect sampleRect = {0, 100 - (128 / 2), SCREEN_WIDTH, 128};

    Tamagotchi animal(renderer);
    animal.loadFile();
    
    int menuCounter = -1;
    bool confirmPage = false;
    bool shouldDecreaseHunger = true;

    float FPS = 60;
    float desiredDelta = 1000 / 60;
    bool running = true;
    SDL_Event e;
    while (running) {
        int start = SDL_GetTicks();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                int key = e.key.keysym.sym;
                if (key == SDLK_p) {
                    running = false;
                } else if (key == SDLK_l) {
                    animal.loadFile();
                } else if (key == SDLK_q) {
                    if (!confirmPage) {
                        menuCounter += 1;

                        if (menuCounter == 1 && confirmPage == false && tamagotchiVals.lightMode == 0) {//special exception
                            confirmPage = true;
                        }

                        if (menuCounter > 6) {
                            menuCounter = 0;
                        }
                    } else {
                        if (menuCounter == 0) {
                            tamagotchiVals.foodChoice++;
                            if (tamagotchiVals.foodChoice > 1) {
                                tamagotchiVals.foodChoice = 0;
                            }
                        } else if (menuCounter == 1) {
                            tamagotchiVals.lightChoice = !tamagotchiVals.lightChoice;
                        } else if (menuCounter == 2 && animal.canChooseNumber()) {
                            tamagotchiVals.leftClick = true;
                            tamagotchiVals.rightClick = false;
                        }
                    }
                } else if (key == SDLK_w) { // confirm page / choice selection
                    if (!confirmPage && menuCounter > -1) { //animation idle
                        confirmPage = true;
                    } else {
                        if (menuCounter == -1) { //show clock
                            tamagotchiVals.showClock = !tamagotchiVals.showClock;
                        } else if (menuCounter == 0) {
                            tamagotchiVals.proceedToEat = true;
                        } else if (menuCounter == 1) { //lights
                            if (tamagotchiVals.lightChoice == true) { // on
                                tamagotchiVals.lightMode = 1;
                                menuCounter = -1;
                                confirmPage = false;
                            } else {
                                tamagotchiVals.lightMode = 0;
                                menuCounter = -1;
                                confirmPage = false;
                            }
                        } else if (menuCounter == 2 && animal.canChooseNumber()) {
                            tamagotchiVals.leftClick = false;
                            tamagotchiVals.rightClick = true;
                        } else if (menuCounter == 5) { //stats
                            tamagotchiVals.statScreenMenu++;
                            if (tamagotchiVals.statScreenMenu > 3) {
                                tamagotchiVals.statScreenMenu = 0;
                            }
                        }
                    }
                } else if (key == SDLK_e) { // confirm CHOICE
                    if (!tamagotchiVals.dontLeavePage) {
                        animal.resetPlay(&tamagotchiVals.leftClick, &tamagotchiVals.rightClick);
                        confirmPage = false;
                        menuCounter = -1;
                    }
                } else if (key == SDLK_y) {
                    animal.movingY -= 1;
                } else if (key == SDLK_h) {
                    animal.movingY += 1;
                } else if (key == SDLK_g) {
                    animal.movingX -= 1;
                } else if (key == SDLK_j) {
                    animal.movingX += 1;
                }
            }
        }
        SDL_RenderClear(renderer);
        
        
        //everything here
        
        updateIcons(renderer, icons, iconRect, iconPos, menuCounter);

        
        if (shouldDecreaseHunger) {
            animal.decreaseHungerAndPoop(&tamagotchiVals.hungerCounter);
        }
        animal.setPoopStandard(); // 2 poops only
        

        if (confirmPage) {
            if (menuCounter == 0) {
                animal.eating(tamagotchiVals.foodChoice, &tamagotchiVals.proceedToEat, &tamagotchiVals.dontLeavePage);
            } else if (menuCounter == 1) {
                animal.lights(&tamagotchiVals.lightChoice);
            } else if (menuCounter == 2) {
                animal.play(&tamagotchiVals.leftClick, &tamagotchiVals.rightClick, &menuCounter, &confirmPage);
            } else if (menuCounter == 3) {
                animal.medicine(&menuCounter, &confirmPage);
            } else if (menuCounter == 4) {
                shouldDecreaseHunger = false;
                animal.toilet(&menuCounter, &confirmPage, &shouldDecreaseHunger);
            } else if (menuCounter == 5) {
                animal.stats(tamagotchiVals.statScreenMenu);
            } else if (menuCounter == 6) {
                animal.scold(&menuCounter, &confirmPage);
            } 
        } else {
            animal.idle(tamagotchiVals.showClock);
        }

        if (tamagotchiVals.lightMode == 0) {
            if (menuCounter != 1) {
                rectangle(renderer, sampleRect);
            }
        }

        

        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderPresent(renderer); // copy pixels / update
        int currentTime = SDL_GetTicks();
        int desiredTime = currentTime - start;
        if (FPS > desiredTime) {
            //cout << "Desired Time: " << desiredTime << "| Delay: " << FPS/desiredTime << endl;
            SDL_Delay(FPS/desiredTime);
        }
    }

    // animal.saveData();

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    srand(time(0));
    gameMain();
    return 0;
}