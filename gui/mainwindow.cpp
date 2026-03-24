#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QCoreApplication>
#include <QFileInfo>

#include <array>
#include <cstdio>
#include <string>

static std::string ejecutarParser(const std::string& rutaArchivo)
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString parserPath = QDir(appDir).filePath("../rust_parser");
    parserPath = QFileInfo(parserPath).absoluteFilePath();

    QFileInfo info(parserPath);
    if (!info.exists()) {
        return "No se encontró el ejecutable en:\n" + parserPath.toStdString();
    }

    if (!info.isExecutable()) {
        return "El archivo existe pero no es ejecutable:\n" + parserPath.toStdString();
    }

    std::string comando = "\"" + parserPath.toStdString() + "\" \"" + rutaArchivo + "\" 2>&1";

    std::array<char, 256> buffer;
    std::string resultado;

    FILE* pipe = popen(comando.c_str(), "r");
    if (!pipe) {
        return "Error ejecutando el parser.";
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        resultado += buffer.data();
    }

    pclose(pipe);

    if (resultado.empty()) {
        return "No hubo salida del parser.";
    }

    return resultado;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    auto *layoutPrincipal = new QVBoxLayout;
    auto *layoutBotones = new QHBoxLayout;

    labelRuta = new QLabel("Ningún archivo seleccionado");
    btnSeleccionar = new QPushButton("Seleccionar archivo");
    btnAnalizar = new QPushButton("Analizar");
    editorSalida = new QTextEdit;

    editorSalida->setReadOnly(true);
    editorSalida->setPlaceholderText("Aquí aparecerá el resultado del análisis...");

    layoutBotones->addWidget(btnSeleccionar);
    layoutBotones->addWidget(btnAnalizar);

    layoutPrincipal->addWidget(labelRuta);
    layoutPrincipal->addLayout(layoutBotones);
    layoutPrincipal->addWidget(editorSalida);

    central->setLayout(layoutPrincipal);
    setCentralWidget(central);

    setWindowTitle("Parser Rust - Interfaz Gráfica");
    resize(700, 450);

    connect(btnSeleccionar, &QPushButton::clicked, this, [this]() {
        seleccionarArchivo();
    });

    connect(btnAnalizar, &QPushButton::clicked, this, [this]() {
        ejecutarAnalisis();
    });
}

void MainWindow::seleccionarArchivo()
{
    QString carpetaInicial = QDir(QCoreApplication::applicationDirPath()).filePath("../tests");
    carpetaInicial = QFileInfo(carpetaInicial).absoluteFilePath();

    QString archivo = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo",
        carpetaInicial,
        "Archivos de texto (*.txt);;Todos los archivos (*)"
    );

    if (archivo.isEmpty())
        return;

    rutaArchivo = archivo;
    labelRuta->setText("Archivo: " + rutaArchivo);
}

void MainWindow::ejecutarAnalisis()
{
    if (rutaArchivo.isEmpty()) {
        editorSalida->setPlainText("Primero debes seleccionar un archivo.");
        return;
    }

    std::string resultado = ejecutarParser(rutaArchivo.toStdString());
    editorSalida->setPlainText(QString::fromStdString(resultado));
}