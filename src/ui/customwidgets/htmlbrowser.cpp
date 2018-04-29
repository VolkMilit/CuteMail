#include "htmlbrowser.h"

Ui::HTMLBrowser::HTMLBrowser(QWidget *parent) :
    QWebView(parent), setting(new Settings())
{
    // disable all sh*t for privacy concerns
    this->settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
    this->settings()->setAttribute(QWebSettings::JavaEnabled, false);
    this->settings()->setAttribute(QWebSettings::PluginsEnabled, false);
    this->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, false);
    this->settings()->setAttribute(QWebSettings::Accelerated2dCanvasEnabled, false);
    this->settings()->setAttribute(QWebSettings::WebGLEnabled, false);
    this->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, false);
    //this->settings()->setAttribute(QWebSettings::AlwaysBlockThirdPartyCookies, false);
    this->settings()->setMaximumPagesInCache(0);
    this->settings()->setOfflineStorageDefaultQuota(0);
    this->settings()->setObjectCacheCapacities(0, 0, 0);
    this->settings()->setOfflineWebApplicationCacheQuota(0);

    // need to be configurable
    this->settings()->setAttribute(QWebSettings::AutoLoadImages, true);

    // don't know if this needed, but, well, let it be
    this->settings()->setDefaultTextEncoding("utf-8");

    // allow using external browser for links
    this->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(this, &QWebView::linkClicked, this, &HTMLBrowser::openExternal);

    this->hide();
}

Ui::HTMLBrowser::~HTMLBrowser()
{
    this->settings()->clearMemoryCaches();
    this->deleteLater();
}

void Ui::HTMLBrowser::openExternal(const QUrl &url)
{
    if (setting->getUseXDGBrowser() == 1)
    {
        QDesktopServices::openUrl(url.toString());
    }
    else
    {
        this->load(url);
        //connect(ui->bt_chngview, &QPushButton::clicked, this, &MainWindow::showFullMessage);
        //showSplash("Using external browser is disabled. You can go back, click this button.", "Go back");
    }
}
