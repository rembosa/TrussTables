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

/* scrollarea.h */

#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>

class ScrollArea : public QScrollArea
{
        Q_OBJECT

    public:
        ScrollArea(QWidget *parent = 0);

        ~ScrollArea();

        QSize sizeHint() const;

        void setWidgetSize();

    public slots:
        void setFactors(int widthFactor,
                        int heightFactor,
                        int margin);

    signals:
        void sizeSignal(int width,
                        int height,
                        int margin);

    protected:
        void resizeEvent(QResizeEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *event);

    private:
        int     mWidthFactor;
        int     mHeightFactor;
        int     mMargin;
        QPointF mInitialPosition;
};

#endif // SCROLLAREA_H
