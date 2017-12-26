#include "generate.h"

cmgenerate::cmgenerate() :
    setting(new settings())
{
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
