#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include "getpost.h"

using std::cout, std::cerr, std::endl, std::string, std::map;

int main() {
    map<string,string> Get;
    initializeGet(Get);

    cout << "Content-type: text/html" << endl << endl;
    cout << "<html lang=\"en\">" << endl;
    cout << "<head><title>Zamazor - Checkout</title>" << endl;
    cout << "<meta charset=\"utf-8\"><link rel=\"icon\" type=\"image/x-icon\" href=\"zamazor.ico\">" << endl;
    cout << "<link rel=\"stylesheet\" href=\"account.css\" type=\"text/css\" /></head>";

    if (Get.find("userID") != Get.end()) {

        mysqlpp::Connection conn(false);
        if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            mysqlpp::Query query = conn.query();
            query << "DELETE FROM 01671166_CartInv WHERE userID = "<<Get["userID"]<<";";
            query.parse();
            mysqlpp::SimpleResult result = query.execute();
            if(result) {
                query.reset();
                query << "SELECT username, password, address FROM 01671166_Users WHERE userID = "<<Get["userID"]<<";";
                query.parse();
                mysqlpp::StoreQueryResult uinfo = query.store();
                if (uinfo) {
                    cout << "<div class=\"main\"><h1>Success!</h1>\n"
                         << "<p>Your items were checked out and will be delivered to your residence at "
                         << uinfo[0]["address"] << ".</p>" << endl;
                    cout << "<form action=\"account.cgi\" method=\"POST\">"
                        << "<input type=\"hidden\" name=\"username\" value=\""<<uinfo[0]["username"]<<"\">"
                        << "<input type=\"hidden\" name=\"password\" value=\""<<uinfo[0]["password"]<<"\">"
                        << "<input type=\"submit\" value=\"Return to account\">"
                        << "</form></div>\n";
                } else {
                    cout << "Issue getting user info" << endl;
                }
                
            } else {
                cout << "Issue with user query" << endl;
            }
        } else {
            cout << "Connection failed" << endl;
        }
    } else {
        cout << "No userid" << endl;
    }

    return 0;
}