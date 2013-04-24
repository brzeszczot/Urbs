/*
 * ObjMove.hpp
 *
 *  Created on: Mar 26, 2013
 *      Author: brzeszczot
 */

#ifndef OBJMOVE_HPP_
#define OBJMOVE_HPP_

#define WORK_TIME 				3000
#define MAX_WALKING_DISTANCE	10		// raz na 10 krokow zmniejsz jednostke zycia

#include <string>
#include <vector>
#include "Object.hpp"
#include "Adds.hpp"
#include "Alg.hpp"

class ObjMove: public Object
{
	public:
		typedef enum e_stat {WAITING, STOP, MOVING};	// tryby poruszania sie
		typedef enum e_work_mode {STANDBY, GO, WORK, BACK, HOME}; // tryby pracy
		struct s_action
		{
			Adds::vertex dest;		// gdzie zakonczyc efekt dzialania akcji (wspolrzedne)
			Adds::vertex gotowork;	// gdzie chodzic do pracy
			int work_time;			// czas spedzony w czasie akcji
			int work_amount;		// wartosc zdota podczas pracy
	        e_work_mode work_mode;	// tryb pracy
	        bool busy;				// jesli prawda - pracownik cos niesie
	        Object *obj_work;		// obiekt w ktorym pracuje obiekt
		};
		ObjMove();
		virtual ~ObjMove();
        void Stop();				// zatrzymaj obiekt
        void DoNothing();			// stop + wyzerowanie wspolrzednych docelowych
        void Update(int current_time, std::vector<Object*> *objects);	// zaktualizuj jego parametry
        void Go(Adds::vertex dest_coord, bool user = false);	// idz do okreslonej wspolrzednej
        void Go();					// idz tam gdzie miales isc :)
        bool changeMovement();		// zmien pozycje obiektu
		void actionUpdate();		// zaktualizuj akcje
		void actionWork();			// tryb pracy
		void actionStart(Object *obj, Adds::vertex where);			// rozpoczecie akcji praca
		void actionStop();			// przerwij prace
        e_stat status;				// status objektu
		s_action action;			// dane zwiazane z praca
		int walkCounter;			// licznik krokow obiektu jak chodzi
		int direction;				// kierunek poruszania sie
		Object *clickedObject;		// zachowaj obiekt na ktory kliknieto
		Object *objOverLap;
		Object *eatFood;			// obiekt ktory idziemy zjesc :)
		bool wasMoved;				// obiekt przemiscil sie w danej iteracji
	private:
		Object* checkColision(std::vector<Object*> *objects);	// 1: jesli byla kolizja zwroc adres objektu kolizji
		void manageColision(Object *col_obj);								// 2: zarzadzaj procesem omijania
		void firstColisionAvoid(ObjMove *obj, Object *col_obj);				// 3: pierwsze ominiecie obiektu
		void nextColisionAvoid(ObjMove *obj);								// 4: kolejne ominiecia
		int move_speed;        		 // szybkosc poruszania sie
		int direction_last;
		Adds::vertex user_destination;	// wspolrzedne gdzie ma sie przemieszczac obiekt - tymczasowa ostatnia
		Adds::vertex destination;	// wspolrzedne gdzie ma sie przemieszczac obiekt
		int destination_current;	// aktualna wspolrzedna z trasy drogi docelowej - czyli index z ponizszego vectora
		std::vector<Adds::vertex> *destination_coords;	// wspolrzedne odcinka destynacji
		int move_timer;				// milisekundy
		Object *colisionWith;		// Objekt z ktorym byla kolizja
		int colisionCorner;			// numer rogu z którym byla kolizja 0-GL, 1-GP, 2-DP, 3-DL
		int goToCorner;				// wierzcholek do ktorego podoza obiekt omijajac obiekt
		bool avoidDirection;
};

#endif /* OBJMOVE_HPP_ */
