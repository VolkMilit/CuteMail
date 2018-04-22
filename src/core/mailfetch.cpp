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

#include "mailfetch.h"

/*most of the code from: https://github.com/dinhviethoa/libetpan/blob/master/tests/imap-sample.c
with fixed formating.
*/

mailfetch::mailfetch(QString account, QString server, QString password) :
    gen(new cmgenerate()),
    setting(new settings())
{
    struct mailimap * imap;
    int r;

    imap = mailimap_new(0, NULL);
    r = mailimap_ssl_connect(imap, server.toStdString().c_str(), 993);

    if (this->checkError(r, "Could not connect to server.") < 0)
        return;

    r = mailimap_login(imap, account.toStdString().c_str(), password.toStdString().c_str());
    if (this->checkError(r, "Could not login.") < 0)
        return;

    r = mailimap_select(imap, "INBOX");

    if (this->checkError(r, "Could not select INBOX.") < 0)
        return;

    this->fetch_messages(imap, account.toStdString().c_str());

    mailimap_logout(imap);
    mailimap_free(imap);
}

mailfetch::~mailfetch()
{
    delete gen;
    delete setting;
}

void mailfetch::fetch_messages(mailimap *imap, const char *account)
{
    struct mailimap_set * set;
    struct mailimap_fetch_type * fetch_type;
    struct mailimap_fetch_att * fetch_att;
    clist * fetch_result;
    clistiter * cur;
    int r;

    database db(setting->getSettingsPath() + "accounts.db", QString(account));

    /* as improvement UIDVALIDITY should be read and the message cache should be cleaned
        if the UIDVALIDITY is not the same */

    set = mailimap_set_new_interval(1, 0); /* fetch in interval 1:* */
    fetch_type = mailimap_fetch_type_new_fetch_att_list_empty();
    fetch_att = mailimap_fetch_att_new_uid();
    mailimap_fetch_type_new_fetch_att_list_add(fetch_type, fetch_att);

    r = mailimap_fetch(imap, set, fetch_type, &fetch_result);
    this->checkError(r, "Could not fetch messages.");

    /* for each message */
    for(cur = clist_begin(fetch_result) ; cur != NULL ; cur = clist_next(cur))
    {
        struct mailimap_msg_att * msg_att;
        uint32_t uid;

        msg_att = (mailimap_msg_att*)clist_content(cur);
        uid = get_uid(msg_att);

        if (uid == 0)
            continue;

        if (fetch_type->ft_type == MAILIMAP_STATUS_ATT_UNSEEN)
        {
            //qDebug() << "UID " << uid << " unseen";
            db.addValue("unseen", "status");
        }

        this->fetch_msg(imap, uid, account);
    }

    mailimap_fetch_list_free(fetch_result);
}

void mailfetch::fetch_msg(mailimap *imap, uint32_t uid, const char *account)
{
    struct mailimap_set * set;
    struct mailimap_section * section;
    //char filename[512];
    size_t msg_len;
    char * msg_content;
    //FILE * f;
    struct mailimap_fetch_type * fetch_type;
    struct mailimap_fetch_att * fetch_att;
    int r;
    clist * fetch_result;
    //struct stat stat_info;

    QString filename = "mail/" + QString(account) + "/incoming/" + QString::number(uid) + ".eml";
    QFile f(filename);
    //snprintf(filename, sizeof(filename), "mail/%s/incoming/%u.eml", (char*)account, (unsigned int) uid);
    //r = stat(fn.toStdString().c_str(), &stat_info);

    if (f.exists())
    {
        qDebug() << (unsigned int) uid << " is already fetched";
        return;
    }

    set = mailimap_set_new_single(uid);
    fetch_type = mailimap_fetch_type_new_fetch_att_list_empty();
    section = mailimap_section_new(NULL);
    fetch_att = mailimap_fetch_att_new_body_peek_section(section);
    mailimap_fetch_type_new_fetch_att_list_add(fetch_type, fetch_att);

    r = mailimap_uid_fetch(imap, set, fetch_type, &fetch_result);
    this->checkError(r, "Could not fetch messages.");
    qDebug() << "fetch " << (unsigned int) uid;

    msg_content = get_msg_content(fetch_result, &msg_len);
    if (msg_content == NULL)
    {
        QStdErr() << "no content";
        mailimap_fetch_list_free(fetch_result);
        return;
    }

    //f = fopen(filename, "w");
    if (f.open(QIODevice::ReadWrite))
    {
        f.write(msg_content, msg_len);
    }
    else
    {
        QStdErr() << "could not write";
        mailimap_fetch_list_free(fetch_result);
    }

    //fwrite(msg_content, 1, msg_len, f);
    //fclose(f);
    f.close();

    qDebug() << (unsigned int) uid << " has been fetched";

    mailimap_fetch_list_free(fetch_result);
}

int mailfetch::checkError(int r, QString msg)
{
    if (r == MAILIMAP_NO_ERROR)
        return 0;

    if (r == MAILIMAP_NO_ERROR_AUTHENTICATED)
        return 0;

    if (r == MAILIMAP_NO_ERROR_NON_AUTHENTICATED)
        return 0;

    QWidget *parent = 0;
    QMessageBox::critical(parent, "CuteMail Error", msg, QMessageBox::Ok);

    return -1;
}

char* mailfetch::get_msg_att_msg_content(mailimap_msg_att *msg_att, size_t *p_msg_size)
{
    clistiter * cur;

    /* iterate on each result of one given message */
    for(cur = clist_begin(msg_att->att_list) ; cur != NULL ; cur = clist_next(cur))
    {
        struct mailimap_msg_att_item * item;

        item = (mailimap_msg_att_item*)clist_content(cur);

        if (item->att_type != MAILIMAP_MSG_ATT_ITEM_STATIC)
            continue;

        if (item->att_data.att_static->att_type != MAILIMAP_MSG_ATT_BODY_SECTION)
            continue;

        *p_msg_size = item->att_data.att_static->att_data.att_body_section->sec_length;

        return item->att_data.att_static->att_data.att_body_section->sec_body_part;
    }

    return NULL;
}

char* mailfetch::get_msg_content(clist *fetch_result, size_t *p_msg_size)
{
    clistiter * cur;

    /* for each message (there will be probably only on message) */
    for(cur = clist_begin(fetch_result) ; cur != NULL ; cur = clist_next(cur))
    {
        struct mailimap_msg_att * msg_att;
        size_t msg_size;
        char * msg_content;

        msg_att = (mailimap_msg_att*)clist_content(cur);
        msg_content = get_msg_att_msg_content(msg_att, &msg_size);

        if (msg_content == NULL)
            continue;

        *p_msg_size = msg_size;

        return msg_content;
    }

    return NULL;
}

uint32_t mailfetch::get_uid(mailimap_msg_att *msg_att)
{
    clistiter * cur;

    /* iterate on each result of one given message */
    for(cur = clist_begin(msg_att->att_list) ; cur != NULL ; cur = clist_next(cur))
    {
        struct mailimap_msg_att_item * item;

        item = (mailimap_msg_att_item*)clist_content(cur);

        if (item->att_type != MAILIMAP_MSG_ATT_ITEM_STATIC)
            continue;

        if (item->att_data.att_static->att_type != MAILIMAP_MSG_ATT_UID)
            continue;

        return item->att_data.att_static->att_data.att_uid;
    }

    return 0;
}
