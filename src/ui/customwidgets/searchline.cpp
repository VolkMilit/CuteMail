#include "searchline.h"

Ui::SearchLine::SearchLine(QWidget *parent) :
    QLineEdit(parent), setting(new Settings())
{
    this->setClearButtonEnabled(true);
    this->updatePlaceholder();
}

Ui::SearchLine::~SearchLine()
{
    this->deleteLater();
}

void Ui::SearchLine::updatePlaceholder()
{
    QString s_search = setting->read(setting->getSettingsFilePath(), "Shortcuts", "Search");
    QShortcut *sc_search = new QShortcut(QKeySequence(s_search), this);
    connect(sc_search, &QShortcut::activated, this, QOverload<>::of(&QLineEdit::setFocus));
    this->setPlaceholderText("Search... (" + s_search + ")");
}
