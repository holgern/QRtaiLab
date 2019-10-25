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
 file:		scopes_window.h
 describtion:
   file for the classes QRL_ScopeWindow
*/
#ifndef _LOG_WINDOW_H
#define _LOG_WINDOW_H 1

#include <QtGui> 

#include "qrtailab.h"
#include "log_data.h"
#include "matrix_view.h"
#include "matrix_plot.h"
#include "xyplot.h"

#ifndef _OLD_LAYOUT_
class QRL_LogWindow : public QDialog
#else
class QRL_LogWindow : public QMdiSubWindow
#endif
{
   Q_OBJECT
 //friend class PlottingScopeDataThread;
public:
    enum matrixDelegate {pixel,blackwhite,colorbar,text};
    enum LogView_Type {MATRIXVIEW,	XYPLOT};
   QRL_LogWindow(QWidget *parent = 0,QRL_LogData *log=0);
   ~QRL_LogWindow();
      LogView_Type getLogType(){return LogViewType;}
      void setLog(LogView_Type);
   void changeRefreshRate(double);
   double getRefreshRate(){return RefreshRate;}
//   void setSaveTime(double t) {saveTime=t;}
//   double getSaveTime(){return saveTime;}
//   void setFileName(QString str) {fileName=str;}
//   QString getFileName() {return fileName;}
    void setVerbose(int v){Verbose=v;}
    void setFileVersion(qint32 v){fileVersion=v;}
    void setPlotting(bool b);
    bool isPlotting(){return plotting;}
//    void setValue(const QVector< QVector<float> > &v);
//     void setMinScale(double min);
//     void setMaxScale(double max);
//     double getMinScale(){return minScale;}
//     double getMaxScale(){return maxScale;}
     void setDelegate(matrixDelegate d);
     matrixDelegate getDelegate(){return actualDelegate;}
//     void setPixelSize(int psize);
//     int  getPixelSize(){return pixelSize;}
//     void  setShowItemNumber(bool n);
//     bool  getShowItemNumber(){return showItemNumber;}
     QPL_MatrixPlot* matrixplot(){return matrixPlot;}
     QPL_XYPlot* xyplot(){return xyPlot;}
public slots:
   void refresh();
  //void setValue(int,float);

protected slots:
  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  qint32 fileVersion;
  int Verbose;
  bool plotting;
  float Value;
  double RefreshRate;
 LogView_Type LogViewType;
//       double minScale;
//     double maxScale;
//      int pixelSize;
//     bool showItemNumber;
  QTimer *timer;
 QRL_LogData *Log;
//  double saveTime;
//  QString fileName;
//     MatrixModel *model;
//      PixelDelegate *pixelView;
//       BlackWhiteDelegate *blackwhiteView;
//       ColorBarDelegate *colorView;
       matrixDelegate actualDelegate;
//      QTableView *matrixPlot;
  QPL_MatrixPlot *matrixPlot;
  QPL_XYPlot *xyPlot;
  friend QDataStream& operator<<(QDataStream &out, const QRL_LogWindow &d);
  friend QDataStream& operator>>(QDataStream &in, QRL_LogWindow(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_LogWindow &d);
	QDataStream& operator>>(QDataStream &in, QRL_LogWindow(&d));


#endif
