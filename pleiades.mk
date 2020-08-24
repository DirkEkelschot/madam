OBJECTS = adapt_output.cpp\
          adapt_compute.cpp\
          adapt_schedule.cpp\
          adapt_operations.cpp\
          hex2tet.cpp \
          adapt_geometry.cpp \
          adapt_math.cpp \
          adapt_recongrad.cpp \
          adapt_io.cpp \
          main.cpp

PARMMG_HOME = /home1/dekelsch/parmmg/build
MMG_HOME = /home1/dekelsch/parmmg/build/Mmg-prefix/src/Mmg-build

CXXFLAGS += -std=c++11 -DMPI_NO_CPPBIND -I$(MMG_HOME)/include -I$(PARMMG_HOME)/include
LDFLAGS += -L$(MMG_HOME)/lib -L$(PARMMG_HOME)/lib
LDLIBS += -lmpi -lparmetis -lmetis -lhdf5 -mkl

all:
	icpc $(CXXFLAGS) $(OBJECTS) -o adapt $(LDLIBS)
#	rm -rf *.o *.mod
