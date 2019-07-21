#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalVariables.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>

using namespace std;
//--------------------------------------

#define sqr(x)	((x) * (x))

LetterStructure letters[20001];
LetterStructure testPattern;
LetterStructure test4000[4001];

int isNormalise = 0;
bool patternsLoadedFromFile;
int MAX_EPOCHS;
double LEARNING_RATE;
int STOP_RATE = 90;
int activation_function = RELU; //SIGMOID;  //0: relu 1:tanh 2:sigmoid(default)
double MSE;
double PGC;
double err2;
//int NUMBER_OF_PATTERNS;


///////////////////////////////////////////////////////

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ANN",
                                                                tr("Are you sure to exit?\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::No);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}  //ok

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //---------------------------------------
    //initialisation of global variables
    //

    //NUMBER_OF_PATTERNS = 20000;

    //LEARNING_RATE=0.2;
    LEARNING_RATE=0.0001;
    patternsLoadedFromFile = false;
    MAX_EPOCHS = 200;

    bp = new Backpropagation;

    //---------------------------------------
    //initialise widgets

    ui->spinBox_training_Epochs->setValue(MAX_EPOCHS);
    ui->horizScrollBar_LearningRate->setValue(int(LEARNING_RATE*1000000));
    ui->horizScrollBar_StopRate->setValue(STOP_RATE);
} //ok

MainWindow::~MainWindow()
{
    delete ui;
}  //ok

void MainWindow::on_checkBox_norm_stateChanged(int arg1)
{
    isNormalise = arg1;
    //calc max 15, min,0
    //int max=0, min =999;
    //for (int i=0 ; i< NUMBER_OF_PATTERNS; i++) {
    //    for (int j=0; j< INPUT_NEURONS; j++) {
    //       if (letters[i].f[j]>max)  max = letters[i].f[j];
    //       if (letters[i].f[j]<min) min = letters[i].f[j];
    //    }
    //}
    //ui->plainTextEdit_results->appendPlainText(QString::number(min));
    //ui->plainTextEdit_results->appendPlainText(QString::number(max));

} //ok

void MainWindow::on_horizScrollBar_StopRate_valueChanged(int value)
{
    ui->lcdNumber_StopRate->setSegmentStyle(QLCDNumber::Filled);
    ui->lcdNumber_StopRate->display(value);
    STOP_RATE = value;
}

void MainWindow::on_horizScrollBar_LearningRate_valueChanged(int value)   //------> change learning rate
{
    ui->lr_label->setText(QString::number(1.0*value/1000000.0, 'f'));
    LEARNING_RATE = value/1000000.0;
}  // -----> end of change rate, ok

void MainWindow::on_radioButton_relu_clicked()
{
    activation_function = RELU;
    ui->plainTextEdit_results->appendPlainText("Relu - Softmax algorithm");
    LEARNING_RATE=0.0001;
    ui->horizScrollBar_LearningRate->setValue(int(LEARNING_RATE*1000000));

}

void MainWindow::on_radioButton_tanh_clicked()
{
    activation_function = TANH;
    ui->plainTextEdit_results->appendPlainText("Tanh - Softmax algorithm");
    LEARNING_RATE=0.00001;
    ui->horizScrollBar_LearningRate->setValue(int(LEARNING_RATE*1000000));

}

void MainWindow::on_radioButton_sigmoid_clicked()
{
    activation_function =SIGMOID;
    ui->plainTextEdit_results->appendPlainText("Sigmoid - Softmax algorithm");
    LEARNING_RATE=0.0028;
    ui->horizScrollBar_LearningRate->setValue(int(LEARNING_RATE*1000000));

}

void MainWindow::on_pushButton_Shuffle_clicked()
{   // old version, need to change

    QString ss;
    //shuffling the whole dataset
    if(!patternsLoadedFromFile) {
        ss.clear();
        ss.append("\nMissing dataset.  Load data set first.\n");
        ui->plainTextEdit_results->appendPlainText(ss);
        return;
    }
    /*
    //letters
    LetterStructure tmp;
    int j;
    ui->plainTextEdit_results->appendPlainText("Shuffing dataset start!");
    for(int i=1; i < NUMBER_OF_PATTERNS; i++){
        if (letters[i].symbol==letters[i-1].symbol) { // 2 the same
            back:
            j = rand() % NUMBER_OF_PATTERNS;
            if (letters[j].symbol!=letters[i].symbol) { //
                ss.sprintf("Shuffing from %d to %d", i, j);
                ui->plainTextEdit_results->appendPlainText(ss);
                tmp = letters[i];
                letters[i] = letters[j];
                letters[j] = tmp;
            }
            else {
                goto back;
            }
        }
    }
    */
    //letters
    LetterStructure tmp;
    int i, j;
    ui->plainTextEdit_results->appendPlainText("Shuffing dataset start!");
    for(i=0; i < NUMBER_OF_TRAINING_PATTERNS; i++){
        back:
        j = rand() % NUMBER_OF_TRAINING_PATTERNS;
        if (letters[j].symbol!=letters[i].symbol) { //
             ss.sprintf("Shuffing from %d to %d", i, j);
             ui->plainTextEdit_results->appendPlainText(ss);
             tmp = letters[i];
             letters[i] = letters[j];
             letters[j] = tmp;
        }
        else
            goto back;
    }

    ui->plainTextEdit_results->appendPlainText("Shuffing dataset finished!");
} //works ok


void MainWindow::on_pushButton_Read_File_clicked()   // ----------->load from file complete_data_set.txt
{
    qDebug() << "\nReading file...";
    QFile file(QCoreApplication::applicationDirPath()+ui->plainTextEdit_ReadFile->toPlainText());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.exists()){
        patternsLoadedFromFile=false;

        qDebug() << "Data file does not exist!";
        return;
    }

    QTextStream in(&file);

    char t;
    char characterSymbol;
    QString line;

    int counterForLetterA=0;
    int counterForLetterB=0;
    int counterForLetterC=0;
    int counterForLetterD=0;
    int counterForLetterE=0;
    int counterForLetterF=0;
    int counterForLetterG=0;
    int counterForLetterH=0;
    int counterForLetterI=0;
    int counterForLetterJ=0;
    int counterForLetterK=0;
    int counterForLetterL=0;
    int counterForLetterM=0;
    int counterForLetterN=0;
    int counterForLetterO=0;
    int counterForLetterP=0;
    int counterForLetterQ=0;
    int counterForLetterR=0;
    int counterForLetterS=0;
    int counterForLetterT=0;
    int counterForLetterU=0;
    int counterForLetterV=0;
    int counterForLetterW=0;
    int counterForLetterX=0;
    int counterForLetterY=0;
    int counterForLetterZ=0;
    //int counterForUnknownLetters=0;

    QString lineOfData;
    QString msg;
    int i=0;
    int types=0;
    //while(i < NUMBER_OF_TRAINING_PATTERNS){
    while(i < NUMBER_OF_PATTERNS){

        //e.g. T,2,8,3,5,1,8,13,0,6,6,10,8,0,8,0,8
        in >> characterSymbol >> t >> letters[i].f[0] >> t >>  letters[i].f[1] >> t >>  letters[i].f[2] >> t >>  letters[i].f[3] >> t >>  letters[i].f[4] >> t >>  letters[i].f[5] >> t >>  letters[i].f[6] >> t >>  letters[i].f[7] >> t >>  letters[i].f[8] >> t >>  letters[i].f[9] >> t >>  letters[i].f[10] >> t >>  letters[i].f[11] >> t >> letters[i].f[12] >> t >> letters[i].f[13] >> t >> letters[i].f[14] >> t >> letters[i].f[15];
        line = in.readLine();

        if(characterSymbol == 'A'){
            letters[i].symbol = LETTER_A;
            types=0;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterA++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'B'){
            letters[i].symbol = LETTER_B;
            types=1;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterB++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'C'){
            letters[i].symbol = LETTER_C;
            types=2;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterC++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'D'){
            letters[i].symbol = LETTER_D;
            types=3;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterD++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'E'){
            letters[i].symbol = LETTER_E;
            types=4;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterE++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'F'){
            letters[i].symbol = LETTER_F;
            types=5;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterF++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'G'){
            letters[i].symbol = LETTER_G;
            types=6;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterG++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'H'){
            letters[i].symbol = LETTER_H;
            types=7;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterH++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'I'){
            letters[i].symbol = LETTER_I;
            types=8;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterI++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'J'){
            letters[i].symbol = LETTER_J;
            types=9;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterJ++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'K'){
            letters[i].symbol = LETTER_K;
            types=10;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterK++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'L'){
            letters[i].symbol = LETTER_L;
            types=11;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterL++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'M'){
            letters[i].symbol = LETTER_M;
            types=12;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterM++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'N'){
            letters[i].symbol = LETTER_N;
            types=13;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterN++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'O'){
            letters[i].symbol = LETTER_O;
            types=14;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterO++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'P'){
            letters[i].symbol = LETTER_P;
            types=15;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterP++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'Q'){
            letters[i].symbol = LETTER_Q;
            types=16;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterQ++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'R'){
            letters[i].symbol = LETTER_R;
            types=17;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterR++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'S'){
            letters[i].symbol = LETTER_S;
            types=18;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterS++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'T'){
            letters[i].symbol = LETTER_T;
            types=19;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterT++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'U'){
            letters[i].symbol = LETTER_U;
            types=20;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterU++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'V'){
            letters[i].symbol = LETTER_V;
            types=21;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterV++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'W'){
            letters[i].symbol = LETTER_W;
            types=22;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterW++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'X'){
            letters[i].symbol = LETTER_X;
            types=23;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterX++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'Y'){
            letters[i].symbol = LETTER_Y;
            types=24;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterY++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } else if(characterSymbol == 'Z'){
            letters[i].symbol = LETTER_Z;
            types=25;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    letters[i].outputs[j] = 0;
                else
                    letters[i].outputs[j] = 1;
            }
            counterForLetterZ++;
//            qDebug() << "Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        } //else {
          //  letters[i].symbol = UNKNOWN;
          //  types=26;
          //  for (int j=0; j<OUTPUT_NEURONS; j++) {
          //      if (j!=types)
          //          letters[i].outputs[j] = 0;
          //      else
          //          letters[i].outputs[j] = 1;
          //  }
          //  counterForUnknownLetters++;
//            qDebug() << "Unknown Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << letters[i].f[j];
//            }
        //}
        if((i % 50==0) || (i == NUMBER_OF_PATTERNS-1)){
            msg.clear();
            lineOfData.sprintf("number of patterns for Letter A = %d\n", counterForLetterA);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter B = %d\n", counterForLetterB);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter C = %d\n", counterForLetterC);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter D = %d\n", counterForLetterD);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter E = %d\n", counterForLetterE);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter F = %d\n", counterForLetterF);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter G = %d\n", counterForLetterG);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter H = %d\n", counterForLetterH);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter I = %d\n", counterForLetterI);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter J = %d\n", counterForLetterJ);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter K = %d\n", counterForLetterK);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter L = %d\n", counterForLetterL);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter M = %d\n", counterForLetterM);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter N = %d\n", counterForLetterN);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter O = %d\n", counterForLetterO);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter P = %d\n", counterForLetterP);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Q = %d\n", counterForLetterQ);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter R = %d\n", counterForLetterR);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter S = %d\n", counterForLetterS);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter T = %d\n", counterForLetterT);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter U = %d\n", counterForLetterU);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter V = %d\n", counterForLetterV);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter W = %d\n", counterForLetterW);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter X = %d\n", counterForLetterX);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Y = %d\n", counterForLetterY);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Z = %d\n", counterForLetterZ);
            msg.append(lineOfData);
            //lineOfData.sprintf("number of patterns for UNKNOWN letters = %d\n", counterForUnknownLetters);
            //msg.append(lineOfData);

            ui->plainTextEdit_results->setPlainText(msg);
            qApp->processEvents();
        }

        i++;
    }
    file.close();

    msg.append("done.");

    ui->plainTextEdit_results->appendPlainText(msg);
    qApp->processEvents();

    patternsLoadedFromFile=true;

} //------> end of load from file, ok

void MainWindow::on_pushButton_Classify_Test_Pattern_clicked()  // ----->push button ok
{
    char characterSymbol, t;
    QString *q;
    double* classificationResults;

    double* outputs;
    outputs = new double[OUTPUT_NEURONS];

//    delete q;
//    delete classificationResults;

    classificationResults = new double[OUTPUT_NEURONS];

    //QTextStream line;
    q = new QString(ui->plainTextEdit_Input_Pattern->toPlainText());

    QTextStream line(q);
    int res = 0;
    line >> characterSymbol >> t >> testPattern.f[0] >> t >>  testPattern.f[1] >> t >>  testPattern.f[2] >> t >>  testPattern.f[3] >> t >>  testPattern.f[4] >> t >>  testPattern.f[5] >> t >>  testPattern.f[6] >> t >>  testPattern.f[7] >> t >>  testPattern.f[8] >> t >>  testPattern.f[9] >> t >>  testPattern.f[10] >> t >>  testPattern.f[11] >> t >> testPattern.f[12] >> t >> testPattern.f[13] >> t >> testPattern.f[14] >> t >> testPattern.f[15];

    if(characterSymbol == 'A'){
        testPattern.symbol = LETTER_A;
        res = 0;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }

    } else if(characterSymbol == 'B'){
        testPattern.symbol = LETTER_B;
        res = 1;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'C'){
        testPattern.symbol = LETTER_C;
        res = 2;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'D'){
        testPattern.symbol = LETTER_D;
        res = 3;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'E'){
        testPattern.symbol = LETTER_E;
        res = 4;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'F'){
        testPattern.symbol = LETTER_F;
        res = 5;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'G'){
        testPattern.symbol = LETTER_G;
        res = 6;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'H'){
        testPattern.symbol = LETTER_H;
        res = 7;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'I'){
        testPattern.symbol = LETTER_I;
        res = 8;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'J'){
        testPattern.symbol = LETTER_J;
        res = 9;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'K'){
        testPattern.symbol = LETTER_K;
        res = 10;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'L'){
        testPattern.symbol = LETTER_L;
        res = 11;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'M'){
        testPattern.symbol = LETTER_M;
        res = 12;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'N'){
        testPattern.symbol = LETTER_N;
        res = 13;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'O'){
        testPattern.symbol = LETTER_O;
        res = 14;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'P'){
        testPattern.symbol = LETTER_P;
        res = 15;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'Q'){
        testPattern.symbol = LETTER_Q;
        res = 16;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'R'){
        testPattern.symbol = LETTER_R;
        res = 17;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'S'){
        testPattern.symbol = LETTER_S;
        res = 18;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'T'){
        testPattern.symbol = LETTER_T;
        res = 19;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'U'){
        testPattern.symbol = LETTER_U;
        res = 20;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'V'){
        testPattern.symbol = LETTER_V;
        res = 21;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'W'){
        testPattern.symbol = LETTER_W;
        res = 22;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'X'){
        testPattern.symbol = LETTER_X;
        res = 23;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'Y'){
        testPattern.symbol = LETTER_Y;
        res = 24;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } else if(characterSymbol == 'Z'){
        testPattern.symbol = LETTER_Z;
        res = 25;
        for (int k=0; k<OUTPUT_NEURONS; k++) {
            if (k == res)
                testPattern.outputs[k] = 1;
            else
                testPattern.outputs[k] = 0;
         }
    } //else {
      //  testPattern.symbol = UNKNOWN;
      //  res = 26;
      //  for (int k=0; k<OUTPUT_NEURONS; k++) {
      //      if (k == res)
      //          testPattern.outputs[k] = 1;
      //      else
      //          testPattern.outputs[k] = 0;
      //   }
    //}


    //---------------------------------
    classificationResults = bp->testNetwork(testPattern);

    ui->lcdNumber_A->display(classificationResults[0]);
    ui->lcdNumber_B->display(classificationResults[1]);
    ui->lcdNumber_C->display(classificationResults[2]);
    ui->lcdNumber_D->display(classificationResults[3]);
    ui->lcdNumber_E->display(classificationResults[4]);
    ui->lcdNumber_F->display(classificationResults[5]);
    ui->lcdNumber_G->display(classificationResults[6]);
    ui->lcdNumber_H->display(classificationResults[7]);
    ui->lcdNumber_I->display(classificationResults[8]);
    ui->lcdNumber_J->display(classificationResults[9]);
    ui->lcdNumber_K->display(classificationResults[10]);
    ui->lcdNumber_L->display(classificationResults[11]);
    ui->lcdNumber_M->display(classificationResults[12]);
    ui->lcdNumber_N->display(classificationResults[13]);
    ui->lcdNumber_O->display(classificationResults[14]);
    ui->lcdNumber_P->display(classificationResults[15]);
    ui->lcdNumber_Q->display(classificationResults[16]);
    ui->lcdNumber_R->display(classificationResults[17]);
    ui->lcdNumber_S->display(classificationResults[18]);
    ui->lcdNumber_T->display(classificationResults[19]);
    ui->lcdNumber_U->display(classificationResults[20]);
    ui->lcdNumber_V->display(classificationResults[21]);
    ui->lcdNumber_W->display(classificationResults[22]);
    ui->lcdNumber_X->display(classificationResults[23]);
    ui->lcdNumber_Y->display(classificationResults[24]);
    ui->lcdNumber_Z->display(classificationResults[25]);
    //ui->lcdNumber_unknown->display(classificationResults[26]);

    //-----------------------------------------------------------
    for(int k=0; k < OUTPUT_NEURONS; k++){
       outputs[k] = testPattern.outputs[k];
    }
    //-----------------------------------------------------------
     QString textClassification;
     switch(bp->action(outputs)){
        case 0:
            textClassification = "letter A";
            break;
        case 1:
            textClassification = "letter B";
            break;
        case 2:
            textClassification = "letter C";
            break;
        case 3:
            textClassification = "letter D";
            break;
        case 4:
            textClassification = "letter E";
            break;
        case 5:
            textClassification = "letter F";
            break;
        case 6:
            textClassification = "letter G";
            break;
        case 7:
            textClassification = "letter H";
            break;
        case 8:
            textClassification = "letter I";
            break;
        case 9:
            textClassification = "letter J";
            break;
        case 10:
            textClassification = "letter K";
            break;
        case 11:
            textClassification = "letter L";
            break;
        case 12:
            textClassification = "letter M";
            break;
        case 13:
            textClassification = "letter N";
            break;
        case 14:
            textClassification = "letter O";
            break;
        case 15:
            textClassification = "letter P";
            break;
        case 16:
            textClassification = "letter Q";
            break;
        case 17:
            textClassification = "letter R";
            break;
        case 18:
            textClassification = "letter S";
            break;
        case 19:
            textClassification = "letter T";
            break;
        case 20:
            textClassification = "letter U";
            break;
        case 21:
            textClassification = "letter V";
            break;
        case 22:
            textClassification = "letter W";
            break;
        case 23:
            textClassification = "letter X";
            break;
        case 24:
            textClassification = "letter Y";
            break;
        case 25:
            textClassification = "letter Z";
            break;
        //case 26:
        //    textClassification = "unknown";
        //    break;
     };

    if (bp->action(classificationResults) == bp->action(outputs)) {
        qDebug() << "correct classification.";
        ui->label_Classification->setText(textClassification + ", - Correct classification!");
    } else {
        qDebug() << "incorrect classification.";
        ui->label_Classification->setText(textClassification + ", -XXX- Incorrect classification.");
    }

}  //-----> Push button classify test pattern, OK


void MainWindow::on_pushButton_Train_Network_Max_Epochs_clicked()   //-------->start to training
{

    //Performance measure:
    //Mean Squared Error (MSE)
    //Percentage of Good Classification (PGC)
    //Confusion Matrix (only for the best NN found)

    double SSE = 0.0;
    double highest_pgc = 0.0;
    int highest_iter=0;
    MSE = 0.0;
    QString msg;

    if(!patternsLoadedFromFile) {
        msg.clear();
        msg.append("\nMissing training patterns.  Load data set first.\n");
        ui->plainTextEdit_results->appendPlainText(msg);
        return;
    }

    MAX_EPOCHS = ui->spinBox_training_Epochs->value();
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QFile file_result1(QCoreApplication::applicationDirPath()+ui->plainTextEdit_evafile->toPlainText());
    file_result1.open(QIODevice::WriteOnly | QIODevice::Text); // | QIODevice::Append);

    QTextStream out_result1(&file_result1);

    char tmp[180];
    QByteArray temp_result1;

    for(int i=1; i <= MAX_EPOCHS; i++){
      msg.clear();
      msg.append("\nTraining in progress...\n");
      msg.append("\nEpoch=");
      msg.append(QString::number(i));
      ui->plainTextEdit_results->appendPlainText(msg);
      qApp->processEvents();
      qDebug() << "start an iteration";
      //SSE and MSE
      SSE = bp->trainNetwork(1);
      MSE = -2.0 * SSE / NUMBER_OF_TRAINING_PATTERNS;
      double MSE_back = MSE;
      //calc PGC train set
      on_pushButton_testNetOnTrainingSet_clicked();
      double MSE_TRAIN, PGC_TRAIN;
      MSE_TRAIN = MSE / NUMBER_OF_TRAINING_PATTERNS;
      PGC_TRAIN = PGC;
      // show result
      ui->lcdNumber_SSE->display(bp->getError_SSE());
      ui->lcdNumber_MSE->display(MSE_back);
      ui->lcdNumber_percentageOfGoodClassification->display(PGC);
      //Test set
      on_pushButton_Test_All_Patterns_clicked();
      MSE = MSE / NUMBER_OF_TEST_PATTERNS;
      if (PGC > highest_pgc) {
          highest_pgc = PGC;
          highest_iter = i;
      }
      update();
      qApp->processEvents();

      qDebug() << "epoch: " << i << ", SSE = " << bp->getError_SSE() << ", MSE = " << MSE <<"PGC = " <<PGC;
      temp_result1.clear();
      //sprintf(tmp,"Epoch: %d, SSE = %f, GetError = %f, MSE from function = %f, Training set MSE = %f, Training set PGC = %f", i,  bp->getError_SSE(), SSE, MSE_back, MSE, PGC);
      sprintf(tmp,"%d, %f, %f, %f, %f, %f", i,  bp->getError_SSE(), MSE_TRAIN, PGC_TRAIN, MSE, PGC);
      temp_result1.append(tmp);
      temp_result1.append("\n");
      out_result1 << temp_result1;

      if(i % 10 ==0){
         bp->saveWeights(ui->plainTextEdit_saveWeightsAs->toPlainText());

         ui->plainTextEdit_results->appendPlainText("Weights saved into file.");
         qApp->processEvents();
      }
      //end the loop if condition is met, 90% of successful result on validation set
      if (PGC >= STOP_RATE) {
          bp->saveWeights(ui->plainTextEdit_saveWeightsAs->toPlainText());

          ui->plainTextEdit_results->appendPlainText("Successfully trained the network, as PGC reaches to target!");
          qApp->processEvents();

          break;

      }
      //

    }
    qDebug() << "Highest PGC = " << highest_pgc << " at iteration " << highest_iter;
    file_result1.close();

    QApplication::restoreOverrideCursor();

} //----->training function, maybe ok, test later

void MainWindow::on_pushButton_Initialise_Network_clicked()   // ------>  backpropagation initialise()
{
    bp->initialise();
    ui->plainTextEdit_results->appendPlainText("Network initalization done.");
}  //init ok

void MainWindow::on_pushButton_Test_All_Patterns_clicked()  //-----> test on test set
{
    char characterSymbol; //, t;
    // QString *q;
    double* classificationResults;
    double* outputs;
    int correctClassifications=0;
    int des = 0;
    classificationResults = new double[OUTPUT_NEURONS];
    outputs = new double[OUTPUT_NEURONS];
    MSE = 0.0;
    //init confusuin matrix report

    int Confusion_Matrix[OUTPUT_NEURONS][OUTPUT_NEURONS];
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        for (int j = 0; j < OUTPUT_NEURONS; j++) {
            Confusion_Matrix[i][j] = 0;
        }
    }

    for(int i=NUMBER_OF_TRAINING_PATTERNS; i < NUMBER_OF_PATTERNS; i++){

            characterSymbol = letters[i].symbol;

            for(int j=0; j < INPUT_NEURONS; j++){
                testPattern.f[j] = letters[i].f[j];
            }

            if(characterSymbol == LETTER_A){
                testPattern.symbol = LETTER_A;
                des = 0;
                for (int j=0; j<OUTPUT_NEURONS; j++) {
                    if (j==des)
                        testPattern.outputs[j] = 1;
                    else
                        testPattern.outputs[j] = 0;
                }

            } else if(characterSymbol == LETTER_B){
                testPattern.symbol = LETTER_B;
                des = 1;
                for (int j=0; j<OUTPUT_NEURONS; j++) {
                    if (j==des)
                        testPattern.outputs[j] = 1;
                    else
                        testPattern.outputs[j] = 0;
                }

            } else if(characterSymbol == LETTER_C){
              testPattern.symbol = LETTER_C;
              des = 2;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_D){
              testPattern.symbol = LETTER_D;
              des = 3;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_E){
              testPattern.symbol = LETTER_E;
              des = 4;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_F){
              testPattern.symbol = LETTER_F;
              des = 5;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_G){
              testPattern.symbol = LETTER_G;
              des = 6;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_H){
              testPattern.symbol = LETTER_H;
              des = 7;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_I){
              testPattern.symbol = LETTER_I;
              des = 8;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_J){
              testPattern.symbol = LETTER_J;
              des = 9;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_K){
              testPattern.symbol = LETTER_K;
              des = 10;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_L){
              testPattern.symbol = LETTER_L;
              des = 11;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_M){
              testPattern.symbol = LETTER_M;
              des = 12;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_N){
              testPattern.symbol = LETTER_N;
              des = 13;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_O){
              testPattern.symbol = LETTER_O;
              des = 14;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_P){
              testPattern.symbol = LETTER_P;
              des = 15;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_Q){
              testPattern.symbol = LETTER_Q;
              des = 16;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_R){
              testPattern.symbol = LETTER_R;
              des = 17;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_S){
              testPattern.symbol = LETTER_S;
              des = 18;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_T){
              testPattern.symbol = LETTER_T;
              des = 19;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_U){
              testPattern.symbol = LETTER_U;
              des = 20;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_V){
              testPattern.symbol = LETTER_V;
              des = 21;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_W){
              testPattern.symbol = LETTER_W;
              des = 22;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_X){
              testPattern.symbol = LETTER_X;
              des = 23;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_Y){
              testPattern.symbol = LETTER_Y;
              des = 24;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            } else if(characterSymbol == LETTER_Z){
              testPattern.symbol = LETTER_Z;
              des = 25;
              for (int j=0; j<OUTPUT_NEURONS; j++) {
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;
              }

            }// else {
             //   testPattern.symbol = UNKNOWN;
             //   des = 26;
             //   for (int j=0; j<OUTPUT_NEURONS; j++) {
             //       if (j==des)
             //           testPattern.outputs[j] = 1;
             //      else
             //           testPattern.outputs[j] = 0;
            //      }
            //}

            //---------------------------------
            classificationResults = bp->testNetwork(testPattern);

            for(int k=0; k < OUTPUT_NEURONS; k++){
               outputs[k] = testPattern.outputs[k];
               MSE += sqr(classificationResults[k]-outputs[k]);
            }

            int predict_outputs = bp->action(classificationResults);
            int target_outputs = bp->action(outputs);
            if (predict_outputs == target_outputs) {
                 correctClassifications++;
            }
            Confusion_Matrix[target_outputs][predict_outputs] ++;

     }

    // we have correctClassification number, and the number of test patterns, being NUMBER_OF_PATTERNS - NUMBER OF TRAINING PATTERNS
    PGC = 100.0 * correctClassifications / ( NUMBER_OF_PATTERNS - NUMBER_OF_TRAINING_PATTERNS);
    QString msg2;
    msg2.sprintf("%.2f", PGC);
    ui->lcdNumber_percentageOfGoodClassification->display(msg2);
    ui->lcdNumber_MSE->display(1.0 * MSE/( NUMBER_OF_PATTERNS - NUMBER_OF_TRAINING_PATTERNS) );
    ui->plainTextEdit_results->appendPlainText("Test network on testing set done!");
    qDebug() << "TEST SET: correctClassifications = " << correctClassifications <<"PGC = " << PGC;
    qApp->processEvents();

    //write result to file

    QFile file_cm("./confusion_matrix_test.txt");
    file_cm.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file_cm);
    char tempBuffer[80];
    QByteArray temp;
    out <<temp;
    for(int i = 0;i<OUTPUT_NEURONS;i++){
        temp.clear();
        for(int j = 0;j<OUTPUT_NEURONS;j++){
            sprintf(tempBuffer, "%d", Confusion_Matrix[i][j]);
            temp.append(tempBuffer);
            if (j != (OUTPUT_NEURONS-1))
                temp.append(",");
        }
        //temp.append(tempBuffer);
        temp.append("\n");
        out << temp;
    }
    file_cm.close();

} // test all pattern ok, but need calculate % and mse

void MainWindow::on_pushButton_Save_Weights_clicked()  //----> save weight
{
    bp->saveWeights(QCoreApplication::applicationDirPath()+ui->plainTextEdit_saveWeightsAs->toPlainText());

    QString msg;
    QString lineOfText;

    lineOfText = "weights saved to file: " + ui->plainTextEdit_saveWeightsAs->toPlainText();

    msg.append(lineOfText);

    ui->plainTextEdit_results->appendPlainText(msg);

}  //---> save weight ok

void MainWindow::on_pushButton_Load_Weights_clicked()  //------> load weight
{
   bp->loadWeights(QCoreApplication::applicationDirPath()+ui->plainTextEdit_fileNameLoadWeights->toPlainText());

   QString msg;

   msg.append("weights loaded.\n");

   ui->plainTextEdit_results->appendPlainText(msg);


}  // load weight, ok

void MainWindow::on_pushButton_testNetOnTrainingSet_clicked()  //----->test on training set 0-16000 ?
{
    char characterSymbol; //, t;
    //QString *q;
    double* classificationResults;
    double* outputs;
    int correctClassifications=0;
    int des = 0;
    classificationResults = new double[OUTPUT_NEURONS];
    outputs = new double[OUTPUT_NEURONS];

    //init confusion matrix report
    MSE = 0.0;

    int Confusion_Matrix[OUTPUT_NEURONS][OUTPUT_NEURONS];
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        for (int j = 0; j < OUTPUT_NEURONS; j++) {
            Confusion_Matrix[i][j] = 0;
        }
    }

    for(int i=0; i < NUMBER_OF_TRAINING_PATTERNS; i++){

            characterSymbol = letters[i].symbol;

            for(int j=0; j < INPUT_NEURONS; j++){
                testPattern.f[j] = letters[i].f[j];
            }

            if(characterSymbol == LETTER_A){
                testPattern.symbol = LETTER_A;
                des = 0;
                for (int j=0; j<OUTPUT_NEURONS; j++)
                    if (j==des)
                        testPattern.outputs[j] = 1;
                    else
                        testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_B){
                testPattern.symbol = LETTER_B;
                des = 1;
                for (int j=0; j<OUTPUT_NEURONS; j++)
                    if (j==des)
                        testPattern.outputs[j] = 1;
                    else
                        testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_C){
              testPattern.symbol = LETTER_C;
              des = 2;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_D){
              testPattern.symbol = LETTER_D;
              des = 3;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_E){
              testPattern.symbol = LETTER_E;
              des = 4;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_F){
              testPattern.symbol = LETTER_F;
              des = 5;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_G){
              testPattern.symbol = LETTER_G;
              des = 6;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_H){
              testPattern.symbol = LETTER_H;
              des = 7;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_I){
              testPattern.symbol = LETTER_I;
              des = 8;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_J){
              testPattern.symbol = LETTER_J;
              des = 9;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_K){
              testPattern.symbol = LETTER_K;
              des = 10;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_L){
              testPattern.symbol = LETTER_L;
              des = 11;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_M){
              testPattern.symbol = LETTER_M;
              des = 12;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_N){
              testPattern.symbol = LETTER_N;
              des = 13;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_O){
              testPattern.symbol = LETTER_O;
              des = 14;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_P){
              testPattern.symbol = LETTER_P;
              des = 15;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_Q){
              testPattern.symbol = LETTER_Q;
              des = 16;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_R){
              testPattern.symbol = LETTER_R;
              des = 17;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_S){
              testPattern.symbol = LETTER_S;
              des = 18;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_T){
              testPattern.symbol = LETTER_T;
              des = 19;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_U){
              testPattern.symbol = LETTER_U;
              des = 20;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_V){
              testPattern.symbol = LETTER_V;
              des = 21;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_W){
              testPattern.symbol = LETTER_W;
              des = 22;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_X){
              testPattern.symbol = LETTER_X;
              des = 23;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_Y){
              testPattern.symbol = LETTER_Y;
              des = 24;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_Z){
              testPattern.symbol = LETTER_Z;
              des = 25;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } //else {
              //  testPattern.symbol = UNKNOWN;
              //  des = 26;
              //  for (int j=0; j<OUTPUT_NEURONS; j++)
              //      if (j==des)
              //          testPattern.outputs[j] = 1;
              //      else
              //          testPattern.outputs[j] = 0;

            //}

            //---------------------------------
            classificationResults = bp->testNetwork(testPattern);

            for(int k=0; k < OUTPUT_NEURONS; k++){
               outputs[k] = testPattern.outputs[k];
               MSE += sqr(classificationResults[k]-outputs[k]);

            }

            int predict_outputs = bp->action(classificationResults);
            int target_outputs = bp->action(outputs);

            if (predict_outputs == target_outputs) {
                 correctClassifications++;
            }
            Confusion_Matrix[target_outputs][predict_outputs] ++;

      }

    // correctClassifications and NUMBER_OF_TRAINING_PATTERNS %
    // we have correctClassification number, and the number of test patterns, being NUMBER_OF_PATTERNS - NUMBER OF TRAINING PATTERNS
    PGC = 100.0 * correctClassifications / NUMBER_OF_TRAINING_PATTERNS;
    QString msg2;
    msg2.sprintf("%.2f", PGC);
    ui->lcdNumber_percentageOfGoodClassification->display(msg2);
    ui->lcdNumber_MSE->display(1.0 * MSE/NUMBER_OF_TRAINING_PATTERNS);

    ui->plainTextEdit_results->appendPlainText("Test network on training set done!");
    qDebug() << "TRAINING SET: correctClassifications = " << correctClassifications << "PGC = " << PGC;

    qApp->processEvents();

    //write result to file

    QFile file_cm("./confusion_matrix_train.txt");
    file_cm.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file_cm);
    char tempBuffer[80];
    QByteArray temp;
    out <<temp;
    for(int i = 0;i<OUTPUT_NEURONS;i++){
        temp.clear();
        for(int j = 0;j<OUTPUT_NEURONS;j++){
            sprintf(tempBuffer, "%d", Confusion_Matrix[i][j]);
            temp.append(tempBuffer);
            if (j != (OUTPUT_NEURONS-1))
                temp.append(",");
        }
        //temp.append(tempBuffer);
        temp.append("\n");
        out << temp;
    }
    file_cm.close();


}  //test on training set, ok


void MainWindow::on_pushButton_TestFromFile_clicked()  //---> new button
{
    // open file and predict similar to test on training set
    qDebug() << "\nReading file...";
    QFile file(QCoreApplication::applicationDirPath()+ui->plainTextEdit_TestFromFile->toPlainText());

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.exists()){

        qDebug() << "Data file does not exist!";
        return;
    }

    QTextStream in(&file);

    char t;
    char characterSymbol;
    QString line;

    int counterForLetterA=0;
    int counterForLetterB=0;
    int counterForLetterC=0;
    int counterForLetterD=0;
    int counterForLetterE=0;
    int counterForLetterF=0;
    int counterForLetterG=0;
    int counterForLetterH=0;
    int counterForLetterI=0;
    int counterForLetterJ=0;
    int counterForLetterK=0;
    int counterForLetterL=0;
    int counterForLetterM=0;
    int counterForLetterN=0;
    int counterForLetterO=0;
    int counterForLetterP=0;
    int counterForLetterQ=0;
    int counterForLetterR=0;
    int counterForLetterS=0;
    int counterForLetterT=0;
    int counterForLetterU=0;
    int counterForLetterV=0;
    int counterForLetterW=0;
    int counterForLetterX=0;
    int counterForLetterY=0;
    int counterForLetterZ=0;
    //int counterForUnknownLetters=0;

    QString lineOfData;
    QString msg;
    int i=0;
    int types=0;
    //while(i < NUMBER_OF_TRAINING_PATTERNS){
    while(i < NUMBER_OF_TEST_PATTERNS){

        //e.g. T,2,8,3,5,1,8,13,0,6,6,10,8,0,8,0,8
        in >> characterSymbol >> t >> test4000[i].f[0] >> t >>  test4000[i].f[1] >> t >>  test4000[i].f[2] >> t >>  test4000[i].f[3] >> t >>  test4000[i].f[4] >> t >>  test4000[i].f[5] >> t >>  test4000[i].f[6] >> t >>  test4000[i].f[7] >> t >>  test4000[i].f[8] >> t >>  test4000[i].f[9] >> t >>  test4000[i].f[10] >> t >>  test4000[i].f[11] >> t >> test4000[i].f[12] >> t >> test4000[i].f[13] >> t >> test4000[i].f[14] >> t >> test4000[i].f[15];
        line = in.readLine();

        if(characterSymbol == 'A'){
            test4000[i].symbol = LETTER_A;
            types=0;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterA++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'B'){
            test4000[i].symbol = LETTER_B;
            types=1;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterB++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'C'){
            test4000[i].symbol = LETTER_C;
            types=2;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterC++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'D'){
            test4000[i].symbol = LETTER_D;
            types=3;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterD++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'E'){
            test4000[i].symbol = LETTER_E;
            types=4;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterE++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'F'){
            test4000[i].symbol = LETTER_F;
            types=5;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterF++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'G'){
            test4000[i].symbol = LETTER_G;
            types=6;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterG++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'H'){
            test4000[i].symbol = LETTER_H;
            types=7;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterH++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'I'){
            test4000[i].symbol = LETTER_I;
            types=8;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterI++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'J'){
            test4000[i].symbol = LETTER_J;
            types=9;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterJ++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'K'){
            test4000[i].symbol = LETTER_K;
            types=10;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterK++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'L'){
            test4000[i].symbol = LETTER_L;
            types=11;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterL++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'M'){
            test4000[i].symbol = LETTER_M;
            types=12;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterM++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'N'){
            test4000[i].symbol = LETTER_N;
            types=13;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterN++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'O'){
            test4000[i].symbol = LETTER_O;
            types=14;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterO++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'P'){
            test4000[i].symbol = LETTER_P;
            types=15;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterP++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'Q'){
            test4000[i].symbol = LETTER_Q;
            types=16;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterQ++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'R'){
            test4000[i].symbol = LETTER_R;
            types=17;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterR++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'S'){
            test4000[i].symbol = LETTER_S;
            types=18;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterS++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'T'){
            test4000[i].symbol = LETTER_T;
            types=19;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterT++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'U'){
            test4000[i].symbol = LETTER_U;
            types=20;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterU++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'V'){
            test4000[i].symbol = LETTER_V;
            types=21;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterV++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'W'){
            test4000[i].symbol = LETTER_W;
            types=22;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterW++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'X'){
            test4000[i].symbol = LETTER_X;
            types=23;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterX++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'Y'){
            test4000[i].symbol = LETTER_Y;
            types=24;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterY++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } else if(characterSymbol == 'Z'){
            test4000[i].symbol = LETTER_Z;
            types=25;
            for (int j=0; j<OUTPUT_NEURONS; j++) {
                if (j!=types)
                    test4000[i].outputs[j] = 0;
                else
                    test4000[i].outputs[j] = 1;
            }
            counterForLetterZ++;
//            qDebug() << "test4000[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        } //else {
          //  test4000[i].symbol = UNKNOWN;
          //  types=26;
          //  for (int j=0; j<OUTPUT_NEURONS; j++) {
          //      if (j!=types)
          //          test4000[i].outputs[j] = 0;
          //      else
          //          test4000[i].outputs[j] = 1;
          //  }
          //  counterForUnknownLetters++;
//            qDebug() << "Unknown Letter[" << i << "] = " << characterSymbol;
//            for(int j=0; j < 16; j++){
//                qDebug() << "f[" << j << "] = " << test4000[i].f[j];
//            }
        //}
        if((i % 50==0) || (i == NUMBER_OF_PATTERNS-1)){
            msg.clear();
            lineOfData.sprintf("number of patterns for Letter A = %d\n", counterForLetterA);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter B = %d\n", counterForLetterB);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter C = %d\n", counterForLetterC);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter D = %d\n", counterForLetterD);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter E = %d\n", counterForLetterE);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter F = %d\n", counterForLetterF);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter G = %d\n", counterForLetterG);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter H = %d\n", counterForLetterH);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter I = %d\n", counterForLetterI);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter J = %d\n", counterForLetterJ);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter K = %d\n", counterForLetterK);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter L = %d\n", counterForLetterL);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter M = %d\n", counterForLetterM);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter N = %d\n", counterForLetterN);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter O = %d\n", counterForLetterO);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter P = %d\n", counterForLetterP);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Q = %d\n", counterForLetterQ);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter R = %d\n", counterForLetterR);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter S = %d\n", counterForLetterS);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter T = %d\n", counterForLetterT);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter U = %d\n", counterForLetterU);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter V = %d\n", counterForLetterV);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter W = %d\n", counterForLetterW);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter X = %d\n", counterForLetterX);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Y = %d\n", counterForLetterY);
            msg.append(lineOfData);
            lineOfData.sprintf("number of patterns for Letter Z = %d\n", counterForLetterZ);
            msg.append(lineOfData);
            //lineOfData.sprintf("number of patterns for UNKNOWN letters = %d\n", counterForUnknownLetters);
            //msg.append(lineOfData);

            ui->plainTextEdit_results->setPlainText(msg);
            qApp->processEvents();
        }

        i++;
    }
    file.close();

    msg.append("done.");

    ui->plainTextEdit_results->appendPlainText(msg);
    qApp->processEvents();

    //start to test and create report

    int Confusion_Matrix[OUTPUT_NEURONS][OUTPUT_NEURONS];
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        for (int j = 0; j < OUTPUT_NEURONS; j++) {
            Confusion_Matrix[i][j] = 0;
        }
    }

    //char characterSymbol; //, t;
    //Symbol characterSymbol1;
    //QString *q;
    double* classificationResults;
    double* outputs;
    int correctClassifications=0;
    int des = 0;
    classificationResults = new double[OUTPUT_NEURONS];
    outputs = new double[OUTPUT_NEURONS];

    MSE = 0.0;

    for(int i=0; i < NUMBER_OF_TEST_PATTERNS; i++){

            characterSymbol = test4000[i].symbol;
            //characterSymbol1 = test4000[i].symbol;

            for(int j=0; j < INPUT_NEURONS; j++){
                testPattern.f[j] = test4000[i].f[j];
            }

            if(characterSymbol == LETTER_A){
                testPattern.symbol = LETTER_A;
                des = 0;
                for (int j=0; j<OUTPUT_NEURONS; j++)
                    if (j==des)
                        testPattern.outputs[j] = 1;
                    else
                        testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_B){
                testPattern.symbol = LETTER_B;
                des = 1;
                for (int j=0; j<OUTPUT_NEURONS; j++)
                    if (j==des)
                        testPattern.outputs[j] = 1;
                    else
                        testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_C){
              testPattern.symbol = LETTER_C;
              des = 2;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_D){
              testPattern.symbol = LETTER_D;
              des = 3;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_E){
              testPattern.symbol = LETTER_E;
              des = 4;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_F){
              testPattern.symbol = LETTER_F;
              des = 5;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_G){
              testPattern.symbol = LETTER_G;
              des = 6;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_H){
              testPattern.symbol = LETTER_H;
              des = 7;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_I){
              testPattern.symbol = LETTER_I;
              des = 8;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_J){
              testPattern.symbol = LETTER_J;
              des = 9;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_K){
              testPattern.symbol = LETTER_K;
              des = 10;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_L){
              testPattern.symbol = LETTER_L;
              des = 11;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_M){
              testPattern.symbol = LETTER_M;
              des = 12;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_N){
              testPattern.symbol = LETTER_N;
              des = 13;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_O){
              testPattern.symbol = LETTER_O;
              des = 14;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_P){
              testPattern.symbol = LETTER_P;
              des = 15;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_Q){
              testPattern.symbol = LETTER_Q;
              des = 16;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_R){
              testPattern.symbol = LETTER_R;
              des = 17;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_S){
              testPattern.symbol = LETTER_S;
              des = 18;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_T){
              testPattern.symbol = LETTER_T;
              des = 19;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_U){
              testPattern.symbol = LETTER_U;
              des = 20;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_V){
              testPattern.symbol = LETTER_V;
              des = 21;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_W){
              testPattern.symbol = LETTER_W;
              des = 22;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_X){
              testPattern.symbol = LETTER_X;
              des = 23;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_Y){
              testPattern.symbol = LETTER_Y;
              des = 24;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } else if(characterSymbol == LETTER_Z){
              testPattern.symbol = LETTER_Z;
              des = 25;
              for (int j=0; j<OUTPUT_NEURONS; j++)
                  if (j==des)
                      testPattern.outputs[j] = 1;
                  else
                      testPattern.outputs[j] = 0;

            } //else {
              //  testPattern.symbol = UNKNOWN;
              //  des = 26;
              //  for (int j=0; j<OUTPUT_NEURONS; j++)
              //      if (j==des)
              //          testPattern.outputs[j] = 1;
              //      else
              //          testPattern.outputs[j] = 0;

            //}

            //---------------------------------
            classificationResults = bp->testNetwork(testPattern);

            for(int k=0; k < OUTPUT_NEURONS; k++){
               outputs[k] = testPattern.outputs[k];
               MSE += sqr(classificationResults[k]-outputs[k]);

            }

            int predict_outputs = bp->action(classificationResults);
            int target_outputs = bp->action(outputs);

            if (predict_outputs == target_outputs) {
                 correctClassifications++;
            }
            //qDebug() << characterSymbol1 <<"=" <<predict_outputs <<"=="<<target_outputs;
            Confusion_Matrix[target_outputs][predict_outputs] ++;

      }

    // correctClassifications and NUMBER_OF_TRAINING_PATTERNS %
    // we have correctClassification number, and the number of test patterns, being NUMBER_OF_PATTERNS - NUMBER OF TRAINING PATTERNS
    PGC = 100.0 * correctClassifications / NUMBER_OF_TEST_PATTERNS;
    ui->lcdNumber_MSE->display(1.0* MSE / NUMBER_OF_TEST_PATTERNS);
    ui->lcdNumber_percentageOfGoodClassification->display(PGC);

    QString msg2;
    msg2.sprintf("MSE = %.2f  PGC = %.2f", 1.0 * MSE / NUMBER_OF_TEST_PATTERNS, PGC);
    ui->plainTextEdit_results->appendPlainText(msg2);
    ui->plainTextEdit_results->appendPlainText("Test network on file done!");
    qDebug() << "TESTING FILE SET: correctClassifications = " << correctClassifications <<"PGC = " << PGC;
    qApp->processEvents();

    //write result to file

    QFile file_cm("./confusion_matrix.txt");
    file_cm.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file_cm);
    char tempBuffer[80];
    QByteArray temp;
    out <<temp;
    for(int i = 0;i<OUTPUT_NEURONS;i++){
        temp.clear();
        for(int j = 0;j<OUTPUT_NEURONS;j++){
            sprintf(tempBuffer, "%d", Confusion_Matrix[i][j]);
            temp.append(tempBuffer);
            if (j != (OUTPUT_NEURONS-1))
                temp.append(",");
        }
        //temp.append(tempBuffer);
        temp.append("\n");
        out << temp;
    }
    file_cm.close();

}  //need to write it to txt file


void MainWindow::on_pushButton_loadweightfile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open weights file"), "/", tr("Weights Files (*.txt)"));
    if (!fileName.isEmpty()) {
        //load
        bp->loadWeights(fileName);
    }
}
