import pandas as pd
from datetime import datetime, timedelta

# Define the csv file path
file = 'Assignment_Timecard.xlsx - Sheet1.csv'

# Load the data from the CSV file into a DataFrame
dataframe = pd.read_csv(file)

# "Time" column = "Time In"
dataframe.rename(columns={'Time': 'Time In'}, inplace=True)

# Convert time columns to datetime objects
dataframe['Time In'] = pd.to_datetime(dataframe['Time In'])
dataframe['Time Out'] = pd.to_datetime(dataframe['Time Out'])

# Sorting the DataFrame by "Employee Name" and "Time In" in asending order
dataframe.sort_values(by=['Employee Name', 'Time In'], inplace=True)

# Define the criteria for analysis
consecutive_days = 7
min_time_between_shifts = timedelta(hours=1)
max_time_between_shifts = timedelta(hours=10)
max_shift_duration = timedelta(hours=14)

# Initialize variables to track employee shifts
current_employee = None
current_shift_start = None
previous_shift_end = None

# Initialize sets to store unique answers
employees_A = set()
employees_B = set()
employees_C = set()

for index, row in dataframe.iterrows():
  if (row['Time Out'] - row['Time In']) > max_shift_duration:
    employees_C.add((current_employee,row['Position ID']))

  if current_employee is None or current_employee != row['Employee Name']:
        current_employee = row['Employee Name']
        previous_shift_end = row['Time Out']
        consecutive_count = 1
  else:
        if row['Time In'] - previous_shift_end > min_time_between_shifts and row['Time In'] - previous_shift_end < max_time_between_shifts:
            employees_B.add((current_employee,row['Position ID']))

        # handling null values
        if(pd.notna(row['Time In']) and pd.notna(previous_shift_end)):
            if (row['Time In'].date() - previous_shift_end.date()) > timedelta(days=1):
                consecutive_count = 1
            elif (row['Time In'].date() - previous_shift_end.date()) == timedelta(days=1):
                consecutive_count += 1
            if (pd.notna(row['Time Out']) and (row['Time Out'].date() - row['Time In'].date()) == timedelta(days=1)):
                consecutive_count +=1
        if(consecutive_count == 7):
            employees_A.add((current_employee,row['Position ID']))

        previous_shift_end = row['Time Out']

# print the output to the console
print('Employees who have worked for 7 consecutive days -')
j = 0
for i in employees_A:
  print(i)
print()
print()

print('Employees  who have worked less than 10 hours of time between shifts but greater than 1 hour are -')
for i in employees_B:
  print(i)
print()
print()

print('Employees who have worked for more than 14 hours in a one shift -')
for i in employees_C:
  print(i)
print()
print()
