#ifndef FETCHMAIL_H
#define FETCHMAIL_H

#include <QProcess>
#include <QObject>

class fetchMail
{
public:
    fetchMail();
    ~fetchMail();
    void run();

private:
    QProcess *fetchmail_process;
};

#endif // FETCHMAIL_H
