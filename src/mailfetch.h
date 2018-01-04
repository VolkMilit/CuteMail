#ifndef MAILFETCH_H
#define MAILFETCH_H

#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
#include <libetpan/libetpan.h>
#include <stdlib.h>
#include <sys/stat.h>
#ifdef __cplusplus
}
#endif

#include "generate.h"
#include "utils.h"

class mailfetch : private utils
{
public:
    mailfetch(QString account, QString server, QString password);
    virtual ~mailfetch();

private:
    void fetch_messages(struct mailimap * imap, const char *account);
    void fetch_msg(struct mailimap * imap, uint32_t uid, const char *account);
    void checkError(int r, QString msg);
    char* get_msg_att_msg_content(struct mailimap_msg_att * msg_att, size_t * p_msg_size);
    char* get_msg_content(clist * fetch_result, size_t * p_msg_size);
    uint32_t get_uid(struct mailimap_msg_att * msg_att);

    cmgenerate *gen;
};

#endif // MAILFETCH_H
