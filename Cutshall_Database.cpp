/*
Program: Assignment 9- Database Project

Purpose: This project queries and inserts data into a SQLite3 database which was created from the command line. This project uses
three Select statements and an Insert statement. The return_code status is shown in order to guage the interaction with SQLite3.

Created: 5/9/2016

Developer: Michael Cutshall
*/


#include "stdafx.h"
#include "C:\SQLite\sqlite3.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

void initializeDB(int);
void selectMajorCode(int, sqlite3*, sqlite3_stmt*, const char*, int);
void selectGPA(int, sqlite3*, sqlite3_stmt*, const char*, int);
void insertStudent(int, sqlite3*, sqlite3_stmt*, const char*, int);
void selectNewStudent(int, sqlite3*, sqlite3_stmt*, const char*, int);
void closeDB(int, sqlite3*, sqlite3_stmt*);
string displayErrorMsg(int sqlite_code);

int main()
{
	//declare variables and constants
	const char* DB_FILE_NAME = "C:\\SQLite\\My_DB.db";//name of sqlite database file containing student data
	//**must use double backslashes for file name**
	sqlite3 *db_ptr = nullptr;//a pointer to the database
	const char* VFS = "";
	sqlite3_stmt* prepared_sql_statement = nullptr;
	const char* tail = nullptr;
	int column_count = 0;
	int return_code = 0;	
	

	initializeDB(return_code);

	//Open a connection to the database
	return_code = sqlite3_open_v2(DB_FILE_NAME, &db_ptr, SQLITE_OPEN_READWRITE, nullptr);
	cout << "\nOpen DB return_code: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	selectMajorCode(return_code, db_ptr, prepared_sql_statement, tail, column_count);
	selectGPA(return_code, db_ptr, prepared_sql_statement, tail, column_count);
	insertStudent(return_code, db_ptr, prepared_sql_statement, tail, column_count);
	selectNewStudent(return_code, db_ptr, prepared_sql_statement, tail, column_count);

	closeDB(return_code, db_ptr, prepared_sql_statement);

}

void initializeDB(int return_code)
{
	//Initialize the database
	cout << "\nInitialize DB return_code: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	//Determine if database was successfully initialized
	if (return_code != SQLITE_OK)
	{
		cout << "\nSQLite3 initialization failed -- return_code: " << return_code << "\n";
		exit(-1);
	}
}

void selectMajorCode(int return_code, sqlite3 *db_ptr, sqlite3_stmt* prepared_sql_statement, const char* tail, int column_count)
{
	char* sql_statement1 = "select * from STUDENT where major_code = 'cis'";
	//Prepare or compile the SQL statement
	return_code = sqlite3_prepare_v2(db_ptr, sql_statement1, strlen(sql_statement1), &prepared_sql_statement, &tail);
	
	cout << "\nPrepare first select return_code: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	//Execute the SQL statements
	return_code = sqlite3_step(prepared_sql_statement);
	
	cout << "\nExecute first select return_code: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	//View how many columns the SQL statement returned
	column_count = sqlite3_column_count(prepared_sql_statement);

	//Enter a loop if the SQL statement returned a row
	while (return_code == SQLITE_ROW)
	{
		// Display the results produced by the SQL statement. 
		cout << "\n-----------------------------------------";
		cout << "\n Students with Major Code 'cis': \n";
		cout << "\n Student ID: " << sqlite3_column_int(prepared_sql_statement, 0) << "\n Last Name: "
			<< sqlite3_column_text(prepared_sql_statement, 1) << "\n First Name: "
			<< sqlite3_column_text(prepared_sql_statement, 2) << "\n Major Code: "
			<< sqlite3_column_text(prepared_sql_statement, 3) << "\n Credits Earned: "
			<< sqlite3_column_int(prepared_sql_statement, 4) << "\n GPA: "
			<< sqlite3_column_double(prepared_sql_statement, 5) << "\n";
		cout << "-----------------------------------------\n";

		//Execute the SQL statement again.
		return_code = sqlite3_step(prepared_sql_statement);
		
	}
	cout << "\n\n";
	//Reset the prepared statement
	return_code = sqlite3_reset(prepared_sql_statement);
	cout << "\nSQLite reset: " << displayErrorMsg(return_code) << "\n";
}

void selectGPA(int return_code, sqlite3 *db_ptr, sqlite3_stmt* prepared_sql_statement, const char* tail, int column_count)
{
	char* sql_statement2 = "select * from STUDENT where gpa < 3.0";
	//Prepare or compile the SQL statement
	return_code = sqlite3_prepare_v2(db_ptr, sql_statement2, strlen(sql_statement2), &prepared_sql_statement, &tail);
	

	cout << "\nPrepare second select return_code: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	//Execute the SQL statements
	return_code = sqlite3_step(prepared_sql_statement);
	
	cout << "\nExecute second select return_code: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	//View how many columns the SQL statement returned
	column_count = sqlite3_column_count(prepared_sql_statement);

	//Enter a loop if the SQL statement returned a row
	cout << "\n-----------------------------------------";
	cout << "\n Students with GPA less than 3.0: \n";
	while (return_code == SQLITE_ROW)
	{
		//Display the results produced by the SQL statement. 		
		cout << "\n Student ID: " << sqlite3_column_int(prepared_sql_statement, 0) << "\n Last Name: "
			<< sqlite3_column_text(prepared_sql_statement, 1) << "\n First Name: "
			<< sqlite3_column_text(prepared_sql_statement, 2) << "\n Major Code: "
			<< sqlite3_column_text(prepared_sql_statement, 3) << "\n Credits Earned: "
			<< sqlite3_column_int(prepared_sql_statement, 4) << "\n GPA: "
			<< sqlite3_column_double(prepared_sql_statement, 5) << "\n";
		cout << "-----------------------------------------\n";

		//Execute the SQL statement again.
		return_code = sqlite3_step(prepared_sql_statement);
	}
	cout << "\n";
	//Reset the prepared statement
	return_code = sqlite3_reset(prepared_sql_statement);
	cout << "\nSQLite reset: " << displayErrorMsg(return_code) << "\n";
}

void insertStudent(int return_code, sqlite3 *db_ptr, sqlite3_stmt* prepared_sql_statement, const char* tail, int column_count)
{
	char* sql_insert_statement = "insert into STUDENT (student_id, last_name, first_name, major_code, credits_earned, gpa)"
		" values(?, ?, ?, ?, ?, ?)";
	sqlite3_stmt* prepared_sql_insert_statement = nullptr;

	//prepare insert statement
	return_code = sqlite3_prepare_v2(db_ptr, sql_insert_statement, strlen(sql_insert_statement), &prepared_sql_insert_statement, &tail);

	cout << "\nreturn_code from preparing the INSERT statement: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	//bind parameters
	return_code = sqlite3_bind_int(prepared_sql_insert_statement, 1, 13);
	return_code = sqlite3_bind_text(prepared_sql_insert_statement, 2, "mason", -1, nullptr);
	return_code = sqlite3_bind_text(prepared_sql_insert_statement, 3, "marcia", -1, nullptr);
	return_code = sqlite3_bind_text(prepared_sql_insert_statement, 4, "mat", -1, nullptr);
	return_code = sqlite3_bind_int(prepared_sql_insert_statement, 5, 60);
	return_code = sqlite3_bind_double(prepared_sql_insert_statement, 6, 3.6);

	//run statement
	sqlite3_step(prepared_sql_insert_statement);

	//See if the insert succeeded
	cout << "\nreturn_code from running the INSERT statement: " << return_code << " " << displayErrorMsg(return_code) << "\n";
}

void selectNewStudent(int return_code, sqlite3 *db_ptr, sqlite3_stmt* prepared_sql_statement, const char* tail, int column_count)
{
	char* sql_statement3 = "select * from STUDENT where last_name = 'mason' and first_name = 'marcia'";
	//Prepare or compile the SQL statement
	return_code = sqlite3_prepare_v2(db_ptr, sql_statement3, strlen(sql_statement3), &prepared_sql_statement, &tail);

	cout << "\nPrepare third select return_code: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	//Execute the SQL statements
	return_code = sqlite3_step(prepared_sql_statement);
	cout << "\nExecute third select return_code: " << return_code << " " << displayErrorMsg(return_code) << "\n";

	//View how many columns the SQL statement returned
	column_count = sqlite3_column_count(prepared_sql_statement);

	//Enter a loop if the SQL statement returned a row
	while (return_code == SQLITE_ROW)
	{
		//Display the results produced by the SQL statement. 
		cout << "\n-----------------------------------------";
		cout << "\n New Student Data: \n";
		cout << "\n Student ID: " << sqlite3_column_int(prepared_sql_statement, 0) << "\n Last Name: "
			<< sqlite3_column_text(prepared_sql_statement, 1) << "\n First Name: "
			<< sqlite3_column_text(prepared_sql_statement, 2) << "\n Major Code: "
			<< sqlite3_column_text(prepared_sql_statement, 3) << "\n Credits Earned: "
			<< sqlite3_column_int(prepared_sql_statement, 4) << "\n GPA: "
			<< sqlite3_column_double(prepared_sql_statement, 5) << "\n";
		cout << "-----------------------------------------\n";
		//Execute the SQL statement again.
		return_code = sqlite3_step(prepared_sql_statement);
	}
	cout << "\n\n";
	//Reset the prepared statement
	return_code = sqlite3_reset(prepared_sql_statement);
	cout << "\nSQLite reset: " << displayErrorMsg(return_code) << "\n";
}

void closeDB(int return_code, sqlite3* db_ptr, sqlite3_stmt* prepared_sql_statement)
{
	//Finalize prepared statements
	sqlite3_finalize(prepared_sql_statement);

	//Close the database connection
	sqlite3_close(db_ptr);

	//Shutdown the database
	return_code = sqlite3_shutdown();

	//pause visual studio
	char ch = cin.get();
}

string displayErrorMsg(int sqlite_code)
{
	string msg_text = "";

	switch (sqlite_code)
	{
	case SQLITE_OK:

		msg_text = "SQLITE_OK: statement completed succesfully";
		break;

	case SQLITE_ERROR:

		msg_text = "SQLITE_ERROR: a generic error occurred";
		break;

	case SQLITE_CONSTRAINT:

		msg_text = "SQLITE_CONSTRAINT: a constraint was violated";
		break;

	case SQLITE_BUSY:

		msg_text = "SQLITE_BUSY: database is locked";
		break;

	case SQLITE_NOMEM:

		msg_text = "SQLITE_NOMEM: memory llocation failed";
		break;

	case SQLITE_CORRUPT:

		msg_text = "SQLITE_CORRUPT: database is corrupt";
		break;

	case SQLITE_READONLY:

		msg_text = "SQLITE_READONLY: attempted to write to a DB opened for readonly";
		break;

	case SQLITE_EMPTY:

		msg_text = "SQLITE_EMPTY: database file is empty";
		break;

	case SQLITE_MISMATCH:

		msg_text = "SQLITE_MISMATCH: data type mismatch";
		break;

	case SQLITE_RANGE:

		msg_text = "SQLITE_RANGE: bind parameter index is invalid";
		break;

	case SQLITE_CANTOPEN:

		msg_text = "SQLITE_CANTOPEN: can't open DB file";
		break;

	case SQLITE_DONE:

		msg_text = "SQLITE_DONE: the SQL statement has run to completion";
		break;

	case SQLITE_ROW:

		msg_text = "SQLITE_ROW: another row of data is available";
		break;

	case SQLITE_MISUSE:

		msg_text = "SQLITE_MISUSE: the application uses any SQLite interface in a way that is undefined or unsupported, e.g., using a prepared statement it has been finalized";
		break;

	default:

		msg_text = "Didn't match any SQLITE3 codes listed in this function";
		break;
	}

	return msg_text;
}
