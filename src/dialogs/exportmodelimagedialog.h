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

/* exportmodelimagedialog.h */

#ifndef EXPORTMODELIMAGEDIALOG_H
#define EXPORTMODELIMAGEDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QFileDialog>

namespace Ui
{
    class ExportModelImageDialog;
}

class ExportModelImageDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ExportModelImageDialog(QPixmap *pixmap                       = 0,
                                        QString *lastExportModelImageLocation = 0,
                                        QWidget *parent                       = 0);

        ~ExportModelImageDialog();

        QPoint position() const;

    public slots:
        void setQualityValueLabelText(int value);
        void exportImage();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::ExportModelImageDialog *ui;
        QPixmap                    *mPixmap;
        QString                    *mLastExportModelImageLocation;
        QPoint                     mPoint;
};

#endif // EXPORTMODELIMAGEDIALOG_H
