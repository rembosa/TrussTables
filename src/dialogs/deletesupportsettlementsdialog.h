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

/* deletesupportsettlementsdialog.h */

#ifndef DELETESUPPORTSETTLEMENTSDIALOG_H
#define DELETESUPPORTSETTLEMENTSDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

#include "joint.h"
#include "support.h"
#include "supportsettlement.h"

namespace Ui
{
    class DeleteSupportSettlementsDialog;
}

class DeleteSupportSettlementsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit DeleteSupportSettlementsDialog(QList<Joint *>             *jointsList                  = 0,
                                                QList<SupportSettlement *> *supportSettlementsList      = 0,
                                                QTableView                 *supportSettlementsTableView = 0,
                                                QWidget                    *parent                      = 0);

        ~DeleteSupportSettlementsDialog();

        QPoint position() const;

    signals:
        void inputsSignal();

    public slots:
        void setSelectedSupportSettlements(const QItemSelection &selected, const QItemSelection &deselected);
        void deleteSupportSettlements();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::DeleteSupportSettlementsDialog *ui;
        QList<Joint *>                     *mJointsList;
        QList<SupportSettlement *>         *mSupportSettlementsList;
        QTableView                         *mSupportSettlementsTableView;
        QPoint                             mPoint;
};

#endif // DELETESUPPORTSETTLEMENTSDIALOG_H
