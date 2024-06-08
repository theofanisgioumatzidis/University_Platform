#include <iostream>
#include <sqlite3.h>
#include <filesystem>

using namespace std;
void init_db();
void bootstrap_db();
int main(){


    cout << "Wellcome to the University Platform" << endl;
    if(!filesystem::exists("University_Platform.db")){
        // Creating an sql database if it doesnt exist
        init_db();
        bootstrap_db();
    }

    return 0;
}


void init_db(){
    // Creating the database tables

    int errors {};
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("University_Platform.db", &DB);

    if (exit) {
        cerr << "Error openning DB " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Creating Users table
    string sql =    "CREATE TABLE IF NOT EXISTS Users ("
                    "User_id INT ,"
                    "Username	TEXT NOT NULL,"
                    "Password	TEXT NOT NULL,"
                    "Credentials	TEXT NOT NULL,"
                    "PRIMARY KEY(User_id)"
                    " );";

    exit = 0;
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating Users Table" << endl;
        cout<< messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Creating Students table
    sql =           "CREATE TABLE IF NOT EXISTS Students ("
                    "S_id INT ,"
                    "User_id INT ,"
                    "FirstName	TEXT,"
                    "LastName	TEXT,"
                    "Age	INT,"
                    "StartDate  TEXT,"
                    "StudyYear INT ,"
                    "FOREIGN KEY(User_id) REFERENCES Users(User_id) ON DELETE CASCADE,"
                    "PRIMARY KEY(S_id)"
                    " );";

    exit = 0;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating Students Table" << endl;
        cout<< messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Creating Employees table
    sql =           "CREATE TABLE IF NOT EXISTS Employees ("
                    "E_id INT ,"
                    "User_id INT ,"
                    "FirstName	TEXT,"
                    "LastName	TEXT,"
                    "Age	INT,"
                    "Wage	INT,"
                    "StartDate  TEXT,"
                    "Role TEXT ,"
                    "FOREIGN KEY(User_id) REFERENCES Users(User_id) ON DELETE CASCADE,"
                    "PRIMARY KEY(E_id)"
                    " );";

    exit = 0;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating Employees Table" << endl;
        cout<< messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Creating Courses table
    sql =           "CREATE TABLE IF NOT EXISTS Courses ("
                    "C_id INT ,"
                    "E_id INT ,"
                    "CourseTitle	TEXT,"
                    "StudyYear	INT,"
                    "Semester	TEXT,"
                    "WritingMarkPerc	INT,"
                    "PRIMARY KEY(C_id)"
                    " );";

    exit = 0;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating Courses Table" << endl;
        cout<< messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Creating CourseTimetable table
    sql =           "CREATE TABLE IF NOT EXISTS CourseTimetable ("
                    "C_id INT ,"
                    "Day	TEXT,"
                    "StartTime	INT,"
                    "EndTime	INT,"
                    "FOREIGN KEY(C_id) REFERENCES Courses(C_id) ON DELETE CASCADE,"
                    "PRIMARY KEY(C_id,Day,StartTime)"
                    " );";

    exit = 0;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating CourseTimetable Table" << endl;
        cout<< messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Creating CourseEnrolled table
    sql =           "CREATE TABLE IF NOT EXISTS CourseEnrolled ("
                    "S_id INT ,"
                    "C_id INT ,"
                    "OralMark	INT,"
                    "WritingMark	INT,"
                    "FinalMark	INT,"
                    "FOREIGN KEY(S_id) REFERENCES Students(S_id) ON DELETE CASCADE,"
                    "PRIMARY KEY(S_id,C_id)"
                    " );";

    exit = 0;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating CourseEnrolled Table" << endl;
        cout<< messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Creating CleanningSchedule table
    sql =           "CREATE TABLE IF NOT EXISTS CleanningSchedule ("
                    "The_Date TEXT ,"
                    "Shift TEXT ,"
                    "E_id	INT,"
                    "FOREIGN KEY(E_id) REFERENCES Employees(E_id) ON DELETE CASCADE,"
                    "PRIMARY KEY(The_Date,Shift)"
                    " );";

    exit = 0;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating CleanningSchedule Table" << endl;
        cout<< messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    if(errors == 0){
        cout << "Database created successfully!" << endl;
    }else{
        cout << errors << " Tables return an error upon creation" << endl;
    }

    sqlite3_close(DB);
}

void bootstrap_db(){
    // Inserting sample data

    int errors {};
    sqlite3* DB;
    char* messaggeError;
    int exit;
    string sql;

    exit = sqlite3_open("University_Platform.db", &DB);
    if (exit != SQLITE_OK) {
        cerr << "Error opening Db on bootstrap_db function" << endl;
        sqlite3_free(messaggeError);
        return;
    }

    // Inserting the User login info
    sql   =     "INSERT INTO Users ( User_id, Username, Password, Credentials)"
                "VALUES"
                "(1,'admin','admin','admin'),"
                "(2,'master_admin','master_admin','admin'),"
                "(3,'GFanis','1234','User'),"
                "(4,'Maria99','1234','User'),"
                "(5,'XTrider','1234','User'),"
                "(6,'chris_skater','1234','User'),"
                "(7,'Kostas33','1234','User'),"
                "(8,'Txrisa','1234','User'),"
                "(9,'G_Papoulias','1234','User'),"
                "(10,'Tazes','1234','User'),"
                "(11,'Souka','1234','User'),"
                "(12,'Nikos_Sherlock','1234','User'),"
                "(13,'Nikos_Zen','1234','User'),"
                "(14,'happyapple','1234','User'),"
                "(15,'Katerina95','1234','User'),"
                "(16,'Mitsaras','1234','User'),"
                "(17,'Anna97','1234','User'),"
                "(18,'Eleni_Star','1234','User'),"
                "(19,'Panos_G4','1234','User'),"
                "(20,'Giannis_G3','1234','User'),"
                "(21,'Ntina99','1234','User'),"
                "(22,'Filippos','1234','User'),"
                "(23,'B_Pavlidou','1234','User'),"
                "(24,'I_Papadakis','1234','User'),"
                "(25,'G_Tsironis','1234','User'),"
                "(26,'Ks_Zotos','1234','User'),"
                "(27,'S_Trahanas','1234','User')"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert Users" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Inserting Student info
    sql   =     "INSERT INTO Students (S_id, User_id, FirstName , LastName, Age, StartDate,StudyYear)"
                "VALUES"
                "(1,3,'Fanis','Gioumatzidis',25,'09/2021',NULL),"
                "(2,4,'Maria','Mpoutkari',24,'09/2021',NULL),"
                "(3,5,'Sakis','Xristodoulakis',25,'09/2021',NULL),"
                "(4,6,'Christos','Markou',27,'09/2021',NULL),"
                "(5,7,'Kostantinos','Aivazidis',22,'09/2021',NULL),"
                "(6,8,'Xrisa','Tousidou',23,'09/2022',2),"
                "(7,9,'Giorgos','Papoulias',23,'09/2022',2),"
                "(8,10,'Christos','Tazes',23,'09/2022',2),"
                "(9,11,'Thanasis','Soukaras',27,'09/2022',2),"
                "(10,12,'Nikos','Karantonis',25,'09/2022',2),"
                "(11,13,'Nikos','Kokkinidis',25,'09/2023',1),"
                "(12,14,'Chara','Mylona',25,'09/2023',1),"
                "(13,15,'Katerina','Soulidopoulou',27,'09/2023',1),"
                "(14,16,'Dimitris','Giannousis',27,'09/2023',1),"
                "(15,17,'Anna','Petridou',23,'09/2023',1)"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert Students" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }


    // Inserting Courses info
    sql   =     "INSERT INTO Courses ( C_id, E_id, CourseTitle , StudyYear,Semester,WritingMarkPerc)"
                "VALUES"
                "(1,23,'Electrodynamics',1,'Fall',7),"
                "(2,25,'Introduction to Machine Learning',1,'Spring',8),"
                "(3,26,'Computational Physics',2,'Fall',8),"
                "(4,27,'Quantum Mechanics',2,'Spring',7)"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert Courses" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Inserting Student Enrollment info for past classes
    sql   =     "INSERT INTO CourseEnrolled ( S_id, C_id, OralMark , WritingMark)"
                "VALUES"
                // First semester
                "(1,1,80,67),"
                "(2,1,60,87),"
                "(3,1,60,55),"
                "(4,1,60,60),"
                "(5,1,80,90),"
                "(6,1,30,60),"
                "(7,1,50,55),"
                "(8,1,50,75),"
                "(9,1,70,76),"
                "(10,1,50,80),"
                "(11,1,70,67),"
                "(12,1,60,60),"
                "(13,1,80,88),"
                "(14,1,90,85),"
                "(15,1,50,50),"
                //Second semester
                "(1,2,90,77),"
                "(2,2,50,67),"
                "(3,2,70,85),"
                "(4,2,70,90),"
                "(5,2,90,80),"
                "(6,2,30,60),"
                "(7,2,60,55),"
                "(8,2,60,65),"
                "(9,2,80,66),"
                "(10,2,60,90),"
                //Third semester
                "(1,3,80,97),"
                "(2,3,60,60),"
                "(3,3,60,80),"
                "(4,3,70,80),"
                "(5,3,80,50),"
                "(6,3,60,65),"
                "(7,3,80,50),"
                "(8,3,70,75),"
                "(9,3,80,76),"
                "(10,3,90,60),"
                //Forth semester
                "(1,4,60,97),"
                "(2,4,70,80),"
                "(3,4,70,80),"
                "(4,4,90,80),"
                "(5,4,70,90)"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert CourseEnrolled" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Calculating and filling the Final Mark feature
    sql =       "UPDATE CourseEnrolled "
                "SET FinalMark = (OralMark * (10 - (SELECT WritingMarkPerc FROM Courses WHERE CourseEnrolled.C_id = Courses.C_id)) / 10 + "
                "WritingMark * (SELECT WritingMarkPerc FROM Courses WHERE CourseEnrolled.C_id = Courses.C_id) / 10);";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert Final Mark feature" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Inserting Students that are enrolled this semester
    sql   =     "INSERT INTO CourseEnrolled ( S_id, C_id)"
                "VALUES"
                //-- Second semester
                "(11,2),"
                "(12,2),"
                "(13,2),"
                "(14,2),"
                "(15,2),"
                //-- Forth semester
                "(6,4),"
                "(7,4),"
                "(8,4),"
                "(9,4),"
                "(10,4)"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert CourseEnrolled" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Inserting The cleanning Schedule
    sql   =     "INSERT INTO CleanningSchedule ( The_Date,Shift, E_id)"
                "VALUES"
                "('05/06/2024','Morning',8),"
                "('05/06/2024','Afternoon',9),"
                "('06/06/2024','Morning',10),"
                "('06/06/2024','Afternoon',8),"
                "('07/06/2024','Morning',9),"
                "('07/06/2024','Afternoon',10),"
                "('08/06/2024','Morning',8),"
                "('08/06/2024','Afternoon',9),"
                "('09/06/2024','Morning',10),"
                "('09/06/2024','Afternoon',8),"
                "('10/06/2024','Morning',9),"
                "('10/06/2024','Afternoon',10),"
                "('11/06/2024','Morning',8),"
                "('11/06/2024','Afternoon',9),"
                "('12/06/2024','Morning',10),"
                "('12/06/2024','Afternoon',8)"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert CleanningSchedule" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Inserting The Courses Timetable
    sql   =     "INSERT INTO CourseTimetable ( C_id, Day, StartTime, Endtime)"
                "VALUES"
                "(1,'Friday',1800,2000),"
                "(1,'Saturday',2000,2200),"
                "(2,'Friday',1800,2000),"
                "(2,'Saturday',2000,2200),"
                "(3,'Friday',1800,2000),"
                "(3,'Saturday',2000,2200),"
                "(4,'Friday',1800,2000),"
                "(4,'Saturday',2000,2200)"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert CourseTimetable" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }
    if(errors == 0){
        cout << "Database filled successfully!" << endl;
    }else{
        cout << errors << " Tables return an error while incerting" << endl;
    }

    sqlite3_close(DB);

}
