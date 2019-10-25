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
   file for the classe QRL_LogsManager
*/

#include "logs_manager.h"


/**
* @brief Initialize Logs Manager
*/
QRL_LogsManager::QRL_LogsManager(QWidget *parent,int numLogs, QRL_LogData **logs, int verb)
        :QDialog(parent),Num_Logs(numLogs),Logs(logs),verbose(verb)
{
	setupUi(this);
        //Num_Logs=qTargetInterface->getLogNumber();
        //Logs=qTargetInterface->getLogs();
	const QIcon LogIcon =QIcon(QString::fromUtf8(":/icons/log_icon.xpm"));
	LogWindows = new QRL_LogWindow* [Num_Logs]; 
	for (int i=0; i<Num_Logs; ++i){
                //logItems << new QListWidgetItem(LogIcon,qTargetInterface->getLogName(i), logListWidget);
                logItems << new QListWidgetItem(LogIcon,tr("%1").arg(i), logListWidget);
                LogWindows[i]=new QRL_LogWindow(parent,Logs[i]);
	}
	//connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showScope(int) ) );
	connect( logListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showLogOptions( QListWidgetItem *  ) ) );
	connect( logListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showLogOptions( QListWidgetItem *  ) ) );
	//connect( rrLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeRefreshRate(const QString&) ) );
	connect( savePushButton, SIGNAL( pressed() ), this, SLOT( startSaving() ) );
	connect( stopPushButton, SIGNAL( pressed() ), this, SLOT( stopSaving() ) );
	connect( fileLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeFileName(const QString&) ) );
	connect( dirPushButton, SIGNAL( pressed()), this, SLOT(setFileDirectory() ) );

	connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showLog(int) ) );
	connect( holdCheckBox , SIGNAL( stateChanged(int) ), this, SLOT( holdPlot(int) ) );

        connect(minScaleCounter, SIGNAL(valueChanged(double)), this, SLOT(setMinScale(double)));
        connect(maxScaleCounter, SIGNAL(valueChanged(double)), this, SLOT(setMaxScale(double)));
        connect( delegateComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDelegate(int) ) );
        connect( pixelSizeSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( setPixelSize(int) ) );

        connect( viewNumberCheckBox , SIGNAL( stateChanged(int) ), this, SLOT( setShowItemNumber(int) ) );
        connect(rrCounter, SIGNAL(valueChanged(double)), this, SLOT(changeRefreshRate(double)));

        connect(viewComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeLogView(int) ) );
        connect(pointDistCounter, SIGNAL(valueChanged(double)), this, SLOT(changeHistDistance(double)));
        connect(histLengthDistCounter, SIGNAL(valueChanged(double)), this, SLOT(changeHistLength(double)));
        connect( dxComboBox, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( changeDx(const QString&) ) );
        connect( dxComboBox, SIGNAL( editTextChanged(const QString &) ), this, SLOT( changeDx(const QString&) ) );
        connect( dyComboBox, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( changeDy(const QString&) ) );
        connect( dyComboBox, SIGNAL( editTextChanged(const QString &) ), this, SLOT( changeDy(const QString&) ) );
        connect(xOffsetCounter, SIGNAL(valueChanged(double)), this, SLOT(changeXOffset(double)));
        connect(yOffsetCounter, SIGNAL(valueChanged(double)), this, SLOT(changeYOffset(double)));

	currentLog=0;
	holdCheckBox->setCheckState(Qt::Checked);
	LogWindows[currentLog]->setPlotting(false);
        rowDimLineEdit->setText(tr("%1").arg(Logs[currentLog]->getNRow()));
        colDimLineEdit->setText(tr("%1").arg(Logs[currentLog]->getNCol()));

     model = new MatrixModel(this);
     legendTableView->setShowGrid(false);
     legendTableView->horizontalHeader()->hide();
     legendTableView->verticalHeader()->hide();
     legendTableView->horizontalHeader()->setMinimumSectionSize(1);
     legendTableView->verticalHeader()->setMinimumSectionSize(1);
     legendTableView->setModel(model);
     pixelView = new PixelDelegate(this);
     blackwhiteView = new BlackWhiteDelegate(this);
     colorView= new ColorBarDelegate(this);
     legendTableView->setItemDelegate(colorView);


      v.resize(9);

      for (int t=0; t<9; t++){
                      v[t].resize(1);
        }
      for (int t=0; t<9; t++){
         v[8-t][0]=(float)t/8.*(LogWindows[currentLog]->matrixplot()->getMaxScale()-LogWindows[currentLog]->matrixplot()->getMinScale())+LogWindows[currentLog]->matrixplot()->getMinScale();
     }
         colorView->setMinScale(LogWindows[currentLog]->matrixplot()->getMinScale());
         colorView->setMaxScale(LogWindows[currentLog]->matrixplot()->getMaxScale());
         colorView->setPixelSize(20);
         colorView->setShowValue(true);
         pixelView->setMinScale(LogWindows[currentLog]->matrixplot()->getMinScale());
         pixelView->setMaxScale(LogWindows[currentLog]->matrixplot()->getMaxScale());
         pixelView->setPixelSize(20);
                 pixelView->setShowValue(true);
         blackwhiteView->setMinScale(LogWindows[currentLog]->matrixplot()->getMinScale());
         blackwhiteView->setPixelSize(20);
                 blackwhiteView->setShowValue(true);
         model->setData(v);
    legendTableView->resizeColumnsToContents();
    legendTableView->resizeRowsToContents();
    changeDelegate(0);

// 	for(int i=0; i<1; ++i){
// 		//tabWidget->addTab(new QWidget(tabWidget->widget(1)),tr("Trace ")+tr("%1").arg(i+1));
// 		traceComboBox->addItem(tr("Trace ")+tr("%1").arg(i+1));
// 	}
	if (Num_Logs > 0)  showLogOptions(currentLog);
	
//	if (Num_Scopes > 0) Get_Scope_Data_Thread = new GetScopeDataThread [Num_Scopes];
	//for (int i=0; i<Num_Scopes; ++i){
	//	connect(&Get_Scope_Data_Thread[i],SIGNAL(value(int,float)),ScopeWindows[i],SLOT(setValue(int,float)));
	//}
	
//	tabWidget->addTab(traceWidget,tr("trace %1").arg(Scopes[currentScope].ntraces));

        MatrixViewOptions = tabWidget->widget(1);
        XYPlotOptions = tabWidget->widget(2);


       switch(LogWindows[currentLog]->getLogType())
        {
                case QRL_LogWindow::MATRIXVIEW:
                     tabWidget->removeTab(2);


                        break;
                case QRL_LogWindow::XYPLOT:
                        tabWidget->removeTab(1);

                        break;
                default:break;
        }
        tabWidget->setCurrentIndex(0);



	timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        //RefreshRate=LogWindows[currentLog]->getRefreshRate();
        timer->start((int)(1./5.*1000.));

}
QRL_LogsManager::~QRL_LogsManager()
{
        for (int i=0; i<Num_Logs; ++i){
                LogWindows[i]->hide();
                delete LogWindows[i];

        }
        delete[] LogWindows;
	//stopScopeThreads();
	//if (Get_Scope_Data_Thread)
	//	delete[] Get_Scope_Data_Thread;
}

void QRL_LogsManager::setFileVersion(qint32 v){
      fileVersion=v;

}
void  QRL_LogsManager::setLogName(int i,QString name){

    if (i<logItems.size())
    logItems[i]->setText(name);
    showLogOptions(currentLog);

}
void QRL_LogsManager::refresh()
{
      if (!LogWindows[currentLog]->isVisible() && showCheckBox->isChecked())
      showCheckBox->setCheckState(Qt::Unchecked);

  if (Logs[currentLog]->data2disk()->getIsSaving()==0){
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
    saveProgressBar->setMaximum(Logs[currentLog]->data2disk()->n_points_to_save());
     saveProgressBar->setValue(Logs[currentLog]->data2disk()->getSavedPoints());
  }

//  for (int n=0; n<Num_Logs; ++n){
//      if (Logs[n]->isPlotting()){
//          if (LogWindows[n]->getLogType()==QRL_LogWindow::MATRIXVIEW)
//            LogWindows[n]->matrixplot()->setValue( Logs[n]->getLogValue());
//          else if (LogWindows[n]->getLogType()==QRL_LogWindow::XYPLOT)
//              LogWindows[n]->xyplot()->setValue( Logs[n]->getLogValueHist());
//     }
//  }
}

void QRL_LogsManager::changeDx(const QString& text)
{
        if (!text.isEmpty() &&text.toDouble()!=0.0 ){
                double dx=text.toDouble();
                for (int i=0;i<LogWindows[currentLog]->xyplot()->getNCurve();i++){
                    //double xoffset = LogWindows[currentLog]->xyplot()->
                    LogWindows[currentLog]->xyplot()->trace(i)->setDx(dx);

                }
//                double xoffset = ScopeWindows[currentScope]->scope()->trace(currentTrace)->getOffset();
//                xoffset=(offset*ScopeWindows[currentScope]->scope()->trace(currentTrace)->getDx())/dx;
//                changeXOffset(xoffset);
//                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setDy(dx);
        }
}

void QRL_LogsManager::changeDy(const QString& text)
{
        if (!text.isEmpty() &&text.toDouble()!=0.0 ){
                double dy=text.toDouble();
                for (int i=0;i<LogWindows[currentLog]->xyplot()->getNCurve();i++){
                    //double xoffset = LogWindows[currentLog]->xyplot()->
                    LogWindows[currentLog]->xyplot()->trace(i)->setDy(dy);

                }
//                double yoffset = LogWindows[currentLog]->xyplot()->trace(currentTrace)->getOffset();
//                yoffset=(offset*ScopeWindows[currentScope]->scope()->trace(currentTrace)->getDy())/dy;
//                changeYOffset(yoffset);
//                ScopeWindows[currentScope]->scope()->trace(currentTrace)->setDy(dy);
        }
}

void QRL_LogsManager::changeXOffset(double o)
{
  for (int i=0;i<LogWindows[currentLog]->xyplot()->getNCurve();i++){
    LogWindows[currentLog]->xyplot()->trace(i)->setXOffset(o);
  }



}
void QRL_LogsManager::changeYOffset(double o)
{

  for (int i=0;i<LogWindows[currentLog]->xyplot()->getNCurve();i++){
    LogWindows[currentLog]->xyplot()->trace(i)->setYOffset(o);
  }


}

void QRL_LogsManager::showLog(int state) 
{
	if(state==Qt::Checked){
		LogWindows[currentLog]->show();
	} else {
		LogWindows[currentLog]->hide();
	}

}

void QRL_LogsManager::changeLogView(int type) {

        switch(type)
        {
                case 0: //matrixview
                        tabWidget->removeTab(1);
                        tabWidget->addTab(MatrixViewOptions,tr("Matrix View Options"));
                        if (LogWindows[currentLog]->getLogType()!=QRL_LogWindow::MATRIXVIEW)
                                LogWindows[currentLog]->setLog(QRL_LogWindow::MATRIXVIEW);
                          Logs[currentLog]->setHistory(false);
                        break;
                case 1: //xyplot
                        tabWidget->removeTab(1);
                        tabWidget->addTab(XYPlotOptions,tr("X-Y Plot Options"));
                        //tabWidget->addTab(xyPlot,tr("X-Y Plot Options"));
                        if (LogWindows[currentLog]->getLogType()!=QRL_LogWindow::XYPLOT)
                                LogWindows[currentLog]->setLog(QRL_LogWindow::XYPLOT);
                          Logs[currentLog]->setHistory(true);
                        break;

                default:break;
        }
        //}
        showLogOptions(currentLog);



}
void QRL_LogsManager::setMinScale(double min) {
    if (min<=LogWindows[currentLog]->matrixplot()->getMaxScale()){
        LogWindows[currentLog]->matrixplot()->setMinScale(min);
             colorView->setMinScale(min);
         pixelView->setMinScale(min);
         blackwhiteView->setMinScale(min);
       for (int t=0; t<9; t++)
         v[8-t][0]=(float)t/8.*(LogWindows[currentLog]->matrixplot()->getMaxScale()-LogWindows[currentLog]->matrixplot()->getMinScale())+LogWindows[currentLog]->matrixplot()->getMinScale();

         model->setData(v);
     }else
        minScaleCounter->setValue(LogWindows[currentLog]->matrixplot()->getMinScale());


}
void QRL_LogsManager::setMaxScale(double max) {

    if (max>=LogWindows[currentLog]->matrixplot()->getMinScale()){
        LogWindows[currentLog]->matrixplot()->setMaxScale(max);
                 colorView->setMaxScale(max);
         pixelView->setMaxScale(max);
      for (int t=0; t<9; t++)
         v[8-t][0]=(float)t/8.*(LogWindows[currentLog]->matrixplot()->getMaxScale()-LogWindows[currentLog]->matrixplot()->getMinScale())+LogWindows[currentLog]->matrixplot()->getMinScale();

         model->setData(v);
     } else
        maxScaleCounter->setValue(LogWindows[currentLog]->matrixplot()->getMaxScale());



}
void QRL_LogsManager::setPixelSize(int psize) {


     LogWindows[currentLog]->matrixplot()->setPixelSize(psize);

}
void QRL_LogsManager::changeDelegate(int d)
{
        switch(d)
        {
        case 0:
                LogWindows[currentLog]->setDelegate(QRL_LogWindow::colorbar);
                 legendTableView->setItemDelegate(colorView);
                break;
        case 1:
                LogWindows[currentLog]->setDelegate(QRL_LogWindow::blackwhite);
                 legendTableView->setItemDelegate(blackwhiteView);
                break;
         case 2:
                LogWindows[currentLog]->setDelegate(QRL_LogWindow::pixel);
                 legendTableView->setItemDelegate(pixelView);
                break;
         case 3:
                LogWindows[currentLog]->setDelegate(QRL_LogWindow::text);
                break;
        default:
                break;
        }
                 model->setData(v);

}
void QRL_LogsManager::setShowItemNumber(int state)
{
        if(state==Qt::Checked){
                LogWindows[currentLog]->matrixplot()->setShowItemNumber(true);
        } else {
                LogWindows[currentLog]->matrixplot()->setShowItemNumber(false);
        }

}

void QRL_LogsManager::holdPlot(int state) {

	if( state==Qt::Checked)	
		LogWindows[currentLog]->setPlotting(false);
	else
		LogWindows[currentLog]->setPlotting(true);


}

void QRL_LogsManager::changeRefreshRate(double rr)
{

        //double rr=text.toDouble();
        LogWindows[currentLog]->changeRefreshRate(rr);
        Logs[currentLog]->setLogRefreshRate(rr);


}

void QRL_LogsManager::changeHistDistance(double d) {


 Logs[currentLog]->setHistDistance(d);


}
   void QRL_LogsManager::changeHistLength(double d){


    LogWindows[currentLog]->xyplot()->changeDataPoints(d);

   }

void QRL_LogsManager::setFileDirectory(){

 QString dir = QFileDialog::getExistingDirectory(this, tr("File Directory"),
                                                 dirLineEdit->text(),
                                                 QFileDialog::ShowDirsOnly);
  dirLineEdit->setText(dir);


}
  void QRL_LogsManager::setSaveTime(int logNumber, double savetime, bool autosave){

if (logNumber<Num_Logs){
     Logs[logNumber]->data2disk()->setSaveTime(savetime);
     if (autosave){
                if (QFile::exists(Logs[logNumber]->data2disk()->getFileName())) {
                        printf("File %s exists already.\n",Logs[logNumber]->data2disk()->getFileName().toLocal8Bit().data() );
                        QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("The File exists! Please change the name!"),
                                     QMessageBox::Abort);
                } else {
                //if ((Save_File_Pointer = fopen((File_Name.toLocal8Bit()).data(), "a+")) == NULL) {
                  if (!Logs[logNumber]->data2disk()->startSaving()) {
                        printf("Error in opening file %s\n",Logs[logNumber]->data2disk()->getFileName().toLocal8Bit().data() );
                        QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("Error in opening file!"),
                                     QMessageBox::Abort);
                }

     }
 }
 }
if (logNumber==currentLog){
timeCounter->setValue(savetime);

}



  }
 void QRL_LogsManager::setFileName(int logNumber, const QString& filename){

     if (logNumber<Num_Logs){

         Logs[logNumber]->data2disk()->setFileName(filename);
     }

     if (logNumber==currentLog){

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


void QRL_LogsManager::startSaving()
{
         //FILE* Save_File_Pointer;
	double Save_Time=timeCounter->value();
        if( Logs[currentLog]->data2disk()->getIsSaving()==0){

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
                  if (!Logs[currentLog]->data2disk()->startSaving(File_Name.toLocal8Bit().data(),Save_Time)) {
			printf("Error in opening file %s",File_Name.toLocal8Bit().data() );
			QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("Error in opening file!"),
                                     QMessageBox::Abort);
                }
	       }
       }
}

void QRL_LogsManager::stopSaving()
{
   if (Logs[currentLog]->data2disk()->getIsSaving()==1){
          Logs[currentLog]->data2disk()->stopSaving();
  }
}



void QRL_LogsManager::showLogOptions(QListWidgetItem * item )
{
        int index=logListWidget->row(item);
        showLogOptions(index);


}

void QRL_LogsManager::showLogOptions( int index ){

	currentLog=index;
        if(Logs[currentLog]->data2disk()->getIsSaving()==0){
		savePushButton->setEnabled(true);
	}
	tabWidget->setTabText(0,tr(Logs[currentLog]->getName()));
        rowDimLineEdit->setText(tr("%1").arg(Logs[currentLog]->getNRow()));
        colDimLineEdit->setText(tr("%1").arg(Logs[currentLog]->getNCol()));
        showCheckBox->setChecked(LogWindows[currentLog]->isVisible());
        holdCheckBox->setChecked(!LogWindows[currentLog]->isPlotting());
        minScaleCounter->setValue(LogWindows[currentLog]->matrixplot()->getMinScale());
         maxScaleCounter->setValue(LogWindows[currentLog]->matrixplot()->getMaxScale());
         rrCounter->setValue(LogWindows[currentLog]->getRefreshRate());
         viewComboBox->setCurrentIndex((int)LogWindows[currentLog]->getLogType());
         histLengthDistCounter->setValue(LogWindows[currentLog]->xyplot()->getDataPoints());
         pointDistCounter->setValue(Logs[currentLog]->getHistDistance());
         if (LogWindows[currentLog]->xyplot()->getNCurve()>0){
             xOffsetCounter->setValue(LogWindows[currentLog]->xyplot()->trace(0)->getXOffset());
             yOffsetCounter->setValue(LogWindows[currentLog]->xyplot()->trace(0)->getYOffset());
             dxComboBox->setEditText(tr("%1").arg(LogWindows[currentLog]->xyplot()->trace(0)->getDx()));
             dyComboBox->setEditText(tr("%1").arg(LogWindows[currentLog]->xyplot()->trace(0)->getDy()));
        }
         switch(LogWindows[currentLog]->getDelegate()){
             case QRL_LogWindow::pixel:
                    delegateComboBox->setCurrentIndex(2);
                    break;
             case QRL_LogWindow::blackwhite:
                    delegateComboBox->setCurrentIndex(1);
                    break;
             case QRL_LogWindow::colorbar:
                    delegateComboBox->setCurrentIndex(0);
                    break;

         }
         pixelSizeSpinBox->setValue(LogWindows[currentLog]->matrixplot()->getPixelSize());
         viewNumberCheckBox->setChecked(LogWindows[currentLog]->matrixplot()->getShowItemNumber());
//  	timeCounter->setValue(saveTime[currentLog] );
//  	fileLineEdit->setText(fileName[currentLog] );
         timeCounter->setValue(Logs[currentLog]->data2disk()->getSaveTime() );
        //fileLineEdit->setText(ScopeWindows[currentScope]->getFileName() );
        if (!(Logs[currentLog]->data2disk()->getFileName()).contains(tr("/"))){
          fileLineEdit->setText((Logs[currentLog]->data2disk()->getFileName()));
          dirLineEdit->setText("./");
        }else {
          dirLineEdit->setText((Logs[currentLog]->data2disk()->getFileName()).left((Logs[currentLog]->data2disk()->getFileName()).lastIndexOf(tr("/"))+1));
          fileLineEdit->setText( (Logs[currentLog]->data2disk()->getFileName()).mid((Logs[currentLog]->data2disk()->getFileName()).lastIndexOf(tr("/"))+1));
        }

}



void QRL_LogsManager::changeSaveTime(double time)
{
	//ScopeWindows[currentScope]->setSaveTime(time);
// 	Logs[currentLog].Save_Time=time;
// 	saveTime[currentLog]=time;
	
}

void QRL_LogsManager::changeFileName(const QString& str)
{
	//ScopeWindows[currentScope]->setFileName(str);
// 	fileName[currentLog]=str;
}


QDataStream& operator<<(QDataStream &out, const QRL_LogsManager &d){
	out << d.size()  << d.pos() << d.isVisible();
	out <<(qint32) d.Num_Logs;
        for (int i = 0; i < d.Num_Logs; ++i) {
                out<<*(d.LogWindows[i]);
        }
//	out << d.dirLineEdit->text() << d.fileLineEdit->text() ;
//	out << d.timeCounter->value();
//	for (int i = 0; i < d.Num_Logs; ++i) {
//		//out << d.file_name.at(i) << d.save_time.at(i);
//	}
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_LogsManager(&d)){
        QSize s;QPoint p;bool b; int i;QString str;double dd;qint32 a;
        in >> s;d.resize(s);
        in >> p; d.move(p);
        in >> b; d.setVisible(b);
        in >> a;
        if (d.fileVersion<107) {
            if (d.fileVersion>104){
              in >> str;
              d.dirLineEdit->setText(str);
            }
            in >> str;
            d.fileLineEdit->setText(str );
            in >> dd;
            d.timeCounter->setValue(dd);
            for (int i = 0; i < (int)a; ++i) {
    // 		if (d.Num_Logs>i)
    // 			in>>d.file_name[i]>>d.save_time[i];
    // 		else
    // 			in>>str>>dd;
            }
            d.showLogOptions(d.currentLog);
        } else{

            for (int i = 0; i < (int)a; ++i) {
                if (d.Num_Logs>i){
                        d.LogWindows[i]->setFileVersion(d.fileVersion);
                        in>>*(d.LogWindows[i]);
                }
                else {
                        d.LogWindows[d.Num_Logs-1]->setFileVersion(d.fileVersion);
                        in>>*(d.LogWindows[d.Num_Logs-1]);
                }
            }
            d.showLogOptions(d.currentLog);
        }
	return in;
}





