#include "utils.h"

utils::utils(){}
utils::~utils(){}

QString utils::decodeQP(QString str)
{
    std::string str_std;
    mimetic::QP::Decoder qp;

    mimetic::code(str.toStdString().begin(), \
                  str.toStdString().end(), \
                  qp, \
                  std::back_inserter<std::string>(str_std));

    return QString::fromStdString(str_std);
}

std::string utils::decodeQP(const std::string &str)
{
    std::string str_std;
    mimetic::QP::Decoder qp;

    mimetic::code(str.begin(), \
                  str.end(), \
                  qp, \
                  std::back_inserter<std::string>(str_std));

    return str_std;
}

QString utils::decodeB64(QString str)
{
    std::string str_std;
    mimetic::Base64::Decoder base64;

    mimetic::code(str.toStdString().begin(), \
                  str.toStdString().end(), \
                  base64, \
                  std::back_inserter<std::string>(str_std));

    return QString::fromStdString(str_std);
}

std::string utils::decodeB64(const std::string &str)
{
    std::string str_std;
    mimetic::Base64::Decoder base64;

    mimetic::code(str.begin(), \
                  str.end(), \
                  base64, \
                  std::back_inserter<std::string>(str_std));

    return str_std;
}

/*
    Note that it is necessary to store the bytearray
    before you call data() on it,  a call like the following:
    const char *c_str2 = str2.toLatin1().data();
    will make the application crash as the QByteArray has not
    been stored and hence no longer exists

    from: https://stackoverflow.com/questions/5505221/converting-qstring-to-char
*/
const char *utils::toCChar(QString qstr)
{
    QByteArray qbraw = qstr.toLatin1();
    const char *cc_str = qbraw.data();

    return cc_str;
}

std::vector<std::string> utils::stdSplit(const std::string &str, char delim)
{
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;

    while (std::getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}

QTextStream &utils::QStdErr()
{
    static QTextStream ts(stderr);
    return ts;
}
