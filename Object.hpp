/*
 * Object.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#define MAX_LIFE		100
#define WORK_UNIT		25

#include <string>
#include <vector>
#include "Adds.hpp"
#include "ObjAnim.hpp"

class Object: public ObjAnim
{
    public:
		typedef enum e_group {DEFAULT, BASE, WORKER_ROCK, WORKER_WOOD, WORKER_LAKE, SOURCE_ROCK, SOURCE_WOOD, SOURCE_LAKE, MATERIAL_ROCK, MATERIAL_WOOD, MATERIAL_LAKE}; // grupy obiektow
		struct parameters
		{
            int life;               // zycie
            int life_unit;			// jednostka zmniejszania zycia :)
            int gold;               // zloto :)
		    int skills;				// umiejetnosci
		    int visible_dist;       // na jaka odleglosc objekt potrafi widziec
		    int range;          	// zasieg
		};
		parameters params;
		e_group group;				// grupa
		Object *owner;				// kto jest wlascicielem obiektu
		size_t owner_group;			// grupa - WORLD, USER, ETC
		bool enabled;				// objekt wlaczony wylaczony
		bool obj_unused;			// true - jesli obiekt ma byc wyswietlany ale jako zuzyty
        Object();
        virtual ~Object();
		void enableObject(bool yes);			// wlacz wylacz obiekt
		void unusedObject();						// przelacz obiekt w tryb wyczerpania / bezuzytecznuy
};

#endif /* OBJECT_HPP_ */
