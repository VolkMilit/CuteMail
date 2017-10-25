#include "mhonarc.h"

mhonarc::mhonarc()
{
    QObject *parent = 0;
    mhonarc_process = new QProcess(parent);
}

mhonarc::~mhonarc()
{
    delete mhonarc_process;
}

void mhonarc::run()
{
    mhonarc_process->startDetached("mhonarc -quiet -nomain -nomultipg "
                                   "-nonews -noprintxcomments -nothread "
                                   "-outdir /home/volk/mail/out /home/volk/mail/incoming/msg.phwW");
}
