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

/* editsupportsettlementsdialog.h */

#ifndef EDITSUPPORTSETTLEMENTSDIALOG_H
#define EDITSUPPORTSETTLEMENTSDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

#include "joint.h"
#include "support.h"
#include "supportsettlement.h"
#include "unitsandlimits.h"

namespace Ui
{
    class EditSupportSettlementsDialog;
}

class EditSupportSettlementsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit EditSupportSettlementsDialog(UnitsAndLimits             *unitsAndLimits              = 0,
                                              QList<Joint *>             *jointsList                  = 0,
                                              QList<Support *>           *supportsList                = 0,
                                              QList<SupportSettlement *> *supportSettlementsList      = 0,
                                              QTableView                 *supportSettlementsTableView = 0,
                                              QList<Support *>           *redundantSupportsList       = 0,
                                              QWidget                    *parent                      = 0);

        ~EditSupportSettlementsDialog();

        QPoint position() const;

        void updateSupports();
        void updateUnits();

    signals:
        void inputsSignal(int supportNumber, qreal settlement);

    public slots:
        void setSelectedSupportSettlements(const QItemSelection &selected, const QItemSelection &deselected);
        void editSupportSettlements();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::EditSupportSettlementsDialog *ui;
        UnitsAndLimits                   *mUnitsAndLimits;
        QList<Joint *>                   *mJointsList;
        QList<Support *>                 *mSupportsList;
        QList<SupportSettlement *>       *mSupportSettlementsList;
        QTableView                       *mSupportSettlementsTableView;
        QList<Support *>                 *mRedundantSupportsList;
        QPoint                           mPoint;
};

#endif // EDITSUPPORTSETTLEMENTSDIALOG_H
