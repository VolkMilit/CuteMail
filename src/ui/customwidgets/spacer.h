#ifndef SPACER_H
#define SPACER_H

#include <QObject>
#include <QWidget>

namespace Ui
{

class Spacer : public QWidget
{
    public:
        explicit Spacer(QWidget *parent = 0);
        ~Spacer();
};

}

#endif // SPACER_H
