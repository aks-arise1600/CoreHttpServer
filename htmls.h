#ifndef HTMLS_H
#define HTMLS_H
#include "QStringList"
#include <QDateTime>


#define PRINT_D(func_)          QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+func_+" => "

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
        <p>For more use: ADD / MULTI / Details / Warning.</p>
    </div>
</body>
</html>
)";

QString warning_html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Warnings and Alerts </title>
  <style>
    .warning-box {
      width: 600px;
      margin: 30px auto;
      background-color: red;
      color: white;
      padding: 20px;
      border: 2px solid darkred;
      box-shadow: 2px 2px 10px rgba(0,0,0,0.1);
      text-align: center;
      font-weight: bold;
      font-size: 1.5em;
      animation: blink 1.5s infinite;
    }

    @keyframes blink {
      0%   { opacity: 1; }
      50%  { opacity: 0; }
      100% { opacity: 1; }
    }
  </style>
</head>
<body>
  <div class="warning-box">
    ⚠ WARNING: GEN_MSG!
  </div>
</body>
</html>
)";

QStringList def_params = {"ADD","MULTI"};
bool isReplied = false;



#endif // HTMLS_H
