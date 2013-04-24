/*
 * ObjAnim.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#ifndef OBJANIM_HPP_
#define OBJANIM_HPP_

#define SOURCE_TYPES	3	// ile rodzajow grafik dla jednego zrodla
#define ANIM_FRAMES 	4	// ilosc klatek na animacje - klatek na jeden poziomy wiersz
#define ANIM_TYPES	 	1	// ilosc animacji - ilosc poziomych wierszy

// ANIM_FRAMES = 4, ANIM_TYPES = 2
// #|#|#|#
// #|#|#|#

#include <string>
#include <vector>
#include "Adds.hpp"
#include "Alg.hpp"

class ObjAnim
{
    public:
		struct s_animation
		{
			bool isAnimation;		// czy zasób jest obrazkiem czy animacja
			bool animate;			// animuj lub nie animuj
			bool reverse;			// true = animuj do konca i z powrotem
			bool rev_up;			// dla trybu reverse - jesli true - anim do przodu
			int timer;				// milisekundy
			int speed;				// co ile milisekund zmien animacje
			bool multiline;			// wersja animacji: true - 4 liniowa animacja dla obiektow chodzacych, false - animacji 1 liniowa
			Adds::vertex frame_xy;	// ile klatek na animacje (x), oraz ilosc rodzajow animacji (y)-poziomych wierszy
			Adds::vertex frame;		// x => 0-4 aktualna klatka animacji w poziomie, y => 0-4 zestaw klatek w pionie (4 kierunki swiata)
			Adds::vertex frame_size;// rozmiar malej klatki 1 z (4*4)
		};
		s_animation animation;
		ObjAnim();
        virtual ~ObjAnim();
		int res_file;				// przypisany obrazek do obiektu
		Adds::vertex coords;		// wspolrzedne obiektu
		Adds::vertex size;			// rozmiar obrazka
		bool visible;				// czy widoczny
		bool isWall;				// czy przez obiekt mozna przejsc
		bool border;				// czy wyswietlac obramowanie
		int time;					// aktualny czas w milisekundach
		void changeAnim(int current_time);			// zaktualizuj klatke animacji
		bool resSetUp;				// przy pierwszej probie wyswietlenia animacji ustaw parametry RESOURCES - zmien flage na false
};

#endif /* OBJANIM_HPP_ */
