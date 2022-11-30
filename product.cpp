// Copyright 2022 Yusuf Ozdemir
/*  The main purpose of this web page is to show the information of a
    specific product, along with providing the option to check an item out.
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
void printHead(mysqlpp::String name);

int main() {
    // Initialization of form
    map<string, string> Get;
    initializeGet(Get);

    cout << "Content-type: text/html" << endl << endl;
    cout << "<html lang=\"en\">" << endl;

    // Checks to make sure necessary values are received
    if (Get.find("pID") != Get.end() && Get.find("userID") != Get.end()) {
        // Connects to mySQL server
        mysqlpp::Connection conn(false);
        if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            // Creates a query
            mysqlpp::Query query = conn.query();
            query << "SELECT * FROM 01671166_Products WHERE pID = "
                  << Get["pID"] << ";" << endl;
            query.parse();
            mysqlpp::StoreQueryResult result = query.store();

            // If query is valid
            if (result) {
                printHead(result[0]["name"]);

                // Gets the username and password for the navigation to use
                query.reset();
                query << "SELECT username, password FROM 01671166_Users "
                      << "WHERE userID = " << Get["userID"] << ";" << endl;
                query.parse();
                mysqlpp::StoreQueryResult uinfo = query.store();

                // Prints the navigation
                char *categories[] = { "Clothing", "Groceries",
                        "Utilities/Essentials", "Electronics", "Other" };
                cout << "<div class=\"webnav\" id=\"navigation\">\n";
                    cout << "<form action=\"shop.cgi\" method=\"POST\">"
                         << "<input type=\"hidden\" name=\"username\" value=\""
                                << uinfo[0]["username"] << "\">"
                         << "<input type=\"hidden\" name=\"password\" value=\""
                                << uinfo[0]["password"] << "\">"
                         << "<input type=\"submit\" value=\"Home\">"
                         << "</form>" << endl;
                    for (int i = 0;
                            i < sizeof(categories)/sizeof(categories[0]); i++) {
                        cout << "<a href = \"category.cgi?sort=name&category="
                            << categories[i] << "&userID=" << Get["userID"]
                            << "\">" << categories[i] << "</a>\n";
                    }
                    cout << "<a class = \"split\""
                         << "href=\"confirm.html\">Account</a>\n" <<
                    "</div>" << endl;

                // The main body of the html code:
                cout << "<body><div class=\"ctgr row\">" << endl;
                
                    // Contains all of the information about the item
                    cout << "<div class=\"inner-ctgr col-9\">\n";
                        cout << "<h1>" << result[0]["name"] << "</h1>"
                            << "<h3>Price: $" << result[0]["price"]
                            << "</h3>" << endl;

                        cout << "<ul>\n<li>Type: " << result[0]["type"]
                                << "</li>\n"
                             << "<li>Brand: " << result[0]["brand"]
                                    << "</li></ul>"
                             << "<h3>Description: </h3>\n"
                             << "<p>" << result[0]["description"] << "</p>" << endl;
                    cout << "</div>" << endl;

                    // Contains the stuff to add the item to cart
                    cout << "<div class=\"mini-box col-2 ms-auto\">\n";
                        cout << "<h3>Buy</h3>"
                             << "<h5>In stock: " << result[0]["qty"]
                                    << "</h5>\n"
                             << "<form action=\"cart-success.cgi\""
                                    << "method=\"GET\">"
                             << "<input type=\"number\" name=\"qty\""
                                    << "min=\"1\" max=\"" << result[0]["qty"]
                                    << "\" value=\"1\">\n"
                             << "<input type=\"hidden\" name=\"userID\""
                                    << "value=\"" << Get["userID"] << "\">"
                             << "<input type=\"hidden\" name=\"pID\""
                                    << "value=\"" << result[0]["pID"] << "\">"
                             << "<input type=\"submit\" value=\"Add to Cart\">"
                                    << "</form>" << endl;
                    cout << "</div>" << endl;

                cout << "</div>" << endl;
            }

        } else {
            cout << "<h1>Sorry ):</h1>\n";
            cout << "<p>There was an issue with getting the item.</p>" << endl;
        }
    }
    
    cout << "</body></html>" << endl;
    return 0;
}

// Function that prints all of the information in the head of the html page
void printHead(mysqlpp::String name) {
    cout << "<head><title>Zamazor - " << name << "</title>" << endl;
    cout << "<meta charset=\"utf-8\"><link rel=\"icon\""
         << "type=\"image/x-icon\" href=\"zamazor.ico\">" << endl;
    cout << "<link rel=\"stylesheet\" href=\"newstyle.css\""
         << "type=\"text/css\" />";
    cout << "<link rel=\"stylesheet\" href=\"nav.css\" type=\"text/css\" />";
    cout << "<link href=\"https://cdn.jsdelivr.net/npm/boot"
            << "strap@5.2.2/dist/css/bootstrap.min.css\""
            << "rel=\"stylesheet\""
            << "integrity=\"sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCe"
            << "uOxjzrPF/et3URy9Bv1WTRi\" crossorigin=\"anonymous\">"
            << "</head>" << endl;
}
