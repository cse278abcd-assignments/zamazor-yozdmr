// Copyright 2022 Yusuf Ozdemir
/*  The main purpose of this web page is to be the main page of the
    store. It has all of the navigation at the top and some suggestions
    in the main body.
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
void printNav(map<string, string> Post, mysqlpp::StoreQueryResult uid,
        mysqlpp::Query query);
void printItems(mysqlpp::StoreQueryResult result,
        mysqlpp::StoreQueryResult uid);

int main() {
    // Initialization of form
    map<string, string> Post;
    initializePost(Post);

    // Prints all of the information in the <head> of the document
    cout << "Content-type: text/html" << endl << endl;
    printHead();

    // Connects to the mySQL server
    mysqlpp::Connection conn(false);
    if (conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
        // Creates new query, gets the userID to be used in navingating
        //   the website later on
        mysqlpp::Query query = conn.query();
        query << "SELECT userID FROM 01671166_Users WHERE username = '"
                << Post["username"] << "';";
        query.parse();
        mysqlpp::StoreQueryResult uid = query.store();

        // Prints the navigation bar of the website
        printNav(Post, uid, query);
    
        // The main body of the web page
        cout << "<div class=\"ctgr\">\n" << "<div class=\"inner-ctgr\">\n";

        // New query that gets cheap items, shows them in a category
        //   in the web page
        query.reset();
        query << "SELECT * FROM 01671166_Products WHERE qty > 0 AND price < 15;";
        query.parse();
        mysqlpp::StoreQueryResult cheapItems = query.store();
        cout << "<h2>Affordable</h2>\n"
             << "<div class=\"scroll-box\">\n"; 
        printItems(cheapItems, uid);
        cout << "</div>\n</div>" << endl;

        // New query that gets items with low stock, shows them in a category
        //   in the web page
        cout << "<br>\n<div class=\"inner-ctgr\">\n";
        query.reset();
        query << "SELECT * FROM 01671166_Products WHERE qty > 0 AND qty < 10;";
        query.parse();

        mysqlpp::StoreQueryResult lowStock = query.store();
        cout <<
            "<h2>Running out soon</h2>\n" <<
            "<div class=\"scroll-box\">\n";
        printItems(lowStock, uid);
        cout << "</div>\n</div>" << endl;

        // New query that gets items with middling stock and reasonable price
        //   and shows them in a category in the web page
        cout << "<br>\n<div class=\"inner-ctgr\">\n";
        query.reset();
        query << "SELECT * FROM 01671166_Products WHERE qty > 15 AND qty < 30 AND price > 17 AND price < 180;";
        query.parse();
        mysqlpp::StoreQueryResult recommended = query.store();
        cout <<
            "<h2>Recommended by us</h2>\n" <<
            "<div class=\"scroll-box\">\n";
        printItems(recommended, uid);
        cout << "</div>\n</div>" << endl;

    } else {
        cerr << "Connection failed: " << conn.error() <<endl;
        return 1;
    }

    cout << "</div>\n</body>" << endl;

    cout << "<script>\nwindow.onscroll = function() {stick()};\n"
         << "var navbar = document.getElementById(\"navigation\");\n"
         << "var sticky = navbar.offsetTop;\n"
         << "function stick() {\n"
         << "\tif (window.pageYOffset >= 50) {\n\t\tnavbar.classList.add(\"sticky\");\n\t} "
         << "else {\n\t\tnavbar.classList.remove(\"sticky\");\n\t}\n}\n</script>";
    cout << "</html>" << endl;
    return 0;
}

// Function that prints all of the information in the head of the html page
void printHead() {
    cout << "<html lang=\"en\">" << endl;
    cout << "<head><title>Zamazor - Shop</title>" << endl;
    cout << "<meta charset=\"utf-8\"><link rel=\"icon\""
         << "type=\"image/x-icon\" href=\"zamazor.ico\">" << endl;
    cout << "<link rel=\"stylesheet\" href=\"newstyle.css\""
         << "type=\"text/css\" />";
    cout << "<link rel=\"stylesheet\" href=\"nav.css\""
         << "type=\"text/css\" /></head>";
    cout << "<body>" << endl;
}

// Function that prints the navigation of the document
void printNav(map<string, string> Post, mysqlpp::StoreQueryResult uid, mysqlpp::Query query) {
    // Navigation
    char *categories[] = { "Clothing", "Groceries",
            "Utilities/Essentials", "Electronics", "Other" };
    cout << "<div class=\"webnav\" id=\"navigation\">\n"
        "<a href = \"\" class=\"active\">Home</a>\n";
        for (int i = 0; i < sizeof(categories)/sizeof(categories[0]); i++) {
            cout << "<a href = \"category.cgi?sort=name&category="
                 << categories[i] << "&userID="
                << uid[0]["userID"] << "\">"
                << categories[i] << "</a>\n";
        }
    
    query.reset();
    query << "SELECT pID, qty FROM 01671166_CartInv WHERE userID = '"
            << uid[0]["userID"] << "';";
    query.parse();
    mysqlpp::StoreQueryResult cartitems = query.store();
    double totalCost = 0;
    for (int i = 0; i < cartitems.num_rows(); i++) {
        query.reset();
        query << "SELECT price FROM 01671166_Products WHERE pID = '"
                << cartitems[i]["pID"] << "';";
        query.parse();
        mysqlpp::StoreQueryResult item = query.store();
        double itemCost = item[0]["price"];
        int itemQty = cartitems[i]["qty"];
        totalCost += (itemCost*itemQty);
    }
    cout << "\n<a class=\"split\"><form action=\"account.cgi\" method=\"POST\">"
        << "<input type=\"hidden\" name=\"username\" value=\""
                << Post["username"] << "\">"
        << "<input type=\"hidden\" name=\"password\" value=\"" 
                << Post["password"] << "\">"
        << "<input type=\"submit\" value=\"Account\">"
        << "</form></a>\n";
    cout << "\n<a class=\"split\" href=\"cart.cgi?userID="<<uid[0]["userID"]<<"\">Cart ($"<<totalCost<<")</a>\n";
    cout << "</div>" << endl;
}

// Function that prints the different item categories based on
//   the provided SQL query
void printItems(mysqlpp::StoreQueryResult result,
        mysqlpp::StoreQueryResult uid) {
    if (result) {
        for (int i = 0; i < result.num_rows(); ++i) {
            cout <<
                "<a href=\"product.cgi?pID=" << result[i]["pID"]
                        << "&userID=" << uid[0]["userID"] << "\">"
                        << "<div class=\"itembox\">\n"
                    << "<h4>"<< result[i]["name"] <<"</h4>\n"
                    << "<p>"<< result[i]["type"] <<"</p>\n"
                    << "<p>From " << result[i]["brand"] << "</p>"
                    << "<p>"<< result[i]["price"] << " - "
                            << result[i]["qty"] << " in stock.</p>\n"
                << "</div></a>\n";
        }
    } else {
        cout << "Query failed" << endl;
    }
}
