#include "Main_Window.h"

#include <QApplication>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main_Window w;
    w.show();

//    double num = 0.15;
//    string str = to_string(num);
//    cout << str << endl;


//    string str = "";
//    double num = stod(str);
//    cout << "0: "<< num << endl;

    return a.exec();
}
