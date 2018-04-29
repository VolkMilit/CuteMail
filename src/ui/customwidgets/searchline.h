#ifndef SEARCHLINE_H
#define SEARCHLINE_H

#include <QLineEdit>
#include <QKeySequence>
#include <QShortcut>

#include "utils/settings.h"

namespace Ui
{

class SearchLine : public QLineEdit
{
    public:
        SearchLine(QWidget *parent);
        ~SearchLine();

        void updatePlaceholder();

    private:
        Settings *setting;
};

}

#endif // SEARCHLINE_H
