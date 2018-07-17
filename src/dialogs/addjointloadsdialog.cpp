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

/* addjointloadsdialog.cpp */

#include "addjointloadsdialog.h"
#include "ui_addjointloadsdialog.h"

AddJointLoadsDialog::AddJointLoadsDialog(UnitsAndLimits *unitsAndLimits,
                                         QList<Joint *> *jointsList,
                                         QTableView     *jointsTableView,
                                         QWidget        *parent) : QDialog(parent), ui(new Ui::AddJointLoadsDialog)
{
    ui->setupUi(this);
    mPoint = QPoint(0, 0);

    ui->positionComboBox->addItem(tr("head"));
    ui->positionComboBox->addItem(tr("tail"));
    ui->positionComboBox->setCurrentIndex(-1);

    mUnitsAndLimits  = unitsAndLimits;
    mJointsList      = jointsList;
    mJointsTableView = jointsTableView;
    updateUnits();

    connect(ui->addJointLoadsPushButton, SIGNAL(clicked()), this, SLOT(addJointLoads()));
}

AddJointLoadsDialog::~AddJointLoadsDialog()
{
    delete ui;
}

QPoint AddJointLoadsDialog::position() const
{
    return mPoint;
}

void AddJointLoadsDialog::updateJoints()
{
    ui->loadJointComboBox->clear();

    foreach (Joint *joint, *mJointsList)
    {
        int jointNumber = mJointsList->indexOf(joint) + 1;
        ui->loadJointComboBox->addItem(QString::number(jointNumber));
    }

    ui->loadJointComboBox->setCurrentIndex(-1);
}

void AddJointLoadsDialog::updateUnits()
{
    ui->horizontalComponentLabel->setText(tr("H (%1)").arg(mUnitsAndLimits->loadUnit()));
    ui->verticalComponentLabel->setText(tr("V (%1)").arg(mUnitsAndLimits->loadUnit()));

    ui->horizontalComponentDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                        "Min : %1 (%3)\n"
                                                        "Max :  %2 (%3)")
                                                     .arg(QString::number(-mUnitsAndLimits->maxJointLoadComponent(), 'g'
                                                                          , 3))
                                                     .arg(QString::number(+mUnitsAndLimits->maxJointLoadComponent(), 'g'
                                                                          , 3))
                                                     .arg(mUnitsAndLimits->loadUnit()));

    ui->verticalComponentDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                      "Min : %1 (%3)\n"
                                                      "Max :  %2 (%3)")
                                                   .arg(QString::number(-mUnitsAndLimits->maxJointLoadComponent(), 'g'
                                                                        , 3))
                                                   .arg(QString::number(+mUnitsAndLimits->maxJointLoadComponent(), 'g'
                                                                        , 3))
                                                   .arg(mUnitsAndLimits->loadUnit()));

    ui->horizontalComponentDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxJointLoadComponent());
    ui->horizontalComponentDoubleSpinBox->setMaximum(+mUnitsAndLimits->maxJointLoadComponent());
    ui->horizontalComponentDoubleSpinBox->setDecimals(mUnitsAndLimits->jointLoadDecimals());
    ui->horizontalComponentDoubleSpinBox->clear();
    ui->horizontalComponentDoubleSpinBox->setValue(0.0);

    ui->verticalComponentDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxJointLoadComponent());
    ui->verticalComponentDoubleSpinBox->setMaximum(+mUnitsAndLimits->maxJointLoadComponent());
    ui->verticalComponentDoubleSpinBox->setDecimals(mUnitsAndLimits->jointLoadDecimals());
    ui->verticalComponentDoubleSpinBox->clear();
    ui->verticalComponentDoubleSpinBox->setValue(0.0);
}

void AddJointLoadsDialog::setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected)
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

    if (selectedJoints.size() == 0)
    {
        if (!ui->loadJointComboBox->isEnabled())
        {
            ui->loadJointComboBox->setEnabled(true);
        }
    }
    else
    {
        if (ui->loadJointComboBox->isEnabled())
        {
            ui->loadJointComboBox->setCurrentIndex(-1);
            ui->loadJointComboBox->setEnabled(false);
        }
    }

    ui->selectedJointsTextEdit->clear();

    foreach (int loadJointNumber, selectedJoints)
    {
        ui->selectedJointsTextEdit->append(QString::number(loadJointNumber));
    }
}

void AddJointLoadsDialog::addJointLoads()
{
    int loadJointNumber       = 0;
    qreal horizontalComponent = 0.0;
    qreal verticalComponent   = 0.0;
    QString position;

    if (ui->loadJointComboBox->isEnabled())
    {
        if (ui->loadJointComboBox->currentIndex() < 0)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Load joint : Input error"));
            messageBox.setInformativeText(tr("No selection!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }
        else
        {
            loadJointNumber = ui->loadJointComboBox->currentText().toInt();
        }
    }

    horizontalComponent = ui->horizontalComponentDoubleSpinBox->value();
    verticalComponent   = ui->verticalComponentDoubleSpinBox->value();

    qreal magnitude = std::sqrt(std::pow(horizontalComponent, 2.0) + std::pow(verticalComponent, 2.0));
    bool checkA     = (magnitude < mUnitsAndLimits->minJointLoadResultant());
    bool checkB     = (magnitude > mUnitsAndLimits->maxJointLoadComponent());

    if (checkA || checkB)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Load magnitude : Input error"));
        QString infoString = QString("Invalid input\n"
                                     "Entered : %1 (%4)\n"
                                     "Valid range\n"
                                     "Min : %2 (%4)\n"
                                     "Max : %3 (%4)")
                .arg(QString::number(magnitude, 'g', 3))
                .arg(QString::number(mUnitsAndLimits->minJointLoadResultant(), 'g', 3))
                .arg(QString::number(mUnitsAndLimits->maxJointLoadComponent(), 'g', 3))
                .arg(mUnitsAndLimits->loadUnit());

        messageBox.setInformativeText(infoString);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    if (ui->positionComboBox->currentIndex() < 0)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Load position : Input error"));
        messageBox.setInformativeText(tr("No selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else
    {
        position = ui->positionComboBox->currentText();
    }

    emit inputsSignal(loadJointNumber,
                      horizontalComponent,
                      verticalComponent,
                      position);
}

void AddJointLoadsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
