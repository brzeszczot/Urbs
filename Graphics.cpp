/*
 * Graphics.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#include "Graphics.hpp"

Graphics::Graphics(Game *game_in)
{
	game = game_in;

	maScreenStateEventsOn();
	maScreenSetOrientation(SCREEN_ORIENTATION_DYNAMIC);

	graph_buffer = maCreatePlaceholder();
	int ssize = maGetScrSize();
	screenSize.x = EXTENT_X(ssize);
	screenSize.y = EXTENT_Y(ssize);
	game->gameScreenSize.x = screenSize.x;	// przekazanie wspolrzednych ekranu do klasy GAME
	game->gameScreenSize.y = screenSize.y;
	int screenSizeLonger = std::max(screenSize.x, screenSize.y);
	maCreateDrawableImage(graph_buffer, screenSizeLonger + MARGIN, screenSizeLonger + MARGIN);	// kiepskie rozwiazanie ale inaczej ratacja ekranu ne dziala dobrze

	// przygotowanie tla
	gameAreaSize.x = AREA_SIZE_X;
	gameAreaSize.y = AREA_SIZE_Y;

	// przygotuj vector z rozmiarami zasobów resurces
	preResArray();

	// przygotuj tlo menu (screenSize.x)
	bg_menu = maCreatePlaceholder();
	maCreateDrawableImage(bg_menu, screenSizeLonger + MARGIN, MENU_HEIGHT);
	maSetDrawTarget(bg_menu);
	maSetColor(BLACK);
	maFillRect(0, 0, screenSizeLonger + MARGIN, MENU_HEIGHT);
	maSetColor(0x222222); maLine(0, 2, screenSizeLonger + MARGIN, 2);
	maSetColor(0x444444); maLine(0, 1, screenSizeLonger + MARGIN, 1);
	maSetColor(0x666666); maLine(0, 0, screenSizeLonger + MARGIN, 0);

	// przygotowanie tla mapy
	MAHandle bg_picture = RES_BG1;
	bgSize.x = EXTENT_X(maGetImageSize(bg_picture));
	bgSize.y = EXTENT_Y(maGetImageSize(bg_picture));
	bg_buffer = maCreatePlaceholder();
	maCreateDrawableImage(bg_buffer, gameAreaSize.x, gameAreaSize.y);
	maSetDrawTarget(bg_buffer);
	maSetColor(BLACK);
	maFillRect(0, 0, gameAreaSize.x, gameAreaSize.x);
	for(int yy=0;yy<(int)(gameAreaSize.y/bgSize.y)+1;yy++)
		for(int xx=0;xx<(int)(gameAreaSize.x/bgSize.x)+1;xx++)
			maDrawImage(bg_picture, (int)bgSize.x*xx, (int)bgSize.y*yy);

	// wygeneruj statyczna grafike
	//for(int ii=0;ii<80;ii++)
	//	maDrawImage(RES_BG_ROCKS, rand() % gameAreaSize.x, rand() % gameAreaSize.y);
	//for(int ii=0;ii<50;ii++)
	//	maDrawImage(RES_TREE1, rand() % gameAreaSize.x, rand() % gameAreaSize.y);

	// ustaw glowny buffor kreslenia grafiki
	maSetDrawTarget(graph_buffer);
}

void Graphics::doGraphics()
{
	printBackground();

	for(int ii=0;ii<game->objects->size();ii++)
	{
		ObjMove *obj = static_cast<ObjMove*>((*game->objects)[ii]);
		if(obj->group == Object::WORKER_ROCK || obj->group == Object::WORKER_WOOD || obj->group == Object::WORKER_LAKE)
			continue;
		printObject(obj);
	}
	for(int ii=0;ii<game->objects->size();ii++)
	{
		ObjMove *obj = static_cast<ObjMove*>((*game->objects)[ii]);
		if(obj->group != Object::WORKER_ROCK && obj->group != Object::WORKER_WOOD && obj->group != Object::WORKER_LAKE)
			continue;
		// dorysuj plecak dla obiektu ruchomego jak jest w trybie pracy
		if(obj->action.work_mode == ObjMove::BACK)
			printObject(obj, RES_WORKER_BACKPACK);
		else
			printObject(obj);

		// rysuj namioty podczas pracy
		if(obj->action.work_mode == ObjMove::WORK)
		{
			int tent_res;// = RES_TENT_ROCK;
			switch(obj->group)
			{
				case Object::WORKER_ROCK: tent_res = RES_TENT_ROCK;	break;
				case Object::WORKER_WOOD: tent_res = RES_TENT_WOOD;	break;
				case Object::WORKER_LAKE: tent_res = RES_TENT_LAKE;	break;
			}
			maDrawImage(tent_res, (obj->coords.x + game->map_moving.x) - 12, obj->coords.y + game->map_moving.y);
		}
	}
	if(game->selectedHumanObject!=NULL)
	{
		if(game->selectedHumanObject->action.work_mode == ObjMove::BACK)
			printObject(game->selectedHumanObject, RES_WORKER_BACKPACK);
		else
			printObject(game->selectedHumanObject);

		if(0)//DEBUG)
		{
			Adds::vertex c = game->selectedHumanObject->coords;
			c.y += 12;
			debugPrintInt(c, game->selectedHumanObject->walkCounter);
		}
	}

	if(game->discoveryMapOn)
		printDiscoveryMap();

	if(game->isMenu != MENU_CATEGORY_NONE)
		printMenu();

	printOnScreen();
}

void Graphics::setResourceSize(Object *obj)
{
	// jesli obiekt chodzacy - ustaw 4 rodzaje animacji
	if(obj->group == Object::WORKER_ROCK || obj->group == Object::WORKER_WOOD || obj->group == Object::WORKER_LAKE)
		obj->animation.frame_xy.y = 4;
	obj->size.x = res_array[obj->res_file-1].size.x;
	obj->size.y = res_array[obj->res_file-1].size.y;
	obj->animation.frame_size.x = (int) obj->size.x / obj->animation.frame_xy.x;
	obj->animation.frame_size.y = (int) obj->size.y / obj->animation.frame_xy.y;
	// jesli obiekt nie jest animacja - ustaw rozmiar klatki jak rozmiar obrazka
	if(!obj->animation.isAnimation)
	{
		obj->animation.frame_size.x = (int)obj->size.x;
		obj->animation.frame_size.y = (int)obj->size.y;
	}
	obj->resSetUp = false;
}

void Graphics::preResArray()
{
	// RES_BUTTON_EXIT musi byc ostatni na liscie resources!!!!!!!!!!!!!!
	for(int ii=1; ii<=RES_BUTTON_EXIT; ii++)//RES_BUTTON_EXIT
	{
		res_params rp;
		rp.res_file = ii;
		rp.size.x = EXTENT_X(maGetImageSize(RES_WORKER_ROCK + (ii-1)));
		rp.size.y = EXTENT_Y(maGetImageSize(RES_WORKER_ROCK + (ii-1)));
		res_array.push_back(rp);
	}
}

void Graphics::printMenu()
{
	// wyswietle tlo polprzezroczyste tlko dla menu glownego i nie w trybie edycji mapy
	if(!game->mapEditMode && game->isMenu == MENU_CATEGORY_MAIN)
	{
		MAHandle bg_transp = RES_TRANSP;
		Adds::vertex size;
		size.x = EXTENT_X(maGetImageSize(bg_transp));
		size.y = EXTENT_Y(maGetImageSize(bg_transp));
		for(int yy=0;yy<(int)(gameAreaSize.y/size.y)+1;yy++)
			for(int xx=0;xx<(int)(gameAreaSize.x/size.x)+1;xx++)
				maDrawImage(bg_transp, (int)size.x*xx, (int)size.y*yy);
	}

	maDrawImage(bg_menu, 0, screenSize.y - MENU_HEIGHT);
	maDrawImage(RES_BUTTON_OUT, MENU_SEPARATOR, (screenSize.y - MENU_HEIGHT) + MENU_SEPARATOR);
	if(game->isMenu == MENU_CATEGORY_OBJECT)
	{
		int move_button, action_button;
		if(game->selectedHumanObject->status == ObjMove::STOP)//game->selectedHumanObject->destination_coords->size()>0)
			move_button = RES_BUTTON_GO;
		else if(game->selectedHumanObject->status == ObjMove::MOVING)
			move_button = RES_BUTTON_STOP;

		if(game->selectedAction)
			action_button = RES_BUTTON_WORK_SEARCH;
		else if(game->selectedHumanObject->action.work_mode == ObjMove::STANDBY)
			action_button = RES_BUTTON_WORK;
		else if(game->selectedHumanObject->action.work_mode != ObjMove::STANDBY)
			action_button = RES_BUTTON_WORK_STOP;

		maDrawImage(move_button, (MENU_SEPARATOR * 2) + MENU_BUTTON_SIZE + MENU_SEPARATOR, (screenSize.y - MENU_HEIGHT) + MENU_SEPARATOR);
		maDrawImage(action_button, (MENU_SEPARATOR * 4) + (MENU_BUTTON_SIZE * 2) + MENU_SEPARATOR, (screenSize.y - MENU_HEIGHT) + MENU_SEPARATOR);
	}
	else if(game->isMenu == MENU_CATEGORY_MAIN)
	{
		// wyswietl pozostale elementy
		int editmap_button;
		if(game->mapEditMode)	editmap_button = RES_BUTTON_WORK_STOP;
		else					editmap_button = RES_BUTTON_WORK;

		maDrawImage(editmap_button, (MENU_SEPARATOR * 2) + MENU_BUTTON_SIZE + MENU_SEPARATOR, (screenSize.y - MENU_HEIGHT) + MENU_SEPARATOR);
		maDrawImage(RES_BUTTON_EXIT, (MENU_SEPARATOR * 4) + (MENU_BUTTON_SIZE * 2) + MENU_SEPARATOR, (screenSize.y - MENU_HEIGHT) + MENU_SEPARATOR);
	}

	// *6 , *3
	maSetColor(WHITE);
	std::string text = "NONE";
	/*
	switch(game->isMenu)
	{
		case MENU_CATEGORY_MAIN: text = "MENU GLOWNE"; break;
		case MENU_CATEGORY_OBJECT: text = "MENU OBIEKTU"; break;
		default: text = "NONE";
	}
	*/
	maDrawText((MENU_SEPARATOR * 6) + (MENU_BUTTON_SIZE * 3) + MENU_MARGIN, (screenSize.y - MENU_HEIGHT) + MENU_SEPARATOR, text.c_str());
}

void Graphics::printObject(Object *obj, int addRes)
{
	// jesli nie ustawiono parametrow obrazka ustaw je tylko raz
	if(obj->resSetUp)
		setResourceSize(obj);

	Adds::vertex coords, vc, vs;
	coords.x = obj->coords.x + game->map_moving.x;
	coords.y = obj->coords.y + game->map_moving.y;
	vc.x = std::abs(game->map_moving.x) - obj->animation.frame_size.x;
	vc.y = std::abs(game->map_moving.y) - obj->animation.frame_size.y;
	vs.x = screenSize.x + obj->animation.frame_size.x;
	vs.y = screenSize.y + obj->animation.frame_size.y;

	if(obj->visible && Adds::checkOverlap(obj->coords, vc, vs))
	{
		// wyswietl obramowanie dla obiktow typu source
		if(game->selectedAction && game->selectedHumanObject!=NULL && obj->params.life > 0)
		{
			bool fit = false;
			bool hungry = false;
			if(game->selectedHumanObject->params.life < MAX_LIFE)
				hungry = true;
			switch(game->selectedHumanObject->group)
			{
				case Object::WORKER_ROCK: 	if(obj->group == Object::SOURCE_ROCK || (obj->group == Object::MATERIAL_LAKE && hungry))	fit = true;	break;
				case Object::WORKER_WOOD: 	if(obj->group == Object::SOURCE_WOOD || (obj->group == Object::MATERIAL_LAKE && hungry))	fit = true;	break;
				case Object::WORKER_LAKE: 	if(obj->group == Object::SOURCE_LAKE || (obj->group == Object::MATERIAL_LAKE && hungry))	fit = true;	break;
			}
			if(fit)
				this->printBorder(obj, 2, BLUE);
		}

		// jesli obiekt posiada animacje to wyswielt wskazany fragment obrazka
		if(addRes == RES_WORKER_BACKPACK)
			this->printObjAnim(obj, RES_WORKER_BACKPACK);
		else
			this->printObjAnim(obj);

		// wyswietl obramowanie objektu
		if(obj->border)
			this->printBorder(obj, 2, GREEN);
	}

	if(obj->animation.multiline && obj->params.life > 0)
	{
		Adds::vertex vv = obj->coords;
		vv.y -= 14;
		debugPrintInt(vv, obj->params.life);
	}
}

void Graphics::printObjAnim(ObjAnim *obj, int addRes)
{
	if(obj->animation.isAnimation)
	{
		MARect rect;
		MAPoint2d point;
		point.x = obj->coords.x + game->map_moving.x;
		point.y = obj->coords.y + game->map_moving.y;
		rect.left = obj->animation.frame_size.x * obj->animation.frame.x;
		rect.top = obj->animation.frame_size.y * obj->animation.frame.y;
		rect.width = obj->animation.frame_size.x;
		rect.height = obj->animation.frame_size.y;
		maDrawImageRegion(obj->res_file, &rect, &point, TRANS_NONE);

		// dorysuj dodatkowa grafike dla animacji
		if(addRes != 0)
			maDrawImageRegion(addRes, &rect, &point, TRANS_NONE);
	}
	else
		maDrawImage(obj->res_file, obj->coords.x + game->map_moving.x, obj->coords.y + game->map_moving.y);
}

void Graphics::printBorder(Object *obj, const int border_size, int rgb)
{
	maSetColor(rgb);
	maLine((obj->coords.x-border_size) + game->map_moving.x, (obj->coords.y-border_size) + game->map_moving.y, (obj->coords.x+obj->animation.frame_size.x+border_size) + game->map_moving.x, (obj->coords.y-border_size) + game->map_moving.y);
	maLine((obj->coords.x-border_size) + game->map_moving.x, (obj->coords.y+obj->animation.frame_size.y+border_size) + game->map_moving.y, (obj->coords.x+obj->animation.frame_size.x+border_size) + game->map_moving.x, (obj->coords.y+obj->animation.frame_size.y+border_size) + game->map_moving.y);
	maLine((obj->coords.x-border_size) + game->map_moving.x, (obj->coords.y-border_size) + game->map_moving.y, (obj->coords.x-border_size) + game->map_moving.x, obj->coords.y+(obj->animation.frame_size.y+border_size) + game->map_moving.y);
	maLine((obj->coords.x+obj->animation.frame_size.x+border_size) + game->map_moving.x, (obj->coords.y-border_size) + game->map_moving.y, (obj->coords.x+obj->animation.frame_size.x+border_size) + game->map_moving.x+1, (obj->coords.y+obj->animation.frame_size.y+border_size) + game->map_moving.y+1);
	//maFillRect(obj->coords.x-2, obj->coords.y-2, obj->size.x+4, obj->size.y+4);
}

void Graphics::printBackground()
{
	//maWait(20);
	maSetDrawTarget(graph_buffer);

	MARect rect;
	MAPoint2d point;
	point.x = 0;
	point.y = 0;
	rect.left = std::abs(game->map_moving.x);
	rect.top = std::abs(game->map_moving.y);
	rect.width = screenSize.x;
	rect.height = screenSize.y;
	maDrawImageRegion(bg_buffer, &rect, &point, TRANS_NONE);

	//maDrawImage(bg_buffer, map_moving.x, map_moving.y);

	//maDrawImage(RES_IMAGE_MY, 0, 0);
	//maSetDrawTarget(HANDLE_SCREEN);
	//MARect myRect;
	//maSetClipRect(0,0,100,100);
}

void Graphics::printDiscoveryMap()
{
	maSetColor(BLACK);
	int move_eq_x = (int)std::abs(game->map_moving.x) / DISCOVERY_PIECE_X;
	int move_eq_y = (int)std::abs(game->map_moving.y) / DISCOVERY_PIECE_Y;
	int field_per_ssize_x = (int)(screenSize.x / DISCOVERY_PIECE_X) + move_eq_x + 2;
	int field_per_ssize_y = (int)(screenSize.y / DISCOVERY_PIECE_Y) + move_eq_y + 2;

	for(int yy = move_eq_y; yy < field_per_ssize_y; yy++)
		for(int xx = move_eq_x; xx < field_per_ssize_x; xx++)
			if(!game->discoveryMap[yy][xx])
				maFillRect((int)(DISCOVERY_PIECE_X * xx) + game->map_moving.x, (int)(DISCOVERY_PIECE_Y * yy) + game->map_moving.y, DISCOVERY_PIECE_X, DISCOVERY_PIECE_Y);
}

void Graphics::printOnScreen()
{
	if(DEBUG)
	{
		////////////// temp
		if((maGetMilliSecondCount() - fps_time)>1000)
		{
			fps_print = fps;
			fps=0;
			fps_time = maGetMilliSecondCount();
		}
		char buffor[32];
		itoa((int)fps_print, buffor, 10);
		maSetColor(0xFFFFFF);
		maDrawText(2, 2, buffor);
		fps++;
	}

/*
	//maLine(game->selectedHumanObject->coords.x, game->selectedHumanObject->coords.y, game->selectedHumanObject->action.dest.x, game->selectedHumanObject->action.dest.y);
	if(game->selectedHumanObject!=NULL)
	{
		maSetColor(GREEN);
		maFillRect(game->selectedHumanObject->destination.x, game->selectedHumanObject->destination.y, 5, 5);
	}

	// dodatkowe linie pomocnicze
	if(game->selectedHumanObject!=NULL && game->selectedHumanObject->status==Object::MOVING)
	{
		maSetColor(GREEN);
		maFillRect(game->selectedHumanObject->destination.x, game->selectedHumanObject->destination.y, 3, 3);

		maSetColor(0xFF0000);
		switch(game->selectedHumanObject->direction)
		{
			case DIRECTION_DOWN: maLine(game->selectedHumanObject->coords.x, game->selectedHumanObject->coords.y, game->selectedHumanObject->coords.x, game->selectedHumanObject->coords.y+100); break;
			case DIRECTION_UP: maLine(game->selectedHumanObject->coords.x, game->selectedHumanObject->coords.y, game->selectedHumanObject->coords.x, game->selectedHumanObject->coords.y-100); break;
			case DIRECTION_RIGHT: maLine(game->selectedHumanObject->coords.x, game->selectedHumanObject->coords.y, game->selectedHumanObject->coords.x+100, game->selectedHumanObject->coords.y); break;
			case DIRECTION_LEFT: maLine(game->selectedHumanObject->coords.x, game->selectedHumanObject->coords.y, game->selectedHumanObject->coords.x-100, game->selectedHumanObject->coords.y); break;
		}
		//for(int ii=0;ii<game->selectedHumanObject->destination_coords->size();ii++)
		//	maPlot((*game->selectedHumanObject->destination_coords)[ii].x,(*game->selectedHumanObject->destination_coords)[ii].y);

		Adds::vertex vert, vert2;
		vert.x = vert.y = 0;
		maSetColor(0x0000FF);
		bool widac = true;
		vert.x = vert.y = 0;
		for(int ii=0;ii<game->objects->size();ii++)
		{
			if(!Algorithms::ObjectVisible(vert, game->selectedHumanObject->coords, (*game->objects)[ii]->coords, (*game->objects)[ii]->animation.frame_size))
			{
				widac = false;
				break;
			}
		}
		if(widac)
			maLine(vert.x, vert.y, game->selectedHumanObject->coords.x, game->selectedHumanObject->coords.y);

		maSetColor(0x0000FF);
		maFillRect(game->selectedHumanObject->user_destination.x, game->selectedHumanObject->user_destination.y, 5, 5);

		maSetColor(0xFF0000);
		maFillRect(game->selectedHumanObject->colisionWith->coords.x+20, game->selectedHumanObject->colisionWith->coords.y+20, 10, 10);
	}
*/
/////////////////////////

	maDrawImage(graph_buffer, 0, 0);
	maUpdateScreen();
}

void Graphics::moveMap()
{
	int temp_x = std::max(pointerDraggedCoords.x, pointerPressedCoords.x) - std::min(pointerDraggedCoords.x, pointerPressedCoords.x);
	int temp_y = std::max(pointerDraggedCoords.y, pointerPressedCoords.y) - std::min(pointerDraggedCoords.y, pointerPressedCoords.y);
	if(pointerDraggedCoords.x < pointerPressedCoords.x)
		temp_x *= -1;
	if(pointerDraggedCoords.y < pointerPressedCoords.y)
		temp_y *= -1;
	game->map_moving = game->map_moving_temp;
	game->map_moving.x += temp_x;
	game->map_moving.y += temp_y;
	if(game->map_moving.x>0)	game->map_moving.x = 0;
	if(game->map_moving.y>0)	game->map_moving.y = 0;
	if((game->map_moving.x + gameAreaSize.x)<(screenSize.x))	game->map_moving.x = screenSize.x - gameAreaSize.x;
	if((game->map_moving.y + gameAreaSize.y)<(screenSize.y))	game->map_moving.y = screenSize.y - gameAreaSize.y;
}
/*
void Graphics::setResource(ObjAnim *obj, int res)
{
	obj->res_file = res;
	obj->size.x = EXTENT_X(maGetImageSize(res));
	obj->size.y = EXTENT_Y(maGetImageSize(res));
	if(obj->animation.isAnimation)
	{
		obj->animation.frame_size.x = (int)(obj->size.x / obj->animation.frame_xy.x);
		obj->animation.frame_size.y = (int)(obj->size.y / obj->animation.frame_xy.y);
	}
	else
	{
		obj->animation.frame_size.x = (int)obj->size.x;
		obj->animation.frame_size.y = (int)obj->size.y;
	}
}
*/

void Graphics::debugPrintInt(Adds::vertex coords, int var)
{
	char buffor[32];
	strcpy(buffor,"");
	itoa(var, buffor, 10);
	maSetColor(0xCCCCCC);
	maDrawText(coords.x + game->map_moving.x, coords.y + game->map_moving.y, buffor);
}

Graphics::~Graphics()
{
	maDestroyPlaceholder(bg_menu);
	maDestroyPlaceholder(graph_buffer);
	maDestroyPlaceholder(bg_buffer);
}

/*
		MARect rect;
		rect.left = 0;
		rect.top = 0;
		rect.width = 320;
		rect.height = 620;
		MAUI::Scaler::Scaler 	(obj->res_file, &rect, 0.5,0.5,1,MAUI::Scaler::ST_BILINEAR);
*/

