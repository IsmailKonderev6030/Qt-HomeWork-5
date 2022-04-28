#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , is_ctrl(false)
{
    ui->setupUi(this);

    setWindowTitle("Text editer");

    dialogForHotkeys = new HotKeys;

    keys = dialogForHotkeys->getKeys();

    filename = "";
    file.setFileName(filename);

    ui->comboBox->addItem("ENG");
    ui->comboBox->addItem("RUS");

    ui->comboBox_2->addItem(tr("Light"));
    ui->comboBox_2->addItem(tr("Dark"));

    ui->centralwidget->  setWindowTitle(tr("Text editer"));
    ui->pushButton   ->  setText(tr("Open file"));
    ui->pushButton_2 ->  setText(tr("Save file"));
    ui->pushButton_3 ->  setText(tr("Information"));
    ui->pushButton_4 ->  setText(tr("Hotkeys"));
    ui->checkBox     ->  setText(tr("Only read"));
    ui->label        ->  setText(tr("Language"));

    labelForInfo = new QLabel;
    labelForInfo->resize(200,120);

    QFile info("about.txt");
    if(info.open(QIODevice::ReadOnly))
        labelForInfo->setText(info.readAll());
    else
        labelForInfo->setText("Error open file");

    forInfo = new QWidget;
    forInfo->setWindowTitle(tr("Info"));

    QVBoxLayout test(forInfo);
    test.addWidget(labelForInfo);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete forInfo;
    delete labelForInfo;
    delete dialogForHotkeys;
}

void MainWindow::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,tr("Select file txt"),QDir::currentPath(),"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::ReadWrite)){
            QByteArray text = file.readAll();
            ui->plainTextEdit->setPlainText(text);
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    filename = QFileDialog::getSaveFileName(this,tr("Save file txt"),filename,"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::WriteOnly)){
            QString str = ui->plainTextEdit->toPlainText();
            QByteArray text = str.toUtf8();
            file.write(text);
            file.close();
            ui->plainTextEdit->setPlainText("");
        }
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    forInfo->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    dialogForHotkeys->setModal(true);
    dialogForHotkeys->exec();
    keys = dialogForHotkeys->getKeys();
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->plainTextEdit->setReadOnly(arg1);
}


void MainWindow::on_comboBox_activated(int index)
{
    if(index){
        if(translater.load("switch_ru")){
            qApp->installTranslator(&translater);
            dialogForHotkeys->SetLanguage(translater);
        }
    }
    else{
        qApp->removeTranslator(&translater);
        dialogForHotkeys->RemoveLanguage(translater);
    }

    MainWindow::retranslateUi();
}


void MainWindow::keyPressEvent(QKeyEvent *myKey){
    if(!is_ctrl && myKey->key() == ctrl) is_ctrl = true;
}


void MainWindow::keyReleaseEvent(QKeyEvent *myKey){
    if(myKey->key() == ctrl) is_ctrl = false;

    if(is_ctrl){
        quint32 temp_key = myKey->key() + ctrl;

        for(int i=0;i<4;i++)
            if(keys[i] == temp_key)
                switch (i) {
                case 0: MainWindow::on_pushButton_clicked();break;
                case 1: MainWindow::on_pushButton_2_clicked();break;
                case 2: createNewFile(); break;
                case 3: MainWindow::close();
                }
    }
    is_ctrl = false;
}


void MainWindow::createNewFile(){
    filename = QFileDialog::getSaveFileName(this,tr("Create file txt"),filename,"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::ReadWrite)){
            QByteArray text = file.readAll();
            ui->plainTextEdit->setPlainText(text);
        }
    }
}


void MainWindow::retranslateUi(){
    setWindowTitle(QCoreApplication::translate("MainWindow", "Text editer", nullptr));

    ui->pushButton   ->  setText(QCoreApplication::translate("MainWindow","Open file", nullptr));
    ui->pushButton_2 ->  setText(QCoreApplication::translate("MainWindow","Save file", nullptr));
    ui->pushButton_3 ->  setText(QCoreApplication::translate("MainWindow","Information", nullptr));
    ui->pushButton_4 ->  setText(QCoreApplication::translate("MainWindow","Hotkeys", nullptr));
    ui->checkBox     ->  setText(QCoreApplication::translate("MainWindow","Only read", nullptr));
    ui->label        ->  setText(QCoreApplication::translate("MainWindow","Language", nullptr));
    ui->comboBox_2   ->  setItemText(0,QCoreApplication::translate("MainWindow","Light", nullptr));
    ui->comboBox_2   ->  setItemText(1,QCoreApplication::translate("MainWindow","Dark", nullptr));

    dialogForHotkeys->retranslateUi();

}

void MainWindow::on_comboBox_2_activated(int index)
{
    if(index){
        qApp->setStyleSheet("QPushButton,QComboBox,QPlainTextEdit,QLabel,QCheckBox,QLineEdit {background-color: #2F4F4F; color: #FFFFFF; border-color: white} "
                            "QMainWindow,QDialog {background-color: black}");
    }
    else {
        qApp->setStyleSheet("");
    }
}

