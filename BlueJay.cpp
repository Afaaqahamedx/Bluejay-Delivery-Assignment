#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

struct Employee
{
    string positionID;
    string positionStatus;
    tm timeIn;
    tm timeOut;
    string timecardHours;
    string payCycleStartDate;
    string payCycleEndDate;
    string employeeName;
    string fileNumber;
};

// Function to parse a CSV line into Employee structure
Employee parseCSVLine(const string &line)
{
    Employee employee;
    stringstream ss(line);
    getline(ss, employee.positionID, ',');
    getline(ss, employee.positionStatus, ',');
    string timeInStr, timeOutStr;
    getline(ss, timeInStr, ',');
    getline(ss, timeOutStr, ',');
    getline(ss, employee.timecardHours, ',');
    getline(ss, employee.payCycleStartDate, ',');
    getline(ss, employee.payCycleEndDate, ',');
    getline(ss, employee.employeeName, ',');
    getline(ss, employee.fileNumber, ',');

    tm tmIn = {};
    tm tmOut = {};

    sscanf(timeInStr.c_str(), "%d/%d/%d %d:%d", &employee.timeIn.tm_mon, &employee.timeIn.tm_mday,
           &employee.timeIn.tm_year, &employee.timeIn.tm_hour, &employee.timeIn.tm_min);

    sscanf(timeOutStr.c_str(), "%d/%d/%d %d:%d", &employee.timeOut.tm_mon, &employee.timeOut.tm_mday,
           &employee.timeOut.tm_year, &employee.timeOut.tm_hour, &employee.timeOut.tm_min);

    // Adjust tm_mon and tm_year
    employee.timeIn.tm_mon -= 1;
    employee.timeIn.tm_year -= 1900;
    employee.timeOut.tm_mon -= 1;
    employee.timeOut.tm_year -= 1900;  
    return employee;
}

// Function to check if an employee worked for 7 consecutive days
bool workedFor7ConsecutiveDays(const vector<Employee>& employees, int index) {
    if (index + 7 > employees.size())
    {
        return false;
    }

    for (int i = index + 1; i < index + 7; ++i)
    {
        if (employees[i].timeIn.tm_mday - employees[i - 1].timeIn.tm_mday != 1)
        {
            return false;
        }
    }

    return true;
}

// Function to check if an employee has less than 10 hours between shifts but greater than 1 hour
bool lessThan10HoursBetweenShifts(const Employee& emp1, const Employee& emp2) {
    tm timeOutEmp1Copy = emp1.timeOut;
    tm timeInEmp2Copy = emp2.timeIn;

    time_t timeOutEmp1 = mktime(&timeOutEmp1Copy);
    time_t timeInEmp2 = mktime(&timeInEmp2Copy);

    double hoursBetween = difftime(timeInEmp2, timeOutEmp1) / 3600.0;

    return hoursBetween > 1 && hoursBetween < 10;
}

// Function to check if an employee has worked for more than 14 hours in a single shift
bool workedMoreThan14Hours(const Employee& employee) {
    tm timeInCopy = employee.timeIn;
    tm timeOutCopy = employee.timeOut;

    time_t timeInEmp = mktime(&timeInCopy);
    time_t timeOutEmp = mktime(&timeOutCopy);

    double hoursWorked = difftime(timeOutEmp, timeInEmp) / 3600.0;

    return hoursWorked > 14;
}

int main()
{
    ifstream inputFile("Assignment_Timecard.xlsx - Sheet1.csv");
    string line;
    vector<Employee> employees;

    if (!inputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    // Read and parse CSV file
    while (getline(inputFile, line))
    {
        if (!line.empty())
        {
            employees.push_back(parseCSVLine(line));
            
        }
    }
    // Analyze and print results
    for (int i = 0; i < employees.size(); ++i)
    {
        if (workedFor7ConsecutiveDays(employees, i))
        {
            cout << "Employee " << employees[i].employeeName << " worked for 7 consecutive days." << endl;
        }

        if (i > 0 && lessThan10HoursBetweenShifts(employees[i - 1], employees[i]))
        {
            cout << "Employee " << employees[i - 1].employeeName << " and " << employees[i].employeeName << " have less than 10 hours between shifts but greater than 1 hour." << endl;
        }

        if (workedMoreThan14Hours(employees[i]))
        {
            cout << "Employee " << employees[i].employeeName << " worked for more than 14 hours in a single shift." << endl;
        }
    }

    inputFile.close();
    return 0;
}
