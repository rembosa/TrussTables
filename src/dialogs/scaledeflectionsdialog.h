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

/* scaledeflectionsdialog.h */

#ifndef SCALEDEFLECTIONSDIALOG_H
#define SCALEDEFLECTIONSDIALOG_H

#include <QCloseEvent>
#include <QDialog>

namespace Ui
{
    class ScaleDeflectionsDialog;
}

class ScaleDeflectionsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ScaleDeflectionsDialog(QWidget *parent = 0);

        ~ScaleDeflectionsDialog();

        QPoint position() const;

        qreal deflectionFactor();
        void setDeflectionFactor(qreal deflectionFactor);

    signals:
        void factorSignal(qreal factor);

    public slots:
        void onFactorChanged();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::ScaleDeflectionsDialog *ui;
        QPoint                     mPoint;
};

#endif // SCALEDEFLECTIONSDIALOG_H
