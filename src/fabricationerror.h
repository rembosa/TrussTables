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

/* FabricationError.h */

#ifndef FABRICATIONERROR_H
#define FABRICATIONERROR_H

#include <QObject>

#include "bar.h"

class FabricationError : public QObject
{
        Q_OBJECT
    public:
        explicit FabricationError(Bar     *bar        = 0,
                                  qreal   lengthError = 0.0,
                                  QObject *parent     = 0);

        ~FabricationError();

        Bar *fabricationErrorBar() const;

        qreal lengthError() const;

        void setFabricationErrorBar(Bar *bar);

        void setLengthError(qreal lengthError);

    private:
        Bar   *mBar;
        qreal mLengthError;
};

#endif // FABRICATIONERROR_H
