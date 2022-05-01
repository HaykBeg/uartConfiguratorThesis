#include "uartRXwriter.h"

uartRXwriter::uartRXwriter(
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

void uartRXwriter::getModuleDeclaration()
{
    if(m_isVerilog)
    {
        m_uartRXContext << "module uart_rx" << '\n';
        m_uartRXContext << "    #(" << '\n';
        m_uartRXContext << "        parameter DBIT = 8," << '\n';
        m_uartRXContext << "                  SB_TICK = 16" << '\n';
        m_uartRXContext << "    )" << '\n';
    }
}

void uartRXwriter::getPortLits()
{
    if(m_isVerilog)
    {
        m_uartRXContext << "    (" << '\n';
        m_uartRXContext << "    input wire clk, reset," << '\n';
        m_uartRXContext << "    input wire rx, s_tick," << '\n';
        m_uartRXContext << "    output reg rx_done_tick," << '\n';
        m_uartRXContext << "    output wire [7:0] dout" << '\n';
        m_uartRXContext << "    );" << '\n';
    }
}

void uartRXwriter::getSignalsList()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartRXContext << "    reg [1:0] state_reg, state_next;" << '\n';
        m_uartRXContext << "    reg [3:0] s_reg, s_next;" << '\n';
        m_uartRXContext << "    reg [2:0] n_reg, n_next;" << '\n';
        m_uartRXContext << "    reg [7:0] b_reg, b_next;" << '\n';
    }
}

void uartRXwriter::getFSMStates()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartRXContext << "    localparam [1:0]" << '\n';
        m_uartRXContext << "        idle = 2'b00," << '\n';
        m_uartRXContext << "        start = 2'b01," << '\n';
        m_uartRXContext << "        data = 2'b10," << '\n';
        m_uartRXContext << "        stop = 2'b11;" << '\n';
    }
}

void uartRXwriter::getClockAlways()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartRXContext << "    always @(posedge clk, posedge reset)" << '\n';
        m_uartRXContext << "    begin" << '\n';
        m_uartRXContext << "        if(reset)" << '\n';
        m_uartRXContext << "        begin" << '\n';
        m_uartRXContext << "            state_reg <= idle;" << '\n';
        m_uartRXContext << "            s_reg <= 0;" << '\n';
        m_uartRXContext << "            n_reg <= 0;" << '\n';
        m_uartRXContext << "            b_reg <= 0;" << '\n';
        m_uartRXContext << "        end" << '\n';
        m_uartRXContext << "        else" << '\n';
        m_uartRXContext << "            begin" << '\n';
        m_uartRXContext << "                state_reg <= state_next;" << '\n';
        m_uartRXContext << "                s_reg <= s_next;" << '\n';
        m_uartRXContext << "                n_reg <= n_next;" << '\n';
        m_uartRXContext << "                b_reg <= b_next;" << '\n';
        m_uartRXContext << "            end" << '\n';
        m_uartRXContext << "    end" << '\n';

    }
}

void uartRXwriter::getFSMAlways()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartRXContext << "    always @*" << '\n';
        m_uartRXContext << "    begin" << '\n';
        m_uartRXContext << "        state_next = state_reg;" << '\n';
        m_uartRXContext << "        rx_done_tick = 1'b0;" << '\n';
        m_uartRXContext << "        s_next = s_reg;" << '\n';
        m_uartRXContext << "        n_next = n_reg;" << '\n';
        m_uartRXContext << "        b_next = b_reg;" << '\n';
        m_uartRXContext << "        case (state_reg)" << '\n';
        m_uartRXContext << "            idle:" << '\n';
        m_uartRXContext << "                if (~rx)" << '\n';
        m_uartRXContext << "                begin" << '\n';
        m_uartRXContext << "                    state_next = start;" << '\n';
        m_uartRXContext << "                    s_next = 0;" << '\n';
        m_uartRXContext << "                end" << '\n';
        m_uartRXContext << "            start:" << '\n';
        m_uartRXContext << "                if (s_tick)" << '\n';
        m_uartRXContext << "                    if (s_reg == 7)" << '\n';
        m_uartRXContext << "                    begin" << '\n';
        m_uartRXContext << "                        state_next = data;" << '\n';
        m_uartRXContext << "                        s_next = 0;" << '\n';
        m_uartRXContext << "                        n_next = 0;" << '\n';
        m_uartRXContext << "                    end" << '\n';
        m_uartRXContext << "                else" << '\n';
        m_uartRXContext << "                    s_next = s_reg + 1;" << '\n';
        m_uartRXContext << "            data:" << '\n';
        m_uartRXContext << "                if (s_tick)" << '\n';
        m_uartRXContext << "                    if(s_reg == 15)" << '\n';
        m_uartRXContext << "                    begin" << '\n';
        m_uartRXContext << "                        s_next = 0;" << '\n';
        m_uartRXContext << "                        b_next = {rx, b_reg[7:1]};" << '\n';
        m_uartRXContext << "                        if (n_reg == (DBIT-1))" << '\n';
        m_uartRXContext << "                            state_next = stop;" << '\n';
        m_uartRXContext << "                        else" << '\n';
        m_uartRXContext << "                            n_next = n_reg + 1;" << '\n';
        m_uartRXContext << "                    end" << '\n';
        m_uartRXContext << "                    else" << '\n';
        m_uartRXContext << "                        s_next = s_reg + 1;" << '\n';
        m_uartRXContext << "            stop:" << '\n';
        m_uartRXContext << "                if (s_tick)" << '\n';
        m_uartRXContext << "                    if ( s_reg == (SB_TICK-1))" << '\n';
        m_uartRXContext << "                    begin" << '\n';
        m_uartRXContext << "                        state_next = idle;" << '\n';
        m_uartRXContext << "                        rx_done_tick = 1'b1;" << '\n';
        m_uartRXContext << "                    end" << '\n';
        m_uartRXContext << "                    else" << '\n';
        m_uartRXContext << "                        s_next = s_reg + 1;" << '\n';
        m_uartRXContext << "            endcase" << '\n';
        m_uartRXContext << "                 " << '\n';   
        m_uartRXContext << "    end" << '\n';
    }
}

void uartRXwriter::getAssignments()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartRXContext << "   assign dout = b_reg;" << '\n';
    }
}

void uartRXwriter::getEndFile()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartRXContext << "endmodule" << '\n';
    }
}

void uartRXwriter::addEnter()
{
    m_uartRXContext << '\n' << '\n' ;
}

void uartRXwriter::writeToFile()
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
        myfile << m_uartRXContext.str();
        myfile.close();
    }

}

