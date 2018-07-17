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

/* solver.h */

#ifndef SOLVER_H
#define SOLVER_H

#include <algorithm>
#include <cmath>
#include <config.h>

#include <QAction>
#include <QCheckBox>
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QSaveFile>
#include <QSettings>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QTableView>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "bar.h"
#include "fabricationerror.h"
#include "joint.h"
#include "jointload.h"
#include "point.h"
#include "support.h"
#include "supportsettlement.h"
#include "thermaleffect.h"

#include "abouttrusstablesdialog.h"
#include "addbarsdialog.h"
#include "addfabricationerrorsdialog.h"
#include "addinfluenceloadsdialog.h"
#include "addjointloadsdialog.h"
#include "addjointsdialog.h"
#include "addsupportsdialog.h"
#include "addsupportsettlementsdialog.h"
#include "addthermaleffectsdialog.h"
#include "combobox.h"
#include "deletebarsdialog.h"
#include "deletefabricationerrorsdialog.h"
#include "deleteinfluenceloadsdialog.h"
#include "deletejointloadsdialog.h"
#include "deletejointsdialog.h"
#include "deletesupportsdialog.h"
#include "deletesupportsettlementsdialog.h"
#include "deletethermaleffectsdialog.h"
#include "editbarsdialog.h"
#include "editfabricationerrorsdialog.h"
#include "editinfluenceloadsdialog.h"
#include "editjointloadsdialog.h"
#include "editjointsdialog.h"
#include "editsupportsdialog.h"
#include "editsupportsettlementsdialog.h"
#include "editthermaleffectsdialog.h"
#include "exportmodelimagedialog.h"
#include "htmlreportexporter.h"
#include "influenceloadresult.h"
#include "modelareadialog.h"
#include "modelchecker.h"
#include "modelsolver.h"
#include "modelviewer.h"
#include "notesdialog.h"
#include "scaledeflectionsdialog.h"
#include "scaleforcesdialog.h"
#include "scrollarea.h"
#include "unitsandlimits.h"
#include "unitsandsetupdialog.h"

class Solver : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Solver(QWidget *parent = 0);

        ~Solver();

        bool loadFile(const QString &fileName);

    protected:
        void closeEvent(QCloseEvent *event);

    signals:
        void disableJointsInput();
        void enableJointsInput();
        void updateModelSignal();
        void modelLoadSignal(ModelViewer::LoadDisplayOption option);
        void modelSolutionSignal(ModelViewer::SolutionDisplayOption option, int influenceLoadBarIndex);

    private:
        void initializeUiComponents();
        void createJointsModelHeader();
        void createBarsModelHeader();
        void createSupportsModelHeader();
        void createJointLoadsModelHeader();
        void createSupportSettlementsModelHeader();
        void createThermalEffectsModelHeader();
        void createFabricationErrorsModelHeader();
        void createInfluenceLoadsModelHeader();
        void createJointDeflectionsModelHeader();
        void createBarLoadsModelHeader();
        void createSupportReactionsModelHeader();
        void createInfluenceLoadResultsModelHeader();
    
        void readSettings();
        void writeSettings();
        bool promptForSave();
        bool saveFile(const QString &fileName);
        void setCurrentFile(const QString &fileName);
        void updateRecentFileActions();
        QString strippedName(const QString &fullFileName);

    private slots:
        void updateModelViewer();
        void newFile();
        void open();
        bool promptForOpenFileName();
        bool save();
        bool saveAs();
        void openRecentFile();
        void clearRecentFileActions();
        void setUnits();
        void updateUnits();
        void showHelp();
        void showAboutTrussTablesDialog();
        void showAboutQtDialog();
        void showSelectionPanel();
        void showNotes();
        void showModelAreaDialog();
        void showExportModelImageDialog();
    
        void showAddJointsDialog();
        void showEditJointsDialog();
        void showDeleteJointsDialog();
    
        void setCoordinatesLabelText(QString &position);
        void setJointsSelectionMode(bool checked);
    
        void normalizeJointCoordinates();
        bool hasLoads();
    
        void addJoints(qreal deltaX,
                       qreal deltaY,
                       int   count);
        int addJoint(qreal u, qreal v);
        void mirrorJoints(QString axis, int axisJointNumber);
        void editJoints(qreal deltaX, qreal deltaY);
        int editJoints(qreal      deltaX,
                       qreal      deltaY,
                       QList<int> selectedJoints);
        void deleteJoints();
    
        void showAddBarsDialog();
        void showEditBarsDialog();
        void showDeleteBarsDialog();
    
        void setBarsSelectionMode(bool checked);
        void setAreaModulusOption(bool areaModulusOption);
        void setFactorOption(bool factorOption);
    
        void addBars(int   firstJoint,
                     int   secondJoint,
                     qreal area,
                     qreal modulus,
                     qreal factor,
                     qreal unitWeight,
                     int   repeat,
                     qreal deltaX,
                     qreal deltaY);
        void addBar(qreal xFirstJoint,
                    qreal yFirstJoint,
                    qreal xSecondJoint,
                    qreal ySecondJoint,
                    qreal area,
                    qreal modulus,
                    qreal factor,
                    qreal unitWeight);
        void mirrorBars(QString axis, int axisJointNumber);
        void linkBars(qreal area,
                      qreal modulus,
                      qreal factor,
                      qreal unitWeight);
        void editBar(int   firstJointNumber,
                     int   secondJointNumber,
                     qreal area,
                     qreal modulus,
                     qreal factor,
                     qreal unitWeight);
        void editBar(int   barIndex,
                     qreal area,
                     qreal modulus,
                     qreal factor,
                     qreal unitWeight);
        void deleteBar();
        void deleteBar(int barIndex);
    
        void showAddSupportsDialog();
        void showEditSupportsDialog();
        void showDeleteSupportsDialog();
    
        void showScaleForcesDialog();
    
        void setSupportsSelectionMode(bool checked);
    
        void addSupports(int                         supportJointNumber,
                         UnitsAndLimits::SupportType type,
                         qreal                       angle);
    
        void editSupport(int                         supportJointNumber,
                         UnitsAndLimits::SupportType type,
                         qreal                       angle);
        void deleteSupport();
    
        void onTabIndexChanged(int option);
        void onActionCheckModelTriggered();
        void checkModel();
        void setNote(QString note);
        void setRedundantBarsList(QList<Bar *> barsList);
        void setRedundantSupportsList(QList<Support *> supportsList);
        void setModelStability(bool stability);
        void enableLoadsEntry();
    
        void setLoadsTableView(int index);
        void setLoadDisplayOption(int index);
        void setLoadDisplayOption(bool checked);
    
        void setLoadsSelectionMode(bool checked);
    
        void showAddLoadsDialog();
        void showEditLoadsDialog();
        void showDeleteLoadsDialog();
    
        void addJointLoad(int     loadJointNumber,
                          qreal   horizontalComponent,
                          qreal   verticalComponent,
                          QString position);
        void editJointLoad(int     loadJointNumber,
                           qreal   horizontalComponent,
                           qreal   verticalComponent,
                           QString position);
        void deleteJointLoad();
    
        void addSupportSettlement(int supportNumber, qreal settlement);
        void editSupportSettlement(int supportNumber, qreal settlement);
        void deleteSupportSettlement();
    
        void addThermalEffect(int   thermalEffectBarNumber,
                              qreal temperatureChange,
                              qreal thermalCoefficient);
        void editThermalEffect(int   thermalEffectBarNumber,
                               qreal temperatureChange,
                               qreal thermalCoefficient);
        void deleteThermalEffect();
    
        void addFabricationError(int fabricationErrorBarNumber, qreal lengthError);
        void editFabricationError(int fabricationErrorBarNumber, qreal lengthError);
        void deleteFabricationError();
    
        void addInfluenceLoad(QString      name,
                              QString      direction,
                              QList<qreal> pointLoads,
                              QList<qreal> pointLoadPositions);
        void editInfluenceLoad(QString      name,
                               QString      direction,
                               QList<qreal> pointLoads,
                               QList<qreal> pointLoadPositions);
        void deleteInfluenceLoad();
    
        void setSolutionOptions();
        void setSolutionTableView(int index);
        void solveModel();
        void setSolutionDisplayOptionA(int index);
        void setSolutionDisplayOptionB(int index);
        void setInfluenceLoadBarOptions(int index);
        void enableSolutionReset();
        void setSolutionTableViewModels();
        void setJointHorizontalDeflectionsList(QList<qreal> horizontalDeflectionComponentsList);
        void setJointVerticalDeflectionsList(QList<qreal> verticalDeflectionComponentsList);
        void setBarLoadsList(QList<qreal> barLoadsList);
        void setReactionHorizontalComponentsList(QList<qreal> reactionHorizontalComponentsList);
        void setReactionVerticalComponentsList(QList<qreal> reactionVerticalComponentsList);
    
        void showScaleDeflectionsDialog();
    
        void clearSolution();
        void reportSolution();

    private:
        QTabWidget  *mSelectionTabWidget;
        QDockWidget *mSelectionDockWidget;
    
        QRadioButton *mJointsSingleRadioButton;
        QRadioButton *mJointsMultipleRadioButton;
        QGroupBox    *mJointsModeGroupBox;
        QToolButton  *mJointsClearToolButton;
        QPushButton  *mAddJointsPushButton;
        QPushButton  *mEditJointsPushButton;
        QPushButton  *mDeleteJointsPushButton;
        QTableView   *mJointsTableView;
    
        QRadioButton *mBarsSingleRadioButton;
        QRadioButton *mBarsMultipleRadioButton;
        QGroupBox    *mBarsModeGroupBox;
        QRadioButton *mFactorRadioButton;
        QRadioButton *mValueRadioButton;
        QGroupBox    *mAxialRigidityOptionGroupBox;
        QToolButton  *mBarsClearToolButton;
        QPushButton  *mAddBarsPushButton;
        QPushButton  *mEditBarsPushButton;
        QPushButton  *mDeleteBarsPushButton;
        QTableView   *mBarsTableView;
    
        QRadioButton *mSupportsSingleRadioButton;
        QRadioButton *mSupportsMultipleRadioButton;
        QGroupBox    *mSupportsModeGroupBox;
        QToolButton  *mSupportsClearToolButton;
        QPushButton  *mAddSupportsPushButton;
        QPushButton  *mEditSupportsPushButton;
        QPushButton  *mDeleteSupportsPushButton;
        QTableView   *mSupportsTableView;
    
        QListWidget  *mLoadTypeListWidget;
        QRadioButton *mLoadsSingleRadioButton;
        QRadioButton *mLoadsMultipleRadioButton;
        QGroupBox    *mLoadsModeGroupBox;
        QRadioButton *mYesRadioButton;
        QRadioButton *mNoRadioButton;
        QGroupBox    *mSelfWeightDisplayGroupBox;
        QToolButton  *mLoadsClearToolButton;
        QPushButton  *mAddLoadPushButton;
        QPushButton  *mEditLoadPushButton;
        QPushButton  *mDeleteLoadPushButton;
        QTableView   *mLoadsTableView;
    
        QGroupBox   *mLoadsGroupBox;
        QCheckBox   *mJointLoadsCheckBox;
        QCheckBox   *mSelfWeightCheckBox;
        QCheckBox   *mSupportSettlementsCheckBox;
        QCheckBox   *mThermalEffectsCheckBox;
        QCheckBox   *mFabricationErrorsCheckBox;
        QCheckBox   *mInfluenceLoadsCheckBox;
        ComboBox    *mInfluenceLoadsComboBox;
        QLabel      *mSolutionOptionLabel;
        ComboBox    *mSolutionOptionComboBox;
        QLabel      *mInfluenceLoadBarLabel;
        ComboBox    *mInfluenceLoadBarComboBox;
        QPushButton *mSolvePushButton;
        QPushButton *mClearPushButton;
        QPushButton *mReportPushButton;
        QTableView  *mSolutionsTableView;
    
        UnitsAndLimits         mUnitsAndLimits;
        QString                mDescriptionText;
        ScrollArea             *mScrollArea;
        ModelViewer            *mModelViewer;
        QLabel                 *mCoordinatesLabel;
        NotesDialog            *mNotesDialog;
        ModelAreaDialog        *mModelAreaDialog;
        ExportModelImageDialog *mExportModelImageDialog;
    
        QAction *mNewAction;
        QAction *mOpenAction;
        QAction *mSaveAction;
        QAction *mSaveAsAction;
        QAction *mExportModelImageAction;
        QAction *mExitAction;
        QAction *mSelectionPanelAction;
        QAction *mShowNotesAction;
        QAction *mUnitsAction;
        QAction *mSetModelAreaAction;
        QAction *mShowHideAxesAction;
        QAction *mShowHideJointNumberAction;
        QAction *mShowHideBarNumberAction;
        QAction *mScaleForcesAction;
        QAction *mScaleDeflectionsAction;
        QAction *mCheckModelAction;
        QAction *mHelpAction;
        QAction *mAboutTrussTablesAction;
        QAction *mAboutQtAction;
    
        QMenu *mFileMenu;
        QMenu *mOpenRecentMenu;
        QMenu *mViewMenu;
        QMenu *mModelMenu;
        QMenu *mHelpMenu;
    
        QToolBar *mToolBar;
    
        AddJointsDialog    *mAddJointsDialog;
        EditJointsDialog   *mEditJointsDialog;
        DeleteJointsDialog *mDeleteJointsDialog;
    
        AddBarsDialog    *mAddBarsDialog;
        EditBarsDialog   *mEditBarsDialog;
        DeleteBarsDialog *mDeleteBarsDialog;
    
        AddSupportsDialog    *mAddSupportsDialog;
        EditSupportsDialog   *mEditSupportsDialog;
        DeleteSupportsDialog *mDeleteSupportsDialog;
    
        ScaleForcesDialog *mScaleForcesDialog;
    
        AddJointLoadsDialog    *mAddJointLoadsDialog;
        EditJointLoadsDialog   *mEditJointLoadsDialog;
        DeleteJointLoadsDialog *mDeleteJointLoadsDialog;
    
        AddSupportSettlementsDialog    *mAddSupportSettlementsDialog;
        EditSupportSettlementsDialog   *mEditSupportSettlementsDialog;
        DeleteSupportSettlementsDialog *mDeleteSupportSettlementsDialog;
    
        AddThermalEffectsDialog    *mAddThermalEffectsDialog;
        EditThermalEffectsDialog   *mEditThermalEffectsDialog;
        DeleteThermalEffectsDialog *mDeleteThermalEffectsDialog;
    
        AddFabricationErrorsDialog    *mAddFabricationErrorsDialog;
        EditFabricationErrorsDialog   *mEditFabricationErrorsDialog;
        DeleteFabricationErrorsDialog *mDeleteFabricationErrorsDialog;
    
        AddInfluenceLoadsDialog    *mAddInfluenceLoadsDialog;
        EditInfluenceLoadsDialog   *mEditInfluenceLoadsDialog;
        DeleteInfluenceLoadsDialog *mDeleteInfluenceLoadsDialog;
    
        QList<Joint *>             mJointsList;
        QList<Bar *>               mBarsList;
        QList<Support *>           mSupportsList;
        QList<JointLoad *>         mJointLoadsList;
        QList<SupportSettlement *> mSupportSettlementsList;
        QList<ThermalEffect *>     mThermalEffectsList;
        QList<FabricationError *>  mFabricationErrorsList;
        QList<InfluenceLoad *>     mInfluenceLoadsList;
    
        QStandardItemModel *mJointsStandardItemModel;
        QStandardItemModel *mBarsStandardItemModel;
        QStandardItemModel *mSupportsStandardItemModel;
        QStandardItemModel *mJointLoadsStandardItemModel;
        QStandardItemModel *mSupportSettlementsStandardItemModel;
        QStandardItemModel *mThermalEffectsStandardItemModel;
        QStandardItemModel *mFabricationErrorsStandardItemModel;
        QStandardItemModel *mInfluenceLoadsStandardItemModel;
        QStandardItemModel *mJointDeflectionsStandardItemModel;
        QStandardItemModel *mBarLoadsStandardItemModel;
        QStandardItemModel *mSupportReactionsStandardItemModel;
        QStandardItemModel *mInfluenceLoadResultsStandardItemModel;
    
        bool             mModelCheckRequired;
        bool             mUpdateJointLoadOption;
        bool             mUpdateSelfWeightOption;
        bool             mUpdateSupportSettlementOption;
        bool             mUpdateThermalEffectOption;
        bool             mUpdateFabricationErrorOption;
        bool             mUpdateInfluenceLoadOption;
        QProgressBar     *mProgressBar;
        QList<Bar *>     mRedundantBarsList;
        QList<Support *> mRedundantSupportsList;
        bool             mIsStable;
    
        QList<qreal>        mHorizontalDeflectionComponentsList;
        QList<qreal>        mVerticalDeflectionComponentsList;
        QList<qreal>        mBarLoadsList;
        QList<qreal>        mReactionHorizontalComponentsList;
        QList<qreal>        mReactionVerticalComponentsList;
        InfluenceLoadResult *mInfluenceLoadResult;
    
        bool    mHasSolution;
        QString mSolutionInfluenceLoadName;
    
        ScaleDeflectionsDialog *mScaleDeflectionsDialog;
    
        QStringList mRecentFiles;
        QString     mLastOpenLocation;
        QString     mLastSaveLocation;
        QString     mLastExportModelImageLocation;
        QString     mLastReportLocation;
        QString     mCurrentFile;
        enum {MAX_RECENT_FILES = 10};
        QAction *mClearMenuAction;
        QAction *mRecentFileActionsArray[MAX_RECENT_FILES];
        QAction *mSeparatorAction;
};

#endif // SOLVER_H
