/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License           *
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
 file:		meter_window.h
 describtion:
   file for the class QRL_MeterWindow
*/

#ifndef _METER_WINDOW_H
#define _METER_WINDOW_H 1
#include <QtGui>
#include <qwt_thermo.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_engine.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
//#include "qmeter.h"
 #include <QMdiSubWindow>
#include "meter_data.h"
#include "thermo_qwt.h"
#include "dial_qwt.h"
#include "lcd_qlabel.h"
/**
 * @brief Display Meter 
 * @todo calculate lineargradient for horizontal layout
 */
#ifdef _OLD_LAYOUT_
class QRL_MeterWindow : public QMdiSubWindow
#else
class QRL_MeterWindow : public QDialog
#endif
{
   Q_OBJECT
   Q_ENUMS( Meter_Type )
public:
   enum Meter_Type {THERMO,	DIAL,	LCD};
   QRL_MeterWindow(QWidget *parent = 0,QRL_MeterData *meter=0);
   ~QRL_MeterWindow();
   double getRefreshRate(){return RefreshRate;}
   void changeRefreshRate(double);
   void refresh();
   void setMin(double);
   double getMin(){return Min;}
   void setMax(double);
    double getMax(){return Max;}
   void setMeter(Meter_Type);
   Meter_Type getMeterType(){return MeterType;}
   QPL_ThermoQwt* thermo(){return Thermo;}
   QPL_DialQwt* dial(){return Dial;}
   QPL_LcdQLabel* lcd(){return Lcd;}

   void setFileVersion(qint32 v){fileVersion=v;}
protected slots:
  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  qint32 fileVersion;
  // void setDistance(const QwtThermo* const);
 // float Value;
   QRL_MeterData* Meter;
  //QwtThermo *Thermo;
  QPL_ThermoQwt *Thermo;
  QPL_DialQwt *Dial;
 // QMeter *Dial;
//  QLCDNumber *Lcd;
  //QLabel *Lcd;
  QPL_LcdQLabel* Lcd;
  double RefreshRate;
  Meter_Type MeterType;
//  QColor thermoColor1,thermoColor2,alarmThermoColor1,alarmThermoColor2;
//  bool gradientEnabled;
 // int pipeWidth,pipeDistance;
  double Min,Max;
//  double alarmLevel;
//  QBrush brush;
//  QLinearGradient gradient,alarmGradient;
//  QwtDialSimpleNeedle *needle;
//  int precision;
//  char format;
  friend QDataStream& operator<<(QDataStream &out, const QRL_MeterWindow &d);
  friend QDataStream& operator>>(QDataStream &in, QRL_MeterWindow(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_MeterWindow &d);
	QDataStream& operator>>(QDataStream &in, QRL_MeterWindow(&d));


#endif
