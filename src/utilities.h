/***************************************************************************
 *   Copyright (C) 2006 by Massimiliano Torromeo                           *
 *   massimiliano.torromeo@gmail.com                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

#include "gmenu2x.h"
#include "inputmanager.h"

class case_less {
public:
	bool operator()(const std::string &left, const std::string &right) const;
};

std::string trim(const std::string& s);
std::string strreplace(std::string orig, const std::string &search, const std::string &replace);
std::string cmdclean(std::string cmdline);

bool fileExists(const std::string &file);
bool dirExists(const std::string &dir);
bool rmtree(std::string path);

int constrain(int x, int imin, int imax);

int evalIntConf(int val, int def, int imin, int imax);
int evalIntConf(ConfIntHash& hash, const std::string &key, int def, int imin, int imax);

bool split(std::vector<std::string> &vec, const std::string &str,
		const std::string &delim, bool destructive=true);

int intTransition(int from, int to, long int tickStart, long duration=500,
		long tickNow=-1);

void inject_user_event(enum EventCode code = REPAINT_MENU,
			void *data1 = NULL, void *data2 = NULL);

#endif // UTILITIES_H
