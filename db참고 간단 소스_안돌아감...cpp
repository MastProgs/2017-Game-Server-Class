// SQLConnect_ref.cpp  
// compile with: odbc32.lib 
#include <iostream>
#include <windows.h>  
#include <sqlext.h>

#define UNICODE  

#define NAME_LEN 10

using namespace std;

void show_error() {
	printf("error\n");
}

int main() {
	SQLHENV henv;		// 핸들
	SQLHDBC hdbc;		// 핸들
	SQLHSTMT hstmt = 0;	// 핸들
	SQLRETURN retcode;	// 리턴 코드

	SQLWCHAR szName[NAME_LEN];
	int ID, pLevel;
	SQLLEN cbName = 0, cbCustID = 0, cbPhone = 0;

	SQLCHAR * OutConnStr = (SQLCHAR *)malloc(255);
	SQLSMALLINT * OutConnStrLen = (SQLSMALLINT *)malloc(255);

	// Allocate environment handle - 핸들을 할당 받고
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute - 리턴코드 검사
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

		// Allocate connection handle - 
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds - 5초 이상 딜레이 나면 리턴 ( 안하면 한도끝도 없이 딜레이
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"my_game_server", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

				// Allocate statement handle  - 성공하면...
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

					// Process data  - 여기서 데이터를 뽑는당.
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

						cout << "Data Base Access Complete\n";

						retcode = SQLExecDirect(hstmt, (SQLWCHAR *)L"SELECT ID, Name, PLevel FROM dbo.user_data ORDER BY 2, 1, 3", SQL_NTS);
						if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

							// Bind columns 1, 2, and 3  
							retcode = SQLBindCol(hstmt, 1, SQL_C_LONG, &ID, 100, &cbCustID);
							retcode = SQLBindCol(hstmt, 2, SQL_C_CHAR, szName, NAME_LEN, &cbName);
							retcode = SQLBindCol(hstmt, 3, SQL_C_LONG, &pLevel, 100, &cbPhone);

							// Fetch and print each row of data. On an error, display a message and exit.  
							for (int i = 0; ; i++) {
								retcode = SQLFetch(hstmt);
								if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
									show_error();
								if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
									cout << i + 1 << " [ " << ID << " ] Name = " << szName << ", level = " << pLevel << "\n";
								else
									break;
							}


							SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
						}

						SQLDisconnect(hdbc);
					}

					SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
				}
			}
			SQLFreeHandle(SQL_HANDLE_ENV, henv);
		}

		cout << "Program End\n";
	}
}