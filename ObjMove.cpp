/*
 * ObjMove.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: brzeszczot
 */

#include "ObjMove.hpp"

ObjMove::ObjMove()
{
	status = WAITING;
	move_speed = 25; // co ile ms ruch
	colisionWith = NULL;
	eatFood = NULL;
	colisionCorner = goToCorner = -1;
	walkCounter = 0;
	wasMoved = false;

	destination.x = destination.y = user_destination.x = user_destination.y = 0;
	destination_coords = new std::vector<Adds::vertex>;
	destination_current = 0;
	direction = direction_last = -1;

	clickedObject = objOverLap = NULL;
	action.work_mode = STANDBY;
	action.work_amount = 0;
	action.work_time = 0;
	action.busy = false;
	action.obj_work = NULL;
}

ObjMove::~ObjMove()
{
    destination_coords->clear();
    delete destination_coords;
}

void ObjMove::Update(int current_time, std::vector<Object*> *objects)
{
	time = current_time;
	// jesli obiekt zostal przesuniety
	if(changeMovement())
	{
		// zabij obiekt ktory ma zycia ponizej zera
		if(params.life <= 0)
		{
			DoNothing();
			//unusedObject();
			enableObject(false);
		}

		objOverLap = NULL;
		Object *col_obj = checkColision(objects);

		// zmniejsz zycie w zaleznosci ile obiekt przeszedl
		if(walkCounter >= MAX_WALKING_DISTANCE)
		{
			params.life--;
			walkCounter = 0;
		}

		// kolizja byla z obiektem sciana
		if(col_obj != NULL && col_obj->isWall)
		{
			// kolizja z obiektem z ktorym pracuje LUB kliknieto bezposrednio w kolizyjny obiekt
			if(((action.obj_work == colisionWith) || (clickedObject != NULL && clickedObject == col_obj)))// || Algorithms::SectionLength(this->coords, this->destination) < (this->animation.frame_size.x + this->animation.frame_size.y))
			{
				DoNothing();	// zatrzymaj i zresetuj obiekt
				actionWork();	// rozpocznij prace
				col_obj = NULL;
			}
		}

		// kolizja byla ALE nie sciana wiec wyzeruj obiekt kolizji
		if(objOverLap != NULL && !objOverLap->isWall && eatFood != NULL && objOverLap->group == Object::MATERIAL_LAKE)
		{
			// kolizja z obiektem ktory mielismy zjesc
			objOverLap->enableObject(false);
			eatFood = objOverLap = NULL;
			DoNothing();
			params.life = MAX_LIFE;
		}

		// omijanie obiektu
		manageColision(col_obj);
	}
	// aktualizuj akcje
	actionUpdate();
}

void ObjMove::DoNothing()
{
	destination_coords->clear();
	destination_current = 0;
	Stop();
	status = ObjMove::WAITING;
}

void ObjMove::Stop()
{
	status = STOP;
	animation.animate = false;
	animation.frame.x = animation.frame.y = 2;// klatka neutralna - przodem
}

void ObjMove::Go()
{
	status = MOVING;
	animation.animate = true;
	animation.frame.y = direction;
}

void ObjMove::Go(Adds::vertex dest_coord, bool user)
{
	if(user)
	{
		colisionWith = NULL;
		colisionCorner = goToCorner = -1;
		user_destination = dest_coord;		// zapamietanie wsp wyznaczonych przez gracza
	}
	destination = dest_coord;
	status = MOVING;
	animation.animate = true;
	direction = Adds::getDirection(coords, destination);
	animation.frame.y = direction;

	destination_coords->clear();
	destination_coords->push_back(coords);
	Algorithms::SectionCalc(coords, destination, 1, destination_coords);
	destination_current = 0;
	//float ret = alg.SectionLength(selectedHumanObject->coords, selectedHumanObject->destination);
}

bool ObjMove::changeMovement()
{
	if(time - move_timer > move_speed && status==MOVING)
	{
		move_timer = time;
		// jesli to nie koniec drogi - ustaw kolejna wspolrzedna drogi
		if(destination_current < destination_coords->size())
		{
			coords = (*destination_coords)[destination_current];
			destination_current++;
			walkCounter++;

			wasMoved = true;
		}
		else
			DoNothing();

		return true;
	}
	return false;
}

void ObjMove::actionUpdate()
{
	if(action.work_mode != ObjMove::STANDBY)
	{
		// tryb rozny od pracy
		if(action.work_mode != ObjMove::WORK)
			action.work_time = time;
		// tryb pracy - po pracy skieruj pracownika do miejsca startowego
		if(action.work_mode == ObjMove::WORK && (time - action.work_time) > WORK_TIME)
		{
			// zabierz zycie surowcowi
			if(action.obj_work != NULL)
				action.obj_work->params.life -= action.obj_work->params.life_unit;

			visible = true;
			action.busy = true;
			action.work_mode = ObjMove::BACK;
			Go(action.dest, true);
		}
		// z miejsca startowego wyslij ponownie do pracy
		if(action.work_mode == ObjMove::BACK && Algorithms::SectionLength(coords, action.dest) <= 3)
		{
			action.work_mode = ObjMove::GO;
			Go(action.gotowork, true);
		}
	}
}

void ObjMove::actionWork()
{
	// jesli pracownik doszedl do celu pracy
	if(action.work_mode == ObjMove::GO)
	{
		action.work_mode = ObjMove::WORK;
		visible = false;
	}
}

void ObjMove::actionStart(Object *obj, Adds::vertex where)
{
	action.gotowork = where;
	action.dest = coords;
	action.work_mode = ObjMove::GO;
	action.obj_work = obj;
}

void ObjMove::actionStop()
{
	visible = true;
	action.work_mode = ObjMove::STANDBY;
	action.busy = false;
	action.obj_work = NULL;
	DoNothing();
}

Object* ObjMove::checkColision(std::vector<Object*> *objects)
{
	// kolizja moze byc tylko z obiektem ktory jest nie do przejscia i ktory nie rusza sie
	if(this->status==ObjMove::MOVING)
	{
		for(int ii=0;ii<objects->size();ii++)
		{
			Object *obj_colision = (*objects)[ii];
			if(obj_colision->enabled)
			{
				// sprawdzaj kolizje dla obiektow znajdujacych sie dalej niz to co nizej
				if(Algorithms::SectionLength(this->coords, obj_colision->coords) > (this->animation.frame_size.x + this->animation.frame_size.y + obj_colision->animation.frame_size.x + obj_colision->animation.frame_size.y))
					continue;

				Adds::vertex vert;
				for(int jj=0;jj<5;jj++)
				{
					Adds::getCornerCoord(this->coords, this->animation.frame_size, jj, vert);
					if(Adds::checkOverlap(vert, obj_colision->coords, obj_colision->animation.frame_size) && this != obj_colision)
					{
						// zapisz pierwszy obiekt jaki sie nalozyl z badanym obiektem chodzacym
						if(objOverLap == NULL)
							objOverLap = obj_colision;

						if(obj_colision->isWall)
						{
							// przywroc poprzednie polozenie obiektu
							if(this->destination_coords->size()>=1)
							{
								this->destination_current-=2;
								if(this->destination_current<0)
									this->destination_current = 0;
								this->coords = (*this->destination_coords)[this->destination_current];
							}
							this->colisionWith = obj_colision;
							this->colisionCorner = jj;

							return obj_colision;
						}
					}
				}
			}
		}
	}
	return NULL;
}

void ObjMove::manageColision(Object *col_obj)
{
	if(col_obj != NULL)
		firstColisionAvoid(this, col_obj);	// pierwsze ominiecie przeszkody po kolizji
	else if(this->goToCorner >= 0 && this->colisionWith != NULL && (status == ObjMove::STOP || status == ObjMove::WAITING))	// dalej omijaj
	{
		Adds::vertex vert;
		int jj;
		for(jj=0;jj<4;jj++) 	// sprawdz wszsytkie rogi obiektu czy widza lokalizacje uzytkownika
		{
			Adds::getCornerCoord(this->coords, this->animation.frame_size, jj, vert);
			if(!Algorithms::ObjectVisible(vert, this->user_destination, this->colisionWith->coords, this->colisionWith->animation.frame_size))
				break;
		}
		if(jj>=4)	// wszsytkie rogi widac - idz do lokalizacji uzytkownika
			this->Go(this->user_destination, true);
		else
			nextColisionAvoid(this);	// dalej omijaj przeszkode
	}
}

void ObjMove::firstColisionAvoid(ObjMove *obj, Object *col_obj)
{
	int left, right, left_corner, right_corner;
	left = right = 0;
	// sprawdz wierzcholki po ktorej stronie leza prostej miedzy obiektem i wsp docelowymi
	// vert: punkt 0-4 przeszkody, vert2: wsp punktu kolizji, vert3: wsp punktu kolizji ale w pozycji docelowej
	Adds::vertex vert, vert2, vert3, left_temp, right_temp;
	Adds::getCornerCoord(obj->coords, obj->animation.frame_size, obj->colisionCorner, vert2);
	Adds::getCornerCoord(obj->destination, obj->animation.frame_size, obj->colisionCorner, vert3);
	left_temp.x = left_temp.y = right_temp.x = right_temp.y = 9999;
	for(int jj=0;jj<4;jj++)
	{
		Adds::getCornerCoord(col_obj->coords, col_obj->animation.frame_size, jj, vert);
		if(Algorithms::multilane(vert2, vert3, vert))
		{
			if(Algorithms::SectionLength(vert2, vert) < Algorithms::SectionLength(vert2, left_temp))
			{
				left_temp = vert;
				left_corner = jj;
			}
			left++;
		}
		else
		{
			if(Algorithms::SectionLength(vert2, vert) < Algorithms::SectionLength(vert2, right_temp))
			{
				right_temp = vert;
				right_corner = jj;
			}
			right++;
		}
	}

	if(left==1)
		obj->goToCorner = left_corner; //vert = left_temp;
	else if(right==1)
		obj->goToCorner = right_corner; // vert = right_temp;
	else if(left == right && left == 2)
	{
		if(Algorithms::SectionLength(obj->destination, left_temp) < Algorithms::SectionLength(obj->destination, right_temp))
			obj->goToCorner = left_corner;
			//vert = left_temp;
		else
			obj->goToCorner = right_corner;
			//vert = right_temp;
	}

	Adds::getDestCornerCord(obj->goToCorner, col_obj->coords, col_obj->animation.frame_size, obj->animation.frame_size, vert);

	// sprawdz w ktora strone omijaj przeszkode - true - zgodnie z ruchem wskazowek zegara, false - przeciwnie
	obj->avoidDirection = false;

	if(obj->goToCorner>=0 && obj->goToCorner<=3)
	{
		switch(obj->goToCorner)
		{
			case 0: if((obj->direction==DIRECTION_UP) || (obj->direction==DIRECTION_RIGHT)) obj->avoidDirection = true; break;
			case 1: if((obj->direction==DIRECTION_DOWN) || (obj->direction==DIRECTION_RIGHT)) obj->avoidDirection = true; break;
			case 2: if((obj->direction==DIRECTION_DOWN) || (obj->direction==DIRECTION_LEFT)) obj->avoidDirection = true; break;
			case 3: if((obj->direction==DIRECTION_UP) || (obj->direction==DIRECTION_LEFT)) obj->avoidDirection = true; break;
		}
	}

	obj->Go(vert);
}

void ObjMove::nextColisionAvoid(ObjMove *obj)
{
	// omijaj przeszkode
	if(obj->avoidDirection)
		obj->goToCorner++;
	else
		obj->goToCorner--;

	if(obj->goToCorner >= 4)
		obj->goToCorner = 0;
	else if(obj->goToCorner < 0)
		obj->goToCorner = 3;

	Adds::vertex vert;
	Adds::getDestCornerCord(obj->goToCorner, obj->colisionWith->coords, obj->colisionWith->animation.frame_size, obj->animation.frame_size, vert);

	obj->Go(vert);
}
