/*
 * Graphics.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#define WHITE 	0xAAAAAA
#define BLACK 	0x000000
#define RED		0xCC0000
#define GREEN	0x00AA00
#define BLUE	0x0000AA

#define MARGIN				50 // szerokosc menu androida - nie widomo ile
#define MENU_HEIGHT 		70
#define MENU_BUTTON_SIZE	50
#define MENU_MARGIN 		10
#define MENU_SEPARATOR 		10

#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <maxtoa.h>
#include <MAHeaders.h>
#include <string>
#include <vector>
#include "Adds.hpp"
#include "Object.hpp"
#include "Game.hpp"
#include "ObjAnim.hpp"
//#include <MAUI/Scaler.h> // , maui.lib // ,  %mosync-home%/include/MAUI/

class Graphics
{
	public:
		Graphics(Game *game_in);
		virtual ~Graphics();
		void doGraphics();
		void moveMap();
		Adds::vertex bgSize;					// rozmiar pojedynczego obrazka tla
		Adds::vertex screenSize;				// rozdzielczosc ekranu
		Adds::vertex gameAreaSize;				// rozdzielczosc pola gry
		Adds::vertex pointerPressedCoords;		// wspolrzedne po kliknieciu
		Adds::vertex pointerDraggedCoords;		// wspolrzedne podczas przesuwania
		Adds::vertex pointerReleasedCoords;		// wspolrzedne zwolnienia przycisku
	private:
		void printOnScreen();
		void printBackground();
		void printObject(Object *obj, int addRes = 0);
		void printObjAnim(ObjAnim *objAnim, int addRes = 0);
		void printMenu();
		void printBorder(Object *obj, const int border_size, int rgb);
		void preResArray();
		void setResourceSize(Object *obj);
		void printDiscoveryMap();
		Game *game;
		MAHandle graph_buffer;
		MAHandle bg_buffer;
		MAHandle bg_menu;
		int fps, fps_time, fps_print;
		typedef struct res_params
		{
			int res_file;
			Adds::vertex size;
		};
		std::vector<res_params> res_array;
		void debugPrintInt(Adds::vertex coords, int var);								// wyswietl jakas liczbe (debug mode)
};

#endif /* GRAPHICS_HPP_ */
