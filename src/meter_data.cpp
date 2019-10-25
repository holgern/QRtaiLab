/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
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
 file:		parameters.h
 describtion:
   file for the classes QRL_ParametersManager
*/



#include "meter_data.h"

QRL_MeterData::QRL_MeterData(float d,char* c_name)
        :dt(d)
{
		visible = false;
		meterRefreshRate=20.;
		MeterValues=0;
                name=std::string(c_name);
}

int QRL_MeterData::setMeterRefreshRate(double rr)
{
int ret=-1;
if (rr>0. && rr<50.){

	ret=1;
	meterRefreshRate=rr;

}
 //ret= Get_Meter_Data_Thread[n].setRefreshRate(rr);
return ret;
}

double QRL_MeterData::getMeterRefreshRate()
{
        double ret=-1;

		ret=meterRefreshRate;
	
	return ret;
}


void QRL_MeterData::setMeterValue(float v){

	//MeterValues[n].append(v);

// 	if (MeterValues[n].size()>0)
// 		MeterValues[n][0]=(v);
// 	else
// 		MeterValues[n].append(v);
	MeterValues=v;



}

float QRL_MeterData::getMeterValue(){

float ret=-1;

 //ret= Get_Led_Data_Thread[n].getValue();
//    if (MeterValues[n].size()>1)
//      ret=MeterValues[n].takeAt(0);
//    else
//      ret=MeterValues[n].at(0);
	ret = MeterValues;

return ret;



}
