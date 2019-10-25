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
 file:		targets_manager.h
 describtion:
   file for the class  QRL_TargetsManager
*/

#ifndef _TARGETS_MANAGER_H
#define _TARGETS_MANAGER_H 1


#include "ui_qrl_targets_manager.h"
#include "qrtailab_core.h"


/**
 * @brief Managed all targets
 */
class QRL_TargetsManager : public QDialog, private Ui::QRL_TargetsManager
{
   Q_OBJECT
public:
  QRL_TargetsManager(QWidget *parent = 0, QRtaiLabCore* qtargetinterface=NULL);
  ~QRL_TargetsManager();
  void setTargetIsConnected(bool);
  void setTargetIsRunning(bool);
    void setPreferences(Preferences_T p);
    Preferences_T getPreferences();
    void setFileVersion(qint32 v){fileVersion=v;}
signals:
    void startTarget();
    void stopTarget();
    void connectToTarget();
    void disconnectFromTarget();
public slots:
protected slots:
    void start();
    void stop();
    void connectTarget();
    void disconnectTarget();
    void hrtModusChanged(int);
private:
  qint32 fileVersion;
    Preferences_T Preferences;
  QRtaiLabCore* qTargetInterface;
  int currentTarget;
  QList<QListWidgetItem *> targetItems;
friend QDataStream& operator<<(QDataStream &out, const QRL_TargetsManager &d);
friend QDataStream& operator>>(QDataStream &in, QRL_TargetsManager(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_TargetsManager &d);
	QDataStream& operator>>(QDataStream &in, QRL_TargetsManager(&d));



#endif
