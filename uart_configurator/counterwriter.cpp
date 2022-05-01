#include "counterwriter.h"

counterWriter::counterWriter(
        bool isVerilog,
        std::string sFilePath)
{
    m_isVerilog = isVerilog;

    if(isVerilog)
    {
        EXPORT_FILE_PATH = sFilePath + "/" + VERILOG_FILE_NAME;
    }
    else
    {
        EXPORT_FILE_PATH = sFilePath + "/" + VHDL_FILE_NAME;
    }
}

void counterWriter::getModuleDeclaration()
{
    if(m_isVerilog)
    {
        m_counterContext << "module mod_m_counter" << '\n';
        m_counterContext << "    #(" << '\n';
        m_counterContext << "        parameter N=4,  //number of bits" << '\n';
        m_counterContext << "                  M=10  //mod-M" << '\n';
        m_counterContext << "    )" << '\n';
    }
}

void counterWriter::addEnter()
{
    m_counterContext << '\n' << '\n' ;
}

void counterWriter::getPortLits()
{
    if(m_isVerilog)
    {
        m_counterContext << "    (" << '\n';
        m_counterContext << "        input wire clk, reset," << '\n';
        m_counterContext << "        output wire max_tick," << '\n';
        m_counterContext << "        output wire [N-1:0] q" << '\n';
        m_counterContext << "    );" << '\n';
    }
}

void counterWriter::getSignalsList()
{
    addEnter();
    if(m_isVerilog)
    {
        m_counterContext << "    reg [N-1:0] r_reg " << '\n';
        m_counterContext << "    wire [N-1:0] r_next;" << '\n';
    }
}

void counterWriter::getClockAlways()
{
    addEnter();
    if(m_isVerilog)
    {
        m_counterContext << "    always @(posedge clk, posedge reset)" << '\n';
        m_counterContext << "    begin" << '\n';
        m_counterContext << "       if(reset)" << '\n';
        m_counterContext << "           r_reg <= 0;" << '\n';
        m_counterContext << "       else" << '\n';
        m_counterContext << "           r_reg <= r_next;" << '\n';
        m_counterContext << "    end" << '\n';
    }

}

void counterWriter::getAssignments()
{
    addEnter();
    if(m_isVerilog)
    {
        m_counterContext << "    //next state" << '\n';
        m_counterContext << "    assign r_next = (r_reg==(M-1)) ? 0 : r_reg + 1;" << '\n';
        addEnter();
        m_counterContext << "    //output" << '\n';
        m_counterContext << "    assign q = r_reg;" << '\n';
        m_counterContext << "    assign max_tick = (r_reg == (M-1)) ? 1'b1 : 1'b0;" << '\n';
    }
    
}

void counterWriter::getEndFile()
{
    addEnter();
    if(m_isVerilog)
    {
        m_counterContext << "endmodule" << '\n';
    }
}


void counterWriter::writeToFile()
{
    m_counterContext.str(std::string());
    getModuleDeclaration();
    getPortLits();
    getSignalsList();
    getClockAlways();
    getAssignments();
    getEndFile();

    std::ofstream myfile;
    myfile.open (EXPORT_FILE_PATH);
    if (myfile.is_open())
    {
        myfile << m_counterContext.str();
        myfile.close();
    }
}
