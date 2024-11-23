#include <iostream>
#include <ctime>
#include <string>
#include <fstream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_FontCache.h"
#include "headers/tamagotchi.h"
#include "headers/loadimage.h"
using namespace std;

void Tamagotchi::loadFile() {

	string line;
	ifstream saveFile("save.txt");
	if (saveFile.is_open()) {
		int counter = 0;
		while (getline(saveFile, line)) {
			tamagotchiData[counter] = line;
			counter++;
		}
		
		saveFile.close();
	} else {
		cout << "Could not open save file." << endl;
		return;
	}

	for (int i=0; i<sizeof(tamagotchiData)/sizeof(string); i++) { // read line
		int location = tamagotchiData[i].find("=");
		string key = tamagotchiData[i].substr(0, location);
		string value = tamagotchiData[i].substr(location+1, sizeof(tamagotchiData)/sizeof(string) - 1);

		if (key == "poop") {
			poop = stoi(value);
		} else if (key == "mass") {
			mass = stoi(value);
		} else if (key == "age") {
			age = stoi(value);
		} else if (key == "hunger") {
			hungry = stof(value);
		} else if (key == "discipline") {
			discipline = stoi(value);
		} else if (key == "happy") {
			happy = stof(value);
		}
	}
}

void Tamagotchi::saveData() {
	ofstream saveFile("save.txt");

	if (saveFile.is_open()) {
		saveFile << "poop=" << poop << endl;
		saveFile << "mass=" << mass << endl;
		saveFile << "age=" << age << endl;
		saveFile << "hunger=" << hungry << endl;
		saveFile << "discipline=" << discipline << endl;
		saveFile << "happy=" << happy << endl;

		saveFile.close();
	} else {
		cout << "Could not save file." << endl;
		return;
	}
}

Tamagotchi::Tamagotchi(SDL_Renderer *renderer) {
	pointerOfRenderer = renderer;

	image = loadImage("./images/tamagotchi.png", pointerOfRenderer);
	imagePos = {100 - (128 / 2), 100 - (128 / 2), 128, 128};
	imageRect = {0, 0, 128, 128};

	font = FC_CreateFont();
	FC_LoadFont(font, pointerOfRenderer, "font/Minecraft.ttf", 20, FC_MakeColor(255,255,255,255), TTF_STYLE_NORMAL); 

	statScreenImage = loadImage("./images/statsscreen.png", pointerOfRenderer);
	statScreenImagePos = {100 - (128/2), 100 - (128/2), 128, 128};
	statScreenImageRect = {0, 0, 128, 128};

	eatingScreenImage = loadImage("./images/eatingscreen.png", pointerOfRenderer);
	eatingScreenImagePos = {100 - (128/2), 100 - (128/2), 128, 128};
	eatingScreenImageRect = {0, 0, 128, 128};

	lightsScreenImage = loadImage("./images/lights.png", pointerOfRenderer);
	lightsScreenImagePos = {100 - (128/2), 100 - (128/2), 128, 128};
	lightsScreenImageRect = {0, 0, 128, 128};

}

string Tamagotchi::getTime() {
	time_t currentTimestamp = time(NULL); //timestamp;
	tm *localT = localtime(&currentTimestamp);

	string convertedTime;
	string hour;
	string min;
	string sec;

	if (localT->tm_min < 10) {
		min = "0" + to_string(localT->tm_min);
	} else {
		min = to_string(localT->tm_min);
	}

	if (localT->tm_sec < 10) {
		sec = "0" + to_string(localT->tm_sec);
	} else {
		sec = to_string(localT->tm_sec);
	}

	if (localT->tm_hour > 12) {
		convertedTime = to_string(localT->tm_hour - 12) + ":" + min + ":" + sec + "PM";
	} else {
		convertedTime = to_string(localT->tm_hour) + ":" + min + ":" + sec + "AM";
	}

	return convertedTime;
}

void Tamagotchi::idle(bool showClock) {
	string convertedTime = getTime();
	if (!showClock) {
		animationCounter++;
		if (animationCounter % 30 == 0) {
			clock++;
			if (clock > 4) {
				clock = 0;
			}

			imageRect.x = 128 * clock;
		}
	}
	

	if (showClock) {
		if (imagePos.x != -128) {
			imagePos.x -= 2;
			if (clockX >= 10) {
				clockX -= 2;
			}
		}
	} else {
		if (imagePos.x != 100 - (128 / 2)) {
			imagePos.x += 2;
			if (clockX < 201) {
				clockX += 2;
			}
		}
	}

	FC_Draw(font, pointerOfRenderer, clockX, 70, convertedTime.c_str());
	SDL_RenderCopy(pointerOfRenderer, image, &imageRect, &imagePos);
}


void Tamagotchi::eating(int choiceOfFood, bool *proceedToEat, bool *dontLeavePage) {
	if (choiceOfFood == 0) { // burger
		if (*proceedToEat == true) {
			*dontLeavePage = true;
			animationCounter++;
			if (hungry == 100) { //dont eat animation
				if (animationCounter % 20 == 0) {
					if (dontEatFrame > 4) {
						dontEatFrame = 0;
						*dontLeavePage = false;
						*proceedToEat = false;
					} else {
						eatingScreenImageRect.x = 128 * dontEatFrame;
						eatingScreenImageRect.y = 128 * 3;
						dontEatFrame++;
					}
					
				}
			} else {
				if (animationCounter % 30 == 0) {
					if (eatingFrame > 7) {
						eatingFrame = 0;
						hungry += 12.5;
						*dontLeavePage = false;
						*proceedToEat = false;
					} else {
						eatingScreenImageRect.x = 128 * eatingFrame;
						eatingScreenImageRect.y = 128 * 2;
						eatingFrame++;
					}
					
				}
			}
			
		} else {
			eatingScreenImageRect.x = 0;
			eatingScreenImageRect.y = 0;
		}
	} else { // cake
		if (*proceedToEat == true) {
			*dontLeavePage = true;
			animationCounter++;
			if (hungry == 100) {
				if (animationCounter % 20 == 0) {
					if (dontEatFrame > 4) {
						dontEatFrame = 0;
						*dontLeavePage = false;
						*proceedToEat = false;
					}
					eatingScreenImageRect.x = 128 * dontEatFrame;
					eatingScreenImageRect.y = 128 * 3;
					dontEatFrame++;
				}
			} else {
				if (animationCounter % 30 == 0) {
					if (eatingFrame > 7) {
						eatingFrame = 0;
						hungry += 12.5;
						*dontLeavePage = false;
						*proceedToEat = false;
					}
					eatingScreenImageRect.x = 128 * eatingFrame;
					eatingScreenImageRect.y = 128;
					eatingFrame++;
				}
			}
		} else {
			eatingScreenImageRect.x = 128;
			eatingScreenImageRect.y = 0;
		}
	}

	SDL_RenderCopy(pointerOfRenderer, eatingScreenImage, &eatingScreenImageRect, &eatingScreenImagePos);
}

void Tamagotchi::lights(bool *lightChoice) {
	if (*lightChoice) {
		lightsScreenImageRect.x = 0;
		lightsScreenImageRect.y = 0;
	} else {
		lightsScreenImageRect.x = 128;
		lightsScreenImageRect.y = 0;
	}

	SDL_RenderCopy(pointerOfRenderer, lightsScreenImage, &lightsScreenImageRect, &lightsScreenImagePos);
	
}

void Tamagotchi::play() {
	
}

void Tamagotchi::medicine() {
	
}

void Tamagotchi::toilet() {
	
}

void Tamagotchi::stats(int screen) {
	if (screen == 0) { // 
		statScreenImageRect.x = 0;
		statScreenImageRect.y = 128 * 0;
	} else if (screen == 1) {
		if (discipline == 0) {
			statScreenImageRect.x = 0;
		} else {
			statScreenImageRect.x = 128 * ((discipline / 25));
		}
		statScreenImageRect.y = 128 * 1;
	} else if (screen == 2) {
		if (hungry == 0) {
			statScreenImageRect.x = 0;
		} else {
			statScreenImageRect.x = 128 * (hungry / 12.5);
		}
		
		statScreenImageRect.y = 128 * 2;
	} else if (screen == 3) {
		if (happy == 0) {
			statScreenImageRect.x = 0;
		} else {
			statScreenImageRect.x = 128 * (happy / 12.5);
		}
		statScreenImageRect.y = 128 * 3;
	}

	SDL_RenderCopy(pointerOfRenderer, statScreenImage, &statScreenImageRect, &statScreenImagePos);
}

void Tamagotchi::scold() {
	
}