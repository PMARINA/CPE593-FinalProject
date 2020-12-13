BUILD_DIR = build
SOURCE = src
MATH = $(SOURCE)/math
DATA_PROCESS = $(SOURCE)/data_process
OBJ = $(SOURCE)/obj
EULER_TEST = $(BUILD_DIR)/output

EULER_TEST_MAIN: make_build $(BUILD_DIR)/output
	@echo "Successfully built. Run $(EULER_TEST) to run the Euler Demo"

$(EULER_TEST): $(BUILD_DIR)/predictor_corrector_Euler_test.o
	g++ $^ -o $@

$(BUILD_DIR)/predictor_corrector_Euler_test.o: $(MATH)/predictor_corrector_Euler_test.cpp
	g++ -c $< -o $@

make_build: 
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)