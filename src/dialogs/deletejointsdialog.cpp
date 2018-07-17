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

/* deletejointsdialog.cpp */

#include "deletejointsdialog.h"
#include "ui_deletejointsdialog.h"

DeleteJointsDialog::DeleteJointsDialog(QTableView *jointsTableView, QWidget *parent) : QDialog(parent),
    ui(new Ui::DeleteJointsDialog)
{
    ui->setupUi(this);
    mJointsTableView = jointsTableView;
    mPoint           = QPoint(0, 0);

    connect(ui->deleteJointsPushButton, SIGNAL(clicked()), this, SLOT(deleteJoints()));
}

DeleteJointsDialog::~DeleteJointsDialog()
{
    delete ui;
}

QPoint DeleteJointsDialog::position() const
{
    return mPoint;
}

void DeleteJointsDialog::deleteJoints()
{
    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No joint selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Joint Deletion"));
    messageBox.setText(tr("Confirm Joint(s) Deletion!"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (messageBox.exec() == QMessageBox::Cancel)
    {
        return;
    }
    else
    {
        emit inputsSignal();
    }
}

void DeleteJointsDialog::setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedJoints;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJoints.append(index.row() + 1);
    }

    ui->selectedJointsTextEdit->clear();

    foreach (int jointNumber, selectedJoints)
    {
        ui->selectedJointsTextEdit->append(QString::number(jointNumber));
    }
}

void DeleteJointsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
