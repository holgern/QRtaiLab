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
 file:		led_window.cpp
 describtion:
   file for the classes QRL_LedWindow QLed
*/

#include "led_window.h"



QRL_LedWindow::QRL_LedWindow(QWidget *parent,QPL_LedData *led)
#ifndef _OLD_LAYOUT_
        :QDialog(parent),Led(led)
#else
        :QMdiSubWindow(parent),Led(led)
#endif
{
	num_leds = Led->getNLeds();
	if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_LedWindow"));
    //this->resize(122, 119);
    this->resize(80, 60);
//	frame=new QFrame(this);
//	frame->resize(122, 119);
   // layout=new QGridLayout(this);
   // this->setLayout(layout);
    Leds = new QPL_Led* [num_leds];
   //ledLabels = new QLabel* [num_leds];
#ifndef _OLD_LAYOUT_
      this->setLayout(new QVBoxLayout);
#endif
    for(int i=0;i<num_leds;++i){
        Leds[i] = new QPL_Led(this);
	//Leds[i]->setObjectName(QString::fromUtf8("Led"));
	Leds[i]->setGeometry(QRect(10, 20+i*35, 30, 30));
	Leds[i]->setValue(false);
        Leds[i]->setText(tr("%1").arg(i+1));

//	ledLabels[i] = new QLabel(frame);
//	ledLabels[i]->setGeometry(QRect(10+40, 20+i*35, 30, 30));
//	ledLabels[i]->setText(tr("%1").arg(i+1));
//        QFrame *ledFrame = new QFrame(this);
//        ledFrame->setLayout(new QHBoxLayout);
//        ledFrame->layout()->addWidget(Leds[i]);
//        ledFrame->layout()->addWidget(ledLabels[i]);
	//layout->addWidget(Leds[i],i,1);
#ifndef _OLD_LAYOUT_
         this->layout()->addWidget(Leds[i]);
#else
          this->layout()->addWidget(Leds[i]);
#endif

    }
        //this->setWidget(frame);
 this->hide();

    this->setMinimumSize(60,20+num_leds*35+10);
    
    this->setWindowTitle(Led->getName());
   // Led->setText(QApplication::translate("QRL_LedWindow", "Led1", 0, QApplication::UnicodeUTF8));
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/led_icon.xpm")));
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint );
}

QRL_LedWindow::~QRL_LedWindow(){
//Plotting_Scope_Data_Thread->stopThread();
//Plotting_Scope_Data_Thread->wait();
//delete Plotting_Scope_Data_Thread;
//delete mY;
for (unsigned int j=0;j<num_leds;j++){
	delete Leds[j];
}
delete[] Leds;
//delete frame;
}

void QRL_LedWindow::setLedColor(QColor c)
{
	 for (int i = 0; i < num_leds; i++) {
		Leds[i]->setColor(c);
	}

}

void QRL_LedWindow::refresh()
{
      unsigned int v = Led->getLedValue();
        for (int i = 0; i < num_leds; i++) {
		if (v & (1 << i)) {
			Leds[i]->setValue(true);
		} else {
			Leds[i]->setValue(false);
		}
	}
}

  void QRL_LedWindow::closeEvent ( QCloseEvent * event )
{
	event->ignore(); 
	this->hide(); 
}



QDataStream& operator<<(QDataStream &out, const QRL_LedWindow &d){
	out << d.Leds[0]->color() << d.size()  << d.pos() << d.isVisible();
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_LedWindow(&d)){
	QSize s;QPoint p;bool b; QColor c;
	in >> c; d.setLedColor(c);
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	return in;
}
