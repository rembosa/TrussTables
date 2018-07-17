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

/* deletesupportsdialog.cpp */

#include "deletesupportsdialog.h"
#include "ui_deletesupportsdialog.h"

DeleteSupportsDialog::DeleteSupportsDialog(QList<Joint *>   *jointsList,
                                           QList<Support *> *supportsList,
                                           QTableView       *supportsTableView,
                                           QWidget          *parent) : QDialog(parent), ui(new Ui::DeleteSupportsDialog)
{
    ui->setupUi(this);
    mJointsList        = jointsList;
    mSupportsList      = supportsList;
    mSupportsTableView = supportsTableView;
    mPoint             = QPoint(0, 0);

    connect(ui->deleteSupportsPushButton, SIGNAL(clicked()), this, SLOT(deleteSupports()));
}

DeleteSupportsDialog::~DeleteSupportsDialog()
{
    delete ui;
}

QPoint DeleteSupportsDialog::position() const
{
    return mPoint;
}

void DeleteSupportsDialog::deleteSupports()
{
    QItemSelectionModel *itemSelectionModel = mSupportsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No support selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Support Deletion"));
    messageBox.setText(tr("Confirm Support(s) Deletion!"));
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

void DeleteSupportsDialog::setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mSupportsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedSupports;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedSupports.append(index.row() + 1);
    }

    ui->selectedSupportsTextEdit->clear();

    foreach (int supportNumber, selectedSupports)
    {
        Support *support = mSupportsList->at(supportNumber - 1);
        Joint *joint     = support->supportJoint();
        int jointNumber  = mJointsList->indexOf(joint) + 1;
        ui->selectedSupportsTextEdit->append(QString::number(supportNumber) + " @ joint "
                                             + QString::number(jointNumber));
    }
}

void DeleteSupportsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
