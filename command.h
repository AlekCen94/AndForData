#ifndef COMMAND_H
#define COMMAND_H
#include <QString>
#include <QProcess>
#include "constants.h"

class Command
{
protected:
    QString m_Command_Output; // > Izlaz izvrsenja komande
    static Command m_Instance; // > Instanca objekta
    QString m_Device; // > Selektovani uredjaj
    
private:
    QProcess m_Process;
    
private:
    Command(); // > Konstruktor

public:

    ~Command(); // > Destruktor


public:
    /// Metoda koja vraca instancu Command objekta
    ///
    static Command& getInstance();

    /// Metoda koja vraca izlaz komande
    ///
    QString getCommandOutput();
    
    /// Metoda koja vraca id string selektovanog uredjaja
    /// 
    QString getDevice();
    
    /// Metoda koja postavlja selekciju uredjaja
    /// 
    void setDevice(QString device);

    /// Metoda koja proverava koliko ima spojenih uredjaja na racunaru
    ///
    void adbDevices();

    /// Metoda koja resetuje adb-server
    ///
    void adbRestart();

    /// Metoda koja selektuje uredjaj
    /// \param device uredjaj koji se seletktuje
    ///
    void adbSell(QString command);

    /// Metoda koja instalira android aplikaciju
    /// \param path putanja do aplikacije
    /// 
    void adbInstall(QString path);

    /// Metoda koja povlaci fajil sa uredjaja
    /// \param sourpath putanja do fajla
    /// \param dispath odredisna putanja
    ///
    void adbPull(QString sourpath,QString distpath);

    /// Metoda koja prikazuje sadryaj datoteke
    /// \param path putanja do fajla
    ///
    void adbCat(QString path);

    /// Metoda koja proverava da li je uredjaj rutovan
    ///
    bool isRooted();

    /// Metoda koja proverava da li je java intalirana na masini
    ///
    bool checkJava();

    /// Metoda koja proverava da li je instaliran SDK (adb tool) na masini
    ///
    bool checkSDK();
    
    /// Metoda koja vadi kontakte iz Android uredjaja
    /// 
    void adbGetContacts();

    /// Metoda koja vadi listu poziva iz Android uredjaja
    ///
    void adbGetCalls();

    /// Metoda koja vadi sms poruke iz Android uredjaja
    ///
    void adbGetMessages();
};

#endif // COMMAND_H
