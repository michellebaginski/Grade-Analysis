/*main.cpp*/

//
// <Michelle Baginski>
// U. of Illinois, Chicago
// CS 341, Fall 2019
// Project #03: GradeUtil UI
// This program is C++11 dependent
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <math.h>

using namespace std;

// includes for gradeutil
#include "gradeutil.h"


// parses the .csv file and sets up the data structure - maps each department name <string> to a vector containing that department's courses
unordered_map<string, vector<Course>> InputGradeData(string fname, College &college) {
    unordered_map<string, vector<Course>> map;
    string line, col, sem, year;
    ifstream csv(fname);
    
    if (!csv.is_open()) {
        cout << "Error reading from file.\n";
    }
    
    // get the first line with college info
    getline(csv, line);
    stringstream ss(line);
    
    // parse the first line of the .csv for the college name, semester, year
    getline(ss, col, ',');
    getline(ss, sem, ',');
    getline(ss, year, '\0');
    
    cout << "** " << "College of " << col << ", " << sem << " " << stoi(year) << " ** \n";
    getline(csv, line);
    

    int studentsTaught = 0;
    int students = 0;
    // parse the courses from the file, map the department name to the vector of courses for the dept
    while (getline(csv, line)) {
        Course c = ParseCourse(line);
        
        // get the department name, add it to the list of departments in the college if it does not exist
        if (map.find(c.Dept) == map.end()) {
            college.addDepartment(c.Dept);
        }
        
        map[c.Dept].emplace_back(c);
        students+= c.getNumStudents();
        studentsTaught++;
    }
    
    cout << "# of courses taught: " << studentsTaught << endl;
    cout << "# of students taught: " << students << endl;
    
    return map;
}


// creates a college from the map.
// adds all the courses to their appropriate departments within the college
College addCoursesToDepts(unordered_map<string, vector<Course>> &map, College &college) {

    for (auto &name: map) {
        string deptName = name.first;
        // traverse the college department vector to find the matching department
        for (auto &dept: college.Depts) {
            // initialize the department with the correct courses from the map
            if (deptName == dept.Name) {
                dept.Courses = name.second;
            }
        }
    }
    
    return college;
}


// locates the department within the college, returns it
Dept findDepartment(const College &college, const string &name) {
    
    for (auto &dept: college.Depts) {
        if (dept.Name == name) {
            return dept;
        }
    }
    cout << "**dept not found\n";
    return Dept("?");
}

// displays the grade distribution for a specific course
void printDistribution(const GradeStats &stats) {
    cout << std::fixed;
    cout << std::setprecision(2);
    
    cout << " grade distribution (A-F): " << stats.PercentA << "%, " << stats.PercentB << "%, " << stats.PercentC <<
    "%, " << stats.PercentD <<"%, " << stats.PercentF << "%" << endl;
    
}


// displays DFW rate for a college
void printDFW(const College &college) {
    int DFW, N;
    
    double DFWrate = GetDFWRate(college, DFW, N);
    cout << " DFW rate: " << DFWrate << "%" << endl;
}


// displays DFW rate for a department
void printDFW(const Dept &dept) {
    int DFW, N;
    
    double DFWrate = GetDFWRate(dept, DFW, N);
    cout << " DFW rate: " << DFWrate << "%" << endl;
}


// displays DFW rate for a course
void printDFW(const Course &course) {
    int DFW, N;
    
    double DFWrate = GetDFWRate(course, DFW, N);
    cout << " DFW rate: " << DFWrate << "%" << endl;
}


// outputs the number of courses taught, number of students taught
void printCourseInfo(const size_t &numCourses, const int &numStudents) {
    cout << " # courses taught: " << numCourses << endl;
    cout << " # students taught: " << numStudents << endl;
}


// checks the grading type and returns it as a string
string gradingTypeAsStr(const Course &c) {
    if (c.getGradingType() == Course::GradingType::Letter)
        return "letter";
    else if (c.getGradingType() == Course::GradingType::Satisfactory)
        return "satisfactory";
    else
        return "unknown";
}


// displays the results from the letterB, satisfactory, search functions
void printResults(const vector<Course> &courses) {
    string gradingType;
    for (auto &c: courses) {
        cout << c.Dept << " " << c.Number << " (section " << c.Section << "): " << c.Instructor << endl;
        cout << " # students: " << c.getNumStudents() << endl;
        cout << " course type: " << gradingTypeAsStr(c) << endl;
        printDistribution(GetGradeDistribution(c));
        printDFW(c);
    }
}


// checks if the result of a search is empty, otherwise prints the results
void checkAndPrint(const vector<Course> &courses) {
    if (courses.empty()) {
        cout << "**none found\n";
        return;
    }
    printResults(courses);
}


// displays the summary for the summary function
void printSummary(const Dept &dept) {
    printCourseInfo(dept.NumCourses(), dept.NumStudents());
    printDistribution(GetGradeDistribution(dept));
    printDFW(dept);
}


// calculates the total gpa for a given department
double calculateDeptGPA(const Dept &dept) {
    double GPA;
    int weightA, weightB, weightC, weightD, total=0;
    unordered_map<string, int> gpaToDept;
    int numCourses = 0;
    
    vector<double> courseGPA;
    
    // for each course in the department
    for (auto &c: dept.Courses) {
        if (c.getGradingType() == Course::GradingType::Letter) {
            numCourses++;
            
            total = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
             
            weightA = (c.NumA);
            weightB = (c.NumB);
            weightC = (c.NumC);
            weightD = (c.NumD);

            GPA = (weightA*4.0 + weightB*3.0 + weightC*2.0 + weightD*1.0)/(double)total;
            courseGPA.push_back(GPA);
            gpaToDept[c.Title] = GPA;
    }
}

    GPA = 0;
    // sum all of the course GPAs in that department
    for (auto &x: courseGPA) {
        GPA += x;
    }
    GPA = (GPA)/(double)numCourses;
    return GPA;
}


// calculates the gpa for the course
double calculateCourseGPA(const Course &c) {
    double GPA = 0.0;
    int weightA, weightB, weightC, weightD, total;

    if (c.getGradingType() == Course::GradingType::Letter) {
            total = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
             
            weightA = (c.NumA);
            weightB = (c.NumB);
            weightC = (c.NumC);
            weightD = (c.NumD);

            GPA = (weightA*4.0 + weightB*3.0 + weightC*2.0 + weightD*1.0)/(double)total;
    }
    return GPA;
}


/* The summary command outputs a college-wide summary, or a departmental summary. The user is prompted for a department name, e.g. “CS”, or “all” which means the college. The input is case-sensitive. Here’s a departmental summary:
 */
void summary(const string &input, const College &college, const unordered_map<string, vector<Course>> &map) {
    // calculate department-wide summary
    if (!(input == "all")) {
        
        Dept department = findDepartment(college, input);
        printSummary(department);
    }
    
    // calculate college-wide summary
    if (input == "all") {
        vector <Dept>depts = college.Depts;
        
        // sort in alphabetical order by department name
        std::sort(depts.begin(), depts.end(), [](const Dept &a, const Dept &b) {
            if (a.Name < b.Name) {
                return true;
            }
            else {
                return false;
            }
        });
        
        // display sorted results
        for (auto &dept: depts) {
            cout << dept.Name << ":\n";
            printSummary(dept);
        }
    }
}


/* The search command performs a department or college-wide search by course number or instructor prefix. The user is first prompted for a department name, e.g. “CS”, or “all” which means the college. The user is then prompted to enter a course number or instructor prefix. All input is case-sensitive. Here’s a departmental search for CS 341:
 */
void search(const string &input, College &college, const unordered_map<string, vector<Course>> &map) {
    vector<Course> courses;
    string instructorPrefix;
    bool isNumeric = false;
    int courseNum;
    
    cout << "course # or instructor prefix? ";
    cin >> instructorPrefix;
    stringstream ss(instructorPrefix);
    ss >> courseNum;
    
    // determine whether the input is numeric(course #) or non-numeric (instructor prefix)
    if (ss.fail()) {
        isNumeric = false;
    }
    else {
        isNumeric = true;
    }
    
    // calculate department-wide summary
    if (!(input == "all")) {
        Dept department = findDepartment(college, input);
         // search by course number
         if (isNumeric) {
             courses = FindCourses(department, courseNum);
         }
        // search by instructor prefix
         else if (!isNumeric) {
             courses = FindCourses(department, instructorPrefix);
         }
     }
    
    // calculate college-wide summary
    else if (input == "all") {
        if (isNumeric) {
            courses = FindCourses(college, courseNum);
        }
        else if (!isNumeric) {
            courses = FindCourses(college, instructorPrefix);
        }
    }
    
    // display results
    if (courses.empty() && input == "all") {
             cout << "**none found\n";
    }
    printResults(courses);
}


/* The satisfactory command performs a department or college-wide search for all courses whose Grading type is Course::Satisfactory. The user is prompted for a department name, e.g. “CS”, or “all” which means the college. Here are the satisfactory courses for the ME department (output ordered by course #, then section #):
 */
void satisfactory(const string &input, const College &college, const unordered_map<string, vector<Course>> &map) {
    vector<Course> satisfactoryCourses;
    
    // perform department-wide summary
    if (!(input == "all")) {
        // retrieve the courses in the department
        vector<Course> satCourses= map.at(input);
        
        // get all satisfactory-type courses within that department
        for (auto &x: satCourses) {
            if (x.getGradingType() == Course::GradingType::Satisfactory)
                satisfactoryCourses.push_back(x);
        }
    }

    // perform college-wide summary
    else if (input == "all") {
        
        // get all satisfactory-type courses within the department
        for (auto &depts: college.Depts) {
            for (auto &c: depts.Courses) {
                if (c.getGradingType() == Course::GradingType::Satisfactory)
                    satisfactoryCourses.push_back(c);
            }
        }
    }
    
    // sort the unsatisfactory vector by deptartment name > course number > section number
    sort(satisfactoryCourses.begin(), satisfactoryCourses.end(), [] (Course &a, Course &b) {
        if (a.Dept < b.Dept) {
            return true;
        }
        else if (a.Dept > b.Dept){
            return false;
        }
        else {
            if (a.Number < b.Number) {
                return true;
            }
            else if (a.Number > b.Number) {
                return false;
            }
            else {
                if (a.Section < b.Section) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    });
    
    if (satisfactoryCourses.empty()) {
        cout << "**none found\n";
        return;
    }
    
    // display sorted results
    for (auto &c: satisfactoryCourses) {
        cout << c.Dept << " " << c.Number << " (section " << c.Section << "): " << c.Instructor << endl;
        cout << " # students: " << c.getNumStudents() << endl;
        cout << " course type: satisfactory\n";
    }
}


/* The dfw command performs a department or college-wide search for all courses whose DFW rate exceeds a threshold input by the user. Here’s an example for the CS department with a threshold of 40.0%. The order is descending by DFW rate */
void dfw(const string &input, const College &college, const unordered_map<string, vector<Course>> &map) {
    double threshold;
    string  gradingType;
    cout << "dfw threshold? ";
    cin >> threshold;
    
    vector<Course> dfwList;
    // perform a departmen-wide summary
    if (!(input == "all")) {
        
        if (map.find(input) == map.end()) {
            cout << "**dept not found\n";
            return;
        }
        
        // retrieve the courses in the specified department
        vector<Course> deptCourses = map.at(input);
        
        // add all courses to the list that exceed the threshold entered by the user
        for (auto &c: deptCourses) {
            int DFW, N;
            if (GetDFWRate(c, DFW, N) > threshold) {
                dfwList.push_back(c);
            }
        }
    }
    
    // perform a college-wide simmary
    else if (input == "all") {
        // add all courses to the list that exceed the threshold entered by the user
        for (auto &d: college.Depts) {
            for (auto &c: d.Courses) {
                int DFW, N;
                if (GetDFWRate(c, DFW, N) > threshold) {
                    dfwList.push_back(c);
                }
            }
        }
    }
    
    // sort the dfw vector by dfw > department > course number > section number
    sort(dfwList.begin(), dfwList.end(), [=] (Course &a, Course &b) {
        int DFW, N;
        if (GetDFWRate(a, DFW, N) > GetDFWRate(b, DFW, N)) {
            return true;
        }
        else if (GetDFWRate(a, DFW, N) < GetDFWRate(b, DFW, N)) {
            return false;
        }
        else {
            if (a.Dept < b.Dept) {
                return true;
            }
            else if (a.Dept > b.Dept) {
                return false;
            }
            else {
                if (a.Number < b.Number) {
                    return true;
                }
                else if (a.Number > b.Number) {
                    return false;
                }
                else {
                    if (a.Section < b.Section) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
            }
        }
    });
    
    checkAndPrint(dfwList);
}


/* The letterB command performs a department or college-wide search for all courses whose percentage of letter B grades exceeds a threshold input by the user. Here’s an example for the CS department with a letter B threshold of 50.0%. Notice the output is in descending order the percentage of B grades, where if 2 courses have the same percentage, the output is then ordered by dept, course #, section # (all in ascending order)
 */
void letterB (const string &input, const College &college, const unordered_map<string, vector<Course>> &map) {
    double percentage;
    string gradingType;
    cout << "letter B threshold? ";
    cin >> percentage;
    
    vector<Course> coursePercentages;
    // perform a department-wide summary
    if (!(input == "all")) {
        
        if (map.find(input) == map.end()) {
            cout << "**dept not found\n";
            return;
        }
        
        // get the courses from the department
        vector<Course> deptCourses = map.at(input);
        
        // retrieve the courses that have a greater percentage of B grades than that entered by the user
        for (auto &c: deptCourses) {
            GradeStats stats = GetGradeDistribution(c);
            if (stats.PercentB > percentage) {
                coursePercentages.push_back(c);
            }
        }
    }
    
    // perform a college-wide search
    else if (input == "all") {

        for (auto &d: college.Depts) {
            for (auto &c: d.Courses) {
                GradeStats stats = GetGradeDistribution(c);
                if (stats.PercentB > percentage) {
                    coursePercentages.push_back(c);
                }
            }
        }
    }
    
    // sort the B percentages vector by percentB > department > course number > section number
    sort(coursePercentages.begin(), coursePercentages.end(), [=] (Course &a, Course &b) {
        GradeStats aStats = GetGradeDistribution(a);
        GradeStats bStats = GetGradeDistribution(b);
        
        if ((fabs(aStats.PercentB) < fabs(bStats.PercentB) < .0001)) {
            return true;
        }
        else if ((fabs(aStats.PercentB) > fabs(bStats.PercentB) < .0001)) {
            return false;
        }
        else {
            if (a.Dept < b.Dept) {
                return true;
            }
            else if (a.Dept > b.Dept) {
                return false;
            }
            else {
                if (a.Number < b.Number) {
                    return true;
                }
                else if (a.Number > b.Number) {
                    return false;
                }
                else {
                    if (a.Section < b.Section) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
            }
        }
    });

    checkAndPrint(coursePercentages);
}


/* The average command performs a department or college-wide sort by average GPA. If all departments is chosen, the departments in the college are listed in descending order by average GPA, determined by taking the average of the average GPAs per course. If a specific department is chosen, the courses in that department are listed in descending order of average GPA. If 2 courses or departments have the same percentage, the output is then ordered by dept, course #, section # (all in ascending order)
 */
void average(const string &input, College &college, const unordered_map<string, vector<Course>> &map) {
    Dept department("?");
    
    // perform a department wide search
    if (!(input == "all")) {
        
        department = findDepartment(college, input);
        
        // sort by gpa in descending order, then course number, section number, in ascending order
        sort(department.Courses.begin(), department.Courses.end(), [=] (Course &a, Course &b) {
            double gpaA = calculateCourseGPA(a);
            double gpaB = calculateCourseGPA(b);
            
            if (gpaA > gpaB) {
                return true;
            }
            else if (gpaA < gpaB) {
                return false;
            }
            else {
                if (a.Number < b.Number) {
                    return true;
                }
                else if (a.Number > b.Number) {
                    return false;
                }
                else {
                    if (a.Section < b.Section) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
            }
        });
        
        // display the sorting
        for (auto &course: department.Courses) {
            double gpa = calculateCourseGPA(course);
            if (gpa > .0001)
                cout << "Overall GPA for " << course.Dept << " " << course.Number << "(" << course.Section << ") : " << gpa << endl;
        }
    }
    
    // perform a college wide search
    if (input == "all") {
        
        // sort the output by GPA in descending order, then by department name
        sort(college.Depts.begin(), college.Depts.end(), [=] (Dept &a, Dept&b) {
            double gpaA = calculateDeptGPA(a);
            double gpaB = calculateDeptGPA(b);
            if (gpaA > gpaB) {
                return true;
            }
            else if (gpaA < gpaB) {
                return false;
            }
            else {
                if (a.Name < b.Name) {
                    return true;
                }
                else {
                    return false;
                }
            }
        });
        
        // display the sorting
        for (auto &dept: college.Depts) {
            double gpa = calculateDeptGPA(dept);
            if (gpa > .0001) {
                cout << "Overall GPA for " << dept.Name << " : " << calculateDeptGPA(dept) << endl;
            }
        }
    }
}


int main() {
    College college;
    unordered_map<string, vector<Course>> map;
    
    cout << std::fixed;
    cout << std::setprecision(2);
    
    // take in a file name
    std::string filename;
    std::cin >> filename;
    
    map = InputGradeData(filename, college);
    college = addCoursesToDepts(map, college);
    GradeStats stats = GetGradeDistribution(college);
    
    // display initial output prior to user input
    cout << "grade distribution (A-F): " << stats.PercentA << "%, " << stats.PercentB << "%, " << stats.PercentC <<
    "%, " << stats.PercentD <<"%, " << stats.PercentF << "%" << endl;
    int DFW, N;
    double rate = GetDFWRate(college, DFW, N);
    cout << "DFW rate: " << rate << "%\n\n";
    
    cout << "Enter a command> ";
    string input = "";
    cin >> input;
    
    // loop that handles user input and exits the program on '#'
    while (input != "#") {
        if (input == "summary") {
            cout << "dept name, or all? ";
            cin >> input;
            summary(input, college, map);
        }
        else if (input == "search") {
            cout << "dept name, or all? ";
            cin >> input;
            search(input, college, map);
        }
        else if (input == "satisfactory") {
            cout << "dept name, or all? ";
            cin >> input;
            satisfactory(input, college, map);
        }
        else if (input == "dfw") {
            cout << "dept name, or all? ";
            cin >> input;
            dfw(input, college, map);
        }
        else if (input == "letterB") {
            cout << "dept name, or all? ";
            cin >> input;
            letterB(input, college, map);
        }
        else if (input == "average") {
            cout << "dept name, or all? ";
            cin >> input;
            average(input, college, map);
        }
        else {
            cout << "**unknown command\n";
        }
        
        cout << "Enter a command> ";
        cin >> input;
    }
    
    if (input == "#") exit(0);
    
    return 0;
}
