#include "uartTXwriter.h"

uartTXwriter::uartTXwriter(
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

void uartTXwriter::getModuleDeclaration()
{
    if(m_isVerilog)
    {
        m_uartTXContext << "module uart_tx" << '\n';
        m_uartTXContext << "    #(" << '\n';
        m_uartTXContext << "        parameter DBIT = 8," << '\n';
        m_uartTXContext << "                  SB_TICK = 16" << '\n';
        m_uartTXContext << "    )" << '\n';
    }
}

void uartTXwriter::getPortLits()
{
    if(m_isVerilog)
    {
        m_uartTXContext << "    (" << '\n';
        m_uartTXContext << "    input wire clk, reset," << '\n';
        m_uartTXContext << "    input wire tx_start, s_tick," << '\n';
        m_uartTXContext << "    input wire [7:0] din," << '\n';
        m_uartTXContext << "    output reg tx_done_tick," << '\n';
        m_uartTXContext << "    output wire tx" << '\n';
        m_uartTXContext << "    );" << '\n';
    }
}


void uartTXwriter::getSignalsList()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTXContext << "    reg [1:0] state_reg, state_next;" << '\n';
        m_uartTXContext << "    reg [3:0] s_reg, s_next;" << '\n';
        m_uartTXContext << "    reg [2:0] n_reg, n_next;" << '\n';
        m_uartTXContext << "    reg [7:0] b_reg, b_next;" << '\n';
        m_uartTXContext << "    reg tx_reg, tx_next;" << '\n';
    }
}

void uartTXwriter::getFSMStates()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTXContext << "    localparam [1:0]" << '\n';
        m_uartTXContext << "    idle = 2'b00," << '\n';
        m_uartTXContext << "    start = 2'b01," << '\n';
        m_uartTXContext << "    data = 2'b10," << '\n';
        m_uartTXContext << "    stop = 2'b11;" << '\n';
    }
}

void uartTXwriter::getClockAlways()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTXContext << "    always @(posedge clk, posedge reset)" << '\n';
        m_uartTXContext << "    begin" << '\n';
        m_uartTXContext << "        if(reset)" << '\n';
        m_uartTXContext << "        begin" << '\n';
        m_uartTXContext << "            state_reg <= idle;" << '\n';
        m_uartTXContext << "            s_reg <= 0;" << '\n';
        m_uartTXContext << "            n_reg <= 0;" << '\n';
        m_uartTXContext << "            b_reg <= 0;" << '\n';
        m_uartTXContext << "            tx_reg <= 1'b1;" << '\n';
        m_uartTXContext << "        end" << '\n';
        m_uartTXContext << "        else" << '\n';
        m_uartTXContext << "        begin" << '\n';
        m_uartTXContext << "            state_reg <= state_next;" << '\n';
        m_uartTXContext << "            s_reg <= s_next;" << '\n';
        m_uartTXContext << "            n_reg <= n_next;" << '\n';
        m_uartTXContext << "            b_reg <= b_next;" << '\n';
        m_uartTXContext << "            tx_reg <= tx_next;" << '\n';
        m_uartTXContext << "        end" << '\n';
        m_uartTXContext << "    end" << '\n';
    }
}


void uartTXwriter::getFSMAlways()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTXContext << "    always @*" << '\n';
        m_uartTXContext << "    begin" << '\n';
        m_uartTXContext << "        state_next = state_reg;" << '\n';
        m_uartTXContext << "        tx_done_tick = 1'b0;" << '\n';
        m_uartTXContext << "        s_next = s_reg;" << '\n';
        m_uartTXContext << "        n_next = n_reg;" << '\n';
        m_uartTXContext << "        b_next = b_reg;" << '\n';
        m_uartTXContext << "        tx_next = tx_reg;" << '\n';
        m_uartTXContext << "        case(state_reg)" << '\n';
        m_uartTXContext << "            idle:" << '\n';
        m_uartTXContext << "                begin" << '\n';
        m_uartTXContext << "                    tx_next = 1'b1;" << '\n';
        m_uartTXContext << "                    if(tx_start)" << '\n';
        m_uartTXContext << "                    begin" << '\n';
        m_uartTXContext << "                        state_next = start;" << '\n';
        m_uartTXContext << "                        s_next = 0;" << '\n';
        m_uartTXContext << "                        b_next = din;" << '\n';
        m_uartTXContext << "                    end" << '\n';
        m_uartTXContext << "                end" << '\n';
        m_uartTXContext << "            start:" << '\n';
        m_uartTXContext << "                begin" << '\n';
        m_uartTXContext << "                    tx_next = 1'b0;" << '\n';
        m_uartTXContext << "                    if(s_tick)" << '\n';
        m_uartTXContext << "                    begin" << '\n';
        m_uartTXContext << "                        if(s_reg == 15)" << '\n';
        m_uartTXContext << "                        begin" << '\n';
        m_uartTXContext << "                            state_next = data;" << '\n';
        m_uartTXContext << "                            s_next = 0;" << '\n';
        m_uartTXContext << "                            n_next = 0;" << '\n';
        m_uartTXContext << "                        end" << '\n';
        m_uartTXContext << "                        else" << '\n';
        m_uartTXContext << "                            s_next = s_reg + 1;" << '\n';
        m_uartTXContext << "                    end" << '\n';
        m_uartTXContext << "                end" << '\n';
        m_uartTXContext << "            data:" << '\n';
        m_uartTXContext << "                begin" << '\n';
        m_uartTXContext << "                    tx_next = b_reg[0];" << '\n';
        m_uartTXContext << "                    if(s_tick)" << '\n';
        m_uartTXContext << "                    begin" << '\n';
        m_uartTXContext << "                        if(s_reg == 15)" << '\n';
        m_uartTXContext << "                        begin" << '\n';
        m_uartTXContext << "                            s_next = 0;" << '\n';
        m_uartTXContext << "                            b_next = b_reg >> 1;" << '\n';
        m_uartTXContext << "                            if(n_reg == (DBIT-1))" << '\n';
        m_uartTXContext << "                                state_next = stop;" << '\n';
        m_uartTXContext << "                            else" << '\n';
        m_uartTXContext << "                                n_next = n_reg + 1;" << '\n';
        m_uartTXContext << "                        end" << '\n';
        m_uartTXContext << "                        else" << '\n';
        m_uartTXContext << "                            s_next = s_reg + 1;" << '\n';
        m_uartTXContext << "                    end" << '\n';
        m_uartTXContext << "                end" << '\n';
        m_uartTXContext << "            stop:" << '\n';
        m_uartTXContext << "                begin" << '\n';
        m_uartTXContext << "                    tx_next = 1'b1;" << '\n';
        m_uartTXContext << "                    if(s_tick)" << '\n';
        m_uartTXContext << "                    begin" << '\n';
        m_uartTXContext << "                        if(s_reg == (SB_TICK -1 ))" << '\n';
        m_uartTXContext << "                        begin" << '\n';
        m_uartTXContext << "                            state_next = idle;" << '\n';
        m_uartTXContext << "                            tx_done_tick = 1'b1;" << '\n';
        m_uartTXContext << "                        end" << '\n';
        m_uartTXContext << "                        else" << '\n';
        m_uartTXContext << "                            s_next = s_reg + 1;" << '\n';
        m_uartTXContext << "                    end" << '\n';
        m_uartTXContext << "                end" << '\n';
        m_uartTXContext << "        endcase" << '\n';
        m_uartTXContext << "    end" << '\n';
    }
}

void uartTXwriter::getAssignments()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTXContext << "    assign tx = tx_reg;" << '\n';
    }
}

void uartTXwriter::getEndFile()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTXContext << "endmodule" << '\n';
    }
}

void uartTXwriter::addEnter()
{
    m_uartTXContext << '\n' << '\n' ;
}

void uartTXwriter::writeToFile()
{
    getModuleDeclaration();
    getPortLits();
    getFSMStates();
    getSignalsList();
    getClockAlways();
    getFSMAlways();
    getAssignments();
    getEndFile();

    std::ofstream myfile;
    myfile.open (EXPORT_FILE_PATH);
    if (myfile.is_open())
    {
        myfile << m_uartTXContext.str();
        myfile.close();
    }
}

