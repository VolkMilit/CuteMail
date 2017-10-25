#ifndef MHONARC_H
#define MHONARC_H

#include <QProcess>
#include <QDir>

class mhonarc
{
public:
    mhonarc();
    ~mhonarc();
    void run();

private:
    QProcess *mhonarc_process;
};

#endif // MHONARC_H
