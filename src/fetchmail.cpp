#include "fetchmail.h"
#include <QDebug>

fetchMail::fetchMail()
{
    QObject *parent = 0;
    fetchmail_process = new QProcess(parent);
}

fetchMail::~fetchMail()
{
    delete fetchmail_process;
}

void fetchMail::run()
{
    fetchmail_process->startDetached("fetchmail -a -k -m procmail");
}
