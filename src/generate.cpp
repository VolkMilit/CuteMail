#include "generate.h"

cmgenerate::cmgenerate() :
    setting(new settings())
{
    fetchmailConfigPath = QDir::homePath() + "/.fetchmailrc";
    procmailConfigPath = QDir::homePath() + "/.procmailrc";
    mailFolderPath = QDir::homePath() + "/mail/"; // will be configurable
}

cmgenerate::~cmgenerate()
{
    delete setting;
}

QString cmgenerate::getMailFolderPath()
{
    return mailFolderPath;
}

// temporarily "fix" for creating executable script for fetchmail and mha
// ps there is resource file
// https://www.mhonarc.org/MHonArc/doc/resources.html
// need to look closely, but I think we can generate file
// btw for the first prototipe version this is better solution to not fuck everyting up
void cmgenerate::mhaExecutable(QString account_name)
{
    const QString dirTmp = QDir::homePath() + "/mail/" + account_name + "/out/*";
    const QString dirIncoming = QDir::homePath() + "/mail/" + account_name + "/incoming/";

    QFile file(setting->getSettingsPath() + "start.sh");
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << "#!/bin/sh" << endl;
        stream << "fetchmail -a -k -m procmail &&" << endl;
        stream << "mhonarc -quiet -nomain -nomultipg -nonews -noprintxcomments "
                  "-nothread -outdir " + dirIncoming +
                  " " + dirTmp << endl;
    }

    file.setPermissions(QFile::ExeOwner | QFile::ReadOwner);
    file.close();
}

void cmgenerate::deleteExecutable()
{
    QFile file(setting->getSettingsPath() + "start.sh");

    if (file.exists())
        file.remove();
}

void cmgenerate::accauntsFolders(QString account_name)
{
    QDir account(mailFolderPath + account_name);
    QDir account_incoming(mailFolderPath + account_name + "/incoming");
    QDir account_junk(mailFolderPath + account_name + "/junk");
    QDir account_trash(mailFolderPath + account_name + "/trash");

    if (!account.exists())
        account.mkdir(mailFolderPath + account_name);

    if (!account_incoming.exists())
        account.mkdir(mailFolderPath + account_name + "/incoming");

    if (!account_junk.exists())
        account.mkdir(mailFolderPath + account_name + "/junk");

    if (!account_trash.exists())
        account.mkdir(mailFolderPath + account_name + "/trash");
}

// -------- procmail -------- //
void cmgenerate::procmailDelete()
{
    QFile file(procmailConfigPath);

    if (file.exists())
        file.remove();
}

// todo
// setup junk mail according to man pages
/*
 :0           # sink all junk mail
              * ^Subject:.*junk
              /dev/null

 */

// but also, I see another (and probably better) way to do it:
// https://github.com/MyTheValentinus/junkMail
// there is all mail domains with "junk" mail, so I think we
// must just generate file with this domain, check if
// mail is junk, and put it in the "junk" directory,
// so user can decide, when (s)he need to delete or restore
// mail from junk
// P.S. they have json file, so maybe we shall parse json
void cmgenerate::createProcmailConfig(QString account_name)
{
    QFile file(procmailConfigPath);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << "DEFAULT=" + QDir::homePath() + "/mail/" + account_name + "/out" << endl;
    }

    file.close();
}

void cmgenerate::procmailConfig(QString path)
{
    QFile file(procmailConfigPath);

    if (file.exists())
        file.remove();
    createProcmailConfig(path);
}
// -------------------------- //

// -------- fetchmail -------- //
void cmgenerate::fetchmailDelete()
{
    QFile file(fetchmailConfigPath);

    if (file.exists())
        file.remove();
}

void cmgenerate::createFetchFile(QString server, QString protocol, QString username, QString password)
{
    QFile file(fetchmailConfigPath);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << "poll " << protocol << "." << server << endl;
        stream << "protocol " << protocol << endl;
        stream << "username " << "\"" << username << "\"" << endl;
        stream << "password " << "\"" << password << "\"" << endl;
        stream << "options ssl" << endl;
    }

    // fetchmail want file permision 0700
    file.setPermissions(QFile::ExeOwner | QFile::ReadOwner | QFile::WriteOwner);
    file.close();
}

void cmgenerate::fetchmailConfig(QString server, QString protocol, QString username, QString password)
{
    QFile file(fetchmailConfigPath);

    if (file.exists())
        file.remove();
    createFetchFile(server, protocol, username, password);
}
// -------------------------- //
