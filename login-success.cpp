// Copyright 2022 Yusuf Ozdemir
/*  The purpose of this web page is to confirm the user's login from
    the index.html web page.
*/

#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include "getpost.h"

using std::cout, std::cerr, std::endl, std::string, std::map;

// Function prototype
void printHead();
int loginValid(map<string, string> Post);

int main() {
    // Initialization of form
    map<string, string> Post;
    initializePost(Post);

    // Prints all of the information in the <head> of the document
    cout << "Content-type: text/html" << endl << endl;
    printHead();

    // Gets the result of the loginValid function
    int resultVal = loginValid(Post);

    // If the return value from loginValid() is successful
    if (resultVal > 0) {
        cout << "<h1>Congratulations!</h1>\n";
        cout << "<p>You are logged in.</p>\n";
        cout << "<form action=\"shop.cgi\" method=\"POST\">"
             << "<input type=\"hidden\" name=\"username\" value=\""
             << Post["username"] << "\">"
             << "<input type=\"hidden\" name=\"password\" value=\""
             << Post["password"] << "\">"
             << "<input type=\"submit\" value=\"Go to Zamazor\">"
             << "</form>" << endl;
    } else {
        // Error codes here
        cout << "<h1>Login failed</h1>\n<p>" << endl;
        if (resultVal == -10) {
            cout << "Some or all of the submitted form was left blank."
                 << "Please make sure you fill out the form"
                 << "completely before submitting." << endl;
        } else if (resultVal == -11) {
            cout << "The provided username was not"
                 << "found in our database." << endl;
        } else if (resultVal == -12) {
            cout << "The provided password does not"
                 << "match the provided user." << endl;
        } else if (resultVal == -90) {
            cout << "The connection to our database failed."
                 << "Sorry for the inconvenience." << endl;
        } else if (resultVal == -91) {
            cout << "An issue occured when trying to retrieve"
                 << "information from our database."
                 << "Sorry for the inconvenience." << endl;
        } else {
            cout << "Unknown invalid code returned. Sorry for"
                 << "the inconvenience." << endl;
        }

        cout << "</p><br>\n<a class=\"login register\""
             << "href=\"index.html\">Return to login</a>" << endl;
    }
    cout << "</div></body></html>" << endl;
    return 0;
}

// Function that prints all of the information in the head of the html page
void printHead() {
    cout << "<html lang=\"en\">" << endl;
    cout << "<head><title>Zamazor - Confirmation</title>" << endl;
    cout << "<meta charset=\"utf-8\"><link rel=\"icon\" type=\"image/x-icon\""
         << "href=\"images/zamazor.ico\">" << endl;
    cout << "<link rel=\"stylesheet\" href=\"login.css\" type=\"text/css\"/>"
         << "</head>";
    cout << "<body><div class=\"login-card\">" << endl;
}

/*  This function gets the username and password and confirms
    whether the values are valid (can be found in the database).
    It will return the appropriate error code if an issue occurs, but
    will return the userID otherwise (values < 1 are error codes)
*/
int loginValid(map<string, string> Post) {
    if (Post.find("username") != Post.end()
            && Post.find("password") != Post.end()) {
        mysqlpp::Connection conn(false);
        if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            mysqlpp::Query query = conn.query();
            query << "SELECT userID, username, password FROM 01671166_Users;";
            query.parse();
            mysqlpp::StoreQueryResult result = query.store();

            if (result) {
                for (int i = 0; i < result.num_rows(); ++i) {
                    if (result[i]["username"] == Post["username"]) {
                        if (result[i]["password"] == Post["password"]) {
                            return result[i]["userID"];
                        }
                        return -12;
                    }
                }
                return -11;
            } else {
                cerr << "Query error: " << query.error() << endl;
                return -91;
            }
        } else {
            cerr << "Connection failed: " << conn.error() <<endl;
            return -90;
        }
    } else {
        cerr << "Fill out the form before submitting" << endl;
        return -10;
    }
}
