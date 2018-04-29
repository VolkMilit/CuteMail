#ifndef HTMLBROWSER_H
#define HTMLBROWSER_H

#include <QWebView>
#include <QDesktopServices>
#include <QUrl>

#include "utils/settings.h"

namespace Ui
{

class HTMLBrowser : public QWebView
{
    public:
        HTMLBrowser(QWidget *parent);
        virtual ~HTMLBrowser();

    private slots:
        void openExternal(const QUrl &url);

    private:
        Settings *setting;
};

}

#endif // HTMLBROWSER_H
