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

/* deletejointloadsdialog.cpp */

#include "deletejointloadsdialog.h"
#include "ui_deletejointloadsdialog.h"

DeleteJointLoadsDialog::DeleteJointLoadsDialog(QList<Joint *>     *jointsList,
                                               QList<JointLoad *> *jointLoadsList,
                                               QTableView         *jointLoadsTableView,
                                               QWidget            *parent) : QDialog(parent),
    ui(new Ui::DeleteJointLoadsDialog)
{
    ui->setupUi(this);
    mPoint               = QPoint(0, 0);
    mJointsList          = jointsList;
    mJointLoadsList      = jointLoadsList;
    mJointLoadsTableView = jointLoadsTableView;

    connect(ui->deleteJointLoadsPushButton, SIGNAL(clicked()), this, SLOT(deleteJointLoads()));
}

DeleteJointLoadsDialog::~DeleteJointLoadsDialog()
{
    delete ui;
}

QPoint DeleteJointLoadsDialog::position() const
{
    return mPoint;
}

void DeleteJointLoadsDialog::setSelectedJointLoads(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mJointLoadsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedJointLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJointLoads.append(index.row() + 1);
    }

    ui->selectedLoadsTextEdit->clear();

    foreach (int loadNumber, selectedJointLoads)
    {
        JointLoad *jointLoad = mJointLoadsList->at(loadNumber - 1);
        Joint *joint         = jointLoad->loadJoint();
        int jointNumber      = mJointsList->indexOf(joint) + 1;
        ui->selectedLoadsTextEdit->append(QString::number(loadNumber) + " @ joint " + QString::number(jointNumber));
    }
}

void DeleteJointLoadsDialog::deleteJointLoads()
{
    QItemSelectionModel *itemSelectionModel = mJointLoadsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No load selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Joint Load Deletion"));
    messageBox.setText(tr("Confirm Joint Load(s) Deletion!"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (messageBox.exec() == QMessageBox::Cancel)
    {
        return;
    }

    emit inputsSignal();
}

void DeleteJointLoadsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
