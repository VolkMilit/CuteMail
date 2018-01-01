#ifndef UTILS_H
#define UTILS_H

#include <QString>

#include <mimetic/mimetic.h>
#include <iostream>
#include <vector>

class utils
{
public:
    utils();
    virtual ~utils();

protected:
    QString decodeQP(QString str);
    std::string decodeQP(const std::string &str);

    QString decodeB64(QString str);
    std::string decodeB64(const std::string &str);

    const char *toCChar(QString str);
    std::vector<std::string> stdSplit(const std::string &str, char delim);
};

#endif // UTILS_H
