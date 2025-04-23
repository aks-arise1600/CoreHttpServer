/**
* @file HTTP_Handler.cpp
* @author Anil Kumar
* @date 19April2025
* @brief HTTP_Handler class has Responsible to start HTTP Server and Handles requests.
*/
#include "HTTP_Handler.h"
#include "htmls.h"

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

    int port = 8080;
    const char* env_p = std::getenv("PORT");
    if (env_p) port = std::stoi(env_p);
    std::cout << "Listening on port: " << port << std::endl;


    //obj_svr = std::unique_ptr<httplib::Server>();
    m_processRequest();
    obj_svr.listen("0.0.0.0", port);
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

    int port = 8443;
    const char* env_p = std::getenv("PORTS");
    if (env_p) port = std::stoi(env_p);
    std::cout << "Listening on port: " << port << std::endl;


    obj_Secure_svr = std::make_unique<httplib::SSLServer>(setup_ssl);
    m_processRequest_secure();
    obj_Secure_svr->listen("0.0.0.0", port);

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
        Q_UNUSED(req)
        qDebug() <<PRINT_D("m_processRequest") <<"root requests ---";
        QString client_ip = QString::fromStdString(req.remote_addr);
        QString user_agent = QString::fromStdString (req.get_header_value("User-Agent"));

        qDebug() <<PRINT_D("m_processRequest_secure") <<"client_ip = "<<client_ip;
        qDebug() <<PRINT_D("m_processRequest_secure") <<"user_agent = "<<user_agent;

        res.set_content(tmp_html.toStdString(),"text/html");
    });


    obj_svr.Get("/maths", [](const httplib::Request& req, httplib::Response& res)
    {
        qDebug() <<PRINT_D("m_processRequest") <<"maths requests ---";
        std::multimap<std::string, std::string> req_Params = req.params;
        QString t_Key, t_Values;
        for (const auto& [key, value] : req_Params)
        {
            t_Key = QString::fromStdString(key);
            t_Values = QString::fromStdString(value);
            qDebug() <<PRINT_D("m_processRequest")<< t_Key <<" -> " <<t_Values;
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

        qDebug() <<PRINT_D("m_processRequest")<< "found_param_index ="<< found_param_index << ",isExist "<<isExist;

        QString str_response = m_ResultRequestedData(found_param_index, isExist, t_Values);
        res.set_content(str_response.toStdString(),"text/xml");
    });

    obj_svr.Get("/Details", [](const httplib::Request& req, httplib::Response& res)
    {
        qDebug() <<PRINT_D("m_processRequest") <<"Details requests ---";

        Q_UNUSED(req)
        QProcess *objProcess = new QProcess;
        auto os = QOperatingSystemVersion::currentType();
        QByteArray res_data;
        QString cmd;
        QStringList args;

        switch (os)
        {
        case QOperatingSystemVersion::Windows:
            qDebug() <<PRINT_D("m_processRequest") << "Runtime OS: Windows";
            cmd = "systeminfo";
            objProcess->start(cmd,args);
            objProcess->waitForFinished();
            res_data = objProcess->readAllStandardOutput();
            res.set_content(res_data.toStdString(),"text/plain");
            break;
        default:
            qDebug() <<PRINT_D("m_processRequest") << "Runtime OS: LINUX";
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

    obj_svr.Get("/Warning", [](const httplib::Request& req, httplib::Response& res)
    {
        qDebug() <<PRINT_D("m_processRequest") <<"warning requests ---";
        std::multimap<std::string, std::string> req_Params = req.params;
        QString t_Key, t_Values;
        for (const auto& [key, value] : req_Params)
        {
            t_Key = QString::fromStdString(key);
            t_Values = QString::fromStdString(value);
            qDebug() <<PRINT_D("m_processRequest") << t_Key <<" -> " <<t_Values;
        }

        QString str_response = warning_html;
        if(req.has_param("WRNG_ID"))
        {
            switch (t_Values.toInt())
            {
            case 1:
                str_response.replace("GEN_MSG","Memory leak detected");
                break;
            case 2:
                str_response.replace("GEN_MSG","System overload detected");
                break;
            case 3:
                str_response.replace("GEN_MSG","System Overheating");
                break;
            default:
                str_response.replace("GEN_MSG","unknown");
                break;
            }
            res.set_content(str_response.toStdString(),"text/html");
        }
        else
        {
            str_response = m_ResultRequestedData(-1, false, t_Values);
            res.set_content(str_response.toStdString(),"text/xml");
        }
    });

}

void HTTP_Handler::m_processRequest_secure()
{
    obj_Secure_svr->Get("/", [](const httplib::Request& req, httplib::Response& res)
    {
        qDebug() <<PRINT_D("m_processRequest_secure") <<"root requests ---";
        QString client_ip = QString::fromStdString(req.remote_addr);
        QString user_agent = QString::fromStdString (req.get_header_value("User-Agent"));

        qDebug() <<PRINT_D("m_processRequest_secure") <<"client_ip = "<<client_ip;
        qDebug() <<PRINT_D("m_processRequest_secure") <<"user_agent = "<<user_agent;

        res.set_content(tmp_html.replace("HTTP ","HTTPS ").toStdString(),"text/html");
    });


    obj_Secure_svr->Get("/maths", [](const httplib::Request& req, httplib::Response& res)
    {
        qDebug() <<PRINT_D("m_processRequest_secure") <<"maths requests ---";
        std::multimap<std::string, std::string> req_Params = req.params;
        QString t_Key, t_Values;
        for (const auto& [key, value] : req_Params)
        {
            t_Key = QString::fromStdString(key);
            t_Values = QString::fromStdString(value);
            qDebug() <<PRINT_D("m_processRequest_secure")<< t_Key <<" -> " <<t_Values;
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

        qDebug() <<PRINT_D("m_processRequest_secure")<< "found_param_index ="<< found_param_index << ",isExist "<<isExist;

        QString str_response = m_ResultRequestedData(found_param_index, isExist, t_Values);
        res.set_content(str_response.toStdString(),"text/xml");
    });

    obj_Secure_svr->Get("/Details", [](const httplib::Request& req, httplib::Response& res)
    {
        qDebug() <<PRINT_D("m_processRequest_secure") <<"Details requests ---";

        Q_UNUSED(req)
        QProcess *objProcess = new QProcess;
        auto os = QOperatingSystemVersion::currentType();
        QByteArray res_data;
        QString cmd;
        QStringList args;

        switch (os)
        {
        case QOperatingSystemVersion::Windows:
            qDebug() <<PRINT_D("m_processRequest_secure") << "Runtime OS: Windows";
            cmd = "systeminfo";
            objProcess->start(cmd,args);
            objProcess->waitForFinished();
            res_data = objProcess->readAllStandardOutput();
            res.set_content(res_data.toStdString(),"text/plain");
            break;
        default:
            qDebug() <<PRINT_D("m_processRequest_secure") << "Runtime OS: LINUX";
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

    obj_Secure_svr->Get("/Warning", [](const httplib::Request& req, httplib::Response& res)
    {
        qDebug() <<PRINT_D("m_processRequest_secure") <<"warning requests ---";
        std::multimap<std::string, std::string> req_Params = req.params;
        QString t_Key, t_Values;
        for (const auto& [key, value] : req_Params)
        {
            t_Key = QString::fromStdString(key);
            t_Values = QString::fromStdString(value);
            qDebug() <<PRINT_D("m_processRequest_secure") << t_Key <<" -> " <<t_Values;
        }

        QString str_response = warning_html;
        if(req.has_param("WRNG_ID"))
        {
            switch (t_Values.toInt())
            {
            case 1:
                str_response.replace("GEN_MSG","Memory leak detected");
                break;
            case 2:
                str_response.replace("GEN_MSG","System overload detected");
                break;
            case 3:
                str_response.replace("GEN_MSG","System Overheating");
                break;
            default:
                str_response.replace("GEN_MSG","unknown");
                break;
            }
            res.set_content(str_response.toStdString(),"text/html");
        }
        else
        {
            str_response = m_ResultRequestedData(-1, false, t_Values);
            res.set_content(str_response.toStdString(),"text/xml");
        }
    });
}
