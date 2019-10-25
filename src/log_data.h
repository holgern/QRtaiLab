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

#ifndef _LOG_DATA_H
#define _LOG_DATA_H 1

#include <QtGui> 

#include "qrtailab.h"
#include "data2disk.h"

class QRL_LogData
{
public:
  // enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
        QRL_LogData(int, int, float,char*);
        ~QRL_LogData();
        const char* getName(){return name.c_str();}
	float getDt(){return dt;} 
	int getNRow(){return nrow;}
	int getNCol(){return ncol;}
//    int get_points_counter(){return Saved_Points;}
//    int getIsSaving(){return isSaving;}
//    int start_saving();
//    void startSaving(FILE* Save_File_Pointer,double Save_Time);
//     FILE* save_file();
//     void stop_saving();
//      int n_points_to_save();
//      void set_points_counter(int cnt);
    int setLogRefreshRate(double rr);
    double getLogRefreshRate();
    void setPlotting(bool b){plotting=b;}
    bool isPlotting(){return plotting;}
    void setHistory(bool b){hist=b;}
    bool History(){return hist;}
    void add2Hist();
    void setHistDistance(int d){hist_distance=d;}
    int getHistDistance(){return hist_distance;}
 QPL_Data2Disk* data2disk(){return d2d;}
        void setLogValue(float v, int row, int col);
        QVector< QVector<float> > getLogValue();
        QVector< QVector< QVector<float> > > getLogValueHist();
private:
        const int nrow;
        const int ncol;
        const float dt;
            std::string name;
        double logRefreshRate;
        QVector< QVector <float> >  LogValues;
        QVector< QVector< QVector <float> > > LogValuesHist;
        bool hist;
        int hist_length;
        int hist_distance;
        int hist_counter;
//	int isSaving;
//   	FILE* Save_File_Pointer;
//   	double Save_Time;
//	int Saved_Points;
    double scopeRefreshRate;
   bool plotting;
    int visible;
        QPL_Data2Disk* d2d;
};



#endif
