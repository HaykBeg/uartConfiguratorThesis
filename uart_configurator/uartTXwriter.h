#ifndef UARTTXWRITER_H
#define UARTTXWRITER_H

#include <QObject>
#include <QWidget>

#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

class uartTXwriter
{
    const std::string VERILOG_FILE_NAME = "uart_tx.v";
    const std::string VHDL_FILE_NAME = "uart_tx.vhdl";
    std::string EXPORT_FILE_PATH = "/home/hayk_b/Documents/ysu/master_thesis/uart_tx.v";

public:
    uartTXwriter(
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

    std::stringstream m_uartTXContext;
    bool m_isVerilog;

};

#endif // UARTTXWRITER_H
