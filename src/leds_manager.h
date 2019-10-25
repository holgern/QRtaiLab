/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                                                                         *
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
 file:		leds_manager.h
 describtion:
   file for the classes GetLedDataThread and  QRL_LedManager*/

#ifndef _LEDS_MANAGER_H
#define _LEDS_MANAGER_H 1


#include "ui_qrl_leds_manager.h"
#include "qrtailab_core.h"
#include "led_window.h"





/**
 * @brief Managed all Led windows
 */

class QRL_LedsManager : public QDialog, private Ui::QRL_LedsManager
{
   Q_OBJECT
public:
   QRL_LedsManager(QWidget *parent = 0, int numLeds=0, QPL_LedData **leds=0, int verb=0 );
   ~QRL_LedsManager();
    QRL_LedWindow** getLedWindows(){return LedWindows;}
 void refreshView();
        void stopRefresh(){timer->stop();}
         void setLedName(int i,QString name);
public slots:
  void  showLed(int);
  void showLedOptions( QListWidgetItem * item );
  void changeLedColor(int);
   void refresh();
private:
  int Num_Leds;
  unsigned int currentLed;
  QRtaiLabCore* qTargetInterface;
  QRL_LedWindow** LedWindows;
   QPL_LedData **Leds;
    QList<QListWidgetItem *> ledItems;
  QTimer *timer;
  int verbose;
    double RefreshRate;

friend QDataStream& operator<<(QDataStream &out, const QRL_LedsManager &d);
friend QDataStream& operator>>(QDataStream &in, QRL_LedsManager(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_LedsManager &d);
	QDataStream& operator>>(QDataStream &in, QRL_LedsManager(&d));

#endif
