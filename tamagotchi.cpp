#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <cmath>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_FontCache.h"
#include "headers/tamagotchi.h"
#include "headers/loadimage.h"
using namespace std;

Tamagotchi::Tamagotchi(SDL_Renderer *renderer) {
	pointerOfRenderer = renderer;

	image = loadImage("./images/tamagotchi.png", pointerOfRenderer);
	imagePos = {100 - (128 / 2), 100 - (128 / 2), 128, 128};
	imageRect = {0, 0, 128, 128};

	font = FC_CreateFont();
	FC_LoadFont(font, pointerOfRenderer, "font/Minecraft.ttf", 28, FC_MakeColor(255,255,255,255), TTF_STYLE_NORMAL); 

	statScreenImage = loadImage("./images/statsscreen.png", pointerOfRenderer);
	statScreenImagePos = {100 - (128/2), 100 - (128/2), 128, 128};
	statScreenImageRect = {0, 0, 128, 128};

	eatingScreenImage = loadImage("./images/eatingscreen.png", pointerOfRenderer);
	eatingScreenImagePos = {100 - (128/2), 100 - (128/2), 128, 128};
	eatingScreenImageRect = {0, 0, 128, 128};

	lightsScreenImage = loadImage("./images/lights.png", pointerOfRenderer);
	lightsScreenImagePos = {100 - (128/2), 100 - (128/2), 128, 128};
	lightsScreenImageRect = {0, 0, 128, 128};

	poopImage = loadImage("./images/poop.png", pointerOfRenderer);
	poopImagePos = {0, 0, 64, 64};
	poopImageRect = {64, 0, 64, 64};

	toiletImage = loadImage("./images/toilet.png", pointerOfRenderer);
	toiletImagePos = {96, 100 - (128 / 2), 128, 128};
	toiletImageRect = {0, 0, 128, 128};

	scoldImage = loadImage("./images/scold.png", pointerOfRenderer);
	scoldImagePos = {100 - (128 / 2), 100 - (128 / 2), 128, 128};
	scoldImageRect = {0, 0, 128, 128};

}

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
		string value = tamagotchiData[i].substr(location+1, tamagotchiData[i].size());

		if (key == "poop") {
			poop = stoi(value);
		} else if (key == "age") {
			age = stoi(value);
			mass = this->ageToMassConversion(age);
		} else if (key == "hunger") {
			hungry = stof(value);
		} else if (key == "discipline") {
			discipline = stoi(value);
		} else if (key == "happy") {
			happy = stof(value);
		} else if (key == "start_date") {
			startDate = stoul(value);
			time_t today;
			time(&today);
			if (startDate == 0) {
				startDate = today;
				age = (int)((difftime(today, startDate) / 3600) / 24);
			} else {
				age = (int)((difftime(today, startDate) / 3600) / 24);
			}

		}
	}
}

void Tamagotchi::saveData() {
	ofstream saveFile("save.txt");

	if (hungry < 0) {
		hungry = 0;
	}

	if (saveFile.is_open()) {
		saveFile << "poop=" << poop << endl;
		saveFile << "mass=" << mass << endl;
		saveFile << "age=" << age << endl;
		saveFile << "hunger=" << hungry << endl;
		saveFile << "discipline=" << discipline << endl;
		saveFile << "happy=" << happy << endl;
		saveFile << "start_date="  << startDate << endl;

		saveFile.close();
	} else {
		cout << "Could not save file." << endl;
		return;
	}
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
		//poop clock
		if (animationCounter % 20 == 0) {
			poopAnimation++;
			if (poopAnimation > 1) {
				poopAnimation = 0;
			}
			poopImageRect.x = 64 * poopAnimation;
		}

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

		if (poopPoints[0][0] != -51) {
			for (int i=0; i<2; i++) {
				poopPoints[i][0] -= 2;
			}
		}

	} else {
		if (imagePos.x != 100 - (128 / 2)) {
			imagePos.x += 2;
			if (clockX < 201) {
				clockX += 2;
			}
		}

		if (poopPoints[0][0] != 149) {
			for (int i=0; i<2; i++) {
				poopPoints[i][0] += 2;
			}
		}
	}

	FC_Draw(font, pointerOfRenderer, clockX, 70, convertedTime.c_str());
	SDL_RenderCopy(pointerOfRenderer, image, &imageRect, &imagePos);

	for (int i=0; i<poop; i++) {

		poopImagePos.x = poopPoints[i][0];
		poopImagePos.y = poopPoints[i][1];

		SDL_RenderCopy(pointerOfRenderer, poopImage, &poopImageRect, &poopImagePos);
	}
	
}

void Tamagotchi::decreaseHungerAndPoop(int *counter) {
	(*counter)++;
	if (*counter % 100 == 0) {
		if (poop < 2 && hungry == 50 || hungry == 0) {
			poop++;
		}
		if (hungry > -1) {
			hungry -= 12.5;
			cout << hungry << endl;
			if (hungry == -12.5) {
				hungry == 0;
			}
		}
		*counter = 0;
	}
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

void Tamagotchi::toilet(int *menuCounter, bool *confirmPage, bool *shouldDecreaseHunger) {
	
	if (imagePos.x > -220) {
		toiletImagePos.x -= 2;
		imagePos.x -= 2;
		for (int i=0; i<poop; i++) {
			poopPoints[i][0] -= 2;
		}

		SDL_RenderCopy(pointerOfRenderer, image, &imageRect, &imagePos);

		for (int i=0; i<poop; i++) {

			poopImagePos.x = poopPoints[i][0];
			poopImagePos.y = poopPoints[i][1];

			SDL_RenderCopy(pointerOfRenderer, poopImage, &poopImageRect, &poopImagePos);
		}

		SDL_RenderCopy(pointerOfRenderer, toiletImage, &toiletImageRect, &toiletImagePos);

	} else {
		toiletImagePos.x = 96;
		poop = 0;
		*confirmPage = false;
		*menuCounter = -1;
		*shouldDecreaseHunger = true;
	}

	
}

int Tamagotchi::ageToMassConversion(int age) {
	int mass;

	if (age < 5) {
		mass = age * 2.1;
	} else if (age > 5 && age < 10) {
		mass = age * 2.15;
	} else if (age > 10 && age < 15) {
		mass = age * 2.16;
	} else if (age > 15 && age < 20) {
		mass = age * 2.17;
	} else if (age > 20 && age < 25) {
		mass = age * 2.19;
	} else if (age > 25 && age < 30) {
		mass = age * 2.22;
	} 

	return mass;
}

void Tamagotchi::stats(int screen) {
	if (screen == 0) { // 
		statScreenImageRect.x = 0;
		statScreenImageRect.y = 128 * 0;

		string ageString = to_string(age) + " y";
		string massString = to_string(mass) + " g";
		
		FC_Draw(font, pointerOfRenderer, 125, 56, ageString.c_str());
		FC_Draw(font, pointerOfRenderer, 125, 127, massString.c_str());

	} else if (screen == 1) {
		if (discipline == 0) {
			statScreenImageRect.x = 0;
		} else {
			statScreenImageRect.x = 128 * (discipline / 25);
		}
		statScreenImageRect.y = 128 * 1;
	} else if (screen == 2) {
		if (hungry == 0 || hungry == -12.5) {
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

void Tamagotchi::scold(int *menuCounter, bool *confirmPage) {
	animationCounter++;
	if (animationCounter % 30 == 0) {
		scoldAnimation++;
		if (animationCounter > 450) {
			if (discipline == 100) {
				discipline = 100;
			} else {
				discipline += 25;
			}
			animationCounter = 0;
			*confirmPage = false;
			*menuCounter = -1;
		}

		if (scoldAnimation > 1) {
			scoldAnimation = 0;
		}

		scoldImageRect.x = 128 * scoldAnimation;
	}

	SDL_RenderCopy(pointerOfRenderer, scoldImage, &scoldImageRect, &scoldImagePos);
}

void Tamagotchi::resetAnimationCounter() {
	animationCounter = 0;
}