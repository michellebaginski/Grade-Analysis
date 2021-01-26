#pragma once
/*gradeutil.h*/

//
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 341, Spring 2019
// Project #02: GradeUtil API
//
// Updated by Sean Deitz for
// CS 341 Fall 2019
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;

#include <iostream>
using namespace std;


//
// class Course
//
class Course {
private:
public:
    //
    // Types of course grading:
    //
    enum GradingType { Letter, Satisfactory, Unknown };
    
    //
    // Data members:
    //
    string  Dept;
    string  Title;
    int     Number;
    int     Section;
    string  Instructor;
    int     NumA, NumB, NumC, NumD, NumF, NumI, NumS, NumU, NumW, NumNR;
    
    //
    // Default constructor:
    //
    Course()
    : Dept("?"), Title("?"), Number(0), Section(0), Instructor("?"),
    NumA(0), NumB(0), NumC(0), NumD(0), NumF(0), NumI(0), NumS(0), NumU(0), NumW(0), NumNR(0)
    { }
    
    //
    // Parameterized constructor:
    //
    Course(string dept, string title, int number, int section, string instructor,
           int A, int B, int C, int D, int F, int I, int S, int U, int W, int NR)
    : Dept(dept), Title(title), Number(number), Section(section), Instructor(instructor),
    NumA(A), NumB(B), NumC(C), NumD(D), NumF(F), NumI(I), NumS(S), NumU(U), NumW(W), NumNR(NR)
    { }
    
    //
    // Methods:
    //
    GradingType getGradingType() const
    {
        // any letter grades?
        if ((this->NumA + this->NumB + this->NumC + this->NumD + this->NumF) > 0)
            return Letter;
        else if ((this->NumS + this->NumU) > 0)  // any S or U grades?
            return Satisfactory;
        else
            return Unknown;
    }
    
    int getNumStudents() const
    {
        int N = 0;
        
        // # students = # of grades that were assigned:
        N = this->NumA + this->NumB + this->NumC + this->NumD + this->NumF;
        
        if (N > 0)
        {
            // include any incompletes and "no reports":
            N = N + this->NumI + this->NumNR;
            return N;
        }
        
        // no grades assigned, so perhaps it was graded S/U:
        N = this->NumS + this->NumU;
        
        if (N > 0)
        {
            // include any incompletes and "no reports":
            N = N + this->NumI + this->NumNR;
            return N;
        }
        
        // if we get here, no grades were submitted, so # of
        // students = # of incompletes and "no reports":
        N = this->NumI + this->NumNR;
        return N;
    }
    
};


//
// class Dept
//
// Contains the name of the department (e.g. "CS"), and a vector
// of all the courses taught in the department.
//
class Dept
{
private:
public:
    //
    // Data members:
    //
    string         Name;
    vector<Course> Courses;
    
    //
    // Default constructor:
    //
    Dept()
    : Name("?")
    { }
    
    //
    // Parameterized constructor:
    //
    Dept(string name)
    : Name(name)
    { }
    
    //
    // NumCourses:
    //
    size_t NumCourses() const
    {
        return this->Courses.size();
    }
    
    //
    // NumStudents:
    //
    int NumStudents() const
    {
        int count = 0;
        
        for (const Course& c : this->Courses)
            count += c.getNumStudents();
        
        return count;
    }
    
    void addCourse(Course course);
    
};


//
// class College
//
// Contains the name of the college (e.g. "Engineering"), and a vector
// of all the departments in the college.
//
class College
{
private:
public:
    //
    // Data members:
    //
    string        Name;
    string        Semester;
    int           Year;
    vector<Dept>  Depts;
    
    //
    // Default constructor:
    //
    College()
    : Name("?"), Semester("?"), Year(0)
    { }
    
    //
    // Parameterized constructor:
    //
    College(string name, string semester, int year)
    : Name(name), Semester(semester), Year(year)
    { }
    
    //
    // Num courses:
    //
    size_t NumCourses() const
    {
        size_t count = 0;
        
        for (const Dept& d : this->Depts)
            count += d.NumCourses();
        
        return count;
    }
    
    //
    // Num students:
    //
    int NumStudents() const
    {
        int count = 0;
        
        for (const Dept& d : this->Depts)
            count += d.NumStudents();
        
        return count;
    }
    
    void addDepartment(Dept department);
        
    
};


//
// class GradeStats
//
// Holds data about grade distributions for a course, department, or college.
//
class GradeStats
{
private:
public:
    //
    // Data members:
    //
    int    N;  // total # of grades assigned:
    int    NumA, NumB, NumC, NumD, NumF;  // number of A's, B's, etc.
    double PercentA, PercentB, PercentC, PercentD, PercentF;  // percentage of A's, B's, etc.
    
    //
    // Default constructor:
    //
    GradeStats()
    : N(0), NumA(0), NumB(0), NumC(0), NumD(0), NumF(0),
    PercentA(0.0), PercentB(0.0), PercentC(0.0), PercentD(0.0), PercentF(0.0)
    { }
    
    //
    // Parameterized constructor:
    //
    GradeStats(int n, int numA, int numB, int numC, int numD, int numF,
               double percentA, double percentB, double percentC, double percentD, double percentF)
    : N(n), NumA(numA), NumB(numB), NumC(numC), NumD(numD), NumF(numF),
    PercentA(percentA), PercentB(percentB), PercentC(percentC), PercentD(percentD), PercentF(percentF)
    { }
    
};


//
// API:
//

//
// ParseCourse:
//
// Parses a CSV (comma-separated values) line into a Course
// object, which is then returned.  The given line must have
// the following format:
//
//   Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor
//
// Example:
//   BIOE,101,01,Intro to Bioengineering,22,8,2,1,0,1,0,0,0,5,Eddington
//
// Note the lack of spaces, except perhaps in the title.
// If the given line does not have this format, the behavior
// of the function is undefined (it may crash, it may throw
// an exception, it may return).
// You are probably familiar with A-F, the other letters are as follows
// I – Incomplete, grading has been deferred
// NR – No report, the grade has not been reported
// S – Satisfactory, passing in a course that does not use letter grades
// U – Unsastifactory, failing in a course that does not use letter grades
// W – Withdrawn – student withdrew from the course so no grade could be assigned
//
Course ParseCourse(string csvline);

//
// GetDFWRate:
//
// Returns the DFW rate as a percentage for a given course,
// department, or college.  For a course whose grading type
// is defined as Course::Letter, the DFW rate is defined as
//
//   # of D grades + F grades + Withdrawals
//   -------------------------------------- * 100.0
//   # of A, B, C, D, F grades + Withdrawls
//
// The numerator is returned via the reference parameter DFW;
// the denominator is returned via the reference parameter N.
// If the course grading type is not Course::Letter, the DFW
// rate is 0.0, and parameters DFW and N are set to 0.
//
// When computed for a dept or college, all courses of type
// Course::Letter are considered in computing an overall DFW
// rate for the dept or college.  The reference parameters
// DFW and N are also computed across the dept or college.
//
double GetDFWRate(const Course& c, int& DFW, int& N);
double GetDFWRate(const Dept& dept, int& DFW, int& N);
double GetDFWRate(const College& college, int& DFW, int& N);

//
// GetGradeDistribution
//
// Returns an object containing the grade distribution for a given
// course, dept or college.  For a course whose grading type is
// defined as Course::Letter, the grade distribution is defined by
// the following values:
//
//   N: the # of A, B, C, D, F grades
//   NumA, NumB, NumC, NumD, NumF: # of A, B, C, D, F grades
//   PercentA, PercentB, PercentC, PercentD, PercentF: % of A, B,
//     C, D, F grades.  Example: PercentA = NumA / N * 100.0
//
// If the course grading type is not Course::Letter, all values
// are 0.  When computed for a dept or college, all courses of
// type Course::Letter are considered in computing an overall
// grade distribution for the dept or college.
//
GradeStats GetGradeDistribution(const Course& c);
GradeStats GetGradeDistribution(const Dept& dept);
GradeStats GetGradeDistribution(const College& college);

//
// FindCourses(dept, courseNumber)
//
// Searches the courses in the department for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by section number.
//
vector<Course> FindCourses(const Dept& dept, int courseNumber);

//
// FindCourses(dept, instructorPrefix)
//
// Searches the courses in the department for those whose
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".
//
// If none are found, then the returned vector is empty.  If
// one or more courses are found, copies of the course objects
// are returned in a vector, with the courses appearing in
// ascending order by course number.  If two courses have the
// same course number, they are given in ascending order by
// section number.  Note that courses are NOT sorted by instructor
// name.
//
vector<Course> FindCourses(const Dept& dept, string instructorPrefix);

//
// FindCourses(college, courseNumber)
//
// Searches for all courses in the college for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department, then course number,
// and then section number.
//
vector<Course> FindCourses(const College& college, int courseNumber);

//
// FindCourses(college, instructorPrefix)
//
// Searches all the courses in the college for those whose
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department,
// then course number, and then section number.
//
vector<Course> FindCourses(const College& college, string instructorPrefix);

unordered_map<string, vector<Course>> InputGradeData(string fname, College &col);

College addCoursesToDepts(unordered_map<string, vector<Course>> &map, College &college);

void printDistribution(const GradeStats &stats);

void printDFW(const College &college);

void printDFW(const Dept &dept);

void printDFW(const Course &course);

void printCourseInfo(const size_t &numCourses, const int &numStudents);

void printSearchInfo(string &name, vector<Course> &courses);

void printResults(const vector<Course> &courses);

void summary(const string &input, const College &college);

void search(const string &input, College &college, const unordered_map<string, vector<Course>> &map);

void satisfactory(const string &input, const College &college, const unordered_map<string, vector<Course>> &map);

void dfw(const string &input, const College &college, const unordered_map<string, vector<Course>> &map);

void letterB (const string &input, const College &college, const unordered_map<string, vector<Course>> &map);

double calculateCourseGPA(const Course &c);

double calculateDeptGPA(const Dept &dept);

