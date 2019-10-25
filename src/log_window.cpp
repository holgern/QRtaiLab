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

#include "log_window.h"
#include <stdlib.h>



//
//  Initialize Log window
//

QRL_LogWindow::QRL_LogWindow(QWidget *parent,QRL_LogData *log)
#ifndef _OLD_LAYOUT_
        :QDialog(parent),Log(log)
#else
         :QMdiSubWindow(parent),Log(log)
#endif
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_LogWindow"));
    this->resize(350, 249);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/log_icon.xpm")));
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint );
   RefreshRate=20.;
LogViewType = QRL_LogWindow::MATRIXVIEW;
    matrixPlot = new QPL_MatrixPlot(this);
    xyPlot = new QPL_XYPlot(this);
     xyPlot->setVisible(false);

     if (Log->getNCol()==2)
        xyPlot->initTraces(Log->getNRow(),100);
 //     model = new MatrixModel(this);
//     matrixPlot = new QTableView;
//     matrixPlot->setShowGrid(false);
//     matrixPlot->horizontalHeader()->hide();
//     matrixPlot->verticalHeader()->hide();
//     matrixPlot->horizontalHeader()->setMinimumSectionSize(1);
//     matrixPlot->verticalHeader()->setMinimumSectionSize(1);
//     matrixPlot->setModel(model);
//     pixelView = new PixelDelegate(this);
//     blackwhiteView = new BlackWhiteDelegate(this);
//     colorView= new ColorBarDelegate(this);
//     matrixPlot->setItemDelegate(colorView);


//       matrixPlot = new QTableWidget(Log->getNRow(),Log->getNCol(),this);

//     for (int i=0;i<Log->getNRow();i++)
//           for (int j=0;j<Log->getNCol();j++){
//             QTableWidgetItem *newItem = new QTableWidgetItem(tr("0"));
//             matrixPlot->setItem(i, j, newItem);
//            }
    //qwtPlot=this;
//    matrixPlot->setObjectName(QString::fromUtf8("qwtPlot"));
#ifdef _OLD_LAYOUT_
    this->setWidget(matrixPlot);
#else
     this->hide();
     this->setLayout(new QVBoxLayout);
        this->layout()->addWidget(matrixPlot);
#endif
    this->setWindowTitle(QApplication::translate("QRL_LogWindow", Log->getName(), 0, QApplication::UnicodeUTF8));
//     minScale=0;
//     maxScale=1;
//       pixelSize=24;
       //showItemNumber=false;
       plotting=false;
actualDelegate=colorbar;

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./RefreshRate*1000.));


}

QRL_LogWindow::~QRL_LogWindow(){
   // delete model;
    delete matrixPlot;
    delete xyPlot;
}


void QRL_LogWindow::refresh()
{

      if (Log->isPlotting()){
          if (getLogType()==QRL_LogWindow::MATRIXVIEW)
            matrixplot()->setValue( Log->getLogValue());
          else if (getLogType()==QRL_LogWindow::XYPLOT)
              xyplot()->setValue( Log->getLogValueHist());
     }

}

 void QRL_LogWindow::setLog(LogView_Type logtype){

LogViewType=logtype;
        switch (LogViewType){
        case MATRIXVIEW:
                xyPlot->setVisible(false);
                matrixPlot->setVisible(true);
                // this->setWidget(Dial);
                 this->layout()->removeWidget(this->layout()->widget());
                 this->layout()->addWidget(matrixPlot);

                // delete Thermo;
                break;
        case XYPLOT:
                if (xyPlot->getNCurve()==0){
                     QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("xy-plot is not possible. you need 2 columns!"),
                                     QMessageBox::Abort);
                    break;
                }

                matrixPlot->setVisible(false);
                 xyPlot->setVisible(true);
                 this->layout()->removeWidget(this->layout()->widget());
                 this->layout()->addWidget(xyPlot);

                break;
        default:
                break;
        }



 }

void QRL_LogWindow::setDelegate(matrixDelegate d){
    actualDelegate=d;
    matrixPlot->setDelegate((QPL_MatrixPlot::matrixDelegate)d);
//     switch(d)
//        {
//        case pixel:
//         matrixPlot->setItemDelegate(pixelView);
//         pixelView->setMinScale(minScale);
//         pixelView->setMaxScale(maxScale);
//          pixelView->setPixelSize(pixelSize);
//           pixelView->setShowItemNumber(showItemNumber);
//         break;
//        case blackwhite:
//          matrixPlot->setItemDelegate(blackwhiteView);
//           blackwhiteView->setMinScale(minScale);
//            blackwhiteView->setPixelSize(pixelSize);
//             blackwhiteView->setShowItemNumber(showItemNumber);
//          break;
//         case colorbar:
//           matrixPlot->setItemDelegate(colorView);
//           colorView->setMinScale(minScale);
//             colorView->setMaxScale(maxScale);
//              colorView->setPixelSize(pixelSize);
//             colorView->setShowItemNumber(showItemNumber);
//          break;
//        default:
//                break;
//    }
}
//
//  void QRL_LogWindow::setMinScale(double min)
// {
//            minScale=min;
//     switch(actualDelegate)
//     {
//        case pixel:
//         pixelView->setMinScale(minScale);
//         break;
//        case blackwhite:
//           blackwhiteView->setMinScale(minScale);
//          break;
//         case colorbar:
//           colorView->setMinScale(minScale);
//        default:
//                break;
//    }
//
// }
//
//   void QRL_LogWindow::setMaxScale(double max)
// {
//       maxScale=max;
//     switch(actualDelegate)
//     {
//        case pixel:
//         pixelView->setMaxScale(maxScale);
//         break;
//        case blackwhite:
//
//          break;
//         case colorbar:
//           colorView->setMaxScale(maxScale);
//        default:
//                break;
//    }
// }
//
//      void QRL_LogWindow::setPixelSize(int psize){
//          pixelSize=psize;
//         switch(actualDelegate)
//     {
//        case pixel:
//         pixelView->setPixelSize(pixelSize);
//         break;
//        case blackwhite:
//           blackwhiteView->setPixelSize(pixelSize);
//          break;
//         case colorbar:
//           colorView->setPixelSize(pixelSize);
//        default:
//                break;
//    }
//
//
//      }
//void  QRL_LogWindow::setShowItemNumber(bool n){
//
//              showItemNumber=n;
//         switch(actualDelegate)
//     {
//        case pixel:
//         pixelView->setShowItemNumber(showItemNumber);
//         break;
//        case blackwhite:
//           blackwhiteView->setShowItemNumber(showItemNumber);
//          break;
//         case colorbar:
//         colorView->setShowItemNumber(showItemNumber);
//        default:
//                break;
//    }
//
//
//}

void QRL_LogWindow::setPlotting(bool b){
    plotting=b;
    if (plotting){
      Log->setPlotting(true);
   } else {
	Log->setPlotting(false);
    }
    
  }
  
   void QRL_LogWindow::changeRefreshRate(double rr)
{
	RefreshRate=rr;
	if (	RefreshRate<0.)
		RefreshRate=20.;
        timer->stop();
        timer->start((int)(1./RefreshRate*1000.));
	
	//Plotting_Scope_Data_Thread->changeRefreshRate(RefreshRate);

}
//void QRL_LogWindow::setValue(const QVector< QVector<float> > &v){
////for (int i=0;i<v.size();i++){
////        for (int j=0;j<v.at(i).size();j++){
//////             printf(" %f ",v.at(i).at(j));
////             (matrixPlot->item(i,j))->setText(tr("%1").arg(v.at(i).at(j)));
////         }
//////         printf("\n");
////}
////  printf("\n");
//
//    model->setData(v);
//    matrixPlot->resizeColumnsToContents();
//    matrixPlot->resizeRowsToContents();
//}




QDataStream& operator<<(QDataStream &out, const QRL_LogWindow &d){
        qint32 a;
        out  << d.size()  << d.pos() << d.isVisible();
        out <<d.plotting;
        out << d.RefreshRate;
//        out << d.minScale;
//        out << d.maxScale;
//        a=d.pixelSize; out << a;
//        out << d.showItemNumber;
        out << *(d.matrixPlot);
        out << *(d.xyPlot);
        out << (qint32)d.Log->getHistDistance();
        out << (qint32)d.LogViewType;
        out << d.Log->data2disk()->getSaveTime();//out << d.saveTime;
        out << d.Log->data2disk()->getFileName();// out << d.fileName;
        a= (qint32)d.actualDelegate; out << a;

	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_LogWindow(&d)){
        QSize s;QPoint p;bool b; QColor c; qint32 a;QFont f; double dd;
        QString str;
        in >> s;d.resize(s);
        in >> p; d.move(p);
        in >> b; d.setVisible(b);
        in >> b; d.setPlotting(b);
        in >> dd; d.changeRefreshRate(dd);
//        in >> dd; d.setMinScale(dd);
//        in >> dd; d.setMaxScale(dd);
//        in >> a; d.setPixelSize(a);
//        in >> b; d.setShowItemNumber(b);
        in >> *(d.matrixPlot);
        if (d.fileVersion>109){
            in >> *(d.xyPlot);
            in >> a;d.Log->setHistDistance(a);
            in >> a; d.setLog((QRL_LogWindow::LogView_Type) a);
        }
        in >> dd; d.Log->data2disk()->setSaveTime(dd);//d.setSaveTime(dd);
        in >> str;d.Log->data2disk()->setFileName(str);// d.setFileName(str);
        in >> a;   d.setDelegate((QRL_LogWindow::matrixDelegate)a);
		  
	return in;
}
