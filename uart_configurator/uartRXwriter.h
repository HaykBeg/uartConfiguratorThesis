#ifndef UARTRXWRITER_H
#define UARTRXWRITER_H

#include <QObject>
#include <QWidget>

#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

class uartRXwriter
{
    const std::string VERILOG_FILE_NAME = "uart_rx.v";
    const std::string VHDL_FILE_NAME = "uart_rx.vhdl";
    std::string EXPORT_FILE_PATH = "/home/hayk_b/Documents/ysu/master_thesis/uart_rx.v";

public:
    uartRXwriter(
            bool isVerilog = true,
            std::string sFilePath = "");

    void writeToFile();

private:
    void getModuleDeclaration();
    void getPortLits();
    void getSignalsList();
    void getAssignments();
    void getEndFile();

    void getClockAlways();
    void getFSMAlways();

    void getFSMStates();


    void addEnter();

    std::stringstream m_uartRXContext;
    bool m_isVerilog;
};

#endif // UARTRXWRITER_H
