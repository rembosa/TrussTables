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

/* addinfluenceloadsdialog.cpp */

#include "addinfluenceloadsdialog.h"
#include "ui_addinfluenceloadsdialog.h"

AddInfluenceLoadsDialog::AddInfluenceLoadsDialog(UnitsAndLimits         *unitsAndLimits,
                                                 QList<InfluenceLoad *> *influenceLoadsList,
                                                 QList<Joint *>         *jointsList,
                                                 QTableView             *jointsTableView,
                                                 QWidget                *parent) : QDialog(parent),
    ui(new Ui::AddInfluenceLoadsDialog)
{
    ui->setupUi(this);
    mUnitsAndLimits     = unitsAndLimits;
    mInfluenceLoadsList = influenceLoadsList;
    mJointsList         = jointsList;
    mJointsTableView    = jointsTableView;
    mPoint              = QPoint(0, 0);
    updateUnits();
    updateDefaultName();

    connect(ui->addPointLoadPushButton, SIGNAL(clicked()), this, SLOT(addPointLoad()));
    connect(ui->removePointLoadPushButton, SIGNAL(clicked()), this, SLOT(removePointLoad()));
    connect(ui->addInfluenceLoadPushButton, SIGNAL(clicked()), this, SLOT(addInfluenceLoads()));
}

AddInfluenceLoadsDialog::~AddInfluenceLoadsDialog()
{
    delete ui;
}

QPoint AddInfluenceLoadsDialog::position() const
{
    return mPoint;
}

void AddInfluenceLoadsDialog::updateUnits()
{
    ui->pointLoadLabel->setText(tr("V (%1)").arg(mUnitsAndLimits->loadUnit()));
    ui->pointLoadDoubleSpinBox->setToolTip(tr("Valid range\n"
                                              "Min : %1 (%3)\n"
                                              "Max : %2 (%3)")
                                           .arg(QString::number(mUnitsAndLimits->minPointLoad(), 'g', 3))
                                           .arg(QString::number(mUnitsAndLimits->maxPointLoad(), 'g', 3))
                                           .arg(mUnitsAndLimits->loadUnit()));

    ui->offsetLabel->setText(tr("Offset (%1)").arg(mUnitsAndLimits->coordinateUnit()));
    ui->offsetDoubleSpinBox->setToolTip(tr("Valid range\n"
                                           "Min : %1 (%3)\n"
                                           "Max : %2 (%3)")
                                        .arg(QString::number(mUnitsAndLimits->minOffset(), 'g', 3))
                                        .arg(QString::number(mUnitsAndLimits->maxOffset(), 'g', 3))
                                        .arg(mUnitsAndLimits->coordinateUnit()));

    ui->pointLoadDoubleSpinBox->setDecimals(mUnitsAndLimits->pointLoadDecimals());
    ui->pointLoadDoubleSpinBox->setMinimum(mUnitsAndLimits->minPointLoad());
    ui->pointLoadDoubleSpinBox->setMaximum(mUnitsAndLimits->maxPointLoad());
    ui->pointLoadDoubleSpinBox->clear();
    ui->pointLoadDoubleSpinBox->setValue(mUnitsAndLimits->minPointLoad());

    ui->offsetDoubleSpinBox->setDecimals(mUnitsAndLimits->offsetDecimals());
    ui->offsetDoubleSpinBox->setMinimum(mUnitsAndLimits->minOffset());
    ui->offsetDoubleSpinBox->setMaximum(mUnitsAndLimits->maxOffset());
    ui->offsetDoubleSpinBox->clear();
    ui->offsetDoubleSpinBox->setValue(mUnitsAndLimits->minOffset());

    ui->pointLoadsListWidget->clear();
    mPointLoads.clear();
    mPointLoadPositions.clear();
}

void AddInfluenceLoadsDialog::updateDefaultName()
{
    int count = mInfluenceLoadsList->size();
    ui->nameLineEdit->setText(tr("Influence Load ") + QString::number(count + 1));
    ui->nameLineEdit->setFocus();
    ui->nameLineEdit->selectAll();
}

void AddInfluenceLoadsDialog::setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected)
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

    mPath = selectedJoints;

    ui->selectedJointsTextEdit->clear();

    foreach (int jointNumber, selectedJoints)
    {
        ui->selectedJointsTextEdit->append(QString::number(jointNumber));
    }
}

void AddInfluenceLoadsDialog::addPointLoad()
{
    if (mPointLoads.size() == mUnitsAndLimits->maxPointLoads())
    {
        return;
    }

    qreal pointLoadValue = ui->pointLoadDoubleSpinBox->value();
    qreal offset         = 0.0;

    if (ui->pointLoadsListWidget->count() == 0)
    {
        mPointLoads.append(pointLoadValue);
        mPointLoadPositions.append(offset);

        ui->pointLoadsListWidget->addItem(tr("%1 %2 @ %3 %4")
                                          .arg(QString::number(pointLoadValue, 'f',
                                                               mUnitsAndLimits->pointLoadDecimals()))
                                          .arg(mUnitsAndLimits->loadUnit())
                                          .arg(QString::number(offset, 'f', mUnitsAndLimits->offsetDecimals()))
                                          .arg(mUnitsAndLimits->coordinateUnit()));
    }
    else
    {
        offset = ui->offsetDoubleSpinBox->value();
        mPointLoads.append(pointLoadValue);
        qreal pointLoadPosition = mPointLoadPositions.last() + offset;
        mPointLoadPositions.append(pointLoadPosition);
        ui->pointLoadsListWidget->addItem(tr("%1 %2 @ %3 %4")
                                          .arg(QString::number(pointLoadValue, 'f',
                                                               mUnitsAndLimits->pointLoadDecimals()))
                                          .arg(mUnitsAndLimits->loadUnit())
                                          .arg(QString::number(pointLoadPosition, 'f',
                                                               mUnitsAndLimits->offsetDecimals()))
                                          .arg(mUnitsAndLimits->coordinateUnit()));
    }
}

void AddInfluenceLoadsDialog::removePointLoad()
{
    if (ui->pointLoadsListWidget->count() > 0)
    {
        mPointLoads.removeLast();
        mPointLoadPositions.removeLast();
        int row = ui->pointLoadsListWidget->count() - 1;
        QListWidgetItem *item = ui->pointLoadsListWidget->takeItem(row);
        delete item;
    }
}

void AddInfluenceLoadsDialog::addInfluenceLoads()
{
    if (ui->nameLineEdit->text().isEmpty())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Name : Input error"));
        messageBox.setInformativeText(tr("No entry!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else
    {
        QString name = ui->nameLineEdit->text();
        bool check   = false;

        foreach (InfluenceLoad *load, *mInfluenceLoadsList)
        {
            if (name == load->name())
            {
                check = true;
                break;
            }
        }

        if (check)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Name : Input error"));
            messageBox.setInformativeText(tr("Entry not unique!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }
    }

    mName = ui->nameLineEdit->text();

    if (ui->leftToRightRadioButton->isChecked())
    {
        mDirection = tr("LR");
    }
    else
    {
        mDirection = tr("RL");
    }

    if (ui->pointLoadsListWidget->count() == 0)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Point Loads : Input error"));
        messageBox.setInformativeText(tr("No entry!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }


    if (mPath.isEmpty())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
        messageBox.setInformativeText(tr("No joints selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    int count = mPath.size();

    if (count < 2)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
        messageBox.setInformativeText(tr("At least two joints requred!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    int index = 0;

    forever
    {
        Joint *jointA = mJointsList->at(mPath.at(index) - 1);
        Joint *jointB = mJointsList->at(mPath.at(index + 1) - 1);
        qreal x1      = jointA->xCoordinate();
        qreal y1      = jointA->yCoordinate();
        qreal x2      = jointB->xCoordinate();
        qreal y2      = jointB->yCoordinate();
        qreal deltaX  = x2 - x1;
        qreal deltaY  = y2 - y1;

        if (!jointA->connectedJoints()->contains(jointB))
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
            messageBox.setInformativeText(tr("Path not connected!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        if (deltaX < 0.0)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
            messageBox.setInformativeText(tr("Path must increase left to right!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        if (std::fabs(deltaY) > 0.0)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
            messageBox.setInformativeText(tr("Path not horizontal!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        if (++index == (count - 1))
        {
            break;
        }
    }


    Joint *firstJoint           = mJointsList->at(mPath.first() - 1);
    qreal x1                    = firstJoint->xCoordinate();
    qreal y1                    = firstJoint->yCoordinate();
    qreal epsilonMagnitudeSmall = 1.0e-12;

    foreach (Joint *joint, *firstJoint->connectedJoints())
    {
        qreal x2     = joint->xCoordinate();
        qreal y2     = joint->yCoordinate();
        qreal deltaX = x2 - x1;
        qreal deltaY = y2 - y1;

        bool checkA = deltaX < 0.0;
        bool checkB = (std::fabs(deltaY) < epsilonMagnitudeSmall);

        if (checkA && checkB)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
            messageBox.setInformativeText(tr("Path length not maximum!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }
    }

    Joint *lastJoint = mJointsList->at(mPath.last() - 1);
    x1 = lastJoint->xCoordinate();
    y1 = lastJoint->yCoordinate();

    foreach (Joint *joint, *lastJoint->connectedJoints())
    {
        qreal x2     = joint->xCoordinate();
        qreal y2     = joint->yCoordinate();
        qreal deltaX = x2 - x1;
        qreal deltaY = y2 - y1;

        bool checkA = deltaX > 0.0;
        bool checkB = (std::fabs(deltaY) < epsilonMagnitudeSmall);

        if (checkA && checkB)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
            messageBox.setInformativeText(tr("Path length not maximum!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }
    }

    emit inputsSignal(mName,
                      mDirection,
                      mPointLoads,
                      mPointLoadPositions);
}

void AddInfluenceLoadsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
