/*
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double computeRMS(const vector<double>& signal) {
  
    const double* p = signal.data();

  
    double sum = 0.0;
    for (size_t i = 0; i < signal.size(); i++) {
        sum += *(p + i) * *(p + i); 
    }

   
    return sqrt(sum / signal.size());
}

int main() {
    
    vector<double> signal = { 2.0, 3.0, 1.0, 4.0, 3.0 };

    double rms = computeRMS(signal);

    cout << "RMS value: " << rms << endl;

    return 0;
}
*/