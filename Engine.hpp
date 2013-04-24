/*
 * Engine.hpp
 *
 *  Created on: Nov 17, 2012
 *      Author: brzeszczot
 */

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#define FPS		30	///

//#include <mastdlib.h>
#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <maxtoa.h>
#include <string>
#include <vector>
#include "Adds.hpp"
#include "Object.hpp"
#include "Graphics.hpp"
#include "Game.hpp"
#include "Polygon.hpp"

//#include <IX_WIDGET.h>
using namespace MAUtil;

class Engine: public Moblet
{
	public:
		Engine();
		virtual ~Engine();
	private:
		Graphics *graphics;											// uchwyt z grafika
		Game *game;													// uchwyt do danych z gra
		bool isRunning;												// true -glowna petla dziala
		MAEvent event;
		int gameId;													// losowa liczba GUID gry
		void Run();													// start dzialania programu
		void debugPrintInt(int var);								// wyswietl jakas liczbe (debug mode)
		Object* checkObjectSelection(Adds::vertex coord);			// zwraca klikniety obiekt albo NULL jesli nie kliknieto w zaden obiekt
		void selectObject(ObjMove *obj = NULL);						// zaznacz/odznacz obiekt
		Object* selectActionObject(Object *obj, Adds::vertex coord);	// wybierz obiekt z ktorym bedzie zachodzila interakcja obiektu
		int menuButtonPressed();									// zwroc numer kliknietego przycisku z menu
		bool menuLogic(int button);									// obsluzenie przyciskow menu
		Adds::vertex coodrsByMap(Adds::vertex coord, Adds::vertex add);				// wspolrzedne z uwzglednieniem przesuniecia mapy
		void saveMap();												// zapisz mape do pliku
		void readMap();												// wczytaj mape z pliku
		time_t gameStartTime;										// timestamp wejscia do gry
		float fps;													// licznik czasu dla regulacji szybkosci pracy programu
		float fpsOnEvery;
		int isMenu;													// rodzaj menu (wylaczone = MENU_CATEGORY_NONE)
		bool mapEditMode;											// tryb edycji mapy
};

#endif
