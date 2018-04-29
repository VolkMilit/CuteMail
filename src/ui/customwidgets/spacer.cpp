#include "spacer.h"

Ui::Spacer::Spacer(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

Ui::Spacer::~Spacer()
{
    this->deleteLater();
}
