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

/* deletesupportsettlementsdialog.cpp */

#include "deletesupportsettlementsdialog.h"
#include "ui_deletesupportsettlementsdialog.h"

DeleteSupportSettlementsDialog::DeleteSupportSettlementsDialog(QList<Joint *>             *jointsList,
                                                               QList<SupportSettlement *> *supportSettlementsList,
                                                               QTableView                 *supportSettlementsTableView,
                                                               QWidget                    *parent) : QDialog(parent),
    ui(new Ui::DeleteSupportSettlementsDialog)
{
    ui->setupUi(this);
    mPoint                       = QPoint(0, 0);
    mJointsList                  = jointsList;
    mSupportSettlementsList      = supportSettlementsList;
    mSupportSettlementsTableView = supportSettlementsTableView;

    connect(ui->deleteSupportSettlementsPushButton, SIGNAL(clicked()), this, SLOT(deleteSupportSettlements()));
}

DeleteSupportSettlementsDialog::~DeleteSupportSettlementsDialog()
{
    delete ui;
}

QPoint DeleteSupportSettlementsDialog::position() const
{
    return mPoint;
}

void DeleteSupportSettlementsDialog::setSelectedSupportSettlements(const QItemSelection &selected,
                                                                   const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mSupportSettlementsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedSupportSettlements;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedSupportSettlements.append(index.row() + 1);
    }

    ui->selectedSupportSettlementsTextEdit->clear();

    foreach (int supportSettlementNumber, selectedSupportSettlements)
    {
        SupportSettlement *settlement = mSupportSettlementsList->at(supportSettlementNumber - 1);
        Support *support              = settlement->settlementSupport();
        Joint *joint                  = support->supportJoint();
        int jointNumber               = mJointsList->indexOf(joint) + 1;
        ui->selectedSupportSettlementsTextEdit->append(QString::number(supportSettlementNumber) + " @ joint "
                                                       + QString::number(jointNumber));
    }
}

void DeleteSupportSettlementsDialog::deleteSupportSettlements()
{
    QItemSelectionModel *itemSelectionModel = mSupportSettlementsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No support settlement selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Support Settlement Deletion"));
    messageBox.setText(tr("Confirm Support Settlement(s) Deletion!"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (messageBox.exec() == QMessageBox::Cancel)
    {
        return;
    }

    emit inputsSignal();
}

void DeleteSupportSettlementsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
