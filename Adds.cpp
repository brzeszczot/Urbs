/*
 * Adds.cpp
 *
 *  Created on: Dec 14, 2012
 *      Author: brzeszczot
 */

#include "Adds.hpp"

namespace Adds
{
	void getDestCornerCord(int corner, Adds::vertex coords, Adds::vertex col_size, Adds::vertex obj_size, Adds::vertex &res)
	{
		switch(corner)
		{
			case 0: res.y = coords.y - obj_size.y - 2; res.x = coords.x - obj_size.x - 2; break;
			case 1: res.y = coords.y - obj_size.y - 2; res.x = coords.x + col_size.x + 2; break;
			case 2: res.y = coords.y + col_size.y + 2; res.x = coords.x + col_size.x + 2; break;
			case 3: res.y = coords.y + col_size.y + 2; res.x = coords.x - obj_size.x - 2; break;
		}
	}

	unsigned int getDirection(Adds::vertex p1, Adds::vertex p2)
	{
		int dif_x = std::max(p2.x, p1.x) - std::min(p2.x, p1.x);
		int dif_y = std::max(p2.y, p1.y) - std::min(p2.y, p1.y);

		if(dif_x <= dif_y && ((p2.x <= p1.x && p2.y <= p1.y) || (p2.x >= p1.x && p2.y <= p1.y)))
			return DIRECTION_UP;
		else if(dif_x <= dif_y && ((p2.x <= p1.x && p2.y >= p1.y) || (p2.x >= p1.x && p2.y >= p1.y)))
			return DIRECTION_DOWN;
		else if(dif_x >= dif_y && ((p2.x <= p1.x && p2.y <= p1.y) || (p2.x <= p1.x && p2.y >= p1.y)))
			return DIRECTION_LEFT;
		else if(dif_x >= dif_y && ((p2.x >= p1.x && p2.y <= p1.y) || (p2.x >= p1.x && p2.y >= p1.y)))
			return DIRECTION_RIGHT;
	}

	bool checkOverlap(Adds::vertex vert, Adds::vertex coords, Adds::vertex size)
	{
		if(vert.x >= coords.x && vert.y >= coords.y && vert.x <= (coords.x + size.x) && vert.y <= (coords.y + size.y))
			return true;
		return false;
	}

	void getCornerCoord(Adds::vertex coords, Adds::vertex size, unsigned int corner, Adds::vertex &vert)
	{
		switch(corner)
		{
			case 0: vert.x = coords.x; vert.y = coords.y; break;
			case 1: vert.x = (coords.x + size.x); vert.y = coords.y; break;
			case 2: vert.x = (coords.x + size.x); vert.y = (coords.y + size.y); break;
			case 3: vert.x = coords.x; vert.y = (coords.y + size.y); break;
			case 4: vert.x = coords.x + (size.x / 2); vert.y = coords.y + (size.y / 2); break;				// wspolrzedne srodka obiektu
		}
	}

	const std::string getStrDate(time_t now)
	{
		tm * localtm = localtime(&now);

		char buffor[11];
		char buffor_int[5];

		itoa((int)localtm->tm_year+1900, buffor_int, 10);
		strcpy(buffor,buffor_int);
		strcat(buffor,"-");
		itoa((int)localtm->tm_mon+1, buffor_int, 10);
		strcat(buffor,buffor_int);
		strcat(buffor,"-");
		itoa((int)localtm->tm_mday, buffor_int, 10);
		strcat(buffor,buffor_int);

		std::string strDate(buffor);
		return strDate;
	}

	const std::string getStrTime(time_t now)
	{
		tm * localtm = localtime(&now);

		char buffor[6];
		char buffor_int[3];

		itoa((int)localtm->tm_hour, buffor_int, 10);
		strcpy(buffor,buffor_int);
		strcat(buffor,":");
		itoa((int)localtm->tm_min, buffor_int, 10);
		strcat(buffor,buffor_int);

		std::string strTime(buffor);
		return strTime;
	}
}

