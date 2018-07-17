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

/* abouttrusstablesdialog.cpp */

#include "abouttrusstablesdialog.h"
#include "ui_abouttrusstablesdialog.h"

AboutTrussTablesDialog::AboutTrussTablesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutTrussTablesDialog)
{
    ui->setupUi(this);
    ui->logoLabel->setPixmap(QPixmap(":/icons/tt.png"));

    QString majorVersion(tr("0"));
    QString minorVersion(tr("0"));
    QString patchVersion(tr("0"));
    QString preReleaseVersion(tr(""));

#ifdef TRUSSTABLES_VERSION_MAJOR
    majorVersion = QString::number(TRUSSTABLES_VERSION_MAJOR);
#endif

#ifdef TRUSSTABLES_VERSION_MINOR
    minorVersion = QString::number(TRUSSTABLES_VERSION_MINOR);
#endif

#ifdef TRUSSTABLES_VERSION_PATCH
    patchVersion = QString::number(TRUSSTABLES_VERSION_PATCH);
#endif

#ifdef TRUSSTABLES_VERSION_LABEL
    preReleaseVersion = tr("-%1").arg(tr(TRUSSTABLES_VERSION_LABEL));
#endif

    QString version(tr("%1.%2.%3%4").arg(majorVersion).arg(minorVersion).arg(patchVersion).arg(preReleaseVersion));
    ui->applicationVersionLabel->setText(version);
    ui->applicationShortDescriptionLabel->setText(tr("A 2D Truss Solver"));

    connect(ui->closePushButton, SIGNAL(clicked()), this, SLOT(accept()));
}

AboutTrussTablesDialog::~AboutTrussTablesDialog()
{
    delete ui;
}
