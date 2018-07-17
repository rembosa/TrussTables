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

/* addjointsdialog.h */

#ifndef ADDJOINTSDIALOG_H
#define ADDJOINTSDIALOG_H

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
    class AddJointsDialog;
}

class AddJointsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit AddJointsDialog(UnitsAndLimits *unitsAndLimits  = 0,
                                 QList<Joint *> *jointsList      = 0,
                                 QTableView     *jointsTableView = 0,
                                 QWidget        *parent          = 0);

        ~AddJointsDialog();

        QPoint position() const;

        void updateJoints();
        void updateUnits();

    signals:
        void inputsSignal(qreal deltaX, qreal deltaY, int repeat);
        void inputsSignal(QString axis, int axisJointNumber);

    public slots:
        void setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected);
        void setDisableInput();
        void setEnableInput();
        void addJoints();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::AddJointsDialog *ui;
        UnitsAndLimits      *mUnitsAndLimits;
        QList<Joint *>      *mJointsList;
        QTableView          *mJointsTableView;
        QPoint              mPoint;
};

#endif // ADDJOINTSDIALOG_H
