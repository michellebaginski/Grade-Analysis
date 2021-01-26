Steps To Running Application
---------------------------------------------------

1. Compile and run the project
2. When running, input "fall-2018.csv" to input the data.

All input files are CSV files that contains two header rows followed by 1 or more rows of data. The first line consists of three values: college name, semester, and year. The second line consists of the column headers, and can be ignored.
The data files are organized as follows: 

Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor 

“I” refers to the # of incompletes, “W” the # of withdrawals, and “NR” the # of “no reports” --- e.g. students who never attended class and thus no grade could be reported

3. Input one of the 6 supported commands to obtain analysis:

summary : Summary of a department or college
search : Search by course # or instructor prefix
satisfactory: List of courses with grading type of satisfactory
dfw: List of courses by DFW rate
letterB: List of courses by percentage of B letter grades
average: List of courses by average GPA. 
