.mode columns
.headers on
.nullvalue NULL
PRAGMA foreign_keys = ON;

drop table if exists Student;
drop table if exists Course;
drop table if exists Dept;
drop table if exists Enroll;

CREATE TABLE Student(
studentID INT PRIMARY KEY,
studentName TEXT,
major TEXT,
class TEXT,
gpa REAL CHECK(gpa >= 0 AND gpa <= 4.0),
FOREIGN KEY (major) REFERENCES Dept(deptID)
ON UPDATE CASCADE
ON DELETE RESTRICT);

CREATE TABLE Course(
courseNum INT NOT NULL,
deptID TEXT NOT NULL,
courseName TEXT,
location TEXT,
meetDay TEXT CHECK(meetDay != "Sunday" AND meetday != "Saturday"),
meetTime TEXT CHECK(meetTime >= "07:00" AND meetTime <= "17:00"),
PRIMARY KEY (courseNum, deptID),
FOREIGN KEY (deptID) REFERENCES Dept(deptID)
ON UPDATE CASCADE
ON DELETE RESTRICT);

CREATE TABLE Dept(
deptID TEXT PRIMARY KEY CHECK(LENGTH(deptID) <= 4),
name TEXT UNIQUE NOT NULL,
building TEXT
);

CREATE TABLE Enroll(
courseNum INT,
deptID TEXT,
studentID INT,
PRIMARY KEY (courseNum, deptID, studentID),
FOREIGN KEY (courseNum, deptID) REFERENCES Course(courseNum, deptID)
ON UPDATE CASCADE
ON DELETE RESTRICT,
FOREIGN KEY (studentID) REFERENCES Student(studentID)
ON UPDATE CASCADE
ON DELETE RESTRICT);





select studentName, studentID, max(count) FROM (SELECT studentID, count(StudentID) as count from Enroll group by StudentID);
