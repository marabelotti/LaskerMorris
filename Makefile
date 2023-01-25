CXX			:= g++
SRC_DIR		:= src
OBJ_DIR 	:= obj
TEST_DIR 	:= test
BIN_DIR 	:= .


EXE 		:= $(BIN_DIR)/NineMenMorris
EXE_TEST	:= $(BIN_DIR)/test1
SRC 		:= $(wildcard $(SRC_DIR)/*.cpp)
SRC_TEST	:= $(wildcard $(TEST_DIR)/*.cpp)
OBJ 		:= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ_TEST	:= $(filter-out $(OBJ_DIR)/main.o, $(OBJ)) $(SRC_TEST:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS 	:= -Iinclude -MMD -MP
CFLAGS   	:= -std=c++17
LDFLAGS 	:= `sdl2-config --libs` -Llib
LDLIBS		:= -lSDL2 -lSDL2_image -lSDL2_ttf

# --------------------------------------------------------------
.PHONY: all

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	g++ -fopenmp $(LDFLAGS) $^ $(LDLIBS) -o $@



$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $() | $(OBJ_DIR)
	g++ -fopenmp $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# --------------------------------------------------------------
.PHONY: test

test: $(EXE_TEST)

$(EXE_TEST): $(OBJ_TEST)
	g++ -fopenmp $(LDFLAGS) $^ $(LDLIBS) -o $@ && echo "[CREATED EXECUTABLE]  $@"

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp 
	g++ -fopenmp $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# --------------------------------------------------------------
.PHONY: clean

clean:
	@$(RM) -rv $(OBJ_DIR)
	@$(RM) -rv $(EXE) $(EXE_TEST)

-include $(OBJ:.o=.d)
