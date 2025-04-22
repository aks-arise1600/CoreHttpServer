/**
* @file HTTP_Handler.cpp
* @author Anil Kumar
* @date 19April2025
* @brief Header of HTTP_Handler class.
*/
#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "httplib.h"

#include <QObject>
#include "QDebug"
#include "QProcess"
#include "QOperatingSystemVersion"
#include "QFile"


class HTTP_Handler : public QObject
{
    Q_OBJECT
public:
    explicit HTTP_Handler(QObject *parent = nullptr);
    void m_Start();
    void m_StartSecure();
signals:
private:
    QByteArray m_loadResource(const QString &resourcePath);
//    httplib::Server obj_svr; // http
    std::unique_ptr<httplib::Server> obj_svr;
    const char* cert_path = "server.crt";
    const char* private_key_path = "server.key";
//    httplib::SSLServer *obj_Secure_svr;//(cert_path, private_key_path,,,); // https
    std::unique_ptr<httplib::SSLServer> obj_Secure_svr;
    void m_processRequest();
    void m_processRequest_secure();
};

#endif // HTTP_HANDLER_H
