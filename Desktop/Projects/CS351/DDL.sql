.mode columns
.headers on
.nullvalue NULL
PRAGMA foreign_keys = ON;

drop table if exists Grants;
drop table if exists Associates;
drop table if exists Faculty;
drop table if exists FacultyPhone;
drop table if exists FacultyEmail;
drop table if exists Course;
drop table if exists Sect;
drop table if exists Belong;
drop table if exists Department;
drop table if exists Major;
drop table if exists Enroll;
drop table if exists Student;
drop table if exists AcademicCoordinator;
drop table if exists AcademicCoordinatorPhone;
drop table if exists AcademicCoordinatorEmail;

CREATE TABLE Grants(
    awardNum INT PRIMARY KEY,
    amountLeft INT,
    amount INT,
    beginDate TEXT,
    endDate TEXT,
    awardTitle TEXT
);

CREATE TABLE Associates(
    awardNum INT,
    facultyID INT,
    PRIMARY KEY (awardNum, facultyID),
    FOREIGN KEY (awardNum) REFERENCES Grants(awardNum)
    ON UPDATE CASCADE ON DELETE RESTRICT,
    FOREIGN KEY (facultyID) REFERENCES Faculty(id)
    ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE Faculty(
    id INT PRIMARY KEY,
    name TEXT,
    salary INT,
    rank TEXT CHECK (rank = "Assistant Professor"
                  OR rank = "Associate Professor"
                  OR rank = "Professor"),
    officeBldg TEXT,
    officeRoomNum INT
);

CREATE TABLE FacultyPhone(
   id INT,
   phone TEXT,
   PRIMARY KEY (id, phone),
   FOREIGN KEY (id) REFERENCES Faculty(id)
      ON UPDATE CASCADE ON DELETE RESTRICT
);
 
CREATE TABLE FacultyEmail(
   id INT,
   email TEXT,
   PRIMARY KEY (id, email),
   FOREIGN KEY (id) REFERENCES Faculty(id)
      ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE Course(
    courseNum INT NOT NULL,
    deptID TEXT,
    courseName TEXT,
    PRIMARY KEY (deptID, courseNum),
    FOREIGN KEY (deptID) REFERENCES Department(deptID)
    	    ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE Sect(
    secNum INT,
    bldg TEXT,
    roomNum INT,
    meetDay TEXT CHECK (meetDay != "Sunday" AND meetDay != "Saturday"),
    meetTime TEXT CHECK (meetTime >= "07:00" AND meetTime <= "17:00" AND meetTime LIKE '%:%'),
    courseNum INT,
    deptID INT,
    facultyID INT,
    PRIMARY KEY (secNum, courseNum, deptID),
    FOREIGN KEY (courseNum, deptID) REFERENCES Course(courseNum, deptID)
        ON UPDATE CASCADE ON DELETE RESTRICT,
    FOREIGN KEY (facultyID) REFERENCES Faculty(id)
        ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE Belong(
    facultyID INT,
    deptID TEXT,
    researchInterest TEXT,
    PRIMARY KEY (facultyID, deptID),
    FOREIGN KEY (facultyID) REFERENCES Faculty(id)
    ON UPDATE CASCADE ON DELETE RESTRICT,
    FOREIGN KEY (deptID) REFERENCES Department(deptID)
    ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE Department(
    deptID TEXT PRIMARY KEY CHECK (LENGTH(deptID) <= 4),
    name TEXT UNIQUE NOT NULL,
    building TEXT
);

CREATE TABLE Major(
    studentID INT,
    deptID TEXT,
    PRIMARY KEY (studentID, deptID),
    FOREIGN KEY (studentID) REFERENCES Student(studentID)
    ON UPDATE CASCADE ON DELETE RESTRICT,
    FOREIGN KEY (deptID) REFERENCES Department(deptID)
    ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE Enroll(
    courseNum INT,
    deptID TEXT,
    studentID INT,
    secNum INT,
    PRIMARY KEY (studentID, secNum, courseNum, deptID),
    FOREIGN KEY (studentID) REFERENCES Student(studentID)
    	    ON UPDATE CASCADE ON DELETE RESTRICT,
    FOREIGN KEY (secNum, courseNum, deptID) REFERENCES Sect(secNum, courseNum, deptID)
    	    ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE Student(
    studentID INT PRIMARY KEY,
    gpa REAL CHECK(gpa >= 0 AND gpa <= 4.0),
    studentName TEXT,
    academicCoordID INT,
    FOREIGN KEY (academicCoordID)
        REFERENCES AcademicCoordinator(id)
    ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE AcademicCoordinator(
    id INT PRIMARY KEY,
    name TEXT,
    salary INT,
    officeBldg TEXT,
    officeRoomNum INT
);

CREATE TABLE AcademicCoordinatorPhone(
   id INT,
   phone TEXT,
   PRIMARY KEY (id, phone),
   FOREIGN KEY (id) REFERENCES AcademicCoordinator(id)
      ON UPDATE CASCADE ON DELETE RESTRICT
);
 
CREATE TABLE AcademicCoordinatorEmail(
   id INT,
   email TEXT,
   PRIMARY KEY (id, email),
   FOREIGN KEY (id) REFERENCES AcademicCoordinator(id)
      ON UPDATE CASCADE ON DELETE RESTRICT
);
