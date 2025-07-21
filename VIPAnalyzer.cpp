
#include "./containers/Array.h"          // Make dynamic array class Array available
#include "./containers/Map.h"            // Make associated hash table class Map available

#include "./util/Message.h"        // Make message handlers available, not used in this example

#include "./verilog/veri_file.h"      // Make Verilog reader available

#include "./verilog/VeriModule.h"     // Definition of a VeriModule and VeriPrimitive
#include "./verilog/VeriId.h"         // Definitions of all identifier definition tree nodes
#include "./verilog/VeriExpression.h" // Definitions of all verilog expression tree nodes
#include "./verilog/VeriModuleItem.h" // Definitions of all verilog module item tree nodes
#include "./verilog/VeriStatement.h"  // Definitions of all verilog statement tree nodes
#include "./verilog/VeriMisc.h"       // Definitions of all extraneous verilog tree nodes (ie. range, path, strength, etc...)
#include "./verilog/VeriScope.h"      // Symbol table of locally declared identifiers
#include "./verilog/VeriLibrary.h"    // Definition of VeriLibrary
#include "./verilog/veri_yacc.h"

#include <string.h>

#ifdef VERIFIC_NAMESPACE
using namespace Verific ;
#endif

extern std::string checkAndReturnBusDimension(char *busName);
bool saveUncompressGZfile=false;
std::string uncompressFileName;

int main(int argc, char **argv)
{

    std::string file_nm;
    bool isGZfile=false;
    //--------------------------------------------------------------
    // PARSE ARGUMENTS
    //--------------------------------------------------------------
    const char *file_name = 0 ;
    for (int i = 1; i < argc; i++) {
        if (Strings::compare(argv[i], "-o")) {
            i++ ;
            file_name = (i < argc) ? argv[i]: 0 ;
            continue ;
        } else if (Strings::compare(argv[i], "-i")) {
            i++ ;
            file_nm = (i < argc) ? argv[i]: 0 ;
            continue ;
        } else if (Strings::compare(argv[i], "-gz")) {
            isGZfile =true;
            continue ;
        } else if (Strings::compare(argv[i], "-gzo")) {
            saveUncompressGZfile =true;
            i++ ;
            uncompressFileName = (i < argc) ? argv[i]: 0 ;
            continue ;
        }
    }
    if(argc==1)
    {
        Message::PrintLine("Usage: Verilog IP analyzer -o <output csv file> -i<input sv file> ") ;
        Message::PrintLine("       -gz <use only when input file is compressed >\n") ;
        Message::PrintLine("       -gzo <uncompressed file name>\n") ;
        return 1 ;
    }

    if(file_nm.empty()) {
        Message::PrintLine("Input file is missing!") ;
        return 1 ;
    }
    if(saveUncompressGZfile && uncompressFileName.empty())
        uncompressFileName = "decompressed.sv" ;
    if(isGZfile) {
        // Register the call back routine to return my stream capable of reading from gzipped files:
        //        veri_file::RegisterFlexStreamCallBack(GetGZippedStream) ;
    }
    if (!veri_file::Analyze(file_nm.c_str(), veri_file::SYSTEM_VERILOG)) return 2 ;

    // Get the list of top modules
    Array *top_mod_array = veri_file::GetTopModules() ;
    if (!top_mod_array) {
        // If there is no top level module then issue error
        Message::Error(0,"Cannot find any top module. Check for recursive instantiation") ;
        return 4 ;
    }
    VeriModule *module  = (VeriModule *) top_mod_array->GetFirst() ; // Get the first top level module
    delete top_mod_array ; top_mod_array = 0 ; // Cleanup, it is not required anymore

    // Just to see that this is a module, and not a primitive
    VeriIdDef *module_id = (module) ? module->Id() : 0 ;
    if(!module_id) {
        Message::Error(0, "module should always have an 'identifier'.") ;
    }

    if (module_id->IsUdp()) {
        /* This is a Verilog UDP, a primitive */
    } else {
        /* This is a module */
    }

    // Iterate through the module's list of ports ('95 style port or ANSI port declarations)
    Array *ports = module->GetPortConnects() ;
    VeriExpression *port ;
    unsigned i ;
    FILE * pCSVFile;
    if(!file_name)
        pCSVFile = fopen ("exportPorts.csv","w");
    else {
        pCSVFile = fopen (file_name,"w");
    }
    if(!pCSVFile)
    {
        printf("Error in export file open\n") ;
        return 1;
    }
    printf("\n####################################################\n");
    printf("Top module\n#################################################### \n%s\n",module_id->Name());
    printf("####################################################\n\n");
    printf("PortName \t\t\t\tDirection \tType \n") ;
    fprintf (pCSVFile,"PortName \t\t\t\tDirection \tType \n") ;
    FOREACH_ARRAY_ITEM(ports, i, port) {
        if (!port) continue ; // Check for NULL pointer
        char portPrintParams[512]={};
        switch(port->GetClassId()) {
        case ID_VERIANSIPORTDECL:
        {
            VeriAnsiPortDecl *ansi_port = static_cast<VeriAnsiPortDecl*>(port) ;
            if(!ansi_port)
                continue;
            // eg. :
            // input reg [5:0] a, b, c ...

            // Get data type for this declaration
            VeriDataType *data_type = ansi_port->GetDataType() ; // VeriDataType includes 'type' (VERI_REG, VERI_WIRE, VERI_STRUCT etc), array dimension(s), signing ...
            if(!data_type) continue;
            unsigned port_dir = ansi_port->GetDir() ; // a token : VERI_INPUT, VERI_OUTPUT or VERI_INOUT..
            unsigned j ;
            char *portDir;
            if(port_dir==VERI_INPUT)
                portDir="Input";
            else if(port_dir==VERI_OUTPUT)
                portDir="Output";
            else if(port_dir==VERI_INOUT)
                portDir="Inout";
            else
                portDir="Unknown dir";
            VeriIdDef *port_id ;
            // Iterate through all ids declared in this Ansi port decl
            FOREACH_ARRAY_ITEM(ansi_port->GetIds(), j, port_id) {
                if (!port_id) continue ;
                char *port_name = const_cast<char *>(port_id->Name());
                strcat(portPrintParams,port_name);
                std::string busDim=checkAndReturnBusDimension(port_name);
                if(!busDim.empty())
                    strcat(portPrintParams,busDim.c_str());
                if(strlen(port_name)<=5)
                    strcat(portPrintParams,"\t,\t\t\t\t");
                if(strlen(port_name)<=15 && strlen(port_name)>5)
                    strcat(portPrintParams,"\t,\t\t\t");
                if(strlen(port_name)>15 && strlen(port_name)<24)
                    strcat(portPrintParams,"\t,\t\t");
                if(strlen(port_name)>=24)
                    strcat(portPrintParams,"\t,\t");
                port_name="";
                char *port_type="";
                if ( data_type->GetType() == VERI_REAL) //port is REAL
                    port_type="REAL";
                else if ( data_type->GetType() == VERI_WIRE) //port is WIRE
                    port_type="WIRE";
                else if ( data_type->GetType() == VERI_LOGIC) //port is WIRE
                    port_type="LOGIC";
                else if ( data_type->GetType() == VERI_REG) //port is IXS_TYPE
                    port_type="REG";
                else if ( data_type->GetType() == VERI_TRI) //port is VERI_TRI
                    port_type="TRI";
                else if ( data_type->GetType() == VERI_WAND) //port is VERI_WAND
                    port_type="WAND";
                else if ( data_type->GetType() == VERI_TRIAND)
                    port_type="TRIAND";
                else if ( data_type->GetType() == VERI_WOR)
                    port_type="WOR";
                else if ( data_type->GetType() == VERI_TRIOR)
                    port_type="TRIOR";
                else if ( data_type->GetType() == VERI_TRIREG)
                    port_type="TRIREG";
                else if ( data_type->GetType() == VERI_TRI0)
                    port_type="TRI0";
                else if ( data_type->GetType() == VERI_TRI1)
                    port_type="TRI1";
                else if ( data_type->GetType() == VERI_UWIRE)
                    port_type="UWIRE";
                else if ( data_type->GetType() == VERI_SUPPLY0)
                    port_type="SUPPLY0";
                else if ( data_type->GetType() == VERI_SUPPLY1)
                    port_type="SUPPLY1";
                else if ( data_type->GetType() == VERI_INTEGER)
                    port_type="INTEGER";
                else if ( data_type->GetType() == VERI_INT)
                    port_type="INT";
                else if ( data_type->GetType() == VERI_BYTE)
                    port_type="BYTE";
                else if ( data_type->GetType() == VERI_SHORTINT)
                    port_type="SHORTINT";
                else if ( data_type->GetType() == VERI_LONGINT)
                    port_type="LONGINT";
                else if ( data_type->GetType() == VERI_BIT)
                    port_type="BIT";
                else if ( data_type->GetType() == VERI_SHORTREAL)
                    port_type="SHORTREAL";

                else {
                    port_type=const_cast<char *> (data_type->GetName());
                    if(!port_type)
                        port_type="Unknown";
                }

                strcat(portPrintParams,portDir);
                strcat(portPrintParams,"\t,\t");
                strcat(portPrintParams,port_type);
                printf(portPrintParams);
                printf(",\n");
                fprintf (pCSVFile,portPrintParams);
                fprintf (pCSVFile,",\n");
                int ii=0;
                while(portPrintParams[ii]!='\0') {
                    portPrintParams[ii] = '\0';
                    ++ii;
                }
            }
            break ;
        }
        case ID_VERIINDEXEDID:
        {
            // Downcast VeriExpression->VeriIndexedId. now that we know it is a VeriIndexedId
            VeriIndexedId *indexed_id = static_cast<VeriIndexedId*>(port) ;
            // eg. :
            // module split_ports (a[7:4]) ;
            // Here 'a' has a direction (always), an optional type and a range.
            // Here you can test all that :
            if(!indexed_id)
                continue;
            unsigned port_dir = indexed_id->PortExpressionDir() ; // a token : VERI_INPUT, VERI_OUTPUT or VERI_INOUT..
            std::string portDir;
            if(port_dir==VERI_INPUT)
                portDir="Input";
            else if(port_dir==VERI_OUTPUT)
                portDir="Output";
            else if(port_dir==VERI_INOUT)
                portDir="Inout";
            else
                portDir="Unknown dir";

            const char *port_name = indexed_id->GetName() ; // Get port name
            Message::Info(indexed_id->Linefile(),",'", port_name, "' is an indexed id in a port declaration") ;
            const char *prefix_name = indexed_id->GetName() ; // Get prefix name in char* form
            VeriIdDef *prefix_id = indexed_id->GetId() ; // Get resolved referenced (prefix) identifier

            // Here you can put more code to get more info about the port_id.
            // The full set of routines that are allowed on a VeriIdDef is in VeriId.h.
            break ;
        }
        case ID_VERIPORTCONNECT:
        {
            // Downcast VeriExpression->VeriPortConnect. now that we know it is a  VeriPortConnect
            VeriPortConnect *port_conn = static_cast<VeriPortConnect*>(port) ;

            // eg.:
            // module same_port (.a(i)) ;
            // Name 'i' is declared within the module as a normal declaration with a direction, type and an optional range
            // Here you can test all that :

            VeriExpression *conn = port_conn->GetConnection() ; // Return the 'actual' expression

            Message::Info(port_conn->Linefile(),",'", port_conn->GetNamedFormal(), "' is a named port declaration") ;
            // Here you can put more code to get more info about the port_conn.
            // The full set of routines that are allowed on a VeriPortConnect is in VeriExpression.h
            break ;
        }
        case ID_VERICONCAT:
        {
            // Downcast VeriExpression->VeriConcat. now that we know it is a  VeriConcat
            VeriConcat *concat = static_cast<VeriConcat*>(port) ;

            // eg. :
            // module same_port ({i, j[2]}) ;
            // Net 'i' and 'j' are declared within the module as a normal declaration with a direction, type and an optional range
            // Here you can test all that :

            unsigned port_dir = concat->PortExpressionDir() ; // Can take up values: VERI_INPUT, VERI_OUTPUT or VERI_INOUT

            Array *expr_arr = concat->GetExpressions() ; // Get an array of expressions for the concatenation
            unsigned j ;
            // Iterate through all expressions
            VeriExpression *expr ;
            FOREACH_ARRAY_ITEM(expr_arr, j, expr) {
                if (!expr) continue ;
                const char *port_name = expr->GetName() ;
                // Here you can put more code to get more info about the port_conn.
                // The full set of routines that are allowed on a VeriPortConnect can be found in VeriExpression.h
            }
            Message::Info(concat->Linefile(),",is a 'concat' port declaration") ;
            break ;
        }
        case ID_VERIIDREF:
        {
            VeriIdRef *id_ref = static_cast<VeriIdRef*>(port) ;
            if(!id_ref)
                continue;

            // eg. :
            // module same_port (i) ;
            // That is the 'unresolved' name :
            // Get unresolved name of this IdRef.
            const char *name = id_ref->GetName() ;
            // 'name' will be '0', since the id-reference is resolved in the analyzer.
            // This means that the (port)reference now has a pointer directly to the identifier that it refers to.
            // Here is how you pick that one up :

            // Get the resolved identifier definition that is referred here
            VeriIdDef *id = id_ref->FullId() ;

            char *port_name = const_cast<char *>(id->Name());
            strcat(portPrintParams,port_name);
            std::string busDim=checkAndReturnBusDimension(port_name);
            if(!busDim.empty())
                strcat(portPrintParams,busDim.c_str());

            if(strlen(port_name)<=5)
                strcat(portPrintParams,"\t,\t\t\t\t");
            if(busDim.empty()) {
                if(strlen(port_name)<=15 && strlen(port_name)>5)
                    strcat(portPrintParams,"\t,\t\t\t");
                if(strlen(port_name)>15 && strlen(port_name)<24)
                    strcat(portPrintParams,"\t,\t\t");
                if(strlen(port_name)>=24)
                    strcat(portPrintParams,"\t,\t");
            }
            else {
                if(strlen(port_name)>=24)
                    strcat(portPrintParams,"\t,\t");
                else
                    strcat(portPrintParams,"\t,\t\t");
            }


            port_name="";
            unsigned is_port    = id->IsPort() ;  // Should return true in this case
            unsigned port_dir   = id->Dir() ;     // returns VERI_INPUT, VERI_OUTPUT, VERI_INOUT
            std::string portDir;
            if(port_dir==VERI_INPUT)
                portDir="Input";
            else if(port_dir==VERI_OUTPUT)
                portDir="Output";
            else if(port_dir==VERI_INOUT)
                portDir="Inout";
            else
                portDir="Unknown dir";
            unsigned port_type  = id->Type() ;    // returns VERI_WIRE, VERI_REG, etc ...
            char *port_typeName="";
            if ( port_type == VERI_REAL) //port is REAL
                port_typeName="REAL";
            else if ( port_type == VERI_WIRE) //port is WIRE
                port_typeName="WIRE";
            else if (port_type == VERI_LOGIC) //port is WIRE
                port_typeName="LOGIC";
            else if ( port_type == VERI_REG) //port is IXS_TYPE
                port_typeName="REG";
            else if ( port_type == VERI_TRI) //port is VERI_TRI
                port_typeName="TRI";
            else if ( port_type == VERI_WAND) //port is VERI_WAND
                port_typeName="WAND";
            else if (port_type == VERI_TRIAND)
                port_typeName="TRIAND";
            else if ( port_type == VERI_WOR)
                port_typeName="WOR";
            else if ( port_type == VERI_TRIOR)
                port_typeName="TRIOR";
            else if ( port_type == VERI_TRIREG)
                port_typeName="TRIREG";
            else if ( port_type == VERI_TRI0)
                port_typeName="TRI0";
            else if ( port_type == VERI_TRI1)
                port_typeName="TRI1";
            else if ( port_type == VERI_UWIRE)
                port_typeName="UWIRE";
            else if ( port_type == VERI_SUPPLY0)
                port_typeName="SUPPLY0";
            else if ( port_type == VERI_SUPPLY1)
                port_typeName="SUPPLY1";
            else if ( port_type == VERI_INTEGER)
                port_typeName="INTEGER";
            else if ( port_type == VERI_INT)
                port_typeName="INT";
            else if ( port_type == VERI_BYTE)
                port_typeName="BYTE";
            else if ( port_type == VERI_SHORTINT)
                port_typeName="SHORTINT";
            else if ( port_type == VERI_LONGINT)
                port_typeName="LONGINT";
            else if ( port_type == VERI_BIT)
                port_typeName="BIT";
            else if ( port_type == VERI_SHORTREAL)
                port_typeName="SHORTREAL";
            else {
                VeriAnsiPortDecl *ansi_port = static_cast<VeriAnsiPortDecl*>(port) ;
                if(!ansi_port)
                    continue;
                VeriDataType *data_type = ansi_port->GetDataType() ;
                if(!data_type)
                    continue;
                port_typeName=const_cast<char *> ( data_type->GetName());
                if(!port_typeName /*|| !strstr(typesList,port_type)*/)
                    port_typeName="Unknown";
            }
            strcat(portPrintParams,portDir.c_str());
            strcat(portPrintParams,"\t,\t");
            strcat(portPrintParams,port_typeName);
            printf(portPrintParams);
            printf(",\n");
            fprintf (pCSVFile,portPrintParams);
            fprintf (pCSVFile,",\n");
            int ii=0;
            while(portPrintParams[ii]!='\0') {
                portPrintParams[ii] = '\0';
                ++ii;
            }

            break ;
        }
        default:
            Message::Error(port->Linefile(),"unknown port found") ;
        }
    }
    fclose(pCSVFile);

    return 0 ; // status OK.
}

//verific_stream *
//GetGZippedStream(const char *file_name)
//{
//    if (!file_name || !FileSystem::IsFile(file_name)) return 0 ;

//    // Return a newly allocated object of 'verific_zip_stream' class:
//    verific_stream *zip_stream = new verific_zip_stream(file_name) ;
//    if (zip_stream->fail()) {
//        // Something bad happned:
//        delete zip_stream ;
//        zip_stream = 0 ;
//    }

//    return zip_stream ; // This object will be absorbed by Verific lexer.
//}
