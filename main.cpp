#include <iostream>
// date to num conversion
#include <math.h>
#include <cmath>
// parsing csv
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>
// regression
#include <algorithm>
#include <numeric>
#include <vector>


using namespace std;

int convertdate(int day, int month, int year)
{
    // based on excels formula for date to number, where 1/1/1900 is day 1. Therefore, all the
    // proceeding dates after this will be counted in days after this date.
    // note: does not account for leap years!

    int intmonth, intyear, finalint;
    int yeardiff = year-1900;
    intyear = yeardiff*365;

    int month31[7] = {1,3,5,7,8,10,12}; //months with 31 days
    int month30[4] = {4,6,9,11}; //months with 30 days
    int month28 = 2; //february

    for (int i=0; i < 7; i++){
        if (month31[i]== month){
            intmonth = 31;
        } else if (month30[i]== month){
            intmonth = 30;
        } else if (month == month28){
            intmonth = 28;
        }
    }

    //this value will be used in the linear regression x value, because dates cant be used in linear regression

     finalint = intyear+intmonth+day;
     return finalint;
};

/*
void polyRegression(vector<double>& x, vector<double>& y, int finalint) {
    int n = x.size();
    vector<int> r(n);
    iota(r.begin(), r.end(), 0);
    double xm = accumulate(x.begin(), x.end(), 0.0) / x.size();
    double ym = accumulate(y.begin(), y.end(), 0.0) / y.size();
    double x2m = transform_reduce(r.begin(), r.end(), 0.0, plus<double>{}, [](double a) {return a * a; }) / r.size();
    double x3m = transform_reduce(r.begin(), r.end(), 0.0, plus<double>{}, [](double a) {return a * a * a; }) / r.size();
    double x4m = transform_reduce(r.begin(), r.end(), 0.0, plus<double>{}, [](double a) {return a * a * a * a; }) / r.size();

    double xym = transform_reduce(x.begin(), x.end(), y.begin(), 0.0, plus<double>{}, multiplies<double>{});
    xym /= fmin(x.size(), y.size());

    double x2ym = transform_reduce(x.begin(), x.end(), y.begin(), 0.0, plus<double>{}, [](double a, double b) { return a * a * b; });
    x2ym /= fmin(x.size(), y.size());

    double sxx = x2m - xm * xm;
    double sxy = xym - xm * ym;
    double sxx2 = x3m - xm * x2m;
    double sx2x2 = x4m - x2m * x2m;
    double sx2y = x2ym - x2m * ym;

    double b = (sxy * sx2x2 - sx2y * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
    double c = (sx2y * sxx - sxy * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
    double a = ym - b * xm - c * x2m;

    auto abc = [a, b, c](int xx) {
        return a + b * xx + c * xx*xx;
    };

    cout << "y = " << a << " + " << b << "x + " << c << "x^2" << endl;
    double datenum = finalint;
    double prediction = a + b*datenum + c*pow(datenum, 2);
    cout<< "Predicted Price of One USD to PHP is: P"<<prediction<<endl;
}
*/


int main()
{
    string Date, Price;
    float dateval[334];
    float priceval[334];
    float x, y, a, b, d;
    float sumx=0,sumxsq=0,sumy=0,sumxy=0;
    int i,day,month,year,finalint;


    ifstream inputFile;
    inputFile.open("USD_PHP_Historical_Data.csv");

    string line;

    // parse and transfer the content of the csv file into the arrays dateval and priceval
    //NOTE: DATES ARE ALREADY CONVERTED TO THEIR NUMERICAL FORMS IN THE CSV -> REFER TO convertdate() for explanation

    for (i = 0; i < 334; i++) {
        getline(inputFile, Date, ',');
        getline(inputFile, Price);

        dateval[i] = stof(Date);
        priceval[i] = stof(Price);
    }

   // vector<float> datevec(dateval, dateval + sizeof dateval/ sizeof dateval[0]);
   // vector<float> pricevec(priceval, priceval + sizeof priceval/ sizeof priceval[0]);

    cout<<"Enter Day:"<<endl;
    // if month contains 31days, days must be limited to 1-31
    // if month contains 30 days, days must be limited to 1-30
    // if month is 2 (february) , limit the days to 1-28
    cin>>day; // lalagyan pa ng input checkers para maiwasan errors! for now just to test if working
    cout<<"Enter Month:"<<endl;
    // limit to 1-12 only
    cin>>month;
    cout<<"Enter Year"<<endl;
    // 1901 is the minimum
    cin>> year;
    finalint = convertdate(day,month,year);

    //linear regression prediction model //polyRegression(datevec, pricevec, finalint);
    //using linear regression does not guarantee accuracy. r^2 = 0.21
    for (i=0; i< 334; i++){
        x = dateval[i];
        y = priceval[i];

        sumx=sumx+x;
        sumxsq=sumxsq+(x*x);
        sumy=sumy+y;
        sumxy=sumxy+(x*y);
    }
    //linear regression is basically y=ax+b

    d=334*sumxsq-sumx*sumx; //denominator
    a=(334*sumxy-sumx*sumy)/d; //ax
    b=(sumy*sumxsq-sumx*sumxy)/d; //b

    cout<<"Value of a: "<<a<<" "<<"Value of b: "<<b<<endl;
    cout<<"Best Fit Line: "<<a<<"x"<<b<<endl;

    cout<<"============================="<<endl;
    float predval = a*(finalint)+b;
    cout<<"Predicted Value of One USD to PHP @ "<<month<<"/"<<day<<"/"<<year<<" is "<<predval<<endl;

}
