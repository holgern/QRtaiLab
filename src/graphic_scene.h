/***************************************************************************
 *   Copyright (C) 2010 by Holger Nahrstaedt                               *
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

#ifndef GRAPHIC_SCENE_H
#define GRAPHIC_SCENE_H

#include <QtGui>
#ifdef _use_opengl_
#include <QtOpenGL>
#endif
#ifndef QT_NO_CONCURRENT
#include <QFutureWatcher>
#endif

class Model;
class QRL_OpenGLScene : public QGraphicsScene
{
    Q_OBJECT

public:
    QRL_OpenGLScene(QGraphicsView* v);

    void drawBackground(QPainter *painter, const QRectF &rect);
    void addDialog(QDialog *);
public slots:
    void setBackgroundColor();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent * wheelEvent);

private:
   //QDialog *createDialog(const QString &windowTitle) const;

    QColor m_backgroundColor;
    QTime m_time;
    int m_lastTime;
    int m_mouseEventTime;

   QTimer *scopeTimer;
QGraphicsView* view;

#ifndef QT_NO_CONCURRENT
    QFutureWatcher<Model *> m_modelLoader;
#endif
};


#endif // GRAPHIC_SCENE_H
