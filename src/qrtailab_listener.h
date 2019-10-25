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

#include "target_thread.h"
/**
 * @brief QThread for reading Scope data
 */

/*
class GetScopeDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
    int setDt(double);
QWaitCondition threadStarted;
QMutex mutex;
    QMutex data;
 //signals:
   // void value(int,float);
 public slots:
    void start(void* arg,TargetThread* targetthread);//,QRL_ScopeWindow* scopewindow);
 private:
    int index,x,y,w,h;
    char* mbx_id;
    TargetThread* targetThread;
  //  QRL_ScopeWindow* ScopeWindow;
    int MsgData, MsgLen, MaxMsgLen, TracesBytes,MsgWait;
    long int Ndistance;
    double dt;
 };
*/






static void *rt_get_scope_data(void *arg)
{
	RT_TASK *GetScopeDataTask;
	MBX *GetScopeDataMbx;
	char GetScopeDataMbxName[7];
	long GetScopeDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, TracesBytes,MsgWait;
        #ifndef _RTAI_3_7_
        double MsgBuf[MAX_MSG_LEN/sizeof(double)];
        #else
        float MsgBuf[MAX_MSG_LEN/sizeof(float)];
        #endif
	int n, nn, js, jl;
	int index = ((Args_T *)arg)->index;
	char *mbx_id = strdup(((Args_T *)arg)->mbx_id);
	int save_idx = 0;
	TargetThread* targetThread=(TargetThread*)((Args_T *)arg)->targetThread;
	int hardRealTime = ((Args_T *)arg)->hardRealTime;
        QPL_ScopeData* scope = targetThread->getScopes()[index];
	double dt=scope->getScopeDt();
	 long Target_Node = targetThread->getTargetNode();
	RT_TASK *Target_Interface_Task = targetThread->getTask();
	
	rt_allow_nonroot_hrt();
	//mlockall(MCL_CURRENT | MCL_FUTURE);
	if (!(GetScopeDataTask = rt_task_init_schmod(qrl::get_an_id("HGS"), 90, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetScopeData Task\n");
		return (void *)1;
	}
	if(Target_Node == 0) GetScopeDataPort = 0;
	else GetScopeDataPort = rt_request_port(Target_Node);
	sprintf(GetScopeDataMbxName, "%s%d", mbx_id, index);
	if (!(GetScopeDataMbx = (MBX *)RT_get_adr(Target_Node, GetScopeDataPort, GetScopeDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetScopeDataMbxName);
		return (void *)1;
	}
	//munlockall();
        #ifndef _RTAI_3_7_
        TracesBytes = (scope->getNTraces() + 1)*sizeof(double);
        #else
        TracesBytes = (scope->getNTraces() + 1)*sizeof(float);
        #endif
	MaxMsgLen = (MAX_MSG_LEN/TracesBytes)*TracesBytes;
	MsgLen = (((int)(TracesBytes*dt*(1./scope->getDt())))/TracesBytes)*TracesBytes;
	//MsgLen = (((int)(TracesBytes*(1./targetThread->getScopeRefreshRate(index))*(1./scope.dt)))/TracesBytes)*TracesBytes;

	if (MsgLen < TracesBytes) MsgLen = TracesBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/TracesBytes;

	MsgWait = (int)(((int)((MAX_MSG_SIZE-MsgLen)/TracesBytes))*scope->getDt()*1000);
	MsgWait=(MsgWait>WAIT_MIN)?MsgWait:WAIT_MIN;
	MsgWait=(MsgWait<WAIT_MAX)?MsgWait:WAIT_MAX;

	// Ndistance defines the distance between plotted datapoints, to archive the given refresh rate.
	int Ndistance=(int)(dt*(1./scope->getDt()));  //doesnt work
	//if (Ndistance<1)
		Ndistance=1;
	int ntraces=scope->getNTraces();
	//QVector <float>  data; 
	//data.resize(ntraces);

	rt_send(Target_Interface_Task, 0);
	
	n=(int)Ndistance;
        RTIME t0,t,told; int time=0; double time10=0.;double t10=0;int n10=0;
	t0 = rt_get_cpu_time_ns();
	mlockall(MCL_CURRENT | MCL_FUTURE);
	if (hardRealTime==1)
		rt_make_hard_real_time();
	while (true) {
		if (targetThread->getEndApp() || ! targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(Target_Node, GetScopeDataPort, GetScopeDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;
			//rt_sleep(nano2count(TEN_MS_IN_NS));
			//msleep(10);
			msleep(MsgWait);
		}

	if (targetThread->getVerbose()){
		t = rt_get_cpu_time_ns();told=t0;
		t0=rt_get_cpu_time_ns();
		if (n10==1000) {
			printf("Time failure soll %f zu ist: %f \n",time10,(t10));
			n10=0;
			t10=0.;
			time10=0.;
		} else {
			n10++;
			t10+=(t - told)*1.0E-6;
			time10+=time*dt*1.0E3;
		}
// 		if (((t - told)*1.0E-6)>25.)
// 		   printf("Time failure  %f (%f)\n",(t - told)*1.0E-6,time*dt*1.0E3 );
		}
		time=0;
		if (n>MsgData)
			n=n-MsgData;
		else {
                  
		   js = (n-1)*(ntraces+1);
		   if (n==1)
			js=0;
		   while(n<=MsgData){
                        if (scope->data2disk()->isSaveScopeTime()){
// 			    printf("Time %f\n",MsgBuf[js]);
			    scope->setScopeTime(MsgBuf[js++]);
			    
			}else
			    js++;
			for (nn = 0; nn < ntraces ; nn++) {
			
			// if (ScopeWindow)
			  //  ScopeWindow->setValue(nn,MsgBuf[js++]);
				//data[nn]=MsgBuf[js++];
			      if (scope->isPlotting())
				scope->setScopeValue(MsgBuf[js++],nn);
			      else
				js++;
			    //emit value(nn,MsgBuf[js++]);
				//temp1[nn][time-1]=MsgBuf[js++];
			    //ScopeWindow->getThread()->setValue(nn,MsgBuf[js++]);
			}
			time++;
			js=js+(Ndistance-1)*(ntraces+1);
			n=n+Ndistance;
		   }
		   n=n-MsgData;
		   
		}

		if (scope->getScopeDt()!=dt){
			dt=scope->getScopeDt();
			Ndistance=(int)(dt*(1./scope->getDt()));  //doesnt work
			if (Ndistance<1)
				Ndistance=1;
			MsgLen = (((int)(TracesBytes*dt*(1./scope->getDt())))/TracesBytes)*TracesBytes;
			//MsgLen = (((int)(TracesBytes*(1./targetThread->getScopeRefreshRate(index))*(1./scope.dt)))/TracesBytes)*TracesBytes;
			if (MsgLen < TracesBytes) MsgLen = TracesBytes;
			if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
			MsgData = MsgLen/TracesBytes;
			// Ndistance defines the distance between plotted datapoints, to archive the given refresh rate.


			MsgWait = (int)(((int)((MAX_MSG_SIZE-MsgLen)/TracesBytes))*scope->getDt()*1000);
			MsgWait=(MsgWait>WAIT_MIN)?MsgWait:WAIT_MIN;
			MsgWait=(MsgWait<WAIT_MAX)?MsgWait:WAIT_MAX;
			if (targetThread->getVerbose()){
				printf("change in dt: %f, MsgLen= %d, MsgData= %d, Ndistance= %d, MsgWait= %d\n",dt,MsgLen,MsgData,Ndistance,MsgWait);
			}
		}



                if (scope->data2disk()->getIsSaving()) {
			jl = 0;
			//printf("%d from %d saved\n",save_idx,targetThread->n_points_to_save(index));
			for (n = 0; n < MsgData; n++) {
				for (nn = 0; nn < ntraces + 1; nn++) {
                                        scope->data2disk()->writeNextData(MsgBuf[jl++],10);
                                        //fprintf(scope->save_file(), "%1.10f ", MsgBuf[jl++]);
				}
                                scope->data2disk()->newLine();
                                //fprintf(scope->save_file(), "\n");
				save_idx++;
                                scope->data2disk()->set_points_counter(save_idx);
                                //scope->set_points_counter_scope(save_idx);
                                if (save_idx == scope->data2disk()->n_points_to_save()) {
                                        //scope->stop_saving();
                                        scope->data2disk()->stopSaving();
					save_idx = 0;
					break;
				}
			}
		} else
		  save_idx = 0;
	}

end:
	if (hardRealTime==1)
		rt_make_soft_real_time();
	if (targetThread->getVerbose()) {
		printf("Deleting scope thread number...%d\n", index);
	}
	
	rt_release_port(Target_Node, GetScopeDataPort);
	rt_task_delete(GetScopeDataTask);

	return 0;
}







 //!QThread for reading Meter data
/*
class GetMeterDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
   QWaitCondition threadStarted;
   QMutex mutex;
 //signals:
   // void value(int,float);
    int setRefreshRate(double);
 public slots:
    void start(void* arg,TargetThread* targetthread); //, QRL_MeterWindow* meterwindow);
 private:
    int index,x,y,w,h;
    double RefreshRate;
    char* mbx_id;
    TargetThread* targetThread;
    //QRL_MeterWindow* MeterWindow;
    int MsgData, MsgLen, MaxMsgLen, DataBytes;
    long int Ndistance;
    double value;

 };
*/




static void *rt_get_meter_data(void *arg)
{
	RT_TASK *GetMeterDataTask;
	MBX *GetMeterDataMbx;
	char GetMeterDataMbxName[7];
	long GetMeterDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, DataBytes;
	float MsgBuf[MAX_MSG_LEN/sizeof(float)];
	int n;
	 long int Ndistance;
	int index = ((Args_T *)arg)->index;
	char *mbx_id = strdup(((Args_T *)arg)->mbx_id);
        TargetThread* targetThread=(TargetThread*)((Args_T *)arg)->targetThread;
	int hardRealTime = ((Args_T *)arg)->hardRealTime;

 	long Target_Node = targetThread->getTargetNode();
      	RT_TASK *Target_Interface_Task = targetThread->getTask();
	QRL_MeterData* meter = targetThread->getMeters()[index];
	double RefreshRate=meter->getMeterRefreshRate();
	rt_allow_nonroot_hrt();
	//mlockall(MCL_CURRENT | MCL_FUTURE);
	if (!(GetMeterDataTask = rt_task_init_schmod(qrl::get_an_id("HGM"), 97, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetMeterData Task\n");
		return (void *)1;
	}

	if(Target_Node == 0) GetMeterDataPort=0;
	else GetMeterDataPort = rt_request_port(Target_Node);

	sprintf(GetMeterDataMbxName, "%s%d", mbx_id, index);
	if (!(GetMeterDataMbx = (MBX *)RT_get_adr(Target_Node, GetMeterDataPort, GetMeterDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetMeterDataMbxName);
		exit(1);
	}
	//munlockall();
	DataBytes = sizeof(float);
	MaxMsgLen = (MAX_MSG_LEN/DataBytes)*DataBytes;
	MsgLen = (((int)(DataBytes/RefreshRate*(1./meter->getDt())))/DataBytes)*DataBytes;
	if (MsgLen < DataBytes) MsgLen = DataBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/DataBytes;
	Ndistance=(long int)(1./RefreshRate/meter->getDt());
	if (Ndistance<1)
		Ndistance=1;
	rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	n=Ndistance;
	if (hardRealTime==1)
		rt_make_hard_real_time();

	while (true) {
		if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(Target_Node, GetMeterDataPort, GetMeterDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;
			msleep(10); //waits for new Data from the mailbox
			//rt_sleep(nano2count(TEN_MS_IN_NS)); should not be used to high cpu load
		}

		if (n>MsgData)
			n=n-MsgData;
		else {
                   
		   int i;
		   for (i=(int)n-1; i < MsgData; i=i+(int)Ndistance){
			//if (MeterWindow)
			  //  MeterWindow->setValue(MsgBuf[i]);
			meter->setMeterValue(MsgBuf[i]);
		   }
		   n=Ndistance;
		}
		if (RefreshRate!=meter->getMeterRefreshRate()){
		
		RefreshRate=meter->getMeterRefreshRate();
		MsgLen = (((int)(DataBytes/RefreshRate*(1./meter->getDt())))/DataBytes)*DataBytes;
		if (MsgLen < DataBytes) MsgLen = DataBytes;
		if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
		MsgData = MsgLen/DataBytes;
                Ndistance=(long int)(1./RefreshRate/meter->getDt());
		if (Ndistance<1)
			Ndistance=1;
		
		}
	}       


end:
	if (hardRealTime==1)
		rt_make_soft_real_time();
	
	if (targetThread->getVerbose()) {
		printf("Deleting meter thread number...%d\n", index);
	}
	//Meter_Win->hide();
	rt_release_port(Target_Node, GetMeterDataPort);
	rt_task_delete(GetMeterDataTask);

	return 0;
}







/**
 * @brief QThread for reading Led data
 */
/*
class GetLedDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
QWaitCondition threadStarted;
QMutex mutex;
//signals:
    //void value(int,float);
 public slots:
    void start(void* arg,TargetThread* targetthread);//,QRL_LedWindow* ledwindow);
 private:
    int index,x,y,w,h;
    char* mbx_id;
    TargetThread* targetThread;
  //  QRL_LedWindow* LedWindow;
    unsigned int Led_Mask;
 };
*/

static void *rt_get_led_data(void *arg)
{
	RT_TASK *GetLedDataTask;
	MBX *GetLedDataMbx;
	char GetLedDataMbxName[7];
	long GetLedDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, DataBytes;
	unsigned int MsgBuf[MAX_MSG_LEN/sizeof(unsigned int)];	

	int index = ((Args_T *)arg)->index;
	char *mbx_id = strdup(((Args_T *)arg)->mbx_id);
	unsigned int Led_Mask = 0;
	TargetThread* targetThread=(TargetThread*)((Args_T *)arg)->targetThread;
	int hardRealTime = ((Args_T *)arg)->hardRealTime;
	 long Target_Node = targetThread->getTargetNode();
		RT_TASK *Target_Interface_Task = targetThread->getTask();
        QPL_LedData* led = targetThread->getLeds()[index];
	rt_allow_nonroot_hrt();
	//mlockall(MCL_CURRENT | MCL_FUTURE);
	if (!(GetLedDataTask = rt_task_init_schmod(qrl::get_an_id("HGE"), 97, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetLedData Task\n");
		return (void *)1;
	}
	if(Target_Node == 0) GetLedDataPort=0;
	else GetLedDataPort = rt_request_port(Target_Node);
	sprintf(GetLedDataMbxName, "%s%d", mbx_id, index);
	if (!(GetLedDataMbx = (MBX *)RT_get_adr(Target_Node, GetLedDataPort, GetLedDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetLedDataMbxName);
		exit(1);
	}
	//munlockall();
	DataBytes = sizeof(unsigned int);
	MaxMsgLen = (MAX_MSG_LEN/DataBytes)*DataBytes;
	MsgLen = (((int)(DataBytes*REFRESH_RATE*(1./led->getDt())))/DataBytes)*DataBytes;
	if (MsgLen < DataBytes) MsgLen = DataBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/DataBytes;


	rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	if (hardRealTime==1)
		rt_make_hard_real_time();

	while (true) {
		if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(Target_Node, GetLedDataPort, GetLedDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;

			msleep(10);
			//rt_sleep(nano2count(TEN_MS_IN_NS));
		}
		//for (n = 0; n < MsgData; n++) {
			Led_Mask = MsgBuf[0];
			led->setLedValue(Led_Mask);
		//}

	}


end:
	if (hardRealTime==1)
		rt_make_soft_real_time();
	if (targetThread->getVerbose()) {
		printf("Deleting led thread number...%d\n", index);
	}
	//Led_Win->hide();
	rt_release_port(Target_Node, GetLedDataPort);
	rt_task_delete(GetLedDataTask);

	return 0;
}












static void *rt_get_alog_data(void *arg)
{	
	RT_TASK *GetALogDataTask;				
	MBX *GetALogDataMbx;
	char GetALogDataMbxName[7];
	long GetALogDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, DataBytes;
	float MsgBuf[MAX_MSG_LEN/sizeof(float)];

	int n, i, j, k;
	int index = ((Alog_T *)arg)->index;
	char *mbx_id = strdup(((Alog_T *)arg)->mbx_id);
	char *alog_file_name = strdup(((Alog_T *)arg)->alog_name);   //read alog block name and set it to file name
	TargetThread* targetThread=(TargetThread*)((Alog_T *)arg)->targetThread;
	QRL_ALogData* alog = targetThread->getALogs()[index];
	int hardRealTime = ((Args_T *)arg)->hardRealTime;
	FILE *saving;
	long size_counter = 0;

	 long Target_Node = targetThread->getTargetNode();
		RT_TASK *Target_Interface_Task = targetThread->getTask();
	if((saving = fopen(alog_file_name, "a+")) == NULL){
		printf("Error opening auto log file %s\n", alog_file_name);
		}
	
	rt_allow_nonroot_hrt();
	//mlockall(MCL_CURRENT | MCL_FUTURE);
	if (!(GetALogDataTask = rt_task_init_schmod(qrl::get_an_id("HGA"), 95, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetALogData Task\n");
		return (void *)1;
	}


	
	if(Target_Node == 0) GetALogDataPort=0;
	else GetALogDataPort = rt_request_port(Target_Node);
	sprintf(GetALogDataMbxName, "%s%d", mbx_id, index);

	if (!(GetALogDataMbx = (MBX *)RT_get_adr(Target_Node, GetALogDataPort, GetALogDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetALogDataMbxName);
		exit(1);
	}
	 //munlockall();
	DataBytes = (alog->getNRow()*alog->getNCol())*sizeof(float)+sizeof(float);
	MaxMsgLen = (MAX_MSG_LEN/DataBytes)*DataBytes;
	MsgLen = (((int)(DataBytes*REFRESH_RATE*(1./alog->getDt())))/DataBytes)*DataBytes;
	if (MsgLen < DataBytes) MsgLen = DataBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/DataBytes;
	
	//printf("MsgData %d MsgLen %d MaxMsgLen %d DataBytes %d DimBuf= %d\n", MsgData, MsgLen, MaxMsgLen, DataBytes,MAX_MSG_LEN/sizeof(float));
	
	rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	if (hardRealTime==1)
		rt_make_hard_real_time();
	while (true) {
		if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) break;

		while (RT_mbx_receive_if(Target_Node, GetALogDataPort, GetALogDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;
			
			msleep(10);
		}
			for (n = 0; n < MsgData; n++) {
				size_counter=ftell(saving);    			//get file dimension in bytes
				//printf("Size counter: %d\n", size_counter);
				if(((int)MsgBuf[(((n+1)*alog->getNRow()*alog->getNCol() + (n+1))-1)]) &&
				size_counter<=1000000){
					for (i = 0; i < alog->getNRow(); i++) {
						j = n*alog->getNRow()*alog->getNCol() + i;
						for (k = 0; k < alog->getNCol(); k++) {
							fprintf(saving,"%1.5f ",MsgBuf[j]); 
							j += alog->getNRow();
						}
						fprintf(saving, "\n");
						j++;
					}
				}
				/*if (size_counter > 100000){
					fclose(saving);
					system("tar -czvf logged.tgz RTAI_ALOG");
					if((saving = fopen(alog_file_name, "a+")) == NULL){
						printf("Error opening auto log file %s\n", alog_file_name);
						}
				}*/		
			}
			
	}
end:
	if (hardRealTime==1)
		rt_make_soft_real_time();
	if (targetThread->getVerbose()) {
		printf("Deleting auto log thread number...%d\n", index);
	}
	fclose(saving);
	rt_release_port(Target_Node, GetALogDataPort);
	rt_task_delete(GetALogDataTask);

	return 0;
}



static void *rt_get_log_data(void *arg)
{
	RT_TASK *GetLogDataTask;
	MBX *GetLogDataMbx;
	char GetLogDataMbxName[7];
	long GetLogDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, DataBytes;
	float MsgBuf[MAX_MSG_LEN/sizeof(float)];
        long int Ndistance;
        int n, i, j, k, m, DataCnt = 0;
	int index = ((Args_T *)arg)->index;
	char *mbx_id = strdup(((Args_T *)arg)->mbx_id);
	TargetThread* targetThread=(TargetThread*)((Args_T *)arg)->targetThread;
	int hardRealTime = ((Args_T *)arg)->hardRealTime;
	QRL_LogData* log = targetThread->getLogs()[index];
         double RefreshRate=log->getLogRefreshRate();
	 long Target_Node = targetThread->getTargetNode();
	//float MsgBuf[log->getNCol()*log->getNRow()];
	RT_TASK *Target_Interface_Task = targetThread->getTask();
	rt_allow_nonroot_hrt();
	//mlockall(MCL_CURRENT | MCL_FUTURE);
	if (!(GetLogDataTask = rt_task_init_schmod(qrl::get_an_id("HGL"), 90, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetLogData Task\n");
		return (void *)1;
	}

	if(Target_Node == 0) GetLogDataPort=0;
	else GetLogDataPort = rt_request_port(Target_Node);
	sprintf(GetLogDataMbxName, "%s%d", mbx_id, index);

	if (!(GetLogDataMbx = (MBX *)RT_get_adr(Target_Node, GetLogDataPort, GetLogDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetLogDataMbxName);
		exit(1);
	}
	//munlockall();
	DataBytes = (log->getNRow()*log->getNCol())*sizeof(float);
	MaxMsgLen = (MAX_MSG_LEN/DataBytes)*DataBytes;
        MsgLen = (((int)(DataBytes/RefreshRate*(1./log->getDt())))/DataBytes)*DataBytes;
        Ndistance=(long int)(1./RefreshRate/log->getDt());
        if (Ndistance<1)
                Ndistance=1;
        n=Ndistance;
	if (MsgLen < DataBytes) MsgLen = DataBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/DataBytes;

	rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	if (hardRealTime==1)
		rt_make_hard_real_time();
	while (true) {
		if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(Target_Node, GetLogDataPort, GetLogDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;
			msleep(10);
		}
                if (log->isPlotting()) {
                    if (n>MsgData)
                        n=n-MsgData;
                    else {

                      int k;
                        for (m=(int)n-1; m < MsgData; m=m+(int)Ndistance){
                         n=MsgData-m-1;
                         for (i = 0; i < log->getNRow(); i++) {
                                        j = m*log->getNRow()*log->getNCol() + i;
                                        for (k = 0; k < log->getNCol(); k++) {
                                            log->setLogValue(MsgBuf[j],i,k);
                                                j += log->getNRow();
                                        }
                                }
                         log->add2Hist();
                        }
                        n=Ndistance-(n);
                    }
//                        for (n = 0; n < MsgData; n++) {
//                                for (i = 0; i < log->getNRow(); i++) {
//                                        j = n*log->getNRow()*log->getNCol() + i;
//                                        for (k = 0; k < log->getNCol(); k++) {
//                                            log->setLogValue(MsgBuf[j],i,k);
//                                                j += log->getNRow();
//                                        }
//                                }
//                                printf("n: %d,more then one matrix,ndist %d\n",n,Ndistance);
//                        }
                    if (!log->History()){
                    if (RefreshRate!=log->getLogRefreshRate()){

                        RefreshRate=log->getLogRefreshRate();
                        MsgLen = (((int)(DataBytes/RefreshRate*(1./log->getDt())))/DataBytes)*DataBytes;
                        if (MsgLen < DataBytes) MsgLen = DataBytes;
                        if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
                        MsgData = MsgLen/DataBytes;
                        Ndistance=(long int)(1./RefreshRate/log->getDt());
                        if (Ndistance<1)
                                Ndistance=1;

                    }
                   } else {
                        Ndistance=1;
                   }
                }


                if (log->data2disk()->getIsSaving()) {
			for (n = 0; n < MsgData; n++) {
				++DataCnt;
//				fprintf(targetThread->save_file_log(index), "Data # %d\n", ++DataCnt);
				for (i = 0; i < log->getNRow(); i++) {
					j = n*log->getNRow()*log->getNCol() + i;
					for (k = 0; k < log->getNCol(); k++) {
                                                //fprintf(log->save_file(), "%1.5f ", MsgBuf[j]);
                                                log->data2disk()->writeNextData(MsgBuf[j],5);
						j += log->getNRow();
					}
                                        log->data2disk()->newLine();
                                        //fprintf(log->save_file(), "\n");
				}
                                log->data2disk()->set_points_counter(DataCnt);
                                if (DataCnt == log->data2disk()->n_points_to_save()) {
                                        log->data2disk()->stopSaving();
					DataCnt = 0;
					break;
				}
			}
		} else
		    DataCnt = 0;
		
	}
end:
	if (hardRealTime==1)
		rt_make_soft_real_time();
	if (targetThread->getVerbose()) {
		printf("Deleting log thread number...%d\n", index);
	}
	rt_release_port(Target_Node, GetLogDataPort);
	rt_task_delete(GetLogDataTask);

	return 0;
}

