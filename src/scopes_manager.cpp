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
 file:		scopes_manager.cpp
 describtion:
   file for the classe QRL_ScopesManager
*/

#include "scopes_manager.h"


/**
* @brief Initialize Scopes Manager
*/
QRL_ScopesManager::QRL_ScopesManager(QWidget *parent, int numScopes, QPL_ScopeData **scopes, int v)
        :QDialog(parent),Num_Scopes(numScopes),Scopes(scopes),verbose(v)
{
	setupUi(this);
        //Num_Scopes=qTargetInterface->getScopeNumber();
        //Scopes=qTargetInterface->getScopes();
	const QIcon ScopeIcon =QIcon(QString::fromUtf8(":/icons/scope_icon.xpm"));
        ScopeWindows = new QRL_ScopeWindow* [Num_Scopes];
	for (int i=0; i<Num_Scopes; ++i){
                //scopeItems << new QListWidgetItem(ScopeIcon,qTargetInterface->getScopeName(i), scopeListWidget);
                scopeItems << new QListWidgetItem(ScopeIcon,tr("%1").arg(i), scopeListWidget);

		//if (qTargetInterface->getNumberOfTraces(i)>0){
			ScopeWindows[i]=new QRL_ScopeWindow(parent,Scopes[i],i);
			//connect( ScopeWindows[i], SIGNAL( stopSaving(int) ), this, SLOT( stopSaving(int) ) );
                        //ScopeWindows[i]->setVerbose(qTargetInterface->getVerbose());
                        ScopeWindows[i]->scope()->setVerbose(verbose);
		//} else
		//	ScopeWindows[i]=NULL;
	}

	tabWidget->setCurrentIndex(0);
	connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showScope(int) ) );
	connect( scopeListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showOptions( QListWidgetItem *  ) ) );
	connect( scopeListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showOptions( QListWidgetItem *  ) ) );
	connect( scopeListWidget, SIGNAL( itemSelectionChanged() ), this, SLOT( showSelectedOptions() ) );
	//connect( rrLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeRefreshRate(const QString&) ) );
	connect( rrCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeRefreshRate(double) ) );
	connect( dataCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDataPoints(double) ) );
	connect( dxComboBox, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( changeDX(const QString&) ) );
	connect( dxComboBox, SIGNAL( editTextChanged(const QString &) ), this, SLOT( changeDX(const QString&) ) );
	connect( savePushButton, SIGNAL( pressed() ), this, SLOT( startSaving() ) );
        connect( displayComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDisplayModus(int) ) );
	connect( directionComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDirection(int) ) );
	connect( optionComboBox, SIGNAL( activated(int) ), this, SLOT( setOptions(int) ) );
	connect( traceColorPushButton, SIGNAL( pressed()), this, SLOT(changeTraceColor()));
	connect( lineWidthCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeTraceWidth(double) ) );
	connect( offsetCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeOffset(double) ) );
	connect( offsetWheel, SIGNAL( valueChanged(double) ), this, SLOT( changeOffset(double) ) );
	connect( dyComboBox, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( changeDy(const QString&) ) );
	connect( dyComboBox, SIGNAL( editTextChanged(const QString &) ), this, SLOT( changeDy(const QString&) ) );
	connect( tabWidget, SIGNAL( currentChanged(int) ), this, SLOT( changeScopeList(int) ) );
	connect( triggerCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeTriggerLevel(double) ) );
	connect( triggerPushButton, SIGNAL( pressed()), this, SLOT(manualTrigger() ) );
	connect( oneShotCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( changeSingleMode(int) ) );
	connect( startTriggerPushButton, SIGNAL( pressed()), this, SLOT(startSingleRun() ) );
	connect( traceNameLineEdit,SIGNAL( textChanged ( const QString &  ) ), this , SLOT( changeTraceText(const QString & ) ) );
        connect( traceComboBox,SIGNAL( currentIndexChanged(int) ),this, SLOT(changeTriggerChannel(int) ) );
        connect( zeroAxisCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeZeroAxis(int) ) );
	 connect( labelCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeTraceLabel(int) ) );
	 connect( unitCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeUnitLabel(int) ) );
	 connect( averageCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeAverageLabel(int) ) );
	 connect( minCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeMinLabel(int) ) );
	 connect( maxCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeMaxLabel(int) ) );
	 connect( ppCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changePPLabel(int) ) );
	 connect( rmsCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeRMSLabel(int) ) );
	connect( showTraceCheckBox , SIGNAL( stateChanged(int) ), this, SLOT( showTrace(int) ) );
         connect( timeCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeSaveTime(double) ) );
	connect( fileLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeFileName(const QString&) ) );
	connect( dirLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeFileDir(const QString&) ) );
	connect( dividerCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDivider(double) ) );
	connect( stopPushButton, SIGNAL( pressed() ), this, SLOT( stopSaving() ) );
	connect( holdCheckBox , SIGNAL( stateChanged(int) ), this, SLOT( holdPlot(int) ) );
	connect( dirPushButton, SIGNAL( pressed()), this, SLOT(setFileDirectory() ) );
	connect( setMeanPushButton, SIGNAL( pressed()), this, SLOT(setOffsetToMean() ) );
        connect( fitDyPushButton, SIGNAL( pressed()), this, SLOT(fitDytoPP() ) );
        connect( styleComboBox,SIGNAL( currentIndexChanged(int) ),this, SLOT(setTraceStyle(int) ) );

         connect( lineStyleComboBox,SIGNAL( currentIndexChanged(int) ),this, SLOT(setLineStyle(int) ) );
         connect( symbolStyleComboBox,SIGNAL( currentIndexChanged(int) ),this, SLOT(setSymbolStyle(int) ) );
         connect( symbolSizeCounter, SIGNAL( valueChanged(double) ), this, SLOT( setSymbolSize(double) ) );
         connect( sPenColorPushButton, SIGNAL( pressed()), this, SLOT(changeSymbolBrushColor()));
         connect( sBrushColorPushButton, SIGNAL( pressed()), this, SLOT(changeSymbolPenColor()));

	currentScope=0;
// 	for(int i=0; i<1; ++i){
// 		//tabWidget->addTab(new QWidget(tabWidget->widget(1)),tr("Trace ")+tr("%1").arg(i+1));
// 		traceComboBox->addItem(tr("Trace ")+tr("%1").arg(i+1));
// 	}
	traceComboBox->clear();
	for(int i=0; i<Scopes[currentScope]->getNTraces();i++){
                traceComboBox->addItem(	ScopeWindows[currentScope]->scope()->trace(i)->getName() );
	}
	if (Num_Scopes > 0)  showScopeOptions(currentScope);
	
//	if (Num_Scopes > 0) Get_Scope_Data_Thread = new GetScopeDataThread [Num_Scopes];
	//for (int i=0; i<Num_Scopes; ++i){
	//	connect(&Get_Scope_Data_Thread[i],SIGNAL(value(int,float)),ScopeWindows[i],SLOT(setValue(int,float)));
	//}
	offsetWheel->setMass(0.5);
	offsetWheel->setRange(-1e6, 1e6, 0.25);
	offsetWheel->setTotalAngle(360.0*2e6);
	offsetWheel->setFixedHeight(30);
	dxComboBox->setCompleter(0);
	dxComboBox->setValidator(new QDoubleValidator(this));

       // RefreshRate=20.;
	timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./5.*1000.));
	
	
//	tabWidget->addTab(traceWidget,tr("trace %1").arg(Scopes[currentScope].getNTraces()));

}
QRL_ScopesManager::~QRL_ScopesManager()
{
	for (int i=0; i<Num_Scopes; ++i){
		ScopeWindows[i]->hide();
		delete ScopeWindows[i];
	  
	}
	delete[] ScopeWindows;
	//stopScopeThreads();
	//if (Get_Scope_Data_Thread)
	//	delete[] Get_Scope_Data_Thread;
}

void QRL_ScopesManager::refresh()
{

//try {
//  for (int n=0; n<Num_Scopes; ++n){
//	//QVector< QVector<float> > v;
//	//v = qTargetInterface->getTargetThread()->getScopeValue(n);
//	//if (v.size()>0) {
//
////	printf("size scopevector:  %d x %d\n",v.size(),v.at(0).size());
//
//// 	for (int t=0; t<qTargetInterface->getNumberOfTraces(n);++t)
//// 	for (int k=0; k<v.at(0).size(); ++k){
//// 			if (k<v.at(t).size())
//// 			ScopeWindows[n]->setValue(t,v.at(t).at(k));
//// 	}
//     if (Scopes[n]->dataAvailable()) {
//	if (Scopes[n]->getNTraces()>0) {
//          if (Scopes[n]->data2disk()->isSaveScopeTime())
//             ScopeWindows[n]->scope()->setTime( Scopes[n]->getScopeTime());
//          ScopeWindows[n]->scope()->setValue( Scopes[n]->getScopeValue());
//	 }
//        //}
//        }
//    }



// } catch (...){
// 	qDebug()<<"error in ScopesManager::refresh";
// 
// }

  if (!ScopeWindows[currentScope]->isVisible() && showCheckBox->isChecked())
      showCheckBox->setCheckState(Qt::Unchecked);

  if (Scopes[currentScope]->data2disk()->getIsSaving()==0){
      savePushButton->setEnabled(true);
      stopPushButton->setEnabled(false);
       saveProgressBar->setEnabled(false);
      saveProgressBar->setMaximum(100);
      saveProgressBar->setValue(100);
  }
  else
  {
     savePushButton->setEnabled(false);
     stopPushButton->setEnabled(true);
    saveProgressBar->setEnabled(true);
    saveProgressBar->setMaximum(Scopes[currentScope]->data2disk()->n_points_to_save());
     saveProgressBar->setValue(Scopes[currentScope]->data2disk()-> getSavedPoints());
  }


}

    void QRL_ScopesManager::setFileVersion(qint32 v){
      fileVersion=v;
      for (int i=0; i<Num_Scopes;i++)
        ScopeWindows[i]->scope()->setFileVersion(v);

}

/**
* @brief set new refresh rate
* @param rr refresh rate
*/
void QRL_ScopesManager::changeRefreshRate(double rr)
{
	//double rr=text.toDouble();
        ScopeWindows[currentScope]->scope()->changeRefreshRate(rr);
        ScopeWindows[currentScope]->changeRefreshRate(rr);
        Scopes[currentScope]->setScopeRefreshRate(rr);
}


void QRL_ScopesManager::changeDX(const QString& text)
{
	if (!text.isEmpty() &&text.toDouble()!=0.0 ){
	double dx=text.toDouble();
        ScopeWindows[currentScope]->scope()->changeDX(dx);
        emit dxComboBox->setEditText(tr("%1").arg(ScopeWindows[currentScope]->scope()->getDX()));
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
         Scopes[currentScope]->setScopeDt(ScopeWindows[currentScope]->scope()->getDt());

	showScopeOptions(currentScope);
	}
}

void  QRL_ScopesManager::setScopeName(int i,QString name){

    if (i<scopeItems.size())
    scopeItems[i]->setText(name);

    showScopeOptions(currentScope);

}



/**
* @brief change the number of datapoints in the plot
* @param dp data points
*/
void QRL_ScopesManager::changeDataPoints(double dp)
{
	//double rr=text.toDouble();
        ScopeWindows[currentScope]->scope()->changeDataPoints(dp);
	//ScopeWindows[currentScope]->changeDX(dxComboBox->currentText().toDouble());
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
         Scopes[currentScope]->setScopeDt(ScopeWindows[currentScope]->scope()->getDt());

	if (Num_Scopes > 0)  showScopeOptions(currentScope);

}

/**
* @brief change the number of datapoints in the plot
* @param dp data points
*/
void QRL_ScopesManager::changeDivider(double div)
{
	//double rr=text.toDouble();
        ScopeWindows[currentScope]->scope()->changeDivider(div);
	//ScopeWindows[currentScope]->changeDataPoints(dataCounter->value()/div);
	//ScopeWindows[currentScope]->changeDX(dxComboBox->currentText().toDouble());
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
         Scopes[currentScope]->setScopeDt(ScopeWindows[currentScope]->scope()->getDt());

	if (Num_Scopes > 0)  showScopeOptions(currentScope);

}

void QRL_ScopesManager::setOptions(int index)
{
	QColor color;
	switch(index) {
		case 1	:
                color= QColorDialog::getColor(ScopeWindows[currentScope]->scope()->getBgColor());
                ScopeWindows[currentScope]->scope()->setBgColor(color);
		break;
		case 2 :
                color= QColorDialog::getColor(ScopeWindows[currentScope]->scope()->getGridColor());
                ScopeWindows[currentScope]->scope()->setGridColor(color);
		break;
		default	:;
	}
}

void QRL_ScopesManager::setFileDirectory(){

 QString dir = QFileDialog::getExistingDirectory(this, tr("File Directory"),
                                                 dirLineEdit->text(),
                                                 QFileDialog::ShowDirsOnly);
  dirLineEdit->setText(dir);

}

 void QRL_ScopesManager::setFileName(int scopeNumber, const QString& filename){

  if (scopeNumber<Num_Scopes)
     Scopes[scopeNumber]->data2disk()->setFileName(filename);

     if (scopeNumber==currentScope){


        if (!filename.contains(QDir::separator())){
            dirLineEdit->clear();
            fileLineEdit->clear();
            dirLineEdit->insert(tr("./"));
            fileLineEdit->insert(filename);
        } else {
                dirLineEdit->clear();
            fileLineEdit->clear();
            int pos = filename.lastIndexOf(    QDir::separator());
            dirLineEdit->insert(filename.mid(0,pos+1));
            fileLineEdit->insert(filename.mid(pos+1,filename.size()));

        }
    }
}

  void QRL_ScopesManager::setSaveTime(int scopeNumber, double savetime, bool autosave){

if (scopeNumber<Num_Scopes){
     Scopes[scopeNumber]->data2disk()->setSaveTime(savetime);
     if (autosave){
                if (QFile::exists(Scopes[scopeNumber]->data2disk()->getFileName())) {
                        printf("File %s exists already.\n",Scopes[scopeNumber]->data2disk()->getFileName().toLocal8Bit().data() );
                        QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("The File exists! Please change the name!"),
                                     QMessageBox::Abort);
                } else {
                //if ((Save_File_Pointer = fopen((File_Name.toLocal8Bit()).data(), "a+")) == NULL) {
                  if (!Scopes[scopeNumber]->data2disk()->startSaving()) {
                        printf("Error in opening file %s\n",Scopes[scopeNumber]->data2disk()->getFileName().toLocal8Bit().data() );
                        QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("Error in opening file!"),
                                     QMessageBox::Abort);
                }

     }
 }

 }
if (scopeNumber==currentScope){
timeCounter->setValue(savetime);

}



  }

void QRL_ScopesManager::startSaving()
{

	double Save_Time=timeCounter->value();
        if( Scopes[currentScope]->data2disk()->getIsSaving()==0){

                    QString File_Name=dirLineEdit->text();
                    if (File_Name.isEmpty())
                        File_Name="./";
                    else if (!File_Name.endsWith(QDir::separator()))
                          File_Name+=QDir::separator();

                               File_Name+=fileLineEdit->text();
		if (QFile::exists(File_Name)) {
			printf("File %s exists already.",File_Name.toLocal8Bit().data() );
			QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("The File exists! Please change the name!"),
                                     QMessageBox::Abort);
		} else {
		 
		  
		
                //if ((Save_File_Pointer = fopen((File_Name.toLocal8Bit()).data(), "a+")) == NULL) {
                  if (!Scopes[currentScope]->data2disk()->startSaving(File_Name.toLocal8Bit().data(),Save_Time)) {
			printf("Error in opening file %s",File_Name.toLocal8Bit().data() );
			QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("Error in opening file!"),
                                     QMessageBox::Abort);
                } //else {
			//savePushButton->setEnabled(false);
                //	Scopes[currentScope]->startSaving(Save_File_Pointer,Save_Time);
                //}

		
		//ScopeWindows[currentScope]->startSaving(fileLineEdit->text());
		//targetThread->startSaving(fileLineEdit->text(),currentScope);


	       }
       }


}

void QRL_ScopesManager::stopSaving()
{
 if (Scopes[currentScope]->data2disk()->getIsSaving()==1){
         Scopes[currentScope]->data2disk()->stopSaving();
  }
}

void QRL_ScopesManager::changeScopeList(int index)
{
	if (index==0){
		scopeListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		for(int i=0; i<scopeItems.size();i++)
			scopeItems[i]->setHidden(false);
		for(int i=0; i<traceItems.size();i++)
			traceItems[i]->setHidden(true);
	}else{
	   if (Scopes[currentScope]->getNTraces()>0) {
		//scopeListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
		for(int i=0; i<scopeItems.size();i++)
			scopeItems[i]->setHidden(true);
		if (traceItems.size()==0) {
			for(int i=0; i<Scopes[currentScope]->getNTraces();i++)
				traceItems << new QListWidgetItem(QIcon(),tr("trace %1").arg(i), scopeListWidget);
		} else {
			for(int i=0; i<traceItems.size();i++)
				traceItems[i]->setHidden(false);
		}
			showTraceOptions(currentTrace+scopeItems.size());
           } else {
		tabWidget->setCurrentIndex(0);
	  }
	}
}



void QRL_ScopesManager::showTraceOptions(int index)
{
	currentTrace=index-scopeItems.size();	
        lineWidthCounter->setValue(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getWidth());
        offsetCounter->setValue(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getOffset());
        dyComboBox->setEditText(tr("%1").arg(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getDy()));

        symbolSizeCounter->setValue(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getSymbolSize());
        lineStyleComboBox->setCurrentIndex(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getLineStyle());
        symbolStyleComboBox->setCurrentIndex(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getSymbolStyle()+1);

	if (currentTrace<traceItems.size())
	traceNameLineEdit->setText(traceItems[currentTrace]->text());
	//tabWidget->setTabText(1,traceItems[currentTrace]->text());
        if (ScopeWindows[currentScope]->scope()->trace(currentTrace)->getZeroAxis())
		zeroAxisCheckBox->setCheckState(Qt::Checked);
	else
		zeroAxisCheckBox->setCheckState(Qt::Unchecked);

        if (ScopeWindows[currentScope]->scope()->getTraceLabel(currentTrace))
		labelCheckBox->setCheckState(Qt::Checked);
	else
		labelCheckBox->setCheckState(Qt::Unchecked);

        if (ScopeWindows[currentScope]->scope()->getUnitLabel(currentTrace))
		unitCheckBox->setCheckState(Qt::Checked);
	else
		unitCheckBox->setCheckState(Qt::Unchecked);

        if (ScopeWindows[currentScope]->scope()->getAverageLabel(currentTrace))
		averageCheckBox->setCheckState(Qt::Checked);
	else
		averageCheckBox->setCheckState(Qt::Unchecked);

        if (ScopeWindows[currentScope]->scope()->getMinLabel(currentTrace))
		minCheckBox->setCheckState(Qt::Checked);
	else
		minCheckBox->setCheckState(Qt::Unchecked);

        if (ScopeWindows[currentScope]->scope()->getMaxLabel(currentTrace))
		maxCheckBox->setCheckState(Qt::Checked);
	else
		maxCheckBox->setCheckState(Qt::Unchecked);

        if (ScopeWindows[currentScope]->scope()->getPPLabel(currentTrace))
		ppCheckBox->setCheckState(Qt::Checked);
	else
		ppCheckBox->setCheckState(Qt::Unchecked);

        if (ScopeWindows[currentScope]->scope()->getRMSLabel(currentTrace))
		rmsCheckBox->setCheckState(Qt::Checked);
	else
		rmsCheckBox->setCheckState(Qt::Unchecked);

        if (ScopeWindows[currentScope]->scope()->trace(currentTrace)->isVisible())
		showTraceCheckBox->setCheckState(Qt::Checked);
	else
		showTraceCheckBox->setCheckState(Qt::Unchecked);


}

void QRL_ScopesManager::changeTraceText(const QString & text ){
	if (currentTrace<traceItems.size())
	traceItems[currentTrace]->setText(text);
	//tabWidget->setTabText(1,traceItems[currentTrace]->text());
         ScopeWindows[currentScope]->scope()->trace(currentTrace)->setName(text);
	traceComboBox->setItemText(currentTrace,text);
	//traceComboBox->setCurrentIndex(	ScopeWindows[currentScope]->getTriggerChannel());
}

/**
* @brief update manager dialog for the choosen scope
* @param item scope number
*/




void QRL_ScopesManager::showScopeOptions( int index ){

	currentScope=index;
        if(Scopes[currentScope]->data2disk()->getIsSaving()==0){
		savePushButton->setEnabled(true);
	}
        tabWidget->setTabText(0,(Scopes[currentScope]->getName()));
	//fileLineEdit->setText(tr(Scopes[currentScope]->getName()));
        if(ScopeWindows[currentScope]->scope()->isVisible())
		showCheckBox->setCheckState(Qt::Checked);
	else
		showCheckBox->setCheckState(Qt::Unchecked);
        disconnect( rrCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeRefreshRate(double) ) );
        rrCounter->setValue(ScopeWindows[currentScope]->getRefreshRate());
        connect( rrCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeRefreshRate(double) ) );

        disconnect( dataCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDataPoints(double) ) );
        dataCounter->setValue(ScopeWindows[currentScope]->scope()->getDataPoints());
        connect( dataCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDataPoints(double) ) );

        disconnect( dxComboBox, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( changeDX(const QString&) ) );
        disconnect( dxComboBox, SIGNAL( editTextChanged(const QString &) ), this, SLOT( changeDX(const QString&) ) );
        dxComboBox->setEditText(tr("%1").arg(ScopeWindows[currentScope]->scope()->getDX()));
        connect( dxComboBox, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( changeDX(const QString&) ) );
        connect( dxComboBox, SIGNAL( editTextChanged(const QString &) ), this, SLOT( changeDX(const QString&) ) );

        disconnect( dividerCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDivider(double) ) );
        dividerCounter->setValue(ScopeWindows[currentScope]->scope()->getDivider());
        connect( dividerCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDivider(double) ) );

	currentTrace=0;
	for(int i=0; i<traceItems.size();i++)
		delete traceItems[i];
	traceItems.clear();
	//traceComboBox->clear();
	for(int i=0; i<Scopes[currentScope]->getNTraces();i++){
                traceItems << new QListWidgetItem(QIcon(),ScopeWindows[currentScope]->scope()->trace(i)->getName(), scopeListWidget);
		if (i<traceItems.size())
			traceItems[i]->setHidden(true);
		//traceComboBox->addItem(	ScopeWindows[currentScope]->getTraceName(i) );
                traceComboBox->setItemText(i,ScopeWindows[currentScope]->scope()->trace(i)->getName() );
	}
	
        traceComboBox->setCurrentIndex(	ScopeWindows[currentScope]->scope()->getTriggerChannel());
        triggerCounter->setValue(ScopeWindows[currentScope]->scope()->getTriggerLevel() );
        oneShotCheckBox->setChecked( ScopeWindows[currentScope]->scope()->getSingleMode() );
        timeCounter->setValue(Scopes[currentScope]->data2disk()->getSaveTime() );
	//fileLineEdit->setText(ScopeWindows[currentScope]->getFileName() );
        if (!(Scopes[currentScope]->data2disk()->getFileName()).contains(tr("/"))){
          fileLineEdit->setText((Scopes[currentScope]->data2disk()->getFileName()));
	  dirLineEdit->setText("./");
	}else {
          dirLineEdit->setText((Scopes[currentScope]->data2disk()->getFileName()).left((Scopes[currentScope]->data2disk()->getFileName()).lastIndexOf(tr("/"))+1));
          fileLineEdit->setText( (Scopes[currentScope]->data2disk()->getFileName()).mid((Scopes[currentScope]->data2disk()->getFileName()).lastIndexOf(tr("/"))+1));
	}
	
	disconnect( directionComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDirection(int) ) );
	disconnect( displayComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDisplayModus(int) ) );

        if (ScopeWindows[currentScope]->scope()->getPlottingDirection()==Qt::RightToLeft)
		directionComboBox->setCurrentIndex(0);
	else
		directionComboBox->setCurrentIndex(1);
	
        switch(ScopeWindows[currentScope]->scope()->getPlottingMode())
	{
		case 0:	
			displayComboBox->setCurrentIndex(0);
			break;
		case 1:
			displayComboBox->setCurrentIndex(1);
			break;
		case 2://trigger down
                        if (ScopeWindows[currentScope]->scope()->getTriggerUpDirection())
				displayComboBox->setCurrentIndex(3);
			else
				displayComboBox->setCurrentIndex(2);
			break;
// 		case 3:
// 			displayComboBox->setCurrentIndex(4);
// 			break;
		default:
			displayComboBox->setCurrentIndex(0);
			break;
	}

	connect( directionComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDirection(int) ) );
        connect( displayComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDisplayModus(int) ) );
	
        if(ScopeWindows[currentScope]->scope()->isPlotting())
		holdCheckBox->setCheckState(Qt::Unchecked);
	else
		holdCheckBox->setCheckState(Qt::Checked);
}

void QRL_ScopesManager::showOptions(QListWidgetItem * item )
{
	int index=scopeListWidget->row(item);
	if(tabWidget->currentIndex()==0){
		showScopeOptions(index);
	}else{
		showTraceOptions(index);
	}

}
void QRL_ScopesManager::showSelectedOptions()
{

}

/**
* @brief show scope windows
* @param state set display status
*/
void QRL_ScopesManager::showScope(int state) 
{
	if(state==Qt::Checked){
		ScopeWindows[currentScope]->show();
	} else {
		ScopeWindows[currentScope]->hide();
	}

}

void QRL_ScopesManager::showTrace(int state) 
{
	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->show(true);
	} else {
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->show(false);
	}

}

  void QRL_ScopesManager::changeTriggerLevel(double l) {
        ScopeWindows[currentScope]->scope()->setTriggerLevel(l);

}

  void QRL_ScopesManager::changeTriggerChannel(int trace) {
        ScopeWindows[currentScope]->scope()->setTriggerChannel(trace);

}
void QRL_ScopesManager::manualTrigger(){

        ScopeWindows[currentScope]->scope()->manualTriggerSignal();
}
void QRL_ScopesManager::startSingleRun(){

        ScopeWindows[currentScope]->scope()->startSingleRun();

}

void QRL_ScopesManager::changeSingleMode(int state){
	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->setSingleMode(true);
	} else {
                ScopeWindows[currentScope]->scope()->setSingleMode(false);
	}

}

void QRL_ScopesManager::changeDirection(int d)
{
	switch(d)
	{
	case 0:		
                ScopeWindows[currentScope]->scope()->setPlottingDirection(Qt::RightToLeft);
		break;
	case 1:
                ScopeWindows[currentScope]->scope()->setPlottingDirection(Qt::LeftToRight);
		break;
	default:
		break;
	}

}

void QRL_ScopesManager::changeDisplayModus(int mode)
{
	switch(mode)
	{
	case 0:	
                ScopeWindows[currentScope]->scope()->setPlottingMode(QPL_Scope::roll);
                ScopeWindows[currentScope]->scope()->setPlottingDirection(Qt::RightToLeft);
		directionComboBox->setCurrentIndex(0);
		break;
	case 1:
                ScopeWindows[currentScope]->scope()->setPlottingMode(QPL_Scope::overwrite);
                ScopeWindows[currentScope]->scope()->setPlottingDirection(Qt::LeftToRight);
		directionComboBox->setCurrentIndex(1);
		break;
	case 2://trigger down
                ScopeWindows[currentScope]->scope()->setPlottingMode(QPL_Scope::trigger);
                ScopeWindows[currentScope]->scope()->setPlottingDirection(Qt::LeftToRight);
		directionComboBox->setCurrentIndex(1);
                ScopeWindows[currentScope]->scope()->setTriggerUpDirection(false);
		break;
	case 3: // trigger up
                ScopeWindows[currentScope]->scope()->setPlottingMode(QPL_Scope::trigger);
                ScopeWindows[currentScope]->scope()->setPlottingDirection(Qt::LeftToRight);
                ScopeWindows[currentScope]->scope()->setTriggerUpDirection(true);
		directionComboBox->setCurrentIndex(1);
		break;
// 	case 4: //hold
// 		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::hold);
// 		break;
	default:
		break;
	}

}


void QRL_ScopesManager::holdPlot(int state) {

	if( state==Qt::Checked)	
                ScopeWindows[currentScope]->scope()->setPlotting(false);
	else
                ScopeWindows[currentScope]->scope()->setPlotting(true);


}

void QRL_ScopesManager::changeTraceColor()
{
        QColor color= QColorDialog::getColor(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getColor());
        ScopeWindows[currentScope]->scope()->trace(currentTrace)->setColor(color);
}

void QRL_ScopesManager::changeTraceWidth(double v)
{
	int traceWidth=(int)v;
        ScopeWindows[currentScope]->scope()->trace(currentTrace)->setWidth(traceWidth);
}

void QRL_ScopesManager::changeOffset(double offset)
{
        ScopeWindows[currentScope]->scope()->trace(currentTrace)->setOffset(offset);
	offsetWheel->setValue(offset);
	offsetCounter->setValue(offset);
	
}

void QRL_ScopesManager::setOffsetToMean()
{
        double offset = ScopeWindows[currentScope]->scope()->trace(currentTrace)->getAverage();

        offset=-(offset/ScopeWindows[currentScope]->scope()->trace(currentTrace)->getDy());
	changeOffset(offset);
}


void QRL_ScopesManager::fitDytoPP()
{
        double pp = ScopeWindows[currentScope]->scope()->trace(currentTrace)->getPP();

        double dy =pp/10;
        dyComboBox->setEditText(tr("%1").arg(dy));
}

void QRL_ScopesManager::changeSaveTime(double time)
{
        Scopes[currentScope]->data2disk()->setSaveTime(time);
	
}

void QRL_ScopesManager::changeFileName(const QString& str)
{
        Scopes[currentScope]->data2disk()->setFileName(dirLineEdit->text()+str);
}
void QRL_ScopesManager::changeFileDir(const QString& str)
{
        Scopes[currentScope]->data2disk()->setFileName(str+fileLineEdit->text());
}


void QRL_ScopesManager::changeDy(const QString& text)
{
	if (!text.isEmpty() &&text.toDouble()!=0.0 ){
		double dy=text.toDouble();
                double offset = ScopeWindows[currentScope]->scope()->trace(currentTrace)->getOffset();
                offset=(offset*ScopeWindows[currentScope]->scope()->trace(currentTrace)->getDy())/dy;
		changeOffset(offset);
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setDy(dy);
	}
}

  void QRL_ScopesManager::changeZeroAxis(int state){

	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setZeroAxis(true);
	} else {
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setZeroAxis(false);
	}

}

  void QRL_ScopesManager::changeTraceLabel(int state){

	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->setTraceLabel(true,currentTrace);
	} else {
                ScopeWindows[currentScope]->scope()->setTraceLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());

}

  void QRL_ScopesManager::changeUnitLabel(int state){

	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->setUnitLabel(true,currentTrace);
	} else {
                ScopeWindows[currentScope]->scope()->setUnitLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changeAverageLabel(int state){

	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->setAverageLabel(true,currentTrace);
	} else {
                ScopeWindows[currentScope]->scope()->setAverageLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changeMinLabel(int state){

	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->setMinLabel(true,currentTrace);
	} else {
                ScopeWindows[currentScope]->scope()->setMinLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changeMaxLabel(int state){

	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->setMaxLabel(true,currentTrace);
	} else {
                ScopeWindows[currentScope]->scope()->setMaxLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changePPLabel(int state){

	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->setPPLabel(true,currentTrace);
	} else {
                ScopeWindows[currentScope]->scope()->setPPLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changeRMSLabel(int state){

	if(state==Qt::Checked){
                ScopeWindows[currentScope]->scope()->setRMSLabel(true,currentTrace);
	} else {
                ScopeWindows[currentScope]->scope()->setRMSLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

   void QRL_ScopesManager::setLineStyle(int index){
        switch(index){
            case QwtPlotCurve::NoCurve: // NoCurve
                 ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::NoCurve);
                 break;
            case QwtPlotCurve::Lines: // Lines
                 ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::Lines);
                 break;
            case QwtPlotCurve::Sticks: // Sticks
                 ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::Sticks);
                 break;
            case QwtPlotCurve::Steps: // Steps
                 ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::Steps);
                 break;
            case QwtPlotCurve::Dots: // Dots
                 ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::Dots);
                 break;
        }
   }

      void QRL_ScopesManager::setSymbolStyle(int index){
        switch(index){
            case 0: // NoSymbol
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::NoSymbol);
                 break;
            case 1: // Ellipse
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Ellipse);
                 break;
            case 2: // Rect
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Rect);
                 break;
            case 3: // Diamond
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Diamond);
                 break;
            case 4: // Triangle
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Triangle);
                 break;
            case 5: // DTriangle
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::DTriangle);
                 break;
            case 6: // UTriangle
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::UTriangle);
                 break;
            case 7: // LTriangle
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::LTriangle);
                 break;
            case 8: // RTriangle
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::RTriangle);
                 break;
            case 9: // Cross
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Cross);
                 break;
            case 10: // XCross
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::XCross);
                 break;
             case 11: // HLine
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::HLine);
                 break;
             case 12: // VLine
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::VLine);
                 break;
             case 13: // Star1
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Star1);
                 break;
             case 14: // Star2
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Star2);
                 break;
             case 15: // Hexagon
                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Hexagon);
                 break;

        }
   }

        void QRL_ScopesManager::setSymbolSize(double size){

        ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolSize(size);

        }

void QRL_ScopesManager::changeSymbolPenColor()
{
        QColor color= QColorDialog::getColor(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getSymbolPenColor());
        ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolPenColor(color);
}

void QRL_ScopesManager::changeSymbolBrushColor()
{
        QColor color= QColorDialog::getColor(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getSymbolBrushColor());
        ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolBrushColor(color);
}




  void QRL_ScopesManager::setTraceStyle(int index){


switch(index){

    case 1: //line none
         ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::Lines);
         ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::NoSymbol);
    break;
    case 2: //line cross
         ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Cross);
          ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolPenColor(Qt::black);
         ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolSize(5);
            ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::Lines);
    break;
    case 3: //sticks ellipse
          ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::Ellipse);
            ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolPenColor(Qt::blue);
            ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolBrushColor(Qt::yellow);
            ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolSize(5);
           ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::Sticks);
    break;
    case 4: //steps none
        ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::Steps);
          ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::NoSymbol);
    break;
    case 5: //none xcross
                  ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolStyle(QwtSymbol::XCross);
            ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolPenColor(Qt::darkMagenta);
            ScopeWindows[currentScope]->scope()->trace(currentTrace)->setSymbolSize(5);
            ScopeWindows[currentScope]->scope()->trace(currentTrace)->setLineStyle(QwtPlotCurve::NoCurve);

    break;

}
if (index>0) {
 symbolSizeCounter->setValue(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getSymbolSize());
        lineStyleComboBox->setCurrentIndex(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getLineStyle());
        symbolStyleComboBox->setCurrentIndex(ScopeWindows[currentScope]->scope()->trace(currentTrace)->getSymbolStyle()+1);
        styleComboBox->setCurrentIndex(0);
    }
}



QDataStream& operator<<(QDataStream &out, const QRL_ScopesManager &d){
	out << d.size()  << d.pos() << d.isVisible();
	out <<(qint32) d.Num_Scopes;
	for (int i = 0; i < d.Num_Scopes; ++i) {
                out<<*(d.ScopeWindows[i]);
	}
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_ScopesManager(&d)){
	QSize s;QPoint p;bool b; 
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	qint32 a;
	in >> a;
	for (int i = 0; i < (int)a; ++i) {
		if (d.Num_Scopes>i){
                        d.ScopeWindows[i]->setFileVersion(d.fileVersion);
                        in>>*(d.ScopeWindows[i]);
		}
		else {
                        d.ScopeWindows[d.Num_Scopes-1]->setFileVersion(d.fileVersion);
                        in>>*(d.ScopeWindows[d.Num_Scopes-1]);
		}
	}
	d.showScopeOptions(d.currentScope);
	return in;
}





