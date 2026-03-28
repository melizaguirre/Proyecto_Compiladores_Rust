#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QCoreApplication>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>
#include <QRegularExpression>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QColor>
#include <QTextFormat>
#include <QTextBlock>

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
        return "Error ejecutando rust_parser.";
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
    auto *layoutCodigo = new QHBoxLayout;

    labelRuta = new QLabel("Ningún archivo seleccionado");
    btnSeleccionar = new QPushButton("Seleccionar archivo");
    btnAnalizar = new QPushButton("Analizar");

    editorLineas = new QPlainTextEdit;
    editorCodigo = new QPlainTextEdit;
    editorResultado = new QTextEdit;

    editorLineas->setReadOnly(true);
    editorLineas->setFixedWidth(60);
    editorLineas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editorLineas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    editorCodigo->setReadOnly(true);
    editorCodigo->setPlaceholderText("Aquí se mostrará el código cargado...");

    editorResultado->setReadOnly(true);
    editorResultado->setPlaceholderText("Aquí aparecerá el resultado del análisis...");

    labelRuta->setStyleSheet(
        "font-weight: bold;"
        "color: #0d47a1;"
        "font-size: 15px;"
    );

    btnSeleccionar->setStyleSheet(
        "QPushButton {"
        "  background-color: #1976d2;"
        "  color: white;"
        "  border-radius: 6px;"
        "  padding: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1565c0;"
        "}"
    );

    btnAnalizar->setStyleSheet(
        "QPushButton {"
        "  background-color: #2e7d32;"
        "  color: white;"
        "  border-radius: 6px;"
        "  padding: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1b5e20;"
        "}"
    );

    editorLineas->setStyleSheet(
        "background-color: #f5f5f5;"
        "color: #666;"
        "border: 1px solid #d0d0d0;"
        "font-family: monospace;"
        "font-size: 14px;"
    );

    editorCodigo->setStyleSheet(
        "background-color: #fcfcfc;"
        "color: #1e1e1e;"
        "border: 1px solid #d0d0d0;"
        "font-family: monospace;"
        "font-size: 14px;"
    );

    editorResultado->setStyleSheet(
        "background-color: #ffffff;"
        "border: 1px solid #d0d0d0;"
        "font-size: 14px;"
    );

    layoutBotones->addWidget(btnSeleccionar);
    layoutBotones->addWidget(btnAnalizar);

    layoutCodigo->addWidget(editorLineas);
    layoutCodigo->addWidget(editorCodigo);

    layoutPrincipal->addWidget(labelRuta);
    layoutPrincipal->addLayout(layoutBotones);
    layoutPrincipal->addLayout(layoutCodigo, 3);
    layoutPrincipal->addWidget(editorResultado, 1);

    central->setLayout(layoutPrincipal);
    setCentralWidget(central);

    setWindowTitle("Parser Rust - Interfaz Gráfica");
    resize(950, 700);

    connect(btnSeleccionar, &QPushButton::clicked, this, [this]() {
        seleccionarArchivo();
    });

    connect(btnAnalizar, &QPushButton::clicked, this, [this]() {
        ejecutarAnalisis();
    });

    connect(editorCodigo->verticalScrollBar(), &QScrollBar::valueChanged,
            editorLineas->verticalScrollBar(), &QScrollBar::setValue);
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

    cargarContenidoArchivo(rutaArchivo);
    editorResultado->clear();

    editorCodigo->setExtraSelections({});
}

void MainWindow::cargarContenidoArchivo(const QString& ruta)
{
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        editorCodigo->setPlainText("No se pudo abrir el archivo.");
        editorLineas->clear();
        return;
    }

    QTextStream in(&archivo);
    QString contenido = in.readAll();
    archivo.close();

    editorCodigo->setPlainText(contenido);
    actualizarLineas();
}

void MainWindow::actualizarLineas()
{
    int totalLineas = editorCodigo->document()->blockCount();
    QString numeros;

    for (int i = 1; i <= totalLineas; ++i) {
        numeros += QString::number(i) + "\n";
    }

    editorLineas->setPlainText(numeros);
}

void MainWindow::resaltarLineaError(const QString& salida)
{
    QList<QTextEdit::ExtraSelection> selecciones;

    QRegularExpression re("linea\\s+(\\d+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(salida);

    if (match.hasMatch()) {
        int numeroLinea = match.captured(1).toInt();

        QTextBlock bloque = editorCodigo->document()->findBlockByLineNumber(numeroLinea - 1);
        if (bloque.isValid()) {
            QTextCursor cursor(bloque);

            QTextEdit::ExtraSelection seleccion;
            seleccion.cursor = cursor;
            seleccion.format.setBackground(QColor(255, 204, 204));
            seleccion.format.setForeground(QColor(120, 0, 0));
            seleccion.format.setProperty(QTextFormat::FullWidthSelection, true);

            selecciones.append(seleccion);

            editorCodigo->setTextCursor(cursor);
            editorCodigo->centerCursor();
        }
    }

    editorCodigo->setExtraSelections(selecciones);
}

void MainWindow::ejecutarAnalisis()
{
    if (rutaArchivo.isEmpty()) {
        editorResultado->setHtml(
            "<span style='color:#c62828; font-weight:bold;'>Primero debes seleccionar un archivo.</span>"
        );
        return;
    }

    std::string resultado = ejecutarParser(rutaArchivo.toStdString());
    QString salida = QString::fromStdString(resultado);

    if (salida.contains("Error de sintaxis", Qt::CaseInsensitive)) {
        editorResultado->setHtml(
            "<div style='color:#c62828; font-weight:bold;'>Error encontrado</div>"
            "<pre style='color:#222; font-family: monospace;'>" + salida.toHtmlEscaped() + "</pre>"
        );
    } else {
        editorResultado->setHtml(
            "<div style='color:#2e7d32; font-weight:bold;'>Sintaxis correcta</div>"
            "<pre style='color:#222; font-family: monospace;'>" + salida.toHtmlEscaped() + "</pre>"
        );
    }

    resaltarLineaError(salida);
}