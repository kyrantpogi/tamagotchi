#pragma once
#include <string>
#include <ctime>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_FontCache.h"
using namespace std;

class Tamagotchi {
	private:
		//tamagotchi animation
		SDL_Rect imageRect;
		SDL_Rect imagePos;
		SDL_Texture *image;
		SDL_Renderer *pointerOfRenderer;
		FC_Font *font;
		int animationCounter;
		int clock;
		int poopAnimation = 0;

		//internal clock
		int clockX = 201;

		//stat screen
		long int startDate;
		int age = 0;
		int poop = 0;
		int mass = 0;
		string tamagotchiData[7];
		int discipline = 0; //+25 only
		float hungry = 0; //+12.5 only
		float happy = 0;  //+12.5 only
		SDL_Texture *statScreenImage;
		SDL_Rect statScreenImagePos;
		SDL_Rect statScreenImageRect;

		//eating screen
		SDL_Texture *eatingScreenImage;
		SDL_Rect eatingScreenImageRect;
		SDL_Rect eatingScreenImagePos;
		int eatingFrame = 0;
		int dontEatFrame = 0;

		//lights screen
		SDL_Texture *lightsScreenImage;
		SDL_Rect lightsScreenImagePos;
		SDL_Rect lightsScreenImageRect;

		//poop
		int poopPoints[2][2] = {
			{149, 108},
			{149, 26}
		};
		SDL_Texture *poopImage;
		SDL_Rect poopImagePos;
		SDL_Rect poopImageRect;

		//toilet
		SDL_Texture *toiletImage;
		SDL_Rect toiletImagePos;
		SDL_Rect toiletImageRect;


	public:
		//positioning
		float movingX = 0;
		float movingY = 0;

		Tamagotchi(SDL_Renderer *renderer);
		string getTime();
		void idle(bool showClock);
		void decreaseHungerAndPoop(int *counter);
		void eating(int choiceOfFood, bool *proceedToEat, bool *dontLeavePage);
		void lights(bool *lightChoice);
		void play();
		void medicine();
		void toilet(int *menuCounter, bool *confirmPage, bool *shouldDecreaseHunger);
		int ageToMassConversion(int age);
		void stats(int screen);
		void scold();
		void loadFile();
		void saveData();
};