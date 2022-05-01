#ifndef FIFOWRITER_H
#define FIFOWRITER_H

#include <QObject>
#include <QWidget>

#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

class fifoWriter
{
    const std::string VERILOG_FILE_NAME = "fifo.v";
    const std::string VHDL_FILE_NAME = "fifo.vhdl";
    std::string EXPORT_FILE_PATH = "/home/hayk_b/Documents/ysu/master_thesis/fifo.v";

public:
    fifoWriter(
            bool isVerilog = true,
            std::string sFilePath = "");

    void writeToFile();

private:
    void getPortLits();
    void getSignalsList();
    void getModuleDeclaration();
    void getAssignments();
    void getEndFile();

    void getFifoCtrl();
    void getFifoWrite();
    void getFifoFSM();

    void addEnter();

    std::stringstream m_fifoContext;
    bool m_isVerilog;

};

#endif // FIFOWRITER_H
