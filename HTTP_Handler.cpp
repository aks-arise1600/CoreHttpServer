/**
* @file HTTP_Handler.cpp
* @author Anil Kumar
* @date 19April2025
* @brief HTTP_Handler class has Responsible to start HTTP Server and Handles requests.
*/
#include "HTTP_Handler.h"

QStringList def_params = {"ADD","MULTI"};

/**
 * @brief HTTP_Handler::HTTP_Handler
 * @param parent
 */
HTTP_Handler::HTTP_Handler(QObject *parent)
    : QObject{parent}
{
    m_processRequest();
    obj_svr.listen("0.0.0.0", 8190);
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
        std::multimap<std::string, std::string> req_Params = req.params;
        QString t_Key, t_Values;
        for (const auto& [key, value] : req_Params) {
                std::cout << key << " => " << value << std::endl;
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

}
