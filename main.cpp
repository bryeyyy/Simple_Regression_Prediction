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

float linearregression (float dateval[], float priceval[], int finalint){
    float x, y, a, b, d;
    float sumx=0,sumxsq=0,sumy=0,sumxy=0;
    int i;
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

    float predval = a*(finalint)+b; //get the value

    return predval;
}

int main()
{
    string Date, Price, line;
    float dateval[334];
    float priceval[334];
    float userinput, convvalue, predval;
    int i,day,month,year,finalint,key1,key2, key3;

    ifstream inputFile;
    inputFile.open("USD_PHP_Historical_Data.csv");


    // parse and transfer the content of the csv file into the arrays dateval and priceval
    //NOTE: DATES ARE ALREADY CONVERTED TO THEIR NUMERICAL FORMS IN THE CSV -> REFER TO convertdate() for explanation

    for (i = 0; i < 334; i++) {
        getline(inputFile, Date, ',');
        getline(inputFile, Price);

        dateval[i] = stof(Date);
        priceval[i] = stof(Price);
    }
    cout<<"USD-PHP Currency Value Converter and Forecaster"<<endl;

    a:
    cout<<"Choose an Action to Do: "<<endl;
    cout<<"     [1] Convert at Latest Rate"<<endl;
    cout<<"     [2] Predict and Convert"<<endl;
    cout<<"     [3] View Data"<<endl;
    cout<<"     [4] Exit"<<endl;

    //error handling needed
    cin>>key1;
    system ("CLS");

    switch (key1){

        case 1:
            cout<<"USD to PHP or PHP to USD?"<<endl;
            cout<<"     [1]USD to PHP"<<endl;
            cout<<"     [2]PHP to USD"<<endl;

            //error handling needed
            cin>>key2;

            switch(key2){
                case 1:
                    cout<<"Enter of Amount of USD"<<endl;
                    cin>>userinput;

                    convvalue = priceval[333]*userinput;
                    cout<<"\n\n";
                    cout<<"USD-PHP Latest Rate @ October 1, 2022: P"<<priceval[333]<<endl;;
                    cout<<"Converted Value: $"<<userinput<<" -> P"<<convvalue<<endl;
                    //continue?
                    break;
                case 2:
                    cout<<"Enter of Amount of PHP"<<endl;
                    cin>>userinput;

                    convvalue = userinput/priceval[333];
                    cout<<"\n\n";
                    cout<<"\n USD-PHP Latest Rate @ October 1, 2022: P"<<priceval[333]<<endl;
                    cout<<"Converted Value: P"<<userinput<<" -> $"<<convvalue<<endl;
                    //continue?
                    break;
                default:
                    cout<<"Invalid Input. Returning to Main Menu"<<endl;
                    system ("CLS");
                    goto a;
                    break;
            }
        break;

        case 2:
            cout<<"Predict and Convert at Given Date."<<endl;
            cout<<"Enter Month:"<<endl;
            // limit to 1-12 only
            cin>>month;

            cout<<"Enter Day:"<<endl;
            // if month contains 31days, days must be limited to 1-31
            // if month contains 30 days, days must be limited to 1-30
            // if month is 2 (february) , limit the days to 1-28
            cin>>day; // lalagyan pa ng input checkers para maiwasan errors! for now just to test if working

            cout<<"Enter Year:"<<endl;
            // 1901 is the minimum
            cin>> year;

            cout<<"Enter of Amount of USD:"<<endl;
            cin>>userinput;

            finalint = convertdate(day,month,year);
            predval = linearregression(dateval, priceval, finalint);

            cout<<"\n\nPredicted Value of $"<<userinput<<" to PHP @ "<<month<<"/"<<day<<"/"<<year<<" is P"<<predval*userinput<<endl;
        break;

        case 3:
            cout<<"View Data"<<endl;
            cout<<"Date (Numeric Form)  |  Price (USD-PHP)"<<endl;
            for (i = 0; i < 334; i++){
                cout.width(5);
                cout<<dateval[i]<<" | "<<priceval[i]<<endl;
            }
            cout<<"Enter 1 to Return to Main Menu."<<endl;
            cin>>key3;
            if (key3 == 1){
                system ("CLS");
                goto a;
            }
        break;

        case 4:
            exit(0);
        break;

        default:
            cout<<"Invalid Input. Returning to Main Menu"<<endl;
            system ("CLS");
            goto a;
        break;
    }
}
