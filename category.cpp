// Copyright 2022 Yusuf Ozdemir
/*  The main purpose of this web page is to show all products of a
    specific category.
*/
#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include "getpost.h"

using std::cout, std::cerr, std::endl, std::string, std::map;

int main() {
    // Initialization of form
    map<string, string> Get;
    initializeGet(Get);

    cout << "Content-type: text/html" << endl << endl;
    cout << "<html lang=\"en\">" << endl;

    if (Get.find("category") != Get.end() && Get.find("sort") != Get.end()) {
        mysqlpp::Connection conn(false);
        if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            mysqlpp::Query query = conn.query();
            query << "SELECT * FROM 01671166_Products WHERE type = '"
                    << Get["category"] << "' ORDER BY "
                    << Get["sort"] << ";" << endl;
            query.parse();
            mysqlpp::StoreQueryResult result = query.store();

            if (result) {
                cout << "<head><title>Zamazor - " << Get["category"]
                        << "</title>" << endl;
                cout << "<meta charset=\"utf-8\"><link rel=\"icon\""
                     << "type=\"image/x-icon\" href=\"images/zamazor.ico\">"
                     << endl;
                cout << "<link rel=\"stylesheet\" href=\"newstyle.css\""
                     << "type=\"text/css\" />";
                cout << "<link rel=\"stylesheet\" href=\"nav.css\""
                     << "type=\"text/css\" />";
                cout << "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap"
                     << "@5.2.2/dist/css/bootstrap.min.css\""
                     << "rel=\"stylesheet\" integrity=\"sha384-Zenh87qX5JnK"
                     << "2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1W"
                     << "TRi\" crossorigin=\"anonymous\"></head>" << endl;

                // Navigation
                query.reset();
                query << "SELECT username, password FROM 01671166_Users "
                      << "WHERE userID = " << Get["userID"] << ";" << endl;
                query.parse();
                mysqlpp::StoreQueryResult uinfo = query.store();
                if (uinfo) {
                    char *categories[] = { "Clothing", "Groceries",
                            "Utilities/Essentials", "Electronics", "Other" };
                    cout << "<div class=\"webnav\" id=\"navigation\">\n";
                        cout << "<form action=\"shop.cgi\" method=\"POST\">"
                            << "<input type=\"hidden\" name=\"username\" "
                                    << "value=\"" << uinfo[0]["username"]
                                    << "\">"
                            << "<input type=\"hidden\" name=\"password\""
                                    << "value=\"" << uinfo[0]["password"]
                                    << "\">"
                            << "<input type=\"submit\" value=\"Home\">"
                            << "</form>" << endl;
                        for (int i = 0;
                                i < sizeof(categories)/sizeof(categories[0]);
                                i++) {
                            cout << "<a href=\"category.cgi?"
                                 << "sort=name&category="
                                 << categories[i] << "&userID="
                                 << Get["userID"] << "\">"
                                 << categories[i] << "</a>\n";
                        }
                        cout << "<a class=\"split\" "
                             << "href=\"account/confirm.html\">"
                             << "Account</a>\n</div>" << endl;
                } else {
                    cout << "Bad user query" << endl;
                }
                
                cout << "<div class=\"ctgr row\">\n";
                    cout << "<div class=\"sort-box col-2\">\n";
                    cout << "<h3>Sort by: </h3>\n";
                    cout << "<form action=\"category.cgi\" METHOD=\"GET\">\n"
                        << "<input type=\"radio\" id=\"name\" name=\"sort\""
                                << "value=\"name\">\n"
                        << "<label for=\"name\">Sort by name</label><br>\n"
                        << "<input type=\"radio\" id=\"price\" name=\"sort\""
                                << "value=\"price\">\n"
                        << "<label for=\"price\">Sort by price</label><br>\n"
                        << "<input type=\"radio\" id=\"qty\" name=\"sort\""
                                << "value=\"qty\">\n"
                        << "<label for=\"qty\">Sort by quantity</label><br>\n"
                        << "<input type=\"hidden\" name=\"category\" value=\""
                                << Get["category"] << "\"><br>"
                        << "<input type=\"hidden\" name=\"userID\" value=\""
                                << Get["userID"] << "\">"
                        << "<input type=\"submit\">\n"
                    << "</form>\n";
                cout << "</div>\n<div class=\"item-view col-9\">\n";
                for (int i = 0; i < result.num_rows(); i++) {
                    cout << "<a href=\"product.cgi?pID=" << result[i]["pID"]
                            << "&userID=" << Get["userID"]
                            << "\"><div class=\"itembox\">\n"
                        << "<h4>" << result[i]["name"] << "</h4>\n"
                        << "<p>" << result[i]["type"] << "</p>"
                        << "<p>From " << result[i]["brand"] << "</p>"
                        << "<p>$" << result[i]["price"] << " - "
                            << result[i]["qty"] << " in stock.</p>"
                    << "</div></a>" << endl;
                }
                cout << "</div>\n<div>" << endl;
            } else {
                cout << "Query error";
            }

        } else {
            cout << "Connection error";
        }

    } else {
        cout << "Form error";
    }

    return 0;
}
