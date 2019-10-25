/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                         P. Sereno                                       *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License                  *
 *   as published by  the Free Software Foundation; either version 2       *
 *   of the License, or  (at your option) any later version.               *
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

/*
 file:		led_window.h
 describtion:
   file for the classes QRL_LedWindow, QLed;
*/
 
#ifndef _LED_WINDOW_H
#define _LED_WINDOW_H 1

#include <QtGui>
#include "led_data.h"
#include "led.h"

/**
 * @brief LedWindow
 */
#ifndef _OLD_LAYOUT_
class QRL_LedWindow : public QDialog
#else
class QRL_LedWindow : public QMdiSubWindow
#endif
{
   Q_OBJECT
public:
   QRL_LedWindow(QWidget *parent = 0,QPL_LedData* led=0);
   ~QRL_LedWindow();
   void refresh();
   void setLedColor(QColor);
   QColor getLedColor(){return Leds[0]->color();}
protected slots:
  void closeEvent ( QCloseEvent * event );
private:
//   float Value;
    QPL_LedData* Led;
  QPL_Led **Leds;
  QFrame * frame;
  int num_leds;
  QLabel **ledLabels;


friend QDataStream& operator<<(QDataStream &out, const QRL_LedWindow &d);
friend QDataStream& operator>>(QDataStream &in, QRL_LedWindow(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_LedWindow &d);
	QDataStream& operator>>(QDataStream &in, QRL_LedWindow(&d));


#endif
