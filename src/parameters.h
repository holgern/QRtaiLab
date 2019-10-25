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

#ifndef _PARAMETERS_H
#define _PARAMETERS_H 1

#include <QtCore>
#include "target_thread.h"
/**
 * @brief Parameter Manager
 * @todo mark batched parameters
 */
class QRL_ParameterMatrix
{
public:
  QRL_ParameterMatrix();
  //QRL_ParameterMatrix(QString n, int rows, int cols);
  ~QRL_ParameterMatrix();
  double getValue(int r,int c){return values.at(r).at(c);}
  void setValue(int r, int c, double value) { values[r][c]=value;}
  unsigned int getRows(){return n_rows;}
  unsigned int getCols(){return n_cols;}
  void setName(QString n);
  QString getName(){return name;}
  void setDim(unsigned int r,unsigned int c);

private:
  QString name;
  unsigned int n_rows;
  unsigned int n_cols;
  unsigned int data_type;
  unsigned int data_class;
  QVector < QVector <double> > values;
};

class QRL_ParameterBlock
{
public:
  QRL_ParameterBlock(QString n, int num_parameters);
  ~QRL_ParameterBlock();
  int getNumberOfParameters(){return Num_Parameters;}
  unsigned int getParameterRows(int param){return ParameterMatrix[param]->getRows();}
  unsigned int getParameterCols(int param){return ParameterMatrix[param]->getCols();}
  QString getName()  {return name;}
  bool isVisible() {return visible;}
  void setVisible(bool v){visible=v;}
void setParameterName(int param,QString name);
  QString getParameterName(int param){return ParameterMatrix[param]->getName();}
  void setParameterDim(int param, unsigned int n_rows, unsigned int n_cols);
  void setParameterValue(int param, unsigned int n_rows, unsigned int n_cols, double value);
  double getParameterValue(int param, unsigned int n_rows, unsigned int n_cols){
    return ParameterMatrix[param]->getValue(n_rows,n_cols);}
  
private:
  QRL_ParameterMatrix **ParameterMatrix;
  QString name;
  int Num_Parameters;
  bool visible;
};



class QRL_Parameters
{
public:
  // enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
   QRL_Parameters( TargetThread	*t=NULL);
   ~QRL_Parameters();
    void  uploadParameters();
    QString getTargetName(){return TargetName;}
    QString getBlockName(int block){return ParameterBlocks[block]->getName();}
    QString getParameterName(int block, int param){return ParameterBlocks[block]->getParameterName(param);}
    bool isBlockVisible(int param){return ParameterBlocks[param]->isVisible();}
    void setBlockVisible(int param, bool v){ParameterBlocks[param]->setVisible(v);}
    int getParameterNumber(){return Num_Tunable_Parameters;}
    int getBlockNumber(){return Num_Tunable_Blocks;}
    int getNumberOfParameters(int block){return ParameterBlocks[block]->getNumberOfParameters();}
    int getParameterCols(int block, int param) {return ParameterBlocks[block]->getParameterCols(param);}
    int getParameterRows(int block, int param) {return ParameterBlocks[block]->getParameterRows(param);}
    double getParameterValue(int block, int param, int n_rows, int n_cols){return ParameterBlocks[block]->getParameterValue(param,n_rows,n_cols);}

     void resetBatchMode() {targetthread->resetBatchMode();}
    void batchParameterDownload();
    void updateParameterValue(int blk,int prm, int nr,int nc,double value);
    void addToBatch(int blk,int prm, int nr,int nc, double value);

    void reload();



private:
//    QString getParameterName2(int blk,int prm);
//    QString getBlockName2(int blk);
//  int getNumberOfParameters2(int blk);
//    unsigned int getParameterCols2(int blk,int prm);
//    unsigned int getParameterRows2(int blk,int prm);
//   double getParameterValue2(int blk,int prm, int nr,int nc);
   //  double get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx);
  QRL_ParameterBlock ** ParameterBlocks;
  int Num_Tunable_Parameters;
  int Num_Tunable_Blocks;
  QString TargetName;
//    QRtaiLabCore	*qTargetInterface;
  TargetThread* targetthread;
//   int batchModus;
};



#endif
