/*
#include <iostream>
#include <vector>

using namespace std;

void analyzePressure(const vector<double>& data, double& minVal, double& maxVal, double& avgVal) {
    
    const double* p = data.data();

   
    minVal = *p;
    maxVal = *p;
    double sum = 0.0;

 
    for (size_t i = 0; i < data.size(); i++) {
        double value = *(p + i);  

       
        if (value < minVal) minVal = value;
        if (value > maxVal) maxVal = value;

  
        sum += value;
    }


    avgVal = sum / data.size();
}

int main() {
   
    vector<double> pressure = { 101.3, 100.8, 101.9, 100.5, 102.0 };

    double minVal, maxVal, avgVal;


    analyzePressure(pressure, minVal, maxVal, avgVal);

   
    cout << "Min pressure: " << minVal << endl;
    cout << "Max pressure: " << maxVal << endl;
    cout << "Average pressure: " << avgVal << endl;

    return 0;
}
*/