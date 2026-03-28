#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QPushButton;
class QTextEdit;
class QPlainTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QLabel *labelRuta;
    QPushButton *btnSeleccionar;
    QPushButton *btnAnalizar;

    QPlainTextEdit *editorLineas;
    QPlainTextEdit *editorCodigo;
    QTextEdit *editorResultado;

    QString rutaArchivo;

    void seleccionarArchivo();
    void ejecutarAnalisis();
    void cargarContenidoArchivo(const QString& ruta);
    void actualizarLineas();
    void resaltarLineaError(const QString& salida);
};

#endif