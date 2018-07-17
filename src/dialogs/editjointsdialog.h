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

/* editjointsdialog.h */

#ifndef EDITJOINTSDIALOG_H
#define EDITJOINTSDIALOG_H

#include <cmath>

#include <QCloseEvent>
#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

#include "unitsandlimits.h"

namespace Ui
{
    class EditJointsDialog;
}

class EditJointsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit EditJointsDialog(UnitsAndLimits *unitsAndLimits  = 0,
                                  QTableView     *jointsTableView = 0,
                                  QWidget        *parent          = 0);

        ~EditJointsDialog();

        QPoint position() const;

        void updateUnits();

    signals:
        void inputsSignal(qreal deltaX, qreal deltaY);

    public slots:
        void editJoints();
        void setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected);

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::EditJointsDialog *ui;
        UnitsAndLimits       *mUnitsAndLimits;
        QTableView           *mJointsTableView;
        QPoint               mPoint;
};

#endif // EDITJOINTSDIALOG_H
