#include "command.h"
#include <QDebug>
Command Command::m_Instance;

Command::Command()
{

}

Command& Command::getInstance()
{
    return m_Instance;
}

QString Command::getCommandOutput()
{
    return m_Command_Output;
}

Command::~Command()
{

}

void Command::adbDevices()
{
    m_Process.start("powershell adb devices");
   if ( m_Process. waitForFinished())
    {   
       m_Command_Output = m_Process.readAllStandardOutput();
       m_Command_Output.replace(TAB," ");
       qDebug()<<m_Command_Output;
   }
   else
   {
       m_Command_Output = m_Process.readAllStandardError();
        qDebug()<< m_Command_Output;
   }
}

void Command::adbRestart()
{
    m_Process.start("powershell adb kill-server");
    m_Process.waitForFinished();
    m_Command_Output = m_Process.readAllStandardOutput();
    qDebug()<<m_Command_Output;

    m_Process.start("powershell pkexec adb start-server");
    m_Process.waitForFinished();
    m_Command_Output = m_Process.readAllStandardOutput();
    qDebug()<<m_Command_Output;
}

void Command::adbSell(QString command)
{
    m_Process.start("powershell adb -s "+m_Device+" shell "+command);
   if( m_Process.waitForFinished())
        m_Command_Output = m_Process.readAllStandardOutput();
   else
       m_Command_Output = m_Process.readAllStandardError();
       qDebug()<<m_Command_Output;


}

void Command::adbInstall(QString path)
{
    m_Process.start("powershell adb -s "+m_Device+" install -t "+path);
   if( m_Process.waitForFinished())
        m_Command_Output = m_Process.readAllStandardOutput();
   else
       m_Command_Output = m_Process.readAllStandardError();
       qDebug()<<m_Command_Output;
}

void Command::adbPull(QString sourpath, QString distpath)
{
    m_Process.start("powershell adb -s "+m_Device+" pull "+sourpath+" "+distpath);
   if( m_Process.waitForFinished())
        m_Command_Output = m_Process.readAllStandardOutput();
   else
       m_Command_Output = m_Process.readAllStandardError();
       qDebug()<<m_Command_Output;
}

void Command::adbCat(QString path)
{
    m_Process.start("powershell adb -s "+m_Device+" shell cat "+path);
   if( m_Process.waitForFinished())
        m_Command_Output = m_Process.readAllStandardOutput();
   else
       m_Command_Output = m_Process.readAllStandardError();
       qDebug()<<m_Command_Output;
}

bool Command::isRooted()
{

}

void Command::adbGetContacts()
{
    m_Process.start("powershell adb -s "+m_Device+" shell content query --uri content://contacts/phones/  --projection display_name:number # google contacts?");

        if(m_Process.waitForFinished())
        {
            m_Command_Output = m_Process.readAllStandardOutput();
            qDebug()<<m_Command_Output;
        }
}

void Command::adbGetCalls()
{
    m_Process.start("powershell adb -s "+m_Device+" shell content query --uri content://call_log/calls");

        if(m_Process.waitForFinished())
        {
            m_Command_Output = m_Process.readAllStandardOutput();
            qDebug()<<m_Command_Output;

        }
}
void Command::adbGetMessages()
{
    m_Process.start("powershell adb -s "+m_Device+" shell content query --uri content://sms/inbox");

        if(m_Process.waitForFinished())
        {
            m_Command_Output = m_Process.readAllStandardOutput();
            qDebug()<<m_Command_Output;

        }
}


bool Command::checkJava()
{
    m_Process.start("powershell java");
    if(m_Process.waitForFinished())
    {
        m_Command_Output = m_Process.readAllStandardOutput();
        qDebug()<<m_Command_Output;
        return true;
    }
    else
        return false;
}

bool Command::checkSDK()
{
    m_Process.start("powershell whereis adb");
    if(m_Process.waitForFinished()&&m_Process.readAllStandardOutput()!="adb:")
    {
        m_Command_Output = m_Process.readAllStandardOutput();
        qDebug()<<m_Command_Output;
        return true;
    }
    else
        return false;
}

void Command::setDevice(QString device)
{
    m_Device = device;
}
