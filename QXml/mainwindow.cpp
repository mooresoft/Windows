#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QXmlStreamWriter>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->writeButton, SIGNAL(clicked()), this, SLOT(on_actionSave_triggered()));
    connect(ui->readButton, SIGNAL(clicked()), this, SLOT(on_actionLoad_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered() {
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Xml"), ".",
        tr("Xml files (*.xml)"));
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Parameters");
    xmlWriter.writeTextElement("para1",QString::number(1));
    xmlWriter.writeTextElement("para2",QString::number(2));
    xmlWriter.writeTextElement("para3",QString::number(3));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
}

void MainWindow::on_actionLoad_triggered() {
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Xml"), ".",
        tr("Xml files (*.xml)"));
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(filename)
            << ": " << qPrintable(file.errorString())
            << std::endl;
    }
    QXmlStreamReader xmlReader(&file);
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if(xmlReader.isStartElement()) {
            if(xmlReader.name() == "Parameters") {
                xmlReader.readNext();
            } else if (xmlReader.name() == "para1") {
                int para1 = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            } else if (xmlReader.name() == "preFilterCap") {
                int para2 = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            } else if (xmlReader.name() == "SADWindowSize") {
                int para3 = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            } else {
                xmlReader.raiseError(QObject::tr("Not a options file"));
            }
        } else {
            xmlReader.readNext();
        }
    }

    file.close();
    if (xmlReader.hasError()) {
        std::cerr << "Error: Failed to parse file "
                  << qPrintable(filename) << ": "
                  << qPrintable(xmlReader.errorString()) << std::endl;
    } else if (file.error() != QFile::NoError) {
        std::cerr << "Error: Cannot read file " << qPrintable(filename)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
    }
}
