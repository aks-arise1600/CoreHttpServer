/**
* @file HTTP_Handler.cpp
* @author Anil Kumar
* @date 19April2025
* @brief Header of HTTP_Handler class.
*/
#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <QObject>
#include "httplib.h"
#include "QDebug"
#include "QProcess"
#include "QOperatingSystemVersion"

class HTTP_Handler : public QObject
{
    Q_OBJECT
public:
    explicit HTTP_Handler(QObject *parent = nullptr);

signals:
private:
    httplib::Server obj_svr;
    void m_processRequest();

};

#endif // HTTP_HANDLER_H
