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
 file:		scopes_manager.h
 describtion:
   file for the class  QRL_ScopesManager
*/

#ifndef _SCOPES_MANAGER_H
#define _SCOPES_MANAGER_H 1


#include "ui_qrl_scopes_manager.h"
#include "qrtailab_core.h"
#include "scope_window.h"






/**
 * @brief Managed all Scope windows
 * @todo add own color dialog
 */
class QRL_ScopesManager : public QDialog, private Ui::QRL_ScopesManager
{
   Q_OBJECT
public:
  QRL_ScopesManager(QWidget *parent = 0, int numScopes=0, QPL_ScopeData **scopes = 0, int v=0);
  ~QRL_ScopesManager();
  //void startScopeThreads();
  //void stopScopeThreads();
  QRL_ScopeWindow** getScopeWindows(){return ScopeWindows;}
    void setFileVersion(qint32 v);
    void startRefresh(){   timer->start((int)(1./5.*1000.)); for (int i = 0; i < Num_Scopes; ++i)ScopeWindows[i]->startRefresh(); }
    void stopRefresh(){timer->stop(); for (int i = 0; i < Num_Scopes; ++i)ScopeWindows[i]->stopRefresh(); }
    void setScopeName(int i,QString name);
public slots:
  void setFileName(int scopeNumber, const QString& filename);
  void setSaveTime(int scopeNumber, double savetime, bool autosave);
   void refresh();
  void  showScope(int);
  void  showTrace(int);
  void showScopeOptions( int );
  void showSelectedOptions();
  void showOptions( QListWidgetItem * item );
  void changeRefreshRate(double);
  void changeDataPoints(double);
  void changeDivider(double);
 // void changeSaveTime(double);
  void changeDX(const QString&);
  void startSaving();
  void stopSaving();
  void setOptions(int);
  void changeTraceColor();
  void changeTraceWidth(double);
  void changeOffset(double);
  void changeDy(const QString&);
  void changeDisplayModus(int);
  void changeDirection(int);
  void showTraceOptions(int);
  void changeScopeList(int);
  void changeSaveTime(double);
  void changeFileName(const QString&);
  void changeFileDir(const QString&);
  void changeTriggerLevel(double);
  void changeTriggerChannel(int);
  void manualTrigger();
  void changeSingleMode(int);
  void startSingleRun();
  void changeTraceText(const QString & text );
  void changeZeroAxis(int);
  void changeTraceLabel(int);
    void changeUnitLabel(int);
    void changeAverageLabel(int);
    void changeMinLabel(int);
    void changeMaxLabel(int);
    void changePPLabel(int);
    void changeRMSLabel(int);
    void holdPlot(int);
    void setFileDirectory();
  void setOffsetToMean();
    void fitDytoPP();
    void setTraceStyle(int);
    void setLineStyle(int);
    void setSymbolStyle(int index);
      void setSymbolSize(double size);
      void changeSymbolBrushColor();
      void changeSymbolPenColor();

private:
  qint32 fileVersion;
  int Num_Scopes;
  QPL_ScopeData **Scopes;
  int verbose;
  unsigned int currentScope;
 // GetScopeDataThread* Get_Scope_Data_Thread;
  //QRtaiLabCore* qTargetInterface;
  //TargetThread* targetThread;
  QRL_ScopeWindow** ScopeWindows;
  int currentTrace;
  QList<QListWidgetItem *> scopeItems,traceItems;
  QTimer *timer;
  //double RefreshRate;
friend QDataStream& operator<<(QDataStream &out, const QRL_ScopesManager &d);
friend QDataStream& operator>>(QDataStream &in, QRL_ScopesManager(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_ScopesManager &d);
	QDataStream& operator>>(QDataStream &in, QRL_ScopesManager(&d));


#endif
