/**
* @file main.cpp
* @author Anil Kumar
* @date 18April2025
* @brief main class.
*/
#include <iostream>
#include "HTTP_Handler.h"

using namespace std;

int main()
{
    qDebug() << "STARTING HTTP SERVER !";
    HTTP_Handler obj;
    obj.m_Start();
//    obj.m_StartSecure();

    return 0;
}
