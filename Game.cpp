/*
 * Game.cpp
 *
 *  Created on: Dec 14, 2012
 *      Author: brzeszczot
 */

#include "Game.hpp"

Game::Game()
{
	gameAreaSize.x = AREA_SIZE_X;
	gameAreaSize.y = AREA_SIZE_Y;
	map_moving.x = map_moving.y = 0;
	objects = new std::vector<Object*>;
	selectedHumanObject = NULL;
	mapEditMode = false;
	editObj = NULL;
	isMenu = MENU_CATEGORY_NONE;
	selectedAction = false;

	//stworzenie macierzy zacieniowania mapy
	discoverySize.x = AREA_SIZE_X / DISCOVERY_PIECE_X;
	discoverySize.y = AREA_SIZE_Y / DISCOVERY_PIECE_Y;
	discoveryMap = new bool*[discoverySize.y];
	for(size_t yy = 0; yy < discoverySize.y; yy++)
	{
		discoveryMap[yy] = new bool[discoverySize.x];
		for(size_t xx=0; xx < discoverySize.x; xx++)
			discoveryMap[yy][xx] = false;
	}
	discoveryMapOn = true;
	if(DEBUG)	discoveryMapOn = false;
}

Game::~Game()
{
    for(size_t ii=0;ii<(size_t)objects->size();ii++)
        delete (*objects)[ii];
    objects->clear();
	delete objects;
}

void Game::centralizeMap()
{
	map_moving.x -= (gameAreaSize.x / 2) - (gameScreenSize.x / 2);
	map_moving.y -= (gameAreaSize.y / 2) - (gameScreenSize.y / 2);
	coord_central.x = (gameAreaSize.x / 2);
	coord_central.y = (gameAreaSize.y / 2);
	int median_y = (discoverySize.y / 2);
	int median_x = (discoverySize.x / 2);
	for(size_t yy = median_y - 1; yy <= median_y + 1; yy++)
		for(size_t xx = median_x - 1; xx <= median_x + 1; xx++)
			discoveryMap[yy][xx] = true;
}

void Game::doLogic(int current_time)
{
	// obliczaj logike tylko w trybie gry
	if(isMenu != MENU_CATEGORY_MAIN)
	{
		// animuj i zmien logike dla objektow
		for(int ii=0;ii<objects->size();ii++)
		{
			if((*objects)[ii]->animation.multiline)
			{
				// aktualizuj obiekty chodzace
				ObjMove *objmov = static_cast<ObjMove*>((*objects)[ii]);

				// zostaw wykopany material na miejscu gdzie stoimy
				if(objmov->action.work_mode == ObjMove::GO && objmov->action.busy)
				{
					objmov->action.busy = false;
					switch(objmov->group)
					{
						case Object::WORKER_ROCK: addMaterial(objmov, Object::MATERIAL_ROCK); break;
						case Object::WORKER_WOOD: addMaterial(objmov, Object::MATERIAL_WOOD); break;
						case Object::WORKER_LAKE: addMaterial(objmov, Object::MATERIAL_LAKE); break;
					}

					// przerwij chodzenie do pracy - zrodlo sie wyczerpalo
					if(objmov->action.obj_work != NULL)
					{
						if(objmov->action.obj_work->params.life <= 0)
						{
							//deleteObject(ii);
							objmov->DoNothing();
							objmov->action.work_mode = ObjMove::STANDBY;
							objmov->action.obj_work = NULL;
						}
					}
				}

				objmov->Update(current_time, objects);

				// zmiana pola widzenia discover
				if(discoveryMapOn && objmov->wasMoved)
				{
					int x = (int)(objmov->coords.x + (objmov->animation.frame_size.x / 2)) / DISCOVERY_PIECE_X;
					int y = (int)(objmov->coords.y + (objmov->animation.frame_size.y / 2)) / DISCOVERY_PIECE_Y;
					discoveryMap[y][x] = true;
					switch(objmov->direction)
					{
						case 0: discoveryMap[y-1][x] = true; discoveryMap[y-1][x-1] = true; discoveryMap[y-1][x+1] = true; break;
						case 1: discoveryMap[y][x+1] = true; discoveryMap[y-1][x+1] = true; discoveryMap[y+1][x+1] = true; break;
						case 2: discoveryMap[y+1][x] = true; discoveryMap[y+1][x-1] = true; discoveryMap[y+1][x+1] = true; break;
						case 3: discoveryMap[y][x-1] = true; discoveryMap[y-1][x-1] = true; discoveryMap[y+1][x-1] = true; break;
					}
					objmov->wasMoved = false;
				}
			}

			Object *obj = (*objects)[ii];

			// remove empty obiekt jesli parametry zycia mniejsze rowne zero
			if(!obj->obj_unused && obj->params.life <= 0)
			{
				obj->unusedObject();
				if(obj->animation.multiline)
				{
					Object *o = addObject(obj->coords, Object::DEFAULT);
					o->res_file = RES_RIP;
					o->isWall = false;
					o->animation.isAnimation = false;
					selectedHumanObject = NULL;
					isMenu = MENU_CATEGORY_NONE;
				}
			}

			// zaktualizuj animacje
			obj->changeAnim(current_time);
		}
	}
}

void Game::addMaterial(Object *obj, Object::e_group grp)
{
	Adds::vertex coords;

	coords.x = obj->coords.x + rand() % 15;
	coords.y = obj->coords.y + rand() % 25;
	addObject(coords, grp, DEFAULT_HUMAN_OWNER, obj);
}

void Game::deleteObject(int index)
{
	delete (*objects)[index];
	objects->erase(objects->begin() + index);
	/*
	Object* f = NULL;
	std::vector<Object*>::iterator it;
	std::find(objects->begin(), objects->end(), f);
	if (it != objects->end())
		objects->erase(it);
	*/
}

Object* Game::addObject(Adds::vertex coords, Object::e_group grp, size_t owner_grp, Object *own, bool mv)
{
	Object *obj;
	if(mv)
		obj = new ObjMove();
	else
		obj = new Object();
	obj->coords = coords;
	obj->group = grp;
	obj->owner_group = owner_grp;
	obj->owner = own;
	switch(obj->group)
	{																					// obiekt stoi przodem 		// wylacz animacjae - obiekt stoi :)
		case Object::WORKER_ROCK: obj->res_file = RES_WORKER_ROCK; obj->isWall = false; break;
		case Object::WORKER_WOOD: obj->res_file = RES_WORKER_WOOD; obj->isWall = false; break;
		case Object::WORKER_LAKE: obj->res_file = RES_WORKER_LAKE; obj->isWall = false; break;
		case Object::SOURCE_ROCK: obj->res_file = RES_SOURCE_ROCK1 + ((int)rand() % SOURCE_TYPES); obj->animation.reverse = true; break;
		case Object::SOURCE_WOOD: obj->res_file = RES_SOURCE_TREE1 + ((int)rand() % SOURCE_TYPES); obj->animation.reverse = true; break;
		case Object::SOURCE_LAKE: obj->res_file = RES_SOURCE_LAKE1 + ((int)rand() % SOURCE_TYPES); obj->animation.reverse = true; break;
		case Object::MATERIAL_ROCK: obj->res_file = RES_MATERIAL_ROCK; obj->isWall = false; obj->animation.reverse = true; break;
		case Object::MATERIAL_WOOD: obj->res_file = RES_MATERIAL_WOOD; obj->isWall = false; obj->animation.reverse = true; break;
		case Object::MATERIAL_LAKE: obj->res_file = RES_MATERIAL_LAKE; obj->isWall = false; obj->animation.reverse = true; break;
	}
	if(mv)
	{
		obj->animation.multiline = true;
		obj->animation.frame.y = 2;
		obj->animation.animate = false;
	}
	objects->push_back(obj);

	if(!mv)	return obj;
	else return NULL;
}

void Game::setRandomObjects()
{
	Adds::vertex coords;

	// dodaj kilka obiektow gracza
	for(int ii=0;ii<3;ii++)
	{
		coords.x = (coord_central.x - 40) + (ii * 40);
		coords.y = coord_central.y;
		Object::e_group grp;
		switch(ii)
		{
			case 0: grp = Object::WORKER_ROCK; break;
			case 1: grp = Object::WORKER_WOOD; break;
			case 2: grp = Object::WORKER_LAKE; break;
		}
		addObject(coords, grp, DEFAULT_HUMAN_OWNER, NULL, true);
	}

	// jaskinie
	for(int ii=0;ii<10;ii++)
	{
		coords.x = (rand() % (gameAreaSize.x - 50));
		coords.y = (rand() % (gameAreaSize.y - 50)) + 50;
		Object  *obj = addObject(coords, Object::SOURCE_ROCK);
	}

	// drzewa
	for(int ii=0;ii<10;ii++)
	{
		coords.x = (rand() % (gameAreaSize.x - 50));
		coords.y = (rand() % (gameAreaSize.y - 50)) + 50;
		addObject(coords, Object::SOURCE_WOOD);
	}

	// jeziorka
	for(int ii=0;ii<10;ii++)
	{
		coords.x = (rand() % (gameAreaSize.x - 50));
		coords.y = (rand() % (gameAreaSize.y - 50)) + 50;
		addObject(coords, Object::SOURCE_LAKE);
	}

}


