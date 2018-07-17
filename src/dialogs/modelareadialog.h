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

/* modelareadialog.h */

#ifndef MODELAREADIALOG_H
#define MODELAREADIALOG_H

#include <QCloseEvent>
#include <QDialog>

namespace Ui
{
    class ModelAreaDialog;
}

class ModelAreaDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ModelAreaDialog(QWidget *parent = 0);

        ~ModelAreaDialog();

        QPoint position() const;

        int widthFactor();
        void setWidthFactor(int widthFactor);
        int heightFactor();
        void setHeightFactor(int heightFactor);
        int margin();
        void setMargin(int margin);

    signals:
        void inputsSignal(int w, int h, int margin);

    public slots:
        void setMarginValueLabelText(int value);
        void setModelArea();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::ModelAreaDialog *ui;
        QPoint              mPoint;
};

#endif // MODELAREADIALOG_H
