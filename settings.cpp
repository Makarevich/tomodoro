/*
 *  Copyright 2012 Yury Makarevich
 *
 *  This file is part of Tomodoro.
 *
 *  Tomodoro is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Tomodoro is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Tomodoro.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "settings.h"

#define SETTING_NAME(name)      \
    const char  Settings::name_ ## name[] = #name

SETTING_NAMES

#undef SETTING_NAME
