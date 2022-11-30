#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include "getpost.h"

using std::cout, std::cerr, std::endl, std::string, std::map;
int loginValid(map<string,string> Post);

int main() {
    map<string,string> Get;
    initializeGet(Get);

    cout << "Content-type: text/html" << endl << endl;
    cout << "<html lang=\"en\">" << endl;
    cout << "<head><title>Zamazor - Cart</title>" << endl;
    cout << "<meta charset=\"utf-8\"><link rel=\"icon\" type=\"image/x-icon\" href=\"zamazor.ico\">" << endl;
    cout << "<link rel=\"stylesheet\" href=\"account.css\" type=\"text/css\" />";
    cout << "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css\""
         << "rel=\"stylesheet\" integrity=\"sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi\" crossorigin=\"anonymous\"></head>" << endl;

    if (Get.find("userID") != Get.end()) {

        mysqlpp::Connection conn(false);
        if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            mysqlpp::Query query = conn.query();
            query << "SELECT username, password FROM 01671166_Users WHERE userID = "<<Get["userID"]<<";";
            query.parse();

            mysqlpp::StoreQueryResult result = query.store();
            if(result) {
                
                // Navigation
                cout << "<div class=\"sidenav\">\n";
                cout << "<form action=\"shop.cgi\" method=\"POST\">"
                        << "<input type=\"hidden\" name=\"username\" value=\"" << result[0]["username"]<<"\">"
                        << "<input type=\"hidden\" name=\"password\" value=\"" << result[0]["password"]<<"\">"
                        << "<input type=\"submit\" value=\"Home\">"
                        << "</form>" << endl;
                cout << "<form action=\"account.cgi\" method=\"POST\">"
                     << "<input type=\"hidden\" name=\"username\" value=\""<<result[0]["username"]<<"\">"
                     << "<input type=\"hidden\" name=\"password\" value=\""<<result[0]["password"]<<"\">"
                     << "<input type=\"submit\" value=\"Account\">"
                     << "</form>\n";
                cout << "<a href=\"\">Cart</a>\n"
                     << "<a href=\"index.html\">Log out</a>\n"
                << "</div>" << endl;

                cout << "<div class=\"main row\">\n";
                // CartInv
                query.reset();
                query << "SELECT * FROM 01671166_CartInv WHERE userID = "<<Get["userID"]<<";";
                query.parse();
                mysqlpp::StoreQueryResult cartItems = query.store();
                if (cartItems) {
                    double totalCost = 0;

                    cout << "<div class=\"cartitem-container col-7\">\n";
                    cout << "<h2>Items in cart:</h2>\n";
                    for (int i = 0; i < cartItems.num_rows(); ++i) {
                        query.reset();
                        query << "SELECT * FROM 01671166_Products WHERE pID = "<<cartItems[i]["pID"]<<";";
                        query.parse();
                        mysqlpp::StoreQueryResult item = query.store();
                        if (item) {
                            double tempPrice = item[0]["price"];
                            int tempQTY = cartItems[i]["qty"];
                            double tempTotal = tempPrice * tempQTY;
                            totalCost += tempTotal;

                            
                            cout << "<div class=\"cartitem\">"
                                 << "<h4>" << item[0]["name"] << "</h4>"
                                 << "<p><strong>Item type:</strong> " << item[0]["type"] << "</p>"
                                 << "<p><strong>Quantity in cart:</strong> " << cartItems[i]["qty"] << "</p>"
                                 << "<p><strong>Cost:</strong> $" << tempTotal << "</p>"
                            << "</div>\n";
                        } else {
                            cout << "Issue with product query" << endl;
                        }
                    }
                    cout << "</div>" << endl;

                    cout << "<div class=\"col-2 ms-auto\">\n";
                    cout << "<h3>Check out:</h3>\n"
                        << "<p><strong>Total cost: $" << totalCost << "</strong><p>\n";
                    cout << "<form action=\"checkout.cgi\" method=\"GET\">"
                         << "<input type=\"hidden\" name=\"userID\" value=\""<<Get["userID"]<<"\">"
                         << "<input style=\"width:8em;\" type=\"submit\" value=\"Checkout\">"
                         << "</form>" << endl;
                    cout << "</div>\n</div>" << endl;
                } else {
                    cout << "Issue with cart query" << endl;
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