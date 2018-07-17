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

/* deleteinfluenceloadsdialog.cpp */

#include "deleteinfluenceloadsdialog.h"
#include "ui_deleteinfluenceloadsdialog.h"

DeleteInfluenceLoadsDialog::DeleteInfluenceLoadsDialog(QTableView *influenceLoadsTableView,
                                                       QWidget    *parent) : QDialog(parent),
    ui(new Ui::DeleteInfluenceLoadsDialog)
{
    ui->setupUi(this);
    mPoint                   = QPoint(0, 0);
    mInfluenceLoadsTableView = influenceLoadsTableView;

    connect(ui->deleteInfluenceLoadsPushButton, SIGNAL(clicked()), this, SLOT(deleteInfluenceLoads()));
}

DeleteInfluenceLoadsDialog::~DeleteInfluenceLoadsDialog()
{
    delete ui;
}

QPoint DeleteInfluenceLoadsDialog::position() const
{
    return mPoint;
}

void DeleteInfluenceLoadsDialog::setSelectedInfluenceLoads(const QItemSelection &selected,
                                                           const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mInfluenceLoadsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedInfluenceLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedInfluenceLoads.append(index.row() + 1);
    }

    ui->selectedInfluenceLoadsTextEdit->clear();

    foreach (int influenceLoadNumber, selectedInfluenceLoads)
    {
        ui->selectedInfluenceLoadsTextEdit->append(QString::number(influenceLoadNumber));
    }
}

void DeleteInfluenceLoadsDialog::deleteInfluenceLoads()
{
    QItemSelectionModel *itemSelectionModel = mInfluenceLoadsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No influence load selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Influence Load Deletion"));
    messageBox.setText(tr("Confirm Influence Load(s) Deletion!"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (messageBox.exec() == QMessageBox::Cancel)
    {
        return;
    }

    emit inputsSignal();
}

void DeleteInfluenceLoadsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
