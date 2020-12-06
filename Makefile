output: predictor_corrector_Euler_test.o
	g++ predictor_corrector_Euler_test.o -o output

predictor_corrector_Euler_test.0: predictor_corrector_Euler_test.cpp
	g++ -c predictor_corrector_Euler_test.cpp

clean:
	rm *.o output