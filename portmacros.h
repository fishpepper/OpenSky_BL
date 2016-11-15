/*
    Copyright 2016 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    author: fishpepper <AT> gmail.com
*/

#ifndef PORTMACROS_H_
#define PORTMACROS_H_

// define some helpers
#define PORT2DIR_(X) X ## DIR
#define PORT2DIR(PORTNAME)  PORT2DIR_(PORTNAME)
#define PORT2BIT_(X, N) X ## _ ## N
#define PORT2BIT(PORTNAME, PIN)  PORT2BIT_(PORTNAME, PIN)

#endif  // PORTMACROS_H_
