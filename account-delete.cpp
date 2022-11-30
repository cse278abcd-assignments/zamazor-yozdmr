#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include "getpost.h"

using std::cout, std::cerr, std::endl, std::string, std::map;
int loginValid(map<string,string> Post);
void stateList();

int main() {
    map<string,string> Post;
    initializePost(Post);

    cout << "Content-type: text/html" << endl << endl;
    cout << "<html lang=\"en\">" << endl;
    cout << "<head><title>Zamazor - Account Deleted</title>" << endl;
    cout << "<meta charset=\"utf-8\"><link rel=\"icon\" type=\"image/x-icon\" href=\"zamazor.ico\">" << endl;
    cout << "<link rel=\"stylesheet\" href=\"account.css\" type=\"text/css\" /></head>";
    cout << "<body>" << endl;

    cout << "<div class=\"main\">\n";
    if (Post.find("userID") != Post.end()) {
        int uid = stoi(Post["userID"]);

        cout << "User ID:" << uid << endl;
        if (uid == 1) {
            cout << "Cannot delete root user." << endl;
            cout << "<a href=\"account.cgi\">Return to account page</a>" << endl;
        } else {
            mysqlpp::Connection conn(false);
            if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
                mysqlpp::Query query = conn.query();
                query << "DELETE FROM 01671166_Users WHERE userID = '"<<uid<<"';";
                query.parse();

                mysqlpp::SimpleResult result = query.execute();

                cout << "Your account has been successfully deleted.\n";
                cout << "<a href=\"index.html\">Return to Login</a>" << endl;
            }
        }
    } else {
        cout << "User id not received. Check to make sure you were logged in." << endl;
    }
    cout << "</div>" << endl;

    return 0;
}