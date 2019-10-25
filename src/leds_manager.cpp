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
 file:		leds_manager.cpp
 describtion:
   file for the classes GetScopeDataThread and  QRL_ScopesManager
*/

#include "leds_manager.h"



/**
* @brief Initialize Meters Manager
*/
QRL_LedsManager::QRL_LedsManager(QWidget *parent,int numLeds, QPL_LedData **leds, int verb)
        :QDialog(parent),Num_Leds(numLeds),Leds(leds),verbose(verb)
{
	setupUi(this);
        //Num_Leds=qTargetInterface->getLedNumber();
	//Leds=targetThread->getLeds();
	const QIcon LedIcon =QIcon(QString::fromUtf8(":/icons/led_icon.xpm"));
	LedWindows = new QRL_LedWindow* [Num_Leds]; 
	for (int i=0; i<Num_Leds; ++i){
                //ledItems << new QListWidgetItem(LedIcon,qTargetInterface->getLedName(i), ledListWidget);
            ledItems << new QListWidgetItem(LedIcon,tr("%1").arg(i), ledListWidget);
                //LedWindows[i]=new QRL_LedWindow(parent,qTargetInterface->getLed(i));
                LedWindows[i]=new QRL_LedWindow(parent,Leds[i]);
	}
	connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showLed(int) ) );
	connect( ledColorComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeLedColor(int) ) );
	connect( ledListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showLedOptions( QListWidgetItem *  ) ) );
	connect( ledListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showLedOptions( QListWidgetItem *  ) ) );

	if (Num_Leds > 0) showLedOptions(ledListWidget->item(0));

  	RefreshRate=20.;
	timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./RefreshRate*1000.));

}

QRL_LedsManager::~QRL_LedsManager()
{
	for (int i=0; i<Num_Leds; ++i){
		LedWindows[i]->hide();
		delete LedWindows[i];
	}
	delete[] LedWindows;
}


void QRL_LedsManager::refresh()
{
for (int i=0; i<Num_Leds; ++i){
	//if (LedWindows[i]->isVisible()){
		LedWindows[i]->refresh();
	//}
}
  if (!LedWindows[currentLed]->isVisible() && showCheckBox->isChecked())
      showCheckBox->setCheckState(Qt::Unchecked);
}

void  QRL_LedsManager::setLedName(int i,QString name){

    if (i<ledItems.size())
    ledItems[i]->setText(name);
    showLedOptions(ledItems[currentLed]);

}


void QRL_LedsManager::refreshView()
{


	showLedOptions(ledListWidget->item(currentLed));


}



void QRL_LedsManager::changeLedColor(int color)
{
	switch(color){
	case 0:
		LedWindows[currentLed]->setLedColor(QColor(Qt::red));
		break;
	case 1:
		LedWindows[currentLed]->setLedColor(QColor(Qt::green));
		break;
	case 2:
		LedWindows[currentLed]->setLedColor(QColor(Qt::blue));
		break;
	case 3:
		LedWindows[currentLed]->setLedColor(QColor(Qt::yellow));
		break;
	default:
		LedWindows[currentLed]->setLedColor(QColor(Qt::red));
		break;
	}
}



/**
* @brief update manager dialog for the choosen led
* @param item led number
*/
void QRL_LedsManager::showLedOptions( QListWidgetItem * item ){

	currentLed= ledListWidget->row(item);
        //tabWidget->setTabText(0,qTargetInterface->getLedName(currentLed));
        tabWidget->setTabText(0,ledItems.at(currentLed)->text());

	if(LedWindows[currentLed]->isVisible())
		showCheckBox->setCheckState(Qt::Checked);
	else
		showCheckBox->setCheckState(Qt::Unchecked);
}


/**
* @brief show scope windows
* @param state set display status
*/
void QRL_LedsManager::showLed(int state) 
{
	if(state==Qt::Checked){
		LedWindows[currentLed]->show();
	} else {
		LedWindows[currentLed]->hide();
	}

}


QDataStream& operator<<(QDataStream &out, const QRL_LedsManager &d){
	out << d.size()  << d.pos() << d.isVisible();
	out <<(qint32) d.Num_Leds;
	for (int i = 0; i < d.Num_Leds; ++i) {
		out<<*(d.LedWindows[i]);
	}
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_LedsManager(&d)){
	QSize s;QPoint p;bool b;
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	qint32 a;
	in >> a;
	for (int i = 0; i < (int)a; ++i) {
		if (d.Num_Leds>i)
			in>>*(d.LedWindows[i]);
		else 
			in>>*(d.LedWindows[d.Num_Leds-1]);
	}
	return in;
}
