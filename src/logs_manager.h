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
 file:		scopes_manager.h
 describtion:
   file for the class  QRL_ScopesManager
*/

#ifndef _LOGS_MANAGER_H
#define _LOGS_MANAGER_H 1


#include "ui_qrl_logs_manager.h"
#include "qrtailab_core.h"
#include "log_window.h"
#include "matrix_view.h"
#include "xyplot.h"



/**
 * @brief Managed all Logs windows
 */
class QRL_LogsManager : public QDialog, private Ui::QRL_LogsManager
{
   Q_OBJECT
public:
  QRL_LogsManager(QWidget *parent = 0, int numLogs=0, QRL_LogData **logs=0, int verb=0);
  ~QRL_LogsManager();
      void setFileVersion(qint32 v);
  QRL_LogWindow** getLogWindows(){return LogWindows;}
         void stopRefresh(){timer->stop();}
         void setLogName(int i,QString name);
         void setFileName(int logNumber, const QString& filename);
         void setSaveTime(int logNumber, double savetime, bool autosave);
public slots:
   void showLogOptions( QListWidgetItem * item  );
  void showLogOptions( int );
  void startSaving();
  void stopSaving();
  void changeSaveTime(double);
  void changeFileName(const QString&);
   void refresh();
  void changeRefreshRate(double);
   void setFileDirectory();
  void  showLog(int);
   void holdPlot(int);
   void setMinScale(double);
   void setMaxScale(double);
   void changeDelegate(int d);
   void setPixelSize(int);
   void setShowItemNumber(int);
   void changeLogView(int);
   void changeHistDistance(double);
   void changeHistLength(double);
  void changeDx(const QString&);
  void changeDy(const QString&);
  void changeXOffset(double);
  void changeYOffset(double);
private:
  qint32 fileVersion;
  int Num_Logs;
    QRL_LogData **Logs;
    int verbose;
  unsigned int currentLog;
  QRL_LogWindow** LogWindows;
 // GetScopeDataThread* Get_Scope_Data_Thread;
  //QRtaiLabCore* qTargetInterface;
  QTimer *timer;
     QWidget* MatrixViewOptions;
   QWidget* XYPlotOptions;
       MatrixModel *model;
      PixelDelegate *pixelView;
       BlackWhiteDelegate *blackwhiteView;
       ColorBarDelegate *colorView;
     QVector< QVector< float> > v;
  //TargetThread* targetThread;
  QList<QListWidgetItem *> logItems;
friend QDataStream& operator<<(QDataStream &out, const QRL_LogsManager &d);
friend QDataStream& operator>>(QDataStream &in, QRL_LogsManager(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_LogsManager &d);
	QDataStream& operator>>(QDataStream &in, QRL_LogsManager(&d));


#endif
