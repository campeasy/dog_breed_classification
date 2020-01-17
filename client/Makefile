SRC_DIR := src
OBJ_DIR := obj
DEP_DIR := dep
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$(SRC_FILES))
OUT_FILE := main.out

FLAGS := -std=c++11

NODEPS := clean

# We don't want the dep files to be deleted at the end
.PRECIOUS: $(DEP_DIR)/%.d

$(OUT_FILE): $(OBJ_FILES)
	g++ $(FLAGS) -o $@ $^

run: $(OUT_FILE)
	./$(OUT_FILE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | ${OBJ_DIR}
	g++ $(FLAGS) -c -o $@ $<

$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp | ${DEP_DIR}
	g++ $(FLAGS) -MM -MG -MP $< -MT $(patsubst $(DEP_DIR)/%.d,$(OBJ_DIR)/%.o,$@) -MT $@ -MF $(@)

${OBJ_DIR} ${DEP_DIR}:
	mkdir -p $@

clean:
	rm -rf ${OBJ_DIR}
	rm -rf ${DEP_DIR}
	rm -f $(OUT_FILE)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEP_FILES)
endif
