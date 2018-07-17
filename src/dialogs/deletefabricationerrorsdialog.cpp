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

/* deletefabricationerrorsdialog.cpp */

#include "deletefabricationerrorsdialog.h"
#include "ui_deletefabricationerrorsdialog.h"

DeleteFabricationErrorsDialog::DeleteFabricationErrorsDialog(QList<Bar *>              *barsList,
                                                             QList<FabricationError *> *fabricationErrorsList,
                                                             QTableView                *fabricationErrorsTableView,
                                                             QWidget                   *parent) : QDialog(parent),
    ui(new Ui::DeleteFabricationErrorsDialog)
{
    ui->setupUi(this);
    mPoint                      = QPoint(0, 0);
    mBarsList                   = barsList;
    mFabricationErrorsList      = fabricationErrorsList;
    mFabricationErrorsTableView = fabricationErrorsTableView;

    connect(ui->deleteFabricationErrorsPushButton, SIGNAL(clicked()), this, SLOT(deleteFabricationErrors()));
}

DeleteFabricationErrorsDialog::~DeleteFabricationErrorsDialog()
{
    delete ui;
}

QPoint DeleteFabricationErrorsDialog::position() const
{
    return mPoint;
}

void DeleteFabricationErrorsDialog::setSelectedFabricationErrors(const QItemSelection &selected,
                                                                 const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mFabricationErrorsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedFabricationErrors;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedFabricationErrors.append(index.row() + 1);
    }

    ui->selectedFabricationErrorsTextEdit->clear();

    foreach (int fabricationErrorNumber, selectedFabricationErrors)
    {
        FabricationError *fabricationError = mFabricationErrorsList->at(fabricationErrorNumber - 1);
        Bar *bar                           = fabricationError->fabricationErrorBar();
        int barNumber                      = mBarsList->indexOf(bar) + 1;
        ui->selectedFabricationErrorsTextEdit->append(QString::number(fabricationErrorNumber) + " @ bar "
                                                   + QString::number(barNumber));
    }
}

void DeleteFabricationErrorsDialog::deleteFabricationErrors()
{
    QItemSelectionModel *itemSelectionModel = mFabricationErrorsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No fabrication error selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Fabrication Error Deletion"));
    messageBox.setText(tr("Confirm Fabrication Error(s) Deletion!"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (messageBox.exec() == QMessageBox::Cancel)
    {
        return;
    }

    emit inputsSignal();
}

void DeleteFabricationErrorsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
