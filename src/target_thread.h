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
 file:		qrtailab.h
 describtion:
   file for the classes TargetThread
*/



#ifndef TARGET_THREAD_H
#define TARGET_THREAD_H

#include <QtGui> 
#include "qrtailab.h"
#include <scope_data.h>
#include "led_data.h"
#include "meter_data.h"
#include "log_data.h"
#include "alog_data.h"



using namespace qrl_types;

/**
* @brief controls connection to one target
* @bug   scopethread, meterthread, ledthread are in conflict to QRtaiLabCore -> not threadsafe ->segfault
* @todo  prevent multiple access to variables at the same time. make it threadsafe

*/
class TargetThread : public QThread
 {
 Q_OBJECT
 Q_ENUMS( Manager_Type )
 Q_ENUMS( Commands )
 Q_ENUMS( Param_Class )
 public:
    enum Manager_Type {PARAMS_MANAGER,SCOPES_MANAGER,LOGS_MANAGER,ALOGS_MANAGER,LEDS_MANAGER,METERS_MANAGER,SYNCHS_MANAGER};
 enum Commands{	CONNECT_TO_TARGET,	CONNECT_TO_TARGET_WITH_PROFILE,	DISCONNECT_FROM_TARGET,	START_TARGET,	STOP_TARGET,	UPDATE_PARAM,	GET_TARGET_TIME,	BATCH_DOWNLOAD,	GET_PARAMS,	CLOSE} ;
   enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
     TargetThread();
    ~TargetThread();
    void run();
    void setPreferences(Preferences_T);

    //get information about target //should be removed not threadsafe -> segfault?
    unsigned int getIsTargetConnected(){return Is_Target_Connected;}
    int getIsTargetRunning(){return Is_Target_Running;}
    int getScopeNumber(){return Num_Scopes;}
    int getMeterNumber(){return Num_Meters;}
    int getLedNumber(){return Num_Leds;}
 int getALogNumber(){return Num_ALogs;}
    int getLogNumber(){return Num_Logs;}
    int getParameterNumber(){return Num_Tunable_Parameters;}
    int getBlockNumber(){return Num_Tunable_Blocks;}
      long getTargetNode(){return Target_Node;}
    int getEndApp(){return End_App;}
    int getVerbose(){return Verbose;}
    // should  be removed
    QPL_ScopeData** getScopes(){return Scopes;}
    QRL_MeterData** getMeters(){return Meters;}
    QPL_LedData** getLeds(){return Leds;}
    QRL_ALogData** getALogs(){return ALogs;}
    QRL_LogData** getLogs(){return Logs;}
    Target_Parameters_T* getParameters(){return Tunable_Parameters;}
    Target_Blocks_T* getBlocks(){return Tunable_Blocks;}
   // Batch_Parameters_T* getBatchParameters(){return Batch_Parameters;}
    void setHardRealTime(int hrScope,int hrLog,int hrAlog){hardRealTimeScope=hrScope;hardRealTimeLog=hrLog;hardRealTimeALog=hrAlog;}
   //prameter down- and upload

    int addToBatch(int map_offset,int ind,double value);
    double get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx);
    int update_parameter(int idx, int mat_idx, double val);
    int get_map_offset(int blk, int prm);
    int get_parameter_ind(int blk, int prm, int prm_row,int prm_col);
    void resetBatchMode();
   int getBatchCounter(){return batchCounter;}
   QString getParameterName(int blk,int prm);
    QString getBlockName(int blk);
 int getNumberOfParameters(int blk);
   unsigned int getParameterCols(int blk,int prm);
   unsigned int getParameterRows(int blk,int prm);
  double getParameterValue(int blk,int prm, int nr,int nc);

    void setVerbose(int v){Verbose=v;}
    Preferences_T getPreferences(){return Preferences;}


    void startScopeThreads(); //QRL_ScopeWindow** ScopeWindows);
    void stopScopeThreads();
      QString getScopeName(int);

    

   void startMeterThreads();//QRL_MeterWindow** MeterWindows);
    void stopMeterThreads();
    QString getMeterName(int);

    void startLedThreads();//QRL_LedWindow** LedWindows);
    void stopLedThreads();
      QString getLedName(int);

    void startALogThreads();
    void stopALogThreads();

    void startLogThreads();
    void stopLogThreads();

    const char* getTargetName(){return RLG_Target_Name;}
    long getTargetPort(){return Target_Port;}
    RT_TASK * getTask(){return Target_Interface_Task;}

    void setMainTask(RT_TASK *task){RLG_Main_Task=task;}

// friend
   
 public slots:
    void start();
 private:
   void printBlocksInfo();
    void setEndApp(int EndApp){End_App=EndApp;}
    void setLedValue(int,unsigned int);


   void upload_parameters_info(long port, RT_TASK *task);
  unsigned int Num_Tunable_Parameters;
  unsigned int Num_Tunable_Blocks;
  int Num_Scopes;
  int Num_Logs;
  int Num_ALogs;
  int Num_Leds;
  int Num_Meters;
  int Num_Synchs;
  int hardRealTimeScope;
  int hardRealTimeLog;
  int hardRealTimeALog;
  Target_Parameters_T *Tunable_Parameters;
  Target_Blocks_T *Tunable_Blocks;
  QPL_ScopeData **Scopes;
  QRL_LogData **Logs;
  QRL_ALogData **ALogs;
  QPL_LedData **Leds;
  QRL_MeterData **Meters;
  Target_Synchs_T *Synchs;
  Batch_Parameters_T Batch_Parameters[MAX_BATCH_PARAMS];
//   Profile_T *Profile;
  long Target_Port;
  const char *RLG_Target_Name;
  Preferences_T Preferences;
   int Verbose;
   int Is_Target_Connected;
   unsigned int Is_Target_Running;
   int End_App;
   long Target_Node;
   int test;
   int batchCounter;
  //GetScopeDataThread* Get_Scope_Data_Thread;
  //GetMeterDataThread* Get_Meter_Data_Thread;
  //GetLedDataThread* Get_Led_Data_Thread;
 
 pthread_t *Get_Led_Data_Thread;
 pthread_t *Get_Meter_Data_Thread;
 pthread_t *Get_Scope_Data_Thread;
 pthread_t *Get_ALog_Data_Thread;
 pthread_t *Get_Log_Data_Thread;

 RT_TASK *Target_Interface_Task;
 RT_TASK *RLG_Main_Task;

 };









#endif


