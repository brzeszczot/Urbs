/*
 * Engine.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: brzeszczot
 */

#include "Engine.hpp"

Engine::Engine()
{
	srand(maTime());
	gameId = 1000 + (rand() % 100000);
	gameStartTime = time(0);
	fpsOnEvery = 1000 / FPS;			// co ile ms klatka. np: FPS = 30, 1000ms / 30 klatek = 33.33333 ms - co 33.33 ms zmien klatke

	game = new Game();
	graphics = new Graphics(game);

	isRunning = true;


/*
		int len = maGetDataSize(TEXTFILE);
		char buffer[len + 1];
		maReadData(TEXTFILE, &buffer, 0, len);
		buffer[len] = '\0';
		int xxx;
		sscanf (buffer, "%d;", &xxx);
		maMessageBox("Debug",buffer);
		debugPrintInt(xxx);
*/




	///////////////////////////////////////
	game->centralizeMap();
	game->setRandomObjects();
	//readMap();
	///////////////////////////////////////
	Run();
}

Engine::~Engine()
{
	delete game, graphics;
}

void Engine::Run()
{
	// glowna petla gry
	while (isRunning)
	{
		// spowalniaj program do stalej predkosci
		//if(maGetMilliSecondCount() - fps >= fpsOnEvery)
		//	fps = maGetMilliSecondCount();
		//if(maGetMilliSecondCount() - fps >= 250)
		//	fps = maGetMilliSecondCount();
		//else continue;

		// zmien logike i wyswietl
		if(game->isMenu != MENU_CATEGORY_MAIN) // tylko jak nie jestesmy w menu glownym
			game->doLogic(maGetMilliSecondCount());
		graphics->doGraphics();

		// petla zdarzen
		while (maGetEvent(&event))
		{
			if(EVENT_TYPE_SCREEN_CHANGED == event.type)
			{
				int ssize = maGetScrSize();
				graphics->screenSize.x = EXTENT_X(ssize);
				graphics->screenSize.y = EXTENT_Y(ssize);
			}

			if(event.nativeKey==82 && game->isMenu != MENU_CATEGORY_MAIN)
			{
				selectObject();	// odznacz obiekt
				game->isMenu = MENU_CATEGORY_MAIN;
			}

			if (EVENT_TYPE_CLOSE == event.type)
			{
				isRunning = false;
				//maExit(0);
				break;
			}
			else if (EVENT_TYPE_KEY_PRESSED == event.type)
			{
				switch(event.key)
				{
					case MAK_BACK: isRunning = false; break;
					//case MAK_MENU:
				}
			}
			else if (EVENT_TYPE_POINTER_DRAGGED == event.type)
			{
				graphics->pointerDraggedCoords.x = event.point.x;
				graphics->pointerDraggedCoords.y = event.point.y;
				// przesuwaj mape tylko gdy nie jestesmy w trybie menu LUB objekt nie jest zaznaczony i tryb edycji mapy LUB tryb akcji jest wlaczony
				if((game->mapEditMode && game->editObj == NULL && event.point.y < graphics->screenSize.y - MENU_HEIGHT) || (game->selectedHumanObject != NULL && game->selectedAction && event.point.y < graphics->screenSize.y - MENU_HEIGHT) || (game->isMenu == MENU_CATEGORY_NONE && game->selectedHumanObject == NULL))
					graphics->moveMap();

				// dla trybu edycji mapy - przesuwaj zaznaczony obiekt
				if(game->mapEditMode && game->editObj != NULL)
				{
					Adds::vertex c = coodrsByMap(graphics->pointerDraggedCoords, game->editObj->animation.frame_size);
					c.x -= 50;
					c.y -= 50;
					game->editObj->coords = c;
				}
			}
			else if (EVENT_TYPE_POINTER_PRESSED == event.type)
			{
				graphics->pointerPressedCoords.x = event.point.x;
				graphics->pointerPressedCoords.y = event.point.y;
				game->map_moving_temp = game->map_moving;

				Adds::vertex dest;
				dest.x = event.point.x;
				dest.y = event.point.y;

				// sprawdz przyciski menu i obsluz je | jesli menuLogic zwroci prawde - odznacz obiekt
				if(game->isMenu != MENU_CATEGORY_NONE && event.point.y >= graphics->screenSize.y - MENU_HEIGHT)
					if(menuLogic(menuButtonPressed()))
					{
						game->selectedAction = false;
						selectObject();
					}

				// sprwdzenie czy kliknieto w obiekt - jesli nie zwroc NULL
				Object *obj = checkObjectSelection(dest);
				ObjMove *objMove = NULL;
				if(obj->animation.multiline && obj != NULL)
					objMove = static_cast<ObjMove*>(obj);

				// dla trybu edycji mapy
				if(game->mapEditMode && obj != NULL)
				{
					game->editObj = obj;
					game->editObj->border = true;

					game->isMenu = MENU_CATEGORY_MAIN;
				}
				// dla normalnego trybu gry
				else if(!game->mapEditMode)
				{
					// zaznacz / odznacz obiekt
					if(game->isMenu != MENU_CATEGORY_MAIN && !game->selectedAction)
					{
						if(objMove != NULL && objMove->owner_group == DEFAULT_HUMAN_OWNER && objMove != game->selectedHumanObject && !objMove->obj_unused)
							selectObject(objMove);
						else if(objMove != NULL && objMove->owner_group == DEFAULT_HUMAN_OWNER && objMove == game->selectedHumanObject)
							selectObject();
					}

					// w trybie akcji
					Object *objWorkWith = NULL;
					if(obj != NULL && game->selectedHumanObject != NULL && game->selectedAction) // && obj->owner == DEFAULT_WORLD_OWNER
						objWorkWith = selectActionObject(obj, dest);

					// wpraw obiekt w ruch
					if(game->selectedHumanObject!=NULL && event.point.y < graphics->screenSize.y - MENU_HEIGHT)
					{
						// zwyczajne wskazanie obiektowi gdzie ma isc
						if(obj!=game->selectedHumanObject && !game->selectedAction && game->selectedHumanObject->action.work_mode == ObjMove::STANDBY)
							game->selectedHumanObject->Go(coodrsByMap(dest, game->selectedHumanObject->animation.frame_size), true);
						// wybrano obiekt z akcja
						else if(objWorkWith != NULL)
							game->selectedHumanObject->Go(game->selectedHumanObject->action.gotowork, true);
					}
				}
			}
			else if (EVENT_TYPE_POINTER_RELEASED == event.type)
			{
				graphics->pointerReleasedCoords.x = event.point.x;
				graphics->pointerReleasedCoords.y = event.point.y;
				// dla trybu edycji mapy
				if(game->mapEditMode && game->editObj != NULL)
				{
					game->editObj->border = false;
					game->editObj = NULL;
				}
			}
		}
	}

	maExit(0);
}

Object* Engine::checkObjectSelection(Adds::vertex coord)
{
	int treshold = 5;
	Object *obj;
	for(int ii=0;ii<game->objects->size();ii++)
	{
		obj = (*game->objects)[ii];
		if(obj->enabled)
		{
			if((coord.x >= (obj->coords.x + game->map_moving.x) - treshold && coord.x <= (obj->coords.x + game->map_moving.x) + obj->animation.frame_size.x + treshold) &&
					(coord.y >= (obj->coords.y + game->map_moving.y) - treshold && coord.y <= (obj->coords.y + game->map_moving.y) + obj->animation.frame_size.y + treshold))// && obj!=game->selectedHumanObject)
			{
				game->selectedHumanObject->clickedObject = obj;
				return obj;
			}
		}
	}
	game->selectedHumanObject->clickedObject = NULL;
	return NULL;
}

void Engine::selectObject(ObjMove *obj)
{
	if(game->selectedHumanObject!=NULL)
		game->selectedHumanObject->border = false;
	if(obj != NULL)
	{
		obj->border = true;
		game->selectedHumanObject = obj;
		game->isMenu = MENU_CATEGORY_OBJECT;
	}
	else
	{
		obj->border = false;
		game->selectedHumanObject = NULL;
		game->isMenu = MENU_CATEGORY_NONE;
	}
}

Object* Engine::selectActionObject(Object *obj, Adds::vertex coord)
{
	bool fit_source = false;
	bool fit_material = false;
	switch(game->selectedHumanObject->group)
	{
		case Object::WORKER_ROCK: 	if(obj->group == Object::SOURCE_ROCK)	fit_source = true;	break;
		case Object::WORKER_WOOD: 	if(obj->group == Object::SOURCE_WOOD)	fit_source = true;	break;
		case Object::WORKER_LAKE: 	if(obj->group == Object::SOURCE_LAKE)	fit_source = true;	break;
	}
	// jesli ktorys z robotnikow - moga uzyc materialu z jeziora
	if(game->selectedHumanObject->params.life < MAX_LIFE)
	{
		switch(game->selectedHumanObject->group)
		{
			case Object::WORKER_ROCK: 	if(obj->group == Object::MATERIAL_LAKE)	fit_material = true;	break;
			case Object::WORKER_WOOD: 	if(obj->group == Object::MATERIAL_LAKE)	fit_material = true;	break;
			case Object::WORKER_LAKE: 	if(obj->group == Object::MATERIAL_LAKE)	fit_material = true;	break;
		}

		// jesli wybrano material
		if(fit_material)
		{
			game->selectedAction = false;
			game->selectedHumanObject->eatFood = obj;
			return obj;
		}
	}

	// jesli wybrano zrodlo
	if(fit_source && obj->params.life > 0)
	{
		game->selectedHumanObject->actionStart(obj, coodrsByMap(coord, game->selectedHumanObject->animation.frame_size));
		game->selectedAction = false;
		return obj;
	}
	return NULL;
}

int Engine::menuButtonPressed()
{
	Adds::vertex vc, vs;
	for(int ii=0;ii<6;ii++)
	{
		vc.x = ii * ((MENU_SEPARATOR * 2) + MENU_BUTTON_SIZE);
		vc.y = (graphics->screenSize.y - MENU_HEIGHT);
		vs.x = vs.y = MENU_BUTTON_SIZE + (MENU_SEPARATOR * 2);
		if(Adds::checkOverlap(graphics->pointerPressedCoords, vc, vs))
			return ii;
	}
	return -1;
}

Adds::vertex Engine::coodrsByMap(Adds::vertex coord, Adds::vertex add)
{
	coord.x = coord.x + std::abs(game->map_moving.x);
	coord.y = coord.y + std::abs(game->map_moving.y);
	coord.x -= (add.x/2);
	coord.y -= (add.y/2);
	//coord.x -= (obj->animation.frame_size.x/2);
	//coord.y -= (obj->animation.frame_size.y/2);

	return coord;
}

bool Engine::menuLogic(int button)
{
	if(button != -1)
	{
		switch(game->isMenu)
		{
			case MENU_CATEGORY_MAIN:
				switch(button)
				{
					case 0: return true; break;	// odznacz objekt i schowaj menu
					case 1: if(game->mapEditMode)
							{
								game->discoveryMapOn = true;
								//saveMap();
							}
							else
								game->discoveryMapOn = false;
							game->mapEditMode = !game->mapEditMode;
							if(game->editObj != NULL)
								game->editObj->border = false;
							game->editObj = NULL;
							break;
					case 2: isRunning = false; break;
				}
				break;
			case MENU_CATEGORY_OBJECT:
				if(game->selectedHumanObject != NULL)
				{
					switch(button)
					{
						case 0: return true; break;	// odznacz objekt i schowaj menu
						case 1: if(game->selectedHumanObject->status == ObjMove::STOP)
									game->selectedHumanObject->Go();
								else if(game->selectedHumanObject->status == ObjMove::MOVING)
									game->selectedHumanObject->Stop();
								break;
						case 2: if(game->selectedAction)
									game->selectedAction = false;
								else if(game->selectedHumanObject->action.work_mode == ObjMove::STANDBY)
									game->selectedAction = true;
								else if(game->selectedHumanObject->action.work_mode != ObjMove::STANDBY)
									game->selectedHumanObject->actionStop();
								break;
					}
				}
				break;
		}
	}
	return false;
}

void Engine::readMap()
{
	char full_path[256]={0};
	maGetSystemProperty("mosync.path.local", full_path, sizeof(full_path));
	strcat(full_path, "urbs.map");
	FILE *file = fopen(full_path, "r");
	if(!file) maPanic(1, "Could not open file!");
	while(!feof(file))
	{
		Object *obj = new Object();
		obj->owner_group = DEFAULT_WORLD_OWNER;
		obj->isWall = true;

		//int res;
		fscanf (file, "%d;", &obj->coords.x);
		fscanf (file, "%d;", &obj->coords.y);
		fscanf (file, "%d;", &obj->group);
		fscanf (file, "%d;", &obj->res_file);

		//graphics->setResource(obj, RES_WORKER_ROCK + (--res));
		game->objects->push_back(obj);
	}
	fclose (file);
	game->objects->pop_back();
}

void Engine::saveMap()
{
	char buffor_int[10];
	itoa((int)gameId, buffor_int, 10);

	char full_path[256]={0};
	strcpy(full_path, "/mnt/sdcard/");
	//maGetSystemProperty("mosync.path.local", full_path, sizeof(full_path)); // zwraca to: /data/data/com.brzeszczot.urbs/files/
	strcat(full_path, Adds::getStrDate(gameStartTime).c_str());
	strcat(full_path, "_");
//	strcat(full_path, getStrTime(gameStartTime).c_str());
//	strcat(full_path, "_");
	strcat(full_path, buffor_int);
	strcat(full_path, "_urbs.map");

	FILE *file = fopen(full_path, "w");
	if(!file) maPanic(1, "Could not save to file!");

	for(int ii=0;ii<game->objects->size();ii++)
	{
		if((*game->objects)[ii]->owner_group == DEFAULT_WORLD_OWNER)
		{
			fprintf (file, "%d;", (*game->objects)[ii]->coords.x);
			fprintf (file, "%d;", (*game->objects)[ii]->coords.y);
			fprintf (file, "%d;", (*game->objects)[ii]->group);
			fprintf (file, "%d;", (*game->objects)[ii]->res_file);
		}
	}
	fclose (file);
}

void Engine::debugPrintInt(int var)
{
	char buffor[32];
	strcpy(buffor,"");
	itoa(var, buffor, 10);
	maMessageBox("Debug",buffor);
}
