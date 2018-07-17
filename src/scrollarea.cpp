/********************************************************************************************
 * This file is part of TrussTables
 * Copyright 2018, Ambrose Louis Okune <sambero.osilu@gmail.com>
 *
 * TrussTables is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Public License as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * TrussTables is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with TrussTables.
 * If not, see <http://www.gnu.org/licenses/>.
 ********************************************************************************************/

/* scrollarea.cpp */

#include "scrollarea.h"

ScrollArea::ScrollArea(QWidget *parent) : QScrollArea(parent)
{
    mWidthFactor  = 1;
    mHeightFactor = 1;
    mMargin       = 75;

    mInitialPosition.setX(0.0);
    mInitialPosition.setY(0.0);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

ScrollArea::~ScrollArea()
{

}

QSize ScrollArea::sizeHint() const
{
    return QSize(450, 450);
}

void ScrollArea::setWidgetSize()
{
    int offset = 20;

    int w = mWidthFactor * width() - offset;
    int h = mHeightFactor * height() - offset;

    emit sizeSignal(w,
                    h,
                    mMargin);
    ensureVisible(0, widget()->height());
}

void ScrollArea::setFactors(int widthFactor,
                            int heightFactor,
                            int margin)
{
    mWidthFactor  = widthFactor;
    mHeightFactor = heightFactor;
    mMargin       = margin;

    setWidgetSize();
}

void ScrollArea::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);
    setWidgetSize();
}

void ScrollArea::mousePressEvent(QMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);

    mInitialPosition.setX(event->globalX());
    mInitialPosition.setY(event->globalY());
}

void ScrollArea::mouseReleaseEvent(QMouseEvent *)
{
    unsetCursor();
}

void ScrollArea::mouseMoveEvent(QMouseEvent *event)
{
    QPointF finalPosition;

    finalPosition.setX(event->globalX());
    finalPosition.setY(event->globalY());

    qreal deltaX = finalPosition.x() - mInitialPosition.x();
    qreal deltaY = finalPosition.y() - mInitialPosition.y();

    mInitialPosition.setX(event->globalX());
    mInitialPosition.setY(event->globalY());

    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - deltaX);
    verticalScrollBar()->setValue(verticalScrollBar()->value() - deltaY);
}
