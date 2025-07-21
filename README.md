
# VIPAnalyzer

Verilog IP Analyzer uses the commercial Verific library to analyze and extract information and primitives from the IP described in a Verilog file.
In this light version, this software finds the top module and extracts its ports and information about the types and directions. It exports them to a CSV formatted file for further processing with CSVParser or TCL scripts.
![](https://via.placeholder.com/468x300?text=App+Screenshot+Here)
The full version of the software can detect and export many more features and here I have shown in this sample extracted file:
```javascript
-- Analyzing Verilog file '.\test_designs\mb_processor.v' (VERI-1482)

####################################################
Top module
####################################################
mb_processor
####################################################

PortName                                		Direction       Type
rzq     ,                               		Inout   ,       LOGIC,
mcbx_dram_we_n  ,                       		Output  ,       LOGIC,
mcbx_dram_udqs  ,                       		Inout   ,       LOGIC,
mcbx_dram_udm   ,                       		Output  ,       LOGIC,
mcbx_dram_ras_n ,                       		Output  ,       LOGIC,
mcbx_dram_ldm   ,                       		Output  ,       LOGIC,
mcbx_dram_dqs   ,                       		Inout   ,       LOGIC,
mcbx_dram_dq[15:0]      ,               		Inout   ,       WIRE,
mcbx_dram_clk_n ,                       		Output  ,       LOGIC,
mcbx_dram_clk   ,                       		Output  ,       LOGIC,
mcbx_dram_cke   ,                       		Output  ,       LOGIC,
mcbx_dram_cas_n ,                       		Output  ,       LOGIC,
mcbx_dram_ba[1:0]       ,               		Output  ,       WIRE,
mcbx_dram_addr[12:0]    ,               		Output  ,       WIRE,
USB_Uart_sout   ,                       		Output  ,       LOGIC,
USB_Uart_sin    ,                       		Input   ,       LOGIC,
SPI_FLASH_Wn    ,                       		Output  ,       LOGIC,
SPI_FLASH_SS    ,                       		Inout   ,      	LOGIC,
SPI_FLASH_SCLK  ,                       		Inout   ,      	LOGIC,
SPI_FLASH_MOSI  ,                       		Inout   ,      	LOGIC,
SPI_FLASH_MISO  ,                       		Inout   ,      	LOGIC,
SPI_FLASH_HOLDn ,                       		Output  ,       LOGIC,
RESET   ,                               		Input   ,       LOGIC,
LEDs_4Bits_TRI_O[3:0]   ,               		Output  ,       WIRE,
CLK_66MHZ       ,                       		Input   ,       LOGIC,
DIP_Switches_GPIO_IO_I_pin[3:0] ,       	    Input   ,       WIRE,
.\test_designs\mb_processor.v(34): INFO: ,   'data declaration'
.\test_designs\mb_processor.v(59): INFO: ,   'data declaration'
.\test_designs\mb_processor.v(63): INFO: , 'net declaration'
.\test_designs\mb_processor.v(64): INFO: , 'net declaration'
.\test_designs\mb_processor.v(297): INFO: , 'net declaration'
.\test_designs\mb_processor.v(298): INFO: , 'net declaration'
.\test_designs\mb_processor.v(299): INFO: , 'net declaration'
.\test_designs\mb_processor.v(300): INFO: , 'net declaration'
.\test_designs\mb_processor.v(304): INFO: , 'continuous assignment'
.\test_designs\mb_processor.v(305): INFO: , 'continuous assignment'
.\test_designs\mb_processor.v(329): INFO: , 'continuous assignment'
.\test_designs\mb_processor.v(356): INFO: ,'proc_sys_reset_0_wrapper' instantiated module
.\test_designs\mb_processor.v(333): INFO: ,'proc_sys_reset_0', instance
.\test_designs\mb_processor.v(386): INFO: ,'microblaze_0_ilmb_wrapper' instantiated module
.\test_designs\mb_processor.v(360): INFO: ,'microblaze_0_ilmb', instance
.\test_designs\mb_processor.v(471): INFO: ,'microblaze_0_i_bram_ctrl_wrapper' instantiated module
.\test_designs\mb_processor.v(390): INFO: ,'microblaze_0_i_bram_ctrl', instance
.\test_designs\mb_processor.v(501): INFO: ,'microblaze_0_dlmb_wrapper' instantiated module
.\test_designs\mb_processor.v(475): INFO: ,'microblaze_0_dlmb', instance
.\test_designs\mb_processor.v(586): INFO: ,'microblaze_0_d_bram_ctrl_wrapper' instantiated module
.\test_designs\mb_processor.v(505): INFO: ,'microblaze_0_d_bram_ctrl', instance
.\test_designs\mb_processor.v(605): INFO: ,'microblaze_0_bram_block_wrapper' instantiated module
.\test_designs\mb_processor.v(590): INFO: ,'microblaze_0_bram_block', instance
.\test_designs\mb_processor.v(1205): INFO: ,'microblaze_0_wrapper' instantiated module
.\test_designs\mb_processor.v(609): INFO: ,'microblaze_0', instance
.\test_designs\mb_processor.v(1355): INFO: ,'debug_module_wrapper' instantiated module
.\test_designs\mb_processor.v(1209): INFO: ,'debug_module', instance
.\test_designs\mb_processor.v(1385): INFO: ,'clock_generator_0_wrapper' instantiated module
.\test_designs\mb_processor.v(1359): INFO: ,'clock_generator_0', instance
.\test_designs\mb_processor.v(1593): INFO: ,'axi4lite_0_wrapper' instantiated module
.\test_designs\mb_processor.v(1389): INFO: ,'axi4lite_0', instance
.\test_designs\mb_processor.v(1801): INFO: ,'axi4_0_wrapper' instantiated module
.\test_designs\mb_processor.v(1597): INFO: ,'axi4_0', instance
.\test_designs\mb_processor.v(1828): INFO: ,'usb_uart_wrapper' instantiated module
.\test_designs\mb_processor.v(1805): INFO: ,'USB_Uart', instance
.\test_designs\mb_processor.v(1866): INFO: ,'spi_flash_wrapper' instantiated module
.\test_designs\mb_processor.v(1832): INFO: ,'SPI_FLASH', instance
.\test_designs\mb_processor.v(2137): INFO: ,'mcb3_lpddr_wrapper' instantiated module
.\test_designs\mb_processor.v(1870): INFO: ,'MCB3_LPDDR', instance
.\test_designs\mb_processor.v(2168): INFO: ,'dip_switches_wrapper' instantiated module
.\test_designs\mb_processor.v(2141): INFO: ,'DIP_Switches', instance
.\test_designs\mb_processor.v(2193): INFO: ,'axi_pwm_0_wrapper' instantiated module
.\test_designs\mb_processor.v(2172): INFO: ,'axi_pwm_0', instance
.\test_designs\mb_processor.v(2201): INFO: ,'IOBUF' instantiated module
.\test_designs\mb_processor.v(2196): INFO: ,'iobuf_0', instance
.\test_designs\mb_processor.v(2209): INFO: ,'IOBUF' instantiated module
.\test_designs\mb_processor.v(2204): INFO: ,'iobuf_1', instance
.\test_designs\mb_processor.v(2217): INFO: ,'IOBUF' instantiated module
.\test_designs\mb_processor.v(2212): INFO: ,'iobuf_2', instance
.\test_designs\mb_processor.v(2225): INFO: ,'IOBUF' instantiated module
.\test_designs\mb_processor.v(2220): INFO: ,'iobuf_3', instance
Press <RETURN> to close this window...

```
Since the Verific library is a commercial source that is paid for, it is not included here. For information about the Verific Verilog parser and library, see www.verific.com. 


## Build
Open the provided .pro file in QTCreator and run the build.
Tested and built in Windows 10, it should open and convert without problems in Linux.
For a successful build, you need the full source code of Verific.
Under Linux, after opening the .pro file, a full Makefile is written, which can also be built with the make command.
## Usage

```javascript
Usage: 
   VIPAnalyzer -o <output csv file> -i<input sv file> 
   -gz <use only when input file is compressed >
   -gzo <uncompressed file name>
```


## Test
Located in the test_designs folder is a shareware IP from Xilinx which contains a complete Verilog description of the Microblaze softcore processor generated in Xilinx Vivado design software.
You can see the sample exported CSV file in portList.csv
## Run test
```javascript
VIPAnalyzer -i ../test_designs/mb_processor.v -o portList.csv
```


