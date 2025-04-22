/**
* @file HTTP_Handler.cpp
* @author Anil Kumar
* @date 19April2025
* @brief HTTP_Handler class has Responsible to start HTTP Server and Handles requests.
*/
#include "HTTP_Handler.h"

QString tmp_html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <style>
        .box {
            width: 300px;
            margin: 20px auto;
            padding: 20px;
            border: 2px solid #333;
            background-color: #f9f9f9;
            border-radius: 10px;
            box-shadow: 2px 2px 10px rgba(0,0,0,0.1);
            text-align: center;
        }
    </style>
</head>
<body>
    <div class="box">
        <h2>Core HTTP Server</h2>
        <h4>Version 1.0</h4>
        <p>For more use: ADD / MULTI / Details.</p>
    </div>
</body>
</html>
)";

QStringList def_params = {"ADD","MULTI"};
bool isReplied = false;


/**
 * @brief HTTP_Handler::HTTP_Handler
 * @param parent
 */
HTTP_Handler::HTTP_Handler(QObject *parent)
    : QObject{parent}
{

}

void HTTP_Handler::m_Start()
{
    qDebug()<<"Starting HTTP Server (HTTP). ";

    m_processRequest();
    obj_svr.listen("0.0.0.0", 8190);
}

void HTTP_Handler::m_StartSecure()
{
    qDebug()<<"Starting Secure HTTP Server (HTTPS). ";

    QByteArray certData = m_loadResource(":ssl/arise1600_local.crt");
    QByteArray keyData = m_loadResource(":ssl/arise1600_local.key");

    auto setup_ssl = [certData, keyData](SSL_CTX& ctx) {
        BIO* bio_cert = BIO_new_mem_buf(certData.constData(), certData.size());
        X509* cert = PEM_read_bio_X509(bio_cert, nullptr, nullptr, nullptr);
        BIO_free(bio_cert);

        BIO *bio_key = BIO_new_mem_buf(keyData.data(), keyData.size());
        EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bio_key, nullptr, nullptr, nullptr);
        BIO_free(bio_key);

        if (!cert || !pkey) {
            qCritical() << "Failed to load cert or key from memory";
            if (cert) X509_free(cert);
            if (pkey) EVP_PKEY_free(pkey);
            return false;
        }

        if (SSL_CTX_use_certificate(&ctx, cert) != 1) {
            qCritical() << "Failed to use certificate";
            X509_free(cert);
            EVP_PKEY_free(pkey);
            return false;
        }

        if (SSL_CTX_use_PrivateKey(&ctx, pkey) != 1) {
            qCritical() << "Failed to use private key";
            X509_free(cert);
            EVP_PKEY_free(pkey);
            return false;
        }

        X509_free(cert);
        EVP_PKEY_free(pkey);
        return true;
    };

    obj_Secure_svr = std::make_unique<httplib::SSLServer>(setup_ssl);
    m_processRequest_secure();
    obj_Secure_svr->listen("0.0.0.0", 8193);

}

QByteArray HTTP_Handler::m_loadResource(const QString& resourcePath)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to load resource:" << resourcePath;
        return QByteArray();
    }
    return file.readAll();
}

/**
 * @brief m_ResultRequestedData
 * @param indx
 * @param isExist
 * @param tt_Values
 * @return
 */
QString m_ResultRequestedData(int indx, bool isExist, QString tt_Values)
{
    QString r_send_Data;

    if(isExist)
    {
        switch (indx)
        {
        case 0:
        {
            if(tt_Values.size())
            {
                QStringList Number_data;
                if(tt_Values.contains(" "))
                    Number_data = tt_Values.split(' ');
                else if(tt_Values.contains(","))
                    Number_data = tt_Values.split(',');
                else if(tt_Values.contains("+"))
                    Number_data = tt_Values.split('+');

                int sum = 0;
                foreach(QString n_data, Number_data)
                    sum += n_data.toInt();
                r_send_Data = "<mData><msg>REQUEST OK</msg><Result>"+QString::number(sum)+"</Result></mData>";
            }
            else
                r_send_Data = "<mData><msg>REQUEST OK</msg><Result>NA</Result></mData>";
        }
            break;
        case 1:
        {
            if(tt_Values.size())
            {
                QStringList Num_data;
                if(tt_Values.contains(" "))
                    Num_data = tt_Values.split(' ');
                else if(tt_Values.contains(","))
                    Num_data = tt_Values.split(',');
                else if(tt_Values.contains("*"))
                    Num_data = tt_Values.split('*');

                int multi = 1;
                foreach(QString n_data, Num_data)
                    multi *= n_data.toInt();
                r_send_Data = "<mData><msg>REQUEST OK</msg><Result>"+QString::number(multi)+"</Result></mData>";
            }
            else
                r_send_Data = "<mData><msg>REQUEST OK</msg><Result>NA</Result></mData>";
        }
            break;
        }
    }
    else
        r_send_Data = "<mData><msg>REQUEST FAILED</msg><Details>Request not found.\nError 501 !!</Details></mData>";

    return r_send_Data;
}

/**
 * @brief HTTP_Handler::m_processRequest
 */
void HTTP_Handler::m_processRequest()
{
    obj_svr.Get("/", [](const httplib::Request& req, httplib::Response& res)
    {
        res.set_content(tmp_html.toStdString(),"text/html");
    });


    obj_svr.Get("/maths", [](const httplib::Request& req, httplib::Response& res)
    {
        std::multimap<std::string, std::string> req_Params = req.params;
        QString t_Key, t_Values;
        for (const auto& [key, value] : req_Params) {
                t_Key = QString::fromStdString(key);
                t_Values = QString::fromStdString(value);
                qDebug()<< t_Key <<" -> " <<t_Values;
            }

        bool isExist = false;
        int indx_param = 0, found_param_index = -1;
        foreach(QString tmpParam, def_params)
        {
            if(req.has_param(tmpParam.toStdString()))
            {
                isExist = true;
                found_param_index = indx_param;
            }

            indx_param ++;
        }

        qDebug()<< "found_param_index ="<< found_param_index << ",isExist "<<isExist;

        QString str_response = m_ResultRequestedData(found_param_index, isExist, t_Values);
        res.set_content(str_response.toStdString(),"text/xml");
    });

    obj_svr.Get("/Details", [](const httplib::Request& req, httplib::Response& res)
    {
        Q_UNUSED(req)
        QProcess *objProcess = new QProcess;
        auto os = QOperatingSystemVersion::currentType();
        QByteArray res_data;
        QString cmd;
        QStringList args;

        switch (os)
        {
        case QOperatingSystemVersion::Windows:
            qDebug() << "Runtime OS: Windows";
            cmd = "systeminfo";
            objProcess->start(cmd,args);
            objProcess->waitForFinished();
            res_data = objProcess->readAllStandardOutput();
            res.set_content(res_data.toStdString(),"text/plain");
            break;
        default:
            qDebug() << "Runtime OS: LINUX";
            cmd = "lshw";
            args << "-html";
            objProcess->start(cmd,args);
            objProcess->waitForFinished();
            res_data = objProcess->readAllStandardOutput();
            res.set_content(res_data.toStdString(),"text/html");
            break;
        }

        delete objProcess;
    });

}

void HTTP_Handler::m_processRequest_secure()
{
    obj_Secure_svr->Get("/", [](const httplib::Request& req, httplib::Response& res)
    {
        res.set_content(tmp_html.replace("HTTP ","HTTPS ").toStdString(),"text/html");
    });


    obj_Secure_svr->Get("/maths", [](const httplib::Request& req, httplib::Response& res)
    {
        std::multimap<std::string, std::string> req_Params = req.params;
        QString t_Key, t_Values;
        for (const auto& [key, value] : req_Params) {
                t_Key = QString::fromStdString(key);
                t_Values = QString::fromStdString(value);
                qDebug()<< t_Key <<" -> " <<t_Values;
            }

        bool isExist = false;
        int indx_param = 0, found_param_index = -1;
        foreach(QString tmpParam, def_params)
        {
            if(req.has_param(tmpParam.toStdString()))
            {
                isExist = true;
                found_param_index = indx_param;
            }

            indx_param ++;
        }

        qDebug()<< "found_param_index ="<< found_param_index << ",isExist "<<isExist;

        QString str_response = m_ResultRequestedData(found_param_index, isExist, t_Values);
        res.set_content(str_response.toStdString(),"text/xml");
    });

    obj_Secure_svr->Get("/Details", [](const httplib::Request& req, httplib::Response& res)
    {
        Q_UNUSED(req)
        QProcess *objProcess = new QProcess;
        auto os = QOperatingSystemVersion::currentType();
        QByteArray res_data;
        QString cmd;
        QStringList args;

        switch (os)
        {
        case QOperatingSystemVersion::Windows:
            qDebug() << "Runtime OS: Windows";
            cmd = "systeminfo";
            objProcess->start(cmd,args);
            objProcess->waitForFinished();
            res_data = objProcess->readAllStandardOutput();
            res.set_content(res_data.toStdString(),"text/plain");
            break;
        default:
            qDebug() << "Runtime OS: LINUX";
            cmd = "lshw";
            args << "-html";
            objProcess->start(cmd,args);
            objProcess->waitForFinished();
            res_data = objProcess->readAllStandardOutput();
            res.set_content(res_data.toStdString(),"text/html");
            break;
        }

        delete objProcess;
    });

}
