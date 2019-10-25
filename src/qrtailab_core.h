/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                   Lorenzo Dozio (dozio@aero.polimi.it)                  *
 *		    Paolo Mantegazza (mantegazza@aero.polimi.it)           *
 *		    Roberto Bucher (roberto.bucher@supsi.ch)               *
 *		    Peter Brier (pbrier@dds.nl)                            *
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
 file:		qrtailab.h
 describtion:
   file for the classes TargetThread
*/



#ifndef QRTAILAB_CORE_H
#define QRTAILAB_CORE_H

#include <QtGui> 
#include "qrtailab.h"
#include "target_thread.h"
#include "parameters.h"
// static RT_TASK *Target_Interface_Task;
// static RT_TASK *RLG_Main_Task;
using namespace qrl_types;

/**
* @brief controls connection to one target
* @bug   scopethread, meterthread, ledthread are in conflict to QRtaiLabCore -> not threadsafe ->segfault
* @todo  prevent multiple access to variables at the same time. make it threadsafe

*/

class QRtaiLabCore :  public QObject {
	
	Q_OBJECT
	Q_ENUMS( Commands )
public:
    QRtaiLabCore(QObject *parent=0,int Verbose = 0);
    ~QRtaiLabCore();
//     TargetThread* getTargetThread(){return targetthread;}
    int stopTarget();
    int startTarget();
    int connectToTarget();
    int disconnectFromTarget();
    void closeTargetThread();

    void setPreferences(Preferences_T p){targetthread->setPreferences(p);}
    Preferences_T getPreferences(){return targetthread->getPreferences();}

    unsigned int getIsTargetConnected(){return targetthread->getIsTargetConnected();}
    int getIsTargetRunning(){return targetthread->getIsTargetRunning();}
    int getEndApp(){return targetthread->getEndApp();}
    int getVerbose(){return targetthread->getVerbose();}
    int setVerbose(int v){ targetthread->setVerbose(v);}
    void setHardRealTime(int hrScope,int hrLog,int hrAlog){targetthread->setHardRealTime( hrScope, hrLog, hrAlog);}
    const char* getTargetName(){return targetthread->getTargetName();}

//     void uploadParameters();
//     void batchParameterDownload();
    QRL_Parameters* getParameters(){return Parameters;}
    int getParameterNumber(){return targetthread->getParameterNumber();}
    int getBlockNumber(){return targetthread->getBlockNumber();}
   QString getParameterName(int blk,int prm);
   QString getBlockName(int blk);
//    void resetBatchMode(){targetthread->resetBatchMode();}
//     int getNumberOfParameters(int blk);
//    unsigned int getParameterCols(int blk,int prm);
//    unsigned int getParameterRows(int blk,int prm);
//     double getParameterValue(int blk,int prm, int nr,int nc);
//     void updateParameterValue(int blk,int prm, int nr,int nc,double value);
//     void addToBatch(int blk,int prm, int nr,int nc, double value);

  QPL_ScopeData** getScopes(){return targetthread->getScopes();}
     int getScopeNumber();
   QString getScopeName(int);
    int	    getScopeDt(int);
    int     getNumberOfTraces(int);

     QRL_MeterData** getMeters(){return targetthread->getMeters();}
        QRL_MeterData* getMeter(int i){return targetthread->getMeters()[i];}
      int getMeterNumber();
    QString getMeterName(int);
    int	    getMeterDt(int);

   QPL_LedData** getLeds(){return targetthread->getLeds();}
    QPL_LedData* getLed(int i){return targetthread->getLeds()[i];}
    int getLedNumber();
    QString getLedName(int);
    int	    getLedDt(int);
    int     getNumberOfLeds(int);

  QRL_LogData** getLogs(){return targetthread->getLogs();}
int getLogNumber();
    QString getLogName(int);

  QRL_ALogData** getALogs(){return targetthread->getALogs();}
 int getALogNumber();
     QString getALogName(int);

public slots:
    //void refreshData();
 signals:
   void statusBarMessage(const QString &);
	
private:
  QRL_Parameters *Parameters;
  TargetThread* targetthread;
  Preferences_T Preferences;
  RT_TASK *Target_Interface_Task;
  RT_TASK *RLG_Main_Task;

};



#endif


