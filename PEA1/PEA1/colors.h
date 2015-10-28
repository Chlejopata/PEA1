/*
 * colors.h
 *
 *  Created on: 27 kwi 2015
 *      Author: bartosz
 */

#ifndef HEADERS_COLORS_H_
#define HEADERS_COLORS_H_



const char defFormat[] =
{ "\033[0m" };

const char boldFormat[] =
{ "\033[1m" };

// ====== FOREGROUNDS ======

const char black[] =
{ "\033[0;30m" };

const char red[] =
{ "\033[0;31m" };

const char green[] =
{ "\033[0;32m" };

const char brown[] =
{ "\033[0;33m" };

const char blue[] =
{ "\033[0;34m" };

const char magenta[] =
{ "\033[0;35m" };

const char cyan[] =
{ "\033[0;36m" };

const char lightGray[] =
{ "\033[0;37m" };

// ====== BACKGROUNDS ======

const char blackBgr[] =
{ "\033[7;30m" };

const char redBgr[] =
{ "\033[7;31m" };

const char greenBgr[] =
{ "\033[7;32m" };

const char brownBgr[] =
{ "\033[7;33m" };

const char blueBgr[] =
{ "\033[7;34m" };

const char magentaBgr[] =
{ "\033[7;35m" };

const char cyanBgr[] =
{ "\033[7;36m" };

const char lightGrayBgr[] =
{ "\033[7;37m" };

const char clearDisp[] =
{ "\033[2J" };

#endif /* HEADERS_COLORS_H_ */
