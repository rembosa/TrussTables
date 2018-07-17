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

/* exportmodelimagedialog.cpp */

#include "exportmodelimagedialog.h"
#include "ui_exportmodelimagedialog.h"

ExportModelImageDialog::ExportModelImageDialog(QPixmap *pixmap,
                                               QString *lastExportModelImageLocation,
                                               QWidget *parent) : QDialog(parent),
    ui(new Ui::ExportModelImageDialog)
{
    ui->setupUi(this);
    mPixmap                       = pixmap;
    mLastExportModelImageLocation = lastExportModelImageLocation;
    mPoint                        = QPoint(0, 0);

    ui->formatComboBox->addItem(tr("BMP"));
    ui->formatComboBox->addItem(tr("JPEG"));
    ui->formatComboBox->addItem(tr("PNG"));
    ui->formatComboBox->addItem(tr("PPM"));
    ui->formatComboBox->setCurrentIndex(0);

    connect(ui->qualityHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setQualityValueLabelText(int)));
    connect(ui->applyPushButton, SIGNAL(clicked()), this, SLOT(exportImage()));
}

ExportModelImageDialog::~ExportModelImageDialog()
{
    delete ui;
}

QPoint ExportModelImageDialog::position() const
{
    return mPoint;
}

void ExportModelImageDialog::setQualityValueLabelText(int value)
{
    ui->qualityValueLabel->setText(QString::number(value));
}

void ExportModelImageDialog::exportImage()
{
    int index = ui->formatComboBox->currentIndex();

    QString filter;
    QString directory;

    if (index == 0)
    {
        directory = *mLastExportModelImageLocation + tr("/untitled.bmp");
        filter    = tr("BMP image(*.bmp)");
    }
    else if (index == 1)
    {
        directory = *mLastExportModelImageLocation + tr("/untitled.jpeg");
        filter    = tr("JPEG image(*.jpeg)");
    }
    else if (index == 2)
    {
        directory = *mLastExportModelImageLocation + tr("/untitled.png");
        filter    = tr("PNG image(*.png)");
    }
    else
    {
        directory = *mLastExportModelImageLocation + tr("/untitled.ppm");
        filter    = tr("PPM image(*.ppm)");
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export File"),
                                                    directory,
                                                    filter);
    int quality = ui->qualityHorizontalSlider->value();

    const char *format = 0;

    if (!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        *mLastExportModelImageLocation = fileInfo.dir().path();
        mPixmap->save(fileName, format, quality);
    }
}

void ExportModelImageDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
