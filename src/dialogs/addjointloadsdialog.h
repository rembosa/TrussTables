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

/* addjointloadsdialog.h */

#ifndef ADDJOINTLOADSDIALOG_H
#define ADDJOINTLOADSDIALOG_H

#include <cmath>

#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

#include "joint.h"
#include "unitsandlimits.h"

namespace Ui
{
    class AddJointLoadsDialog;
}

class AddJointLoadsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit AddJointLoadsDialog(UnitsAndLimits *unitsAndLimits  = 0,
                                     QList<Joint *> *jointsList      = 0,
                                     QTableView     *jointsTableView = 0,
                                     QWidget        *parent          = 0);

        ~AddJointLoadsDialog();

        QPoint position() const;

        void updateJoints();
        void updateUnits();

    signals:
        void inputsSignal(int     loadJointNumber,
                          qreal   horizontalComponent,
                          qreal   verticalComponent,
                          QString position);

    public slots:
        void setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected);
        void addJointLoads();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::AddJointLoadsDialog *ui;
        UnitsAndLimits          *mUnitsAndLimits;
        QList<Joint *>          *mJointsList;
        QTableView              *mJointsTableView;
        QPoint                  mPoint;
};

#endif // ADDJOINTLOADSDIALOG_H
