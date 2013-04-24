/*
 * Game.hpp
 *
 *  Created on: Dec 14, 2012
 *      Author: brzeszczot
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#define VERSION 				0.2
#define DEBUG					false
#define AREA_SIZE_X 			2000
#define AREA_SIZE_Y 			2000
#define DISCOVERY_PIECE_X		50
#define DISCOVERY_PIECE_Y		50

#define MENU_CATEGORY_NONE		0
#define MENU_CATEGORY_MAIN		1
#define MENU_CATEGORY_OBJECT	2

#include <string>
#include <vector>
#include <typeinfo>
#include "Object.hpp"
#include "ObjMove.hpp"
#include <MAHeaders.h>	// stale zasobow
#include <maxtoa.h>

class Game
{
	public:
		Game();
		virtual ~Game();																										// obiekt chodzacy = true
		Object* addObject(Adds::vertex coords, Object::e_group grp, size_t owner_grp = DEFAULT_WORLD_OWNER, Object *own = NULL, bool mv = false);	// dodaj obiekt do gry
		void deleteObject(int index);						// usun obiekt o wskazanym indeksie
		void setRandomObjects();							// dodaj losowo obiekty do gry
		void doLogic(int current_time);						// zaktualizuj logike gry
		void centralizeMap();								// przesun robocza mape na srodek wyswietlacza
		Adds::vertex gameScreenSize;						// rozmiar ekranu
		Adds::vertex map_moving_temp;						// temp
		Adds::vertex map_moving;							// przesuniecie mapy
		Adds::vertex coord_central;							// wspolrzedne srodka mapy
		std::vector<Object*> *objects;						// kontener z obiekatami
		ObjMove* selectedHumanObject;						// wskaznik do aktualnie zaznaczonego obiektu
		bool selectedAction;								// tryb wykonywania akcji
		int isMenu;											// rodzaj menu (wylaczone = MENU_CATEGORY_NONE)
		bool mapEditMode;									// tryb edycji mapy
		Object *editObj;									// obiekt zaznaczony w trybie edycji mapy
		Adds::vertex discoverySize;							// rozmiar macierzy zacieniowania mapy
		bool **discoveryMap;								// macierz z polami do odkrycia
		bool discoveryMapOn;								// wlacz wylacz zakrywanie mapy
	private:
		void addMaterial(Object *obj, Object::e_group grp);	// dodaj obiekt typu surowiec robotnika
		Adds::vertex gameAreaSize;							// rozmiar obszaru gry
};

#endif /* GAME_HPP_ */
