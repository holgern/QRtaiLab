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
 file:		parameter.cpp
 describtion:
   file for the classes QRL_ParametersManager
*/
#include "parameters.h"

QRL_ParameterMatrix::QRL_ParameterMatrix()
{
    n_rows=0;
    n_cols=0;
    name=QString("");

}

QRL_ParameterMatrix::~QRL_ParameterMatrix(){

}

void QRL_ParameterMatrix::setDim(unsigned int r, unsigned int c){
  values.resize(r);
  for (unsigned int i=0; i<r; i++)
    values[i].resize(c);
  n_rows=r;
  n_cols=c;
}

void QRL_ParameterMatrix::setName(QString n){
  name = n;
}

/**
 * @brief Initialize Parameter Clasas
 */
QRL_ParameterBlock::QRL_ParameterBlock(QString n, int num_parameters)
	:name(n),Num_Parameters(num_parameters)
{
	visible=true;
	ParameterMatrix = new QRL_ParameterMatrix* [Num_Parameters]; 
	for (int i=0; i<Num_Parameters; ++i){
		ParameterMatrix[i]=new QRL_ParameterMatrix();
	}
	
}



QRL_ParameterBlock::~QRL_ParameterBlock()
{
  for (int i=0; i<Num_Parameters; ++i) {
    delete ParameterMatrix[i];
  }
  delete[] ParameterMatrix;
}

void QRL_ParameterBlock::setParameterName(int param,QString name)
{
  ParameterMatrix[param]->setName(name);
}

void QRL_ParameterBlock::setParameterDim(int param, unsigned int n_rows, unsigned int n_cols){
  ParameterMatrix[param]->setDim(n_rows,n_cols);
}
void QRL_ParameterBlock::setParameterValue(int param, unsigned int n_rows, unsigned int n_cols, double value)
{
    ParameterMatrix[param]->setValue(n_rows,n_cols,value);


}
/**
 * @brief Initialize Parameter Clasas
 */
QRL_Parameters::QRL_Parameters(TargetThread	*t)
	:targetthread(t)
{
	Num_Tunable_Parameters=targetthread->getParameterNumber();
	Num_Tunable_Blocks=targetthread->getBlockNumber();
// 	TargetName=QObject::tr(targetthread->getTargetName());
	ParameterBlocks = new QRL_ParameterBlock* [Num_Tunable_Blocks]; 
// 	batchModus=0;
	for (int i=0; i<Num_Tunable_Blocks; ++i){
		ParameterBlocks[i]=new QRL_ParameterBlock(targetthread->getBlockName(i), targetthread->getNumberOfParameters(i));
		for (int k=0; k<targetthread->getNumberOfParameters(i);k++){
		  ParameterBlocks[i]->setParameterName(k,targetthread->getParameterName(i,k));
		  ParameterBlocks[i]->setParameterDim(k,targetthread->getParameterRows(i,k),targetthread->getParameterCols(i,k));
		}
	}
// 	uploadParameters();
	  for (int i=0; i<Num_Tunable_Blocks; ++i)
    for (int k=0; k<ParameterBlocks[i]->getNumberOfParameters();k++)
         for(unsigned int r=0; r<ParameterBlocks[i]->getParameterRows(k); r++)
           for(unsigned int c=0; c<ParameterBlocks[i]->getParameterCols(k); c++)
	    ParameterBlocks[i]->setParameterValue(k,r,c,targetthread->getParameterValue(i,k, r, c));
}

/**
 * @brief Delete Parameters Manager
 */
QRL_Parameters::~QRL_Parameters()
{
    for (int i=0; i<Num_Tunable_Blocks; ++i){
      delete ParameterBlocks[i];
    }
    delete[] ParameterBlocks;
}

// QString QRL_Parameters::getParameterName2(int blk,int prm){
// 
// 	return QObject::tr(targetthread->getParameters()[targetthread->getBlocks()[blk].offset+prm].param_name);
// }
// QString QRL_Parameters::getBlockName2(int blk){
// 	return QObject::tr(targetthread->getBlocks()[blk].name);
// }

// int QRL_Parameters::getNumberOfParameters2(int blk){
// int jend;
// 	if (blk == targetthread->getBlockNumber() - 1) 
// 		jend=targetthread->getParameterNumber() - targetthread->getBlocks()[blk].offset;
// 	else
// 		jend=targetthread->getBlocks()[blk+1].offset-targetthread->getBlocks()[blk].offset;
// 
// return jend;
// }
//  unsigned   int QRL_Parameters::getParameterCols2(int blk,int prm){
// 	return targetthread->getParameters()[targetthread->getBlocks()[blk].offset+prm].n_cols;
// }
//    unsigned int QRL_Parameters:: getParameterRows2(int blk,int prm){
// 	return targetthread->getParameters()[targetthread->getBlocks()[blk].offset+prm].n_rows;
// }
// 
// double QRL_Parameters::getParameterValue2(int blk,int prm, int nr,int nc)
// {
// 	int val_idx;
// 	
// 	return targetthread->get_parameter(targetthread->getParameters()[targetthread->getBlocks()[blk].offset+prm], nr, nc, &val_idx);
// 					
// }

void QRL_Parameters::batchParameterDownload()
{

	//for (i = n = 0; i < batchCounter; i++) {
	//	n += Parameters_Manager->update_parameter(Batch_Parameters[i].index, Batch_Parameters[i].mat_index, Batch_Parameters[i].value);
	//}
	if (targetthread->getBatchCounter() > 0) {
 		qrl::RT_RPC(targetthread->getTask(), TargetThread::BATCH_DOWNLOAD, 0);
		resetBatchMode();
	}
//  	uploadParameters();
}
void QRL_Parameters::updateParameterValue(int blk,int prm, int nr,int nc, double value)
{
	int map_offset = targetthread->get_map_offset(blk,prm);
	int ind = targetthread->get_parameter_ind(blk,prm,nr,nc);
	if (targetthread->update_parameter(map_offset, ind, value)) {
		qrl::RT_RPC(targetthread->getTask(), (ind << 20) | (map_offset << 4) | TargetThread::UPDATE_PARAM, 0);
	}
	//printf("prm %d, nr %d nc %d\n",prm, nr, nc);
	ParameterBlocks[blk]->setParameterValue(prm,nr,nc,targetthread->getParameterValue(blk,prm, nr, nc));
}

void QRL_Parameters::addToBatch(int blk,int prm, int nr,int nc,double value)
{
	int map_offset = targetthread->get_map_offset(blk,prm);
	int ind = targetthread->get_parameter_ind(blk,prm,nr,nc);
	if (targetthread->update_parameter(map_offset, ind, value)) {
		if (targetthread->addToBatch(map_offset,ind,value)==-1)
			printf("Could not add to Batch\n");
	}
	ParameterBlocks[blk]->setParameterValue(prm,nr,nc,value);
}

void QRL_Parameters::reload(){

 for (int i=0; i<Num_Tunable_Blocks; ++i){
      delete ParameterBlocks[i];
    }
    delete[] ParameterBlocks;
Num_Tunable_Parameters=targetthread->getParameterNumber();
	Num_Tunable_Blocks=targetthread->getBlockNumber();
	ParameterBlocks = new QRL_ParameterBlock* [Num_Tunable_Blocks]; 
// 	batchModus=0;
	for (int i=0; i<Num_Tunable_Blocks; ++i){
		ParameterBlocks[i]=new QRL_ParameterBlock(targetthread->getBlockName(i), targetthread->getNumberOfParameters(i));
		for (int k=0; k<targetthread->getNumberOfParameters(i);k++){
		  ParameterBlocks[i]->setParameterName(k,targetthread->getParameterName(i,k));
		  ParameterBlocks[i]->setParameterDim(k,targetthread->getParameterRows(i,k),targetthread->getParameterCols(i,k));
		}
	}
	
  for (int i=0; i<Num_Tunable_Blocks; ++i)
    for (int k=0; k<ParameterBlocks[i]->getNumberOfParameters();k++)
         for(unsigned int r=0; r<ParameterBlocks[i]->getParameterRows(k); r++)
           for(unsigned int c=0; c<ParameterBlocks[i]->getParameterCols(k); c++)
	    ParameterBlocks[i]->setParameterValue(k,r,c,targetthread->getParameterValue(i,k, r, c));
TargetName=QObject::tr(targetthread->getTargetName());
}

 void QRL_Parameters::uploadParameters()
 {
 	qrl::RT_RPC(targetthread->getTask(), TargetThread::GET_PARAMS, 0);
  for (int i=0; i<Num_Tunable_Blocks; ++i)
    for (int k=0; k<getNumberOfParameters(i);k++)
     	 for(int r=0; r<getParameterRows(i,k); r++)
	   for(int c=0; c<getParameterCols(i,k); c++)
	    ParameterBlocks[i]->setParameterValue(k,r,c,targetthread->getParameterValue(i,k, r, c));
 }


// void  QRL_Parameters::uploadParameters()
// {
  //qTargetInterface->uploadParameters();
  //qTargetInterface->resetBatchMode();

//   for (int i=0; i<Num_Tunable_Blocks; ++i)
//     for (int k=0; k<ParameterBlocks[i]->getNumberOfParameters();k++)
//      	 for(int r=0; r<ParameterBlocks[i]->getParameterRows(k); r++)
// 	   for(int c=0; c<ParameterBlocks[i]->getParameterCols(k); c++)
// 	    ParameterBlocks[i]->setParameterValue(k,r,c,getParameterValue2(i,k, r, c));
//   
// 
// }
