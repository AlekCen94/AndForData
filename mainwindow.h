#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QPixmap>
#include "command.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
private:
    QButtonGroup m_Commands_Group; // > Grupa dugmeta za komande
    QPixmap m_Icon; // > Ikonica na listi
    QSize m_Icon_Size; // > Velicina ikonice

public:
    MainWindow(QWidget *parent = nullptr); // > Konstruktor
    ~MainWindow(); // > Destruktor


private:
    Ui::MainWindow *ui;

private slots:
    void clicked(int);

    void on_m_Command_Edit_returnPressed();

private:
    /// Metoda koja inicijalizuje komponente na prozoru
    ///
    void init();

    /// Metoda koja se poziva na klik dugmeta Device
    ///
    void devices();

    /// Metoda Metoda koja se poziva na klik dugmeta Restart
    ///
    void restart();

    /// Metoda koja se poziva na klik dugmeta Sell
    ///
    void shell();

    /// Metoda koja se poziva na klik dugmeta Contacts
    ///
    void contacts();

    /// Metoda koja se poziva na klik dugmeta Messages
    ///
    void messages();

    /// Metoda koja se poziva na klik dugmeta Calls
    ///
    void calls();

    /// Metoda koja se poziva na klik dugmeta Install
    ///
    void install();

    /// Metoda koja se poziva na klik dugmeta Cat
    ///
    void cat();

    /// Metoda koja proverava da li je java instalirana u sistemu
    ///
    void javaCheck();

    /// Metoda koja proverava da li je Android SDK (ADB tool) instaliran u sistemu
    ///
    void sdkCheck();
};
#endif // MAINWINDOW_H
