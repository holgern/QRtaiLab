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
 file:		parameters_manager.cpp
 describtion:
   file for the classes QRL_ParametersManager
*/
#include "parameters_manager.h"
#include <iostream>

/**
 * @brief Initialize Parameters Manager
 */
QRL_ParametersManager::QRL_ParametersManager(QWidget *parent, QRL_Parameters	*parameters)
	:QDialog(parent),Parameters(parameters)
{
	setupUi(this);
	
	Num_Tunable_Parameters=Parameters->getParameterNumber();
	Num_Tunable_Blocks=Parameters->getBlockNumber();
	
	connect( blockListWidget, SIGNAL(itemEntered( QListWidgetItem * ) ), this, SLOT( showTunableParameter( QListWidgetItem *  ) ) );
	connect( blockListWidget, SIGNAL(itemActivated( QListWidgetItem * ) ), this, SLOT( showTunableParameter( QListWidgetItem *  ) ) );
	connect( parameterTableWidget, SIGNAL( itemChanged( QTableWidgetItem * ) ), this, SLOT( changeTunableParameter( QTableWidgetItem *  ) ) );
	connect( batchCheckBox, SIGNAL( stateChanged( int  ) ), this, SLOT( batchMode( int  ) ) );
	connect( uploadPushButton, SIGNAL( pressed() ), this, SLOT( uploadParameters() ) );
	connect( downloadPushButton, SIGNAL( pressed() ), this, SLOT( downloadBatchParameters() ) );
	connect( fineRadioButton, SIGNAL( toggled( bool  ) ), this, SLOT( showAllBlocks( bool  ) ) );
	connect( hideRadioButton, SIGNAL( toggled( bool  ) ), this, SLOT( showBlocks( bool  ) ) );
	connect( showRadioButton, SIGNAL( toggled( bool  ) ), this, SLOT( hideBlocks( bool  ) ) );
	connect( paramLineEdit, SIGNAL( textEdited( const QString&  ) ), this, SLOT( changeSearchText( const QString&  ) ) );
	connect( savePushButton, SIGNAL( pressed() ), this, SLOT( saveParameter() ) );
	connect( loadPushButton, SIGNAL( pressed() ), this, SLOT( loadParameter() ) );
	BlockIcon =QIcon(QString::fromUtf8(":/icons/block_icon.xpm"));

	for (int i=0; i<Num_Tunable_Blocks; ++i){
	   
		new QListWidgetItem(BlockIcon,Parameters->getBlockName(i), blockListWidget);
	}
	batchModus=0;
	Parameters->uploadParameters();
	Parameters->resetBatchMode();
}

/**
 * @brief Delete Parameters Manager
 */
QRL_ParametersManager::~QRL_ParametersManager()
{
	blockListWidget->clear();
	parameterTableWidget->clear();
}

void QRL_ParametersManager::setFileVersion(qint32 v){
      fileVersion=v;

}

/**
 * @brief Enable batchmode. In batchmode, changed parameters will be downloaded to the program only by activating the uploading button
 * @param state enables batchmode
 */
void QRL_ParametersManager::batchMode(int state){
	if(state==Qt::Checked){
		downloadPushButton->setEnabled(true);
		batchModus=1;
	}else{
		downloadPushButton->setEnabled(false);
		batchModus=0;
		uploadParameters();
		Parameters->resetBatchMode();
	}
}

void QRL_ParametersManager::showAllBlocks( bool  state){
 blockListWidget->clear();
	if(state==false){
	  //QString str=(paramLineEdit->text()).remove(QChar(' '), Qt::CaseInsensitive);
	  QString str=(paramLineEdit->text());
	  QStringList strList = str.split(" ", QString::SkipEmptyParts);
	  QString tmp;
	    if (hideRadioButton->isChecked()){
	      for (int i=0; i<Num_Tunable_Blocks; ++i)
		Parameters->setBlockVisible(i,true);
	      for (int i=0; i<Num_Tunable_Blocks; ++i){
	       for (int s=0; s<strList.size();s++){
		tmp=Parameters->getBlockName(i);
		if (tmp.contains(strList.at(s), Qt::CaseInsensitive)){
		  Parameters->setBlockVisible(i,false);
		} 
	      }
	     }
	     for (int i=0; i<Num_Tunable_Blocks; ++i)
	       if ( Parameters->isBlockVisible(i))
		new QListWidgetItem(BlockIcon,Parameters->getBlockName(i), blockListWidget);
	    } else {
	      for (int i=0; i<Num_Tunable_Blocks; ++i)
		Parameters->setBlockVisible(i,false);
	      for (int i=0; i<Num_Tunable_Blocks; ++i){
	       for (int s=0; s<strList.size();s++){
		tmp=Parameters->getBlockName(i);
		if (tmp.contains(strList.at(s), Qt::CaseInsensitive)){
		  Parameters->setBlockVisible(i,true);
		} 
	      }
	     }
	     for (int i=0; i<Num_Tunable_Blocks; ++i)
	       if ( Parameters->isBlockVisible(i))
		new QListWidgetItem(BlockIcon,Parameters->getBlockName(i), blockListWidget);
	    }
	  
	}else{
	  for (int i=0; i<Num_Tunable_Blocks; ++i){
	     Parameters->setBlockVisible(i,true);
	      new QListWidgetItem(BlockIcon,Parameters->getBlockName(i), blockListWidget);
	  }
	}
}

void QRL_ParametersManager::showBlocks( bool  state){
  showAllBlocks(fineRadioButton->isChecked());
}

void QRL_ParametersManager::hideBlocks( bool  state){
   showAllBlocks(fineRadioButton->isChecked());
}

void QRL_ParametersManager::changeSearchText( const QString&  t){
if (!fineRadioButton->isChecked())
  showAllBlocks(fineRadioButton->isChecked());
}

/**
 * @brief Upload all parameters from the program, and updates the parameter in the widget
 */
void QRL_ParametersManager::uploadParameters()
{
	Parameters->uploadParameters();
	Parameters->resetBatchMode();
	showTunableParameter(blockListWidget->currentItem());
}
/**
 * @brief Download all changed parameters to the program.
 */
void QRL_ParametersManager::downloadBatchParameters()
{
	if(batchModus)
		Parameters->batchParameterDownload();

}

/**
 * @brief Updates a changed parameter
 */
void QRL_ParametersManager::changeTunableParameter(QTableWidgetItem * item ) 
{
	int blk =0;
	int blockCounter=-1;
	for (int j=0; j<Num_Tunable_Blocks; ++j){
	    if (Parameters->isBlockVisible(j))
		blockCounter++;
            if ((blockCounter==blockListWidget->currentRow())){
	      blk=j; 
              break;
	    }
	      
	}

	int ind=0;
        int prm_row=item->row();
	int prm_col=item->column()-1; //first column is paramter name
	double data_value;
	// get old value
	int jend;
	int prm=prm_row;
	int table_row=0;
        int nrow=0,ncol;
	
	// get old value
	jend=Parameters->getNumberOfParameters(blk);
	for (int j = 0; j <  jend; j++) {
		unsigned int ncols = Parameters->getParameterCols(blk,j);
		unsigned int nrows = Parameters->getParameterRows(blk,j);
		for (unsigned int nr = 0; nr < nrows; nr++) {
			for (unsigned int nc = 0; nc < ncols; nc++) {
				if ((prm_row==table_row) && (prm_col==nc)){
					
					nrow=nr;ncol=nc;
					prm=j;
				}
			}
			table_row++;
		}
		
	}
	data_value=Parameters->getParameterValue(blk,prm,nrow, ncol);
// 	printf("blk %d, parm %d, nr %d, nc %d, value %f,\n",blk,prm,nrow,ncol,data_value);
	
	QDoubleValidator *DoubleTest= new QDoubleValidator(NULL);
	QString s=item->text();
	int pos;
	if (DoubleTest->validate(s,pos)== QValidator::Invalid)
		item->setText(tr("%1").arg(data_value)); // set to old value
	else { // set to new value
		if (batchModus==0){
			double value=(item->text()).toDouble();
// 			printf("Item changed to %f (%d,%d,(%d,%d))\n",value,blk,prm,nrow,ncol);
			Parameters->updateParameterValue(blk,prm,nrow,ncol,value);
		} else {
			double value=(item->text()).toDouble();
			//printf("Item changed to %f (%d,%d)\n",value,blk,prm);
			Parameters->addToBatch(blk,prm,nrow,ncol,value);
		}
	}

	delete DoubleTest;
}
/**
 * @brief show all parameter for a given parameter block
 */
void QRL_ParametersManager::showTunableParameter(QListWidgetItem * item ) 
{
	parameterTableWidget->blockSignals(true);
	int i =0;
	int blockCounter=-1;
	for (int j=0; j<Num_Tunable_Blocks; ++j){
	    if (Parameters->isBlockVisible(j))
		blockCounter++;
            if ((blockCounter==blockListWidget->currentRow())){
	      i=j; 
              break;
	    }  
	}
        //std::cout << "i: "<<i << std::endl;
	parameterTableWidget->clear();
	int jend,val_idx;
	double data_value;
	//const QIcon BlockIcon =QIcon(QString::fromUtf8(":/icons/parameters_icon.xpm"));
	jend=Parameters->getNumberOfParameters(i);
	parameterTableWidget->setRowCount(jend);
	parameterTableWidget->setColumnCount(2);
	QTableWidgetItem *newItem;
	int table_row=0;
	for (int j = 0; j <  jend; j++) {
		newItem = new QTableWidgetItem(Parameters->getParameterName(i,j));
		newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
		parameterTableWidget->setItem(table_row,0,newItem);

		unsigned int ncols = Parameters->getParameterCols(i,j);
		if ((ncols+1)>parameterTableWidget->columnCount())
			parameterTableWidget->setColumnCount(ncols+1);
		unsigned int nrows =Parameters->getParameterRows(i,j);;
		if (nrows>1)
			parameterTableWidget->setRowCount(parameterTableWidget->rowCount()+nrows-1);
		for (unsigned int nr = 0; nr < nrows; nr++) {
			if(nr>0){
				newItem = new QTableWidgetItem(tr(" "));
				newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
				parameterTableWidget->setItem(table_row,0,newItem);
			}
			for (unsigned int nc = 0; nc < ncols; nc++) {
				data_value=Parameters->getParameterValue(i,j, nr, nc);
				newItem = new QTableWidgetItem(tr("%1").arg(data_value));
				parameterTableWidget->setItem(table_row,nc+1,newItem);
			}
			table_row++;
		}
		
	}
	parameterTableWidget->blockSignals(false);
}


    void QRL_ParametersManager::loadParameter(){
        QString filename = QFileDialog::getOpenFileName(this,tr("Load Parameter"), NULL, tr("Parameter(*.qpf);; All Files (*.*)"));
        loadParameter(filename);
}
 void QRL_ParametersManager::loadParameter(QString &filename){
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) return;
    QTextStream in(&file);
	qint32 mm,version;QString line;
	in >> mm >> version;
	
	if (mm!=DATA_STREAM_MAGIC_NUMBER) {
		file.close();
		QMessageBox::warning(NULL,"Error","Wrong file format! Could not load file!", QMessageBox::Ok );
		return;
	} else 	if (version<101) {
		file.close();
		QMessageBox::warning(NULL,"Error","Wrong version number! Could not load file!", QMessageBox::Ok );
		return;
	}
	in.readLine();
	QString target=in.readLine().mid(8);
// 	std::cout << line.toStdString();
	QString date = in.readLine();
        //int blocknr=in.readLine().mid(18).toInt();
// 	std::cout << line.toStdString();
	if (QMessageBox::question(NULL,"Load Parameter",tr("Should I load the parameter file for the target ")+target+tr("?\n ")+date, QMessageBox::Yes | QMessageBox::No )==QMessageBox::No)
	  return;
        int paramnr=0;
        int blocknr=0;
        int nr=0;
        int nc=0;
        double value;
        bool matrixmode=false;
        QString tmp;
        do {

            tmp=in.readLine();
            if (!tmp.isNull() && tmp.left(1)=="[" ){
                blocknr=tmp.mid(tmp.indexOf("[")+1,-1+tmp.indexOf("]")-tmp.indexOf("[")).toInt();
                tmp=in.readLine();
                matrixmode=false;
                //std::cout << blocknr << std::endl;
            }
           if (!tmp.isNull()){
               if (!matrixmode) {
                paramnr=tmp.mid(tmp.indexOf("[")+1,-1+tmp.indexOf("]")-tmp.indexOf("[")).toInt();
                if (tmp.indexOf("=")>-1) {
                    value= tmp.mid(tmp.indexOf("=")+1).toDouble();
                     //std::cout << blocknr << " "<< paramnr<<" "<<value<<std::endl;
                       if (batchModus==0){
                                Parameters->updateParameterValue(blocknr,paramnr,nr,nc,value);
                        } else {
                                Parameters->addToBatch(blocknr,paramnr,nr,nc,value);
                        }

                } else {
                    //matrix case
                    matrixmode=true;
                }
            } else { //matrixmode
                 if (tmp.indexOf("(")>-1) {
                      nr=tmp.mid(tmp.indexOf("(")+1,-1+tmp.indexOf(",")-tmp.indexOf("(")).toInt();
                      nc=tmp.mid(tmp.indexOf(",")+1,-1+tmp.indexOf(")")-tmp.indexOf(",")).toInt();
                      value= tmp.mid(tmp.indexOf("=")+1).toDouble();
                      // std::cout << blocknr << " "<< paramnr<<" "<<nr<<" "<<nc<<" "<<value<<std::endl;
                        if (batchModus==0){
                                Parameters->updateParameterValue(blocknr,paramnr,nr,nc,value);
                        } else {
                                Parameters->addToBatch(blocknr,paramnr,nr,nc,value);
                        }
                 } else{
                      matrixmode=false;
                      paramnr=tmp.mid(tmp.indexOf("[")+1,-1+tmp.indexOf("]")-tmp.indexOf("[")).toInt();
                      if (tmp.indexOf("=")>-1) {
                    value= tmp.mid(tmp.indexOf("=")+1).toDouble();
                     //std::cout << blocknr << " "<< paramnr<<" "<<value<<std::endl;
                       if (batchModus==0){
                                Parameters->updateParameterValue(blocknr,paramnr,nr,nc,value);
                        } else {
                                Parameters->addToBatch(blocknr,paramnr,nr,nc,value);
                        }

                        } else {
                            //matrix case
                            matrixmode=true;
                        }
                  }

            }
            }
        }while(!tmp.isNull());
        file.close();
  if(batchModus)
                Parameters->batchParameterDownload();
}
    void QRL_ParametersManager::saveParameter(){


QString filename = QFileDialog::getSaveFileName(this,tr("Save Parameter"), NULL, tr("parameter (*.qpf);; All Files (*.*)")); 
	QFile file(filename);
  Parameters->uploadParameters();
   QString str;
   if (!file.open(QIODevice::WriteOnly)) return;
   QTextStream out(&file);
   //ts << tr("#");



             // Write a header with a "magic number" and a version
 out << (quint32)DATA_STREAM_MAGIC_NUMBER<<endl;
 out << (qint32)DATA_STREAM_VERSION<<endl;
 out << "Target: " << Parameters->getTargetName()<<endl;
 out << "Date: " << QDate::currentDate().toString("dd.MM.yyyy");
 out << " "<<QTime::currentTime().toString("hh:mm")<<endl;
 //out << "Number of Blocks: "<< Parameters->getBlockNumber()<<endl;
 for (int blk=0;blk<Parameters->getBlockNumber();blk++){
  if (Parameters->isBlockVisible(blk)) {
//  if (blk<10)
//    out << "[ "<<blk<<"] ";
//  else
     out << "["<<blk<<"] ";
    out<<Parameters->getBlockName(blk) << endl;
    // out << "Number of Parameters: "<< Parameters->getNumberOfParameters(blk)<<endl;
    for (int prm=0;prm<Parameters->getNumberOfParameters(blk);prm++){
//        if (prm<10)
//    out << "  [ "<<prm<<"] ";
//  else
     out << "  ["<<prm<<"] ";
      out << Parameters->getParameterName(blk,prm);
        if (Parameters->getParameterRows(blk,prm)==1 && Parameters->getParameterCols(blk,prm)==1){
          out << " = " << Parameters->getParameterValue(blk,prm,0,0) << endl;

        } else {
	out << endl;
	for (int nr=0;nr<Parameters->getParameterRows(blk,prm);nr++){
	   for (int nc=0;nc<Parameters->getParameterCols(blk,prm);nc++){
                out << "  ("<<nr<<","<<nc<<")  = "<<Parameters->getParameterValue(blk,prm,nr,nc)<< " ";
	   }
	  out <<endl;
        }
      }
    }
    }


    }


   file.close();
}

QDataStream& operator<<(QDataStream &out, const QRL_ParametersManager &d){
	out << d.size()  << d.pos() << d.isVisible();
	out << d.paramLineEdit->text();
	out << d.showRadioButton->isChecked();
	out << d.hideRadioButton->isChecked();
	out << d.fineRadioButton->isChecked();
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_ParametersManager(&d)){
	QSize s;QPoint p;bool b; int i;QString str;
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	if (d.fileVersion > 103){
	  in >> str; d.paramLineEdit->setText(str);
	  in >> b; d.showRadioButton->setChecked(b);
	  in >> b; d.hideRadioButton->setChecked(b);
	  in >> b; d.fineRadioButton->setChecked(b);

	}
	return in;
}
