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

/* addsupportsdialog.cpp */

#include "addsupportsdialog.h"
#include "ui_addsupportsdialog.h"

AddSupportsDialog::AddSupportsDialog(QList<Joint *> *jointsList,
                                     QTableView     *jointsTableView,
                                     QWidget        *parent) : QDialog(parent), ui(new Ui::AddSupportsDialog)
{
    ui->setupUi(this);
    mJointsList      = jointsList;
    mJointsTableView = jointsTableView;
    mPoint           = QPoint(0, 0);

    ui->supportTypeComboBox->addItem(QIcon(":/icons/fixed_left.png"), QString("fixed support"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/fixed_top.png"), QString("fixed support"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/fixed_right.png"), QString("fixed support"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/fixed_bottom.png"), QString("fixed support"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_inclined.png"), QString("inclined roller"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_left.png"), QString("roller"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_top.png"), QString("roller"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_right.png"), QString("roller"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_bottom.png"), QString("roller"));

    ui->supportTypeComboBox->setCurrentIndex(-1);
    //ui->supportTypeComboBox->setIconSize(QSize(32, 32));

#ifdef Q_OS_WIN
    ui->supportTypeComboBox->setIconSize(QSize(32, 32));
#endif

#ifdef Q_OS_LINUX
    ui->supportTypeComboBox->setIconSize(QSize(25, 25));
#endif

    connect(ui->supportTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setAngleOption(int)));
    connect(ui->addSupportsPushButton, SIGNAL(clicked()), this, SLOT(addSupports()));
}

AddSupportsDialog::~AddSupportsDialog()
{
    delete ui;
}

QPoint AddSupportsDialog::position() const
{
    return mPoint;
}

void AddSupportsDialog::updateJoints()
{
    ui->supportJointComboBox->clear();

    foreach (Joint *joint, *mJointsList)
    {
        if (joint->isSupported())
        {
            continue;
        }

        int supportJointNumber = mJointsList->indexOf(joint) + 1;
        ui->supportJointComboBox->addItem(QString::number(supportJointNumber));
    }

    ui->supportJointComboBox->setCurrentIndex(-1);
}

void AddSupportsDialog::setAngleOption(int option)
{
    if (option == 4)
    {
        if (!ui->supportAngleDoubleSpinBox->isEnabled())
        {
            ui->supportAngleDoubleSpinBox->setEnabled(true);
            ui->supportAngleDoubleSpinBox->setValue(0.00);
        }
    }
    else
    {
        if (ui->supportAngleDoubleSpinBox->isEnabled())
        {
            ui->supportAngleDoubleSpinBox->clear();
            ui->supportAngleDoubleSpinBox->setValue(0.0);
            ui->supportAngleDoubleSpinBox->setEnabled(false);
        }

        switch (option)
        {
            case 0:
            case 5:
                ui->supportAngleDoubleSpinBox->setValue(90.0);
                break;
            case 1:
            case 6:
                ui->supportAngleDoubleSpinBox->setValue(180.0);
                break;
            case 2:
            case 7:
                ui->supportAngleDoubleSpinBox->setValue(270.0);
                break;
            case 3:
            case 8:
            case -1:
                ui->supportAngleDoubleSpinBox->setValue(0.0);
                break;
            default:
                break;
        }
    }
}

void AddSupportsDialog::setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected)
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
        if (!ui->supportJointComboBox->isEnabled())
        {
            ui->supportJointComboBox->setEnabled(true);
        }
    }
    else
    {
        if (ui->supportJointComboBox->isEnabled())
        {
            ui->supportJointComboBox->setCurrentIndex(-1);
            ui->supportJointComboBox->setEnabled(false);
        }
    }

    ui->selectedJointsTextEdit->clear();

    foreach (int supportJointNumber, selectedJoints)
    {
        ui->selectedJointsTextEdit->append(QString::number(supportJointNumber));
    }
}

void AddSupportsDialog::addSupports()
{
    int supportJointNumber           = 0;
    UnitsAndLimits::SupportType type = UnitsAndLimits::FIXED_BOTTOM;
    qreal angle                      = 0.0;

    if (ui->supportJointComboBox->isEnabled())
    {
        if (ui->supportJointComboBox->currentIndex() < 0)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Support joint : Input error"));
            messageBox.setInformativeText(tr("No selection!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }
        else
        {
            supportJointNumber = ui->supportJointComboBox->currentText().toInt();
        }
    }

    if (ui->supportTypeComboBox->currentIndex() < 0)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Support type : Input error"));
        messageBox.setInformativeText(tr("No selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else
    {
        int index = ui->supportTypeComboBox->currentIndex();

        switch (index)
        {
            case 0:
                type = UnitsAndLimits::FIXED_LEFT;
                break;
            case 1:
                type = UnitsAndLimits::FIXED_TOP;
                break;
            case 2:
                type = UnitsAndLimits::FIXED_RIGHT;
                break;
            case 3:
                type = UnitsAndLimits::FIXED_BOTTOM;
                break;
            case 4:
                type = UnitsAndLimits::ROLLER;
                break;
            case 5:
                type = UnitsAndLimits::ROLLER_LEFT;
                break;
            case 6:
                type = UnitsAndLimits::ROLLER_TOP;
                break;
            case 7:
                type = UnitsAndLimits::ROLLER_RIGHT;
                break;
            case 8:
                type = UnitsAndLimits::ROLLER_BOTTOM;
                break;
            default:
                break;
        }

        angle = ui->supportAngleDoubleSpinBox->value();
    }

    emit inputsSignal(supportJointNumber,
                      type,
                      angle);
}

void AddSupportsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
