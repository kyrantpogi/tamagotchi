#pragma once
#include <string>
#include <ctime>
#include <stdlib.h>

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

		//scold 
		int scoldAnimation = 0;
		SDL_Texture *scoldImage;
		SDL_Rect scoldImagePos;
		SDL_Rect scoldImageRect;

		//play
		int scene = 0;
		bool isLoadingDone = false;
		int randomX;
		int randomY;
		int attempt = 0;
		int humanScore = 0;
		int tamagotchiScore = 0;

		int toggleAnimation = 0;
		int toggleAnimation2 = 3;
		int playAnimationCounter = 0;
		SDL_Texture *playLoadImage;
		SDL_Rect playLoadImagePos;

		SDL_Texture *playImage;
		SDL_Rect playImagePos;
		SDL_Rect playImageRect;

		SDL_Texture *switchImage;

		SDL_Texture *scoreScreenImage;
		SDL_Rect scoreScreenImagePos;


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
		void play(bool *leftClick, bool *rightClick, int *menuCounter, bool *confirmPage);
		void resetPlay(bool *leftClick, bool *rightClick);
		bool canChooseNumber();
		void medicine();
		void toilet(int *menuCounter, bool *confirmPage, bool *shouldDecreaseHunger);
		int ageToMassConversion(int age);
		void stats(int screen);
		void scold(int *menuCounter, bool *confirmPage);
		void loadFile();
		void saveData();
		void resetAnimationCounter();
		void setPoopStandard();
		int randomNum(int min, int max);
};