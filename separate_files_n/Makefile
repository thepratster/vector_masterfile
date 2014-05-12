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
TOAA:c2llvmbc llvmbc2aa
TOVC:c2llvmbc llvmbc2aa  aalink aa2vc 
VC2VHDL: vc2vhdl  vhdlsim
AA2VHDLSIM:aalink aa2vc vc2vhdl  vhdlsim
TOVHDL:TOVC vc2vhdl

LLVM2AAOPTS=

# the top-level module.
TOPMODULES=-T vector_control_daemon


# compile with SW defined.
SW: $(SRC)/prog.h $(SRC)/testbench.c $(SRC)/prog.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/prog.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/fpsub32fi.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/fpadd32fi.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/fpsub32f.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/fpadd32f.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/fpmul32f.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/fdiv32.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/iq_err_calc.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/rotor_flux_calc.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/omega_calc.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/theta_calc.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(PTHREADUTILS_INCLUDE) -I$(SRC) $(SRC)/testbench.c
	gcc -g -o testbench_sw prog.o fpsub32fi.o fpadd32fi.o fpsub32f.o fpadd32f.o fpmul32f.o fdiv32.o iq_err_calc.o rotor_flux_calc.o omega_calc.o theta_calc.o testbench.o -L$(PIPEHANDLER_LIB) -lm -lPipeHandler -lpthread -lrt 

# five steps from C to vhdl simulator.
#HW: c2llvmbc llvmbc2aa  aa2vc  vc2vhdl  vhdlsim
HW: c2llvmbc llvmbc2aa  aalink aa2vc  vc2vhdl  vhdlsim
TOAA: c2llvmbc llvmbc2aa  aalink
TOVC: TOAA aa2vc 
TOVHDL: TOVC vc2vhdl

#AA2VHDL: aa2vc vc2vhdl vhdlsim
AA2VHDL: aa2vc vc2vhdl vhdlsim

# C to llvm byte-code.. use clang.
c2llvmbc: $(SRC)/prog.c $(SRC)/prog.h $(SRC)/fpsub32fi.c $(SRC)/fpadd32fi.c $(SRC)/iq_err_calc.c $(SRC)/rotor_flux_calc.c $(SRC)/omega_calc.c $(SRC)/fpadd32f.c $(SRC)/fpsub32f.c
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/prog.c
	opt --indvars --loopsimplify prog.o -o prog.opt.o 
	llvm-dis prog.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/fpsub32fi.c
	opt --indvars --loopsimplify fpsub32fi.o -o fpsub32fi.opt.o 
	llvm-dis fpsub32fi.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/fpadd32fi.c
	opt --indvars --loopsimplify fpadd32fi.o -o fpadd32fi.opt.o 
	llvm-dis fpadd32fi.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/fpsub32f.c
	opt --indvars --loopsimplify fpsub32f.o -o fpsub32f.opt.o 
	llvm-dis fpsub32f.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/fpadd32f.c
	opt --indvars --loopsimplify fpadd32f.o -o fpadd32f.opt.o 
	llvm-dis fpadd32f.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/fpmul32f.c
	opt --indvars --loopsimplify fpmul32f.o -o fpmul32f.opt.o 
	llvm-dis fpmul32f.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/fdiv32.c
	opt --indvars --loopsimplify fdiv32.o -o fdiv32.opt.o 
	llvm-dis fdiv32.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/iq_err_calc.c
	opt --indvars --loopsimplify iq_err_calc.o -o iq_err_calc.opt.o 
	llvm-dis iq_err_calc.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/rotor_flux_calc.c
	opt --indvars --loopsimplify rotor_flux_calc.o -o rotor_flux_calc.opt.o 
	llvm-dis rotor_flux_calc.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/theta_calc.c
	opt --indvars --loopsimplify theta_calc.o -o theta_calc.opt.o 
	llvm-dis theta_calc.opt.o
	clang -O3 -std=gnu89 -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/omega_calc.c
	opt --indvars --loopsimplify omega_calc.o -o omega_calc.opt.o 
	llvm-dis omega_calc.opt.o
		
# llvm byte-code to Aa..
llvmbc2aa:  prog.opt.o fpsub32fi.opt.o fpadd32fi.opt.o fpsub32f.opt.o fpadd32f.opt.o fpmul32f.opt.o  fdiv32.opt.o  iq_err_calc.opt.o  rotor_flux_calc.opt.o  theta_calc.opt.o  omega_calc.opt.o  
	llvm2aa  prog.opt.o | vcFormat >  prog.aa
	llvm2aa  fpsub32fi.opt.o | vcFormat >  fpsub32fi.aa
	llvm2aa  fpadd32fi.opt.o | vcFormat >  fpadd32fi.aa
	llvm2aa  fpsub32f.opt.o | vcFormat >  fpsub32f.aa
	llvm2aa  fpadd32f.opt.o | vcFormat >  fpadd32f.aa
	llvm2aa  fpmul32f.opt.o | vcFormat >  fpmul32f.aa
	llvm2aa  fdiv32.opt.o | vcFormat >  fdiv32.aa
	llvm2aa  iq_err_calc.opt.o | vcFormat >  iq_err_calc.aa
	llvm2aa  rotor_flux_calc.opt.o | vcFormat >  rotor_flux_calc.aa
	llvm2aa  theta_calc.opt.o | vcFormat >  theta_calc.aa
	llvm2aa  omega_calc.opt.o | vcFormat >  omega_calc.aa
	
# Aa to vC
aalink:  prog.aa fpsub32fi.aa fpadd32fi.aa fpsub32f.aa fpadd32f.aa fpmul32f.aa fdiv32.aa iq_err_calc.aa rotor_flux_calc.aa theta_calc.aa omega_calc.aa
	AaLinkExtMem prog.aa fpsub32fi.aa fpadd32fi.aa fpsub32f.aa fpadd32f.aa fpmul32f.aa fdiv32.aa iq_err_calc.aa rotor_flux_calc.aa theta_calc.aa omega_calc.aa| vcFormat > prog.linked.aa


	AaOpt fpsub32fi.aa | vcFormat > fpsub32fi.opt.aa
	AaOpt fpadd32fi.aa | vcFormat > fpadd32fi.opt.aa
	AaOpt fpsub32f.aa | vcFormat > fpsub32f.opt.aa
	AaOpt fpadd32f.aa | vcFormat > fpadd32f.opt.aa
	AaOpt fpmul32f.aa | vcFormat > fpmul32f.opt.aa
	
	AaLinkExtMem fdiv32.aa fpsub32fi.aa fpadd32fi.aa fpsub32f.aa fpadd32f.aa | vcFormat > fdiv32.linked.aa
	AaOpt fdiv32.linked.aa | vcFormat > fdiv32.linked.opt.aa
	
	AaLinkExtMem iq_err_calc.aa fpmul32f.aa fdiv32.aa fpsub32fi.aa fpadd32fi.aa fpsub32f.aa fpadd32f.aa| vcFormat > iq_err_calc.linked.aa
	AaOpt iq_err_calc.linked.aa | vcFormat > iq_err_calc.linked.opt.aa
	
	AaLinkExtMem theta_calc.aa fpmul32f.aa fpadd32f.aa | vcFormat > theta_calc.linked.aa
	AaOpt theta_calc.linked.aa | vcFormat > theta_calc.linked.opt.aa
	
	AaLinkExtMem rotor_flux_calc.aa fpmul32f.aa fpadd32f.aa | vcFormat > rotor_flux_calc.linked.aa
	AaOpt rotor_flux_calc.linked.aa | vcFormat > rotor_flux_calc.linked.opt.aa
	
	AaLinkExtMem omega_calc.aa fpmul32f.aa fdiv32.aa fpsub32fi.aa fpadd32fi.aa fpsub32f.aa fpadd32f.aa| vcFormat > omega_calc.linked.aa
	AaOpt omega_calc.linked.aa | vcFormat > omega_calc.linked.opt.aa
			

aa2vc: prog.linked.aa fpsub32fi.opt.aa fpadd32fi.opt.aa fpsub32f.opt.aa fpadd32f.opt.aa fpmul32f.opt.aa fdiv32.linked.opt.aa iq_err_calc.linked.opt.aa rotor_flux_calc.linked.opt.aa theta_calc.linked.opt.aa omega_calc.linked.opt.aa 
	Aa2VC -O -C prog.linked.aa | vcFormat > prog.vc
	Aa2VC  fpsub32fi.opt.aa | vcFormat >  fpsub32fi.vc
	Aa2VC  fpadd32fi.opt.aa | vcFormat >  fpadd32fi.vc
	Aa2VC  fpsub32f.opt.aa | vcFormat >  fpsub32f.vc
	Aa2VC  fpadd32f.opt.aa | vcFormat >  fpadd32f.vc
	Aa2VC  fpmul32f.opt.aa | vcFormat >  fpmul32f.vc
	Aa2VC  fdiv32.linked.opt.aa | vcFormat >  fdiv32.vc
	Aa2VC  iq_err_calc.linked.opt.aa | vcFormat >  iq_err_calc.vc
	Aa2VC  rotor_flux_calc.linked.opt.aa | vcFormat >  rotor_flux_calc.vc
	Aa2VC  theta_calc.linked.opt.aa | vcFormat >  theta_calc.vc
	Aa2VC  omega_calc.linked.opt.aa | vcFormat >  omega_calc.vc

# vC to VHDL
vc2vhdl: prog.vc fpsub32fi.vc fpadd32fi.vc fpsub32f.vc fpadd32f.vc fpmul32f.vc fdiv32.vc iq_err_calc.vc rotor_flux_calc.vc theta_calc.vc omega_calc.vc
	vc2vhdl -O -S 4 -I 2 -v -a -C -e ahir_system -w -s ghdl $(TOPMODULES) -f prog.vc -L $(FUNCTIONLIB)/fpu.list fpsub32fi.vc fpadd32fi.vc fpsub32f.vc fpadd32f.vc fpmul32f.vc fdiv32.vc iq_err_calc.vc rotor_flux_calc.vc theta_calc.vc omega_calc.vc
	vhdlFormat < ahir_system_global_package.unformatted_vhdl > ahir_system_global_package.vhdl
	vhdlFormat < ahir_system.unformatted_vhdl > ahir_system.vhdl
	vhdlFormat < ahir_system_test_bench.unformatted_vhdl > ahir_system_test_bench.vhdl

# build testbench and ghdl executable
# note the use of SOCKETLIB in building the testbench.
vhdlsim: ahir_system.vhdl ahir_system_test_bench.vhdl $(SRC)/testbench.c vhdlCStubs.h vhdlCStubs.c
	gcc -c vhdlCStubs.c  -I$(SRC) -I./ -I$(SOCKETLIB_INCLUDE)
	gcc -c $(SRC)/testbench.c -I$(PTHREADUTILS_INCLUDE) -I$(SRC) -I./ -I$(SOCKETLIB_INCLUDE)
	gcc -o testbench_hw testbench.o vhdlCStubs.o  -L$(SOCKETLIB_LIB) -lSocketLib -lpthread -lm
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
	rm -rf *.o* *.cf *.*vhdl vhdlCStubs.* *.vcd in_data* out_data* testbench_sw testbench_hw ahir_system_test_bench vhpi.log *.aa *.vc *.lso xst *.ngc *_xmsgs *.xrpt pipeHandler.log *.srp *.ghw *.dot *.log

PHONY: all clean	
