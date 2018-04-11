/*
    CuteMail - simple mail client for GNU/Linux.
    Copyright (C) 2018 Volk_Milit (aka Ja'Virr-Dar)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
    int checkError(int r, QString msg);
    char* get_msg_att_msg_content(struct mailimap_msg_att * msg_att, size_t * p_msg_size);
    char* get_msg_content(clist * fetch_result, size_t * p_msg_size);
    uint32_t get_uid(struct mailimap_msg_att * msg_att);

    cmgenerate *gen;
};

#endif // MAILFETCH_H
