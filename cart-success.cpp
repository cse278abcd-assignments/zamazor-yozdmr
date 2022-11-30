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
    cout << "<head><title>Zamazor - Added to Cart</title>" << endl;
                cout << "<meta charset=\"utf-8\"><link rel=\"icon\" type=\"image/x-icon\" href=\"zamazor.ico\">" << endl;
                cout << "<link rel=\"stylesheet\" href=\"newstyle.css\" type=\"text/css\" /></head>" << endl;

    if (Get.find("pID") != Get.end() && Get.find("qty") != Get.end() && Get.find("userID") != Get.end()) {
        mysqlpp::Connection conn(false);
        if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {

            mysqlpp::Query query = conn.query();
            query << "INSERT INTO 01671166_CartInv VALUES (" << Get["userID"] << ", " << Get["qty"] << ", " << Get["pID"] << ");" << endl;
            query.parse();
            mysqlpp::SimpleResult addToCart = query.execute();

            query.reset();
            query << "SELECT qty FROM 01671166_Products WHERE pID = " << Get["pID"] << ";" << endl;
            query.parse();
            mysqlpp::StoreQueryResult getqty = query.store();

            query.reset();
            query << "UPDATE 01671166_Products SET qty = " << getqty[0]["qty"] - stoi(Get["qty"]) << " WHERE pID = " << Get["pID"] << ";" << endl;
            query.parse();
            mysqlpp::SimpleResult updtProd = query.execute();

            cout << "<div style=\"width:30%;text-align:center\" class=\"ctgr\">\n";
            if (addToCart && updtProd) {
                query.reset();
                query << "SELECT username, password FROM 01671166_Users WHERE userID = "<<Get["userID"]<<";" << endl;
                query.parse();
                mysqlpp::StoreQueryResult userInfo = query.store();

                cout << "Your product has been added!" << endl;
                cout << "<form action=\"shop.cgi\" method=\"POST\">"
                     << "<input type=\"hidden\" name=\"username\" value=\"" << userInfo[0]["username"]<<"\">"
                     << "<input type=\"hidden\" name=\"password\" value=\"" << userInfo[0]["password"]<<"\">"
                     << "<input type=\"submit\" value=\"Return to shop\">"
                     << "</form>" << endl;
            } else {
                cout << "One of the previous queries failed." << endl;
            }
            cout << "</div>";
        }
    } else {
        cout << "Did not receive expected values." << endl;
    }

    return 0;
}