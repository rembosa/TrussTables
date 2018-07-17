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

/* addfabricationerrorsdialog.h */

#ifndef ADDFABRICATIONERRORSDIALOG_H
#define ADDFABRICATIONERRORSDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

#include "bar.h"
#include "unitsandlimits.h"

namespace Ui
{
    class AddFabricationErrorsDialog;
}

class AddFabricationErrorsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit AddFabricationErrorsDialog(UnitsAndLimits *unitsAndLimits    = 0,
                                            QList<Bar *>   *barsList          = 0,
                                            QList<Bar *>   *redundantBarsList = 0,
                                            QTableView     *barsTableView     = 0,
                                            QWidget        *parent            = 0);

        ~AddFabricationErrorsDialog();

        QPoint position() const;

        void updateBars();
        void updateUnits();

    signals:
        void inputsSignal(int barNumber, qreal lengthError);

    public slots:
        void setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected);
        void addFabricationErrors();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::AddFabricationErrorsDialog *ui;
        UnitsAndLimits                 *mUnitsAndLimits;
        QList<Bar *>                   *mBarsList;
        QList<Bar *>                   *mRedundantBarsList;
        QTableView                     *mBarsTableView;
        QPoint                         mPoint;
};

#endif // ADDFABRICATIONERRORSDIALOG_H
