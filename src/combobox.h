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

/* combobox.h */

#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <QWheelEvent>

class ComboBox : public QComboBox
{
    public:
        ComboBox(QWidget *parent = 0);

        ~ComboBox();

        void wheelEvent(QWheelEvent *e);
};

#endif // COMBOBOX_H
