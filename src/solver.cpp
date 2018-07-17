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

/* solver.cpp */

#include "solver.h"

Solver::Solver(QWidget *parent) : QMainWindow(parent)
{
    initializeUiComponents();

    mNotesDialog = new NotesDialog(this);
    mModelAreaDialog = new ModelAreaDialog(this);
    mExportModelImageDialog = new ExportModelImageDialog(mModelViewer->modelPixmap(),
                                                         &mLastExportModelImageLocation,
                                                         this);

    connect(mModelAreaDialog, SIGNAL(inputsSignal(int,int,int)), mScrollArea, SLOT(setFactors(int,int,int)));
    mAddJointsDialog = new AddJointsDialog(&mUnitsAndLimits,
                                           &mJointsList,
                                           mJointsTableView,
                                           this);
    mEditJointsDialog = new EditJointsDialog(&mUnitsAndLimits,
                                             mJointsTableView,
                                             this);
    mDeleteJointsDialog = new DeleteJointsDialog(mJointsTableView, this);

    mModelCheckRequired            = false;
    mUpdateJointLoadOption         = false;
    mUpdateSelfWeightOption        = false;
    mUpdateSupportSettlementOption = false;
    mUpdateThermalEffectOption     = false;
    mUpdateFabricationErrorOption  = false;
    mUpdateInfluenceLoadOption     = false;
    mIsStable                      = false;
    mHasSolution                   = false;
    mSolutionInfluenceLoadName     = QString();
    mLastOpenLocation              = QDir::toNativeSeparators(QDir::homePath());
    mLastSaveLocation              = QDir::toNativeSeparators(QDir::homePath());
    mLastExportModelImageLocation  = QDir::toNativeSeparators(QDir::homePath());
    mLastReportLocation            = QDir::toNativeSeparators(QDir::homePath());

    mAddBarsDialog  = new AddBarsDialog(&mUnitsAndLimits,
                                       &mJointsList,
                                       &mBarsList,
                                       mJointsTableView,
                                       mBarsTableView,
                                       this);
    mEditBarsDialog = new EditBarsDialog(&mUnitsAndLimits,
                                         &mJointsList,
                                         &mBarsList,
                                         mBarsTableView,
                                         this);
    mDeleteBarsDialog = new DeleteBarsDialog(mBarsTableView, this);

    mAddSupportsDialog = new AddSupportsDialog(&mJointsList,
                                               mJointsTableView,
                                               this);
    mEditSupportsDialog = new EditSupportsDialog(&mJointsList,
                                                 &mSupportsList,
                                                 mSupportsTableView,
                                                 this);
    mDeleteSupportsDialog = new DeleteSupportsDialog(&mJointsList,
                                                     &mSupportsList,
                                                     mSupportsTableView,
                                                     this);
    mScaleForcesDialog = new ScaleForcesDialog(this);

    mAddJointLoadsDialog = new AddJointLoadsDialog(&mUnitsAndLimits,
                                                   &mJointsList,
                                                   mJointsTableView,
                                                   this);

    mEditJointLoadsDialog = new EditJointLoadsDialog(&mUnitsAndLimits,
                                                     &mJointsList,
                                                     &mJointLoadsList,
                                                     mLoadsTableView,
                                                     this);

    mDeleteJointLoadsDialog = new DeleteJointLoadsDialog(&mJointsList,
                                                         &mJointLoadsList,
                                                         mLoadsTableView,
                                                         this);

    mAddSupportSettlementsDialog = new AddSupportSettlementsDialog(&mUnitsAndLimits,
                                                                   &mJointsList,
                                                                   &mSupportsList,
                                                                   &mRedundantSupportsList,
                                                                   this);

    mEditSupportSettlementsDialog = new EditSupportSettlementsDialog(&mUnitsAndLimits,
                                                                     &mJointsList,
                                                                     &mSupportsList,
                                                                     &mSupportSettlementsList,
                                                                     mLoadsTableView,
                                                                     &mRedundantSupportsList,
                                                                     this);

    mDeleteSupportSettlementsDialog = new DeleteSupportSettlementsDialog(&mJointsList,
                                                                         &mSupportSettlementsList,
                                                                         mLoadsTableView,
                                                                         this);

    mAddThermalEffectsDialog = new AddThermalEffectsDialog(&mUnitsAndLimits,
                                                           &mBarsList,
                                                           &mRedundantBarsList,
                                                           mBarsTableView,
                                                           this);

    mEditThermalEffectsDialog = new EditThermalEffectsDialog(&mUnitsAndLimits,
                                                             &mBarsList,
                                                             &mThermalEffectsList,
                                                             mLoadsTableView,
                                                             &mRedundantBarsList,
                                                             this);

    mDeleteThermalEffectsDialog = new DeleteThermalEffectsDialog(&mBarsList,
                                                                 &mThermalEffectsList,
                                                                 mLoadsTableView,
                                                                 this);

    mAddFabricationErrorsDialog = new AddFabricationErrorsDialog(&mUnitsAndLimits,
                                                                 &mBarsList,
                                                                 &mRedundantBarsList,
                                                                 mBarsTableView,
                                                                 this);

    mEditFabricationErrorsDialog = new EditFabricationErrorsDialog(&mUnitsAndLimits,
                                                                   &mBarsList,
                                                                   &mFabricationErrorsList,
                                                                   mLoadsTableView,
                                                                   &mRedundantBarsList,
                                                                   this);

    mDeleteFabricationErrorsDialog = new DeleteFabricationErrorsDialog(&mBarsList,
                                                                       &mFabricationErrorsList,
                                                                       mLoadsTableView,
                                                                       this);

    mAddInfluenceLoadsDialog = new AddInfluenceLoadsDialog(&mUnitsAndLimits,
                                                           &mInfluenceLoadsList,
                                                           &mJointsList,
                                                           mJointsTableView,
                                                           this);

    mEditInfluenceLoadsDialog = new EditInfluenceLoadsDialog(&mUnitsAndLimits,
                                                             &mInfluenceLoadsList,
                                                             &mJointsList,
                                                             mJointsTableView,
                                                             mLoadsTableView,
                                                             this);

    mDeleteInfluenceLoadsDialog = new DeleteInfluenceLoadsDialog(mLoadsTableView, this);

    mScaleDeflectionsDialog = new ScaleDeflectionsDialog(this);

    connect(mModelViewer, SIGNAL(positionSignal(QString&)), this, SLOT(setCoordinatesLabelText(QString&)));
    connect(mUnitsAction, SIGNAL(triggered()), this, SLOT(setUnits()));
    connect(mHelpAction, SIGNAL(triggered()), this, SLOT(showHelp()));
    connect(mAboutTrussTablesAction, SIGNAL(triggered()), this, SLOT(showAboutTrussTablesDialog()));
    connect(mAboutQtAction, SIGNAL(triggered()), this, SLOT(showAboutQtDialog()));
    connect(mSelectionPanelAction, SIGNAL(triggered()), this, SLOT(showSelectionPanel()));
    connect(mShowNotesAction, SIGNAL(triggered()), this, SLOT(showNotes()));
    connect(mSetModelAreaAction, SIGNAL(triggered()), this, SLOT(showModelAreaDialog()));
    connect(mExportModelImageAction, SIGNAL(triggered()), this, SLOT(showExportModelImageDialog()));
    connect(mShowHideAxesAction, SIGNAL(toggled(bool)), mModelViewer, SLOT(showHideAxes(bool)));
    connect(mShowHideJointNumberAction, SIGNAL(toggled(bool)), mModelViewer, SLOT(showHideJointNumber(bool)));
    connect(mShowHideBarNumberAction, SIGNAL(toggled(bool)), mModelViewer, SLOT(showHideBarNumber(bool)));
    connect(mScaleForcesAction, SIGNAL(triggered()), this, SLOT(showScaleForcesDialog()));
    connect(mScaleForcesDialog, SIGNAL(factorSignal(qreal)), mModelViewer, SLOT(updateForceScaleFactor(qreal)));
    connect(mScaleDeflectionsAction, SIGNAL(triggered()), this, SLOT(showScaleDeflectionsDialog()));
    connect(mScaleDeflectionsDialog, SIGNAL(factorSignal(qreal)), mModelViewer,
            SLOT(updateDeflectionScaleFactor(qreal)));

    connect(mAddJointsPushButton, SIGNAL(pressed()), this, SLOT(showAddJointsDialog()));
    connect(mEditJointsPushButton, SIGNAL(pressed()), this, SLOT(showEditJointsDialog()));
    connect(mDeleteJointsPushButton, SIGNAL(pressed()), this, SLOT(showDeleteJointsDialog()));
    connect(mJointsSingleRadioButton, SIGNAL(toggled(bool)), this, SLOT(setJointsSelectionMode(bool)));
    connect(mJointsClearToolButton, SIGNAL(clicked()), mJointsTableView, SLOT(clearSelection()));
    connect(this, SIGNAL(enableJointsInput()), mAddJointsDialog, SLOT(setEnableInput()));
    connect(this, SIGNAL(disableJointsInput()), mAddJointsDialog, SLOT(setDisableInput()));
    connect(mAddJointsDialog, SIGNAL(inputsSignal(qreal,qreal,int)), this, SLOT(addJoints(qreal,qreal,int)));
    connect(mAddJointsDialog, SIGNAL(inputsSignal(QString,int)), this, SLOT(mirrorJoints(QString,int)));
    connect(mJointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mAddJointsDialog, SLOT(setSelectedJoints(QItemSelection,QItemSelection)));
    connect(mEditJointsDialog, SIGNAL(inputsSignal(qreal,qreal)), this, SLOT(editJoints(qreal,qreal)));
    connect(mJointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mEditJointsDialog, SLOT(setSelectedJoints(QItemSelection,QItemSelection)));
    connect(this, SIGNAL(updateModelSignal()), mModelViewer, SLOT(updateModel()));
    connect(mDeleteJointsDialog, SIGNAL(inputsSignal()), this, SLOT(deleteJoints()));
    connect(mJointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mDeleteJointsDialog, SLOT(setSelectedJoints(QItemSelection,QItemSelection)));

    connect(mAddBarsPushButton, SIGNAL(pressed()), this, SLOT(showAddBarsDialog()));
    connect(mEditBarsPushButton, SIGNAL(pressed()), this, SLOT(showEditBarsDialog()));
    connect(mDeleteBarsPushButton, SIGNAL(pressed()), this, SLOT(showDeleteBarsDialog()));
    connect(mBarsSingleRadioButton, SIGNAL(toggled(bool)), this, SLOT(setBarsSelectionMode(bool)));
    connect(mBarsClearToolButton, SIGNAL(clicked()), mBarsTableView, SLOT(clearSelection()));
    connect(mFactorRadioButton, SIGNAL(toggled(bool)), this, SLOT(setFactorOption(bool)));
    connect(mValueRadioButton, SIGNAL(toggled(bool)), this, SLOT(setAreaModulusOption(bool)));
    connect(mJointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mAddBarsDialog, SLOT(setSelectedJoints(QItemSelection,QItemSelection)));
    connect(mBarsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mAddBarsDialog, SLOT(setSelectedBars(QItemSelection,QItemSelection)));
    connect(mAddBarsDialog, SIGNAL(inputsSignal(int,int,qreal,qreal,qreal,qreal,int,qreal,qreal)),
            this, SLOT(addBars(int,int,qreal,qreal,qreal,qreal,int,qreal,qreal)));
    connect(mAddBarsDialog, SIGNAL(inputsSignal(QString,int)), this, SLOT(mirrorBars(QString,int)));
    connect(mAddBarsDialog, SIGNAL(inputsSignal(qreal,qreal,qreal,qreal)),
            this, SLOT(linkBars(qreal,qreal,qreal,qreal)));
    connect(mBarsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mEditBarsDialog, SLOT(setSelectedBars(QItemSelection,QItemSelection)));
    connect(mEditBarsDialog, SIGNAL(inputsSignal(int,int,qreal,qreal,qreal,qreal)),
            this, SLOT(editBar(int,int,qreal,qreal,qreal,qreal)));
    connect(mBarsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mDeleteBarsDialog, SLOT(setSelectedBars(QItemSelection,QItemSelection)));
    connect(mDeleteBarsDialog, SIGNAL(inputsSignal()), this, SLOT(deleteBar()));

    connect(mAddSupportsPushButton, SIGNAL(pressed()), this, SLOT(showAddSupportsDialog()));
    connect(mEditSupportsPushButton, SIGNAL(pressed()), this, SLOT(showEditSupportsDialog()));
    connect(mDeleteSupportsPushButton, SIGNAL(pressed()), this, SLOT(showDeleteSupportsDialog()));
    connect(mSupportsSingleRadioButton, SIGNAL(toggled(bool)), this, SLOT(setSupportsSelectionMode(bool)));
    connect(mSupportsClearToolButton, SIGNAL(clicked()), mSupportsTableView, SLOT(clearSelection()));
    connect(mJointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mAddSupportsDialog, SLOT(setSelectedJoints(QItemSelection,QItemSelection)));
    connect(mAddSupportsDialog, SIGNAL(inputsSignal(int,UnitsAndLimits::SupportType,qreal)), this,
            SLOT(addSupports(int,UnitsAndLimits::SupportType,qreal)));
    connect(mSupportsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mEditSupportsDialog, SLOT(setSelectedSupports(QItemSelection,QItemSelection)));
    connect(mEditSupportsDialog, SIGNAL(inputsSignal(int,UnitsAndLimits::SupportType,qreal)),
            this, SLOT(editSupport(int,UnitsAndLimits::SupportType,qreal)));
    connect(mSupportsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mDeleteSupportsDialog, SLOT(setSelectedBars(QItemSelection,QItemSelection)));
    connect(mDeleteSupportsDialog, SIGNAL(inputsSignal()), this, SLOT(deleteSupport()));

    connect(mSelectionTabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(onTabIndexChanged(int)));
    connect(mCheckModelAction, SIGNAL(triggered()), this, SLOT(onActionCheckModelTriggered()));
    connect(mLoadTypeListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(setLoadsTableView(int)));
    connect(mLoadTypeListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(setLoadDisplayOption(int)));
    connect(mYesRadioButton, SIGNAL(toggled(bool)), this, SLOT(setLoadDisplayOption(bool)));
    connect(mLoadsSingleRadioButton, SIGNAL(toggled(bool)), this, SLOT(setLoadsSelectionMode(bool)));
    connect(mLoadsClearToolButton, SIGNAL(clicked()), mLoadsTableView, SLOT(clearSelection()));
    connect(this, SIGNAL(modelLoadSignal(ModelViewer::LoadDisplayOption)), mModelViewer,
            SLOT(updateLoadDisplayOption(ModelViewer::LoadDisplayOption)));

    connect(mAddLoadPushButton, SIGNAL(pressed()), this, SLOT(showAddLoadsDialog()));
    connect(mEditLoadPushButton, SIGNAL(pressed()), this, SLOT(showEditLoadsDialog()));
    connect(mDeleteLoadPushButton, SIGNAL(pressed()), this, SLOT(showDeleteLoadsDialog()));
    connect(mJointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mAddJointLoadsDialog, SLOT(setSelectedJoints(QItemSelection,QItemSelection)));
    connect(mAddJointLoadsDialog, SIGNAL(inputsSignal(int,qreal,qreal,QString)), this,
            SLOT(addJointLoad(int,qreal,qreal,QString)));
    connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mEditJointLoadsDialog, SLOT(setSelectedJointLoads(QItemSelection,QItemSelection)));
    connect(mEditJointLoadsDialog, SIGNAL(inputsSignal(int,qreal,qreal,QString)), this,
            SLOT(editJointLoad(int,qreal,qreal,QString)));
    connect(mDeleteJointLoadsDialog, SIGNAL(inputsSignal()), this, SLOT(deleteJointLoad()));
    connect(mAddSupportSettlementsDialog, SIGNAL(inputsSignal(int,qreal)), this, SLOT(addSupportSettlement(int,qreal)));
    connect(mEditSupportSettlementsDialog, SIGNAL(inputsSignal(int,qreal)), this,
            SLOT(editSupportSettlement(int,qreal)));
    connect(mDeleteSupportSettlementsDialog, SIGNAL(inputsSignal()), this, SLOT(deleteSupportSettlement()));
    connect(mBarsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mAddThermalEffectsDialog, SLOT(setSelectedBars(QItemSelection,QItemSelection)));
    connect(mAddThermalEffectsDialog, SIGNAL(inputsSignal(int,qreal,qreal)), this,
            SLOT(addThermalEffect(int,qreal,qreal)));
    connect(mEditThermalEffectsDialog, SIGNAL(inputsSignal(int,qreal,qreal)), this,
            SLOT(editThermalEffect(int,qreal,qreal)));
    connect(mDeleteThermalEffectsDialog, SIGNAL(inputsSignal()), this, SLOT(deleteThermalEffect()));
    connect(mBarsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mAddFabricationErrorsDialog, SLOT(setSelectedBars(QItemSelection,QItemSelection)));
    connect(mAddFabricationErrorsDialog, SIGNAL(inputsSignal(int,qreal)), this, SLOT(addFabricationError(int,qreal)));
    connect(mEditFabricationErrorsDialog, SIGNAL(inputsSignal(int,qreal)), this, SLOT(editFabricationError(int,qreal)));
    connect(mDeleteFabricationErrorsDialog, SIGNAL(inputsSignal()), this, SLOT(deleteFabricationError()));
    connect(mJointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mAddInfluenceLoadsDialog, SLOT(setSelectedJoints(QItemSelection,QItemSelection)));
    connect(mAddInfluenceLoadsDialog, SIGNAL(inputsSignal(QString,QString,QList<qreal>,QList<qreal>)), this,
            SLOT(addInfluenceLoad(QString,QString,QList<qreal>,QList<qreal>)));
    connect(mJointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            mEditInfluenceLoadsDialog, SLOT(setSelectedJoints(QItemSelection,QItemSelection)));
    connect(mEditInfluenceLoadsDialog, SIGNAL(inputsSignal(QString,QString,QList<qreal>,QList<qreal>)), this,
            SLOT(editInfluenceLoad(QString,QString,QList<qreal>,QList<qreal>)));
    connect(mDeleteInfluenceLoadsDialog, SIGNAL(inputsSignal()), this, SLOT(deleteInfluenceLoad()));
    connect(mSolutionOptionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setInfluenceLoadBarOptions(int)));
    connect(mSolutionOptionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSolutionTableView(int)));
    connect(mSolutionOptionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSolutionDisplayOptionA(int)));
    connect(mInfluenceLoadBarComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSolutionDisplayOptionB(int)));
    connect(this, SIGNAL(modelSolutionSignal(ModelViewer::SolutionDisplayOption,int)), mModelViewer,
            SLOT(updateSolutionDisplayOption(ModelViewer::SolutionDisplayOption,int)));
    connect(mSolvePushButton, SIGNAL(clicked()), this, SLOT(solveModel()));
    connect(mClearPushButton, SIGNAL(clicked()), this, SLOT(clearSolution()));
    connect(mReportPushButton, SIGNAL(clicked()), this, SLOT(reportSolution()));
    connect(mNewAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(mOpenAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(mClearMenuAction, SIGNAL(triggered()), this, SLOT(clearRecentFileActions()));
    connect(mSaveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(mSaveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(mExitAction, SIGNAL(triggered()), this, SLOT(close()));

    readSettings();
    updateRecentFileActions();
    setCurrentFile(tr(""));
}

Solver::~Solver()
{

}

bool Solver::loadFile(const QString &fileName)
{
    QFile openFile(fileName);

    if (!openFile.open(QFile::ReadOnly))
    {
        QMessageBox messageBox;
        messageBox.setText(tr("File not found."));
        messageBox.exec();
        return false;
    }

    QDataStream in(&openFile);
    in.setVersion(QDataStream::Qt_5_4);

    quint32 magic;
    in >> magic;
    QString version;
    in >> version;

    if (magic != quint32(0x25438F7BEA4))
    {
        QString s1 = tr("TrussTables");
        QString s2 = tr("The file is not a valid TrussTables model file.");
        QMessageBox::warning(this, s1, s2);
        openFile.close();
        return false;
    }

    int tabIndex = -1;

    //Description and Units

    {
        QString s;

        in >> s;
        mDescriptionText = s;
        in >> s;
        mUnitsAndLimits.setSystem(s);
        in >> s;
        mUnitsAndLimits.setCoordinateUnit(s);
        in >> s;
        mUnitsAndLimits.setAreaUnit(s);
        in >> s;
        mUnitsAndLimits.setModulusUnit(s);
        in >> s;
        mUnitsAndLimits.setUnitWeightUnit(s);
        in >> s;
        mUnitsAndLimits.setLoadUnit(s);
        in >> s;
        mUnitsAndLimits.setSupportSettlementUnit(s);
        in >> s;
        mUnitsAndLimits.setTemperatureChangeUnit(s);
        in >> s;
        mUnitsAndLimits.setThermalCoefficientUnit(s);
        in >> s;
        mUnitsAndLimits.setLengthErrorUnit(s);

        updateUnits();
    }

    //Joint Coordinates

    {
        QList<qreal> xCoordinateList;
        QList<qreal> yCoordinateList;

        in >> xCoordinateList;
        in >> yCoordinateList;

        if (!xCoordinateList.isEmpty())
        {
            tabIndex += 1;

            int count = xCoordinateList.size();
            mJointsList.clear();
            emit enableJointsInput();
            mUnitsAction->setEnabled(false);

            mJointsTableView->clearSelection();
            mJointsStandardItemModel->clear();
            mJointsStandardItemModel->setRowCount(0);
            mJointsStandardItemModel->setColumnCount(2);
            createJointsModelHeader();

            for (int i = 0; i < count; ++i)
            {
                Joint *joint = new Joint(xCoordinateList.at(i), yCoordinateList.at(i), this);
                mJointsList.append(joint);

                QList<QStandardItem *> standardItemsList;
                QStandardItem *standardItem;

                QString s;

                s = QString::number(joint->xCoordinate(), 'f', mUnitsAndLimits.deltaXDecimals());
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                s = QString::number(joint->yCoordinate(), 'f', mUnitsAndLimits.deltaYDecimals());
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                mJointsStandardItemModel->appendRow(standardItemsList);
            }

            if (mJointsSingleRadioButton->isChecked())
            {
                mJointsTableView->clearSelection();
                mJointsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            else
            {
                mJointsTableView->clearSelection();
                mJointsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            }

            mAddJointsDialog->updateJoints();
            mAddBarsDialog->updateFirstJointOptions();
            mEditBarsDialog->updateFirstJointOptions();
            mAddSupportsDialog->updateJoints();
            mAddJointLoadsDialog->updateJoints();
            mEditJointLoadsDialog->updateJoints();
            mModelCheckRequired = true;
        }
    }

    //Bars

    {
        //Axial Rigidity Option

        QString axialRigidityOption;
        in >> axialRigidityOption;

        QList<qint32> firstJointList;
        QList<qint32> secondJointList;
        QList<qreal> areaList;
        QList<qreal> modulusList;
        QList<qreal> factorList;
        QList<qreal> unitWeightList;

        in >> firstJointList;
        in >> secondJointList;
        in >> areaList;
        in >> modulusList;
        in >> factorList;
        in >> unitWeightList;

        if (!firstJointList.isEmpty())
        {
            tabIndex += 1;

            if (axialRigidityOption == tr("value"))
            {
                mValueRadioButton->setChecked(true);
                mFactorRadioButton->setChecked(false);
            }
            else
            {
                mFactorRadioButton->setChecked(true);
                mValueRadioButton->setChecked(false);
            }

            int count = firstJointList.size();
            mBarsList.clear();

            mBarsTableView->clearSelection();
            mBarsStandardItemModel->clear();
            mBarsStandardItemModel->setRowCount(0);

            if (axialRigidityOption == tr("value"))
            {
                mBarsStandardItemModel->setColumnCount(5);
            }
            else
            {
                mBarsStandardItemModel->setColumnCount(3);
            }

            createBarsModelHeader();

            for (int i = 0; i < count; ++i)
            {
                if (mBarsList.size() == 0)
                {
                    mFactorRadioButton->setEnabled(false);
                    mValueRadioButton->setEnabled(false);
                }

                int jointNumberA = int(firstJointList.at(i));
                int jointNumberB = int(secondJointList.at(i));
                Joint *jointA    = mJointsList.at(jointNumberA - 1);
                Joint *jointB    = mJointsList.at(jointNumberB - 1);
                qreal area       = areaList.at(i);
                qreal modulus    = modulusList.at(i);
                qreal factor     = factorList.at(i);
                qreal unitWeight = unitWeightList.at(i);

                jointA->connectedJoints()->append(jointB);
                jointB->connectedJoints()->append(jointA);

                Bar *bar = new Bar(jointA, jointB, area, modulus, factor, unitWeight, this);
                mBarsList.append(bar);

                jointA->attachedBars()->append(bar);
                jointB->attachedBars()->append(bar);

                QList<QStandardItem *> standardItemsList;
                QStandardItem *standardItem;

                standardItem = new QStandardItem(QString::number(mJointsList.indexOf(jointA) + 1));
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                standardItem = new QStandardItem(QString::number(mJointsList.indexOf(jointB) + 1));
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                if (mBarsStandardItemModel->columnCount() == 5)
                {
                    QString s;

                    s = QString::number(bar->area(), 'f', mUnitsAndLimits.areaDecimals());
                    standardItem = new QStandardItem(s);
                    standardItem->setTextAlignment(Qt::AlignCenter);
                    standardItem->setEditable(false);
                    standardItemsList.append(standardItem);

                    s = QString::number(bar->modulus(), 'f', mUnitsAndLimits.modulusDecimals());
                    standardItem = new QStandardItem(s);
                    standardItem->setTextAlignment(Qt::AlignCenter);
                    standardItem->setEditable(false);
                    standardItemsList.append(standardItem);

                    s = QString::number(bar->unitWeight(), 'f', mUnitsAndLimits.unitWeightDecimals());
                    standardItem = new QStandardItem(s);
                    standardItem->setTextAlignment(Qt::AlignCenter);
                    standardItem->setEditable(false);
                    standardItemsList.append(standardItem);
                }
                else
                {
                    QString s;

                    s = QString::number(bar->factor(), 'f', mUnitsAndLimits.factorDecimals());
                    standardItem = new QStandardItem(s);
                    standardItem->setTextAlignment(Qt::AlignCenter);
                    standardItem->setEditable(false);
                    standardItemsList.append(standardItem);
                }

                mBarsStandardItemModel->appendRow(standardItemsList);
            }

            if (mBarsSingleRadioButton->isChecked())
            {
                mBarsTableView->clearSelection();
                mBarsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            else
            {
                mBarsTableView->clearSelection();
                mBarsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            }

            mModelCheckRequired = true;
        }
    }

    //Supports

    {
        QList<qint32> supportJointList;
        QList<qint32> supportTypeList;
        QList<qreal> supportAngleList;

        in >> supportJointList;
        in >> supportTypeList;
        in >> supportAngleList;

        if (!supportJointList.isEmpty())
        {
            tabIndex += 1;

            int count = supportJointList.size();
            mSupportsList.clear();

            mSupportsTableView->clearSelection();
            mSupportsStandardItemModel->clear();
            mSupportsStandardItemModel->setRowCount(0);
            mSupportsStandardItemModel->setColumnCount(3);
            createSupportsModelHeader();

            for (int i = 0; i < count; ++i)
            {
                int jointNumber = int(supportJointList.at(i));
                Joint *joint    = mJointsList.at(jointNumber - 1);
                qint32 type     = supportTypeList.at(i);
                qreal angle     = supportAngleList.at(i);

                UnitsAndLimits::SupportType supportType = UnitsAndLimits::FIXED_BOTTOM;
                QString supportTypeName;

                switch (type)
                {
                    case 0:
                        supportType     = UnitsAndLimits::FIXED_LEFT;
                        supportTypeName = tr("fixed");
                        break;
                    case 1:
                        supportType     = UnitsAndLimits::FIXED_TOP;
                        supportTypeName = tr("fixed");
                        break;
                    case 2:
                        supportType     = UnitsAndLimits::FIXED_RIGHT;
                        supportTypeName = tr("fixed");
                        break;
                    case 3:
                        supportType     = UnitsAndLimits::FIXED_BOTTOM;
                        supportTypeName = tr("fixed");
                        break;
                    case 4:
                        supportType     = UnitsAndLimits::ROLLER;
                        supportTypeName = tr("inclined roller");
                        break;
                    case 5:
                        supportType     = UnitsAndLimits::ROLLER_LEFT;
                        supportTypeName = tr("roller");
                        break;
                    case 6:
                        supportType     = UnitsAndLimits::ROLLER_TOP;
                        supportTypeName = tr("roller");
                        break;
                    case 7:
                        supportType     = UnitsAndLimits::ROLLER_RIGHT;
                        supportTypeName = tr("roller");
                        break;
                    case 8:
                        supportType     = UnitsAndLimits::ROLLER_BOTTOM;
                        supportTypeName = tr("roller");
                        break;
                    default:
                        break;
                }

                Support *support = new Support(supportType, angle, joint, this);

                joint->setSupported(true);

                mSupportsList.append(support);

                QList<QStandardItem *> standardItemsList;
                QStandardItem *standardItem;

                standardItem = new QStandardItem(QString::number(jointNumber));
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                standardItem = new QStandardItem(supportTypeName);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                standardItem = new QStandardItem(QString::number(angle, 'f', mUnitsAndLimits.supportAngleDecimals()));
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                mSupportsStandardItemModel->appendRow(standardItemsList);
            }

            if (mSupportsSingleRadioButton->isChecked())
            {
                mSupportsTableView->clearSelection();
                mSupportsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            else
            {
                mSupportsTableView->clearSelection();
                mSupportsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            }

            mModelCheckRequired = true;
        }
    }

    if (tabIndex >= 0)
    {
        mSelectionTabWidget->setCurrentIndex(tabIndex);
        updateModelViewer();
    }

    bool checkHasLoads;
    in >> checkHasLoads;

    if (checkHasLoads)
    {
        mSelectionTabWidget->setCurrentIndex(3);
        checkModel();
    }

    //Joint Loads

    {
        QList<qint32> loadJointList;
        QList<qreal> loadHorizontalComponentList;
        QList<qreal> loadVerticalComponentList;
        QList<QString> loadPositionList;

        in >> loadJointList;
        in >> loadHorizontalComponentList;
        in >> loadVerticalComponentList;
        in >> loadPositionList;

        if (!loadJointList.isEmpty())
        {
            int count = loadJointList.size();
            mJointLoadsList.clear();

            mLoadsTableView->clearSelection();
            mJointLoadsStandardItemModel->clear();
            mJointLoadsStandardItemModel->setRowCount(0);
            mJointLoadsStandardItemModel->setColumnCount(3);
            createJointLoadsModelHeader();

            for (int i = 0; i < count; ++i)
            {
                int jointNumber  = int(loadJointList.at(i));
                Joint *joint     = mJointsList.at(jointNumber - 1);
                qreal H          = loadHorizontalComponentList.at(i);
                qreal V          = loadVerticalComponentList.at(i);
                QString position = loadPositionList.at(i);

                JointLoad *load = new JointLoad(joint, H, V, position, this);

                if (mJointLoadsList.isEmpty())
                {
                    mUpdateJointLoadOption = true;
                }

                mJointLoadsList.append(load);

                QList<QStandardItem *> standardItemsList;
                QStandardItem *standardItem;

                QString s;

                s = QString::number(jointNumber);
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                s = QString::number(H, 'f', mUnitsAndLimits.jointLoadDecimals());
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                s = QString::number(V, 'f', mUnitsAndLimits.jointLoadDecimals());
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                mJointLoadsStandardItemModel->appendRow(standardItemsList);
            }

            if (mLoadsSingleRadioButton->isChecked())
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            else
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            }
        }
    }

    //Support Settlements

    {
        QList<qint32> settlementSupportList;
        QList<qreal> settlementList;

        in >> settlementSupportList;
        in >> settlementList;

        if (!settlementSupportList.isEmpty())
        {
            int count = settlementSupportList.size();
            mSupportSettlementsList.clear();

            mSupportSettlementsStandardItemModel->clear();
            mSupportSettlementsStandardItemModel->setRowCount(0);
            mSupportSettlementsStandardItemModel->setColumnCount(2);
            createSupportSettlementsModelHeader();

            for (int i = 0; i < count; ++i)
            {
                int supportNumber = int(settlementSupportList.at(i));
                Support *support  = mSupportsList.at(supportNumber - 1);
                qreal settlement  = settlementList.at(i);

                SupportSettlement *supportSettlement = new SupportSettlement(support, settlement, this);

                if (mSupportSettlementsList.isEmpty())
                {
                    mUpdateSupportSettlementOption = true;
                }

                mSupportSettlementsList.append(supportSettlement);

                Joint *joint    = support->supportJoint();
                int jointNumber = mJointsList.indexOf(joint) + 1;

                QList<QStandardItem *> standardItemsList;
                QStandardItem *standardItem;

                QString s;

                s = QString::number(supportNumber) + " @ joint " + QString::number(jointNumber);
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                s = QString::number(settlement, 'f', mUnitsAndLimits.supportSettlementDecimals());
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                mSupportSettlementsStandardItemModel->appendRow(standardItemsList);
            }

            if (mLoadsSingleRadioButton->isChecked())
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            else
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            }
        }
    }

    //Thermal Effects

    {
        QList<qint32> thermalEffectBarList;
        QList<qreal> temperatureChangeList;
        QList<qreal> thermalCoefficientList;

        in >> thermalEffectBarList;
        in >> temperatureChangeList;
        in >> thermalCoefficientList;

        if (!thermalEffectBarList.isEmpty())
        {
            int count = thermalEffectBarList.size();
            mThermalEffectsList.clear();

            mThermalEffectsStandardItemModel->clear();
            mThermalEffectsStandardItemModel->setRowCount(0);
            mThermalEffectsStandardItemModel->setColumnCount(3);
            createThermalEffectsModelHeader();

            for (int i = 0; i < count; ++i)
            {
                int barNumber            = int(thermalEffectBarList.at(i));
                Bar *bar                 = mBarsList.at(barNumber - 1);
                qreal temperatureChange  = temperatureChangeList.at(i);
                qreal thermalCoefficient = thermalCoefficientList.at(i);

                ThermalEffect *thermalEffect = new ThermalEffect(bar, temperatureChange, thermalCoefficient, this);

                if (mThermalEffectsList.isEmpty())
                {
                    mUpdateThermalEffectOption = true;
                }

                mThermalEffectsList.append(thermalEffect);

                QList<QStandardItem *> standardItemsList;
                QStandardItem *standardItem;

                QString s;

                s = QString::number(barNumber);
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                s = QString::number(temperatureChange, 'f', mUnitsAndLimits.temperatureChangeDecimals());
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                s = QString::number(thermalCoefficient, 'g', mUnitsAndLimits.thermalCoefficientDecimals());
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                mThermalEffectsStandardItemModel->appendRow(standardItemsList);
            }

            if (mLoadsSingleRadioButton->isChecked())
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            else
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            }
        }
    }

    //Assembly/Fabrication Errors

    {
        QList<qint32> fabricationErrorBarList;
        QList<qreal> lengthErrorList;

        in >> fabricationErrorBarList;
        in >> lengthErrorList;

        if (!fabricationErrorBarList.isEmpty())
        {
            int count = fabricationErrorBarList.size();
            mFabricationErrorsList.clear();

            mFabricationErrorsStandardItemModel->clear();
            mFabricationErrorsStandardItemModel->setRowCount(0);
            mFabricationErrorsStandardItemModel->setColumnCount(2);
            createFabricationErrorsModelHeader();

            for (int i = 0; i < count; ++i)
            {
                int barNumber     = int(fabricationErrorBarList.at(i));
                Bar *bar          = mBarsList.at(barNumber - 1);
                qreal lengthError = lengthErrorList.at(i);

                FabricationError *fabricationError = new FabricationError(bar, lengthError, this);

                if (mFabricationErrorsList.isEmpty())
                {
                    mUpdateFabricationErrorOption = true;
                }

                mFabricationErrorsList.append(fabricationError);

                QList<QStandardItem *> standardItemsList;
                QStandardItem *standardItem;

                QString s;

                s = QString::number(barNumber);
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                s = QString::number(lengthError, 'f', mUnitsAndLimits.lengthErrorDecimals());
                standardItem = new QStandardItem(s);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                mFabricationErrorsStandardItemModel->appendRow(standardItemsList);
            }

            if (mLoadsSingleRadioButton->isChecked())
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            else
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            }
        }
    }

    //Influence Loads

    {
        int influenceLoadCount;
        in >> influenceLoadCount;

        if (influenceLoadCount > 0)
        {
            mInfluenceLoadsList.clear();
            mInfluenceLoadsStandardItemModel->clear();
            mInfluenceLoadsStandardItemModel->setRowCount(0);
            mInfluenceLoadsStandardItemModel->setColumnCount(4);
            createInfluenceLoadsModelHeader();

            for (int i = 0; i < influenceLoadCount; ++i)
            {
                QString name;
                QString direction;
                QList<qint32> pathList;
                QList<qreal> pointLoads;
                QList<qreal> pointLoadPositions;

                in >> name;
                in >> direction;
                in >> pathList;
                in >> pointLoads;
                in >> pointLoadPositions;

                InfluenceLoad *influenceLoad = new InfluenceLoad(name,
                                                                 direction,
                                                                 pathList,
                                                                 pointLoads,
                                                                 pointLoadPositions,
                                                                 this);

                mUpdateInfluenceLoadOption = true;

                mInfluenceLoadsList.append(influenceLoad);

                QString pathString;
                int count = pathList.size();

                for (int i = 0; i < count; ++i)
                {
                    pathString.append(QString::number(pathList.at(i)));

                    if (i < (count - 1))
                    {
                        pathString.append("\n");
                    }
                }

                QString loadsString;
                count = pointLoads.size();

                for (int i = 0; i < count; ++i)
                {
                    QString s1, s2, s3, s4;

                    s1 = QString::number(pointLoads.at(i), 'f', mUnitsAndLimits.pointLoadDecimals());
                    s2 = mUnitsAndLimits.loadUnit();
                    s3 = QString::number(pointLoadPositions.at(i), 'f', mUnitsAndLimits.offsetDecimals());
                    s4 = mUnitsAndLimits.coordinateUnit();

                    loadsString.append(tr("%1 %2 @ %3 %4").arg(s1).arg(s2).arg(s3).arg(s4));

                    if (i < (count - 1))
                    {
                        loadsString.append("\n");
                    }
                }

                QList<QStandardItem *> standardItemList;
                QStandardItem *standardItem;

                standardItem = new QStandardItem(name);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemList.append(standardItem);

                standardItem = new QStandardItem(direction);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemList.append(standardItem);

                standardItem = new QStandardItem(pathString);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemList.append(standardItem);

                standardItem = new QStandardItem(loadsString);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemList.append(standardItem);

                mInfluenceLoadsStandardItemModel->appendRow(standardItemList);

                mAddInfluenceLoadsDialog->updateDefaultName();
            }

            if (mLoadsSingleRadioButton->isChecked())
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
            }
            else
            {
                mLoadsTableView->clearSelection();
                mLoadsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            }
        }
    }

    openFile.close();
    setCurrentFile(fileName);
    return true;
}

void Solver::closeEvent(QCloseEvent *event)
{
    if (promptForSave())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void Solver::initializeUiComponents()
{

#ifdef Q_OS_LINUX
    setWindowIcon(QIcon(tr(":/icons/tt.png")));
#endif

    mScrollArea = new ScrollArea;

    mBarsStandardItemModel = new QStandardItemModel(this);
    mBarsStandardItemModel->setRowCount(0);
    mBarsStandardItemModel->setColumnCount(3);

    mInfluenceLoadResult = new InfluenceLoadResult(0, 0, this);

    mModelViewer = new ModelViewer(&mUnitsAndLimits,
                                   &mJointsList,
                                   &mBarsList,
                                   &mSupportsList,
                                   &mJointLoadsList,
                                   &mSupportSettlementsList,
                                   &mThermalEffectsList,
                                   &mFabricationErrorsList,
                                   &mInfluenceLoadsList,
                                   mBarsStandardItemModel,
                                   &mHorizontalDeflectionComponentsList,
                                   &mVerticalDeflectionComponentsList,
                                   &mBarLoadsList,
                                   &mReactionHorizontalComponentsList,
                                   &mReactionVerticalComponentsList,
                                   &mSolutionInfluenceLoadName,
                                   mInfluenceLoadResult);

    mScrollArea->setWidget(mModelViewer);
    mModelViewer->resize(mScrollArea->width(), mScrollArea->height());
    setCentralWidget(mScrollArea);

    mProgressBar = new QProgressBar;
    mProgressBar->setMinimum(0);
    mProgressBar->setMaximum(100);
    mProgressBar->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(mProgressBar);
    mProgressBar->setHidden(true);
    mCoordinatesLabel = new QLabel(this);
    statusBar()->addPermanentWidget(mCoordinatesLabel);
    mCoordinatesLabel->setText(tr("X: 0.000 %1 Y: 0.000 %1").arg(mUnitsAndLimits.coordinateUnit()));

    connect(mScrollArea, SIGNAL(sizeSignal(int,int,int)), mModelViewer, SLOT(updateSize(int,int,int)));

    mJointsStandardItemModel = new QStandardItemModel(this);
    mJointsStandardItemModel->setRowCount(0);
    mJointsStandardItemModel->setColumnCount(2);

    mSupportsStandardItemModel = new QStandardItemModel(this);
    mSupportsStandardItemModel->setRowCount(0);
    mSupportsStandardItemModel->setColumnCount(3);

    mJointLoadsStandardItemModel = new QStandardItemModel(this);
    mJointLoadsStandardItemModel->setRowCount(0);
    mJointLoadsStandardItemModel->setColumnCount(3);

    mSupportSettlementsStandardItemModel = new QStandardItemModel(this);
    mSupportSettlementsStandardItemModel->setRowCount(0);
    mSupportSettlementsStandardItemModel->setColumnCount(2);

    mThermalEffectsStandardItemModel = new QStandardItemModel(this);
    mThermalEffectsStandardItemModel->setRowCount(0);
    mThermalEffectsStandardItemModel->setColumnCount(3);

    mFabricationErrorsStandardItemModel = new QStandardItemModel(this);
    mFabricationErrorsStandardItemModel->setRowCount(0);
    mFabricationErrorsStandardItemModel->setColumnCount(2);

    mInfluenceLoadsStandardItemModel = new QStandardItemModel(this);
    mInfluenceLoadsStandardItemModel->setRowCount(0);
    mInfluenceLoadsStandardItemModel->setColumnCount(4);

    mJointDeflectionsStandardItemModel = new QStandardItemModel(this);
    mJointDeflectionsStandardItemModel->setRowCount(0);
    mJointDeflectionsStandardItemModel->setColumnCount(3);

    mBarLoadsStandardItemModel = new QStandardItemModel(this);
    mBarLoadsStandardItemModel->setRowCount(0);
    mBarLoadsStandardItemModel->setColumnCount(2);

    mSupportReactionsStandardItemModel = new QStandardItemModel(this);
    mSupportReactionsStandardItemModel->setRowCount(0);
    mSupportReactionsStandardItemModel->setColumnCount(3);

    mInfluenceLoadResultsStandardItemModel = new QStandardItemModel(this);
    mInfluenceLoadResultsStandardItemModel->setRowCount(0);
    mInfluenceLoadResultsStandardItemModel->setColumnCount(4);

    mSelectionDockWidget = new QDockWidget(tr("Selection"), this);
    mSelectionDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    mSelectionDockWidget->setFloating(false);
    mSelectionTabWidget = new QTabWidget(mSelectionDockWidget);
    mSelectionDockWidget->setWidget(mSelectionTabWidget);
    addDockWidget(Qt::RightDockWidgetArea, mSelectionDockWidget);

    //Joints Tab

    QWidget *jointsPageWidget = new QWidget(mSelectionTabWidget);
    QVBoxLayout *verticalBoxLayoutA1 = new QVBoxLayout(jointsPageWidget);
    mJointsModeGroupBox = new QGroupBox(tr("Mode"), jointsPageWidget);
    QVBoxLayout *verticalBoxLayoutA2 = new QVBoxLayout(mJointsModeGroupBox);
    mJointsSingleRadioButton = new QRadioButton(tr("Single"), mJointsModeGroupBox);
    mJointsSingleRadioButton->setChecked(true);
    mJointsMultipleRadioButton = new QRadioButton(tr("Multiple"), mJointsModeGroupBox);
    mJointsMultipleRadioButton->setChecked(false);
    verticalBoxLayoutA2->addWidget(mJointsSingleRadioButton);
    verticalBoxLayoutA2->addWidget(mJointsMultipleRadioButton);
    QHBoxLayout *horizontalBoxLayoutA1 = new QHBoxLayout;
    horizontalBoxLayoutA1->addWidget(mJointsModeGroupBox);
    horizontalBoxLayoutA1->addStretch();
    verticalBoxLayoutA1->addLayout(horizontalBoxLayoutA1);
    mJointsClearToolButton = new QToolButton(jointsPageWidget);
    mJointsClearToolButton->setIcon(QIcon(tr(":/icons/edit-clear.png")));
    mJointsClearToolButton->setToolTip(tr("Clear Selection"));
    verticalBoxLayoutA1->addWidget(mJointsClearToolButton);
    mAddJointsPushButton = new QPushButton(tr("Add"), jointsPageWidget);
    mAddJointsPushButton->setIcon(QIcon(tr(":/icons/list-add.png")));
    mAddJointsPushButton->setToolTip(tr("Add Joints"));
    mEditJointsPushButton = new QPushButton(tr("Edit"), jointsPageWidget);
    mEditJointsPushButton->setIcon(QIcon(tr(":/icons/document-edit.png")));
    mEditJointsPushButton->setToolTip(tr("Edit Joints"));
    mDeleteJointsPushButton = new QPushButton(tr("Delete"), jointsPageWidget);
    mDeleteJointsPushButton->setIcon(QIcon(tr(":/icons/list-remove.png")));
    mDeleteJointsPushButton->setToolTip(tr("Delete Joints"));
    QHBoxLayout *horizontalBoxLayoutA2 = new QHBoxLayout;
    horizontalBoxLayoutA2->addWidget(mAddJointsPushButton);
    horizontalBoxLayoutA2->addWidget(mEditJointsPushButton);
    horizontalBoxLayoutA2->addWidget(mDeleteJointsPushButton);
    horizontalBoxLayoutA2->addStretch();
    verticalBoxLayoutA1->addLayout(horizontalBoxLayoutA2);
    mJointsTableView = new QTableView(jointsPageWidget);
    verticalBoxLayoutA1->addWidget(mJointsTableView);
    mSelectionTabWidget->addTab(jointsPageWidget, tr("Joints"));
    createJointsModelHeader();

    //Bars Tab

    QWidget *barsPageWidget = new QWidget(mSelectionTabWidget);
    QVBoxLayout *verticalBoxLayoutB1 = new QVBoxLayout(barsPageWidget);
    mBarsModeGroupBox = new QGroupBox(tr("Mode"), barsPageWidget);
    QVBoxLayout *verticalBoxLayoutB2 = new QVBoxLayout(mBarsModeGroupBox);
    mBarsSingleRadioButton = new QRadioButton(tr("Single"), mBarsModeGroupBox);
    mBarsSingleRadioButton->setChecked(true);
    mBarsMultipleRadioButton = new QRadioButton(tr("Multiple"), mBarsModeGroupBox);
    mBarsMultipleRadioButton->setChecked(false);
    verticalBoxLayoutB2->addWidget(mBarsSingleRadioButton);
    verticalBoxLayoutB2->addWidget(mBarsMultipleRadioButton);
    mAxialRigidityOptionGroupBox = new QGroupBox(tr("EA Option"), barsPageWidget);
    QVBoxLayout *verticalBoxLayoutB3 = new QVBoxLayout(mAxialRigidityOptionGroupBox);
    mFactorRadioButton = new QRadioButton(tr("Factor"), mAxialRigidityOptionGroupBox);
    mFactorRadioButton->setChecked(true);
    mValueRadioButton = new QRadioButton(tr("Value"), mAxialRigidityOptionGroupBox);
    mValueRadioButton->setChecked(false);
    verticalBoxLayoutB3->addWidget(mFactorRadioButton);
    verticalBoxLayoutB3->addWidget(mValueRadioButton);
    QHBoxLayout *horizontalBoxLayoutB1 = new QHBoxLayout;
    horizontalBoxLayoutB1->addWidget(mBarsModeGroupBox);
    horizontalBoxLayoutB1->addWidget(mAxialRigidityOptionGroupBox);
    horizontalBoxLayoutB1->addStretch();
    verticalBoxLayoutB1->addLayout(horizontalBoxLayoutB1);
    mBarsClearToolButton = new QToolButton(barsPageWidget);
    mBarsClearToolButton->setIcon(QIcon(tr(":/icons/edit-clear.png")));
    mBarsClearToolButton->setToolTip(tr("Clear Selection"));
    verticalBoxLayoutB1->addWidget(mBarsClearToolButton);
    mAddBarsPushButton = new QPushButton(tr("Add"), barsPageWidget);
    mAddBarsPushButton->setIcon(QIcon(tr(":/icons/list-add.png")));
    mAddBarsPushButton->setToolTip(tr("Add Bars"));
    mEditBarsPushButton = new QPushButton(tr("Edit"), barsPageWidget);
    mEditBarsPushButton->setIcon(QIcon(tr(":/icons/document-edit.png")));
    mEditBarsPushButton->setToolTip(tr("Edit Bars"));
    mDeleteBarsPushButton = new QPushButton(tr("Delete"), barsPageWidget);
    mDeleteBarsPushButton->setIcon(QIcon(tr(":/icons/list-remove.png")));
    mDeleteBarsPushButton->setToolTip(tr("Delete Bars"));
    QHBoxLayout *horizontalBoxLayoutB2 = new QHBoxLayout;
    horizontalBoxLayoutB2->addWidget(mAddBarsPushButton);
    horizontalBoxLayoutB2->addWidget(mEditBarsPushButton);
    horizontalBoxLayoutB2->addWidget(mDeleteBarsPushButton);
    horizontalBoxLayoutB2->addStretch();
    verticalBoxLayoutB1->addLayout(horizontalBoxLayoutB2);
    mBarsTableView = new QTableView(barsPageWidget);
    verticalBoxLayoutB1->addWidget(mBarsTableView);
    mSelectionTabWidget->addTab(barsPageWidget, tr("Bars"));
    createBarsModelHeader();

    //Supports Tab

    QWidget *supportsPageWidget = new QWidget(mSelectionTabWidget);
    QVBoxLayout *verticalBoxLayoutC1 = new QVBoxLayout(supportsPageWidget);
    mSupportsModeGroupBox = new QGroupBox(tr("Mode"), supportsPageWidget);
    QVBoxLayout *verticalBoxLayoutC2 = new QVBoxLayout(mSupportsModeGroupBox);
    mSupportsSingleRadioButton = new QRadioButton(tr("Single"), mSupportsModeGroupBox);
    mSupportsSingleRadioButton->setChecked(true);
    mSupportsMultipleRadioButton = new QRadioButton(tr("Multiple"), mSupportsModeGroupBox);
    mSupportsMultipleRadioButton->setChecked(false);
    verticalBoxLayoutC2->addWidget(mSupportsSingleRadioButton);
    verticalBoxLayoutC2->addWidget(mSupportsMultipleRadioButton);
    QHBoxLayout *horizontalBoxLayoutC1 = new QHBoxLayout;
    horizontalBoxLayoutC1->addWidget(mSupportsModeGroupBox);
    horizontalBoxLayoutC1->addStretch();
    verticalBoxLayoutC1->addLayout(horizontalBoxLayoutC1);
    mSupportsClearToolButton = new QToolButton(supportsPageWidget);
    mSupportsClearToolButton->setIcon(QIcon(tr(":/icons/edit-clear.png")));
    mSupportsClearToolButton->setToolTip(tr("Clear Selection"));
    verticalBoxLayoutC1->addWidget(mSupportsClearToolButton);
    mAddSupportsPushButton = new QPushButton(tr("Add"), supportsPageWidget);
    mAddSupportsPushButton->setToolTip(tr("Add Supports"));
    mAddSupportsPushButton->setIcon(QIcon(tr(":/icons/list-add.png")));
    mEditSupportsPushButton = new QPushButton(tr("Edit"), supportsPageWidget);
    mEditSupportsPushButton->setIcon(QIcon(tr(":/icons/document-edit.png")));
    mEditSupportsPushButton->setToolTip(tr("Edit Supports"));
    mDeleteSupportsPushButton = new QPushButton(tr("Delete"), supportsPageWidget);
    mDeleteSupportsPushButton->setIcon(QIcon(tr(":/icons/list-remove.png")));
    mDeleteSupportsPushButton->setToolTip(tr("Delete Supports"));
    QHBoxLayout *horizontalBoxLayoutC2 = new QHBoxLayout;
    horizontalBoxLayoutC2->addWidget(mAddSupportsPushButton);
    horizontalBoxLayoutC2->addWidget(mEditSupportsPushButton);
    horizontalBoxLayoutC2->addWidget(mDeleteSupportsPushButton);
    horizontalBoxLayoutC2->addStretch();
    verticalBoxLayoutC1->addLayout(horizontalBoxLayoutC2);
    mSupportsTableView = new QTableView(supportsPageWidget);
    verticalBoxLayoutC1->addWidget(mSupportsTableView);
    mSelectionTabWidget->addTab(supportsPageWidget, tr("Supports"));
    createSupportsModelHeader();

    //Loads Tab

    QWidget *loadsPageWidget = new QWidget(mSelectionTabWidget);
    QVBoxLayout *verticalBoxLayoutD1 = new QVBoxLayout(loadsPageWidget);
    mLoadTypeListWidget = new QListWidget(mSelectionTabWidget);
    verticalBoxLayoutD1->addWidget(mLoadTypeListWidget);
    QHBoxLayout *horizontalBoxLayoutD1 = new QHBoxLayout;
    mLoadsModeGroupBox = new QGroupBox(tr("Mode"), loadsPageWidget);
    QVBoxLayout *verticalBoxLayoutD2 = new QVBoxLayout(mLoadsModeGroupBox);
    mLoadsSingleRadioButton = new QRadioButton(tr("Single"), mLoadsModeGroupBox);
    mLoadsSingleRadioButton->setChecked(true);
    mLoadsMultipleRadioButton = new QRadioButton(tr("Multiple"), mLoadsModeGroupBox);
    mLoadsMultipleRadioButton->setChecked(false);
    verticalBoxLayoutD2->addWidget(mLoadsSingleRadioButton);
    verticalBoxLayoutD2->addWidget(mLoadsMultipleRadioButton);
    horizontalBoxLayoutD1->addWidget(mLoadsModeGroupBox);
    mSelfWeightDisplayGroupBox = new QGroupBox(tr("Show Self Wt."), loadsPageWidget);
    QVBoxLayout *verticalBoxLayoutD3 = new QVBoxLayout(mSelfWeightDisplayGroupBox);
    mYesRadioButton = new QRadioButton(tr("Yes"), mSelfWeightDisplayGroupBox);
    mYesRadioButton->setChecked(true);
    mNoRadioButton = new QRadioButton(tr("No"), mSelfWeightDisplayGroupBox);
    mNoRadioButton->setChecked(false);
    verticalBoxLayoutD3->addWidget(mYesRadioButton);
    verticalBoxLayoutD3->addWidget(mNoRadioButton);
    horizontalBoxLayoutD1->addWidget(mSelfWeightDisplayGroupBox);
    horizontalBoxLayoutD1->addStretch();
    verticalBoxLayoutD1->addLayout(horizontalBoxLayoutD1);
    mLoadsClearToolButton = new QToolButton(loadsPageWidget);
    mLoadsClearToolButton->setIcon(QIcon(tr(":/icons/edit-clear.png")));
    mLoadsClearToolButton->setToolTip(tr("Clear Selection"));
    verticalBoxLayoutD1->addWidget(mLoadsClearToolButton);
    mAddLoadPushButton = new QPushButton(tr("Add"), loadsPageWidget);
    mAddLoadPushButton->setIcon(QIcon(tr(":/icons/list-add.png")));
    mAddLoadPushButton->setToolTip(tr("Add Loads"));
    mEditLoadPushButton = new QPushButton(tr("Edit"), loadsPageWidget);
    mEditLoadPushButton->setIcon(QIcon(tr(":/icons/document-edit.png")));
    mEditLoadPushButton->setToolTip(tr("Edit Loads"));
    mDeleteLoadPushButton = new QPushButton(tr("Delete"), loadsPageWidget);
    mDeleteLoadPushButton->setIcon(QIcon(tr(":/icons/list-remove.png")));
    mDeleteLoadPushButton->setToolTip(tr("Delete Loads"));
    QHBoxLayout *horizontalBoxLayoutD2 = new QHBoxLayout;
    horizontalBoxLayoutD2->addWidget(mAddLoadPushButton);
    horizontalBoxLayoutD2->addWidget(mEditLoadPushButton);
    horizontalBoxLayoutD2->addWidget(mDeleteLoadPushButton);
    horizontalBoxLayoutD2->addStretch();
    verticalBoxLayoutD1->addLayout(horizontalBoxLayoutD2);
    mLoadsTableView = new QTableView(loadsPageWidget);
    verticalBoxLayoutD1->addWidget(mLoadsTableView);
    mSelectionTabWidget->addTab(loadsPageWidget, tr("Loads"));

    createJointLoadsModelHeader();
    createSupportSettlementsModelHeader();
    createThermalEffectsModelHeader();
    createFabricationErrorsModelHeader();
    createInfluenceLoadsModelHeader();
    createJointDeflectionsModelHeader();
    createBarLoadsModelHeader();
    createSupportReactionsModelHeader();
    createInfluenceLoadResultsModelHeader();

    int maximumHeight = 100;
    mLoadTypeListWidget->setMaximumHeight(maximumHeight);
    mLoadTypeListWidget->addItem(tr("Joint Loads"));
    mLoadTypeListWidget->addItem(tr("Support Settlements"));
    mLoadTypeListWidget->addItem(tr("Thermal Effects"));
    mLoadTypeListWidget->addItem(tr("Assembly/Fabrication Errors"));
    mLoadTypeListWidget->addItem(tr("Influence Loads"));
    mLoadTypeListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mLoadTypeListWidget->setCurrentRow(0);

    mAddLoadPushButton->setEnabled(false);
    mEditLoadPushButton->setEnabled(false);
    mDeleteLoadPushButton->setEnabled(false);

    //Solution Tab

    QWidget *solutionPageWidget = new QWidget(mSelectionTabWidget);
    QVBoxLayout *verticalBoxLayoutE1 = new QVBoxLayout(solutionPageWidget);
    mLoadsGroupBox = new QGroupBox(tr("Loads"), solutionPageWidget);
    QVBoxLayout *verticalBoxLayoutE2 = new QVBoxLayout(mLoadsGroupBox);
    mJointLoadsCheckBox = new QCheckBox(tr("Joint Loads"), mLoadsGroupBox);
    mJointLoadsCheckBox->setChecked(false);
    mJointLoadsCheckBox->setEnabled(false);
    mSelfWeightCheckBox = new QCheckBox(tr("Self Wt."), mLoadsGroupBox);
    mSelfWeightCheckBox->setChecked(false);
    mSelfWeightCheckBox->setEnabled(false);
    mSupportSettlementsCheckBox = new QCheckBox(tr("Support Settlements"), mLoadsGroupBox);
    mSupportSettlementsCheckBox->setChecked(false);
    mSupportSettlementsCheckBox->setEnabled(false);
    mThermalEffectsCheckBox = new QCheckBox(tr("Thermal Effects"), mLoadsGroupBox);
    mThermalEffectsCheckBox->setChecked(false);
    mThermalEffectsCheckBox->setEnabled(false);
    mFabricationErrorsCheckBox = new QCheckBox(tr("Fabrication Errors"), mLoadsGroupBox);
    mFabricationErrorsCheckBox->setChecked(false);
    mFabricationErrorsCheckBox->setEnabled(false);
    mInfluenceLoadsCheckBox = new QCheckBox(tr("Influence Loads"), mLoadsGroupBox);
    mInfluenceLoadsCheckBox->setChecked(false);
    mInfluenceLoadsCheckBox->setEnabled(false);
    verticalBoxLayoutE2->addWidget(mJointLoadsCheckBox);
    verticalBoxLayoutE2->addWidget(mSelfWeightCheckBox);
    verticalBoxLayoutE2->addWidget(mSupportSettlementsCheckBox);
    verticalBoxLayoutE2->addWidget(mThermalEffectsCheckBox);
    verticalBoxLayoutE2->addWidget(mFabricationErrorsCheckBox);
    verticalBoxLayoutE2->addWidget(mInfluenceLoadsCheckBox);
    mInfluenceLoadsComboBox = new ComboBox(mLoadsGroupBox);
    mInfluenceLoadsComboBox->setEnabled(false);
    QHBoxLayout *horizontalBoxLayoutE1 = new QHBoxLayout;
    horizontalBoxLayoutE1->addWidget(mInfluenceLoadsComboBox);
    verticalBoxLayoutE2->addLayout(horizontalBoxLayoutE1);
    verticalBoxLayoutE1->addWidget(mLoadsGroupBox);

    mSolutionOptionLabel = new QLabel(tr("Solution Option"), solutionPageWidget);
    mSolutionOptionComboBox = new ComboBox(solutionPageWidget);
    QGridLayout *gridLayoutE1 = new QGridLayout;
    gridLayoutE1->addWidget(mSolutionOptionLabel, 0, 0, 1, 1);
    gridLayoutE1->addWidget(mSolutionOptionComboBox, 0, 1, 1, 1);
    mInfluenceLoadBarLabel = new QLabel(tr("Influence Load Bar"), solutionPageWidget);
    mInfluenceLoadBarComboBox = new ComboBox(solutionPageWidget);
    gridLayoutE1->addWidget(mInfluenceLoadBarLabel, 1, 0, 1, 1);
    gridLayoutE1->addWidget(mInfluenceLoadBarComboBox, 1, 1, 1, 1);
    gridLayoutE1->setColumnStretch(0, 0);
    gridLayoutE1->setColumnStretch(1, 1);
    verticalBoxLayoutE1->addLayout(gridLayoutE1);

    mSolvePushButton = new QPushButton(tr("Solve"), solutionPageWidget);
    mSolvePushButton->setIcon(QIcon(tr(":/icons/quickopen.png")));
    mSolvePushButton->setToolTip(tr("Solve Model"));
    mClearPushButton = new QPushButton(tr("Clear"), solutionPageWidget);
    mClearPushButton->setIcon(QIcon(tr(":/icons/trash-empty.png")));
    mClearPushButton->setToolTip(tr("Clear Solution"));
    mReportPushButton = new QPushButton(tr("Report"), solutionPageWidget);
    mReportPushButton->setIcon(QIcon(tr(":/icons/application-vnd.oasis.opendocument.text.png")));
    mReportPushButton->setToolTip(tr("Report Solution"));
    QHBoxLayout *horizontalBoxLayoutE2 = new QHBoxLayout;
    horizontalBoxLayoutE2->addWidget(mSolvePushButton);
    horizontalBoxLayoutE2->addWidget(mClearPushButton);
    horizontalBoxLayoutE2->addWidget(mReportPushButton);
    horizontalBoxLayoutE2->addStretch();
    verticalBoxLayoutE1->addLayout(horizontalBoxLayoutE2);
    mSolutionsTableView = new QTableView(solutionPageWidget);
    verticalBoxLayoutE1->addWidget(mSolutionsTableView);
    mSelectionTabWidget->addTab(solutionPageWidget, tr("Solution"));

    mSolutionsTableView->setModel(mJointDeflectionsStandardItemModel);
    mSolutionsTableView->setCornerButtonEnabled(false);
    mSolutionsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mSolutionsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mSolutionsTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    mSolutionsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mSolutionOptionComboBox->addItem(tr("Joint deflections"));
    mSolutionOptionComboBox->addItem(tr("Bar loads"));
    mSolutionOptionComboBox->addItem(tr("Support reactions"));
    mSolutionOptionComboBox->addItem(tr("Influence loads"));
    mSolutionOptionComboBox->setCurrentIndex(-1);
    mSolutionOptionComboBox->setEnabled(false);
    mInfluenceLoadBarComboBox->setEnabled(false);

    mClearPushButton->setEnabled(false);
    mReportPushButton->setEnabled(false);

    mSelectionTabWidget->setCurrentIndex(0);

    //Menus

    mNewAction = new QAction(tr("&New"), this);
    mNewAction->setShortcut(QKeySequence::New);
    mNewAction->setStatusTip(tr("Create new model"));
    mNewAction->setIcon(QIcon(tr(":/icons/document-new.png")));

    mOpenAction = new QAction(tr("&Open..."), this);
    mOpenAction->setShortcut(QKeySequence::Open);
    mOpenAction->setStatusTip(tr("Open existing model file"));
    mOpenAction->setIcon(QIcon(tr(":/icons/document-open.png")));

    mSaveAction = new QAction(tr("&Save"), this);
    mSaveAction->setShortcut(QKeySequence::Save);
    mSaveAction->setStatusTip(tr("Save model to file"));
    mSaveAction->setIcon(QIcon(tr(":/icons/document-save.png")));

    mSaveAsAction = new QAction(tr("Save As..."), this);
    mSaveAsAction->setShortcut(QKeySequence::SaveAs);
    mSaveAsAction->setStatusTip(tr("Save model to file under a different name"));
    mSaveAsAction->setIcon(QIcon(tr(":/icons/document-save-as.png")));

    mExportModelImageAction = new QAction(tr("Export Model Image..."), this);
    mExportModelImageAction->setStatusTip(tr("Export model image to file"));

    mExitAction = new QAction(tr("Exit"), this);
    mExitAction->setShortcut(QKeySequence::Close);
    mExitAction->setStatusTip(tr("Exit application"));
    mExitAction->setIcon(QIcon(tr(":/icons/application-exit.png")));

    mSelectionPanelAction = new QAction(tr("Selection Panel"), this);
    mSelectionPanelAction->setStatusTip(tr("Show selection panel if it was closed or hidden"));

    mShowNotesAction = new QAction(tr("Notes"), this);
    mShowNotesAction->setStatusTip(tr("Show notes dialog"));

    mUnitsAction = new QAction(tr("Units..."), this);
    mUnitsAction->setStatusTip(tr("Set model units"));
    mUnitsAction->setIcon(QIcon(tr(":/icons/measure.png")));

    mSetModelAreaAction = new QAction(tr("Model Area..."), this);
    mSetModelAreaAction->setStatusTip(tr("Set model area size"));

    mShowHideAxesAction = new QAction(tr("Show/Hide Axes"), this);
    mShowHideAxesAction->setStatusTip(tr("Show or hide coordinate axes"));
    mShowHideAxesAction->setCheckable(true);
    mShowHideAxesAction->setChecked(true);

    mShowHideJointNumberAction = new QAction(tr("Show/Hide Joint Number"), this);
    mShowHideJointNumberAction->setStatusTip(tr("Show or hide joint number"));
    mShowHideJointNumberAction->setCheckable(true);
    mShowHideJointNumberAction->setChecked(true);

    mShowHideBarNumberAction = new QAction(tr("Show/Hide Bar Number"), this);
    mShowHideBarNumberAction->setStatusTip(tr("Show or hide bar number"));
    mShowHideBarNumberAction->setCheckable(true);
    mShowHideBarNumberAction->setChecked(true);

    mScaleForcesAction = new QAction(tr("Scale Forces..."), this);
    mScaleForcesAction->setStatusTip(tr("Set scale for drawing forces"));

    mScaleDeflectionsAction = new QAction(tr("Scale Deflections..."), this);
    mScaleDeflectionsAction->setStatusTip(tr("Set scale for drawing deflections"));

    mCheckModelAction = new QAction(tr("Check Model"), this);
    mCheckModelAction->setStatusTip(tr("Check model for validity for analysis"));
    mCheckModelAction->setIcon(QIcon(tr(":/icons/games-solve.png")));

    mHelpAction = new QAction(tr("Help"), this);
    mHelpAction->setShortcut(QKeySequence::HelpContents);
    mHelpAction->setStatusTip(tr("Display help contents for TrussTables"));
    mHelpAction->setIcon(QIcon(tr(":/icons/help-browser.png")));

    mAboutTrussTablesAction = new QAction(tr("About TrussTables..."), this);
    mAboutTrussTablesAction->setStatusTip(tr("About TrussTables"));
    mAboutTrussTablesAction->setIcon(QIcon(tr(":/icons/dialog-information.png")));

    mAboutQtAction = new QAction(tr("About Qt..."), this);
    mAboutQtAction->setStatusTip(tr("About Qt"));
    mAboutQtAction->setIcon(QIcon(tr(":/icons/qtlogo.png")));

    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mNewAction);
    mFileMenu->addAction(mOpenAction);
    mOpenRecentMenu = new QMenu(tr("Open Recent"), this);
    mFileMenu->addMenu(mOpenRecentMenu);
    mFileMenu->addAction(mSaveAction);
    mFileMenu->addAction(mSaveAsAction);
    mFileMenu->addAction(mExportModelImageAction);
    mFileMenu->addAction(mExitAction);

    mViewMenu = menuBar()->addMenu(tr("&View"));
    mViewMenu->addAction(mSelectionPanelAction);
    mViewMenu->addAction(mShowNotesAction);

    mModelMenu = menuBar()->addMenu(tr("&Model"));
    mModelMenu->addAction(mUnitsAction);
    mModelMenu->addAction(mSetModelAreaAction);
    mModelMenu->addAction(mShowHideAxesAction);
    mModelMenu->addAction(mShowHideJointNumberAction);
    mModelMenu->addAction(mShowHideBarNumberAction);
    mModelMenu->addAction(mScaleForcesAction);
    mModelMenu->addAction(mScaleDeflectionsAction);
    mModelMenu->addAction(mCheckModelAction);

    mHelpMenu = menuBar()->addMenu(tr("&Help"));
    mHelpMenu->addAction(mHelpAction);
    mHelpMenu->addAction(mAboutTrussTablesAction);
    mHelpMenu->addAction(mAboutQtAction);

    for (int i = 0; i < MAX_RECENT_FILES; ++i)
    {
        mRecentFileActionsArray[i] = new QAction(this);
        mRecentFileActionsArray[i]->setVisible(false);
        connect(mRecentFileActionsArray[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    mClearMenuAction = new QAction(tr("Clear Menu"), this);
    mOpenRecentMenu->addAction(mClearMenuAction);
    mSeparatorAction = mOpenRecentMenu->insertSeparator(mClearMenuAction);

    for (int i = 0; i < MAX_RECENT_FILES; ++i)
    {
        mOpenRecentMenu->insertAction(mSeparatorAction, mRecentFileActionsArray[i]);
    }

    //ToolBar

    mToolBar = addToolBar(tr("Tools"));
    mToolBar->addAction(mNewAction);
    mToolBar->addAction(mOpenAction);
    mToolBar->addAction(mSaveAction);
    mToolBar->addAction(mUnitsAction);
    mToolBar->addAction(mCheckModelAction);
    mToolBar->setAllowedAreas(Qt::TopToolBarArea);
}

void Solver::createJointsModelHeader()
{
    QStandardItem *xHeaderItem = new QStandardItem(tr("x (%1)").arg(mUnitsAndLimits.coordinateUnit()));
    QStandardItem *yHeaderItem = new QStandardItem(tr("y (%1)").arg(mUnitsAndLimits.coordinateUnit()));

    xHeaderItem->setTextAlignment(Qt::AlignCenter);
    yHeaderItem->setTextAlignment(Qt::AlignCenter);

    mJointsStandardItemModel->setHorizontalHeaderItem(0, xHeaderItem);
    mJointsStandardItemModel->setHorizontalHeaderItem(1, yHeaderItem);

    mJointsTableView->setModel(mJointsStandardItemModel);
    mJointsTableView->setCornerButtonEnabled(true);
    mJointsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mJointsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mJointsTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    mJointsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Solver::createBarsModelHeader()
{
    if (mBarsStandardItemModel->columnCount() == 5)
    {
        QStandardItem *columnA = new QStandardItem(tr("First Joint"));
        columnA->setTextAlignment(Qt::AlignCenter);

        QStandardItem *columnB = new QStandardItem(tr("Second Joint"));
        columnB->setTextAlignment(Qt::AlignCenter);

        QStandardItem *columnC = new QStandardItem(tr("A (%1)").arg(mUnitsAndLimits.areaUnit()));
        columnC->setTextAlignment(Qt::AlignCenter);

        QStandardItem *columnD = new QStandardItem(tr("E (%1)").arg(mUnitsAndLimits.modulusUnit()));
        columnD->setTextAlignment(Qt::AlignCenter);

        QStandardItem *columnE = new QStandardItem(tr("W (%1)").arg(mUnitsAndLimits.unitWeightUnit()));
        columnE->setTextAlignment(Qt::AlignCenter);

        mBarsStandardItemModel->setHorizontalHeaderItem(0, columnA);
        mBarsStandardItemModel->setHorizontalHeaderItem(1, columnB);
        mBarsStandardItemModel->setHorizontalHeaderItem(2, columnC);
        mBarsStandardItemModel->setHorizontalHeaderItem(3, columnD);
        mBarsStandardItemModel->setHorizontalHeaderItem(4, columnE);
    }
    else
    {
        QStandardItem *columnA = new QStandardItem(tr("First Joint"));
        columnA->setTextAlignment(Qt::AlignCenter);

        QStandardItem *columnB = new QStandardItem(tr("Second Joint"));
        columnB->setTextAlignment(Qt::AlignCenter);

        QStandardItem *columnC = new QStandardItem(tr("Factor"));
        columnC->setTextAlignment(Qt::AlignCenter);

        mBarsStandardItemModel->setHorizontalHeaderItem(0, columnA);
        mBarsStandardItemModel->setHorizontalHeaderItem(1, columnB);
        mBarsStandardItemModel->setHorizontalHeaderItem(2, columnC);
    }

    mBarsTableView->setModel(mBarsStandardItemModel);
    mBarsTableView->setCornerButtonEnabled(false);
    mBarsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mBarsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mBarsTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    mBarsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int minimumSectionSize = 80;
    mBarsTableView->horizontalHeader()->setMinimumSectionSize(minimumSectionSize);
}

void Solver::createSupportsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Joint"));
    columnA->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnB = new QStandardItem(tr("Type"));
    columnB->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnC = new QStandardItem(tr("Angle (%1)").arg(QString::fromUtf8("\u00B0")));
    columnC->setTextAlignment(Qt::AlignCenter);
    mSupportsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mSupportsStandardItemModel->setHorizontalHeaderItem(1, columnB);
    mSupportsStandardItemModel->setHorizontalHeaderItem(2, columnC);
    mSupportsTableView->setModel(mSupportsStandardItemModel);
    mSupportsTableView->setCornerButtonEnabled(false);
    mSupportsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mSupportsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mSupportsTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    mSupportsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Solver::createJointLoadsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Joint"));
    columnA->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnB = new QStandardItem(tr("H (%1)").arg(mUnitsAndLimits.loadUnit()));
    columnB->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnC = new QStandardItem(tr("V (%1)").arg(mUnitsAndLimits.loadUnit()));
    columnC->setTextAlignment(Qt::AlignCenter);
    mJointLoadsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mJointLoadsStandardItemModel->setHorizontalHeaderItem(1, columnB);
    mJointLoadsStandardItemModel->setHorizontalHeaderItem(2, columnC);
    mLoadsTableView->setModel(mJointLoadsStandardItemModel);
    mLoadsTableView->setCornerButtonEnabled(false);
    mLoadsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mLoadsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mLoadsTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    mLoadsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int minimumSectionSize = 100;
    mLoadsTableView->horizontalHeader()->setMinimumSectionSize(minimumSectionSize);
    mLoadsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mLoadsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mLoadsTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void Solver::createSupportSettlementsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Support"));
    columnA->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnB = new QStandardItem(tr("%1y (%2)")
                                               .arg(QString::fromUtf8("\u0394"))
                                               .arg(mUnitsAndLimits.supportSettlementUnit()));
    columnB->setTextAlignment(Qt::AlignCenter);
    mSupportSettlementsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mSupportSettlementsStandardItemModel->setHorizontalHeaderItem(1, columnB);
}

void Solver::createThermalEffectsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Bar"));
    columnA->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnB = new QStandardItem(tr("%1t (%2)")
                                               .arg(QString::fromUtf8("\u0394"))
                                               .arg(mUnitsAndLimits.temperatureChangeUnit()));
    columnB->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnC = new QStandardItem(tr("%1 (%2)")
                                               .arg(QString::fromUtf8("\u03B1"))
                                               .arg(mUnitsAndLimits.thermalCoefficientUnit()));
    columnC->setTextAlignment(Qt::AlignCenter);
    mThermalEffectsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mThermalEffectsStandardItemModel->setHorizontalHeaderItem(1, columnB);
    mThermalEffectsStandardItemModel->setHorizontalHeaderItem(2, columnC);
}

void Solver::createFabricationErrorsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Bar"));
    columnA->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnB = new QStandardItem(tr("%1L (%2)")
                                               .arg(QString::fromUtf8("\u0394"))
                                               .arg(mUnitsAndLimits.lengthErrorUnit()));
    columnB->setTextAlignment(Qt::AlignCenter);
    mFabricationErrorsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mFabricationErrorsStandardItemModel->setHorizontalHeaderItem(1, columnB);
}

void Solver::createInfluenceLoadsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Name"));
    columnA->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnB = new QStandardItem(tr("Direction"));
    columnB->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnC = new QStandardItem(tr("Path"));
    columnC->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnD = new QStandardItem(tr("Point Loads (%1)").arg(mUnitsAndLimits.loadUnit()));
    columnD->setTextAlignment(Qt::AlignCenter);
    mInfluenceLoadsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mInfluenceLoadsStandardItemModel->setHorizontalHeaderItem(1, columnB);
    mInfluenceLoadsStandardItemModel->setHorizontalHeaderItem(2, columnC);
    mInfluenceLoadsStandardItemModel->setHorizontalHeaderItem(3, columnD);
}

void Solver::createJointDeflectionsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Joint"));
    columnA->setTextAlignment(Qt::AlignCenter);

    QStandardItem *columnB;

    QString string;

    if (mUnitsAndLimits.system() == tr("metric"))
    {
        string = tr("m");
    }
    else
    {
        string = tr("ft");
    }

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        columnB = new QStandardItem(tr("%1x (%2)")
                                    .arg(QString::fromUtf8("\u0394"))
                                    .arg(string));
    }
    else
    {
        columnB = new QStandardItem(tr("%1x%2AE (%3)")
                                    .arg(QString::fromUtf8("\u0394"))
                                    .arg(QString::fromUtf8("\u00D7"))
                                    .arg(string));
    }

    columnB->setTextAlignment(Qt::AlignCenter);

    QStandardItem *columnC;

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        columnC = new QStandardItem(tr("%1y (%2)")
                                    .arg(QString::fromUtf8("\u0394"))
                                    .arg(string));
    }
    else
    {
        columnC = new QStandardItem(tr("%1y%2AE (%3)")
                                    .arg(QString::fromUtf8("\u0394"))
                                    .arg(QString::fromUtf8("\u00D7"))
                                    .arg(string));
    }

    columnC->setTextAlignment(Qt::AlignCenter);

    mJointDeflectionsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mJointDeflectionsStandardItemModel->setHorizontalHeaderItem(1, columnB);
    mJointDeflectionsStandardItemModel->setHorizontalHeaderItem(2, columnC);
}

void Solver::createBarLoadsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Bar"));
    columnA->setTextAlignment(Qt::AlignCenter);

    QString string;

    if (mUnitsAndLimits.system() == tr("metric"))
    {
        string = tr("N");
    }
    else
    {
        string = tr("lb");
    }

    QStandardItem *columnB = new QStandardItem(tr("Load (%1)").arg(string));
    columnB->setTextAlignment(Qt::AlignCenter);
    mBarLoadsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mBarLoadsStandardItemModel->setHorizontalHeaderItem(1, columnB);
}

void Solver::createSupportReactionsModelHeader()
{
    QStandardItem *columnA = new QStandardItem(tr("Support"));
    columnA->setTextAlignment(Qt::AlignCenter);

    QString string;

    if (mUnitsAndLimits.system() == tr("metric"))
    {
        string = tr("N");
    }
    else
    {
        string = tr("lb");
    }

    QStandardItem *columnB = new QStandardItem(tr("H (%1)").arg(string));
    columnB->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnC = new QStandardItem(tr("V (%1)").arg(string));
    columnC->setTextAlignment(Qt::AlignCenter);
    mSupportReactionsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mSupportReactionsStandardItemModel->setHorizontalHeaderItem(1, columnB);
    mSupportReactionsStandardItemModel->setHorizontalHeaderItem(2, columnC);
}

void Solver::createInfluenceLoadResultsModelHeader()
{
    QString string;

    if (mUnitsAndLimits.system() == tr("metric"))
    {
        string = tr("N");
    }
    else
    {
        string = tr("lb");
    }

    QStandardItem *columnA = new QStandardItem(tr("Pmin (%1)").arg(string));
    columnA->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnB = new QStandardItem(tr("Position"));
    columnB->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnC = new QStandardItem(tr("Pmax (%1)").arg(string));
    columnC->setTextAlignment(Qt::AlignCenter);
    QStandardItem *columnD = new QStandardItem(tr("Position"));
    columnD->setTextAlignment(Qt::AlignCenter);
    mInfluenceLoadResultsStandardItemModel->setHorizontalHeaderItem(0, columnA);
    mInfluenceLoadResultsStandardItemModel->setHorizontalHeaderItem(1, columnB);
    mInfluenceLoadResultsStandardItemModel->setHorizontalHeaderItem(2, columnC);
    mInfluenceLoadResultsStandardItemModel->setHorizontalHeaderItem(3, columnD);
}

void Solver::readSettings()
{
    QSettings settings(tr("TrussTables Project"), tr("TrussTables"));

    restoreGeometry(settings.value(tr("geometry")).toByteArray());

    mRecentFiles = settings.value(tr("recentFiles")).toStringList();

    int defaultWidthFactor  = 1;
    int defaultHeightFactor = 1;
    int defaultMargin       = 75;
    int widthFactor         = settings.value(tr("widthFactor"), defaultWidthFactor).toInt();
    int heightFactor        = settings.value(tr("heightFactor"), defaultHeightFactor).toInt();
    int margin              = settings.value(tr("margin"), defaultMargin).toInt();
    mModelAreaDialog->setWidthFactor(widthFactor);
    mModelAreaDialog->setHeightFactor(heightFactor);
    mModelAreaDialog->setMargin(margin);
    emit mModelAreaDialog->inputsSignal(widthFactor, heightFactor, margin);

    bool showHideAxes = settings.value(tr("showHideAxes"), true).toBool();
    mShowHideAxesAction->setChecked(showHideAxes);
    bool showHideJointNumbers = settings.value(tr("showHideJointNumbers"), true).toBool();
    mShowHideJointNumberAction->setChecked(showHideJointNumbers);
    bool showHideBarNumbers = settings.value(tr("showHideBarNumbers"), true).toBool();
    mShowHideBarNumberAction->setChecked(showHideBarNumbers);

    qreal defaultForceFactor = 0.2;
    qreal forceFactor        = settings.value(tr("forceFactor"), defaultForceFactor).toDouble();
    mScaleForcesDialog->setForceFactor(forceFactor);
    emit mScaleForcesDialog->factorSignal(forceFactor);

    qreal defaultDeflectionFactor = 1.0;
    qreal factor                  = 10000.0;
    qreal deflectionFactor        = settings.value(tr("deflectionFactor"), defaultDeflectionFactor).toDouble();
    mScaleDeflectionsDialog->setDeflectionFactor(deflectionFactor);
    emit mScaleDeflectionsDialog->factorSignal(factor * deflectionFactor);

    bool jointsSingleSelection = settings.value(tr("jointsSelectionMode"), true).toBool();

    if (jointsSingleSelection)
    {
        mJointsSingleRadioButton->setChecked(true);
        mJointsMultipleRadioButton->setChecked(false);
    }
    else
    {
        mJointsSingleRadioButton->setChecked(false);
        mJointsMultipleRadioButton->setChecked(true);
    }

    bool barsSingleSelection = settings.value(tr("barsSelectionMode"), true).toBool();

    if (barsSingleSelection)
    {
        mBarsSingleRadioButton->setChecked(true);
        mBarsMultipleRadioButton->setChecked(false);
    }
    else
    {
        mBarsSingleRadioButton->setChecked(false);
        mBarsMultipleRadioButton->setChecked(true);
    }

    bool factorOption = settings.value(tr("factorOption"), true).toBool();

    if (factorOption)
    {
        mFactorRadioButton->setChecked(true);
        mValueRadioButton->setChecked(false);
        setFactorOption(true);
    }
    else
    {
        mFactorRadioButton->setChecked(false);
        mValueRadioButton->setChecked(true);
        setAreaModulusOption(true);
    }

    bool supportsSingleSelection = settings.value(tr("supportsSelectionMode"), true).toBool();

    if (supportsSingleSelection)
    {
        mSupportsSingleRadioButton->setChecked(true);
        mSupportsMultipleRadioButton->setChecked(false);
    }
    else
    {
        mSupportsSingleRadioButton->setChecked(false);
        mSupportsMultipleRadioButton->setChecked(true);
    }

    bool loadsSingleSelection = settings.value(tr("loadsSelectionMode"), true).toBool();

    if (loadsSingleSelection)
    {
        mLoadsSingleRadioButton->setChecked(true);
        mLoadsMultipleRadioButton->setChecked(false);
    }
    else
    {
        mLoadsSingleRadioButton->setChecked(false);
        mLoadsMultipleRadioButton->setChecked(true);
    }

    bool showSelfWeight = settings.value(tr("showSelfWeight"), true).toBool();

    if (showSelfWeight)
    {
        mYesRadioButton->setChecked(true);
        mNoRadioButton->setChecked(false);
    }
    else
    {
        mYesRadioButton->setChecked(false);
        mNoRadioButton->setChecked(true);
    }

    mLastOpenLocation             = settings.value(tr("lastOpenLocation"), mLastOpenLocation).toString();
    mLastSaveLocation             = settings.value(tr("lastSaveLocation"), mLastSaveLocation).toString();
    mLastExportModelImageLocation = settings.value(tr("lastExportModelImageLocation"),
                                                   mLastExportModelImageLocation).toString();
    mLastReportLocation           = settings.value(tr("lastReportLocation"), mLastReportLocation).toString();

    QDir dir(mLastOpenLocation);

    if (!dir.exists())
    {
        mLastOpenLocation = QDir::toNativeSeparators(QDir::homePath());
    }

    dir.setPath(mLastSaveLocation);

    if (!dir.exists())
    {
        mLastSaveLocation = QDir::toNativeSeparators(QDir::homePath());
    }

    dir.setPath(mLastExportModelImageLocation);

    if (!dir.exists())
    {
        mLastExportModelImageLocation = QDir::toNativeSeparators(QDir::homePath());
    }

    dir.setPath(mLastReportLocation);

    if (!dir.exists())
    {
        mLastReportLocation = QDir::toNativeSeparators(QDir::homePath());
    }
}

void Solver::writeSettings()
{
    QSettings settings(tr("TrussTables Project"), tr("TrussTables"));

    settings.setValue(tr("geometry"), saveGeometry());
    settings.setValue(tr("widthFactor"), mModelAreaDialog->widthFactor());
    settings.setValue(tr("heightFactor"), mModelAreaDialog->heightFactor());
    settings.setValue(tr("margin"), mModelAreaDialog->margin());
    settings.setValue(tr("showHideAxes"), mShowHideAxesAction->isChecked());
    settings.setValue(tr("showHideJointNumbers"), mShowHideJointNumberAction->isChecked());
    settings.setValue(tr("showHideBarNumbers"), mShowHideBarNumberAction->isChecked());
    settings.setValue(tr("forceFactor"), mScaleForcesDialog->forceFactor());
    settings.setValue(tr("deflectionFactor"), mScaleDeflectionsDialog->deflectionFactor());
    settings.setValue(tr("jointsSelectionMode"), mJointsSingleRadioButton->isChecked());
    settings.setValue(tr("barsSelectionMode"), mBarsSingleRadioButton->isChecked());
    settings.setValue(tr("factorOption"), mFactorRadioButton->isChecked());
    settings.setValue(tr("supportsSelectionMode"), mSupportsSingleRadioButton->isChecked());
    settings.setValue(tr("loadsSelectionMode"), mLoadsSingleRadioButton->isChecked());
    settings.setValue(tr("showSelfWeight"), mYesRadioButton->isChecked());
    settings.setValue(tr("lastOpenLocation"), mLastOpenLocation);
    settings.setValue(tr("lastSaveLocation"), mLastSaveLocation);
    settings.setValue(tr("lastExportModelImageLocation"), mLastExportModelImageLocation);
    settings.setValue(tr("lastReportLocation"), mLastReportLocation);
}

bool Solver::promptForSave()
{
    if (isWindowModified())
    {
        QString s1, s2;
        s1 = tr("TrussTables");
        s2 = tr("The Model has been modified.\n"
                "Do you want to save your changes?");
        QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
        QMessageBox::StandardButton result = QMessageBox::warning(this, s1, s2, buttons);

        if (result == QMessageBox::Yes)
        {
            return save();
        }
        else if (result == QMessageBox::Cancel)
        {
            return false;
        }
    }

    return true;
}

bool Solver::saveFile(const QString &fileName)
{

    QSaveFile saveFile(fileName);
    saveFile.setDirectWriteFallback(true);

    if (!saveFile.open(QSaveFile::WriteOnly))
    {
        QString s1, s2;
        s1 = tr("TrussTables");
        s2 = tr("Cannot write file %1:\n%2.").arg(saveFile.fileName().arg(saveFile.errorString()));
        QMessageBox::warning(this, s1, s2);
        return false;
    }

    QDataStream out(&saveFile);
    out.setVersion(QDataStream::Qt_5_4);
    out << quint32(0x25438F7BEA4);

    QString majorVersion(tr("0"));
    QString minorVersion(tr("0"));
    QString patchVersion(tr("0"));

#ifdef TRUSSTABLES_VERSION_MAJOR
    majorVersion = QString::number(TRUSSTABLES_VERSION_MAJOR);
#endif

#ifdef TRUSSTABLES_VERSION_MINOR
    minorVersion = QString::number(TRUSSTABLES_VERSION_MINOR);
#endif

#ifdef TRUSSTABLES_VERSION_PATCH
    patchVersion = QString::number(TRUSSTABLES_VERSION_PATCH);
#endif

    out << tr("%1.%2.%3").arg(majorVersion).arg(minorVersion).arg(patchVersion);

    //Description and Units

    out << mDescriptionText;
    out << mUnitsAndLimits.system();
    out << mUnitsAndLimits.coordinateUnit();
    out << mUnitsAndLimits.areaUnit();
    out << mUnitsAndLimits.modulusUnit();
    out << mUnitsAndLimits.unitWeightUnit();
    out << mUnitsAndLimits.loadUnit();
    out << mUnitsAndLimits.supportSettlementUnit();
    out << mUnitsAndLimits.temperatureChangeUnit();
    out << mUnitsAndLimits.thermalCoefficientUnit();
    out << mUnitsAndLimits.lengthErrorUnit();

    //Joint Coordinates

    QList<qreal> xCoordinateList;
    QList<qreal> yCoordinateList;

    foreach (Joint *joint, mJointsList)
    {
        xCoordinateList.append(joint->xCoordinate());
        yCoordinateList.append(joint->yCoordinate());
    }

    out << xCoordinateList;
    out << yCoordinateList;

    //Bars

    //Axial Rigidity Option

    QString axialRigidityOption;

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        axialRigidityOption = tr("value");
    }
    else
    {
        axialRigidityOption = tr("factor");
    }

    QList<qint32> firstJointList;
    QList<qint32> secondJointList;
    QList<qreal> areaList;
    QList<qreal> modulusList;
    QList<qreal> factorList;
    QList<qreal> unitWeightList;

    foreach (Bar *bar, mBarsList)
    {
        Joint *jointA    = bar->firstJoint();
        Joint *jointB    = bar->secondJoint();
        int jointNumberA = mJointsList.indexOf(jointA) + 1;
        int jointNumberB = mJointsList.indexOf(jointB) + 1;
        firstJointList.append(qint32(jointNumberA));
        secondJointList.append(qint32(jointNumberB));
        areaList.append(bar->area());
        modulusList.append(bar->modulus());
        factorList.append(bar->factor());
        unitWeightList.append(bar->unitWeight());
    }

    out << axialRigidityOption;
    out << firstJointList;
    out << secondJointList;
    out << areaList;
    out << modulusList;
    out << factorList;
    out << unitWeightList;

    //Supports

    QList<qint32> supportJointList;
    QList<qint32> supportTypeList;
    QList<qreal> supportAngleList;

    foreach (Support *support, mSupportsList)
    {
        Joint *supportJoint              = support->supportJoint();
        int jointNumber                  = mJointsList.indexOf(supportJoint) + 1;
        UnitsAndLimits::SupportType type = support->type();

        qint32 supportType;

        switch (type)
        {
            case UnitsAndLimits::FIXED_LEFT:
                supportType = 0;
                break;
            case UnitsAndLimits::FIXED_TOP:
                supportType = 1;
                break;
            case UnitsAndLimits::FIXED_RIGHT:
                supportType = 2;
                break;
            case UnitsAndLimits::FIXED_BOTTOM:
                supportType = 3;
                break;
            case UnitsAndLimits::ROLLER:
                supportType = 4;
                break;
            case UnitsAndLimits::ROLLER_LEFT:
                supportType = 5;
                break;
            case UnitsAndLimits::ROLLER_TOP:
                supportType = 6;
                break;
            case UnitsAndLimits::ROLLER_RIGHT:
                supportType = 7;
                break;
            case UnitsAndLimits::ROLLER_BOTTOM:
                supportType = 8;
                break;
            default:
                break;
        }

        supportJointList.append(qint32(jointNumber));
        supportTypeList.append(supportType);
        supportAngleList.append(support->angle());
    }

    out << supportJointList;
    out << supportTypeList;
    out << supportAngleList;

    //Loads

    bool checkA = !mJointLoadsList.isEmpty();
    bool checkB = !mSupportSettlementsList.isEmpty();
    bool checkC = !mThermalEffectsList.isEmpty();
    bool checkD = !mFabricationErrorsList.isEmpty();
    bool checkE = !mInfluenceLoadsList.isEmpty();

    if (checkA || checkB || checkC || checkD || checkE)
    {
        out << true;

        //Joint Loads

        QList<qint32> loadJointList;
        QList<qreal> loadHorizontalComponentList;
        QList<qreal> loadVerticalComponentList;
        QList<QString> loadPositionList;

        foreach (JointLoad *load, mJointLoadsList)
        {
            Joint *loadJoint = load->loadJoint();
            int jointNumber  = mJointsList.indexOf(loadJoint) + 1;

            loadJointList.append(qint32(jointNumber));
            loadHorizontalComponentList.append(load->horizontalComponent());
            loadVerticalComponentList.append(load->verticalComponent());
            loadPositionList.append(load->position());
        }

        out << loadJointList;
        out << loadHorizontalComponentList;
        out << loadVerticalComponentList;
        out << loadPositionList;

        //Support Settlements

        QList<qint32> settlementSupportList;
        QList<qreal> settlementList;

        foreach (SupportSettlement *supportSettlement, mSupportSettlementsList)
        {
            Support *support  = supportSettlement->settlementSupport();
            int supportNumber = mSupportsList.indexOf(support) + 1;

            settlementSupportList.append(qint32(supportNumber));
            settlementList.append(supportSettlement->settlement());
        }

        out << settlementSupportList;
        out << settlementList;

        //Thermal Effects

        QList<qint32> thermalEffectBarList;
        QList<qreal> temperatureChangeList;
        QList<qreal> thermalCoefficientList;

        foreach (ThermalEffect *thermalEffect, mThermalEffectsList)
        {
            Bar *bar      = thermalEffect->thermalEffectBar();
            int barNumber = mBarsList.indexOf(bar) + 1;

            thermalEffectBarList.append(qint32(barNumber));
            temperatureChangeList.append(thermalEffect->temperatureChange());
            thermalCoefficientList.append(thermalEffect->thermalCoefficient());
        }

        out << thermalEffectBarList;
        out << temperatureChangeList;
        out << thermalCoefficientList;

        //Assembly/Fabrication Errors

        QList<qint32> fabricationErrorBarList;
        QList<qreal> lengthErrorList;

        foreach (FabricationError *fabricationError, mFabricationErrorsList)
        {
            Bar *bar      = fabricationError->fabricationErrorBar();
            int barNumber = mBarsList.indexOf(bar) + 1;

            fabricationErrorBarList.append(qint32(barNumber));
            lengthErrorList.append(fabricationError->lengthError());
        }

        out << fabricationErrorBarList;
        out << lengthErrorList;

        //Influence Loads

        int count = mInfluenceLoadsList.size();
        out << count;

        for (int i = 0; i < count; ++i)
        {
            out << mInfluenceLoadsList.at(i)->name();
            out << mInfluenceLoadsList.at(i)->direction();

            QList<qint32> pathList;

            foreach (int jointNumber, mInfluenceLoadsList.at(i)->path())
            {
                pathList.append(qint32(jointNumber));
            }

            out << pathList;
            out << mInfluenceLoadsList.at(i)->pointLoads();
            out << mInfluenceLoadsList.at(i)->pointLoadPositions();
        }
    }
    else
    {
        out << false;
    }

    bool check = saveFile.commit();

    if (check)
    {
        setCurrentFile(fileName);
    }

    return check;
}

void Solver::setCurrentFile(const QString &fileName)
{
    mCurrentFile = fileName;
    setWindowModified(false);

    QString shownName = tr("untitled");

    if (!mCurrentFile.isEmpty())
    {
        shownName = strippedName(mCurrentFile);

        QSettings settings(tr("TrussTables Project"), tr("TrussTables"));
        QStringList recentFiles = settings.value(tr("recentFiles")).toStringList();
        recentFiles.removeAll(QDir::toNativeSeparators(mCurrentFile));
        recentFiles.prepend(QDir::toNativeSeparators(mCurrentFile));
        mRecentFiles = recentFiles;
        updateRecentFileActions();
        settings.setValue(tr("recentFiles"), mRecentFiles);
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("TrussTables")));
}

void Solver::updateRecentFileActions()
{
    QMutableStringListIterator i(mRecentFiles);

    while (i.hasNext())
    {
        if (!QFile::exists(i.next()))
        {
            i.remove();
        }
    }

    for (int j = 0; j < MAX_RECENT_FILES; ++j)
    {
        if (j < mRecentFiles.size())
        {
            QString s;
            s = tr("&%1 %2").arg(j + 1).arg(mRecentFiles[j]);
            mRecentFileActionsArray[j]->setText(s);
            mRecentFileActionsArray[j]->setData(mRecentFiles[j]);
            mRecentFileActionsArray[j]->setVisible(true);
        }
        else
        {
            mRecentFileActionsArray[j]->setVisible(false);
        }
    }

    mOpenRecentMenu->setEnabled(!mRecentFiles.isEmpty());
}

QString Solver::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void Solver::updateModelViewer()
{
    emit updateModelSignal();
}

void Solver::newFile()
{
    writeSettings();
    Solver *newSolver = new Solver;
    newSolver->resize(this->size());
    newSolver->show();
}

void Solver::open()
{
    if (isWindowModified() || !mCurrentFile.isEmpty())
    {
        writeSettings();
        Solver *newSolver = new Solver;

        if (newSolver->promptForOpenFileName())
        {
            newSolver->show();
        }
        else
        {
            newSolver->destroy();
        }
    }
    else
    {
        promptForOpenFileName();
    }
}

bool Solver::promptForOpenFileName()
{
    QString s1, s2, s3, openFileName;
    s1           = tr("Open TrussTables Model");
    s2           = mLastOpenLocation;
    s3           = tr("TrussTables model files (*.ttmdl)");
    openFileName = QFileDialog::getOpenFileName(this, s1, s2, s3);

    if (openFileName.isEmpty())
    {
        return false;
    }
    else
    {
        QFileInfo fileInfo(openFileName);
        mLastOpenLocation = fileInfo.dir().path();
        return loadFile(openFileName);
    }
}

bool Solver::save()
{
    if (mCurrentFile.isEmpty())
    {
        return saveAs();
    }
    else
    {
        QFileInfo fileInfo(mCurrentFile);
        mLastSaveLocation = fileInfo.dir().path();
        return saveFile(mCurrentFile);
    }
}

bool Solver::saveAs()
{
    QString s1, s2, s3;
    s1 = tr("Save TrussTables Model");
    s2 = mLastSaveLocation;
    s3 = tr("TrussTables model files (*.ttmdl)");
    QFileDialog fileDialog(this, s1, s2, s3);
    fileDialog.setWindowModality(Qt::WindowModal);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setDefaultSuffix(tr("ttmdl"));
    fileDialog.setFileMode(QFileDialog::AnyFile);

    QStringList fileNames;

    if (fileDialog.exec())
    {
        fileNames = fileDialog.selectedFiles();
    }
    else
    {
        return false;
    }

    QFileInfo fileInfo(fileNames.at(0));
    mLastSaveLocation = fileInfo.dir().path();
    return saveFile(fileNames.at(0));
}

void Solver::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action)
    {
        if (isWindowModified() || !mCurrentFile.isEmpty())
        {
            writeSettings();
            Solver *newSolver = new Solver;

            if (newSolver->loadFile(action->data().toString()))
            {
                newSolver->resize(this->size());
                newSolver->show();
            }
            else
            {
                newSolver->destroy();
            }
        }
        else
        {
            loadFile(action->data().toString());
        }
    }
}

void Solver::clearRecentFileActions()
{
    mRecentFiles.clear();
    QSettings settings(tr("TrussTables Project"), tr("TrussTables"));
    settings.setValue(tr("recentFiles"), mRecentFiles);
    updateRecentFileActions();
}

void Solver::setUnits()
{
    if (mJointsList.isEmpty())
    {
        UnitsAndSetupDialog dialog(&mDescriptionText,
                                   &mUnitsAndLimits,
                                   this);

        if (dialog.exec() == QDialog::Accepted)
        {
            updateUnits();
        }
    }
}

void Solver::updateUnits()
{
    mAddJointsDialog->updateUnits();
    mEditJointsDialog->updateUnits();
    mAddBarsDialog->updateUnits();
    mEditBarsDialog->updateUnits();
    mAddJointLoadsDialog->updateUnits();
    mEditJointLoadsDialog->updateUnits();
    mAddSupportSettlementsDialog->updateUnits();
    mEditSupportSettlementsDialog->updateUnits();
    mAddThermalEffectsDialog->updateUnits();
    mEditThermalEffectsDialog->updateUnits();
    mAddFabricationErrorsDialog->updateUnits();
    mEditFabricationErrorsDialog->updateUnits();
    mAddInfluenceLoadsDialog->updateUnits();
    mEditInfluenceLoadsDialog->updateUnits();

    QStandardItem *item = mJointsStandardItemModel->horizontalHeaderItem(0);
    item->setText(tr("x (%1)").arg(mUnitsAndLimits.coordinateUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    item = mJointsStandardItemModel->horizontalHeaderItem(1);
    item->setText(tr("y (%1)").arg(mUnitsAndLimits.coordinateUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        item = mBarsStandardItemModel->horizontalHeaderItem(2);
        item->setText(tr("A (%1)").arg(mUnitsAndLimits.areaUnit()));

        item = mBarsStandardItemModel->horizontalHeaderItem(3);
        item->setText(tr("E (%1)").arg(mUnitsAndLimits.modulusUnit()));

        item = mBarsStandardItemModel->horizontalHeaderItem(4);
        item->setText(tr("W (%1)").arg(mUnitsAndLimits.unitWeightUnit()));
    }

    item = mJointLoadsStandardItemModel->horizontalHeaderItem(1);
    item->setText(tr("H (%1)").arg(mUnitsAndLimits.loadUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    item = mJointLoadsStandardItemModel->horizontalHeaderItem(2);
    item->setText(tr("V (%1)").arg(mUnitsAndLimits.loadUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    item = mSupportSettlementsStandardItemModel->horizontalHeaderItem(1);
    item->setText(tr("%1y (%2)")
                  .arg(QString::fromUtf8("\u0394"))
                  .arg(mUnitsAndLimits.supportSettlementUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    item = mThermalEffectsStandardItemModel->horizontalHeaderItem(1);
    item->setText(tr("%1t (%2)")
                  .arg(QString::fromUtf8("\u0394"))
                  .arg(mUnitsAndLimits.temperatureChangeUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    item = mThermalEffectsStandardItemModel->horizontalHeaderItem(2);
    item->setText(tr("%1 (/%2)")
                  .arg(QString::fromUtf8("\u03B1"))
                  .arg(mUnitsAndLimits.temperatureChangeUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    item = mFabricationErrorsStandardItemModel->horizontalHeaderItem(1);
    item->setText(tr("%1L (%2)")
                  .arg(QString::fromUtf8("\u0394"))
                  .arg(mUnitsAndLimits.lengthErrorUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    item = mInfluenceLoadsStandardItemModel->horizontalHeaderItem(3);
    item->setText(tr("Point Loads (%1)").arg(mUnitsAndLimits.loadUnit()));
    item->setTextAlignment(Qt::AlignCenter);

    item = mJointDeflectionsStandardItemModel->horizontalHeaderItem(1);

    QString string;

    if (mUnitsAndLimits.system() == tr("metric"))
    {
        string = tr("m");
    }
    else
    {
        string = tr("ft");
    }

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        item->setText(tr("%1x (%2)")
                      .arg(QString::fromUtf8("\u0394"))
                      .arg(string));
    }
    else
    {
        item->setText(tr("%1x%2AE (%3)")
                      .arg(QString::fromUtf8("\u0394"))
                      .arg(QString::fromUtf8("\u00D7"))
                      .arg(string));
    }
    item->setTextAlignment(Qt::AlignCenter);

    item = mJointDeflectionsStandardItemModel->horizontalHeaderItem(2);
    if (mBarsStandardItemModel->columnCount() == 5)
    {
        item->setText(tr("%1y (%2)")
                      .arg(QString::fromUtf8("\u0394"))
                      .arg(string));
    }
    else
    {
        item->setText(tr("%1y%2AE (%3)")
                      .arg(QString::fromUtf8("\u0394"))
                      .arg(QString::fromUtf8("\u00D7"))
                      .arg(string));
    }
    item->setTextAlignment(Qt::AlignCenter);

    if (mUnitsAndLimits.system() == tr("metric"))
    {
        string = tr("N");
    }
    else
    {
        string = tr("lb");
    }

    item = mBarLoadsStandardItemModel->horizontalHeaderItem(1);
    item->setText(tr("Load (%1)").arg(string));
    item->setTextAlignment(Qt::AlignCenter);

    item = mSupportReactionsStandardItemModel->horizontalHeaderItem(1);
    item->setText(tr("H (%1)").arg(string));
    item->setTextAlignment(Qt::AlignCenter);
    item = mSupportReactionsStandardItemModel->horizontalHeaderItem(2);
    item->setText(tr("V (%1)").arg(string));
    item->setTextAlignment(Qt::AlignCenter);

    item = mInfluenceLoadResultsStandardItemModel->horizontalHeaderItem(0);
    item->setText(tr("Pmin (%1)").arg(string));
    item->setTextAlignment(Qt::AlignCenter);
    item = mInfluenceLoadResultsStandardItemModel->horizontalHeaderItem(2);
    item->setText(tr("Pmax (%1)").arg(string));
    item->setTextAlignment(Qt::AlignCenter);
}

void Solver::showHelp()
{
    QString s;
    s = tr("file:///") + QCoreApplication::applicationDirPath() + tr("/tt_help/truss_tables.html");
    QDesktopServices::openUrl(QUrl(s, QUrl::TolerantMode));
}

void Solver::showAboutTrussTablesDialog()
{
    AboutTrussTablesDialog dialog(this);
    dialog.exec();
}

void Solver::showAboutQtDialog()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void Solver::showSelectionPanel()
{
    mSelectionDockWidget->show();
}

void Solver::showNotes()
{
    if (mNotesDialog->position() != QPoint(0, 0))
    {
        mNotesDialog->move(mNotesDialog->position());
    }

    mNotesDialog->show();
    mNotesDialog->raise();
    mNotesDialog->activateWindow();
}

void Solver::showModelAreaDialog()
{
    if (mModelAreaDialog->position() != QPoint(0, 0))
    {
        mModelAreaDialog->move(mModelAreaDialog->position());
    }

    mModelAreaDialog->show();
    mModelAreaDialog->raise();
    mModelAreaDialog->activateWindow();
}

void Solver::showExportModelImageDialog()
{
    if (mExportModelImageDialog->position() != QPoint(0, 0))
    {
        mExportModelImageDialog->move(mExportModelImageDialog->position());
    }

    mExportModelImageDialog->show();
    mExportModelImageDialog->raise();
    mExportModelImageDialog->activateWindow();
}

void Solver::showAddJointsDialog()
{
    if (mAddJointsDialog->position() != QPoint(0, 0))
    {
        mAddJointsDialog->move(mAddJointsDialog->position());
    }

    mAddJointsDialog->show();
    mAddJointsDialog->raise();
    mAddJointsDialog->activateWindow();
}

void Solver::showEditJointsDialog()
{
    if (mEditJointsDialog->position() != QPoint(0, 0))
    {
        mEditJointsDialog->move(mEditJointsDialog->position());
    }

    mEditJointsDialog->show();
    mEditJointsDialog->raise();
    mEditJointsDialog->activateWindow();
}

void Solver::showDeleteJointsDialog()
{
    if (mDeleteJointsDialog->position() != QPoint(0, 0))
    {
        mDeleteJointsDialog->move(mDeleteJointsDialog->position());
    }

    mDeleteJointsDialog->show();
    mDeleteJointsDialog->raise();
    mDeleteJointsDialog->activateWindow();
}

void Solver::setCoordinatesLabelText(QString &position)
{
    mCoordinatesLabel->setText(position);
}

void Solver::setJointsSelectionMode(bool checked)
{
    if (checked)
    {
        mJointsTableView->clearSelection();
        mJointsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else
    {
        mJointsTableView->clearSelection();
        mJointsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}

void Solver::normalizeJointCoordinates()
{
    if (mJointsList.isEmpty())
    {
        return;
    }

    qreal xMin = 0.0;
    qreal yMin = 0.0;

    QList<qreal> xCoordinatesList;
    QList<qreal> yCoordinatesList;

    foreach (Joint *joint, mJointsList)
    {
        xCoordinatesList.append(joint->xCoordinate());
        yCoordinatesList.append(joint->yCoordinate());
    }

    xMin = *std::min_element(xCoordinatesList.begin(), xCoordinatesList.end());
    yMin = *std::min_element(yCoordinatesList.begin(), yCoordinatesList.end());

    foreach (Joint *joint, mJointsList)
    {
        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        xCoordinate -= xMin;
        yCoordinate -= yMin;

        joint->setXCoordinate(xCoordinate);
        joint->setYCoordinate(yCoordinate);
    }

    QList<qreal> firstXCoordinatesList;
    QList<qreal> firstYCoordinatesList;
    QList<qreal> secondXCoordinatesList;
    QList<qreal> secondYCoordinatesList;

    if (!mBarsList.isEmpty())
    {
        foreach (Bar *bar, mBarsList)
        {
            Joint *firstJoint  = bar->firstJoint();
            Joint *secondJoint = bar->secondJoint();

            firstXCoordinatesList.append(firstJoint->xCoordinate());
            firstYCoordinatesList.append(firstJoint->yCoordinate());
            secondXCoordinatesList.append(secondJoint->xCoordinate());
            secondYCoordinatesList.append(secondJoint->yCoordinate());

            firstJoint->connectedJoints()->removeOne(secondJoint);
            secondJoint->connectedJoints()->removeOne(firstJoint);

            firstJoint->attachedBars()->removeOne(bar);
            secondJoint->attachedBars()->removeOne(bar);
        }
    }

    QList<qreal> supportJointXCoordinatesList;
    QList<qreal> supportJointYCoordinatesList;

    if (!mSupportsList.isEmpty())
    {
        foreach (Support *support, mSupportsList)
        {
            supportJointXCoordinatesList.append(support->supportJoint()->xCoordinate());
            supportJointYCoordinatesList.append(support->supportJoint()->yCoordinate());
        }
    }

    QList<Point> pointsList;

    foreach (Joint *joint, mJointsList)
    {
        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        Point point(xCoordinate, yCoordinate);

        pointsList.append(point);
    }

    std::stable_sort(pointsList.begin(), pointsList.end());

    foreach (Joint *joint, mJointsList)
    {
        int index   = mJointsList.indexOf(joint);
        Point point = pointsList.at(index);

        qreal xCoordinate = point.xCoordinate();
        qreal yCoordinate = point.yCoordinate();

        joint->setXCoordinate(xCoordinate);
        joint->setYCoordinate(yCoordinate);
    }

    qreal epsilonMagnitudeSmall = 1.0e-12;

    if (!mBarsList.isEmpty())
    {
        foreach (Bar *bar, mBarsList)
        {
            Joint *firstJoint  = 0;
            Joint *secondJoint = 0;

            int index = mBarsList.indexOf(bar);

            qreal firstXCoordinate = firstXCoordinatesList.at(index);
            qreal firstYCoordinate = firstYCoordinatesList.at(index);

            foreach (Joint *joint, mJointsList)
            {
                qreal xDifference = std::fabs(firstXCoordinate - joint->xCoordinate());
                qreal yDifference = std::fabs(firstYCoordinate - joint->yCoordinate());

                bool checkA = xDifference < epsilonMagnitudeSmall;
                bool checkB = yDifference < epsilonMagnitudeSmall;

                if (checkA && checkB)
                {
                    firstJoint = joint;
                    break;
                }
            }

            qreal secondXCoordinate = secondXCoordinatesList.at(index);
            qreal secondYCoordinate = secondYCoordinatesList.at(index);

            foreach (Joint *joint, mJointsList)
            {
                qreal xDifference = std::fabs(secondXCoordinate - joint->xCoordinate());
                qreal yDifference = std::fabs(secondYCoordinate - joint->yCoordinate());

                bool checkA = xDifference < epsilonMagnitudeSmall;
                bool checkB = yDifference < epsilonMagnitudeSmall;

                if (checkA && checkB)
                {
                    secondJoint = joint;
                    break;
                }
            }

            bar->setFirstJoint(firstJoint);
            bar->setSecondJoint(secondJoint);

            firstJoint->connectedJoints()->append(secondJoint);
            secondJoint->connectedJoints()->append(firstJoint);

            firstJoint->attachedBars()->append(bar);
            secondJoint->attachedBars()->append(bar);

            int firstJointNumber  = mJointsList.indexOf(firstJoint) + 1;
            int secondJointNumber = mJointsList.indexOf(secondJoint) + 1;

            QStandardItem *standardItem;

            standardItem = new QStandardItem(QString::number(firstJointNumber));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mBarsStandardItemModel->setItem(index, 0, standardItem);

            standardItem = new QStandardItem(QString::number(secondJointNumber));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mBarsStandardItemModel->setItem(index, 1, standardItem);
        }
    }

    if (!mSupportsList.isEmpty())
    {
        foreach (Joint *joint, mJointsList)
        {
            if (joint->isSupported())
            {
                joint->setSupported(false);
            }
        }

        foreach (Support *support, mSupportsList)
        {
            Joint *supportJoint;
            int index                     = mSupportsList.indexOf(support);
            qreal supportJointXCoordinate = supportJointXCoordinatesList.at(index);
            qreal supportJointYCoordinate = supportJointYCoordinatesList.at(index);

            foreach (Joint *joint, mJointsList)
            {
                qreal xDifference = std::fabs(supportJointXCoordinate - joint->xCoordinate());
                qreal yDifference = std::fabs(supportJointYCoordinate - joint->yCoordinate());

                bool checkA = xDifference < epsilonMagnitudeSmall;
                bool checkB = yDifference < epsilonMagnitudeSmall;

                if (checkA && checkB)
                {
                    supportJoint = joint;
                    break;
                }
            }

            supportJoint->setSupported(true);
            support->setSupportJoint(supportJoint);
            int supportJointNumber = mJointsList.indexOf(supportJoint) + 1;

            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(supportJointNumber));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mSupportsStandardItemModel->setItem(index, 0, standardItem);
        }
    }

    foreach (Joint *joint, mJointsList)
    {
        int jointIndex = mJointsList.indexOf(joint);

        QStandardItem *standardItem;

        QString string = QString::number(joint->xCoordinate(), 'f', mUnitsAndLimits.deltaXDecimals());
        standardItem   = new QStandardItem(string);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mJointsStandardItemModel->setItem(jointIndex, 0, standardItem);

        string       = QString::number(joint->yCoordinate(), 'f', mUnitsAndLimits.deltaYDecimals());
        standardItem = new QStandardItem(string);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mJointsStandardItemModel->setItem(jointIndex, 1, standardItem);
    }
}

bool Solver::hasLoads()
{
    bool checkA = !mJointLoadsList.isEmpty();
    bool checkB = !mSupportSettlementsList.isEmpty();
    bool checkC = !mThermalEffectsList.isEmpty();
    bool checkD = !mFabricationErrorsList.isEmpty();
    bool checkE = !mInfluenceLoadsList.isEmpty();

    if (checkA || checkB || checkC || checkD || checkE)
    {
        QApplication::beep();
        return true;
    }
    else
    {
        return false;
    }
}

void Solver::addJoints(qreal deltaX, qreal deltaY, int count)
{
    if (hasLoads())
    {
        return;
    }

    if (mJointsList.isEmpty())
    {
        Joint *joint = new Joint(0.0, 0.0, this);
        emit enableJointsInput();
        mJointsList.append(joint);

        QList<QStandardItem *> standardItemsList;
        QStandardItem *standardItem = 0;

        QString string = QString::number(joint->xCoordinate(), 'f', mUnitsAndLimits.deltaXDecimals());
        standardItem   = new QStandardItem(string);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);

        string       = QString::number(joint->yCoordinate(), 'f', mUnitsAndLimits.deltaYDecimals());
        standardItem = new QStandardItem(string);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);

        mJointsStandardItemModel->appendRow(standardItemsList);
        mAddJointsDialog->updateJoints();
        mAddBarsDialog->updateFirstJointOptions();
        mEditBarsDialog->updateFirstJointOptions();
        mAddSupportsDialog->updateJoints();
        mAddJointLoadsDialog->updateJoints();
        mEditJointLoadsDialog->updateJoints();
        mUnitsAction->setEnabled(false);
        mModelCheckRequired = true;
        setWindowModified(true);
        updateModelViewer();
    }
    else
    {
        QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();

        if (itemSelectionModel->hasSelection())
        {
            QModelIndexList modelIndexList = itemSelectionModel->selectedRows();

            QList<int> selectedJoints;

            foreach (QModelIndex index, modelIndexList)
            {
                selectedJoints.append(index.row() + 1);
            }

            qreal xMin = 0.0;
            qreal yMin = 0.0;
            qreal xMax = 0.0;
            qreal yMax = 0.0;

            foreach (Joint *joint, mJointsList)
            {
                qreal xCoordinate = joint->xCoordinate();
                qreal yCoordinate = joint->yCoordinate();

                if (xCoordinate > xMax)
                {
                    xMax = xCoordinate;
                }

                if (yCoordinate > yMax)
                {
                    yMax = yCoordinate;
                }
            }

            qreal xRange = xMax - xMin;
            qreal yRange = yMax - yMin;

            QList<int> checksList;

            int counter = 0;

            for (int i = 0; i < count; ++i)
            {
                checksList.clear();

                foreach (int jointNumber, selectedJoints)
                {
                    int jointIndex = jointNumber - 1;

                    qreal pointXCoordinate = mJointsList.at(jointIndex)->xCoordinate();
                    qreal pointYCoordinate = mJointsList.at(jointIndex)->yCoordinate();

                    qreal xCoordinate = pointXCoordinate + (i + 1) * deltaX;
                    qreal yCoordinate = pointYCoordinate + (i + 1) * deltaY;

                    if (xCoordinate > xMax)
                    {
                        xRange = xCoordinate;
                    }

                    if (xCoordinate < xMin)
                    {
                        xRange = xMax - xCoordinate;
                    }

                    bool checkXRange = false;

                    if (xRange < mUnitsAndLimits.maxXCoordinate())
                    {
                        checkXRange = true;
                    }

                    if (yCoordinate > yMax)
                    {
                        yRange = yCoordinate;
                    }

                    if (yCoordinate < yMin)
                    {
                        yRange = yMax - yCoordinate;
                    }

                    bool checkYRange = false;

                    if (yRange < mUnitsAndLimits.maxYCoordinate())
                    {
                        checkYRange = true;
                    }

                    if (checkXRange && checkYRange)
                    {
                        checksList.append(1);
                    }
                    else
                    {
                        checksList.append(0);
                        continue;
                    }

                    bool skip = false;

                    int jointsCount = mJointsList.size();

                    for (int i = 0; i < jointsCount; ++i)
                    {
                        qreal xSeparation = xCoordinate - mJointsList.at(i)->xCoordinate();
                        qreal ySeparation = yCoordinate - mJointsList.at(i)->yCoordinate();

                        qreal separation = std::sqrt(std::pow(xSeparation, 2.0) + std::pow(ySeparation, 2.0));

                        if (separation < mUnitsAndLimits.minBarLength())
                        {
                            skip = true;
                            break;
                        }
                    }

                    if (skip)
                    {
                        continue;
                    }

                    skip                        = false;
                    qreal epsilonMagnitudeSmall = 1.0e-12;

                    foreach (Bar *bar, mBarsList)
                    {
                        qreal x1 = bar->firstJoint()->xCoordinate();
                        qreal y1 = bar->firstJoint()->yCoordinate();
                        qreal x2 = bar->secondJoint()->xCoordinate();
                        qreal y2 = bar->secondJoint()->yCoordinate();

                        qreal firstXComponent  = x2 - x1;
                        qreal firstYComponent  = y2 - y1;
                        qreal secondXComponent = xCoordinate - x1;
                        qreal secondYComponent = yCoordinate - y1;

                        qreal magnitudeCrossProduct = std::fabs(firstXComponent * secondYComponent
                                                                - firstYComponent * secondXComponent);

                        if (magnitudeCrossProduct < epsilonMagnitudeSmall)
                        {
                            qreal quotient = 0.0;

                            if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
                            {
                                quotient = secondYComponent / firstYComponent;
                            }
                            else
                            {
                                quotient = secondXComponent / firstXComponent;
                            }

                            if ((quotient > 0.0) && (quotient < 1.0))
                            {
                                skip = true;
                                break;
                            }
                        }
                    }

                    if (skip)
                    {
                        continue;
                    }

                    Joint *joint = new Joint(xCoordinate, yCoordinate, this);
                    ++counter;
                    mJointsList.append(joint);

                    QList<QStandardItem *> standardItemsList;
                    QStandardItem *standardItem = 0;

                    QString string = QString::number(joint->xCoordinate(), 'f', mUnitsAndLimits.deltaXDecimals());
                    standardItem   = new QStandardItem(string);
                    standardItem->setTextAlignment(Qt::AlignCenter);
                    standardItem->setEditable(false);
                    standardItemsList.append(standardItem);

                    string       = QString::number(joint->yCoordinate(), 'f', mUnitsAndLimits.deltaYDecimals());
                    standardItem = new QStandardItem(string);
                    standardItem->setTextAlignment(Qt::AlignCenter);
                    standardItem->setEditable(false);
                    standardItemsList.append(standardItem);

                    mJointsStandardItemModel->appendRow(standardItemsList);
                }

                if (!checksList.contains(1))
                {
                    if (counter)
                    {
                        mJointsTableView->clearSelection();
                        normalizeJointCoordinates();
                        mJointsTableView->scrollToBottom();
                        mAddJointsDialog->updateJoints();
                        mAddBarsDialog->updateFirstJointOptions();
                        mEditBarsDialog->updateFirstJointOptions();
                        mAddSupportsDialog->updateJoints();
                        mAddJointLoadsDialog->updateJoints();
                        mEditJointLoadsDialog->updateJoints();
                        mModelCheckRequired = true;
                        setWindowModified(true);
                        updateModelViewer();
                    }

                    return;
                }
            }

            if (counter)
            {
                mJointsTableView->clearSelection();
                normalizeJointCoordinates();
                mJointsTableView->scrollToBottom();
                mAddJointsDialog->updateJoints();
                mAddBarsDialog->updateFirstJointOptions();
                mEditBarsDialog->updateFirstJointOptions();
                mAddSupportsDialog->updateJoints();
                mAddJointLoadsDialog->updateJoints();
                mEditJointLoadsDialog->updateJoints();
                mModelCheckRequired = true;
                setWindowModified(true);
                updateModelViewer();
            }

            return;
        }
        else
        {
            qreal xMin = 0.0;
            qreal yMin = 0.0;
            qreal xMax = 0.0;
            qreal yMax = 0.0;

            foreach (Joint *joint, mJointsList)
            {
                qreal xCoordinate = joint->xCoordinate();
                qreal yCoordinate = joint->yCoordinate();

                if (xCoordinate > xMax)
                {
                    xMax = xCoordinate;
                }

                if (yCoordinate > yMax)
                {
                    yMax = yCoordinate;
                }
            }

            qreal xRange = xMax - xMin;
            qreal yRange = yMax - yMin;

            qreal pointXCoordinate = mJointsList.last()->xCoordinate();
            qreal pointYCoordinate = mJointsList.last()->yCoordinate();

            int counter = 0;

            for (int i = 0; i < count; ++i)
            {
                qreal xCoordinate = pointXCoordinate + (i + 1) * deltaX;
                qreal yCoordinate = pointYCoordinate + (i + 1) * deltaY;

                if (xCoordinate > xMax)
                {
                    xRange = xCoordinate;
                }

                if (xCoordinate < xMin)
                {
                    xRange = xMax - xCoordinate;
                }

                bool checkXRange = false;

                if (xRange < mUnitsAndLimits.maxXCoordinate())
                {
                    checkXRange = true;
                }

                if (yCoordinate > yMax)
                {
                    yRange = yCoordinate;
                }

                if (yCoordinate < yMin)
                {
                    yRange = yMax - yCoordinate;
                }

                bool checkYRange = false;

                if (yRange < mUnitsAndLimits.maxYCoordinate())
                {
                    checkYRange = true;
                }

                if (!checkXRange || !checkYRange)
                {
                    break;
                }

                bool skip = false;

                int jointsCount = mJointsList.size();

                for (int i = 0; i < jointsCount; ++i)
                {
                    qreal xSeparation = xCoordinate - mJointsList.at(i)->xCoordinate();
                    qreal ySeparation = yCoordinate - mJointsList.at(i)->yCoordinate();
                    qreal separation  = std::sqrt(std::pow(xSeparation, 2.0) + std::pow(ySeparation, 2.0));

                    if (separation < mUnitsAndLimits.minBarLength())
                    {
                        skip = true;
                        break;
                    }
                }

                if (skip)
                {
                    continue;
                }

                skip = false;
                qreal epsilonMagnitudeSmall = 1.0e-12;

                foreach (Bar *bar, mBarsList)
                {
                    qreal x1 = bar->firstJoint()->xCoordinate();
                    qreal y1 = bar->firstJoint()->yCoordinate();
                    qreal x2 = bar->secondJoint()->xCoordinate();
                    qreal y2 = bar->secondJoint()->yCoordinate();

                    qreal firstXComponent  = x2 - x1;
                    qreal firstYComponent  = y2 - y1;
                    qreal secondXComponent = xCoordinate - x1;
                    qreal secondYComponent = yCoordinate - y1;

                    qreal magnitudeCrossProduct = std::fabs(firstXComponent * secondYComponent
                                                            - firstYComponent * secondXComponent);

                    if (magnitudeCrossProduct < epsilonMagnitudeSmall)
                    {
                        qreal quotient = 0.0;

                        if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
                        {
                            quotient = secondYComponent / firstYComponent;
                        }
                        else
                        {
                            quotient = secondXComponent / firstXComponent;
                        }

                        if ((quotient > 0.0) && (quotient < 1.0))
                        {
                            skip = true;
                            break;
                        }
                    }
                }

                if (skip)
                {
                    continue;
                }

                Joint *joint = new Joint(xCoordinate, yCoordinate, this);
                ++counter;
                mJointsList.append(joint);

                QList<QStandardItem *> standardItemsList;
                QStandardItem *standardItem = 0;

                QString string = QString::number(joint->xCoordinate(), 'f', mUnitsAndLimits.deltaXDecimals());
                standardItem   = new QStandardItem(string);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                string       = QString::number(joint->yCoordinate(), 'f', mUnitsAndLimits.deltaYDecimals());
                standardItem = new QStandardItem(string);
                standardItem->setTextAlignment(Qt::AlignCenter);
                standardItem->setEditable(false);
                standardItemsList.append(standardItem);

                mJointsStandardItemModel->appendRow(standardItemsList);
            }

            if (counter)
            {
                mJointsTableView->clearSelection();
                normalizeJointCoordinates();
                mJointsTableView->scrollToBottom();
                mAddJointsDialog->updateJoints();
                mAddBarsDialog->updateFirstJointOptions();
                mEditBarsDialog->updateFirstJointOptions();
                mAddSupportsDialog->updateJoints();
                mAddJointLoadsDialog->updateJoints();
                mEditJointLoadsDialog->updateJoints();
                mModelCheckRequired = true;
                setWindowModified(true);
                updateModelViewer();
            }
        }
    }
}

int Solver::addJoint(qreal u, qreal v)
{
    //Check for minimum separation between new joint and all existing joints
    int jointsCount = mJointsList.size();

    for (int i = 0; i < jointsCount; ++i)
    {
        qreal jointsXSeparation = u - mJointsList.at(i)->xCoordinate();
        qreal jointsYSeparation = v - mJointsList.at(i)->yCoordinate();

        qreal length = std::sqrt(std::pow(jointsXSeparation, 2.0) + std::pow(jointsYSeparation, 2.0));

        if (length < mUnitsAndLimits.minBarLength())
        {
            return -1;
        }
    }

    //Check for coincidence with existing bar

    qreal epsilonMagnitudeSmall = 1.0e-12;

    foreach (Bar *bar, mBarsList)
    {
        qreal x1 = bar->firstJoint()->xCoordinate();
        qreal y1 = bar->firstJoint()->yCoordinate();
        qreal x2 = bar->secondJoint()->xCoordinate();
        qreal y2 = bar->secondJoint()->yCoordinate();

        qreal firstXComponent  = x2 - x1;
        qreal firstYComponent  = y2 - y1;
        qreal secondXComponent = u - x1;
        qreal secondYComponent = v - y1;

        qreal magnitudeCrossProduct = std::fabs(firstXComponent * secondYComponent
                                                - firstYComponent * secondXComponent);

        if (magnitudeCrossProduct < epsilonMagnitudeSmall)
        {
            qreal quotient = 0.0;

            if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
            {
                quotient = secondYComponent / firstYComponent;
            }
            else
            {
                quotient = secondXComponent / firstXComponent;
            }

            if ((quotient > 0.0) && (quotient < 1.0))
            {
                return -1;
            }
        }
    }

    //Create new joint
    Joint *joint = new Joint(u, v, this);
    mJointsList.append(joint);

    QList<QStandardItem *> standardItemsList;
    QStandardItem *standardItem;

    QString string(QString::number(u, 'f', mUnitsAndLimits.deltaXDecimals()));
    standardItem = new QStandardItem(string);
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemsList.append(standardItem);

    string       = QString::number(v, 'f', mUnitsAndLimits.deltaYDecimals());
    standardItem = new QStandardItem(string);
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemsList.append(standardItem);

    mJointsStandardItemModel->appendRow(standardItemsList);
    mJointsTableView->scrollToBottom();
    mAddJointsDialog->updateJoints();
    mAddBarsDialog->updateFirstJointOptions();
    mEditBarsDialog->updateFirstJointOptions();
    mAddSupportsDialog->updateJoints();
    mAddJointLoadsDialog->updateJoints();
    mEditJointLoadsDialog->updateJoints();
    return mJointsList.indexOf(joint);
}

void Solver::mirrorJoints(QString axis, int axisJointNumber)
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedJoints;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJoints.append(index.row() + 1);
    }

    qreal xMin = 0.0;
    qreal yMin = 0.0;
    qreal xMax = 0.0;
    qreal yMax = 0.0;

    foreach (Joint *joint, mJointsList)
    {
        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        if (xCoordinate > xMax)
        {
            xMax = xCoordinate;
        }

        if (yCoordinate > yMax)
        {
            yMax = yCoordinate;
        }
    }

    qreal xRange = xMax - xMin;
    qreal yRange = yMax - yMin;

    int counter = 0;

    qreal xMirrorAxis = mJointsList.at(axisJointNumber - 1)->xCoordinate();
    qreal yMirrorAxis = mJointsList.at(axisJointNumber - 1)->yCoordinate();

    foreach (int jointNumber, selectedJoints)
    {
        if (jointNumber == axisJointNumber)
        {
            continue;
        }

        int jointIndex = jointNumber - 1;

        qreal pointXCoordinate = mJointsList.at(jointIndex)->xCoordinate();
        qreal pointYCoordinate = mJointsList.at(jointIndex)->yCoordinate();

        qreal xCoordinate = 0.0;
        qreal yCoordinate = 0.0;

        if (axis == tr("Vertical"))
        {
            qreal xOffset = std::fabs(xMirrorAxis - pointXCoordinate);

            if (2.0 * xOffset > mUnitsAndLimits.minBarLength())
            {
                if (xMirrorAxis > pointXCoordinate)
                {
                    xCoordinate = xMirrorAxis + xOffset;
                    yCoordinate = pointYCoordinate;
                }
                else
                {
                    xCoordinate = xMirrorAxis - xOffset;
                    yCoordinate = pointYCoordinate;
                }
            }
            else
            {
                continue;
            }
        }
        else
        {
            qreal yOffset = std::fabs(yMirrorAxis - pointYCoordinate);

            if (2.0 * yOffset > mUnitsAndLimits.minBarLength())
            {
                if (yMirrorAxis > pointYCoordinate)
                {
                    xCoordinate = pointXCoordinate;
                    yCoordinate = yMirrorAxis + yOffset;
                }
                else
                {
                    xCoordinate = pointXCoordinate;
                    yCoordinate = yMirrorAxis - yOffset;
                }
            }
            else
            {
                continue;
            }
        }

        if (xCoordinate > xMax)
        {
            xRange = xCoordinate;
        }

        if (xCoordinate < xMin)
        {
            xRange = xMax - xCoordinate;
        }

        bool checkXRange = false;

        if (xRange < mUnitsAndLimits.maxXCoordinate())
        {
            checkXRange = true;
        }

        if (yCoordinate > yMax)
        {
            yRange = yCoordinate;
        }

        if (yCoordinate < yMin)
        {
            yRange = yMax - yCoordinate;
        }

        bool checkYRange = false;

        if (yRange < mUnitsAndLimits.maxYCoordinate())
        {
            checkYRange = true;
        }

        if (!checkXRange || !checkYRange)
        {
            continue;
        }

        bool skip = false;

        int jointsCount = mJointsList.size();

        for (int i = 0; i < jointsCount; ++i)
        {
            qreal xSeparation = xCoordinate - mJointsList.at(i)->xCoordinate();
            qreal ySeparation = yCoordinate - mJointsList.at(i)->yCoordinate();

            qreal separation = std::sqrt(std::pow(xSeparation, 2.0) + std::pow(ySeparation, 2.0));

            if (separation < mUnitsAndLimits.minBarLength())
            {
                skip = true;
                break;
            }
        }

        if (skip)
        {
            continue;
        }

        skip = false;
        qreal epsilonMagnitudeSmall = 1.0e-12;

        foreach (Bar *bar, mBarsList)
        {
            qreal x1 = bar->firstJoint()->xCoordinate();
            qreal y1 = bar->firstJoint()->yCoordinate();
            qreal x2 = bar->secondJoint()->xCoordinate();
            qreal y2 = bar->secondJoint()->yCoordinate();

            qreal firstXComponent  = x2 - x1;
            qreal firstYComponent  = y2 - y1;
            qreal secondXComponent = xCoordinate - x1;
            qreal secondYComponent = yCoordinate - y1;

            qreal magnitudeCrossProduct = std::fabs(firstXComponent * secondYComponent
                                                    - firstYComponent * secondXComponent);

            if (magnitudeCrossProduct < epsilonMagnitudeSmall)
            {
                qreal quotient = 0.0;

                if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
                {
                    quotient = secondYComponent / firstYComponent;
                }
                else
                {
                    quotient = secondXComponent / firstXComponent;
                }

                if ((quotient > 0.0) && (quotient < 1.0))
                {
                    skip = true;
                    break;
                }
            }
        }

        if (skip)
        {
            continue;
        }

        Joint *joint = new Joint(xCoordinate, yCoordinate, this);
        ++counter;
        mJointsList.append(joint);

        QList<QStandardItem *> standardItemsList;
        QStandardItem *standardItem = 0;

        QString string = QString::number(joint->xCoordinate(), 'f', mUnitsAndLimits.deltaXDecimals());
        standardItem   = new QStandardItem(string);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);

        string       = QString::number(joint->yCoordinate(), 'f', mUnitsAndLimits.deltaYDecimals());
        standardItem = new QStandardItem(string);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);

        mJointsStandardItemModel->appendRow(standardItemsList);
    }

    if (counter)
    {
        mJointsTableView->clearSelection();
        normalizeJointCoordinates();
        mJointsTableView->scrollToBottom();
        mAddJointsDialog->updateJoints();
        mAddBarsDialog->updateFirstJointOptions();
        mEditBarsDialog->updateFirstJointOptions();
        mAddSupportsDialog->updateJoints();
        mAddJointLoadsDialog->updateJoints();
        mEditJointLoadsDialog->updateJoints();
        mModelCheckRequired = true;
        setWindowModified(true);
        updateModelViewer();
    }
}

void Solver::editJoints(qreal deltaX, qreal deltaY)
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedJoints;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJoints.append(index.row() + 1);
    }

    int result = editJoints(deltaX,
                            deltaY,
                            selectedJoints);

    if (result == -1)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Coordinate limits exceeded!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else if (result == -2)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Increase separation from existing joint!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else if (result == -3)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Bar length limits exceeded!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else if (result == -4)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Operation will cause joint to coincide with existing bar"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else if (result == -5)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Operation will cause bar length overlap"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else
    {
        mJointsTableView->clearSelection();
        normalizeJointCoordinates();
        mModelCheckRequired = true;
        setWindowModified(true);
        updateModelViewer();
    }
}

int Solver::editJoints(qreal deltaX,
                       qreal deltaY,
                       QList<int> selectedJoints)
{
    // -----------------------------------------------------------------------------------------------------------------
    // Check if coordinate limits are exceeded
    // -----------------------------------------------------------------------------------------------------------------

    qreal xMin = 0.0;
    qreal yMin = 0.0;
    qreal xMax = 0.0;
    qreal yMax = 0.0;

    foreach (Joint *joint, mJointsList)
    {
        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        if (xCoordinate > xMax)
        {
            xMax = xCoordinate;
        }

        if (yCoordinate > yMax)
        {
            yMax = yCoordinate;
        }
    }

    qreal xRange = xMax - xMin;
    qreal yRange = yMax - yMin;

    foreach (int jointNumber, selectedJoints)
    {
        Joint *joint = mJointsList.at(jointNumber - 1);

        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        xCoordinate += deltaX;
        yCoordinate += deltaY;

        if (xCoordinate > xMax)
        {
            xRange = xCoordinate;
        }

        if (xCoordinate < xMin)
        {
            xRange = xMax - xCoordinate;
        }

        bool checkXRange = false;

        if (xRange < mUnitsAndLimits.maxXCoordinate())
        {
            checkXRange = true;
        }

        if (yCoordinate > yMax)
        {
            yRange = yCoordinate;
        }

        if (yCoordinate < yMin)
        {
            yRange = yMax - yCoordinate;
        }

        bool checkYRange = false;

        if (yRange < mUnitsAndLimits.maxYCoordinate())
        {
            checkYRange = true;
        }

        if (!checkXRange || !checkYRange)
        {
            return -1;
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Check for minimum separation between joints being edited and all other joints
    // -----------------------------------------------------------------------------------------------------------------

    if (selectedJoints.size() != mJointsList.size())
    {
        QList<int> otherJoints;

        int count = mJointsList.size();

        for (int i = 0; i < count; ++i)
        {
            if (selectedJoints.contains(i + 1))
            {
                continue;
            }

            otherJoints.append(i + 1);
        }

        foreach (int firstJointNumber, selectedJoints)
        {
            Joint *firstJoint = mJointsList.at(firstJointNumber - 1);

            qreal x1 = firstJoint->xCoordinate() + deltaX;
            qreal y1 = firstJoint->yCoordinate() + deltaY;

            foreach (int secondJointNumber, otherJoints)
            {
                Joint *secondJoint = mJointsList.at(secondJointNumber - 1);

                qreal x2 = secondJoint->xCoordinate();
                qreal y2 = secondJoint->yCoordinate();

                qreal xSeparation = x2 - x1;
                qreal ySeparation = y2 - y1;

                qreal separationDistance = std::sqrt(std::pow(xSeparation, 2.0) + std::pow(ySeparation, 2.0));

                if (separationDistance < mUnitsAndLimits.minBarLength())
                {
                    return -2;
                }
            }
        }

        // -------------------------------------------------------------------------------------------------------------
        // Check for maximum bar length between each joint being edited and all connected joints
        // -------------------------------------------------------------------------------------------------------------

        foreach (int firstJointNumber, selectedJoints)
        {
            Joint *firstJoint = mJointsList.at(firstJointNumber - 1);

            qreal x1 = firstJoint->xCoordinate() + deltaX;
            qreal y1 = firstJoint->yCoordinate() + deltaY;

            foreach (Joint *secondJoint, *firstJoint->connectedJoints())
            {
                int secondJointNumber = mJointsList.indexOf(secondJoint) + 1;

                if (otherJoints.contains(secondJointNumber))
                {
                    qreal x2 = secondJoint->xCoordinate();
                    qreal y2 = secondJoint->yCoordinate();

                    qreal xSeparation = x2 - x1;
                    qreal ySeparation = y2 - y1;

                    qreal length = std::sqrt(std::pow(xSeparation, 2.0) + std::pow(ySeparation, 2.0));

                    if (!((mUnitsAndLimits.minBarLength() < length) && (mUnitsAndLimits.maxBarLength() > length)))
                    {
                        return -3;
                    }
                }
            }
        }

        // -------------------------------------------------------------------------------------------------------------
        // Check for joint that will coincide with existing bar
        // -------------------------------------------------------------------------------------------------------------

        qreal epsilonMagnitudeSmall = 1.0e-12;
        qreal epsilonMagnitudeLarge = 1.0e+12;

        foreach (int selectedJointNumber, selectedJoints)
        {
            Joint *selectedJoint = mJointsList.at(selectedJointNumber - 1);

            qreal xCoordinate = selectedJoint->xCoordinate() + deltaX;
            qreal yCoordinate = selectedJoint->yCoordinate() + deltaY;

            foreach (Bar *bar, mBarsList)
            {
                int firstJointNumber  = mJointsList.indexOf(bar->firstJoint()) + 1;
                int secondJointNumber = mJointsList.indexOf(bar->secondJoint()) + 1;

                qreal x1;
                qreal y1;
                qreal x2;
                qreal y2;

                if (selectedJoints.contains(firstJointNumber))
                {
                    x1 = bar->firstJoint()->xCoordinate() + deltaX;
                    y1 = bar->firstJoint()->yCoordinate() + deltaY;
                }
                else
                {
                    x1 = bar->firstJoint()->xCoordinate();
                    y1 = bar->firstJoint()->yCoordinate();
                }

                if (selectedJoints.contains(secondJointNumber))
                {
                    x2 = bar->secondJoint()->xCoordinate() + deltaX;
                    y2 = bar->secondJoint()->yCoordinate() + deltaY;
                }
                else
                {
                    x2 = bar->secondJoint()->xCoordinate();
                    y2 = bar->secondJoint()->yCoordinate();
                }

                qreal firstXComponent  = x2 - x1;
                qreal firstYComponent  = y2 - y1;
                qreal secondXComponent = xCoordinate - x1;
                qreal secondYComponent = yCoordinate - y1;

                qreal magnitudeCrossProduct = std::fabs(firstXComponent * secondYComponent
                                                        - firstYComponent * secondXComponent);

                if (magnitudeCrossProduct < epsilonMagnitudeSmall)
                {
                    qreal quotient = 0.0;

                    if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
                    {
                        quotient = secondYComponent / firstYComponent;
                    }
                    else
                    {
                        quotient = secondXComponent / firstXComponent;
                    }

                    if ((quotient > 0.0) && (quotient < 1.0))
                    {
                        return -4;
                    }
                }
            }
        }

        // -------------------------------------------------------------------------------------------------------------
        // Check for any two bars whose lengths will overlap
        // -------------------------------------------------------------------------------------------------------------

        foreach (Bar *firstBar, mBarsList)
        {
            // ---------------------------------------------------------------------------------------------------------
            // End coordinates of first bar
            // ---------------------------------------------------------------------------------------------------------

            qreal firstX1;
            qreal firstY1;
            qreal firstX2;
            qreal firstY2;

            // ---------------------------------------------------------------------------------------------------------
            // End coordinates of second bar
            // ---------------------------------------------------------------------------------------------------------

            qreal secondX1;
            qreal secondY1;
            qreal secondX2;
            qreal secondY2;

            // ---------------------------------------------------------------------------------------------------------
            // Determine end coordinates of first bar
            // ---------------------------------------------------------------------------------------------------------

            int firstJointNumber  = mJointsList.indexOf(firstBar->firstJoint()) + 1;
            int secondJointNumber = mJointsList.indexOf(firstBar->secondJoint()) + 1;

            if (selectedJoints.contains(firstJointNumber))
            {
                firstX1 = firstBar->firstJoint()->xCoordinate() + deltaX;
                firstY1 = firstBar->firstJoint()->yCoordinate() + deltaY;
            }
            else
            {
                firstX1 = firstBar->firstJoint()->xCoordinate();
                firstY1 = firstBar->firstJoint()->yCoordinate();
            }

            if (selectedJoints.contains(secondJointNumber))
            {
                firstX2 = firstBar->secondJoint()->xCoordinate() + deltaX;
                firstY2 = firstBar->secondJoint()->yCoordinate() + deltaY;
            }
            else
            {
                firstX2 = firstBar->secondJoint()->xCoordinate();
                firstY2 = firstBar->secondJoint()->yCoordinate();
            }

            foreach (Bar *secondBar, mBarsList)
            {
                // -----------------------------------------------------------------------------------------------------
                // Skip first bar
                // -----------------------------------------------------------------------------------------------------

                if (secondBar != firstBar)
                {
                    // -------------------------------------------------------------------------------------------------
                    // Determine end coordinates of second bar
                    // -------------------------------------------------------------------------------------------------

                    firstJointNumber  = mJointsList.indexOf(secondBar->firstJoint()) + 1;
                    secondJointNumber = mJointsList.indexOf(secondBar->secondJoint()) + 1;

                    if (selectedJoints.contains(firstJointNumber))
                    {
                        secondX1 = secondBar->firstJoint()->xCoordinate() + deltaX;
                        secondY1 = secondBar->firstJoint()->yCoordinate() + deltaY;
                    }
                    else
                    {
                        secondX1 = secondBar->firstJoint()->xCoordinate();
                        secondY1 = secondBar->firstJoint()->yCoordinate();
                    }

                    if (selectedJoints.contains(secondJointNumber))
                    {
                        secondX2 = secondBar->secondJoint()->xCoordinate() + deltaX;
                        secondY2 = secondBar->secondJoint()->yCoordinate() + deltaY;
                    }
                    else
                    {
                        secondX2 = secondBar->secondJoint()->xCoordinate();
                        secondY2 = secondBar->secondJoint()->yCoordinate();
                    }

                    // -------------------------------------------------------------------------------------------------
                    // Determine x and y components of first and second bars
                    // -------------------------------------------------------------------------------------------------

                    qreal firstXComponent = firstX2 - firstX1;
                    qreal firstYComponent = firstY2 - firstY1;

                    qreal secondXComponent = secondX2 - secondX1;
                    qreal secondYComponent = secondY2 - secondY1;

                    qreal firstSlope;
                    qreal firstIntercept;

                    qreal secondSlope;
                    qreal secondIntercept;

                    // -------------------------------------------------------------------------------------------------
                    // Determine slope and intercept of first and second bar
                    // -------------------------------------------------------------------------------------------------

                    if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
                    {
                        firstSlope     = epsilonMagnitudeLarge;
                        firstIntercept = firstX1;
                    }
                    else
                    {
                        firstSlope     = firstYComponent / firstXComponent;
                        qreal top      = (firstY2 * firstX1 - firstY1 * firstX2);
                        firstIntercept =  top / (-firstXComponent);
                    }

                    if (std::fabs(secondXComponent) < epsilonMagnitudeSmall)
                    {
                        secondSlope     = epsilonMagnitudeLarge;
                        secondIntercept = secondX1;
                    }
                    else
                    {
                        secondSlope     = secondYComponent / secondXComponent;
                        qreal top       = (secondY2 * secondX1 - secondY1 * secondX2);
                        secondIntercept =   top / (-secondXComponent);
                    }

                    // -------------------------------------------------------------------------------------------------
                    // Determine differences of slope and intercept between first and second bar
                    // -------------------------------------------------------------------------------------------------

                    qreal magnitudeSlopeDifference     = std::fabs(firstSlope - secondSlope);
                    qreal magnitudeInterceptDifference = std::fabs(firstIntercept - secondIntercept);

                    bool checkA = ((magnitudeSlopeDifference < epsilonMagnitudeSmall)
                                       && (magnitudeInterceptDifference < epsilonMagnitudeSmall));

                    // -------------------------------------------------------------------------------------------------
                    // Check for possibility of coincidence between first and second bar
                    // -------------------------------------------------------------------------------------------------

                    if (checkA)
                    {
                        std::vector<qreal> xCoordinates;
                        std::vector<qreal> yCoordinates;
                        std::vector<qreal>::iterator position;

                        xCoordinates.push_back(firstX1);
                        xCoordinates.push_back(firstX2);
                        xCoordinates.push_back(secondX1);
                        xCoordinates.push_back(secondX2);
                        yCoordinates.push_back(firstY1);
                        yCoordinates.push_back(firstY2);
                        yCoordinates.push_back(secondY1);
                        yCoordinates.push_back(secondY2);

                        position   = std::min_element(xCoordinates.begin(), xCoordinates.end());
                        qreal minX = *position;

                        position   = std::max_element(xCoordinates.begin(), xCoordinates.end());
                        qreal maxX = *position;

                        position   = std::min_element(yCoordinates.begin(), yCoordinates.end());
                        qreal minY = *position;

                        position   = std::max_element(yCoordinates.begin(), yCoordinates.end());
                        qreal maxY = *position;

                        qreal xComponent = maxX - minX;
                        qreal yComponent = maxY - minY;

                        qreal length       = std::sqrt(std::pow(xComponent, 2.0) + std::pow(yComponent, 2.0));
                        qreal firstLength  = std::sqrt(std::pow(firstXComponent, 2.0)
                                                       + std::pow(firstYComponent, 2.0));
                        qreal secondLength = std::sqrt(std::pow(secondXComponent, 2.0)
                                                       + std::pow(secondYComponent, 2.0));

                        bool checkB = ((firstLength + secondLength) - length) > epsilonMagnitudeSmall;

                        // ---------------------------------------------------------------------------------------------
                        // Confirm coincidence between first and second bar
                        // ---------------------------------------------------------------------------------------------

                        if (checkB)
                        {
                            return -5;
                        }
                    }
                }
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Update joints positions
    // -----------------------------------------------------------------------------------------------------------------

    foreach (int jointNumber, selectedJoints)
    {
        int jointIndex    = jointNumber - 1;
        Joint *joint      = mJointsList.at(jointIndex);
        qreal xCoordinate = joint->xCoordinate() + deltaX;
        qreal yCoordinate = joint->yCoordinate() + deltaY;
        joint->setXCoordinate(xCoordinate);
        joint->setYCoordinate(yCoordinate);
    }

    return 0;
}

void Solver::deleteJoints()
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedJoints;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJoints.append(index.row() + 1);
    }

    QList<Joint *> jointsToDelete;

    int counter = 0;

    foreach (int jointNumber, selectedJoints)
    {
        Joint *joint = mJointsList.at(jointNumber - 1);

        bool checkConnectedJoints = joint->connectedJoints()->isEmpty();
        bool checkAttachedBars    = joint->attachedBars()->isEmpty();
        bool checkSupported       = joint->isSupported();

        if (!checkConnectedJoints || !checkAttachedBars || checkSupported)
        {
            continue;
        }

        jointsToDelete.append(joint);
        ++counter;
    }

    QList<QStandardItem *> itemsToDelete;

    foreach (int jointNumber, selectedJoints)
    {
        Joint *joint = mJointsList.at(jointNumber - 1);

        if (jointsToDelete.contains(joint))
        {
            itemsToDelete.append(mJointsStandardItemModel->item(jointNumber - 1, 0));
        }
    }

    foreach (Joint *joint, jointsToDelete)
    {
        delete joint;
        mJointsList.removeOne(joint);
    }

    if (mJointsList.isEmpty())
    {
        emit disableJointsInput();
        mUnitsAction->setEnabled(true);
    }

    mJointsTableView->clearSelection();

    foreach (QStandardItem *item, itemsToDelete)
    {
        int row = mJointsStandardItemModel->indexFromItem(item).row();
        mJointsStandardItemModel->removeRow(row);
    }

    if (counter > 0)
    {
        mJointsTableView->clearSelection();
        normalizeJointCoordinates();
        mAddJointsDialog->updateJoints();
        mAddBarsDialog->updateFirstJointOptions();
        mEditBarsDialog->updateFirstJointOptions();
        mAddSupportsDialog->updateJoints();
        mAddJointLoadsDialog->updateJoints();
        mEditJointLoadsDialog->updateJoints();
        mModelCheckRequired = true;
        setWindowModified(true);
        updateModelViewer();
    }
}

void Solver::showAddBarsDialog()
{
    if (mAddBarsDialog->position() != QPoint(0, 0))
    {
        mAddBarsDialog->move(mAddBarsDialog->position());
    }

    mAddBarsDialog->show();
    mAddBarsDialog->raise();
    mAddBarsDialog->activateWindow();
}

void Solver::showEditBarsDialog()
{
    if (mEditBarsDialog->position() != QPoint(0, 0))
    {
        mEditBarsDialog->move(mEditBarsDialog->position());
    }

    mEditBarsDialog->show();
    mEditBarsDialog->raise();
    mEditBarsDialog->activateWindow();
}

void Solver::showDeleteBarsDialog()
{
    if (mDeleteBarsDialog->position() != QPoint(0, 0))
    {
        mDeleteBarsDialog->move(mDeleteBarsDialog->position());
    }

    mDeleteBarsDialog->show();
    mDeleteBarsDialog->raise();
    mDeleteBarsDialog->activateWindow();
}

void Solver::setBarsSelectionMode(bool checked)
{
    if (checked)
    {
        mBarsTableView->clearSelection();
        mBarsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else
    {
        mBarsTableView->clearSelection();
        mBarsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}

void Solver::setAreaModulusOption(bool areaModulusOption)
{
    if (areaModulusOption)
    {
        mBarsStandardItemModel->clear();
        mBarsStandardItemModel->setRowCount(0);
        mBarsStandardItemModel->setColumnCount(5);
        createBarsModelHeader();
        mAddBarsDialog->updateAreaModulusFactorOption();
        createJointDeflectionsModelHeader();
        mUpdateSelfWeightOption = true;

        if (mBarsSingleRadioButton->isChecked())
        {
            mBarsTableView->clearSelection();
            mBarsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        }
        else
        {
            mBarsTableView->clearSelection();
            mBarsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        }
    }
}

void Solver::setFactorOption(bool factorOption)
{
    if (factorOption)
    {
        mBarsStandardItemModel->clear();
        mBarsStandardItemModel->setRowCount(0);
        mBarsStandardItemModel->setColumnCount(3);
        createBarsModelHeader();
        mAddBarsDialog->updateAreaModulusFactorOption();
        createJointDeflectionsModelHeader();
        mUpdateSelfWeightOption = true;

        if (mBarsSingleRadioButton->isChecked())
        {
            mBarsTableView->clearSelection();
            mBarsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        }
        else
        {
            mBarsTableView->clearSelection();
            mBarsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        }
    }
}

void Solver::addBars(int firstJoint,
                     int secondJoint,
                     qreal area,
                     qreal modulus,
                     qreal factor,
                     qreal unitWeight,
                     int repeat,
                     qreal deltaX,
                     qreal deltaY)
{
    if (hasLoads())
    {
        return;
    }

    if (firstJoint == 0)
    {
        QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();
        QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
        QList<int> selectedBars;

        foreach (QModelIndex index, modelIndexList)
        {
            selectedBars.append(index.row() + 1);
        }

        QList<int> checksList;

        int initialCount = mBarsList.size();

        qreal xMin = 0.0;
        qreal yMin = 0.0;
        qreal xMax = 0.0;
        qreal yMax = 0.0;

        foreach (Joint *joint, mJointsList)
        {
            qreal xCoordinate = joint->xCoordinate();
            qreal yCoordinate = joint->yCoordinate();

            if (xCoordinate > xMax)
            {
                xMax = xCoordinate;
            }

            if (yCoordinate > yMax)
            {
                yMax = yCoordinate;
            }
        }

        qreal xRange = xMax - xMin;
        qreal yRange = yMax - yMin;

        for (int i = 0; i < repeat; ++i)
        {
            checksList.clear();

            foreach (int barNumber, selectedBars)
            {
                int barIndex = barNumber - 1;

                qreal xFirstJoint = mBarsList.at(barIndex)->firstJoint()->xCoordinate();
                qreal yFirstJoint = mBarsList.at(barIndex)->firstJoint()->yCoordinate();

                qreal xSecondJoint = mBarsList.at(barIndex)->secondJoint()->xCoordinate();
                qreal ySecondJoint = mBarsList.at(barIndex)->secondJoint()->yCoordinate();

                qreal xFirstJointCopy = xFirstJoint + (i + 1) * deltaX;
                qreal yFirstJointCopy = yFirstJoint + (i + 1) * deltaY;

                qreal xSecondJointCopy = xSecondJoint + (i + 1) * deltaX;
                qreal ySecondJointCopy = ySecondJoint + (i + 1) * deltaY;

                QList<QPointF> pointsList;

                pointsList.append(QPointF(xFirstJointCopy, yFirstJointCopy));
                pointsList.append(QPointF(xSecondJointCopy, ySecondJointCopy));

                int counter = 0;

                bool checkA = false;
                bool checkB = false;

                foreach (QPointF point, pointsList)
                {
                    ++counter;

                    qreal xCoordinate = point.x();
                    qreal yCoordinate = point.y();

                    if (xCoordinate > xMax)
                    {
                        xRange = xCoordinate;
                    }

                    if (xCoordinate < xMin)
                    {
                        xRange = xMax - xCoordinate;
                    }

                    bool checkXRange = false;

                    if (xRange < mUnitsAndLimits.maxXCoordinate())
                    {
                        checkXRange = true;
                    }

                    if (yCoordinate > yMax)
                    {
                        yRange = yCoordinate;
                    }

                    if (yCoordinate < yMin)
                    {
                        yRange = yMax - yCoordinate;
                    }

                    bool checkYRange = false;

                    if (yRange < mUnitsAndLimits.maxYCoordinate())
                    {
                        checkYRange = true;
                    }

                    if (checkXRange && checkYRange)
                    {
                        if (counter == 1)
                        {
                            checkA = true;
                        }
                        else
                        {
                            checkB = true;
                        }
                    }
                }

                if (checkA && checkB)
                {
                    checksList.append(1);
                }
                else
                {
                    checksList.append(0);
                    continue;
                }

                qreal areaCopy       = mBarsList.at(barIndex)->area();
                qreal modulusCopy    = mBarsList.at(barIndex)->modulus();
                qreal factorCopy     = mBarsList.at(barIndex)->factor();
                qreal unitWeightCopy = mBarsList.at(barIndex)->unitWeight();

                addBar(xFirstJointCopy,
                       yFirstJointCopy,
                       xSecondJointCopy,
                       ySecondJointCopy,
                       areaCopy,
                       modulusCopy,
                       factorCopy,
                       unitWeightCopy);
            }

            if (!checksList.contains(1))
            {
                break;
            }
        }

        if (mBarsList.size() > initialCount)
        {
            mBarsTableView->clearSelection();
            mBarsTableView->scrollToBottom();
            mModelCheckRequired = true;
            setWindowModified(true);
            normalizeJointCoordinates();
            updateModelViewer();
        }
    }
    else
    {
        qreal xMin = 0.0;
        qreal yMin = 0.0;
        qreal xMax = 0.0;
        qreal yMax = 0.0;

        foreach (Joint *joint, mJointsList)
        {
            qreal xCoordinate = joint->xCoordinate();
            qreal yCoordinate = joint->yCoordinate();

            if (xCoordinate > xMax)
            {
                xMax = xCoordinate;
            }

            if (yCoordinate > yMax)
            {
                yMax = yCoordinate;
            }
        }

        qreal xRange = xMax - xMin;
        qreal yRange = yMax - yMin;

        qreal xFirstJoint = mJointsList.at(firstJoint - 1)->xCoordinate();
        qreal yFirstJoint = mJointsList.at(firstJoint - 1)->yCoordinate();

        qreal xSecondJoint = mJointsList.at(secondJoint - 1)->xCoordinate();
        qreal ySecondJoint = mJointsList.at(secondJoint - 1)->yCoordinate();

        int initialCount = mBarsList.size();

        int count = repeat + 1;

        for (int i = 0; i < count; ++i)
        {
            qreal xFirstJointCopy = xFirstJoint + i * deltaX;
            qreal yFirstJointCopy = yFirstJoint + i * deltaY;

            qreal xSecondJointCopy = xSecondJoint + i * deltaX;
            qreal ySecondJointCopy = ySecondJoint + i * deltaY;

            QList<QPointF> pointsList;

            pointsList.append(QPointF(xFirstJointCopy, yFirstJointCopy));
            pointsList.append(QPointF(xSecondJointCopy, ySecondJointCopy));

            int counter = 0;
            bool checkA = false;
            bool checkB = false;

            foreach (QPointF point, pointsList)
            {
                ++counter;

                qreal xCoordinate = point.x();
                qreal yCoordinate = point.y();

                if (xCoordinate > xMax)
                {
                    xRange = xCoordinate;
                }

                if (xCoordinate < xMin)
                {
                    xRange = xMax - xCoordinate;
                }

                bool checkXRange = false;

                if (xRange < mUnitsAndLimits.maxXCoordinate())
                {
                    checkXRange = true;
                }

                if (yCoordinate > yMax)
                {
                    yRange = yCoordinate;
                }

                if (yCoordinate < yMin)
                {
                    yRange = yMax - yCoordinate;
                }

                bool checkYRange = false;

                if (yRange < mUnitsAndLimits.maxYCoordinate())
                {
                    checkYRange = true;
                }

                if (checkXRange && checkYRange)
                {
                    if (counter == 1)
                    {
                        checkA = true;
                    }
                    else
                    {
                        checkB = true;
                    }
                }
            }

            if (!checkA || !checkB)
            {
                break;
            }

            addBar(xFirstJointCopy,
                   yFirstJointCopy,
                   xSecondJointCopy,
                   ySecondJointCopy,
                   area,
                   modulus,
                   factor,
                   unitWeight);
        }

        if (mBarsList.size() > initialCount)
        {
            mBarsTableView->scrollToBottom();
            mModelCheckRequired = true;
            setWindowModified(true);
            normalizeJointCoordinates();
            updateModelViewer();
        }
    }
}

void Solver::addBar(qreal xFirstJoint,
                    qreal yFirstJoint,
                    qreal xSecondJoint,
                    qreal ySecondJoint,
                    qreal area,
                    qreal modulus,
                    qreal factor,
                    qreal unitWeight)
{
    // -----------------------------------------------------------------------------------------------------------------
    // Check for overlap with existing bars
    // -----------------------------------------------------------------------------------------------------------------

    foreach (Bar *bar, mBarsList)
    {
        // -------------------------------------------------------------------------------------------------------------
        // End coordinates of first bar
        // -------------------------------------------------------------------------------------------------------------

        qreal firstX1 = xFirstJoint;
        qreal firstY1 = yFirstJoint;
        qreal firstX2 = xSecondJoint;
        qreal firstY2 = ySecondJoint;

        // -------------------------------------------------------------------------------------------------------------
        // End coordinates of second bar
        // -------------------------------------------------------------------------------------------------------------

        qreal secondX1 = bar->firstJoint()->xCoordinate();
        qreal secondY1 = bar->firstJoint()->yCoordinate();
        qreal secondX2 = bar->secondJoint()->xCoordinate();
        qreal secondY2 = bar->secondJoint()->yCoordinate();

        // -------------------------------------------------------------------------------------------------------------
        // Determine x and y components of first and second bars
        // -------------------------------------------------------------------------------------------------------------

        qreal firstXComponent = firstX2 - firstX1;
        qreal firstYComponent = firstY2 - firstY1;

        qreal secondXComponent = secondX2 - secondX1;
        qreal secondYComponent = secondY2 - secondY1;

        qreal epsilonMagnitudeSmall = 1.0e-12;
        qreal epsilonMagnitudeLarge = 1.0e+12;

        qreal firstSlope;
        qreal firstIntercept;

        qreal secondSlope;
        qreal secondIntercept;

        // -------------------------------------------------------------------------------------------------------------
        // Determine slope and intercept of first and second bar
        // -------------------------------------------------------------------------------------------------------------

        if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
        {
            firstSlope     = epsilonMagnitudeLarge;
            firstIntercept = firstX1;
        }
        else
        {
            firstSlope     = firstYComponent / firstXComponent;
            qreal top      = (firstY2 * firstX1 - firstY1 * firstX2);
            firstIntercept =  top / (-firstXComponent);
        }

        if (std::fabs(secondXComponent) < epsilonMagnitudeSmall)
        {
            secondSlope     = epsilonMagnitudeLarge;
            secondIntercept = secondX1;
        }
        else
        {
            secondSlope     = secondYComponent / secondXComponent;
            qreal top       = (secondY2 * secondX1 - secondY1 * secondX2);
            secondIntercept =   top / (-secondXComponent);
        }

        // -------------------------------------------------------------------------------------------------------------
        // Determine differences of slope and intercept between first and second bar
        // -------------------------------------------------------------------------------------------------------------

        qreal magnitudeSlopeDifference     = std::fabs(firstSlope - secondSlope);
        qreal magnitudeInterceptDifference = std::fabs(firstIntercept - secondIntercept);

        bool checkA = ((magnitudeSlopeDifference < epsilonMagnitudeSmall)
                           && (magnitudeInterceptDifference < epsilonMagnitudeSmall));

        // -------------------------------------------------------------------------------------------------------------
        // Check for possibility of coincidence between first and second bar
        // -------------------------------------------------------------------------------------------------------------

        if (checkA)
        {
            std::vector<qreal> xCoordinates;
            std::vector<qreal> yCoordinates;
            std::vector<qreal>::iterator position;

            xCoordinates.push_back(firstX1);
            xCoordinates.push_back(firstX2);
            xCoordinates.push_back(secondX1);
            xCoordinates.push_back(secondX2);
            yCoordinates.push_back(firstY1);
            yCoordinates.push_back(firstY2);
            yCoordinates.push_back(secondY1);
            yCoordinates.push_back(secondY2);

            position   = std::min_element(xCoordinates.begin(), xCoordinates.end());
            qreal minX = *position;

            position   = std::max_element(xCoordinates.begin(), xCoordinates.end());
            qreal maxX = *position;

            position   = std::min_element(yCoordinates.begin(), yCoordinates.end());
            qreal minY = *position;

            position   = std::max_element(yCoordinates.begin(), yCoordinates.end());
            qreal maxY = *position;

            qreal xComponent = maxX - minX;
            qreal yComponent = maxY - minY;

            qreal length       = std::sqrt(std::pow(xComponent, 2.0) + std::pow(yComponent, 2.0));
            qreal firstLength  = std::sqrt(std::pow(firstXComponent, 2.0)
                                           + std::pow(firstYComponent, 2.0));
            qreal secondLength = std::sqrt(std::pow(secondXComponent, 2.0)
                                           + std::pow(secondYComponent, 2.0));

            bool checkB = ((firstLength + secondLength) - length) > epsilonMagnitudeSmall;

            // ---------------------------------------------------------------------------------------------------------
            // Confirm coincidence between first and second bar
            // ---------------------------------------------------------------------------------------------------------

            if (checkB)
            {
                return;
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Check for coincidence with existing joint
    // -----------------------------------------------------------------------------------------------------------------

    qreal epsilonMagnitudeSmall = 1.0e-12;

    foreach (Joint *joint, mJointsList)
    {
        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        qreal firstXComponent  = xSecondJoint - xFirstJoint;
        qreal firstYComponent  = ySecondJoint - yFirstJoint;
        qreal secondXComponent = xCoordinate - xFirstJoint;
        qreal secondYComponent = yCoordinate - yFirstJoint;

        qreal magnitudeCrossProduct = std::fabs(firstXComponent * secondYComponent
                                                - firstYComponent * secondXComponent);

        if (magnitudeCrossProduct < epsilonMagnitudeSmall)
        {
            qreal quotient = 0.0;

            if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
            {
                quotient = secondYComponent / firstYComponent;
            }
            else
            {
                quotient = secondXComponent / firstXComponent;
            }

            if ((quotient > 0.0) && (quotient < 1.0))
            {
                return;
            }
        }
    }

    Joint *firstJoint;
    Joint *secondJoint;
    Bar *bar;

    bool checkA = false;
    bool checkB = false;

    foreach (Joint *joint, mJointsList)
    {
        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        bool checkC = std::fabs(xCoordinate - xFirstJoint) < epsilonMagnitudeSmall;
        bool checkD = std::fabs(yCoordinate - yFirstJoint) < epsilonMagnitudeSmall;

        bool checkE = std::fabs(xCoordinate - xSecondJoint) < epsilonMagnitudeSmall;
        bool checkF = std::fabs(yCoordinate - ySecondJoint) < epsilonMagnitudeSmall;

        if (checkC && checkD)
        {
            checkA     = true;
            firstJoint = joint;
        }

        if (checkE && checkF)
        {
            checkB      = true;
            secondJoint = joint;
        }

        if (checkA && checkB)
        {
            break;
        }
    }

    if (!checkA)
    {
        int index = addJoint(xFirstJoint, yFirstJoint);

        if (index >= 0)
        {
            firstJoint = mJointsList.at(index);
        }
        else
        {
            return;
        }
    }

    if (!checkB)
    {
        int index = addJoint(xSecondJoint, ySecondJoint);

        if (index >= 0)
        {
            secondJoint = mJointsList.at(index);
        }
        else
        {
            return;
        }
    }

    if (firstJoint->connectedJoints()->contains(secondJoint))
    {
        return;
    }

    if (mBarsList.size() == 0)
    {
        mFactorRadioButton->setEnabled(false);
        mValueRadioButton->setEnabled(false);
    }

    firstJoint->connectedJoints()->append(secondJoint);
    secondJoint->connectedJoints()->append(firstJoint);

    bar = new Bar(firstJoint, secondJoint, area, modulus, factor, unitWeight, this);
    mBarsList.append(bar);

    firstJoint->attachedBars()->append(bar);
    secondJoint->attachedBars()->append(bar);

    QList<QStandardItem *> standardItemsList;
    QStandardItem *standardItem;

    standardItem = new QStandardItem(QString::number(mJointsList.indexOf(firstJoint) + 1));
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemsList.append(standardItem);

    standardItem = new QStandardItem(QString::number(mJointsList.indexOf(secondJoint) + 1));
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemsList.append(standardItem);

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        standardItem = new QStandardItem(QString::number(bar->area(), 'f', mUnitsAndLimits.areaDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);

        standardItem = new QStandardItem(QString::number(bar->modulus(), 'f', mUnitsAndLimits.modulusDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);

        standardItem = new QStandardItem(QString::number(bar->unitWeight(), 'f', mUnitsAndLimits.unitWeightDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
    }
    else
    {
        standardItem = new QStandardItem(QString::number(bar->factor(), 'f', mUnitsAndLimits.factorDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
    }

    mBarsStandardItemModel->appendRow(standardItemsList);
}

void Solver::mirrorBars(QString axis, int axisJointNumber)
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedBars;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedBars.append(index.row() + 1);
    }

    qreal xMin = 0.0;
    qreal yMin = 0.0;
    qreal xMax = 0.0;
    qreal yMax = 0.0;

    foreach (Joint *joint, mJointsList)
    {
        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        if (xCoordinate > xMax)
        {
            xMax = xCoordinate;
        }

        if (yCoordinate > yMax)
        {
            yMax = yCoordinate;
        }
    }

    qreal xRange = xMax - xMin;
    qreal yRange = yMax - yMin;

    int initialCount = 0;

    qreal xMirrorAxis = mJointsList.at(axisJointNumber - 1)->xCoordinate();
    qreal yMirrorAxis = mJointsList.at(axisJointNumber - 1)->yCoordinate();

    qreal epsilonMagnitudeSmall = 1.0e-12;

    foreach (int barNumber, selectedBars)
    {
        int barIndex = barNumber - 1;

        qreal xFirstJoint = mBarsList.at(barIndex)->firstJoint()->xCoordinate();
        qreal yFirstJoint = mBarsList.at(barIndex)->firstJoint()->yCoordinate();

        qreal xSecondJoint = mBarsList.at(barIndex)->secondJoint()->xCoordinate();
        qreal ySecondJoint = mBarsList.at(barIndex)->secondJoint()->yCoordinate();

        QList<QPointF> pointsList;

        pointsList.append(QPointF(xFirstJoint, yFirstJoint));
        pointsList.append(QPointF(xSecondJoint, ySecondJoint));

        qreal xFirstJointCopy = 0.0;
        qreal yFirstJointCopy = 0.0;

        qreal xSecondJointCopy = 0.0;
        qreal ySecondJointCopy = 0.0;

        bool checkA = false;
        bool checkB = false;

        foreach (QPointF point, pointsList)
        {
            bool checkC = false;
            int index   = pointsList.indexOf(point);

            qreal pointXCoordinate = point.x();
            qreal pointYCoordinate = point.y();

            qreal xCoordinate = 0.0;
            qreal yCoordinate = 0.0;

            if (axis == tr("Vertical"))
            {
                qreal xOffset = std::fabs(xMirrorAxis - pointXCoordinate);

                if (2.0 * xOffset > mUnitsAndLimits.minBarLength())
                {
                    if (xMirrorAxis > pointXCoordinate)
                    {
                        xCoordinate = xMirrorAxis + xOffset;
                        yCoordinate = pointYCoordinate;
                    }
                    else
                    {
                        xCoordinate = xMirrorAxis - xOffset;
                        yCoordinate = pointYCoordinate;
                    }
                }
                else if (xOffset < epsilonMagnitudeSmall)
                {
                    xCoordinate = pointXCoordinate;
                    yCoordinate = pointYCoordinate;
                }
                else
                {
                    checkC = true;
                }
            }
            else
            {
                qreal yOffset = std::fabs(yMirrorAxis - pointYCoordinate);

                if (2.0 * yOffset > mUnitsAndLimits.minBarLength())
                {
                    if (yMirrorAxis > pointYCoordinate)
                    {
                        xCoordinate = pointXCoordinate;
                        yCoordinate = yMirrorAxis + yOffset;
                    }
                    else
                    {
                        xCoordinate = pointXCoordinate;
                        yCoordinate = yMirrorAxis - yOffset;
                    }
                }
                else if (yOffset < epsilonMagnitudeSmall)
                {
                    xCoordinate = pointXCoordinate;
                    yCoordinate = pointYCoordinate;
                }
                else
                {
                    checkC = true;
                }
            }

            if (checkC)
            {
                if (index == 0)
                {
                    checkA = true;
                }
                else
                {
                    checkB = true;
                }
            }
            else
            {
                if (index == 0)
                {
                    xFirstJointCopy = xCoordinate;
                    yFirstJointCopy = yCoordinate;
                }
                else
                {
                    xSecondJointCopy = xCoordinate;
                    ySecondJointCopy = yCoordinate;
                }
            }
        }

        if (checkA || checkB)
        {
            continue;
        }

        checkA = false;
        checkB = false;

        pointsList.clear();
        pointsList.append(QPointF(xFirstJointCopy, yFirstJointCopy));
        pointsList.append(QPointF(xSecondJointCopy, ySecondJointCopy));

        foreach (QPointF point, pointsList)
        {
            int index = pointsList.indexOf(point);

            qreal xCoordinate = point.x();
            qreal yCoordinate = point.y();

            if (xCoordinate > xMax)
            {
                xRange = xCoordinate;
            }

            if (xCoordinate < xMin)
            {
                xRange = xMax - xCoordinate;
            }

            bool checkXRange = false;

            if (xRange < mUnitsAndLimits.maxXCoordinate())
            {
                checkXRange = true;
            }

            if (yCoordinate > yMax)
            {
                yRange = yCoordinate;
            }

            if (yCoordinate < yMin)
            {
                yRange = yMax - yCoordinate;
            }

            bool checkYRange = false;

            if (yRange < mUnitsAndLimits.maxYCoordinate())
            {
                checkYRange = true;
            }

            if (!checkXRange || !checkYRange)
            {
                if (index == 0)
                {
                    checkA = true;
                }
                else
                {
                    checkB = true;
                }
            }
        }

        if (checkA || checkB)
        {
            continue;
        }

        qreal areaCopy       = mBarsList.at(barIndex)->area();
        qreal modulusCopy    = mBarsList.at(barIndex)->modulus();
        qreal factorCopy     = mBarsList.at(barIndex)->factor();
        qreal unitWeightCopy = mBarsList.at(barIndex)->unitWeight();

        addBar(xFirstJointCopy,
               yFirstJointCopy,
               xSecondJointCopy,
               ySecondJointCopy,
               areaCopy,
               modulusCopy,
               factorCopy,
               unitWeightCopy);
    }

    if (mBarsList.size() > initialCount)
    {
        mBarsTableView->clearSelection();
        mBarsTableView->scrollToBottom();
        mModelCheckRequired = true;
        setWindowModified(true);
        normalizeJointCoordinates();
        updateModelViewer();
    }
}

void Solver::linkBars(qreal area,
                      qreal modulus,
                      qreal factor,
                      qreal unitWeight)
{
    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedJoints;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJoints.append(index.row() + 1);
    }

    if (selectedJoints.size() == 1)
    {
        return;
    }

    int count   = selectedJoints.size();
    bool checkA = false;

    foreach (int jointNumber, selectedJoints)
    {
        Joint *jointA;
        Joint *jointB;

        int index = selectedJoints.indexOf(jointNumber);

        if (index < (count - 1))
        {
            int jointAIndex = jointNumber - 1;
            int jointBIndex = selectedJoints.at(index + 1) - 1;

            jointA = mJointsList.at(jointAIndex);
            jointB = mJointsList.at(jointBIndex);

            qreal xCoordinateA = jointA->xCoordinate();
            qreal yCoordinateA = jointA->yCoordinate();

            qreal xCoordinateB = jointB->xCoordinate();
            qreal yCoordinateB = jointB->yCoordinate();

            qreal deltaX = xCoordinateB - xCoordinateA;
            qreal deltaY = yCoordinateB - yCoordinateA;

            qreal separationDistace = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));

            if (!((separationDistace > mUnitsAndLimits.minBarLength())
                  && (separationDistace < mUnitsAndLimits.maxBarLength())))
            {
                checkA = true;
                break;
            }
        }
    }

    if (checkA)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Bar length limits exceeded!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    qreal epsilonMagnitudeSmall = 1.0e-12;
    bool checkB                 = false;

    for (int i = 0; i < (count - 2); ++i)
    {
        Joint *jointA;
        Joint *jointB;
        Joint *jointC;

        int indexA = i;
        int indexB = i + 1;
        int indexC = i + 2;

        jointA = mJointsList.at(selectedJoints.at(indexA) - 1);
        jointB = mJointsList.at(selectedJoints.at(indexB) - 1);
        jointC = mJointsList.at(selectedJoints.at(indexC) - 1);

        qreal aX = jointB->xCoordinate() - jointA->xCoordinate();
        qreal aY = jointB->yCoordinate() - jointA->yCoordinate();

        qreal bX = jointC->xCoordinate() - jointB->xCoordinate();
        qreal bY = jointC->yCoordinate() - jointB->yCoordinate();

        qreal magnitudeCrossProduct = std::fabs(aX * bY - aY * bX);

        if (magnitudeCrossProduct < epsilonMagnitudeSmall)
        {
            qreal quotient = 0.0;

            if (std::fabs(aX) < epsilonMagnitudeSmall)
            {
                quotient = bY / aY;
            }
            else
            {
                quotient = bX / aX;
            }

            if ((quotient > 0.0) && (quotient < 1.0))
            {
                checkB = true;
                break;
            }
        }
    }

    if (checkB)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Operation will cause bar length overlap"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    foreach (int jointNumber, selectedJoints)
    {
        Joint *firstJoint;
        Joint *secondJoint;

        int index = selectedJoints.indexOf(jointNumber);

        if (index < (count - 1))
        {
            int firstJointIndex  = jointNumber - 1;
            int secondJointIndex = selectedJoints.at(index + 1) - 1;

            firstJoint  = mJointsList.at(firstJointIndex);
            secondJoint = mJointsList.at(secondJointIndex);

            qreal xFirstJoint = firstJoint->xCoordinate();
            qreal yFirstJoint = firstJoint->yCoordinate();

            qreal xSecondJoint = secondJoint->xCoordinate();
            qreal ySecondJoint = secondJoint->yCoordinate();

            addBar(xFirstJoint,
                   yFirstJoint,
                   xSecondJoint,
                   ySecondJoint,
                   area,
                   modulus,
                   factor,
                   unitWeight);
        }
    }

    mJointsTableView->clearSelection();
    mBarsTableView->scrollToBottom();
    updateModelViewer();
}

void Solver::editBar(int firstJointNumber,
                     int secondJointNumber,
                     qreal area,
                     qreal modulus,
                     qreal factor,
                     qreal unitWeight)
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedBars;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedBars.append(index.row() + 1);
    }

    if (selectedBars.size() == 1)
    {
        Bar *bar   = mBarsList.at(selectedBars.first() - 1);
        int indexA = bar->firstJoint()->attachedBars()->indexOf(bar);
        int indexB = bar->secondJoint()->attachedBars()->indexOf(bar);
        bar->firstJoint()->attachedBars()->removeAt(indexA);
        bar->secondJoint()->attachedBars()->removeAt(indexB);
        Joint *firstJoint  = bar->firstJoint();
        Joint *secondJoint = bar->secondJoint();
        int indexC = bar->firstJoint()->connectedJoints()->indexOf(secondJoint);
        int indexD = bar->secondJoint()->connectedJoints()->indexOf(firstJoint);
        bar->firstJoint()->connectedJoints()->removeAt(indexC);
        bar->secondJoint()->connectedJoints()->removeAt(indexD);
        int barIndex = mBarsList.indexOf(bar);

        Joint *newFirstJoint  = mJointsList.at(firstJointNumber - 1);
        Joint *newSecondJoint = mJointsList.at(secondJointNumber - 1);
        bar->setFirstJoint(newFirstJoint);
        bar->setSecondJoint(newSecondJoint);
        newFirstJoint->attachedBars()->append(bar);
        newSecondJoint->attachedBars()->append(bar);
        newFirstJoint->connectedJoints()->append(newSecondJoint);
        newSecondJoint->connectedJoints()->append(newFirstJoint);

        foreach (Support *suppot, mSupportsList)
        {
            Joint *joint                     = suppot->supportJoint();
            UnitsAndLimits::SupportType type = suppot->type();

            bool checkA = joint->attachedBars()->size() == 1;
            bool checkB = (type == UnitsAndLimits::ROLLER)
                    || (type == UnitsAndLimits::ROLLER_LEFT)
                    || (type == UnitsAndLimits::ROLLER_TOP)
                    || (type == UnitsAndLimits::ROLLER_RIGHT)
                    || (type == UnitsAndLimits::ROLLER_BOTTOM);

            if (checkA && checkB)
            {
                bar->setFirstJoint(firstJoint);
                bar->setSecondJoint(secondJoint);

                newFirstJoint->attachedBars()->removeOne(bar);
                newSecondJoint->attachedBars()->removeOne(bar);
                newFirstJoint->connectedJoints()->removeOne(newSecondJoint);
                newSecondJoint->connectedJoints()->removeOne(newFirstJoint);

                firstJoint->attachedBars()->append(bar);
                secondJoint->attachedBars()->append(bar);
                firstJoint->connectedJoints()->append(secondJoint);
                secondJoint->connectedJoints()->append(firstJoint);
                return;
            }
        }

        bar->setArea(area);
        bar->setModulus(modulus);
        bar->setFactor(factor);
        bar->setUnitWeight(unitWeight);

        //Updates Table
        mBarsTableView->clearSelection();
        QStandardItem *standardItem;

        standardItem = new QStandardItem(QString::number(firstJointNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mBarsStandardItemModel->setItem(barIndex, 0, standardItem);
        standardItem = new QStandardItem(QString::number(secondJointNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mBarsStandardItemModel->setItem(barIndex, 1, standardItem);

        if (mBarsStandardItemModel->columnCount() == 5)
        {
            standardItem = new QStandardItem(QString::number(area, 'f', mUnitsAndLimits.areaDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mBarsStandardItemModel->setItem(barIndex, 2, standardItem);
            standardItem = new QStandardItem(QString::number(modulus, 'f', mUnitsAndLimits.modulusDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mBarsStandardItemModel->setItem(barIndex, 3, standardItem);
            standardItem = new QStandardItem(QString::number(unitWeight, 'f', mUnitsAndLimits.unitWeightDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mBarsStandardItemModel->setItem(barIndex, 4,standardItem);
        }
        else
        {
            standardItem = new QStandardItem(QString::number(factor, 'f', mUnitsAndLimits.factorDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mBarsStandardItemModel->setItem(barIndex, 2, standardItem);
        }

        updateModelViewer();
        mModelCheckRequired = true;
        setWindowModified(true);
    }
    else
    {
        mBarsTableView->clearSelection();

        foreach (int number, selectedBars)
        {
            int index = number - 1;

            if (mBarsStandardItemModel->columnCount() == 5)
            {
                editBar(index,
                        area,
                        modulus,
                        0.0,
                        unitWeight);
            }
            else
            {
                editBar(index,
                        0.0,
                        0.0,
                        factor,
                        0.0);
            }
        }
    }
}

void Solver::editBar(int barIndex,
                     qreal area,
                     qreal modulus,
                     qreal factor,
                     qreal unitWeight)
{
    Bar *bar = mBarsList.at(barIndex);

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        bar->setArea(area);
        bar->setModulus(modulus);
        bar->setUnitWeight(unitWeight);
    }
    else
    {
        bar->setFactor(factor);
    }

    QStandardItem *standardItem;

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        standardItem = new QStandardItem(QString::number(area, 'f', mUnitsAndLimits.areaDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mBarsStandardItemModel->setItem(barIndex, 2, standardItem);
        standardItem = new QStandardItem(QString::number(modulus, 'f', mUnitsAndLimits.modulusDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mBarsStandardItemModel->setItem(barIndex, 3, standardItem);
        standardItem = new QStandardItem(QString::number(unitWeight, 'f', mUnitsAndLimits.unitWeightDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mBarsStandardItemModel->setItem(barIndex, 4, standardItem);
    }
    else
    {
        standardItem = new QStandardItem(QString::number(factor, 'f', mUnitsAndLimits.factorDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mBarsStandardItemModel->setItem(barIndex, 2, standardItem);
    }
}

void Solver::deleteBar()
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedBars;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedBars.append(index.row() + 1);
    }

    QList<int> barsToDelete;
    QList<Bar *> barsToRemove;
    QList<QStandardItem *> itemsToRemove;

    foreach (int barNumber, selectedBars)
    {
        Bar *bar   = mBarsList.at(barNumber - 1);
        bool check = false;

        foreach (Support *support, mSupportsList)
        {
            if (support->supportJoint() == bar->firstJoint())
            {
                check = true;
                break;
            }

            if (support->supportJoint() == bar->secondJoint())
            {
                check = true;
                break;
            }
        }

        if (check)
        {
            continue;
        }

        barsToDelete.append(barNumber - 1);
        barsToRemove.append(bar);
        itemsToRemove.append(mBarsStandardItemModel->item(barNumber - 1, 0));
    }

    mBarsTableView->clearSelection();

    foreach (int barIndex, barsToDelete)
    {
        deleteBar(barIndex);
    }

    foreach (Bar *bar, barsToRemove)
    {
        mBarsList.removeOne(bar);
    }

    foreach (QStandardItem *item, itemsToRemove)
    {
        int row = mBarsStandardItemModel->indexFromItem(item).row();
        mBarsStandardItemModel->removeRow(row);
    }

    if (mBarsList.size() == 0)
    {
        mFactorRadioButton->setEnabled(true);
        mValueRadioButton->setEnabled(true);
    }

    mModelCheckRequired = true;
    setWindowModified(true);
    updateModelViewer();
}

void Solver::deleteBar(int barIndex)
{
    Bar *bar           = mBarsList.at(barIndex);
    Joint *firstJoint  = bar->firstJoint();
    Joint *secondJoint = bar->secondJoint();
    int indexA = firstJoint->attachedBars()->indexOf(bar);
    firstJoint->attachedBars()->removeAt(indexA);
    int indexB = secondJoint->attachedBars()->indexOf(bar);
    secondJoint->attachedBars()->removeAt(indexB);
    int indexC = firstJoint->connectedJoints()->indexOf(secondJoint);
    firstJoint->connectedJoints()->removeAt(indexC);
    int indexD = secondJoint->connectedJoints()->indexOf(firstJoint);
    secondJoint->connectedJoints()->removeAt(indexD);

    delete bar;
}

void Solver::showAddSupportsDialog()
{
    if (mAddSupportsDialog->position() != QPoint(0, 0))
    {
        mAddSupportsDialog->move(mAddSupportsDialog->position());
    }

    mAddSupportsDialog->show();
    mAddSupportsDialog->raise();
    mAddSupportsDialog->activateWindow();
}

void Solver::showEditSupportsDialog()
{
    if (mEditSupportsDialog->position() != QPoint(0, 0))
    {
        mEditSupportsDialog->move(mEditSupportsDialog->position());
    }

    mEditSupportsDialog->show();
    mEditSupportsDialog->raise();
    mEditSupportsDialog->activateWindow();
}

void Solver::showDeleteSupportsDialog()
{
    if (mDeleteSupportsDialog->position() != QPoint(0, 0))
    {
        mDeleteSupportsDialog->move(mDeleteSupportsDialog->position());
    }

    mDeleteSupportsDialog->show();
    mDeleteSupportsDialog->raise();
    mDeleteSupportsDialog->activateWindow();
}

void Solver::showScaleForcesDialog()
{
    if (mScaleForcesDialog->position() != QPoint(0, 0))
    {
        mScaleForcesDialog->move(mScaleForcesDialog->position());
    }

    mScaleForcesDialog->show();
    mScaleForcesDialog->raise();
    mScaleForcesDialog->activateWindow();
}

void Solver::setSupportsSelectionMode(bool checked)
{
    if (checked)
    {
        mSupportsTableView->clearSelection();
        mSupportsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else
    {
        mSupportsTableView->clearSelection();
        mSupportsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}

void Solver::addSupports(int supportJointNumber,
                         UnitsAndLimits::SupportType type,
                         qreal angle)
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();

    if (itemSelectionModel->hasSelection())
    {
        QList<QModelIndex> modelIndexList = itemSelectionModel->selectedRows();
        QList<int> selectedJoints;

        foreach (QModelIndex index, modelIndexList)
        {
            selectedJoints.append(index.row() + 1);
        }

        int count = 0;

        foreach (int jointNumber, selectedJoints)
        {
            if (mJointsList.at(jointNumber - 1)->isSupported())
            {
                continue;
            }

            if (mJointsList.at(jointNumber - 1)->attachedBars()->size() == 1)
            {
                bool checkA = (type != UnitsAndLimits::FIXED_LEFT);
                bool checkB = (type != UnitsAndLimits::FIXED_TOP);
                bool checkC = (type != UnitsAndLimits::FIXED_RIGHT);
                bool checkD = (type != UnitsAndLimits::FIXED_BOTTOM);

                if (checkA && checkB && checkC && checkD)
                {
                    continue;
                }
            }

            if (mJointsList.at(jointNumber - 1)->attachedBars()->isEmpty())
            {
                continue;
            }

            ++count;

            Support *support;
            QString supportTypeName;
            Joint *joint = mJointsList.at(jointNumber - 1);

            switch (type)
            {
                case UnitsAndLimits::FIXED_LEFT:
                case UnitsAndLimits::FIXED_TOP:
                case UnitsAndLimits::FIXED_RIGHT:
                case UnitsAndLimits::FIXED_BOTTOM:
                    supportTypeName = QString(tr("fixed"));
                    break;
                case UnitsAndLimits::ROLLER:
                    supportTypeName = QString(tr("inclined roller"));
                    break;
                case UnitsAndLimits::ROLLER_LEFT:
                case UnitsAndLimits::ROLLER_TOP:
                case UnitsAndLimits::ROLLER_RIGHT:
                case UnitsAndLimits::ROLLER_BOTTOM:
                    supportTypeName = QString(tr("roller"));
                    break;
                default:
                    break;
            }

            support = new Support(type,
                                  angle,
                                  joint,
                                  this);

            joint->setSupported(true);

            mSupportsList.append(support);

            QList<QStandardItem *> standardItemsList;
            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(jointNumber));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            standardItem = new QStandardItem(supportTypeName);
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            standardItem = new QStandardItem(QString::number(angle, 'f', mUnitsAndLimits.supportAngleDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);

            mSupportsStandardItemModel->appendRow(standardItemsList);
            mSupportsTableView->scrollToBottom();
        }

        if (count)
        {
            mJointsTableView->clearSelection();
            updateModelViewer();
            mModelCheckRequired = true;
            setWindowModified(true);
        }
    }
    else
    {
        if (mJointsList.at(supportJointNumber - 1)->isSupported())
        {
            return;
        }

        if (mJointsList.at(supportJointNumber - 1)->attachedBars()->size() == 1)
        {
            bool checkA = (type != UnitsAndLimits::FIXED_LEFT);
            bool checkB = (type != UnitsAndLimits::FIXED_TOP);
            bool checkC = (type != UnitsAndLimits::FIXED_RIGHT);
            bool checkD = (type != UnitsAndLimits::FIXED_BOTTOM);

            if (checkA && checkB && checkC && checkD)
            {
                return;
            }
        }

        if (mJointsList.at(supportJointNumber - 1)->attachedBars()->isEmpty())
        {
            return;
        }

        Support *support;
        QString supportTypeName;
        Joint *joint = mJointsList.at(supportJointNumber - 1);

        switch (type)
        {
            case UnitsAndLimits::FIXED_LEFT:
            case UnitsAndLimits::FIXED_TOP:
            case UnitsAndLimits::FIXED_RIGHT:
            case UnitsAndLimits::FIXED_BOTTOM:
                supportTypeName = QString(tr("fixed"));
                break;
            case UnitsAndLimits::ROLLER:
                supportTypeName = QString(tr("inclined roller"));
                break;
            case UnitsAndLimits::ROLLER_LEFT:
            case UnitsAndLimits::ROLLER_TOP:
            case UnitsAndLimits::ROLLER_RIGHT:
            case UnitsAndLimits::ROLLER_BOTTOM:
                supportTypeName = QString(tr("roller"));
                break;
            default:
                break;
        }

        support = new Support(type,
                              angle,
                              joint,
                              this);

        joint->setSupported(true);

        mSupportsList.append(support);

        QList<QStandardItem *> standardItemsList;
        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(supportJointNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(supportTypeName);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(angle, 'f', mUnitsAndLimits.supportAngleDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);

        mSupportsStandardItemModel->appendRow(standardItemsList);
        mSupportsTableView->scrollToBottom();

        updateModelViewer();
        mModelCheckRequired = true;
        setWindowModified(true);
    }
}

void Solver::editSupport(int supportJointNumber,
                         UnitsAndLimits::SupportType type,
                         qreal angle)
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mSupportsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedSupports;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedSupports.append(index.row() + 1);
    }

    if (selectedSupports.size() == 1)
    {
        int supportNumber = selectedSupports.first();
        Support *support  = mSupportsList.at(supportNumber - 1);
        Joint *joint      = support->supportJoint();

        joint->setSupported(false);

        //Checks for bar with one end attached to a roller type support only
        bool checkA = mJointsList.at(supportJointNumber - 1)->attachedBars()->size() == 1;
        bool checkB = (type == UnitsAndLimits::ROLLER)
                || (type == UnitsAndLimits::ROLLER_LEFT)
                || (type == UnitsAndLimits::ROLLER_TOP)
                || (type == UnitsAndLimits::ROLLER_RIGHT)
                || (type == UnitsAndLimits::ROLLER_BOTTOM);

        if (checkA && checkB)
        {
            joint->setSupported(true);
            return;
        }

        if (mJointsList.at(supportJointNumber - 1)->attachedBars()->isEmpty())
        {
            joint->setSupported(true);
            return;
        }

        support->setSupportJoint(mJointsList.at(supportJointNumber - 1));
        support->setSupportType(type);
        support->setSupportAngle(angle);
        mJointsList.at(supportJointNumber - 1)->setSupported(true);

        QString supportTypeName;

        switch (type)
        {
            case UnitsAndLimits::FIXED_LEFT:
            case UnitsAndLimits::FIXED_TOP:
            case UnitsAndLimits::FIXED_RIGHT:
            case UnitsAndLimits::FIXED_BOTTOM:
                supportTypeName = QString(tr("fixed"));
                break;
            case UnitsAndLimits::ROLLER:
                supportTypeName = QString(tr("inclined roller"));
                break;
            case UnitsAndLimits::ROLLER_LEFT:
            case UnitsAndLimits::ROLLER_TOP:
            case UnitsAndLimits::ROLLER_RIGHT:
            case UnitsAndLimits::ROLLER_BOTTOM:
                supportTypeName = QString(tr("roller"));
                break;
            default:
                break;
        }

        mSupportsTableView->clearSelection();

        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(supportJointNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mSupportsStandardItemModel->setItem(supportNumber - 1, 0, standardItem);
        standardItem = new QStandardItem(supportTypeName);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mSupportsStandardItemModel->setItem(supportNumber - 1, 1, standardItem);
        standardItem = new QStandardItem(QString::number(angle, 'f', mUnitsAndLimits.supportAngleDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mSupportsStandardItemModel->setItem(supportNumber - 1, 2, standardItem);

        mModelCheckRequired = true;
        setWindowModified(true);
        updateModelViewer();
    }
    else
    {
        int count = 0;
        mSupportsTableView->clearSelection();

        foreach (int supportNumber, selectedSupports)
        {
            //Checks for bar with one end attached to a roller type support only
            bool checkA = mSupportsList.at(supportNumber - 1)->supportJoint()->attachedBars()->size() == 1;
            bool checkB = (type == UnitsAndLimits::ROLLER)
                    || (type == UnitsAndLimits::ROLLER_LEFT)
                    || (type == UnitsAndLimits::ROLLER_TOP)
                    || (type == UnitsAndLimits::ROLLER_RIGHT)
                    || (type == UnitsAndLimits::ROLLER_BOTTOM);

            if (checkA && checkB)
            {
                continue;
            }

            Support *support = mSupportsList.at(supportNumber - 1);
            support->setSupportType(type);
            support->setSupportAngle(angle);

            QString supportTypeName;

            switch (type)
            {
                case UnitsAndLimits::FIXED_LEFT:
                case UnitsAndLimits::FIXED_TOP:
                case UnitsAndLimits::FIXED_RIGHT:
                case UnitsAndLimits::FIXED_BOTTOM:
                    supportTypeName = QString(tr("fixed"));
                    break;
                case UnitsAndLimits::ROLLER:
                    supportTypeName = QString(tr("inclined roller"));
                    break;
                case UnitsAndLimits::ROLLER_LEFT:
                case UnitsAndLimits::ROLLER_TOP:
                case UnitsAndLimits::ROLLER_RIGHT:
                case UnitsAndLimits::ROLLER_BOTTOM:
                    supportTypeName = QString(tr("roller"));
                    break;
                default:
                    break;
            }

            QStandardItem *standardItem;
            standardItem = new QStandardItem(supportTypeName);
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mSupportsStandardItemModel->setItem(supportNumber - 1, 1, standardItem);
            standardItem = new QStandardItem(QString::number(angle, 'f' , mUnitsAndLimits.supportAngleDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mSupportsStandardItemModel->setItem(supportNumber - 1, 2, standardItem);

            ++count;
        }

        if (count)
        {
            mModelCheckRequired = true;
            setWindowModified(true);
            updateModelViewer();
        }
    }
}

void Solver::deleteSupport()
{
    if (hasLoads())
    {
        return;
    }

    QItemSelectionModel *itemSelectionModel = mSupportsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedSupports;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedSupports.append(index.row() + 1);
    }

    QList<Support *> supportsToRemove;
    QList<QStandardItem *> itemsToRemove;

    foreach (int supportNumber, selectedSupports)
    {
        supportsToRemove.append(mSupportsList.at(supportNumber - 1));
        itemsToRemove.append(mSupportsStandardItemModel->item(supportNumber - 1, 0));
    }

    foreach (int supportNumber, selectedSupports)
    {
        mSupportsList.at(supportNumber - 1)->supportJoint()->setSupported(false);
        delete mSupportsList.at(supportNumber - 1);
    }

    foreach (Support *support, supportsToRemove)
    {
        mSupportsList.removeOne(support);
    }

    updateModelViewer();
    mModelCheckRequired = true;
    setWindowModified(true);

    mSupportsTableView->clearSelection();

    foreach (QStandardItem *item, itemsToRemove)
    {
        int row = mSupportsStandardItemModel->indexFromItem(item).row();
        mSupportsStandardItemModel->removeRow(row);
    }
}

void Solver::onTabIndexChanged(int option)
{
    mLoadsTableView->clearSelection();

    if (mAddJointLoadsDialog->isVisible())
    {
        mAddJointLoadsDialog->hide();
    }

    if (mEditJointLoadsDialog->isVisible())
    {
        mEditJointLoadsDialog->hide();
    }

    if (mDeleteJointLoadsDialog->isVisible())
    {
        mDeleteJointLoadsDialog->hide();
    }

    if (mAddSupportSettlementsDialog->isVisible())
    {
        mAddSupportSettlementsDialog->hide();
    }

    if (mEditSupportSettlementsDialog->isVisible())
    {
        mEditSupportSettlementsDialog->hide();
    }

    if (mDeleteSupportSettlementsDialog->isVisible())
    {
        mDeleteSupportSettlementsDialog->hide();
    }

    if (mAddThermalEffectsDialog->isVisible())
    {
        mAddThermalEffectsDialog->hide();
    }

    if (mEditThermalEffectsDialog->isVisible())
    {
        mEditThermalEffectsDialog->hide();
    }

    if (mDeleteThermalEffectsDialog->isVisible())
    {
        mDeleteThermalEffectsDialog->hide();
    }

    if (mAddFabricationErrorsDialog->isVisible())
    {
        mAddFabricationErrorsDialog->hide();
    }

    if (mEditFabricationErrorsDialog->isVisible())
    {
        mEditFabricationErrorsDialog->hide();
    }

    if (mDeleteFabricationErrorsDialog->isVisible())
    {
        mDeleteFabricationErrorsDialog->hide();
    }

    if (mAddInfluenceLoadsDialog->isVisible())
    {
        mAddInfluenceLoadsDialog->hide();
    }

    if (mEditInfluenceLoadsDialog->isVisible())
    {
        mEditInfluenceLoadsDialog->hide();
    }

    if (mDeleteInfluenceLoadsDialog->isVisible())
    {
        mDeleteInfluenceLoadsDialog->hide();
    }

    if ((option == 3) || (option == 4))
    {
        if (mModelCheckRequired)
        {
            checkModel();
        }
        else
        {
            if (option == 3)
            {
                emit modelSolutionSignal(ModelViewer::NO_SOLUTION, 0);
                setLoadDisplayOption(mLoadTypeListWidget->currentRow());
            }
            else
            {
                setSolutionOptions();
                emit modelLoadSignal(ModelViewer::NO_LOAD);

                if (mHasSolution)
                {
                    switch (mSolutionOptionComboBox->currentIndex())
                    {
                        case -1:
                            emit modelSolutionSignal(ModelViewer::NO_SOLUTION, 0);
                            break;
                        case 0:
                            emit modelSolutionSignal(ModelViewer::JOINT_DEFLECTION, 0);
                            break;
                        case 1:
                            emit modelSolutionSignal(ModelViewer::BAR_LOAD, 0);
                            break;
                        case 2:
                            emit modelSolutionSignal(ModelViewer::SUPPORT_REACTION, 0);
                            break;
                        default:
                            break;
                    }

                    if (mInfluenceLoadBarComboBox->currentIndex() >= 0)
                    {
                        emit modelSolutionSignal(ModelViewer::INFLUENCE_LOAD_SOLUTION,
                                                 mInfluenceLoadBarComboBox->currentIndex());
                    }
                }
            }
        }
    }
    else
    {
        emit modelLoadSignal(ModelViewer::NO_LOAD);
        emit modelSolutionSignal(ModelViewer::NO_SOLUTION, 0);
    }
}

void Solver::onActionCheckModelTriggered()
{
    if (mModelCheckRequired)
    {
        checkModel();
    }
}

void Solver::checkModel()
{
    mAddLoadPushButton->setEnabled(false);
    mEditLoadPushButton->setEnabled(false);
    mDeleteLoadPushButton->setEnabled(false);

    mModelCheckRequired = false;
    setModelStability(false);

    ModelChecker *modelChecker = new ModelChecker(mJointsList,
                                                  mBarsList,
                                                  mSupportsList,
                                                  mUnitsAndLimits);

    qRegisterMetaType< QString >("QString");
    qRegisterMetaType< QList<Bar*> >("QList<Bar*>");
    qRegisterMetaType< QList<Support*> >("QList<Support*>");

    connect(modelChecker, SIGNAL(progressSignal(int)), mProgressBar, SLOT(setValue(int)));
    connect(modelChecker, SIGNAL(notesSignal(QString)), this, SLOT(setNote(QString)));
    connect(modelChecker, SIGNAL(redundantBarsListSignal(QList<Bar*>)),
            this, SLOT(setRedundantBarsList(QList<Bar*>)));
    connect(modelChecker, SIGNAL(redundantSupportsListSignal(QList<Support*>)),
            this, SLOT(setRedundantSupportsList(QList<Support*>)));
    connect(modelChecker, SIGNAL(modelStabilitySignal(bool)),
            this, SLOT(setModelStability(bool)));
    connect(modelChecker, SIGNAL(finished()), this, SLOT(enableLoadsEntry()));
    connect(modelChecker, SIGNAL(finished()), mProgressBar, SLOT(reset()));
    connect(modelChecker, SIGNAL(finished()), mProgressBar, SLOT(hide()));

    mProgressBar->setHidden(false);
    modelChecker->start();
}

void Solver::setNote(QString note)
{
    mNotesDialog->setNote(note);
    showNotes();
}

void Solver::setRedundantBarsList(QList<Bar *> barsList)
{
    mRedundantBarsList = barsList;
    mAddThermalEffectsDialog->updateBars();
    mEditThermalEffectsDialog->updateBars();
    mAddFabricationErrorsDialog->updateBars();
    mEditFabricationErrorsDialog->updateBars();
}

void Solver::setRedundantSupportsList(QList<Support *> supportsList)
{
    mRedundantSupportsList = supportsList;
    mAddSupportSettlementsDialog->updateSupports();
    mEditSupportSettlementsDialog->updateSupports();
}

void Solver::setModelStability(bool stability)
{
    bool previousState = mIsStable;
    mIsStable          = stability;

    if (previousState != mIsStable)
    {
        mUpdateSelfWeightOption = true;
    }
}

void Solver::enableLoadsEntry()
{
    setLoadsTableView(mLoadTypeListWidget->currentRow());

    int index = mSelectionTabWidget->currentIndex();

    if (index == 3)
    {
        setLoadDisplayOption(mLoadTypeListWidget->currentRow());
    }
    else
    {
        setSolutionOptions();
        emit modelLoadSignal(ModelViewer::NO_LOAD);
    }
}

void Solver::setLoadsTableView(int index)
{
    mLoadsTableView->clearSelection();

    if (mAddJointLoadsDialog->isVisible())
    {
        mAddJointLoadsDialog->hide();
    }

    if (mEditJointLoadsDialog->isVisible())
    {
        mEditJointLoadsDialog->hide();
    }

    if (mDeleteJointLoadsDialog->isVisible())
    {
        mDeleteJointLoadsDialog->hide();
    }

    if (mAddSupportSettlementsDialog->isVisible())
    {
        mAddSupportSettlementsDialog->hide();
    }

    if (mEditSupportSettlementsDialog->isVisible())
    {
        mEditSupportSettlementsDialog->hide();
    }

    if (mDeleteSupportSettlementsDialog->isVisible())
    {
        mDeleteSupportSettlementsDialog->hide();
    }

    if (mAddThermalEffectsDialog->isVisible())
    {
        mAddThermalEffectsDialog->hide();
    }

    if (mEditThermalEffectsDialog->isVisible())
    {
        mEditThermalEffectsDialog->hide();
    }

    if (mDeleteThermalEffectsDialog->isVisible())
    {
        mDeleteThermalEffectsDialog->hide();
    }

    if (mAddFabricationErrorsDialog->isVisible())
    {
        mAddFabricationErrorsDialog->hide();
    }

    if (mEditFabricationErrorsDialog->isVisible())
    {
        mEditFabricationErrorsDialog->hide();
    }

    if (mDeleteFabricationErrorsDialog->isVisible())
    {
        mDeleteFabricationErrorsDialog->hide();
    }

    if (mAddInfluenceLoadsDialog->isVisible())
    {
        mAddInfluenceLoadsDialog->hide();
    }

    if (mEditInfluenceLoadsDialog->isVisible())
    {
        mEditInfluenceLoadsDialog->hide();
    }

    if (mDeleteInfluenceLoadsDialog->isVisible())
    {
        mDeleteInfluenceLoadsDialog->hide();
    }

    if (index == 0)
    {
        mLoadsTableView->setModel(mJointLoadsStandardItemModel);

        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mEditJointLoadsDialog, SLOT(setSelectedJointLoads(QItemSelection,QItemSelection)));
        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mDeleteJointLoadsDialog, SLOT(setSelectedJointLoads(QItemSelection,QItemSelection)));

        if (mIsStable)
        {
            mAddLoadPushButton->setEnabled(true);
            mEditLoadPushButton->setEnabled(true);
            mDeleteLoadPushButton->setEnabled(true);
        }
    }
    else if (index == 1)
    {
        mLoadsTableView->setModel(mSupportSettlementsStandardItemModel);

        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mEditSupportSettlementsDialog, SLOT(setSelectedSupportSettlements(QItemSelection,QItemSelection)));
        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mDeleteSupportSettlementsDialog, SLOT(setSelectedSupportSettlements(QItemSelection,QItemSelection)));

        if (mIsStable)
        {
            if (mBarsStandardItemModel->columnCount() == 5)
            {
                mAddLoadPushButton->setEnabled(true);
                mEditLoadPushButton->setEnabled(true);
                mDeleteLoadPushButton->setEnabled(true);
            }
            else
            {
                mAddLoadPushButton->setEnabled(false);
                mEditLoadPushButton->setEnabled(false);
                mDeleteLoadPushButton->setEnabled(false);
            }
        }
    }
    else if (index == 2)
    {
        mLoadsTableView->setModel(mThermalEffectsStandardItemModel);

        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mEditThermalEffectsDialog, SLOT(setSelectedThermalEffects(QItemSelection,QItemSelection)));
        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mDeleteThermalEffectsDialog, SLOT(setSelectedThermalEffects(QItemSelection,QItemSelection)));

        if (mIsStable)
        {
            if (mBarsStandardItemModel->columnCount() == 5)
            {
                mAddLoadPushButton->setEnabled(true);
                mEditLoadPushButton->setEnabled(true);
                mDeleteLoadPushButton->setEnabled(true);
            }
            else
            {
                mAddLoadPushButton->setEnabled(false);
                mEditLoadPushButton->setEnabled(false);
                mDeleteLoadPushButton->setEnabled(false);
            }
        }
    }
    else if (index == 3)
    {
        mLoadsTableView->setModel(mFabricationErrorsStandardItemModel);

        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mEditFabricationErrorsDialog, SLOT(setSelectedFabricationErrors(QItemSelection,QItemSelection)));
        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mDeleteFabricationErrorsDialog, SLOT(setSelectedFabricationErrors(QItemSelection,QItemSelection)));

        if (mIsStable)
        {
            if (mBarsStandardItemModel->columnCount() == 5)
            {
                mAddLoadPushButton->setEnabled(true);
                mEditLoadPushButton->setEnabled(true);
                mDeleteLoadPushButton->setEnabled(true);
            }
            else
            {
                mAddLoadPushButton->setEnabled(false);
                mEditLoadPushButton->setEnabled(false);
                mDeleteLoadPushButton->setEnabled(false);
            }
        }
    }
    else
    {
        mLoadsTableView->setModel(mInfluenceLoadsStandardItemModel);

        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mEditInfluenceLoadsDialog, SLOT(setSelectedInfluenceLoads(QItemSelection,QItemSelection)));
        connect(mLoadsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                mDeleteInfluenceLoadsDialog, SLOT(setSelectedInfluenceLoads(QItemSelection,QItemSelection)));

        if (mIsStable)
        {
            mAddLoadPushButton->setEnabled(true);
            mEditLoadPushButton->setEnabled(true);
            mDeleteLoadPushButton->setEnabled(true);
        }
    }

    int minimumSectionSize = 100;
    mLoadsTableView->horizontalHeader()->setMinimumSectionSize(minimumSectionSize);
    mLoadsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mLoadsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mLoadsTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void Solver::setLoadDisplayOption(int index)
{
    bool checkA = (mBarsStandardItemModel->columnCount() == 5);
    bool checkB = (index == 0);
    bool checkC = mBarsList.isEmpty();

    if (mIsStable)
    {
        if (checkA)
        {
            if (checkB)
            {
                if (!checkC)
                {
                    if (mYesRadioButton->isChecked())
                    {
                        emit modelLoadSignal(ModelViewer::JOINT_LOAD_WITH_SELF_WEIGHT);
                    }
                    else
                    {
                        emit modelLoadSignal(ModelViewer::JOINT_LOAD);
                    }
                }
            }
            else
            {
                ModelViewer::LoadDisplayOption option = ModelViewer::NO_LOAD;

                switch (index)
                {
                    case 1:
                        option = ModelViewer::SUPPORT_SETTLEMENT;
                        break;
                    case 2:
                        option = ModelViewer::THERMAL_EFFECT;
                        break;
                    case 3:
                        option = ModelViewer::ASSEMBLY_FABRICATION_ERROR;
                        break;
                    case 4:
                        option = ModelViewer::INFLUENCE_LOAD;
                        break;
                    default:
                        break;
                }

                emit modelLoadSignal(option);
            }
        }
        else
        {
            ModelViewer::LoadDisplayOption option = ModelViewer::NO_LOAD;

            switch (index)
            {
                case 0:
                    option = ModelViewer::JOINT_LOAD;
                    break;
                case 1:
                    option = ModelViewer::SUPPORT_SETTLEMENT;
                    break;
                case 2:
                    option = ModelViewer::THERMAL_EFFECT;
                    break;
                case 3:
                    option = ModelViewer::ASSEMBLY_FABRICATION_ERROR;
                    break;
                case 4:
                    option = ModelViewer::INFLUENCE_LOAD;
                    break;
                default:
                    break;
            }

            emit modelLoadSignal(option);
        }
    }
}

void Solver::setLoadDisplayOption(bool checked)
{
    bool checkA = (mBarsStandardItemModel->columnCount() == 5);
    bool checkB = (mLoadTypeListWidget->currentRow() == 0);
    bool checkC = mBarsList.isEmpty();

    if (mIsStable)
    {
        if (checkA)
        {
            if (checkB)
            {
                if (!checkC)
                {
                    if (checked)
                    {
                        emit modelLoadSignal(ModelViewer::JOINT_LOAD_WITH_SELF_WEIGHT);
                    }
                    else
                    {
                        emit modelLoadSignal(ModelViewer::JOINT_LOAD);
                    }
                }
            }
            else
            {
                ModelViewer::LoadDisplayOption option = ModelViewer::NO_LOAD;

                switch (mLoadTypeListWidget->currentRow())
                {
                    case 1:
                        option = ModelViewer::SUPPORT_SETTLEMENT;
                        break;
                    case 2:
                        option = ModelViewer::THERMAL_EFFECT;
                        break;
                    case 3:
                        option = ModelViewer::ASSEMBLY_FABRICATION_ERROR;
                        break;
                    case 4:
                        option = ModelViewer::INFLUENCE_LOAD;
                        break;
                    default:
                        break;
                }

                emit modelLoadSignal(option);
            }
        }
        else
        {
            ModelViewer::LoadDisplayOption option = ModelViewer::NO_LOAD;

            switch (mLoadTypeListWidget->currentRow())
            {
                case 0:
                    option = ModelViewer::JOINT_LOAD;
                    break;
                case 1:
                    option = ModelViewer::SUPPORT_SETTLEMENT;
                    break;
                case 2:
                    option = ModelViewer::THERMAL_EFFECT;
                    break;
                case 3:
                    option = ModelViewer::ASSEMBLY_FABRICATION_ERROR;
                    break;
                case 4:
                    option = ModelViewer::INFLUENCE_LOAD;
                    break;
                default:
                    break;
            }

            emit modelLoadSignal(option);
        }
    }
}

void Solver::setLoadsSelectionMode(bool checked)
{
    if (checked)
    {
        mLoadsTableView->clearSelection();
        mLoadsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else
    {
        mLoadsTableView->clearSelection();
        mLoadsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}

void Solver::showAddLoadsDialog()
{
    int loadType = mLoadTypeListWidget->currentRow();

    if (loadType == 0)
    {
        if (mAddJointLoadsDialog->position() != QPoint(0, 0))
        {
            mAddJointLoadsDialog->move(mAddJointLoadsDialog->position());
        }

        mAddJointLoadsDialog->show();
        mAddJointLoadsDialog->raise();
        mAddJointLoadsDialog->activateWindow();
    }
    else if (loadType == 1)
    {
        if (mAddSupportSettlementsDialog->position() != QPoint(0, 0))
        {
            mAddSupportSettlementsDialog->move(mAddSupportSettlementsDialog->position());
        }

        mAddSupportSettlementsDialog->show();
        mAddSupportSettlementsDialog->raise();
        mAddSupportSettlementsDialog->activateWindow();
    }
    else if (loadType == 2)
    {
        if (mAddThermalEffectsDialog->position() != QPoint(0, 0))
        {
            mAddThermalEffectsDialog->move(mAddThermalEffectsDialog->position());
        }

        mAddThermalEffectsDialog->show();
        mAddThermalEffectsDialog->raise();
        mAddThermalEffectsDialog->activateWindow();
    }
    else if (loadType == 3)
    {
        if (mAddFabricationErrorsDialog->position() != QPoint(0, 0))
        {
            mAddFabricationErrorsDialog->move(mAddFabricationErrorsDialog->position());
        }

        mAddFabricationErrorsDialog->show();
        mAddFabricationErrorsDialog->raise();
        mAddFabricationErrorsDialog->activateWindow();
    }
    else
    {
        if (mAddInfluenceLoadsDialog->position() != QPoint(0, 0))
        {
            mAddInfluenceLoadsDialog->move(mAddInfluenceLoadsDialog->position());
        }

        mAddInfluenceLoadsDialog->show();
        mAddInfluenceLoadsDialog->raise();
        mAddInfluenceLoadsDialog->activateWindow();
    }
}

void Solver::showEditLoadsDialog()
{
    int loadType = mLoadTypeListWidget->currentRow();

    if (loadType == 0)
    {
        if (mEditJointLoadsDialog->position() != QPoint(0, 0))
        {
            mEditJointLoadsDialog->move(mEditJointLoadsDialog->position());
        }

        mEditJointLoadsDialog->show();
        mEditJointLoadsDialog->raise();
        mEditJointLoadsDialog->activateWindow();
    }
    else if (loadType == 1)
    {
        if (mEditSupportSettlementsDialog->position() != QPoint(0, 0))
        {
            mEditSupportSettlementsDialog->move(mEditSupportSettlementsDialog->position());
        }

        mEditSupportSettlementsDialog->show();
        mEditSupportSettlementsDialog->raise();
        mEditSupportSettlementsDialog->activateWindow();
    }
    else if (loadType == 2)
    {
        if (mEditThermalEffectsDialog->position() != QPoint(0, 0))
        {
            mEditThermalEffectsDialog->move(mEditThermalEffectsDialog->position());
        }

        mEditThermalEffectsDialog->show();
        mEditThermalEffectsDialog->raise();
        mEditThermalEffectsDialog->activateWindow();
    }
    else if (loadType == 3)
    {
        if (mEditFabricationErrorsDialog->position() != QPoint(0, 0))
        {
            mEditFabricationErrorsDialog->move(mEditFabricationErrorsDialog->position());
        }

        mEditFabricationErrorsDialog->show();
        mEditFabricationErrorsDialog->raise();
        mEditFabricationErrorsDialog->activateWindow();
    }
    else
    {
        if (mEditInfluenceLoadsDialog->position() != QPoint(0, 0))
        {
            mEditInfluenceLoadsDialog->move(mEditInfluenceLoadsDialog->position());
        }

        mEditInfluenceLoadsDialog->show();
        mEditInfluenceLoadsDialog->raise();
        mEditInfluenceLoadsDialog->activateWindow();
    }
}

void Solver::showDeleteLoadsDialog()
{
    int loadType = mLoadTypeListWidget->currentRow();

    if (loadType == 0)
    {
        if (mDeleteJointLoadsDialog->position() != QPoint(0, 0))
        {
            mDeleteJointLoadsDialog->move(mDeleteJointLoadsDialog->position());
        }

        mDeleteJointLoadsDialog->show();
        mDeleteJointLoadsDialog->raise();
        mDeleteJointLoadsDialog->activateWindow();
    }
    else if (loadType == 1)
    {
        if (mDeleteSupportSettlementsDialog->position() != QPoint(0, 0))
        {
            mDeleteSupportSettlementsDialog->move(mDeleteSupportSettlementsDialog->position());
        }

        mDeleteSupportSettlementsDialog->show();
        mDeleteSupportSettlementsDialog->raise();
        mDeleteSupportSettlementsDialog->activateWindow();
    }
    else if (loadType == 2)
    {
        if (mDeleteThermalEffectsDialog->position() != QPoint(0, 0))
        {
            mDeleteThermalEffectsDialog->move(mDeleteThermalEffectsDialog->position());
        }

        mDeleteThermalEffectsDialog->show();
        mDeleteThermalEffectsDialog->raise();
        mDeleteThermalEffectsDialog->activateWindow();
    }
    else if (loadType == 3)
    {
        if (mDeleteFabricationErrorsDialog->position() != QPoint(0, 0))
        {
            mDeleteFabricationErrorsDialog->move(mDeleteFabricationErrorsDialog->position());
        }

        mDeleteFabricationErrorsDialog->show();
        mDeleteFabricationErrorsDialog->raise();
        mDeleteFabricationErrorsDialog->activateWindow();
    }
    else
    {
        if (mDeleteInfluenceLoadsDialog->position() != QPoint(0, 0))
        {
            mDeleteInfluenceLoadsDialog->move(mDeleteInfluenceLoadsDialog->position());
        }

        mDeleteInfluenceLoadsDialog->show();
        mDeleteInfluenceLoadsDialog->raise();
        mDeleteInfluenceLoadsDialog->activateWindow();
    }
}

void Solver::addJointLoad(int loadJointNumber,
                          qreal horizontalComponent,
                          qreal verticalComponent,
                          QString position)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();

    if (itemSelectionModel->hasSelection())
    {
        QList<QModelIndex> modelIndexList = itemSelectionModel->selectedRows();

        QList<int> selectedJoints;

        foreach (QModelIndex index, modelIndexList)
        {
            selectedJoints.append(index.row() + 1);
        }

        int count = 0;

        foreach (int jointNumber, selectedJoints)
        {
            Joint *joint = mJointsList.at(jointNumber - 1);

            bool check = false;

            foreach (JointLoad *load, mJointLoadsList)
            {
                if (load->loadJoint() == joint)
                {
                    check = true;
                    break;
                }
            }

            if (check)
            {
                continue;
            }

            JointLoad *load = new JointLoad(joint,
                                            horizontalComponent,
                                            verticalComponent,
                                            position,
                                            this);

            if (mJointLoadsList.isEmpty())
            {
                mUpdateJointLoadOption = true;
            }

            mJointLoadsList.append(load);

            ++count;

            QList<QStandardItem *> standardItemsList;
            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(jointNumber));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            standardItem = new QStandardItem(QString::number(horizontalComponent, 'f',
                                                             mUnitsAndLimits.jointLoadDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            standardItem = new QStandardItem(QString::number(verticalComponent, 'f',
                                                             mUnitsAndLimits.jointLoadDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            mJointLoadsStandardItemModel->appendRow(standardItemsList);
        }

        if (count)
        {
            mLoadsTableView->scrollToBottom();
            mJointsTableView->clearSelection();
            updateModelViewer();
            setWindowModified(true);
        }
    }
    else
    {
        Joint *joint = mJointsList.at(loadJointNumber - 1);

        foreach (JointLoad *load, mJointLoadsList)
        {
            if (load->loadJoint() == joint)
            {
                return;
            }
        }

        JointLoad *load = new JointLoad(joint,
                                        horizontalComponent,
                                        verticalComponent,
                                        position,
                                        this);

        if (mJointLoadsList.isEmpty())
        {
            mUpdateJointLoadOption = true;
        }

        mJointLoadsList.append(load);

        QList<QStandardItem *> standardItemsList;
        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(loadJointNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(horizontalComponent, 'f',
                                                         mUnitsAndLimits.jointLoadDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(verticalComponent, 'f', mUnitsAndLimits.jointLoadDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        mJointLoadsStandardItemModel->appendRow(standardItemsList);

        mLoadsTableView->scrollToBottom();
        mJointsTableView->clearSelection();
        updateModelViewer();
        setWindowModified(true);
    }
}

void Solver::editJointLoad(int loadJointNumber,
                           qreal horizontalComponent,
                           qreal verticalComponent,
                           QString position)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedLoads.append(index.row() + 1);
    }

    if (selectedLoads.size() == 1)
    {
        JointLoad *load = mJointLoadsList.at(selectedLoads.first() - 1);
        Joint *joint    = mJointsList.at(loadJointNumber - 1);

        bool check = false;

        foreach (JointLoad *jointLoad, mJointLoadsList)
        {
            if (jointLoad == load)
            {
                continue;
            }

            if (jointLoad->loadJoint() == joint)
            {
                check = true;
                break;
            }
        }

        if (check)
        {
            mLoadsTableView->clearSelection();
            return;
        }

        load->setLoadJoint(joint);
        load->setHorizontalComponent(horizontalComponent);
        load->setVerticalComponent(verticalComponent);
        load->setPosition(position);

        int loadIndex = selectedLoads.first() - 1;
        mLoadsTableView->clearSelection();

        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(loadJointNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mJointLoadsStandardItemModel->setItem(loadIndex, 0, standardItem);
        standardItem = new QStandardItem(QString::number(horizontalComponent, 'f',
                                                         mUnitsAndLimits.jointLoadDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mJointLoadsStandardItemModel->setItem(loadIndex, 1, standardItem);
        standardItem = new QStandardItem(QString::number(verticalComponent, 'f', mUnitsAndLimits.jointLoadDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mJointLoadsStandardItemModel->setItem(loadIndex, 2, standardItem);

        updateModelViewer();
        setWindowModified(true);
    }
    else
    {
        mLoadsTableView->clearSelection();

        foreach (int loadNumber, selectedLoads)
        {
            JointLoad *load = mJointLoadsList.at(loadNumber - 1);

            load->setHorizontalComponent(horizontalComponent);
            load->setVerticalComponent(verticalComponent);
            load->setPosition(position);

            int loadIndex = loadNumber - 1;

            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(horizontalComponent, 'f',
                                                             mUnitsAndLimits.jointLoadDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mJointLoadsStandardItemModel->setItem(loadIndex, 1, standardItem);
            standardItem = new QStandardItem(QString::number(verticalComponent, 'f',
                                                             mUnitsAndLimits.jointLoadDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mJointLoadsStandardItemModel->setItem(loadIndex, 2, standardItem);
        }

        updateModelViewer();
        setWindowModified(true);
    }
}

void Solver::deleteJointLoad()
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedLoads.append(index.row() + 1);
    }

    QList<JointLoad *> loadsToRemove;
    QList<QStandardItem *> itemsToRemove;

    foreach (int loadNumber, selectedLoads)
    {
        loadsToRemove.append(mJointLoadsList.at(loadNumber - 1));
        itemsToRemove.append(mJointLoadsStandardItemModel->item(loadNumber - 1, 0));
    }

    foreach (int loadNumber, selectedLoads)
    {
        delete mJointLoadsList.at(loadNumber - 1);
    }

    foreach (JointLoad *load, loadsToRemove)
    {
        mJointLoadsList.removeOne(load);
    }

    mLoadsTableView->clearSelection();

    foreach (QStandardItem *item, itemsToRemove)
    {
        int row = mJointLoadsStandardItemModel->indexFromItem(item).row();
        mJointLoadsStandardItemModel->removeRow(row);
    }

    if (mJointLoadsList.isEmpty())
    {
        mUpdateJointLoadOption = true;
    }

    updateModelViewer();
    setWindowModified(true);
}

void Solver::addSupportSettlement(int supportNumber, qreal settlement)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    bool check       = false;
    Support *support = mSupportsList.at(supportNumber - 1);

    foreach (SupportSettlement *supportSettlement, mSupportSettlementsList)
    {
        if (supportSettlement->settlementSupport() == support)
        {
            check = true;
            break;
        }
    }

    if (check)
    {
        return;
    }

    SupportSettlement *supportSettlement = new SupportSettlement(support,
                                                                 settlement,
                                                                 this);

    if (mSupportSettlementsList.isEmpty())
    {
        mUpdateSupportSettlementOption = true;
    }

    mSupportSettlementsList.append(supportSettlement);

    Joint *joint    = support->supportJoint();
    int jointNumber = mJointsList.indexOf(joint) + 1;

    QList<QStandardItem *> standardItemsList;
    QStandardItem *standardItem;
    standardItem = new QStandardItem(QString::number(supportNumber) + " @ joint " + QString::number(jointNumber));
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemsList.append(standardItem);
    standardItem = new QStandardItem(QString::number(settlement, 'f', mUnitsAndLimits.supportSettlementDecimals()));
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemsList.append(standardItem);
    mSupportSettlementsStandardItemModel->appendRow(standardItemsList);

    mLoadsTableView->scrollToBottom();
    updateModelViewer();
    setWindowModified(true);
}

void Solver::editSupportSettlement(int supportNumber, qreal settlement)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedSupportSettlements;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedSupportSettlements.append(index.row() + 1);
    }

    if (selectedSupportSettlements.size() == 1)
    {
        SupportSettlement *currentSupportSettlement = mSupportSettlementsList
                .at(selectedSupportSettlements.first() - 1);
        Support *settlementSupport                  = mSupportsList.at(supportNumber - 1);

        bool check = false;

        foreach (SupportSettlement *supportSettlement, mSupportSettlementsList)
        {
            if (supportSettlement == currentSupportSettlement)
            {
                continue;
            }

            if (supportSettlement->settlementSupport() == settlementSupport)
            {
                check = true;
                break;
            }
        }

        if (check)
        {
            return;
        }

        currentSupportSettlement->setSettlementSupport(settlementSupport);
        currentSupportSettlement->setSettlement(settlement);
        Joint *joint    = settlementSupport->supportJoint();
        int jointNumber = mJointsList.indexOf(joint) + 1;

        int supportSettlementIndex = selectedSupportSettlements.first() - 1;

        mLoadsTableView->clearSelection();

        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(supportNumber) + " @ joint " + QString::number(jointNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mSupportSettlementsStandardItemModel->setItem(supportSettlementIndex, 0, standardItem);
        standardItem = new QStandardItem(QString::number(settlement, 'f', mUnitsAndLimits.supportSettlementDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mSupportSettlementsStandardItemModel->setItem(supportSettlementIndex, 1, standardItem);

        updateModelViewer();
        setWindowModified(true);
    }
    else
    {
        mLoadsTableView->clearSelection();

        foreach (int supportSettlementNumber, selectedSupportSettlements)
        {
            SupportSettlement *currentSupportSettlement = mSupportSettlementsList.at(supportSettlementNumber - 1);
            currentSupportSettlement->setSettlement(settlement);
            int supportSettlementIndex = supportSettlementNumber - 1;

            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(settlement, 'f',
                                                             mUnitsAndLimits.supportSettlementDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mSupportSettlementsStandardItemModel->setItem(supportSettlementIndex, 1, standardItem);
        }

        updateModelViewer();
        setWindowModified(true);
    }
}

void Solver::deleteSupportSettlement()
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedSupportSettlements;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedSupportSettlements.append(index.row() + 1);
    }

    QList<SupportSettlement *> supportSettlementsToRemove;
    QList<QStandardItem *> itemsToRemove;

    foreach (int supportSettlementNumber, selectedSupportSettlements)
    {
        supportSettlementsToRemove.append(mSupportSettlementsList.at(supportSettlementNumber - 1));
        itemsToRemove.append(mSupportSettlementsStandardItemModel->item(supportSettlementNumber - 1, 0));
    }

    foreach (int supportSettlementNumber, selectedSupportSettlements)
    {
        delete mSupportSettlementsList.at(supportSettlementNumber - 1);
    }

    foreach (SupportSettlement *supportSettlement, supportSettlementsToRemove)
    {
        mSupportSettlementsList.removeOne(supportSettlement);
    }

    mLoadsTableView->clearSelection();

    foreach (QStandardItem *item, itemsToRemove)
    {
        int row = mSupportSettlementsStandardItemModel->indexFromItem(item).row();
        mSupportSettlementsStandardItemModel->removeRow(row);
    }

    if (mSupportSettlementsList.isEmpty())
    {
        mUpdateSupportSettlementOption = true;
    }

    updateModelViewer();
    setWindowModified(true);
}

void Solver::addThermalEffect(int thermalEffectBarNumber,
                              qreal temperatureChange,
                              qreal thermalCoefficient)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();

    if (itemSelectionModel->hasSelection())
    {
        QList<QModelIndex> modelIndexList = itemSelectionModel->selectedRows();
        QList<int> selectedBars;

        foreach (QModelIndex index, modelIndexList)
        {
            selectedBars.append(index.row() + 1);
        }

        int count = 0;

        foreach (int barNumber, selectedBars)
        {
            Bar *bar = mBarsList.at(barNumber - 1);

            if (!mRedundantBarsList.contains(bar))
            {
                continue;
            }

            bool check = false;

            foreach (ThermalEffect *thermalEffect, mThermalEffectsList)
            {
                if (thermalEffect->thermalEffectBar() == bar)
                {
                    check = true;
                    break;
                }
            }

            if (check)
            {
                continue;
            }

            ThermalEffect *thermalEffect = new ThermalEffect(bar,
                                                             temperatureChange,
                                                             thermalCoefficient,
                                                             this);

            if (mThermalEffectsList.isEmpty())
            {
                mUpdateThermalEffectOption = true;
            }

            mThermalEffectsList.append(thermalEffect);

            ++count;

            QList<QStandardItem *> standardItemsList;
            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(barNumber));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            standardItem = new QStandardItem(QString::number(temperatureChange, 'f',
                                                             mUnitsAndLimits.temperatureChangeDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            standardItem = new QStandardItem(QString::number(thermalCoefficient, 'g',
                                                             mUnitsAndLimits.thermalCoefficientDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            mThermalEffectsStandardItemModel->appendRow(standardItemsList);
        }

        if (count)
        {
            mLoadsTableView->scrollToBottom();
            mBarsTableView->clearSelection();
            updateModelViewer();
            setWindowModified(true);
        }
    }
    else
    {
        Bar *bar = mBarsList.at(thermalEffectBarNumber - 1);

        bool check = false;

        foreach (ThermalEffect *thermalEffect, mThermalEffectsList)
        {
            if (thermalEffect->thermalEffectBar() == bar)
            {
                check = true;
                break;
            }
        }

        if (check)
        {
            return;
        }

        ThermalEffect *thermalEffect = new ThermalEffect(bar,
                                                         temperatureChange,
                                                         thermalCoefficient,
                                                         this);

        if (mThermalEffectsList.isEmpty())
        {
            mUpdateThermalEffectOption = true;
        }

        mThermalEffectsList.append(thermalEffect);

        QList<QStandardItem *> standardItemsList;
        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(thermalEffectBarNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(temperatureChange, 'f',
                                                         mUnitsAndLimits.temperatureChangeDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(thermalCoefficient, 'g',
                                                         mUnitsAndLimits.thermalCoefficientDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        mThermalEffectsStandardItemModel->appendRow(standardItemsList);

        mLoadsTableView->scrollToBottom();
        updateModelViewer();
        setWindowModified(true);
    }
}

void Solver::editThermalEffect(int thermalEffectBarNumber,
                               qreal temperatureChange,
                               qreal thermalCoefficient)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedThermalEffects;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedThermalEffects.append(index.row() + 1);
    }

    if (selectedThermalEffects.size() == 1)
    {
        int thermalEffectIndex              = selectedThermalEffects.first() - 1;
        ThermalEffect *currentThermalEffect = mThermalEffectsList.at(thermalEffectIndex);

        Bar *thermalEffectBar = mBarsList.at(thermalEffectBarNumber - 1);

        bool check = false;

        foreach (ThermalEffect *thermalEffect, mThermalEffectsList)
        {
            if (thermalEffect == currentThermalEffect)
            {
                continue;
            }

            if (thermalEffect->thermalEffectBar() == thermalEffectBar)
            {
                check = true;
                break;
            }
        }

        if (check)
        {
            return;
        }

        currentThermalEffect->setThermalEffectBar(thermalEffectBar);
        currentThermalEffect->setTemperatureChange(temperatureChange);
        currentThermalEffect->setThermalCoefficient(thermalCoefficient);

        mLoadsTableView->clearSelection();

        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(thermalEffectBarNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mThermalEffectsStandardItemModel->setItem(thermalEffectIndex, 0, standardItem);
        standardItem = new QStandardItem(QString::number(temperatureChange, 'f',
                                                         mUnitsAndLimits.temperatureChangeDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mThermalEffectsStandardItemModel->setItem(thermalEffectIndex, 1, standardItem);
        standardItem = new QStandardItem(QString::number(thermalCoefficient, 'g',
                                                         mUnitsAndLimits.thermalCoefficientDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mThermalEffectsStandardItemModel->setItem(thermalEffectIndex, 2, standardItem);

        updateModelViewer();
        setWindowModified(true);
    }
    else
    {
        mLoadsTableView->clearSelection();

        foreach (int thermalEffectNumber, selectedThermalEffects)
        {
            ThermalEffect *currentThermalEffect = mThermalEffectsList.at(thermalEffectNumber - 1);

            currentThermalEffect->setTemperatureChange(temperatureChange);
            currentThermalEffect->setThermalCoefficient(thermalCoefficient);

            int thermalEffectIndex = thermalEffectNumber - 1;

            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(temperatureChange, 'f',
                                                             mUnitsAndLimits.temperatureChangeDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mThermalEffectsStandardItemModel->setItem(thermalEffectIndex, 1, standardItem);
            standardItem = new QStandardItem(QString::number(thermalCoefficient, 'g',
                                                             mUnitsAndLimits.thermalCoefficientDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mThermalEffectsStandardItemModel->setItem(thermalEffectIndex, 2, standardItem);
        }

        updateModelViewer();
        setWindowModified(true);
    }
}

void Solver::deleteThermalEffect()
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedThermalEffects;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedThermalEffects.append(index.row() + 1);
    }

    QList<ThermalEffect *> thermalEffectsToRemove;
    QList<QStandardItem *> itemsToRemove;

    foreach (int thermalEffectNumber, selectedThermalEffects)
    {
        thermalEffectsToRemove.append(mThermalEffectsList.at(thermalEffectNumber - 1));
        itemsToRemove.append(mThermalEffectsStandardItemModel->item(thermalEffectNumber - 1, 0));
    }

    foreach (int thermalEffectNumber, selectedThermalEffects)
    {
        delete mThermalEffectsList.at(thermalEffectNumber - 1);
    }

    foreach (ThermalEffect *thermalEffect, thermalEffectsToRemove)
    {
        mThermalEffectsList.removeOne(thermalEffect);
    }

    mLoadsTableView->clearSelection();

    foreach (QStandardItem *item, itemsToRemove)
    {
        int row = mThermalEffectsStandardItemModel->indexFromItem(item).row();
        mThermalEffectsStandardItemModel->removeRow(row);
    }

    if (mThermalEffectsList.isEmpty())
    {
        mUpdateThermalEffectOption = true;
    }

    updateModelViewer();
    setWindowModified(true);
}

void Solver::addFabricationError(int fabricationErrorBarNumber, qreal lengthError)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();

    if (itemSelectionModel->hasSelection())
    {
        QList<QModelIndex> modelIndexList = itemSelectionModel->selectedRows();
        QList<int> selectedBars;

        foreach (QModelIndex index, modelIndexList)
        {
            selectedBars.append(index.row() + 1);
        }

        int count = 0;

        foreach (int barNumber, selectedBars)
        {
            Bar *bar = mBarsList.at(barNumber - 1);

            if (!mRedundantBarsList.contains(bar))
            {
                continue;
            }

            bool check = false;

            foreach (FabricationError *fabricationError, mFabricationErrorsList)
            {
                if (fabricationError->fabricationErrorBar() == bar)
                {
                    check = true;
                    break;
                }
            }

            if (check)
            {
                continue;
            }

            FabricationError *fabricationError = new FabricationError(bar,
                                                                      lengthError,
                                                                      this);

            if (mFabricationErrorsList.isEmpty())
            {
                mUpdateFabricationErrorOption = true;
            }

            mFabricationErrorsList.append(fabricationError);

            ++count;

            QList<QStandardItem *> standardItemsList;
            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(barNumber));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            standardItem = new QStandardItem(QString::number(lengthError, 'f', mUnitsAndLimits.lengthErrorDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            standardItemsList.append(standardItem);
            mFabricationErrorsStandardItemModel->appendRow(standardItemsList);
        }

        if (count)
        {
            mLoadsTableView->scrollToBottom();
            mBarsTableView->clearSelection();
            updateModelViewer();
            setWindowModified(true);
        }
    }
    else
    {
        Bar *bar = mBarsList.at(fabricationErrorBarNumber - 1);

        bool check = false;

        foreach (FabricationError *fabricationError, mFabricationErrorsList)
        {
            if (fabricationError->fabricationErrorBar() == bar)
            {
                check = true;
                break;
            }
        }

        if (check)
        {
            return;
        }

        FabricationError *fabricationError = new FabricationError(bar,
                                                                  lengthError,
                                                                  this);

        if (mFabricationErrorsList.isEmpty())
        {
            mUpdateFabricationErrorOption = true;
        }

        mFabricationErrorsList.append(fabricationError);

        QList<QStandardItem *> standardItemsList;
        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(fabricationErrorBarNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(lengthError, 'f', mUnitsAndLimits.lengthErrorDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        standardItemsList.append(standardItem);
        mFabricationErrorsStandardItemModel->appendRow(standardItemsList);

        mLoadsTableView->scrollToBottom();
        updateModelViewer();
        setWindowModified(true);
    }
}

void Solver::editFabricationError(int fabricationErrorBarNumber, qreal lengthError)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedFabricationErrors;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedFabricationErrors.append(index.row() + 1);
    }

    if (selectedFabricationErrors.size() == 1)
    {
        int fabricationErrorIndex                 = selectedFabricationErrors.first() - 1;
        FabricationError *currentFabricationError = mFabricationErrorsList.at(fabricationErrorIndex);
        Bar *fabricationErrorBar                  = mBarsList.at(fabricationErrorBarNumber - 1);

        bool check = false;

        foreach (FabricationError *fabricationError, mFabricationErrorsList)
        {
            if (fabricationError == currentFabricationError)
            {
                continue;
            }

            if (fabricationError->fabricationErrorBar() == fabricationErrorBar)
            {
                check = true;
                break;
            }
        }

        if (check)
        {
            return;
        }

        currentFabricationError->setFabricationErrorBar(fabricationErrorBar);
        currentFabricationError->setLengthError(lengthError);

        mLoadsTableView->clearSelection();

        QStandardItem *standardItem;
        standardItem = new QStandardItem(QString::number(fabricationErrorBarNumber));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mFabricationErrorsStandardItemModel->setItem(fabricationErrorIndex, 0, standardItem);
        standardItem = new QStandardItem(QString::number(lengthError, 'f', mUnitsAndLimits.lengthErrorDecimals()));
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mFabricationErrorsStandardItemModel->setItem(fabricationErrorIndex, 1, standardItem);

        updateModelViewer();
        setWindowModified(true);
    }
    else
    {
        mLoadsTableView->clearSelection();

        foreach (int fabricationErrorNumber, selectedFabricationErrors)
        {
            FabricationError *currentFabricationError = mFabricationErrorsList.at(fabricationErrorNumber - 1);
            currentFabricationError->setLengthError(lengthError);
            int fabricationErrorIndex = fabricationErrorNumber - 1;

            QStandardItem *standardItem;
            standardItem = new QStandardItem(QString::number(lengthError, 'f', mUnitsAndLimits.lengthErrorDecimals()));
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mFabricationErrorsStandardItemModel->setItem(fabricationErrorIndex, 1, standardItem);
        }

        updateModelViewer();
        setWindowModified(true);
    }
}

void Solver::deleteFabricationError()
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedFabricationErrors;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedFabricationErrors.append(index.row() + 1);
    }

    QList<FabricationError *> fabricationErrorsToRemove;
    QList<QStandardItem *> itemsToRemove;

    foreach (int fabricationErrorNumber, selectedFabricationErrors)
    {
        fabricationErrorsToRemove.append(mFabricationErrorsList.at(fabricationErrorNumber - 1));
        itemsToRemove.append(mFabricationErrorsStandardItemModel->item(fabricationErrorNumber - 1, 0));
    }

    foreach (int fabricationErrorNumber, selectedFabricationErrors)
    {
        delete mFabricationErrorsList.at(fabricationErrorNumber - 1);
    }

    foreach (FabricationError *fabricationError, fabricationErrorsToRemove)
    {
        mFabricationErrorsList.removeOne(fabricationError);
    }

    mLoadsTableView->clearSelection();

    foreach (QStandardItem *item, itemsToRemove)
    {
        int row = mFabricationErrorsStandardItemModel->indexFromItem(item).row();
        mFabricationErrorsStandardItemModel->removeRow(row);
    }

    if (mFabricationErrorsList.isEmpty())
    {
        mUpdateFabricationErrorOption = true;
    }

    updateModelViewer();
    setWindowModified(true);
}

void Solver::addInfluenceLoad(QString name,
                              QString direction,
                              QList<qreal> pointLoads,
                              QList<qreal> pointLoadPositions)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedJoints;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJoints.append(index.row() + 1);
    }

    foreach (InfluenceLoad *load, mInfluenceLoadsList)
    {
        if (load->path() == selectedJoints)
        {
            return;
        }
    }

    InfluenceLoad *influenceLoad = new InfluenceLoad(name,
                                                     direction,
                                                     selectedJoints,
                                                     pointLoads,
                                                     pointLoadPositions,
                                                     this);

    mUpdateInfluenceLoadOption = true;

    mInfluenceLoadsList.append(influenceLoad);

    QString pathString;
    int count = selectedJoints.size();

    for (int i = 0; i < count; ++i)
    {
        pathString.append(QString::number(selectedJoints.at(i)));

        if (i < (count - 1))
        {
            pathString.append("\n");
        }
    }

    QString loadsString;
    count = pointLoads.size();

    for (int i = 0; i < count; ++i)
    {
        loadsString.append(tr("%1 %2 @ %3 %4")
                           .arg(QString::number(pointLoads.at(i), 'f', mUnitsAndLimits.pointLoadDecimals()))
                           .arg(mUnitsAndLimits.loadUnit())
                           .arg(QString::number(pointLoadPositions.at(i), 'f', mUnitsAndLimits.offsetDecimals()))
                           .arg(mUnitsAndLimits.coordinateUnit()));

        if (i < (count - 1))
        {
            loadsString.append("\n");
        }
    }

    QList<QStandardItem *> standardItemList;
    QStandardItem *standardItem;
    standardItem = new QStandardItem(name);
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemList.append(standardItem);
    standardItem = new QStandardItem(direction);
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemList.append(standardItem);
    standardItem = new QStandardItem(pathString);
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemList.append(standardItem);
    standardItem = new QStandardItem(loadsString);
    standardItem->setTextAlignment(Qt::AlignCenter);
    standardItem->setEditable(false);
    standardItemList.append(standardItem);
    mInfluenceLoadsStandardItemModel->appendRow(standardItemList);

    mAddInfluenceLoadsDialog->updateDefaultName();
    mLoadsTableView->scrollToBottom();
    mJointsTableView->clearSelection();
    updateModelViewer();
    setWindowModified(true);
}

void Solver::editInfluenceLoad(QString name,
                               QString direction,
                               QList<qreal> pointLoads,
                               QList<qreal> pointLoadPositions)
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedInfluenceLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedInfluenceLoads.append(index.row() + 1);
    }

    if (selectedInfluenceLoads.size() == 1)
    {
        itemSelectionModel = mJointsTableView->selectionModel();

        QList<int> selectedJoints;

        int index                    = selectedInfluenceLoads.first() - 1;
        InfluenceLoad *influenceLoad = mInfluenceLoadsList.at(index);

        if (itemSelectionModel->hasSelection())
        {
            modelIndexList = itemSelectionModel->selectedRows();

            foreach (QModelIndex index, modelIndexList)
            {
                selectedJoints.append(index.row() + 1);
            }
        }
        else
        {
            foreach (int jointNumber, influenceLoad->path())
            {
                selectedJoints.append(jointNumber);
            }
        }

        foreach (InfluenceLoad *load, mInfluenceLoadsList)
        {
            if (load == influenceLoad)
            {
                continue;
            }

            if (load->name() == name)
            {
                return;
            }

            if (load->path() == selectedJoints)
            {
                return;
            }
        }

        influenceLoad->setName(name);
        influenceLoad->setDirection(direction);
        influenceLoad->setPointLoads(pointLoads);
        influenceLoad->setPointLoadPositions(pointLoadPositions);
        influenceLoad->setPath(selectedJoints);

        QString pathString;
        int count = selectedJoints.size();

        for (int i = 0; i < count; ++i)
        {
            pathString.append(QString::number(selectedJoints.at(i)));

            if (i < (count - 1))
            {
                pathString.append("\n");
            }
        }

        QString loadsString;

        count = pointLoads.size();

        for (int i = 0; i < count; ++i)
        {
            loadsString.append(tr("%1 %2 @ %3 %4")
                               .arg(QString::number(pointLoads.at(i), 'f', mUnitsAndLimits.pointLoadDecimals()))
                               .arg(mUnitsAndLimits.loadUnit())
                               .arg(QString::number(pointLoadPositions.at(i), 'f', mUnitsAndLimits.offsetDecimals()))
                               .arg(mUnitsAndLimits.coordinateUnit()));

            if (i < (count - 1))
            {
                loadsString.append("\n");
            }
        }

        mLoadsTableView->clearSelection();
        mJointsTableView->clearSelection();

        QStandardItem *standardItem;
        standardItem = new QStandardItem(name);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mInfluenceLoadsStandardItemModel->setItem(index, 0, standardItem);
        standardItem = new QStandardItem(direction);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mInfluenceLoadsStandardItemModel->setItem(index, 1, standardItem);
        standardItem = new QStandardItem(pathString);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mInfluenceLoadsStandardItemModel->setItem(index, 2, standardItem);
        standardItem = new QStandardItem(loadsString);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItem->setEditable(false);
        mInfluenceLoadsStandardItemModel->setItem(index, 3, standardItem);

        updateModelViewer();
        setWindowModified(true);
        mUpdateInfluenceLoadOption = true;
    }
    else
    {
        mLoadsTableView->clearSelection();

        foreach (int influenceLoadNumber, selectedInfluenceLoads)
        {
            InfluenceLoad *currentInfluenceLoad = mInfluenceLoadsList.at(influenceLoadNumber - 1);
            currentInfluenceLoad->setDirection(direction);
            currentInfluenceLoad->setPointLoads(pointLoads);
            currentInfluenceLoad->setPointLoadPositions(pointLoadPositions);

            QString loadsString;

            int count = pointLoads.size();

            for (int i = 0; i < count; ++i)
            {
                loadsString.append(tr("%1 %2 @ %3 %4")
                                   .arg(QString::number(pointLoads.at(i), 'f', mUnitsAndLimits.pointLoadDecimals()))
                                   .arg(mUnitsAndLimits.loadUnit())
                                   .arg(QString::number(pointLoadPositions.at(i), 'f',
                                                        mUnitsAndLimits.offsetDecimals()))
                                   .arg(mUnitsAndLimits.coordinateUnit()));

                if (i < (count - 1))
                {
                    loadsString.append("\n");
                }
            }

            int index = influenceLoadNumber - 1;
            QStandardItem *standardItem;
            standardItem = new QStandardItem(direction);
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mInfluenceLoadsStandardItemModel->setItem(index, 1, standardItem);
            standardItem = new QStandardItem(loadsString);
            standardItem->setTextAlignment(Qt::AlignCenter);
            standardItem->setEditable(false);
            mInfluenceLoadsStandardItemModel->setItem(index, 3, standardItem);
        }

        updateModelViewer();
        setWindowModified(true);
        mUpdateInfluenceLoadOption = true;
    }
}

void Solver::deleteInfluenceLoad()
{
    if (mHasSolution)
    {
        QApplication::beep();
        return;
    }

    QItemSelectionModel *itemSelectionModel = mLoadsTableView->selectionModel();
    QList<QModelIndex> modelIndexList       = itemSelectionModel->selectedRows();
    QList<int> selectedInfluenceLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedInfluenceLoads.append(index.row() + 1);
    }

    QList<InfluenceLoad *> influenceLoadsToRemove;
    QList<QStandardItem *> itemsToRemove;

    foreach (int influenceLoadNumber, selectedInfluenceLoads)
    {
        influenceLoadsToRemove.append(mInfluenceLoadsList.at(influenceLoadNumber - 1));
        itemsToRemove.append(mInfluenceLoadsStandardItemModel->item(influenceLoadNumber - 1, 0));
    }

    foreach (int influenceLoadNumber, selectedInfluenceLoads)
    {
        delete mInfluenceLoadsList.at(influenceLoadNumber - 1);
    }

    foreach (InfluenceLoad *influenceLoad, influenceLoadsToRemove)
    {
        mInfluenceLoadsList.removeOne(influenceLoad);
    }

    mLoadsTableView->clearSelection();

    foreach (QStandardItem *item, itemsToRemove)
    {
        int row = mInfluenceLoadsStandardItemModel->indexFromItem(item).row();
        mInfluenceLoadsStandardItemModel->removeRow(row);
    }

    mAddInfluenceLoadsDialog->updateDefaultName();
    updateModelViewer();
    setWindowModified(true);
    mUpdateInfluenceLoadOption = true;
}

void Solver::setSolutionOptions()
{
    if (mUpdateJointLoadOption)
    {
        mUpdateJointLoadOption = false;

        if (!mJointLoadsList.isEmpty())
        {
            if (!mJointLoadsCheckBox->isEnabled())
            {
                mJointLoadsCheckBox->setEnabled(true);
                mJointLoadsCheckBox->setChecked(false);
            }
            else
            {
                mJointLoadsCheckBox->setChecked(false);
            }
        }
        else
        {
            if (mJointLoadsCheckBox->isEnabled())
            {
                mJointLoadsCheckBox->setChecked(false);
                mJointLoadsCheckBox->setEnabled(false);
            }
        }
    }

    if (mUpdateSelfWeightOption)
    {
        mUpdateSelfWeightOption = false;

        if (mBarsStandardItemModel->columnCount() == 5)
        {
            if (mIsStable)
            {
                if (!mSelfWeightCheckBox->isEnabled())
                {
                    mSelfWeightCheckBox->setEnabled(true);
                    mSelfWeightCheckBox->setChecked(false);
                }
                else
                {
                    mSelfWeightCheckBox->setChecked(false);
                }
            }
            else
            {
                if (mSelfWeightCheckBox->isEnabled())
                {
                    mSelfWeightCheckBox->setChecked(false);
                    mSelfWeightCheckBox->setEnabled(false);
                }
            }
        }
        else
        {
            if (mSelfWeightCheckBox->isEnabled())
            {
                mSelfWeightCheckBox->setChecked(false);
                mSelfWeightCheckBox->setEnabled(false);
            }
        }
    }

    if (mUpdateSupportSettlementOption)
    {
        mUpdateSupportSettlementOption = false;

        if (!mSupportSettlementsList.isEmpty())
        {
            if (!mSupportSettlementsCheckBox->isEnabled())
            {
                mSupportSettlementsCheckBox->setEnabled(true);
                mSupportSettlementsCheckBox->setChecked(false);
            }
            else
            {
                mSupportSettlementsCheckBox->setChecked(false);
            }
        }
        else
        {
            if (mSupportSettlementsCheckBox->isEnabled())
            {
                mSupportSettlementsCheckBox->setChecked(false);
                mSupportSettlementsCheckBox->setEnabled(false);
            }
        }
    }

    if (mUpdateThermalEffectOption)
    {
        mUpdateThermalEffectOption = false;

        if (!mThermalEffectsList.isEmpty())
        {
            if (!mThermalEffectsCheckBox->isEnabled())
            {
                mThermalEffectsCheckBox->setEnabled(true);
                mThermalEffectsCheckBox->setChecked(false);
            }
            else
            {
                mThermalEffectsCheckBox->setChecked(false);
            }
        }
        else
        {
            if (mThermalEffectsCheckBox->isEnabled())
            {
                mThermalEffectsCheckBox->setChecked(false);
                mThermalEffectsCheckBox->setEnabled(false);
            }
        }
    }

    if (mUpdateFabricationErrorOption)
    {
        mUpdateFabricationErrorOption = false;

        if (!mFabricationErrorsList.isEmpty())
        {
            if (!mFabricationErrorsCheckBox->isEnabled())
            {
                mFabricationErrorsCheckBox->setEnabled(true);
                mFabricationErrorsCheckBox->setChecked(false);
            }
            else
            {
                mFabricationErrorsCheckBox->setChecked(false);
            }
        }
        else
        {
            if (mFabricationErrorsCheckBox->isEnabled())
            {
                mFabricationErrorsCheckBox->setChecked(false);
                mFabricationErrorsCheckBox->setEnabled(false);
            }
        }
    }

    if (mUpdateInfluenceLoadOption)
    {
        mUpdateInfluenceLoadOption = false;

        if (!mInfluenceLoadsList.isEmpty())
        {
            if (!mInfluenceLoadsCheckBox->isEnabled())
            {
                mInfluenceLoadsCheckBox->setEnabled(true);
                mInfluenceLoadsCheckBox->setChecked(false);
            }
            else
            {
                mInfluenceLoadsCheckBox->setChecked(false);
            }

            if (!mInfluenceLoadsComboBox->isEnabled())
            {
                mInfluenceLoadsComboBox->setEnabled(true);
                mInfluenceLoadsComboBox->clear();
            }
            else
            {
                mInfluenceLoadsComboBox->clear();
            }


            foreach (InfluenceLoad *influenceLoad, mInfluenceLoadsList)
            {
                mInfluenceLoadsComboBox->addItem(influenceLoad->name());
            }

            mInfluenceLoadsComboBox->setCurrentIndex(-1);
        }
        else
        {
            if (mInfluenceLoadsCheckBox->isEnabled())
            {
                mInfluenceLoadsCheckBox->setChecked(false);
                mInfluenceLoadsCheckBox->setEnabled(false);
                mInfluenceLoadsComboBox->clear();
                mInfluenceLoadsComboBox->setEnabled(false);
            }
        }
    }
}

void Solver::setSolutionTableView(int index)
{
    if (index == 0)
    {
        mSolutionsTableView->setModel(mJointDeflectionsStandardItemModel);
    }
    else if (index == 1)
    {
        mSolutionsTableView->setModel(mBarLoadsStandardItemModel);
    }
    else if (index == 2)
    {
        mSolutionsTableView->setModel(mSupportReactionsStandardItemModel);
    }
    else if (index == 3)
    {
        mSolutionsTableView->setModel(mInfluenceLoadResultsStandardItemModel);
    }
    else
    {
        mSolutionsTableView->setModel(mJointDeflectionsStandardItemModel);
    }

    mSolutionsTableView->setCornerButtonEnabled(false);
    mSolutionsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mSolutionsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mSolutionsTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    mSolutionsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int minimumSectionSize = 80;
    mSolutionsTableView->horizontalHeader()->setMinimumSectionSize(minimumSectionSize);
}

void Solver::solveModel()
{
    QList<JointLoad *> jointLoadsList;
    bool includeSelfWeight = false;
    bool areaModulusOption = false;
    QList<SupportSettlement *> supportSettlementsList;
    QList<ThermalEffect *> thermalEffectsList;
    QList<FabricationError *> fabricationErrorsList;
    QList<InfluenceLoad *> influenceLoadsList;
    QString influenceLoadName;

    int count = 0;

    if (mJointLoadsCheckBox->isEnabled())
    {
        if (mJointLoadsCheckBox->isChecked())
        {
            ++count;
            jointLoadsList = mJointLoadsList;
        }
    }

    if (mSelfWeightCheckBox->isEnabled())
    {
        if (mSelfWeightCheckBox->isChecked())
        {
            ++count;
            includeSelfWeight = true;
        }
    }

    if (mBarsStandardItemModel->columnCount() == 5)
    {
        areaModulusOption = true;
    }

    if (mSupportSettlementsCheckBox->isEnabled())
    {
        if (mSupportSettlementsCheckBox->isChecked())
        {
            ++count;
            supportSettlementsList = mSupportSettlementsList;
        }
    }

    if (mThermalEffectsCheckBox->isEnabled())
    {
        if (mThermalEffectsCheckBox->isChecked())
        {
            ++count;
            thermalEffectsList = mThermalEffectsList;
        }
    }

    if (mFabricationErrorsCheckBox->isEnabled())
    {
        if (mFabricationErrorsCheckBox->isChecked())
        {
            ++count;
            fabricationErrorsList = mFabricationErrorsList;
        }
    }

    if (mInfluenceLoadsCheckBox->isEnabled())
    {
        if (mInfluenceLoadsCheckBox->isChecked())
        {
            if (mInfluenceLoadsComboBox->currentIndex() < 0)
            {
                QMessageBox messageBox(this);
                messageBox.setWindowTitle(tr("Loads"));
                messageBox.setText(tr("Influence load name"));
                messageBox.setInformativeText(tr("No selection!"));
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.exec();
                return;
            }
            else
            {
                ++count;
                influenceLoadsList         = mInfluenceLoadsList;
                influenceLoadName          = mInfluenceLoadsComboBox->currentText();
                mSolutionInfluenceLoadName = influenceLoadName;
            }
        }
    }

    if (count == 0)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Loads"));
        messageBox.setText(tr("Solution loads"));
        messageBox.setInformativeText(tr("No selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    ModelSolver *modelSolver = new ModelSolver(mJointsList,
                                               mBarsList,
                                               mSupportsList,
                                               jointLoadsList,
                                               includeSelfWeight,
                                               areaModulusOption,
                                               supportSettlementsList,
                                               thermalEffectsList,
                                               fabricationErrorsList,
                                               influenceLoadsList,
                                               influenceLoadName,
                                               mInfluenceLoadResult,
                                               mUnitsAndLimits);

    qRegisterMetaType< QList<qreal> >("QList<qreal>");

    connect(modelSolver, SIGNAL(jointHorizontalDeflectionsSignal(QList<qreal>)),
            this, SLOT(setJointHorizontalDeflectionsList(QList<qreal>)));
    connect(modelSolver, SIGNAL(jointVerticalDeflectionsSignal(QList<qreal>)),
            this, SLOT(setJointVerticalDeflectionsList(QList<qreal>)));
    connect(modelSolver, SIGNAL(barLoadsSignal(QList<qreal>)), this, SLOT(setBarLoadsList(QList<qreal>)));
    connect(modelSolver, SIGNAL(reactionHorizontalComponentsSignal(QList<qreal>)),
            this, SLOT(setReactionHorizontalComponentsList(QList<qreal>)));
    connect(modelSolver, SIGNAL(reactionVerticalComponentsSignal(QList<qreal>)),
            this, SLOT(setReactionVerticalComponentsList(QList<qreal>)));
    connect(modelSolver, SIGNAL(hasSolution()), this, SLOT(enableSolutionReset()));
    connect(modelSolver, SIGNAL(notesSignal(QString)), this, SLOT(setNote(QString)));

    modelSolver->start();
}

void Solver::setSolutionDisplayOptionA(int index)
{
    switch (index)
    {
        case -1:
            emit modelSolutionSignal(ModelViewer::NO_SOLUTION, 0);
            break;
        case 0:
            emit modelSolutionSignal(ModelViewer::JOINT_DEFLECTION, 0);
            break;
        case 1:
            emit modelSolutionSignal(ModelViewer::BAR_LOAD, 0);
            break;
        case 2:
            emit modelSolutionSignal(ModelViewer::SUPPORT_REACTION, 0);
            break;
        default:
            break;
    }
}

void Solver::setSolutionDisplayOptionB(int index)
{
    if (index >= 0)
    {
        emit modelSolutionSignal(ModelViewer::INFLUENCE_LOAD_SOLUTION, index);
    }
}

void Solver::setInfluenceLoadBarOptions(int index)
{
    if (index == 3)
    {
        mInfluenceLoadBarComboBox->setEnabled(true);

        foreach (Bar *bar, mBarsList)
        {
            mInfluenceLoadBarComboBox->addItem(QString::number(mBarsList.indexOf(bar) + 1));
        }

        mInfluenceLoadBarComboBox->setCurrentIndex(0);
    }
    else
    {
        if (mInfluenceLoadBarComboBox->isEnabled())
        {
            mInfluenceLoadBarComboBox->clear();
            mInfluenceLoadBarComboBox->setEnabled(false);
        }
    }
}

void Solver::enableSolutionReset()
{
    mHasSolution = true;
    mSolutionOptionComboBox->setEnabled(true);
    mSolutionOptionComboBox->setCurrentIndex(0);
    mSolvePushButton->setEnabled(false);
    mClearPushButton->setEnabled(true);
    mReportPushButton->setEnabled(true);
    setSolutionTableViewModels();
}

void Solver::setSolutionTableViewModels()
{
    int count = mHorizontalDeflectionComponentsList.size();

    mJointDeflectionsStandardItemModel->setRowCount(0);

    for (int i = 0; i < count; ++i)
    {
        QStandardItem *standardItem;
        QList<QStandardItem *> standardItemsList;
        standardItem = new QStandardItem(QString::number(i + 1));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(mHorizontalDeflectionComponentsList.at(i), 'g', 6));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(mVerticalDeflectionComponentsList.at(i), 'g', 6));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        mJointDeflectionsStandardItemModel->appendRow(standardItemsList);
    }

    count = mBarLoadsList.size();

    mBarLoadsStandardItemModel->setRowCount(0);

    for (int i = 0; i < count; ++i)
    {
        QStandardItem *standardItem;
        QList<QStandardItem *> standardItemsList;
        standardItem = new QStandardItem(QString::number(i + 1));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(mBarLoadsList.at(i), 'g', 6));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        mBarLoadsStandardItemModel->appendRow(standardItemsList);
    }

    count = mReactionHorizontalComponentsList.size();

    mSupportReactionsStandardItemModel->setRowCount(0);

    for (int i = 0; i < count; ++i)
    {
        int supportNumber = i + 1;
        Support *support  = mSupportsList.at(i);
        Joint *joint      = support->supportJoint();
        int jointNumber   = mJointsList.indexOf(joint) + 1;
        QStandardItem *standardItem;
        QList<QStandardItem *> standardItemsList;
        standardItem = new QStandardItem(tr("%1 @ joint %2")
                                         .arg(QString::number(supportNumber))
                                         .arg(QString::number(jointNumber)));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(mReactionHorizontalComponentsList.at(i), 'g', 6));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(mReactionVerticalComponentsList.at(i), 'g', 6));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        mSupportReactionsStandardItemModel->appendRow(standardItemsList);
    }

    count = mInfluenceLoadResult->barsCount();

    mInfluenceLoadResultsStandardItemModel->setRowCount(0);

    for (int i = 0; i < count; ++i)
    {
        QStandardItem *standardItem;
        QList<QStandardItem *> standardItemsList;
        standardItem = new QStandardItem(QString::number(mInfluenceLoadResult->minLoad(i), 'g', 6));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(mInfluenceLoadResult->minloadPosition(i));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(QString::number(mInfluenceLoadResult->maxLoad(i), 'g', 6));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        standardItem = new QStandardItem(mInfluenceLoadResult->maxloadPosition(i));
        standardItem->setEditable(false);
        standardItem->setTextAlignment(Qt::AlignCenter);
        standardItemsList.append(standardItem);
        mInfluenceLoadResultsStandardItemModel->appendRow(standardItemsList);
    }
}

void Solver::setJointHorizontalDeflectionsList(QList<qreal> horizontalDeflectionComponentsList)
{
    mHorizontalDeflectionComponentsList = horizontalDeflectionComponentsList;
}

void Solver::setJointVerticalDeflectionsList(QList<qreal> verticalDeflectionComponentsList)
{
    mVerticalDeflectionComponentsList = verticalDeflectionComponentsList;
}

void Solver::setBarLoadsList(QList<qreal> barLoadsList)
{
    mBarLoadsList = barLoadsList;
}

void Solver::setReactionHorizontalComponentsList(QList<qreal> reactionHorizontalComponentsList)
{
    mReactionHorizontalComponentsList = reactionHorizontalComponentsList;
}

void Solver::setReactionVerticalComponentsList(QList<qreal> reactionVerticalComponentsList)
{
    mReactionVerticalComponentsList = reactionVerticalComponentsList;
}

void Solver::showScaleDeflectionsDialog()
{
    if (mScaleDeflectionsDialog->position() != QPoint(0, 0))
    {
        mScaleDeflectionsDialog->move(mScaleDeflectionsDialog->position());
    }

    mScaleDeflectionsDialog->show();
    mScaleDeflectionsDialog->raise();
    mScaleDeflectionsDialog->activateWindow();
}

void Solver::clearSolution()
{
    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Clear Solution"));
    messageBox.setText(tr("Confirm clear solution : All results will be lost!"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (messageBox.exec() == QMessageBox::Cancel)
    {
        return;
    }
    else
    {
        mHorizontalDeflectionComponentsList.clear();
        mVerticalDeflectionComponentsList.clear();
        mBarLoadsList.clear();
        mReactionHorizontalComponentsList.clear();
        mReactionVerticalComponentsList.clear();
        mInfluenceLoadResult->resetParameters();

        mHasSolution = false;
        mSolutionInfluenceLoadName.clear();
        mSolutionOptionComboBox->setCurrentIndex(-1);
        mSolutionOptionComboBox->setEnabled(false);
        mSolvePushButton->setEnabled(true);
        mClearPushButton->setEnabled(false);
        mReportPushButton->setEnabled(false);
        setSolutionTableViewModels();
    }
}

void Solver::reportSolution()
{
    QString exportDirectory;
    exportDirectory = QFileDialog::getExistingDirectory(this,
                                                        QString("Select Export Folder"),
                                                        mLastReportLocation,
                                                        QFileDialog::ShowDirsOnly);

    if (!exportDirectory.isEmpty())
    {
        mLastReportLocation = exportDirectory;
        QString title;

        if (!mCurrentFile.isEmpty())
        {
            title = strippedName(mCurrentFile);
            title.remove(tr(".ttmdl"));
        }
        else
        {
            title = tr("untitled");
        }

        HtmlReportExporter exporter(title,
                                    &mDescriptionText,
                                    exportDirectory,
                                    &mJointsList,
                                    &mBarsList,
                                    mBarsStandardItemModel,
                                    &mSupportsList,
                                    mModelViewer,
                                    mJointLoadsCheckBox,
                                    mSelfWeightCheckBox,
                                    mSupportSettlementsCheckBox,
                                    mThermalEffectsCheckBox,
                                    mFabricationErrorsCheckBox,
                                    mInfluenceLoadsCheckBox,
                                    &mJointLoadsList,
                                    &mSupportSettlementsList,
                                    &mThermalEffectsList,
                                    &mFabricationErrorsList,
                                    &mInfluenceLoadsList,
                                    &mHorizontalDeflectionComponentsList,
                                    &mVerticalDeflectionComponentsList,
                                    &mBarLoadsList,
                                    &mReactionHorizontalComponentsList,
                                    &mReactionVerticalComponentsList,
                                    &mSolutionInfluenceLoadName,
                                    mInfluenceLoadResult,
                                    &mUnitsAndLimits,
                                    this);

        exporter.exportHtmlReport();
    }
}
