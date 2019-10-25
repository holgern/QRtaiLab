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
 file:		meter_window.cpp
 describtion:
   file for the class QRL_MeterWindow
*/

#include "meter_window.h"
#include <stdlib.h>


QRL_MeterWindow::QRL_MeterWindow(QWidget *parent,QRL_MeterData *meter)
#ifndef _OLD_LAYOUT_
        :QDialog(parent),Meter(meter)
#else
         :QMdiSubWindow(parent),Meter(meter)
#endif
{
if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_MeterWindow"));
    this->move(20,70);
    this->resize(80,261);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //sizePolicy.setHorizontalStretch(0);
    //sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
  //  this->setSizePolicy(sizePolicy);
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint  );
    //this->setWindowFlags(windowFlags() ^ Qt::WindowMinMaxButtonsHint  );

  //  this->setOption(RubberBandMove,true);
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/meter_icon.xpm")));
RefreshRate=20;
//dockingwindow
//this->setAllowedAreas(Qt::NoDockWidgetArea);
//this->setFloating(true);
        Min=-1;
        Max=1;
	MeterType=THERMO;

    Thermo = new QPL_ThermoQwt(this);
    Thermo->setObjectName(QString::fromUtf8("Thermo"));
    Thermo->setGeometry(QRect(50, 20, 52, 261));
    Thermo->setRange(Min,Max);
  //  Thermo->setScale(Min,Max);
    Thermo->setPipeWidth(14);
     Thermo->setAutoScale();


        Dial = new QPL_DialQwt(this);
        //Dial->setObjectName(QString::fromUtf8("Dial"));
   	Dial->setGeometry(QRect(50, 20, 52, 50));
        Dial->setRange(Min,Max);

/*	Dial = new QMeter(this);
	Dial->setStartAngle(230);
	Dial->setEndAngle(-40);
	Dial->setMinValue(Min);
	Dial->setMaxValue(Max);
	Dial->setPrecision(1);
	Dial->hide();*/

	/*Lcd = new QLCDNumber(this);
	Lcd->setSmallDecimalPoint(true);
	Lcd->setNumDigits(5);
	Lcd->setSegmentStyle(QLCDNumber::Flat);
	Lcd->hide();*/
//	precision=4;
//	format='f';
//	Lcd = new QLabel(this);
//	 QFont font("Helvetica", 15, QFont::DemiBold);
// 	Lcd->setFont(font);
//	Lcd->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
//	Lcd->hide();
        Lcd = new QPL_LcdQLabel(this);
#ifdef _OLD_LAYOUT_
        this->setWidget(Thermo);
#else
         this->hide();
        this->setLayout(new QVBoxLayout);
        this->layout()->addWidget(Thermo);
#endif
    this->setWindowTitle(Meter->getName());

}

QRL_MeterWindow::~QRL_MeterWindow()
{
// 	if (Typ=="Dial")
// 		delete Thermo;
// 	else
// 		delete Dial;

}
/**
* @brief changing the meter between thermo and dial
* @param metertype meter typ
*/

void QRL_MeterWindow::setMeter(Meter_Type metertype)
{
	MeterType=metertype;
	switch (MeterType){
	case DIAL:
                Thermo->setVisible(false);
                Lcd->setVisible(false);
                 if (!Dial->isVisible()) Dial->setVisible(true);
#ifdef _OLD_LAYOUT_
                 this->setWidget(Dial);
#else
                 this->layout()->removeWidget(this->layout()->widget());
                 this->layout()->addWidget(Dial);
#endif
		// delete Thermo;
		break;
	case THERMO:
    		//Thermo = new QwtThermo(this);
    		//Thermo->setObjectName(QString::fromUtf8("Thermo"));
    		//Thermo->setGeometry(QRect(50, 20, 52, 261));
		//Thermo->setScale(Min,Max);
		//Thermo->setFillColor(thermoColor);

                Thermo->setVisible(true);
                Lcd->setVisible(false);
                 Dial->setVisible(false);
 #ifdef _OLD_LAYOUT_
                this->setWidget(Thermo);
#else
                 this->layout()->removeWidget(this->layout()->widget());
                 this->layout()->addWidget(Thermo);
#endif
		//pipeDistance=Thermo->pos().x();
                //pipeDistance=Thermo->minimumSizeHint().width()-Thermo->pipeWidth()-Thermo->borderWidth()*2;
		//delete Dial;
		break;
	case LCD:
                Thermo->setVisible(false);
                Lcd->setVisible(true);
                 Dial->setVisible(false);
#ifdef _OLD_LAYOUT_
                this->setWidget(Lcd);
#else
                this->layout()->removeWidget(this->layout()->widget());
                 this->layout()->addWidget(Lcd);
#endif
		break;
	default:
		break;
	}
}

/**
* @brief setting the actual value of the meter
* @param v meter value
*/
void QRL_MeterWindow::refresh()
{
	double v=Meter->getMeterValue();
        //QString str;
        //QLocale loc;
	switch (MeterType){
	case DIAL:
		Dial->setValue(v);
		break;
	case THERMO:
		Thermo->setValue(v);
		break;
	case LCD:
		//str.setNum(v,'g',4);
                Lcd->setValue(v);
		break;
	default:
		break;
	}
}

void QRL_MeterWindow::changeRefreshRate(double rr)
{
	RefreshRate=rr;
}


/**
* @brief setting the maximal range
* @param max maximal range value
*/
void QRL_MeterWindow::setMax(double max)
{
        Max=max;
        Thermo->setMaxValue(max);
        Dial->setMax(max);
        //pipeDistance=Thermo->minimumSizeHint().width()-Thermo->pipeWidth()-Thermo->borderWidth()*2;
        //setPipeWith(pipeWidth);
        //Dial->setRange(Min,Max);
}
/*
void QRL_MeterWindow::setDistance(const QwtThermo* const t){

	int Start,End;
	QFont font;
	//t->scaleDraw()->getBorderDistHint(font,Start,End);;
	printf("BorderWith %d\n",t->borderWidth());
	printf("Pipewidth %d\n",t->pipeWidth());
	printf("Sizehint w %d h %d\n",t->sizeHint().width(),t->sizeHint().height());
}
*/

/**
* @brief setting the minimal range
* @param min minimal range value
*/
void QRL_MeterWindow::setMin(double min)
{

        Min=min;
        Thermo->setMinValue(min);
        //pipeDistance=Thermo->minimumSizeHint().width()-Thermo->pipeWidth()-Thermo->borderWidth()*2;
        //setPipeWith(pipeWidth);
        Dial->setMin(min);
        //Dial->setRange(Min,Max);
}







QDataStream& operator<<(QDataStream &out, const QRL_MeterWindow &d){

        out << d.RefreshRate;
//        out << d.gradientEnabled;
//        out << d.thermoColor1;
//        out << d.thermoColor2;
//        out << d.alarmThermoColor1;
//        out << d.alarmThermoColor2;
//        out << (qint32)d.pipeWidth;
//        out << d.Min << d.Max;
//        out << d.alarmLevel << d.Thermo->alarmEnabled();
        out << *(d.Thermo);
        //out << (QFont)d.Lcd->font() ;

        //out << (d.needle->palette().button().color());
        out << *(d.Dial);
        out << (qint32)d.MeterType;
        out  << d.size()  << d.pos() << d.isVisible();
        //out << (qint32)d.precision << (QChar)d.format;
        out << *(d.Lcd);
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_MeterWindow(&d)){
	QSize s;QPoint p;bool b; QColor c; qint32 a;QFont f; double dd;
	QChar ch;

        in >> dd; d.changeRefreshRate(dd);
// 	in >> b; d.setGradientEnabled(b);
//	in >> c; d.setThermoColor1(c);
//	in >> c; d.setThermoColor2(c);
//	in >> c; d.setAlarmThermoColor1(c);
//	in >> c; d.setAlarmThermoColor2(c);
// 	in >> a; d.setPipeWith(a);
//	in >> dd; d.setMin(dd);
//	in >> dd;  d.setMax(dd);
// 	in >> dd;d.setAlarmLevel(dd);
// 	in >> b; d.setThermoAlarm(b);
// 	//in >> f; d.setLcdFont(f);
        in >> *(d.Thermo);
// 	in >> c; d.setNeedleColor(c);
        in >> *(d.Dial);
        in >> a; d.setMeter((QRL_MeterWindow::Meter_Type)a);
        in >> s;d.resize(s);
        in >> p; d.move(p);
        in >> b; d.setVisible(b);
        if (d.fileVersion > 103){
//	  in >> a; d.setLcdPrecision(a);
//	  in >> ch; d.setLcdFormat(ch.toAscii());
          in >> *(d.Lcd);
        }
	return in;
}
