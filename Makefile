# Default parameters for gradient descent
PARAMS_DEFAULT = "d"
METHOD_DEFAULT = "gd"
EPS_R_DEFAULT = 1e-6
EPS_S_DEFAULT = 1e-6
K_MAX_DEFAULT = 1000
ALPHA_0_DEFAULT = 0.1
MU_DEFAULT = 0.2
STEP_STRATEGY_DEFAULT = "inv"

# Default parameters for heavy ball
ETA_DEFAULT = 0.9

# Default parameters for ADAM
BETA_1_DEFAULT = 0.9
BETA_2_DEFAULT = 0.9
EPS_DEFAULT = 1e-8

# Default compilation flags
CXXFLAGS = -std=c++20 -O2 -fPIC
CPPFLAGS = -DNDEBUG -I../include
LDFLAGS = -L. -Wl,-rpath=${PWD}
LDLIBS =

# Source files
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
EXEC = main

.PHONY: all clean run_main

all: $(EXEC) run_main

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(EXEC) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXEC)

run_main:
	@./$(EXEC)

configure: $(EXEC)
	@echo "---------- Default parameters -----------"
	@echo "Method: $(METHOD_DEFAULT)"
	@echo "Epsilon r: $(EPS_R_DEFAULT)"
	@echo "Epsilon s: $(EPS_S_DEFAULT)"
	@echo "K max: $(K_MAX_DEFAULT)"
	@echo "Alpha 0: $(ALPHA_0_DEFAULT)"
	@echo "Mu: $(MU_DEFAULT)"
	@echo "Step strategy: $(STEP_STRATEGY_DEFAULT)"
	@echo "Eta: $(ETA_DEFAULT)"
	@echo "Beta 1: $(BETA_1_DEFAULT)"
	@echo "Beta 2: $(BETA_2_DEFAULT)"
	@echo "Epsilon: $(EPS_DEFAULT)"
	@echo "Function: "
	@echo "xy + 4x^4 + y^2 + 3x"
	@echo "Gradient (x-component): "
	@echo "y + 16x^3 + 3"; 
	@echo "Gradient (y-component):" 
	@echo "x + 2y"
	@echo "------------------------------------------"
	@echo "Would you like to use default parameters or customize them?"
	@echo "Note: the function, its gradient and the starting point can be changed on the 'main.cpp' file only."
	@read -p "Enter 'd' for default parameters, or 'c' to customize: " choice; \
	if [ "$$choice" = "d" ]; then \
		./$(EXEC) $(PARAMS_DEFAULT) $(METHOD_DEFAULT) $(EPS_R_DEFAULT) $(EPS_S_DEFAULT) $(K_MAX_DEFAULT) $(ALPHA_0_DEFAULT) $(MU_DEFAULT) $(STEP_STRATEGY_DEFAULT) $(ETA_DEFAULT); \
	elif [ "$$choice" = "c" ]; then \
		read -p "Enter optimization method ('gd' for gradient descent, 'hb' for heavy-ball, 'adam' for ADAM): " method; \
		if [ "$$method" = "gd" ]; then \
			read -p "Enter epsilon_r: " eps_r; \
			read -p "Enter epsilon_s: " eps_s; \
			read -p "Enter k_max: " k_max; \
			read -p "Enter alpha_0: " alpha_0; \
			read -p "Enter mu: " mu; \
			read -p "Enter step strategy: " step_strategy; \
			./$(EXEC) $$choice $$method $$eps_r $$eps_s $$k_max $$alpha_0 $$mu $$step_strategy $(ETA_DEFAULT); \
		elif [ "$$method" = "hb" ]; then \
			read -p "Enter epsilon_r: " eps_r; \
			read -p "Enter epsilon_s: " eps_s; \
			read -p "Enter k_max: " k_max; \
			read -p "Enter alpha_0: " alpha_0; \
			read -p "Enter mu: " mu; \
			read -p "Enter step strategy: " step_strategy; \
			read -p "Enter eta: " eta; \
			./$(EXEC) $$choice $$method $$eps_r $$eps_s $$k_max $$alpha_0 $$mu $$step_strategy $$eta; \
		elif [ "$$method" = "adam" ]; then \
			read -p "Enter epsilon_r: " eps_r; \
			read -p "Enter epsilon_s: " eps_s; \
			read -p "Enter k_max: " k_max; \
			read -p "Enter alpha_0: " alpha_0; \
			read -p "Enter beta 1: " beta_1; \
			read -p "Enter beta 2: " beta_2; \
			read -p "Enter epsilon: " eps; \
			./$(EXEC) $$choice $$method $$eps_r $$eps_s $$k_max $$alpha_0 $$beta_1 $$beta_2 $$eps; \
		else \
			echo "Invalid optimization method. Please enter 'gd' or 'hb'."; \
		fi; \
	else \
		echo "Invalid choice. Please enter 'd' for default parameters, or 'c' to customize."; \
	fi
