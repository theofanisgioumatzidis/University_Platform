#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <string>
#include <vector>
#include <iomanip>
#include <windows.h>

using namespace std;

// Structure to hold the table data
struct TableData {
    vector<string> columnNames;
    vector<vector<string>> rows;
};

void init_db();
void bootstrap_db();
static int callback(void* data, int argc, char** argv, char** azColName);
void login(int &User, string &User_Capacity, int &error_login);
void Print_Query(string sql, int error);
int Retrieve_User_id(string FirstName, string LastName, int error);
void Delete_User(int User_id, int error);
bool IsEntryExists_One_Value(sqlite3* db, const string& tableName, const string& columnName, const string& value);
bool IsEntryExists_Two_Values(sqlite3* db, const string& tableName, const string& columnName1, const string& value1, const string& columnName2, const string& value2);
void Insert_User(int error);



int main(){
    // Creating the SQL database if it doesnt exist and filling with data
    if(!filesystem::exists("University_Platform.db")){
            init_db();
            bootstrap_db();
    }
    int User_id ;
    string User_Capacity;

    cout << "Wellcome to the University Platform" << endl;


    while(true){
/*
            //// testing
            int error{-1};
            Insert_User(error);
            string test_action;
            cout << "Continue? [Yes -> y, No -> n] ";
            cin >> test_action;
            if (test_action == "y"){

            }else{
                return 0;
            }




            //// testing
*/


            string action;
            cout << "Do you want to login or terminate the program?" << endl;
            cout << "[Login -> login, Terminate -> terminate] ";
            cin >> action;
            if (action == "terminate")
                return 0;
            if(action != "login"){
                cout << "Wrong input." << endl;
                continue;
            }
            int error_login {};
            User_id = -1;
            User_Capacity = "Nothing";
            login(User_id, User_Capacity, error_login);
            if(error_login){
                cout << "Invalid username or password." << endl;
                continue;
            }

        cout << "Login successful!" << endl;



        while(User_Capacity == "admin"){
                    cout << "What you want to do? [See users -> see ," << endl;
                    cout << "                      Edit entity -> edit," << endl;
                    cout << "                      Insert entity -> insert," << endl;
                    cout << "                      Delete entity -> delete," << endl;
                    cout << "                      Logout -> logout] ";
                    cin >> action;

                    if (action == "logout"){
                        break;
                    }else if( action == "see"){
                            int error {};
                            string sql;
                            sql =              string("SELECT FirstName, LastName, Age, Wage, StartDate, NULL AS StudyYear "
                                               "FROM Employees "
                                               "UNION ALL "
                                               "SELECT FirstName, LastName, Age, NULL AS Wage, StartDate, StudyYear "
                                               "FROM Students "
                                               ";");
                            Print_Query(sql,error);
                    }else if( action == "insert" ){
                        int error{-1};
                        Insert_User(error);
                    }
                    continue;
        }
        while(User_Capacity == "Boarding"){
                    int error {};
                    string sql;
                    sql =              string("SELECT FirstName, LastName, Age, Wage, StartDate, NULL AS StudyYear "
                                       "FROM Employees "
                                       "WHERE "
                                       "UNION ALL "
                                       "SELECT FirstName, LastName, Age, NULL AS Wage, StartDate, StudyYear "
                                       "FROM Students "
                                       ";");
                    Print_Query(sql,error);
                    break;
        }

        while(User_Capacity == "Student"){
                    int error {};
                    string sql;
                    sql =              string("SELECT C.CourseTitle, C.StudyYear, C.Semester, C.WritingMarkPerc, CE.OralMark, CE.WritingMark, CE.FinalMark "
                                       "FROM CourseEnrolled as CE, Students as S, Courses as C "
                                       "WHERE "
                                       "S.S_id = CE.S_id AND "
                                       "S.User_id = ") + to_string(User_id) + string(" AND "
                                       "C.C_id = CE.C_id "
                                       ";");
                    Print_Query(sql,error);
                    break;
        }

        while(User_Capacity == "Teacher"){
                    int error {};
                    string sql;
                    sql =              string("SELECT C.CourseTitle, C.StudyYear, C.Semester, CT.Day, CT.StartTime, CT.EndTime, C.WritingMarkPerc "
                                       "FROM CourseTimetable as CT, Employees as E, Courses as C "
                                       "WHERE "
                                       "E.E_id = C.E_id AND "
                                       "E.User_id = ") + to_string(User_id) + string(" AND "
                                       "C.C_id = CT.C_id "
                                       ";");
                    Print_Query(sql,error);
                    break;
        }
        while(User_Capacity == "Cleaner"){
                    int error {};
                    string sql;
                    sql =              string("SELECT CS.The_Date, CS.Shift "
                                       "FROM CleaningSchedule as CS, Employees as E "
                                       "WHERE "
                                       "E.E_id = CS.E_id AND "
                                       "E.User_id = ") + to_string(User_id) + string(" "
                                       ";");
                    Print_Query(sql,error);
                    break;
        }
    }
    return 0;
}

void init_db(){
    // Creating the database tables

    int errors {};
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("University_Platform.db", &DB);

    if (exit != SQLITE_OK) {
        cerr << "Error openning DB " << sqlite3_errmsg(DB) << endl;

        return;
    }

    // Creating Users table
    string sql =    "CREATE TABLE IF NOT EXISTS Users ("
                    "User_id INTEGER PRIMARY KEY,"
                    "Username	TEXT NOT NULL,"
                    "Password	TEXT NOT NULL,"
                    "Capacity	TEXT NOT NULL"
                    //"PRIMARY KEY(User_id)"
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
                    "S_id INTEGER ,"
                    "User_id INTEGER ,"
                    "FirstName	TEXT,"
                    "LastName	TEXT,"
                    "Age	INTEGER,"
                    "StartDate  TEXT,"
                    "StudyYear INTEGER ,"
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
                    "E_id INTEGER ,"
                    "User_id INTEGER ,"
                    "FirstName	TEXT,"
                    "LastName	TEXT,"
                    "Age	INTEGER,"
                    "Wage	INTEGER,"
                    "StartDate  TEXT,"
                    "Role  TEXT,"
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
                    "C_id INTEGER ,"
                    "E_id INTEGER ,"
                    "CourseTitle	TEXT,"
                    "StudyYear	INTEGER,"
                    "Semester	TEXT,"
                    "WritingMarkPerc	INTEGER,"
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
                    "C_id INTEGER ,"
                    "Day	TEXT,"
                    "StartTime	INTEGER,"
                    "EndTime	INTEGER,"
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
                    "S_id INTEGER ,"
                    "C_id INTEGER ,"
                    "OralMark	INTEGER,"
                    "WritingMark	INTEGER,"
                    "FinalMark	INTEGER,"
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

    // Creating CleaningSchedule table
    sql =           "CREATE TABLE IF NOT EXISTS CleaningSchedule ("
                    "The_Date TEXT ,"
                    "Shift TEXT ,"
                    "E_id	INTEGER,"
                    "FOREIGN KEY(E_id) REFERENCES Employees(E_id) ON DELETE CASCADE,"
                    "PRIMARY KEY(The_Date,Shift)"
                    " );";

    exit = 0;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating CleaningSchedule Table" << endl;
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
    sql   =     "INSERT INTO Users ( User_id, Username, Password, Capacity)"
                "VALUES"
                "(1,'admin','admin','admin'),"
                "(2,'master_admin','master_admin','admin'),"
                "(3,'GFanis','1234','Student'),"
                "(4,'Maria99','1234','Student'),"
                "(5,'XTrider','1234','Student'),"
                "(6,'chris_skater','1234','Student'),"
                "(7,'Kostas33','1234','Student'),"
                "(8,'Txrisa','1234','Student'),"
                "(9,'G_Papoulias','1234','Student'),"
                "(10,'Tazes','1234','Student'),"
                "(11,'Souka','1234','Student'),"
                "(12,'Nikos_Sherlock','1234','Student'),"
                "(13,'Nikos_Zen','1234','Student'),"
                "(14,'happyapple','1234','Student'),"
                "(15,'Katerina95','1234','Student'),"
                "(16,'Mitsaras','1234','Student'),"
                "(17,'Anna97','1234','Student'),"
                "(18,'Eleni_Star','1234','Employee'),"
                "(19,'Panos_G4','1234','Employee'),"
                "(20,'Giannis_G3','1234','Employee'),"
                "(21,'Ntina99','1234','Employee'),"
                "(22,'Filippos','1234','Employee'),"
                "(23,'B_Pavlidou','1234','Employee'),"
                "(24,'I_Papadakis','1234','Employee'),"
                "(25,'G_Tsironis','1234','Employee'),"
                "(26,'Ks_Zotos','1234','Employee'),"
                "(27,'S_Trahanas','1234','Employee')"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert Users" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Inserting Student info
    sql   =     "INSERT INTO Students (S_id, User_id, FirstName , LastName, Age, StartDate, StudyYear)"
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
        cerr << "Error Inser\t Students" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }

    // Inserting Employee info
    sql   =     "INSERT INTO Employees ( E_id, User_id, FirstName, LastName, Age, Wage, StartDate, Role)"
                "VALUES"
                "(1,23,'Basiliki','Pavlidou',50,3000,'09/1995','Teacher'),"
                "(2,24,'Iosif','Papadakis',47,2500,'09/2010','Teacher'),"
                "(3,23,'Giorgos','Tsironis',53,3300,'09/2000','Teacher'),"
                "(4,26,'Ksenofon','Zotos',68,4000,'09/1985','Teacher'),"
                "(5,27,'Stefanos','Trahanas',72,4200,'09/1980','Teacher'),"
                "(6,18,'Eleni','Staridou',45,2700,'09/1999','Boarding'),"
                "(7,19,'Panagiotis','Koutras',48,2500,'09/1990','Boarding'),"
                "(8,20,'Ioannis','Trevlias',27,1500,'09/2018','Cleaner'),"
                "(9,21,'Kostantina','Antoneli',29,1600,'09/2016','Cleaner'),"
                "(10,22,'Filippos','Ioannou',35,1700,'09/2005','Cleaner')"
                "; ";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert Employees" << endl;
        cout << messaggeError << endl;
        sqlite3_free(messaggeError);
        errors++;
    }


    // Inserting Courses info
    sql   =     "INSERT INTO Courses ( C_id, E_id, CourseTitle , StudyYear,Semester,WritingMarkPerc)"
                "VALUES"
                "(1,1,'Electrodynamics',1,'Fall',7),"
                "(2,3,'Machine Learning',1,'Spring',8),"
                "(3,4,'Classical Physics',2,'Fall',8),"
                "(4,5,'Quantum Mechanics',2,'Spring',7)"
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
    sql   =     "INSERT INTO CleaningSchedule ( The_Date,Shift, E_id)"
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
        cerr << "Error Insert CleaningSchedule" << endl;
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

static int callback(void* data, int argc, char** argv, char** azColName) {
    TableData* tableData = static_cast<TableData*>(data);

    // If this is the first row, save the column names
    if (tableData->columnNames.empty()) {
        for (int i = 0; i < argc; i++) {
            tableData->columnNames.push_back(azColName[i]);
        }
    }

    // Save the row data
    vector<string> row;
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "NULL");
    }
    tableData->rows.push_back(row);

    return 0;
}

void login(int &User_id, string &User_Capacity, int &error){
    // getting login info from user
    string username{"_"}, password{"_"};
    cout << "Please type your login info" << endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if( username == "_" || password == "_" ){
        error = 2;
        return;
    }

    // Opening Database
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = 0;
    exit = sqlite3_open("University_Platform.db", &DB);
    if (exit != SQLITE_OK) {
        cerr << "Error opening DB " << sqlite3_errmsg(DB) << endl;
        error = 3;
        sqlite3_close(DB);
        return;
    }

    // Preparing the SQL statement
    string sql = "SELECT User_id,Capacity FROM Users WHERE username = ? AND password = ?;";
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if ( exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        error = 3;
        return;
    }

    // Bind the username and password to the statement
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);


    // Execute the statement and check if the username and password combination exists
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        User_id = sqlite3_column_int(stmt, 0); // Retrieve the User_id
        User_Capacity = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));// Retrieve the User_Capacity
    }else{
        error = 1;
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return;
    }

    // Finalize the first statement
    sqlite3_finalize(stmt);

    if (User_Capacity == "Employee"){
        // Prepare the SQL statement to retrieve the Role
        sql = "SELECT Role FROM Employees WHERE User_id = ?;";
        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        if (exit != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
            sqlite3_close(DB);
            error = 3;
            return;
        }


        // Bind the User_id to the statement
        sqlite3_bind_int(stmt, 1, User_id);

        // Retrieve the Role
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            User_Capacity = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))); // Retrieve the Role
        } else {
            error = 4; // Error retrieving role
            sqlite3_finalize(stmt);
            sqlite3_close(DB);
            return;
        }
        // Finalize the statement
        sqlite3_finalize(stmt);
    }

    // Finalize the statement and closing the database
    sqlite3_close(DB);
    error = 0;

}

void Print_Query(string sql, int error){
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("University_Platform.db", &DB);
    if (exit) {
        cerr << "Error opening DB: " << sqlite3_errmsg(DB) << endl;
        error = 2;
    }


    TableData tableData;

    int rc = sqlite3_exec(DB, sql.c_str(), callback, &tableData, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error SELECT: " << sqlite3_errmsg(DB)  << endl;
        error = 3;
    }

        // Printing the table
        int colWidth = 20;
        for (const string& colName : tableData.columnNames) {
            cout << setw(colWidth) << colName;
        }
        cout << endl;
        cout << string(colWidth * tableData.columnNames.size(), '-') << endl;

        for (const vector<string>& row : tableData.rows) {
            for (const string& value : row) {
                cout << setw(colWidth) << value;
            }
            cout << endl;
        }


    sqlite3_close(DB);
}

int Retrieve_User_id(string FirstName, string LastName, int error){
    int User_id{};

    if( FirstName == "_" || LastName == "_" ){
        error = 2;
        return -1;
    }
    // Opening Database
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = 0;
    exit = sqlite3_open("University_Platform.db", &DB);
    if (exit != SQLITE_OK) {
        cerr << "Error opening DB " << sqlite3_errmsg(DB) << endl;
        error = 3;
        return -1;
    }

    // Preparing the SQL statement
    string sql =            string("SELECT User_id "
                                   "FROM Employees "
                                   "WHERE FirstName = ? AND "
                                   "LastName = ? "
                                   "UNION ALL "
                                   "SELECT User_id "
                                   "FROM Students "
                                   "WHERE FirstName = ? AND "
                                   "LastName = ? "
                                   ";");
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if ( exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        error = 3;
        return -1;
    }

    // Bind the FirstName and LastName to the statement
    sqlite3_bind_text(stmt, 1, FirstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, LastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, FirstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, LastName.c_str(), -1, SQLITE_STATIC);

    // Execute the statement and check if the FirstName and LastName combination exists
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        User_id = sqlite3_column_int(stmt, 0); // Retrieve the User_id
    }else{
        error = 1;
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return -1;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return User_id;
}

void DELETE_TRY(int User_id, int error){
    // Creating the database tables

    int errors {};
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("University_Platform.db", &DB);

    if (exit != SQLITE_OK) {
        cerr << "Error openning DB " << sqlite3_errmsg(DB) << endl;

        return;
    }

    // Creating Users table
    string sql = "UPDATE Students SET FirstName = 'FanisFanis' WHERE User_id = " + to_string(User_id) + " ; ";
    //string sql =    "DELETE FROM Users WHERE User_id = "+ to_string(User_id) + " ; " ;
    sql   =     "INSERT INTO Users ( Username, Password, Capacity)"
                "VALUES"
                "('admin_2','admin_2','admin_2')"
                " ; ";
    exit = 0;
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error Creating Users Table" << endl;
        cout<< messaggeError << endl;
        sqlite3_free(messaggeError);
    }
    sqlite3_close(DB);
}

bool IsEntryExists_One_Value(sqlite3* db, const string& tableName, const string& columnName, const string& value) {
    string sql = "SELECT COUNT(*) FROM " + tableName + " WHERE " + columnName + " = ?;";
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    // Bind the value to the placeholder
    result = sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        exists = (count > 0);
    }

    sqlite3_finalize(stmt);
    return exists;
}

bool IsEntryExists_Two_Values(sqlite3* db, const string& tableName, const string& columnName1, const string& value1, const string& columnName2, const string& value2) {
    string sql = "SELECT COUNT(*) FROM " + tableName + " WHERE " + columnName1 + " = ? AND " + columnName2 + " = ?;";
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    // Bind the values to the placeholders
    result = sqlite3_bind_text(stmt, 1, value1.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
        cerr << "Error binding parameter 1: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    result = sqlite3_bind_text(stmt, 2, value2.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
        cerr << "Error binding parameter 2: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        exists = (count > 0);
    }

    sqlite3_finalize(stmt);
    return exists;
}

void Insert_User(int error) {
    sqlite3_int64 inserted_id; // To be used to print the inserted instance
    string capacity, sql;
    // Opening Database
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("University_Platform.db", &DB);

    if (exit != SQLITE_OK) {
        cerr << "Error opening DB " << sqlite3_errmsg(DB) << endl;
        return;
    }

    string entity_kind;
    cout << "What kind of entity you want to insert? [student, course, cleaner, boarding_member]: ";
    cin >> entity_kind;


    if (entity_kind == "course") {
        string CourseTitle, Semester;
        int StudyYear, WritingMarkPerc;
        cout << "Type the following:" << endl;
        cout << "CourseTitle: ";
        cin >> CourseTitle;
        cout << "StudyYear: ";
        cin >> StudyYear;
        cout << "Semester: ";
        cin >> Semester;
        cout << "WritingMarkPerc: ";
        cin >> WritingMarkPerc;

        // Check if entry already exists based on CourseTitle and StudyYear
        if (IsEntryExists_One_Value(DB, "Courses", "CourseTitle", CourseTitle)) {
            cout << "Course with title '" << CourseTitle << "' for study year " << StudyYear << " already exists." << endl;
            sqlite3_close(DB);
            return;
        }

        // Inserting the Course instance
        sql = "INSERT INTO Courses (E_id, CourseTitle, StudyYear, Semester, WritingMarkPerc) VALUES (NULL, ?, ?, ?, ?);";
        sqlite3_stmt *stmt;
        int result = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << endl;
            sqlite3_close(DB);
            return;
        }

        // Binding the values
        sqlite3_bind_text(stmt, 1, CourseTitle.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, StudyYear);
        sqlite3_bind_text(stmt, 3, Semester.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, WritingMarkPerc);

        // Executing the statement
        result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            cerr << "Error inserting data: " << sqlite3_errmsg(DB) << endl;
            return;
        }
        inserted_id = sqlite3_last_insert_rowid(DB); // Get the last inserted row ID

        // Closing the database
        sqlite3_finalize(stmt);
        sqlite3_close(DB);

    } else if (entity_kind == "student" || entity_kind == "cleaner" || entity_kind == "boarding_member") {
        string role, tableName;
        if (entity_kind == "student"){
            capacity = "Student";
            tableName = "Students";
        } else if (entity_kind == "cleaner"){
            capacity = "Employee";
            tableName = "Employees";
            role = "Cleaner";
        } else if (entity_kind == "boarding_member"){
            capacity = "Employee";
            tableName = "Employees";
            role = "Boarding";
        }

        string username, password;
        cout << "Type the following:" << endl;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        // Check if entry already exists based on Username and Password
        if (IsEntryExists_Two_Values(DB, "Users", "Username", username, "Password", password)) {
            cout << "User with username '" << username << "' already exists." << endl;
            sqlite3_close(DB);
            return;
        }

        string FirstName, LastName;
        cout << "FirstName: ";
        cin >> FirstName;
        cout << "LastName: ";
        cin >> LastName;

        // Check if entry already exists based on FirstName and LastName
        if (IsEntryExists_Two_Values(DB, "Students", "FirstName", FirstName, "LastName", LastName)) {
            cout << "User with Name '" << FirstName << " " << LastName << "' already exists." << endl;
            sqlite3_close(DB);
            return;
        }

        string StartDate;
        int Age, StudyYear, Wage;
        cout << "Age: ";
        cin >> Age;
        if(capacity == "Employee"){
            cout << "Wage: ";
            cin >> Wage;
            cout << "StartDate: ";
            cin >> StartDate;
        }else{

            cout << "StartDate: ";
            cin >> StartDate;
            cout << "StudyYear: ";
            cin >> StudyYear;
        }

        // Inserting the User instance
        sql = "INSERT INTO Users (Username, Password, Capacity) VALUES (?, ?, 'User');";
        sqlite3_stmt *stmt;
        int result = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << endl;
            sqlite3_close(DB);
            return;
        }

        // Binding the values
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        // Executing the statement
        result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            cerr << "Error inserting data: " << sqlite3_errmsg(DB) << endl;
        }
        // Get the last inserted row ID
        sqlite3_int64 User_id = sqlite3_last_insert_rowid(DB);
        sqlite3_finalize(stmt);

        if(capacity == "Student"){
            // Inserting the Student instance to the child table
            sql = "INSERT INTO " + tableName + " (User_id, FirstName, LastName, Age, StartDate, StudyYear) VALUES (?, ?, ?, ?, ?, ?);";
            result = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr);
            if (result != SQLITE_OK) {
                cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << endl;
                sqlite3_close(DB);
                return;
            }

            // Binding the values
            sqlite3_bind_int(stmt, 1, User_id);
            sqlite3_bind_text(stmt, 2, FirstName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, LastName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 4, Age);
            sqlite3_bind_text(stmt, 5, StartDate.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 6, StudyYear);
        } else {
            // Inserting the Employee instance to the child table
            sql = "INSERT INTO " + tableName + " (User_id, FirstName, LastName, Age, Wage, StartDate, Role) VALUES (?, ?, ?, ?, ?, ?, ?);";
            result = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr);
            if (result != SQLITE_OK) {
                cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << endl;
                sqlite3_close(DB);
                return;
            }

            // Binding the values
            sqlite3_bind_int(stmt, 1, User_id);
            sqlite3_bind_text(stmt, 2, FirstName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, LastName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 4, Age);
            sqlite3_bind_int(stmt, 5, Wage);
            sqlite3_bind_text(stmt, 6, StartDate.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 7, role.c_str(), -1, SQLITE_STATIC);
        }


        // Executing the statement
        result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            cerr << "Error inserting data: " << sqlite3_errmsg(DB) << endl;
            return;
        }
        inserted_id = sqlite3_last_insert_rowid(DB); // Get the last inserted row ID
        sqlite3_finalize(stmt);
        sqlite3_close(DB);

    }
    // Printing the inserted instance
    if(capacity == "Student"){
        sql =              string("SELECT FirstName, LastName, Age, StartDate, StudyYear "
                                    "FROM Students "
                                    "WHERE S_id = " + to_string(inserted_id) + " "
                                    ";");
    }else if(capacity == "Employee"){
        sql =              string("SELECT FirstName, LastName, Age, Wage, StartDate, Role "
                                    "FROM Employees "
                                    "WHERE E_id = " + to_string(inserted_id) + " "
                                    ";");
    }else{
        sql =              string("SELECT CourseTitle, StudyYear, Semester, WritingMarkPerc "
                                    "FROM Courses "
                                    "WHERE C_id = " + to_string(inserted_id) + " "
                                    ";");
    }

    int error_print;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD defaultAttributes;

    // Get the current console attributes
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    defaultAttributes = consoleInfo.wAttributes;

    // Change text console color to green
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN  | FOREGROUND_INTENSITY);
    Print_Query(sql,error_print);

    // Reset text console color to default
    SetConsoleTextAttribute(hConsole, defaultAttributes);
}
