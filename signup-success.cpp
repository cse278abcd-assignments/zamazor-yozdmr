// Copyright 2022 Yusuf Ozdemir
/*  The purpose of this web page is to confirm the user's signup from
    the register.html web page.
*/

#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include "getpost.h"

using std::cout, std::cerr, std::endl, std::string, std::map;

// Function prototypes
void printHead();

int main() {
    // Initialization of form
    map<string, string> Post;
    initializePost(Post);

    // Prints all of the information in the <head> of the document
    cout << "Content-type: text/html" << endl << endl;
    printHead();

    // Checks to make sure proper information has been received
    if (Post.find("username") != Post.end()
            && Post.find("password") != Post.end()
            && Post.find("first_name") != Post.end()
            && Post.find("last_name") != Post.end()
            && Post.find("state") != Post.end()) {
        // Connect to mySQL server
        mysqlpp::Connection conn(false);
        if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            // Create a query that adds info to Users table
            mysqlpp::Query query = conn.query();
            query << "INSERT INTO 01671166_Users (username, email, password, "
                  << "firstName, lastName, address, city, "
                  << "state, zipcode) "
                  << "VALUES ('" << Post["username"] << "', '" << Post["email"]
                  << "', '" << Post["password"] << "', '" << Post["first_name"]
                  << "', '" << Post["last_name"] << "', '" << Post["address"]
                  << "', '" << Post["city"] << "', '" << Post["state"] << "', "
                  << Post["zipcode"] << ")";
            query.parse();
            mysqlpp::SimpleResult result = query.execute();

            if (result) {
                // Success message + link to shop page
                cout << "<h1>Congratulations!</h1>\n";
                cout << "<p>Your account has been added to our database.</p>"
                        << endl;
                cout << "<form action=\"shop.cgi\" method=\"POST\">"
                        << "<input type=\"hidden\" name=\"username\" value=\""
                                << Post["username"] << "\">"
                        << "<input type=\"hidden\" name=\"password\" value=\""
                                << Post["password"] << "\">"
                        << "<input type=\"submit\" value=\"Go to Zamazor\">"
                        << "</form>" << endl;
            } else {
                // If the SQL query doesn't work
                cout << "<h1>Registration error</h1>\n";
                    cout << "<p>An issue occured when trying to add your "
                         << "account to our database."
                         << "Sorry for the inconvenience.</p>" << endl;
                    cout << "<br>\n<a class=\"login register\""
                         << "href=\"register.html\">Go back to Register</a>"
                         << endl;
            }
        } else {
            // If the connection to the database fails
            cout << "<h1>Registration error</h1>\n";
            cout << "<p>The connection to our database failed. Sorry for "
                 << "the inconvenience.</p>" << endl;
            cout << "<br>\n<a class=\"login register\" href=\"index.html\">"
                 << "Go back to Login</a>" << endl;
        }
    } else {
        // If the form was not filled out properly
        cout << "<h1>Registration error</h1>\n";
        cout << "<p>There was an issue processing your provided information."
             << "Please go back and make sure you didn't leave any "
             << "required fields blank.</p>" << endl;
        cout << "<br>\n<a class=\"login register\" href=\"index.html\">"
             << "Go back to Login</a>" << endl;
    }
    
    cout << "</div></body></html>" << endl;
    return 0;
}

// Function that prints all of the information in the head of the html page
void printHead() {
    cout << "<html lang=\"en\">" << endl;
    cout << "<head><title>Zamazor - Account Created</title>" << endl;
    cout << "<meta charset=\"utf-8\"><link rel=\"icon\" type=\"image/x-icon\""
         << "href=\"zamazor.ico\">" << endl;
    cout << "<link rel=\"stylesheet\" href=\"login.css\""
         << "type=\"text/css\"/>\n</head>\n";
    cout << "<body><div class=\"login-card\">" << endl;
}
