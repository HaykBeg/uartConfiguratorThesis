#ifndef COUNTERWRITER_H
#define COUNTERWRITER_H

#include <QObject>
#include <QWidget>

#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

class counterWriter
{
    const std::string VERILOG_FILE_NAME = "counter.v";
    const std::string VHDL_FILE_NAME = "counter.vhdl";
    std::string EXPORT_FILE_PATH = "/home/hayk_b/Documents/ysu/master_thesis/counter.v";


public:
    counterWriter(
            bool isVerilog = true,
            std::string sFilePath = "");
    ~counterWriter();

    void writeToFile();


private:
    void getPortLits();
    void getSignalsList();
    void getClockAlways();
    void getModuleDeclaration();
    void getAssignments();
    void getEndFile();

    void addEnter();

    std::stringstream m_counterContext;
    bool m_isVerilog;
};

#endif // COUNTERWRITER_H
