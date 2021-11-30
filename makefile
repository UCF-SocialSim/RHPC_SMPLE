include ./rhpc-smple.env
include ./rhpc-smple.mk

MAIN := libRHPC_SMPLE_LIB.so
MPICXX := mpicxx

LDFLAGS := 	-L${REPAST_HPC_LIB_DIR}


INCLUDES = 	-I${REPAST_HPC_INCLUDE_DIR} \
			-I${REPAST_HPC_INCLUDE_MAIN} \
			-I${BOOST_INCLUDE_DIR} \
			-I${NETCDF_INCLUDE_DIR} \
			-I${NETCDF_CXX_INCLUDE_DIR} \
			-I${MPI_INCLUDE_DIR} \
			-I${INCDIR}

SRCS = $(addprefix $(SRCDIR)/, $(SRCFILES))

MSSRCS = $(addprefix $(SRCDIR)/, $(MSSRCFILES))

OBJS := $(SRCS:.cpp=.o)

MSOBJS = $(MSSRCS:.cpp=.o)

LIB := $(shell basename $(CURDIR)).a

# -Wall  turn on all warnings
# -Wno-deprecated turn off deprecated warning
#  -DNDEBUG  // Release mode
#  -DDEBUG   // does not work with RepastHPC
# -g3 adds lots of debug hooks, -O3 for speed optimization
# to Use the metrics server, add a -DUSING_METRIC_SERVER to MPICXXFLAGS
MPICXXFLAGS := -Wall -Wno-deprecated -std=c++11 -MMD -MP $(INCLUDES) -Wno-reorder -Wno-unused-variable -Wno-sign-compare -DBOOST_SYSTEM_NO_DEPRECATED

all: MPICXXFLAGS += -O3 -DNDEBUG 
all: library

library: $(MAIN)
$(MAIN):  $(OBJS)
	#echo $(OBJS)
	mkdir ${BINDIR} -p
	ar rc ${BINDIR}/$(MAIN) $(OBJS)
	ranlib ${BINDIR}/$(MAIN)

.PHONY: install
install: ${BINDIR}/$(MAIN)
	mkdir ${INSTALL_DIR} -p
	cp ${BINDIR}/$(MAIN) $(INSTALL_DIR)/$(MAIN)
	
.PHONY: debug
debug: MPICXXFLAGS += -O0 -g3 -DDEBUG_CATCH
debug: library

.PHONY: logplatforms
logplatforms: MPICXXFLAGS += -DPLATFORMLOGGING
logplatforms: library

.PHONY: allloggingoff
allloggingoff: MPICXXFLAGS += -DNOCORELOGGING
allloggingoff: library

.cpp.o:
	$(MPICXX) $(LDFLAGS) -c $< -o $@ $(MPICXXFLAGS)

.PHONY: clean
clean:
	rm -f ${BINDIR}/*
	rm -rf ${INSTALL_DIR}
	rm  ./*.OUT.txt
	find ${SRCDIR} -name "*.o" -exec rm {} \;
	find ${SRCDIR} -name "*.d" -exec rm {} \;
	find ${SRCDIR} -name "*.a" -exec rm {} \;

.PHONY: depend
depend: $(SRCS)
	makedepend -I$(INCDIR) $^

# DO NOT DELETE THIS LINE -- make depend depends on it.
