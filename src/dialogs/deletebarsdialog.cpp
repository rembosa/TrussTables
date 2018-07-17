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

/* deletebarsdialog.cpp */

#include "deletebarsdialog.h"
#include "ui_deletebarsdialog.h"

DeleteBarsDialog::DeleteBarsDialog(QTableView *barsTableView, QWidget *parent) : QDialog(parent),
    ui(new Ui::DeleteBarsDialog)
{
    ui->setupUi(this);
    mBarsTableView = barsTableView;
    mPoint         = QPoint(0, 0);

    connect(ui->deleteBarsPushButton, SIGNAL(clicked()), this, SLOT(deleteBars()));
}

DeleteBarsDialog::~DeleteBarsDialog()
{
    delete ui;
}

QPoint DeleteBarsDialog::position() const
{
    return mPoint;
}

void DeleteBarsDialog::deleteBars()
{
    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No bar selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Bar Deletion"));
    messageBox.setText(tr("Confirm Bar(s) Deletion!"));
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

void DeleteBarsDialog::setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedBars;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedBars.append(index.row() + 1);
    }

    ui->selectedBarsTextEdit->clear();

    foreach (int BarNumber, selectedBars)
    {
        ui->selectedBarsTextEdit->append(QString::number(BarNumber));
    }
}

void DeleteBarsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
