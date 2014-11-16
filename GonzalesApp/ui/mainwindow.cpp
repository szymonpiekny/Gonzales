#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <stdexcept>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../qa/IQARepository.h"
#include "../user/UserInfo.h"
#include "../common/Common.h"
#include "../uiobservers/QuestionCollectionPresenter.h"
#include "../qa/QAsSelection/QAsToLearnByUserChecker.h"
#include "TeacherControllerFactory.h"
#include "../uiobservers/QAPresenter.h"
#include <QInputDialog>
#include <QDebug>

void MainWindow::loadUserData()
{
    qARepository->load();
}

MainWindow::MainWindow(const UserInfo &userInfo,
                       std::shared_ptr<IQARepository> qARepository,
                       std::shared_ptr<ImportHandler> importHandler,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_userInfo(userInfo),
    qARepository(qARepository),
    importHandler(importHandler),
    stateIdle(),
    stateLearn(),
    stateQuestionQiven(&stateLearn),
    stateShowAnswer(&stateLearn),
    stateAnswerVerified(&stateLearn)
{
    ui->setupUi(this);
    setupWindowUI();
    setupControllers();
    loadUserData();

}

void MainWindow::setupWindowUI()
{
    setWindowTitle("Logged as "+m_userInfo.login);
    setupStateMachine();
}

void MainWindow::setupControllers()
{
    std::shared_ptr<IQuestionCollectionPresenter> l_questionCollectionPresenter(
                                new QuestionCollectionPresenter(*ui->treeWidgetQuestions));

    std::shared_ptr<IQAPresenter> l_qaPresenter =
            std::make_shared<QAPresenter>(*ui->textEditQuestion,
                                          *ui->textEditAnswer);

    qARepository->registerQuestionCollectionPresenter(l_questionCollectionPresenter);
    TeacherControllerFactory teacherControllerFactory(qARepository);
    std::shared_ptr<QAsSelector> selector = std::make_shared<QAsSelector>();
    selectorSettings = selector;

    auto getterFun = [this](){ return this->ui->horizontalSlider->value();};
    auto uiSetterFun = [this](const int& val){ this->ui->horizontalSlider->setValue(val);};
    auto userSetter = [this](const int& val)
    {
        int newMaterial = this->ui->spinBoxMaterialAmount->value()*(double)val/100;
        int oldMaterial = 1 - newMaterial;
        int repetitions = oldMaterial / 2;
        int notLearned = oldMaterial/2;
        this->selectorSettings->setMaxQA(QAsSelector::SettingsMaxNewQAs, newMaterial);
        this->selectorSettings->setMaxQA(QAsSelector::SettingsMaxForRepeat, repetitions);
        this->selectorSettings->setMaxQA(QAsSelector::SettingsMaxNotLearned, notLearned);
    };
    std::shared_ptr<ISetting> setting = std::make_shared<Setting<int> >
    (
        getterFun,
        uiSetterFun,
        userSetter,
        std::make_shared<ApplicationSettings>(),
        g_Users + "/" + m_userInfo.login + "/" + "settings",
        "MaterialDisposal"
    );

    settings.push_back(setting);
    for(const auto& setting: settings)
    {
        setting->load();
    }

    teacherController = teacherControllerFactory.create(l_qaPresenter, selector);
    connect(teacherController.get(), SIGNAL(stopLearn()), this, SIGNAL(stopLearn()));
}

void MainWindow::setupStateMachine()
{
    stateLearn.setInitialState(&stateQuestionQiven);
    stateIdle.addTransition(this, SIGNAL(startLearn()), &stateLearn);

    stateLearn.addTransition(this, SIGNAL(emptyQA()), &stateIdle);
    stateLearn.addTransition(this, SIGNAL(stopLearn()), &stateIdle);
    stateQuestionQiven.addTransition(this, SIGNAL(showAnswer()), &stateShowAnswer);
    stateShowAnswer.addTransition(this, SIGNAL(questionGiven()), &stateQuestionQiven);

    stateIdle.assignProperty(ui->pushButtonDontKnowIt, "enabled", false);
    stateIdle.assignProperty(ui->pushButtonKnowIt, "enabled", false);
    stateIdle.assignProperty(ui->pushButtonShowAnswer, "enabled", false);

    stateQuestionQiven.assignProperty(ui->pushButtonDontKnowIt, "enabled", false);
    stateQuestionQiven.assignProperty(ui->pushButtonKnowIt, "enabled", false);
    stateQuestionQiven.assignProperty(ui->pushButtonShowAnswer, "enabled", true);

    stateShowAnswer.assignProperty(ui->pushButtonDontKnowIt, "enabled", true);
    stateShowAnswer.assignProperty(ui->pushButtonKnowIt, "enabled", true);
    stateShowAnswer.assignProperty(ui->pushButtonShowAnswer, "enabled", false);

    stateMachine.addState(&stateIdle);
    stateMachine.addState(&stateLearn);

    stateMachine.setInitialState(&stateIdle);
    stateMachine.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showNextQuestion()
{
    teacherController->showNextQuestion();
}

void MainWindow::on_pushButtonKnowIt_clicked()
{
    teacherController->markAsKnownAndShowNextQuestion();
    emit questionGiven();
}

void MainWindow::on_pushButtonDontKnowIt_clicked()
{
    teacherController->markAsUnknownAndShowNextQuestion();
    emit questionGiven();
}

void MainWindow::on_pushButtonShowAnswer_clicked()
{
    teacherController->showCorrectAnswer();
    emit showAnswer();
}

void MainWindow::on_actionStart_triggered()
{
    teacherController->startTeaching();
    ui->MainTab->setCurrentIndex(0);
    emit startLearn();
}

void MainWindow::on_actionImport_QA_triggered()
{
    importHandler->import();
}

void MainWindow::on_treeWidgetQuestions_itemChanged(QTreeWidgetItem *item, int column)
{
    if(column != 2)
        return;
    QAsToLearnByUserChecker selector(qARepository);
    selector.select(*item);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QString newMaterialLabel = "New material :";
    newMaterialLabel += QString::number(value);
    newMaterialLabel += "%";
    ui->labelMaterial->setText(newMaterialLabel);

    QString oldMaterialLabel = "Material for repetition :";
    oldMaterialLabel += QString::number(100 - value);
    oldMaterialLabel += "%";
    ui->labelRepetitions->setText(oldMaterialLabel);
}

void MainWindow::on_toolButtonAddPeriod_clicked()
{
    QInputDialog dialog(this);
    dialog.setModal(true);
    QString days = dialog.getText(this, "Get period", "Period");

    bool ok;
    days.toInt(&ok);
    if(ok)
    {
        ui->listWidgetRepetitionPeriods->addItem(days);
        std::vector<QListWidgetItem> widgets;
        widgets.reserve(ui->listWidgetRepetitionPeriods->count());

        for(int i = 0; i < ui->listWidgetRepetitionPeriods->count(); ++i)
        {
            widgets.push_back(*ui->listWidgetRepetitionPeriods->item(i));
        }

        std::sort(widgets.begin(), widgets.end(),
                  [](const QListWidgetItem& widgetL,
                     const QListWidgetItem& widgetR){
                     return widgetL.text().toInt() < widgetR.text().toInt();});

        ui->listWidgetRepetitionPeriods->clear();
        for(const auto& widget: widgets)
        {
            ui->listWidgetRepetitionPeriods->addItem(widget.text());
        }
    }
}

void MainWindow::on_toolButtonRemovePeriod_clicked()
{
    ui->listWidgetRepetitionPeriods->takeItem(ui->listWidgetRepetitionPeriods->currentRow());
}

void MainWindow::on_spinBoxMaterialAmount_valueChanged(int)
{
    int value = ui->horizontalSlider->value();
    QString newMaterialLabel = "New material :";
    newMaterialLabel += QString::number(value);
    newMaterialLabel += "%";
    ui->labelMaterial->setText(newMaterialLabel);

    QString oldMaterialLabel = "Material for repetition :";
    oldMaterialLabel += QString::number(100 - value);
    oldMaterialLabel += "%";
    ui->labelRepetitions->setText(oldMaterialLabel);


}

void MainWindow::on_pushButton_clicked()
{
    for(const auto& setting: settings)
    {
        setting->update();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    for(const auto& setting: settings)
    {
        setting->load();
    }
}
