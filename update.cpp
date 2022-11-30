#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include "getpost.h"

using std::cout, std::cerr, std::endl, std::string, std::map;

string checkNull(string val);

int main() {
    map<string,string> Post;
    initializePost(Post);

    // Check post
    cout << "Content-type: text/html" << endl << endl;
        cout << "<html lang=\"en\">" << endl;
        cout << "<head><title>Zamazor - Update</title>" << endl;
        cout << "<meta charset=\"utf-8\"><link rel=\"icon\" type=\"image/x-icon\" href=\"zamazor.ico\">" << endl;
        cout << "<link rel=\"stylesheet\" href=\"account.css\" type=\"text/css\" /></head>";
        cout << "<body>" << endl;

        cout << "<div class=\"main\">\n";
        mysqlpp::Connection conn(false);
        if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            mysqlpp::Query query = conn.query();
            query << "UPDATE 01671166_Users SET username = '"<<Post["username"]<<"', "
            << "email = '"<<Post["email"]<<"',"
            << "password = '"<<Post["password"]<<"',"
            << "firstName = '"<<Post["first_name"]<<"',"
            << "lastName = '"<<Post["last_name"]<<"',"
            << "city = '"<<Post["city"]<<"',"
            << "state = '"<<Post["state"]<<"',"
            << "zipcode = "<<checkNull(Post["zipcode"])<<" "
            << "WHERE userID = "<<Post["userID"]<<";";
            query.parse();

            mysqlpp::SimpleResult result = query.execute();
            if(result) {
                cout << "<h1>Success!</h1>"
                    << "Your account information has been updated." << endl;
                cout << "<form action=\"account.cgi\" method=\"POST\">"
                    << "<input type=\"hidden\" name=\"username\" value=\""<<Post["username"]<<"\">"
                    << "<input type=\"hidden\" name=\"password\" value=\""<<Post["password"]<<"\"><br>"
                    << "<input type=\"submit\" name=\"return\" value=\"Return to account\">"
                << "</form>" << endl;
            } else {
                cout << "Error with query" << endl;
            }
        } else {
            cout << "Error with connection" << endl;
        }
        cout << "</div>" << endl;

    return 0;
}

string checkNull(string val) {
    if (val == "NULL") {
        return "NULL";
    } else {
        return "'" + val + "'";
    }
}
