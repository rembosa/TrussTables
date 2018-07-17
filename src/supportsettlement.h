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

/* supportsettlement.h */

#ifndef SUPPORTSETTLEMENT_H
#define SUPPORTSETTLEMENT_H

#include <QObject>

#include "support.h"

class SupportSettlement : public QObject
{
        Q_OBJECT

    public:
        explicit SupportSettlement(Support *support   = 0,
                                   qreal   settlement = 0.0,
                                   QObject *parent    = 0);

        ~SupportSettlement();

        Support *settlementSupport() const;

        qreal settlement();

        void setSettlementSupport(Support *support);

        void setSettlement(qreal settlement);

    private:
        Support *mSupport;
        qreal   mSettlement;
};

#endif // SUPPORTSETTLEMENT_H
