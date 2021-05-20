#PARMETIS_HOME = /Users/dekelsch/Software/parmetis-4.0.3/parmetis-install
#METIS_HOME = /Users/dekelsch/Software/parmetis-4.0.3/metis/metis-install
#HDF5_HOME = /Users/dekelsch/Software/hdf5-1.12.0/hdf5-install
#MPICH_HOME = /Users/dekelsch/Software/mpich-3.3.1/mpich-3.1.1-install
#PARMMG_HOME = /Users/dekelsch/Software/parmmg/build
#MMG_HOME = /Users/dekelsch/Software/parmmg/build/Mmg-prefix/src/Mmg-build

#CXXFLAGS += -std=c++11 -I$(MMG_HOME)/include -I$(PARMMG_HOME)/include -I$(PARMETIS_HOME)/include -I$(MPICH_HOME)/include -I$(HDF5_HOME)/include -I$(METIS_HOME)/include
#LDFLAGS += -L$(MMG_HOME)/lib -L$(PARMMG_HOME)/lib -L$(PARMETIS_HOME)/lib -L$(METIS_HOME)/lib -L$(MPICH_HOME)/lib -L$(HDF5_HOME)/lib
#LDLIBS += -lmetis -lparmetis -lhdf5 -lmpi -llapack -lblas -lmmg -lparmmg
#
#CC = /Users/dekelsch/Software/mpich-3.3.1/mpich-3.1.1-install/bin/mpic++

PARMETIS_HOME = $(SOFT_HOME)/metis
METIS_HOME = $(SOFT_HOME)/metis
HDF5_HOME = $(SOFT_HOME)/hdf5
#MPICH_HOME = /Users/dekelsch/Software/mpich-3.3.1/mpich-3.1.1-install
PARMMG_HOME = $(SOFT_HOME)/parmmg/
MMG_HOME = $(SOFT_HOME)/parmmg/

CXXFLAGS += -std=c++11 -I$(MMG_HOME)/include -I$(PARMMG_HOME)/include -I$(PARMETIS_HOME)/include -I$(HDF5_HOME)/include -I$(METIS_HOME)/include

LDFLAGS += -L$(MMG_HOME)/lib -L$(PARMMG_HOME)/lib -L$(PARMETIS_HOME)/lib -L$(HDF5_HOME)/lib
LDLIBS += -lmetis -lparmetis -lhdf5 -lmpi -llapack -lblas -lmmg -lparmmg
CC = mpic++

