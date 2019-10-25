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
 file:		scopes_window.cpp
 describtion:
   file for the classes QRL_ScopeWindow
*/

#include "scope_window.h"
#include <stdlib.h>




//
//  Initialize Scope window
//

QRL_ScopeWindow::QRL_ScopeWindow(QWidget *parent,QPL_ScopeData *scope,int ind)
#ifndef _OLD_LAYOUT_
        :QDialog(parent),ScopeData(scope),index(ind)
#else
         :QMdiSubWindow(parent),ScopeData(scope),index(ind)
#endif
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_ScopeWindow"));
    this->resize(350, 249);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/scope_icon.xpm")));
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint );
     //qwtscope = new QPL_Scope(this,scope,index);
        qwtscope = new QPL_Scope(this);
        qwtscope->initTraces(scope,index);
    //qwtPlot = new QwtPlot(this);
    //qwtPlot=this;
    qwtscope->setObjectName(QString::fromUtf8("qwtPlot"));
    this->hide();
#ifdef _OLD_LAYOUT_
    this->setWidget(qwtscope);
#else
       this->setLayout(new QVBoxLayout);
    this->layout()->addWidget(qwtscope);
#endif
    this->setWindowTitle(QApplication::translate("QRL_ScopeWindow", scope->getName().toLocal8Bit().data(), 0, QApplication::UnicodeUTF8));

    // Disable polygon clipping
   // QwtPainter::setDeviceClipping(false);
    // We don't need the cache here
     //qwtscope->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
     //qwtscope->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    RefreshRate=20;
            timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./RefreshRate*1000.));

}
QRL_ScopeWindow::~QRL_ScopeWindow(){

    delete qwtscope;

}

void QRL_ScopeWindow::changeRefreshRate(double rr)
{
        RefreshRate=rr;
        if (	RefreshRate<0.)
                RefreshRate=20.;
        timer->stop();
        timer->start((int)(1./RefreshRate*1000.));

        //Plotting_Scope_Data_Thread->changeRefreshRate(RefreshRate);

}

void QRL_ScopeWindow::refresh()
{

     if (ScopeData->dataAvailable()) {
        if (ScopeData->getNTraces()>0) {
          if (ScopeData->data2disk()->isSaveScopeTime())
             scope()->setTime( ScopeData->getScopeTime());
           scope()->setValue( ScopeData->getScopeValue());
         }
        //}
        }

}



QDataStream& operator<<(QDataStream &out, const QRL_ScopeWindow &d){

out  << d.size()  << d.pos() << d.isVisible();
out << *(d.qwtscope);
}

QDataStream& operator>>(QDataStream &in, QRL_ScopeWindow(&d)){
     QSize s;QPoint p;bool b;
    in >> s;d.resize(s);
    in >> p; d.move(p);
    in >> b; d.setVisible(b);
    d.qwtscope->setFileVersion(d.fileVersion);
    in >> *(d.qwtscope);
}
