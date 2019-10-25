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
#ifndef _SCOPE_WINDOW_H
#define _SCOPE_WINDOW_H 1

#include <QtGui> 

#include <qwt_plot.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include  <qwt_plot_marker.h>
#include <qwt_math.h>
#include "qrtailab.h"
#include <scope_trace.h>
#include <scope_data.h>
#include <scope.h>

// class QRL_ScopeWindow;
// 
// /**
//  * @brief QThread for plotting Scope data
//  */
// 
// class PlottingScopeDataThread : public QThread
//  {
//  Q_OBJECT
//  public:
//     void run();
//     void stopThread();
//     void setValue(int,float);
//     void changeRefreshRate(double rr);
//  public slots:
//     void start(QRL_ScopeWindow* scopewindow);
//     void refresh();
//  private:
//     QRL_ScopeWindow* ScopeWindow;
//     int isRunning;
//     QTimer *timer;
//     Scopes_Data_T *ScopeData;
//    QWaitCondition DataReady;
//    QMutex dataMutex;
//  };

/**
 * @brief Display Scope
 * @todo add legend
 * @todo add curser 
 * @todo add parameter labels 
 * @todo add trigger label
* @bug   continuous rolling: a to smal sec/dev value consumes to much cpu power and blocks the gui
 * @bug overwride: for more then one trace the beginning all traces can get different. -> error in time axis
 */
#ifndef _OLD_LAYOUT_
class QRL_ScopeWindow : public QDialog
#else
class QRL_ScopeWindow : public QMdiSubWindow
#endif
 {
   Q_OBJECT
 //friend class PlottingScopeDataThread;
public:
   enum PlottingMode {roll,overwrite,trigger,hold};
   QRL_ScopeWindow(QWidget *parent = 0,QPL_ScopeData *scope=0,int ind=0);
   ~QRL_ScopeWindow();

    QPL_Scope* scope(){return qwtscope;}
   void changeRefreshRate(double);
   double getRefreshRate(){return RefreshRate;}
   void startRefresh(){   timer->start((int)(1./RefreshRate*1000.)); }
   void stopRefresh(){timer->stop(); }
public slots:
   void refresh();
  //void setValue(int,float);
  void setFileVersion(qint32 v){fileVersion=v;}
protected slots:
  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  qint32 fileVersion;
  QPL_Scope* qwtscope;
  QPL_ScopeData* ScopeData;
    QTimer *timer;
 // QwtPlot *qwtPlot;
  double RefreshRate;
  int index;
   friend QDataStream& operator<<(QDataStream &out, const QRL_ScopeWindow &d);
  friend QDataStream& operator>>(QDataStream &in, QRL_ScopeWindow(&d));
};
        QDataStream& operator<<(QDataStream &out, const QPL_Scope &d);
        QDataStream& operator>>(QDataStream &in, QPL_Scope(&d));

#endif
