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
 file:		qrtailab.cpp
 describtion:
   file for the classes TargetThread
*/

#include "qrtailab_core.h"
//#include "qrtailab_listener.h"
//#include "scope_window.h"
//#include "meter_window.h"
//#include "led_window.h"

using namespace qrl_types;
//static pthread_t *Get_Scope_Data_Thread;






QRtaiLabCore::QRtaiLabCore(QObject *parent, int Verbose)
: QObject( parent )
{


    targetthread = new TargetThread();
    targetthread->setVerbose(Verbose);
    unsigned int msg;

   	rt_allow_nonroot_hrt();
    //mlockall(MCL_CURRENT | MCL_FUTURE);
   if (!(RLG_Main_Task = rt_task_init_schmod(qrl::get_an_id("RLGM"), 98, 0, 0, SCHED_FIFO, 0xFF))) {
               printf("Cannot init RTAI-Lab GUI main task! Please load the  RTAI-modules!\n");
		exit( 1 );	
		//return 1;
    }
    //munlockall();
   targetthread->setMainTask(RLG_Main_Task);
   targetthread->start();
   rt_receive(0, &msg);
   Target_Interface_Task=targetthread->getTask();
   //statusBarMessage(tr("Ready..."));
Parameters = new QRL_Parameters(targetthread);
}

QRtaiLabCore::~QRtaiLabCore(){
	closeTargetThread();
 	targetthread->wait();
	delete targetthread;
        delete Parameters;
	rt_task_delete(RLG_Main_Task);
}




int QRtaiLabCore::stopTarget()
{
if(targetthread->getIsTargetConnected()==1){
	if (targetthread->getIsTargetRunning()==1) {
		qrl::RT_RPC(Target_Interface_Task, TargetThread::STOP_TARGET, 0);
	}
}
if (targetthread->getIsTargetRunning()==0)
	statusBarMessage(tr("Ready..."));

return targetthread->getIsTargetRunning();

}

int QRtaiLabCore::startTarget()
{
if(targetthread->getIsTargetConnected()==1){
	if (targetthread->getIsTargetRunning()==0) {
		qrl::RT_RPC(Target_Interface_Task, TargetThread::START_TARGET, 0);
	}
}
return !targetthread->getIsTargetRunning();

}

int QRtaiLabCore::connectToTarget()
{	
	char buf[128];
	sprintf(buf, "Trying to connect to %s", targetthread->getPreferences().Target_IP);
	statusBarMessage(tr(buf));
	qrl::RT_RPC(targetthread->getTask(), TargetThread::CONNECT_TO_TARGET, 0);
	if (targetthread->getIsTargetConnected()==1){

		
		sprintf(buf, "Target: %s.", targetthread->getTargetName());
		statusBarMessage(tr(buf));
	} else {
		if (targetthread->getTargetPort() <= 0) 
			statusBarMessage(tr("Sorry, no route to target"));
		else
			statusBarMessage(tr("No target or bad interface task identifier"));
	}
	return !targetthread->getIsTargetConnected();
}



int QRtaiLabCore::disconnectFromTarget()
{
if(targetthread->getIsTargetConnected()==1){
	qrl::RT_RPC(Target_Interface_Task, TargetThread::DISCONNECT_FROM_TARGET, 0);
	if (targetthread->getIsTargetConnected()==0){
		statusBarMessage(tr("Ready..."));
	}
}

return targetthread->getIsTargetConnected();
}



void QRtaiLabCore::closeTargetThread()
{
  	
	disconnectFromTarget();
	rt_send(Target_Interface_Task,TargetThread::CLOSE);


}

// void QRtaiLabCore::uploadParameters()
// {
// 	qrl::RT_RPC(Target_Interface_Task, TargetThread::GET_PARAMS, 0);
// }




QString QRtaiLabCore::getParameterName(int blk,int prm){

	return tr(targetthread->getParameters()[targetthread->getBlocks()[blk].offset+prm].param_name);
}
QString QRtaiLabCore::getBlockName(int blk){
	return tr(targetthread->getBlocks()[blk].name);
}

/*
void QRtaiLabCore::batchParameterDownload()
{

	//for (i = n = 0; i < batchCounter; i++) {
	//	n += Parameters_Manager->update_parameter(Batch_Parameters[i].index, Batch_Parameters[i].mat_index, Batch_Parameters[i].value);
	//}
	if (targetthread->getBatchCounter() > 0) {
		qrl::RT_RPC(Target_Interface_Task, TargetThread::BATCH_DOWNLOAD, 0);
		resetBatchMode();
	}
}

 unsigned   int QRtaiLabCore::getParameterCols(int blk,int prm){
	return targetthread->getParameters()[targetthread->getBlocks()[blk].offset+prm].n_cols;
}
   unsigned int QRtaiLabCore:: getParameterRows(int blk,int prm){
	return targetthread->getParameters()[targetthread->getBlocks()[blk].offset+prm].n_rows;
}



int QRtaiLabCore::getNumberOfParameters(int blk){
int jend;
	if (blk == targetthread->getBlockNumber() - 1) 
		jend=targetthread->getParameterNumber() - targetthread->getBlocks()[blk].offset;
	else
		jend=targetthread->getBlocks()[blk+1].offset-targetthread->getBlocks()[blk].offset;

return jend;
}





double QRtaiLabCore::getParameterValue(int blk,int prm, int nr,int nc)
{
	int val_idx;
	
	return targetthread->get_parameter(targetthread->getParameters()[targetthread->getBlocks()[blk].offset+prm], nr, nc, &val_idx);
					
}

void QRtaiLabCore::updateParameterValue(int blk,int prm, int nr,int nc, double value)
{
	int map_offset = targetthread->get_map_offset(blk,prm);
	int ind = targetthread->get_parameter_ind(blk,prm,nr,nc);
	if (targetthread->update_parameter(map_offset, ind, value)) {
		qrl::RT_RPC(Target_Interface_Task, (ind << 20) | (map_offset << 4) | TargetThread::UPDATE_PARAM, 0);
	}
}

void QRtaiLabCore::addToBatch(int blk,int prm, int nr,int nc,double value)
{
	int map_offset = targetthread->get_map_offset(blk,prm);
	int ind = targetthread->get_parameter_ind(blk,prm,nr,nc);
	if (targetthread->update_parameter(map_offset, ind, value)) {
		if (targetthread->addToBatch(map_offset,ind,value)==-1)
			printf("Could not add to Batch");
	}

}*/



 int QRtaiLabCore::getScopeNumber(){
   int Num_Scopes= targetthread->getScopeNumber();
   for (int i=0; i<Num_Scopes;i++) {
// good idea? should be changed
	if (targetthread->getScopes()[i]->getNTraces()==0)
		Num_Scopes--;
   }
	return Num_Scopes;
}

QString QRtaiLabCore::getScopeName(int n){
  QString str;
  if (n<targetthread->getScopeNumber())
        str=(targetthread->getScopes()[n]->getName());
  return str;

}
int	    QRtaiLabCore::getScopeDt(int n){
	int dt=-1;
     if (n<targetthread->getScopeNumber())
	dt=targetthread->getScopes()[n]->getDt();
      return dt;

}
    int    QRtaiLabCore::getNumberOfTraces(int n){
        int ntraces=0;
 if (n<targetthread->getScopeNumber())
	ntraces=targetthread->getScopes()[n]->getNTraces();
      return ntraces;

}





int QRtaiLabCore::getMeterNumber(){
int Num_Meters=targetthread->getMeterNumber();

return Num_Meters;
}

QString QRtaiLabCore::getMeterName(int n){
  QString str;
  if (n<targetthread->getMeterNumber())
	str=tr(targetthread->getMeters()[n]->getName());
  return str;

}
int	    QRtaiLabCore::getMeterDt(int n){
	int dt=-1;
     if (n<targetthread->getMeterNumber())
	dt=targetthread->getMeters()[n]->getDt();
      return dt;

}

int QRtaiLabCore::getLedNumber()
{
	 int Num_Leds= targetthread->getLedNumber();
	for (int i=0; i<Num_Leds;i++) {
// good idea? should be changed
	if (targetthread->getLeds()[i]->getNLeds()==0)
		Num_Leds--;
   }
return Num_Leds;

}

QString QRtaiLabCore::getLedName(int n){
  QString str;
  if (n<targetthread->getLedNumber())
	str=tr(targetthread->getLeds()[n]->getName());
  return str;

}
int	    QRtaiLabCore::getLedDt(int n){
	int dt=-1;
     if (n<targetthread->getLedNumber())
	dt=targetthread->getLeds()[n]->getDt();
      return dt;

}
    int    QRtaiLabCore::getNumberOfLeds(int n){
        int n_leds=0;
 if (n<targetthread->getLedNumber())
	n_leds=targetthread->getLeds()[n]->getNLeds();
      return n_leds;

}



int QRtaiLabCore::getLogNumber()
{
	 int Num_Logs= targetthread->getLogNumber();
return Num_Logs;

}

QString QRtaiLabCore::getLogName(int n){
  QString str;
  if (n<targetthread->getLogNumber())
	str=tr(targetthread->getLogs()[n]->getName());
  return str;

}





int QRtaiLabCore::getALogNumber()
{
	 int Num_Logs= targetthread->getALogNumber();
return Num_Logs;

}

QString QRtaiLabCore::getALogName(int n){
  QString str;
  if (n<targetthread->getALogNumber())
	str=tr(targetthread->getALogs()[n]->getName());
  return str;

}
