#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QPushButton;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QLabel *labelRuta;
    QTextEdit *editorSalida;
    QPushButton *btnSeleccionar;
    QPushButton *btnAnalizar;

    QString rutaArchivo;

    void seleccionarArchivo();
    void ejecutarAnalisis();
};

#endif