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

/* unitsandsetupdialog.h */

#ifndef UNITSANDSETUPDIALOG_H
#define UNITSANDSETUPDIALOG_H

#include <QDialog>

#include "unitsandlimits.h"

namespace Ui
{
    class UnitsAndSetupDialog;
}

class UnitsAndSetupDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit UnitsAndSetupDialog(QString        *descriptionText = 0,
                                     UnitsAndLimits *unitsAndLimits  = 0,
                                     QWidget        *parent          = 0);

        ~UnitsAndSetupDialog();

        void setMetricUnits();
        void setImperialUnits();

    public slots:
        void onSystemOptionChanged(bool checked);
        void setThermalCoefficientUnits(int index);
        void setTemperatureChangeUnits(int index);
        void accept();

    private:
        QString                 *mDescriptionText;
        Ui::UnitsAndSetupDialog *ui;
        UnitsAndLimits          *mUnitsAndLimits;
};

#endif // UNITSANDSETUPDIALOG_H
