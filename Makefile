output: predictor_corrector_Euler_test.o
	g++ $^ -o $@

predictor_corrector_Euler_test.o: predictor_corrector_Euler_test.cpp
	g++ -c $< -o $@

clean:
	rm *.o output