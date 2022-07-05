#--------------------------------------------------------------------------------
# Directories and Libraries
#--------------------------------------------------------------------------------

QUESO_DIR = /home/anna/.install/queso-dev
LIBMESH_DIR = /home/anna/.install/libmesh-1.1.0
SLEPC_DIR = /home/anna/.install/slepc-3.7.3
PETSC_DIR = /home/anna/.install/petsc-3.7.4
MATLAB = /usr/local/MATLAB/R2017a/bin/matlab -nodesktop

INC_PATHS = -I. \
-I$(QUESO_DIR)/include/ \
-I$(LIBMESH_DIR)/include/ \
-I$(BOOST_DIR)/include/ \
-I$(GSL_DIR)/include/ \
-I$(SLEPC_DIR)/include/ \
-I$(PETSC_DIR)/include/

LIBS = -lqueso -L$(QUESO_DIR)/lib/ \
	-lgsl -L$(GSL_DIR)/lib/ \
	-lboost_program_options -L$(BOOST_DIR)/lib

COMP_OPTIONS = -DNDEBUG -g -Wall -w

# Library Options (Configure)
include $(LIBMESH_DIR)/Make.common

# Executable Name
target      := ./models_ode_sifp

#--------------------------------------------------------------------------------
# File Management
#--------------------------------------------------------------------------------

# Source Files
srcfiles 	:= $(wildcard *.C)

# Object Files
objects 	:= $(patsubst %.C, %.o, $(srcfiles))

###############################################################################

.PHONY: dust clean distclean plot mat figures complete run

###############################################################################

.SUFFIXES: .o .C

# Target
all:: $(notdir $(target))

# Production Rules
# Determine how to make the target (depends on library configuration)
$(notdir $(target)): $(objects)
	@echo "Linking "$@"..."
	$(libmesh_CXX) $(libmesh_CXXFLAGS) ${objects} -o $(notdir $(target)) $(libmesh_LIBS) $(LIBS) -Wl,-rpath=$(QUESO_DIR)/lib/

%.o : %.C
	@echo "Compiling C++ (in queso mode) "$<"..."
	$(libmesh_CXX) $(COMP_OPTIONS) $(libmesh_CXXFLAGS) -c $(INC_PATHS) $<

# Useful Rules
dust:
	@echo "Deleting old output and runtime files"
	@rm -rf *~ *# *.e *.txt *.pdf *.jpeg *.png *.eps

clean: dust
	@rm -rf *.o

distclean: clean
	@rm -rf $(notdir $(target)) .libs .depend outputData

run: complete

complete: $(wildcard *.in)
	@$(MAKE) -C $(dir $(target)) $(notdir $(target))
	@echo "***************************************************************"
	@echo "* Running App " $(notdir $(target))
	@echo "***************************************************************"
	@echo " "
	${LIBMESH_RUN} $(target) ./queso_options.in $(model) $(inpfile) ${LIBMESH_OPTIONS} 2>&1 | tee job_output.txt
	@echo " "
	@echo "***************************************************************"
	@echo "* Done Running App " $(notdir $(target))
	@echo "***************************************************************"

plot:
	@./plt.sh

figures:
	@${MATLAB} -nosplash -r "tumor_plots_ip"

mat: figures
	@cp Crop Output/
	@cd Output/ ; make -f Crop; pdftk *.pdf cat output allfig.pdf; mv all*.pdf ../

#--------------------------------------------------------------------------------
# Dependencies
#--------------------------------------------------------------------------------

-include .depend

.depend: $(srcfiles) $(LIBMESH_DIR)/include/libmesh/*.h
	@$(perl) $(LIBMESH_DIR)/contrib/bin/make_dependencies.pl -I. $(foreach i, $(LIBMESH_DIR)/include $(wildcard $(LIBMESH_DIR)/include/*), -I$(i)) "-S\$$(obj-suffix)" $(srcfiles) > .depend
