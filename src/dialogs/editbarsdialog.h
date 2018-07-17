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

/* editbarsdialog.h */

#ifndef EDITBARSDIALOG_H
#define EDITBARSDIALOG_H

#include <algorithm>
#include <cmath>
#include <vector>

#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

#include "bar.h"
#include "joint.h"
#include "unitsandlimits.h"

namespace Ui
{
    class EditBarsDialog;
}

class EditBarsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit EditBarsDialog(UnitsAndLimits *unitsAndLimits = 0,
                                QList<Joint *> *jointsList     = 0,
                                QList<Bar *>   *barsList       = 0,
                                QTableView     *barsTableView  = 0,
                                QWidget        *parent         = 0);

        ~EditBarsDialog();

        QPoint position() const;

        void updateUnits();
        void updateFirstJointOptions();

    signals:
        void inputsSignal(int   firstJointNumber,
                          int   secondJointNumber,
                          qreal area,
                          qreal modulus,
                          qreal factor,
                          qreal unitWeight);

    public slots:
        void setSecondJointOptions(int firstJointIndex);
        void setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected);
        void editBars();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::EditBarsDialog *ui;
        UnitsAndLimits     *mUnitsAndLimits;
        QList<Joint *>     *mJointsList;
        QList<Bar *>       *mBarsList;
        QTableView         *mBarsTableView;
        QPoint             mPoint;
};

#endif // EDITBARSDIALOG_H
