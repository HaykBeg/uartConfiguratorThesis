#ifndef UARTTOPWRITER_H
#define UARTTOPWRITER_H

#include <QObject>
#include <QWidget>

#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

class uartTOPwriter
{
    const std::string VERILOG_FILE_NAME = "uart_top.v";
    const std::string VHDL_FILE_NAME = "uart_top.vhdl";
    std::string EXPORT_FILE_PATH = "/home/hayk_b/Documents/ysu/master_thesis/uart_top.v";

public:
    uartTOPwriter(
            bool isVerilog = true,
            std::string sDbit = "8",
            std::string sSbTick = "16",
            std::string sDvsr = "163",
            std::string sDvsrBit = "8",
            std::string sFifoW = "2",
            std::string sFilePath = "");

    void writeToFile();

private:
    void getModuleDeclaration();
    void getPortLits();
    void getSignalsList();
    void getAssignments();
    void getEndFile();

    void getInstantiations();

    void addEnter();

    std::stringstream m_uartTOPContext;

    std::string m_DBIT;
    std::string m_SB_TICK;
    std::string m_DVSR;
    std::string m_DVSR_BIT;
    std::string m_FIFO_W;
    bool m_isVerilog = true;

};

#endif // UARTTOPWRITER_H
