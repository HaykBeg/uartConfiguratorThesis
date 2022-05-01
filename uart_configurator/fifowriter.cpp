#include "fifowriter.h"

fifoWriter::fifoWriter(
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

void fifoWriter::getModuleDeclaration()
{
    if(m_isVerilog)
    {
        m_fifoContext <<"module fifo" << '\n';
        m_fifoContext <<"  #(" << '\n';
        m_fifoContext <<"      parameter B=8, // number of bits in one word" << '\n';
        m_fifoContext <<"                W=4  // number of address bits" << '\n';
        m_fifoContext <<"  )" << '\n';
    }
}

void fifoWriter::addEnter()
{
    m_fifoContext << '\n' << '\n' ;
}

void fifoWriter::getPortLits()
{
    if(m_isVerilog)
    {
       m_fifoContext <<"     (" << '\n';
       m_fifoContext <<"         input wire clk, reset," << '\n';
       m_fifoContext <<"         input wire rd, wr," << '\n';
       m_fifoContext <<"         input wire [B-1:0] w_data," << '\n';
       m_fifoContext <<"         output wire empty, full," << '\n';
       m_fifoContext <<"         output wire [B-1:0] r_data" << '\n';
       m_fifoContext <<"     );" << '\n';
    }
}

void fifoWriter::getSignalsList()
{
    addEnter();
    if(m_isVerilog)
    {
        m_fifoContext<< "    //signals"  << '\n';
        m_fifoContext<< "   reg [B-1:0] array_reg [2**W-1:0]; //register array"  << '\n';
        m_fifoContext<< "   reg [W-1:0] w_ptr_reg, w_ptr_next, w_ptr_succ;"  << '\n';
        m_fifoContext<< "   reg [W-1:0] r_ptr_reg, r_ptr_next, r_ptr_succ;"  << '\n';
        m_fifoContext<< "   reg full_reg, empty_reg, full_next, empty_next;"  << '\n';
        m_fifoContext<< "   wire wr_en;"  << '\n';
    }
}

void fifoWriter::getFifoWrite()
{
    addEnter();
    if(m_isVerilog)
    {
        m_fifoContext <<"     //file write " << '\n';
        m_fifoContext <<"     always @(posedge clk)" << '\n';
        m_fifoContext <<"     begin" << '\n';
        m_fifoContext <<"         if(wr_en)" << '\n';
        m_fifoContext <<"             array_reg[w_ptr_reg] <= w_data;" << '\n';
        m_fifoContext <<"     end" << '\n';
        addEnter();
        m_fifoContext << "    //file read" << '\n';
        m_fifoContext << "    assign r_data = array_reg[r_ptr_reg];" << '\n';
        m_fifoContext << "    //enable write / fifo full" << '\n';
        m_fifoContext << "    assign wr_en = wr & ~full_reg;" << '\n';
    }
}

void fifoWriter::getFifoCtrl()
{
    addEnter();
    if(m_isVerilog)
    {
        m_fifoContext <<"        //fifo ctrl" << '\n';
        m_fifoContext <<"    always @(posedge clk, posedge reset)" << '\n';
        m_fifoContext <<"    begin" << '\n';
        m_fifoContext <<"        if(reset)" << '\n';
        m_fifoContext <<"        begin" << '\n';
        m_fifoContext <<"            w_ptr_reg <= 0;" << '\n';
        m_fifoContext <<"            r_ptr_reg <= 0;" << '\n';
        m_fifoContext <<"            full_reg <= 1'b0;" << '\n';
        m_fifoContext <<"            empty_reg <= 1'b1;" << '\n';
        m_fifoContext <<"        end" << '\n';
        m_fifoContext <<"        else" << '\n';
        m_fifoContext <<"        begin" << '\n';
        m_fifoContext <<"            w_ptr_reg <= w_ptr_next;" << '\n';
        m_fifoContext <<"            r_ptr_reg <= r_ptr_next;" << '\n';
        m_fifoContext <<"            full_reg <= full_next;" << '\n';
        m_fifoContext <<"            empty_reg <= empty_next;" << '\n';
        m_fifoContext <<"        end" << '\n';
        m_fifoContext <<"    end" << '\n';
    }
}

void fifoWriter::getFifoFSM()
{
    addEnter();
    if(m_isVerilog)
    {
        m_fifoContext <<"    always @*" << '\n';
        m_fifoContext <<"    begin" << '\n';
        m_fifoContext <<"        w_ptr_succ = w_ptr_reg + 1;" << '\n';
        m_fifoContext <<"        r_ptr_succ = r_ptr_reg + 1;" << '\n';
        m_fifoContext <<"        " << '\n';
        m_fifoContext <<"        w_ptr_next = w_ptr_reg;" << '\n';
        m_fifoContext <<"        r_ptr_next = r_ptr_reg;" << '\n';
        m_fifoContext <<"        full_next = full_reg;" << '\n';
        m_fifoContext <<"        empty_next = empty_reg;" << '\n';
        m_fifoContext <<"        " << '\n';
        m_fifoContext <<"        case ({wr, rd})" << '\n';
        m_fifoContext <<"            //2'b00 no op" << '\n';
        m_fifoContext <<"            2'b01: //read" << '\n';
        m_fifoContext <<"                if(~empty_reg) //not empty" << '\n';
        m_fifoContext <<"                begin" << '\n';
        m_fifoContext <<"                    r_ptr_next = r_ptr_succ;" << '\n';
        m_fifoContext <<"                    full_next = 1'b0;" << '\n';
        m_fifoContext <<"                    if( r_ptr_succ == w_ptr_reg )" << '\n';
        m_fifoContext <<"                        empty_next = 1'b1;" << '\n';
        m_fifoContext <<"                end" << '\n';
        m_fifoContext <<"            2'b10: //write" << '\n';
        m_fifoContext <<"                if(~full_reg) //not full" << '\n';
        m_fifoContext <<"                begin" << '\n';
        m_fifoContext <<"                    w_ptr_next = w_ptr_succ;" << '\n';
        m_fifoContext <<"                    empty_next = 1'b0;" << '\n';
        m_fifoContext <<"                    if(w_ptr_succ == r_ptr_reg)" << '\n';
        m_fifoContext <<"                        full_next = 1'b1;" << '\n';
        m_fifoContext <<"                end" << '\n';
        m_fifoContext <<"            2'b11: //write and read" << '\n';
        m_fifoContext <<"                begin" << '\n';
        m_fifoContext <<"                    w_ptr_next = w_ptr_succ;" << '\n';
        m_fifoContext <<"                    r_ptr_next = r_ptr_succ;" << '\n';
        m_fifoContext <<"                end" << '\n';
        m_fifoContext <<"        endcase" << '\n';
        m_fifoContext <<"    end" << '\n';
    }
}

void fifoWriter::getAssignments()
{
    addEnter();
    if(m_isVerilog)
    {
        m_fifoContext <<"    assign full = full_reg;" << '\n';
        m_fifoContext <<"    assign empty = empty_reg;" << '\n';
    }
}

void fifoWriter::getEndFile()
{
    addEnter();
    if(m_isVerilog)
    {
        m_fifoContext << "endmodule" << '\n';
    }
}

void fifoWriter::writeToFile()
{
    m_fifoContext.str(std::string());
    getModuleDeclaration();
    getPortLits();
    getSignalsList();
    getFifoWrite();
    getFifoCtrl();
    getFifoFSM();
    getAssignments();
    getEndFile();

    std::ofstream myfile;
    myfile.open (EXPORT_FILE_PATH);
    if (myfile.is_open())
    {
        myfile << m_fifoContext.str();
        myfile.close();
    }
}
