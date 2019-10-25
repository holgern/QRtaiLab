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
 file:		main_window.h
 describtion:
   file for the classes QRL_MainWindow
*/

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 1

#include "ui_qrl_main_window.h"
#include <QtGui>
#include "meters_manager.h"
#include "leds_manager.h"
#include "scopes_manager.h"
#include "parameters_manager.h"
#include "logs_manager.h"
#include "qrtailab_core.h"
#include "targets_manager.h"
#include <locale.h>
#include "graphic_scene.h"
#ifdef _use_opengl_
#include <QGLWidget>
#include <QtOpenGL>
#endif

/**
 * @brief Main QThread 
 */

/*
class MainThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
    QWaitCondition threadStarted;
    QMutex mutex;
 //signals:
   // void value(int,float);
 public slots:
    void start();
 private:
    TargetThread* targetThread;
 };

*/




/**
 * @brief Main Window
 * @todo save and load settings
 * @todo change locale to en -> decimal point instead of komma
 */
class QRL_MainWindow : public QMainWindow, private Ui::QRL_MainWindow
{
    Q_OBJECT
 friend class TestQrtaiLab;
public:
    QRL_MainWindow(int);
  //  ~QRL_MainWindow();
//signals:
 //    void sendOrder(int);
    void setVerbose(int v){Verbose=v; qTargetInterface->setVerbose(v);}

  public slots:
     void connectDialog();
      void loadProfile(QString &filename);
      void loadParameters(QString &filename);
      void setScopeFileName(int ScopeNumber,const QString &filename);
      void setScopeSaveTime(int ScopeNumber,double savetime,bool autosave);
      void setLogFileName(int LogNumber,const QString &filename);
      void setLogSaveTime(int ScopeNumber,double savetime,bool autosave);
protected:
     void closeEvent(QCloseEvent *event);

protected slots:
    void about();
    void disconnectDialog();
   // void startTarget();

    void loadProfile();
    void saveProfile();
    void start();
    void stop();
    void connectToTarget(Preferences_T p);
    void disconnectFromTarget();
    void showMetersManager();
    void showScopesManager();
    void showLedsManager();
    void showLogsManager();
    void showTargetsManager();
    void showParametersManager();
    void setStatusBarMessage(const QString &);
 
private:
    void enableActionStart(bool);
    void enableActionStop(bool);
    void enableActionConnect(bool);
    void enableActionLoadProfile(bool);
    void enableActionDisconnect(bool);
    void enableActionShowMeter(bool);
    void enableActionShowScope(bool);
    void enableActionShowLed(bool);
    void enableActionShowLog(bool);
    void enableActionShowParameter(bool);
    void enableActionSaveProfile(bool);
    void enableActionDeleteProfile(bool);
    QRL_MetersManager *MetersManager;
    QRL_LedsManager *LedsManager;
    QRL_LogsManager *LogsManager;
    QRL_ScopesManager *ScopesManager;
    QRL_ParametersManager *ParametersManager;
    QRL_TargetsManager *TargetsManager;
    QLabel* statusMessage;
    QMdiArea *mdiArea;
     QGraphicsView *view;
    // QGraphicsScene *scene;
     QRL_OpenGLScene *scene;
    int Verbose;
    QRtaiLabCore	*qTargetInterface;
//       QRL_Parameters *Parameters;
    QProcess *target;
    //QSettings *settings;
};




#endif
