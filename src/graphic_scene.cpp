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

#include "graphic_scene.h"




#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


QRL_OpenGLScene::QRL_OpenGLScene(QGraphicsView* v)
//:m_backgroundColor(0, 170, 255)
  :m_backgroundColor(Qt::gray),view(v)
{



//    QWidget *qwt = createDialog(tr("qwt"));
//
//    //DataPlot *plot = new DataPlot();
//    qwt->layout()->addWidget(plot);
//   // plot->setTimerInterval(20);
//
//  QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog);
//  proxy->setWidget(qwt);
//  addItem(proxy);
//
//
//
//    QPointF pos(10, 10);
//    foreach (QGraphicsItem *item, items()) {
//        item->setFlag(QGraphicsItem::ItemIsMovable);
//        //item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
//        item->setCacheMode(QGraphicsItem::ItemCoordinateCache);
//
//        const QRectF rect = item->boundingRect();
//        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
//        pos += QPointF(0, 10 + rect.height());
//    }
//    m_time.start();


      scopeTimer = new QTimer(this);
         connect(scopeTimer, SIGNAL(timeout()), this, SLOT(update()));
         #ifdef _use_opengl_
          scopeTimer->start((int)(1./20*1000.));
#else
           //scopeTimer->start((int)(1./20*1000.));
         #endif
}

  void QRL_OpenGLScene::addDialog(QDialog * newDialog){

QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog);
  proxy->setWidget(newDialog);
  addItem(proxy);

    QPointF pos(10, 10);
    foreach (QGraphicsItem *item, items()) {
        item->setFlag(QGraphicsItem::ItemIsMovable);
        //item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        item->setCacheMode(QGraphicsItem::ItemCoordinateCache);

//        const QRectF rect = item->boundingRect();
//        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
//        pos += QPointF(0, 10 + rect.height());
    }
    m_time.start();


  }

void QRL_OpenGLScene::drawBackground(QPainter *painter, const QRectF &)
{
    #ifdef _use_opengl_
    if (painter->paintEngine()->type() != QPaintEngine::OpenGL)
    //    && painter->paintEngine()->type() != QPaintEngine::OpenGL2)
    {
        qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        return;
    }
#endif
    //painter->beginNativePainting();
#ifdef _use_opengl_
    glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
   // painter->endNativePainting();

   // QTimer::singleShot(20, this, SLOT(update()));
}


void QRL_OpenGLScene::setBackgroundColor()
{
    const QColor color = QColorDialog::getColor(m_backgroundColor);
    if (color.isValid()) {
        m_backgroundColor = color;
        update();
    }
}


void QRL_OpenGLScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted())
        return;
//    if (event->buttons() & Qt::LeftButton) {
//        QRectF rect = this->itemsBoundingRect();
//        if (rect.isNull())
//        {
//            //this->setSceneRect(QRectF(0, 0, 1, 1));
//        }else{
//            this->setSceneRect(rect);
//        }
//    }
    event->accept();
    update();
}

void QRL_OpenGLScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted())
        return;


    event->accept();
}

void QRL_OpenGLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;
 //if (event->buttons() & Qt::LeftButton) {
//        QRectF rect = this->itemsBoundingRect();
//        QRectF arect= this ->sceneRect();
//      if (view->verticalScrollBar()->isVisible() || view->horizontalScrollBar()->isVisible()) {
//        //QRectF arect= this ->sceneRect();
//        //arect.setWidth(rect.width());
//        //arect.setHeight(rect.height());
//        if (rect.isNull())
//        {
//           // this->setSceneRect(QRectF(0, 0, 1, 1));
//        }else{
//            this->setSceneRect(rect);
//                //update();
//        }
//     }






    event->accept();
    //update();
}

void QRL_OpenGLScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
    if (event->isAccepted())
        return;

    event->accept();
    update();
}

