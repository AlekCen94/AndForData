#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QPushButton>
#include <QStringList>
#include "commandtype.h"
#include <QMessageBox>
#include <QFileDialog>

#define ICON_SIZE 20

QIcon icon;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
    for(int i = 0; i<m_Commands_Group.buttons().size(); i++)
    {
        delete  m_Commands_Group.buttons()[i];
    }
    delete ui;
}

void MainWindow::init()
{
    ui->m_Contacts_Table->setVisible(false);
    ui->m_Calls_Table->setVisible(false);
    ui->m_Messages_Table->setVisible(false);
    ui->m_Contacts_Lab->setVisible(false);
    ui->m_Calls_Lab->setVisible(false);
    ui->m_Message_Lab->setVisible(false);
    ui->m_Shell_Output->setVisible(false);
    ui->m_Command_Edit->setVisible(false);
    ui->m_Dir->setVisible(false);
    ui->m_Root_Shell->setVisible(false);


    m_Icon.load(ICON);
    m_Icon_Size.setWidth(ICON_SIZE); m_Icon_Size.setHeight(ICON_SIZE);


    icon.addPixmap(m_Icon);
    icon.pixmap(m_Icon_Size);
    this->setWindowIcon(icon);

    javaCheck(); sdkCheck();
    m_Commands_Group.addButton(new QPushButton("Devices"),DEVICES);
    m_Commands_Group.addButton(new QPushButton("Restart"),RESTART);
    m_Commands_Group.addButton(new QPushButton("Shell"),SELL);
    m_Commands_Group.addButton(new QPushButton("Contacts"),CONTACTS);
    m_Commands_Group.addButton(new QPushButton("Messages"),MESSAGES);
    m_Commands_Group.addButton(new QPushButton("Calls"),CALLS);
    m_Commands_Group.addButton(new QPushButton("Install"),INSTALL);
    m_Commands_Group.addButton(new QPushButton("Cat"),CATLOG);

    m_Commands_Group.connect(&m_Commands_Group, SIGNAL(buttonClicked(int)), this, SLOT(clicked(int)));

    ui->m_Commands->addWidget(m_Commands_Group.button(DEVICES));
    ui->m_Commands->addWidget(m_Commands_Group.button(RESTART));
    ui->m_Commands->addWidget(m_Commands_Group.button(SELL));
    ui->m_Commands->addWidget(m_Commands_Group.button(CONTACTS));
    ui->m_Commands->addWidget(m_Commands_Group.button(MESSAGES));
    ui->m_Commands->addWidget(m_Commands_Group.button(CALLS));
    ui->m_Commands->addWidget(m_Commands_Group.button(INSTALL));
    ui->m_Commands->addWidget(m_Commands_Group.button(CATLOG));
}


void MainWindow::clicked(int id)
{

    switch (id) {
        case DEVICES: devices();break;
        case RESTART: restart();break;
        case SELL: shell();break;
        case CONTACTS: contacts();break;
        case MESSAGES: messages();break;
        case CALLS: calls();break;
        case INSTALL: install();break;
        case CATLOG: cat();break;
        default: break;
    }

}

void MainWindow::devices()
{
    ui->m_Device_List->clear();
    Command::getInstance().adbDevices();
    QString result = Command::getInstance().getCommandOutput().remove("device");
    QStringList devices = result.split(SEPARATOR);
    devices.removeFirst();
    devices.removeAll(EMPTY);

    for(int i = 0; i<devices.size(); i++)
    {
        ui->m_Device_List->addItem(new QListWidgetItem(m_Icon,devices[i]));
    }

}
void MainWindow::restart()
{
    ui->m_Device_List->clear();
    Command::getInstance().adbRestart();
    devices();
}

void MainWindow::shell()
{
    if(ui->m_Device_List->currentItem()!=nullptr)
    {
        Command::getInstance().setDevice(ui->m_Device_List->currentItem()->text());
        ui->m_Command_Edit->setEnabled(true);
        ui->m_Shell_Output->setVisible(true);
        ui->m_Command_Edit->setVisible(true);
        ui->m_Root_Shell->setVisible(true);
        ui->m_Dir->setVisible(true);

    }
    else

        QMessageBox::warning(this, tr(NAME), tr(SELECT_WARNING), QMessageBox::Ok);
}

void MainWindow::contacts()
{
    if(ui->m_Device_List->currentItem()!=nullptr)
    {
        ui->m_Contacts_Table->clear();
        Command::getInstance().setDevice(ui->m_Device_List->currentItem()->text());
        ui->m_Contacts_Table->setVisible(true);
        ui->m_Contacts_Lab->setVisible(true);

        Command::getInstance().adbGetContacts();
        QString result = Command::getInstance().getCommandOutput();

        result.replace("display_name","name");
        result.replace(CSEP,EMPTY);
        result.replace("Row:",EMPTY);

        QStringList contacts = result.split(SEPARATOR);
        ui->m_Contacts_Table->addItems(contacts);

        QFile fOut("contacts.txt");
        if (fOut.open(QFile::WriteOnly | QFile::Text)) {
          QTextStream s(&fOut);
          for (int i = 0; i < contacts.size(); ++i)
            s << contacts.at(i) << '\n';
        } else {

        }
        fOut.close();

    }
    else

        QMessageBox::warning(this, tr(NAME), tr(SELECT_WARNING), QMessageBox::Ok);
}

void MainWindow::messages()
{
    if(ui->m_Device_List->currentItem()!=nullptr)
    {
        Command::getInstance().setDevice(ui->m_Device_List->currentItem()->text());
        Command::getInstance().adbGetMessages();

        if(Command::getInstance().getCommandOutput().contains(" Permission Denial")||Command::getInstance().getCommandOutput()==EMPTY)

            QMessageBox::warning(this, tr(NAME), tr(NOT_ROOT), QMessageBox::Ok);
        else
        {
            ui->m_Messages_Table->setVisible(true);
            ui->m_Message_Lab->setVisible(true);
            QString result = Command::getInstance().getCommandOutput();
            QStringList messages = result.split(SEPARATOR);
            ui->m_Messages_Table->addItems(messages);

        }

    }
    else

        QMessageBox::warning(this, tr(NAME), tr(SELECT_WARNING), QMessageBox::Ok);
}

void MainWindow::calls()
{
    if(ui->m_Device_List->currentItem()!=nullptr)
    {
        Command::getInstance().setDevice(ui->m_Device_List->currentItem()->text());
        Command::getInstance().adbGetCalls();

        if(Command::getInstance().getCommandOutput().contains(" Permission Denial")||Command::getInstance().getCommandOutput()==EMPTY)

            QMessageBox::warning(this, tr(NAME), tr(NOT_ROOT), QMessageBox::Ok);
        else
        {
            ui->m_Calls_Table->setVisible(true);
            ui->m_Calls_Lab->setVisible(true);
            QString result = Command::getInstance().getCommandOutput();
            QStringList calls = result.split(SEPARATOR);
            ui->m_Calls_Table->addItems(calls);

        }

    }
    else

        QMessageBox::warning(this, tr(NAME), tr(SELECT_WARNING), QMessageBox::Ok);
}

void MainWindow::install()
{
    if(ui->m_Device_List->currentItem()!=nullptr)
    {
        Command::getInstance().setDevice(ui->m_Device_List->currentItem()->text());
        QString path = QFileDialog::getOpenFileName(this,OPEN_APK,".",APK);
        qDebug()<<path;
        Command::getInstance().adbInstall(path);

        if(Command::getInstance().getCommandOutput().contains("Success"))
           {
              QMessageBox::warning(this, tr(NAME), tr(INSTALL_APK_SUCCESS), QMessageBox::Ok);
           }
        else
            QMessageBox::warning(this, tr(NAME), tr(INSTALL_APK_ERROR), QMessageBox::Ok);
    }
    else

        QMessageBox::warning(this, tr(NAME), tr(SELECT_WARNING), QMessageBox::Ok);
}

void MainWindow::cat()
{
    if(ui->m_Device_List->currentItem()!=nullptr)
    {
        Command::getInstance().setDevice(ui->m_Device_List->currentItem()->text());
        Command::getInstance().adbCat("/storage/self/primary/AndForData/sms.txt");

        ui->m_Messages_Table->setVisible(true);
        ui->m_Message_Lab->setVisible(true);
        QString result = Command::getInstance().getCommandOutput();
        QStringList messages = result.split("^");
        ui->m_Messages_Table->addItems(messages);

         Command::getInstance().adbCat("/storage/self/primary/AndForData/call.txt");

         ui->m_Calls_Table->setVisible(true);
         ui->m_Calls_Lab->setVisible(true);
         QString result2= Command::getInstance().getCommandOutput();
         QStringList calls = result2.split("^");
         ui->m_Calls_Table->addItems(calls);

         Command::getInstance().adbPull("/storage/self/primary/AndForData/sms.txt",".");
         Command::getInstance().adbPull("/storage/self/primary/AndForData/call.txt",".");
    }

    else

        QMessageBox::warning(this, tr(NAME), tr(SELECT_WARNING), QMessageBox::Ok);
}

void MainWindow::javaCheck()
{
    QPalette sample_palette;
    if(Command::getInstance().checkJava())
    {

        sample_palette.setColor(QPalette::WindowText, Qt::green);
        ui->m_Java_Check->setText("Java is installed");
        ui->m_Java_Check->setPalette(sample_palette);

    }
    else
    {
        sample_palette.setColor(QPalette::WindowText, Qt::red);
        ui->m_Java_Check->setText("Java is not installed");
        ui->m_Java_Check->setPalette(sample_palette);

    }
}


void MainWindow::sdkCheck()
{
    QPalette sample_palette;
    if(Command::getInstance().checkSDK())
    {

        sample_palette.setColor(QPalette::WindowText, Qt::green);
        ui->m_Sdk_Check->setText("SDK is found");
        ui->m_Sdk_Check->setPalette(sample_palette);

    }
    else
    {
        sample_palette.setColor(QPalette::WindowText, Qt::red);
        ui->m_Sdk_Check->setText("SDK is not found");
        ui->m_Sdk_Check->setPalette(sample_palette);

    }
}

void MainWindow::on_m_Command_Edit_returnPressed()
{
    if(ui->m_Root_Shell->isChecked())
        Command::getInstance().adbSell(SUDO + ui->m_Command_Edit->text());

    else
        Command::getInstance().adbSell(ui->m_Command_Edit->text());

    ui->m_Shell_Output->setPlainText(Command::getInstance().getCommandOutput());
    ui->m_Command_Edit->clear();

}
