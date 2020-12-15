BUILD = build
SOURCE = src
MATH = $(SOURCE)/math
DATA_PROCESS = $(SOURCE)/data_process
OBJ = $(SOURCE)/obj
EULER_TEST = $(BUILD)/output
READER_TEST = $(BUILD)/reader
MAIN = src/main.cpp

READER_TEST_MAIN: make_build $(READER_TEST)
	@echo "Successfully built. Run $(READER_TEST) to test the reader."

$(READER_TEST): $(BUILD)/planetary_object.o $(MAIN)
	g++ -g3 $^ -o $@

PLANETARY_OBJ_DEP = $(OBJ)/planetary_object.cpp $(OBJ)/planetary_object.hh

$(BUILD)/planetary_object.o: $(PLANETARY_OBJ_DEP)
	g++ -c -g3 $< -o $@

EULER_TEST_MAIN: make_build $(EULER_TEST)
	@echo "Successfully built. Run $(EULER_TEST) to run the Euler Demo"

$(EULER_TEST): $(BUILD)/predictor_corrector_Euler_test.o
	g++ -g3 $^ -o $@

$(BUILD)/predictor_corrector_Euler_test.o: $(MATH)/predictor_corrector_Euler_test.cpp
	g++ -c -g3 $< -o $@

make_build: 
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)