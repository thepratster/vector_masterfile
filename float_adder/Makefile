# build software version of testbench (to check the "desired behaviour")
AHIR_RELEASE=/home/pratik/Files/MTP/ahir-master/release
SOCKETLIB_INCLUDE=$(AHIR_RELEASE)/CtestBench/include
SOCKETLIB_LIB=$(AHIR_RELEASE)/CtestBench/lib
PIPEHANDLER_INCLUDE=$(AHIR_RELEASE)/pipeHandler/include
PIPEHANDLER_LIB=$(AHIR_RELEASE)/pipeHandler/lib
PTHREADUTILS_INCLUDE=$(AHIR_RELEASE)/pthreadUtils/include
VHDL_LIB=$(AHIR_RELEASE)/vhdl
VHDL_VHPI_LIB=$(AHIR_RELEASE)/CtestBench/vhdl
FUNCTIONLIB=$(AHIR_RELEASE)/functionLibrary/
SRC=./src
all: SW HW 
TOAA:c2llvmbc llvmbc2aa  aalink
TOVC:c2llvmbc llvmbc2aa  aalink aa2vc 
VC2VHDL: vc2vhdl  vhdlsim
AA2VHDLSIM: aa2vc  vc2vhdl  vhdlsim
TOVHDL:TOVC vc2vhdl

LLVM2AAOPTS=

# the top-level module.
TOPMODULES=-t fdiv


# compile with SW defined.
SW: $(SRC)/prog.c $(SRC)/testbench.c 
	gcc -g -c -DSW $(PROGDEFS) -I$(SRC) -I$(PIPEHANDLER_INCLUDE) $(SRC)/prog.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(SRC) $(SRC)/testbench.c
	gcc -g -o testbench_sw prog.o testbench.o -L$(PIPEHANDLER_LIB) -lPipeHandler -lpthread

# five steps from C to vhdl simulator.
HW: c2llvmbc llvmbc2aa  aa2vc  vc2vhdl  vhdlsim

AA2VHDL: aa2vc vc2vhdl vhdlsim

# C to llvm byte-code.. use clang.
c2llvmbc: $(SRC)/prog.c 
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/prog.c
	opt --indvars --loopsimplify prog.o -o prog.opt.o 
	llvm-dis prog.opt.o

# llvm byte-code to Aa..
llvmbc2aa:  prog.opt.o 
	llvm2aa  prog.opt.o | vcFormat >  prog.aa

# Aa to vC
aa2vc: prog.aa
	Aa2VC -O -C prog.aa | vcFormat > prog.vc

# vC to VHDL
vc2vhdl: prog.vc
	vc2vhdl -O -S 4 -I 2 -v -a -C -e ahir_system -w -s ghdl $(TOPMODULES) -f prog.vc -L $(FUNCTIONLIB)/fpu.list
	vhdlFormat < ahir_system_global_package.unformatted_vhdl > ahir_system_global_package.vhdl
	vhdlFormat < ahir_system.unformatted_vhdl > ahir_system.vhdl
	vhdlFormat < ahir_system_test_bench.unformatted_vhdl > ahir_system_test_bench.vhdl

# build testbench and ghdl executable
# note the use of SOCKETLIB in building the testbench.
vhdlsim: ahir_system.vhdl ahir_system_test_bench.vhdl $(SRC)/testbench.c vhdlCStubs.h vhdlCStubs.c
	gcc -c vhdlCStubs.c  -I$(SRC) -I./ -I$(SOCKETLIB_INCLUDE)
	gcc -c $(SRC)/testbench.c -I$(PTHREADUTILS_INCLUDE) -I$(SRC) -I./ -I$(SOCKETLIB_INCLUDE)
	gcc -o testbench_hw testbench.o vhdlCStubs.o  -L$(SOCKETLIB_LIB) -lSocketLib -lpthread
	ghdl --clean
	ghdl --remove
	ghdl -i --work=GhdlLink  $(VHDL_LIB)/GhdlLink.vhdl
	ghdl -i --work=ahir  $(VHDL_LIB)/ahir.vhdl
	ghdl -i --work=aHiR_ieee_proposed  $(VHDL_LIB)/aHiR_ieee_proposed.vhdl
	ghdl -i --work=work ahir_system_global_package.vhdl
	ghdl -i --work=work ahir_system.vhdl
	ghdl -i --work=work ahir_system_test_bench.vhdl
	ghdl -m --work=work -Wl,-L$(SOCKETLIB_LIB) -Wl,-lVhpi ahir_system_test_bench 

clean:
	rm -rf *.o* *.cf *.*vhdl vhdlCStubs.* *.vcd in_data* out_data* testbench_sw testbench_hw ahir_system_test_bench vhpi.log *.aa *.vc *.lso xst *.ngc *_xmsgs *.xrpt pipeHandler.log *.srp *.ghw *.dot

PHONY: all clean	
