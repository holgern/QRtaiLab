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
 file:		parameters.h
 describtion:
   file for the classes QRL_ParametersManager
*/



#include "log_data.h"

QRL_LogData::QRL_LogData(int r,int c,float d,char* c_name)
        :nrow(r),ncol(c),dt(d)
{
	    visible = false;
//	    isSaving=0;
//  Save_File_Pointer=NULL;
	    plotting=false;

                logRefreshRate=20.;
                 LogValues.resize(this->getNRow());

                for (int t=0; t<this->getNRow(); t++){
                        LogValues[t].resize(this->getNCol());
                }
                name=std::string(c_name);
                d2d = new QPL_Data2Disk(dt);
                hist=false;
                hist_distance=100;
                hist_counter=0;
}

QRL_LogData::~QRL_LogData()
{
    delete d2d;
}


 void QRL_LogData::setLogValue(float v, int row, int col){

     if (hist_distance<=hist_counter) {
        hist_counter=0;
        LogValuesHist.append(LogValues);
     }
        LogValues[row][col]=v;

 }
void QRL_LogData::add2Hist(){


if (hist){
    //LogValuesHist.append(LogValues);
    hist_counter++;
}


}

  QVector< QVector<float> > QRL_LogData::getLogValue(){

        return LogValues;
}

   QVector<  QVector< QVector<float> > > QRL_LogData::getLogValueHist(){

       QVector<  QVector< QVector<float> > > ret(LogValuesHist);
       LogValuesHist.clear();
         // hist_counter=0;
       if (ret.size()==0)
            ret.append(LogValues);

       return ret;
}

  int QRL_LogData::setLogRefreshRate(double rr)
{
int ret=-1;
if (rr>0. && rr<150.){

        scopeRefreshRate=rr;

}
 //ret= Get_Meter_Data_Thread[n].setRefreshRate(rr);
return ret;
}

double QRL_LogData::getLogRefreshRate()
{
        double ret=-1;

                ret=scopeRefreshRate;

        return ret;
}
//int  QRL_LogData::start_saving() {return isSaving ;}
//
//void  QRL_LogData::startSaving(FILE* fp,double save_time){
//	Save_File_Pointer=fp;
//	Save_Time=save_time;
//	isSaving=1;
//}
//FILE*  QRL_LogData::save_file() {
//
//	return Save_File_Pointer;
//
//}
//     void  QRL_LogData::stop_saving(){
//	isSaving=0;
//	fclose(Save_File_Pointer);
//	Save_File_Pointer=NULL;
//	//emit stopSaving(index);
//
//}
//
//      int  QRL_LogData::n_points_to_save(){
//	int n_points;
//
//	n_points = (int)(Save_Time/dt);
//	if (n_points < 0) return 0;
//	return n_points;
//
//}
//
//
//
//  void  QRL_LogData::set_points_counter(int cnt){
//
// Saved_Points=cnt;
//
//}
