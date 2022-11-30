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
    cout << "<head><title>Zamazor - Account</title>" << endl;
    cout << "<meta charset=\"utf-8\"><link rel=\"icon\" type=\"image/x-icon\" href=\"zamazor.ico\">" << endl;
    cout << "<link rel=\"stylesheet\" href=\"account.css\" type=\"text/css\" /></head>";
    cout << "<body>" << endl;

    int userID = loginValid(Post);

    if (userID > 0) {
        mysqlpp::Connection conn(false);
        if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {
            mysqlpp::Query query = conn.query();
            query << "SELECT * FROM 01671166_Users WHERE userID = "<<userID<<";";
            query.parse();

            mysqlpp::StoreQueryResult result = query.store();
            if(result) {
                cout << "<div class=\"sidenav\">\n";
                cout << "<form action=\"shop.cgi\" method=\"POST\">"
                     << "<input type=\"hidden\" name=\"username\" value=\"" << result[0]["username"]<<"\">"
                     << "<input type=\"hidden\" name=\"password\" value=\"" << result[0]["password"]<<"\">"
                     << "<input type=\"submit\" value=\"Home\">"
                     << "</form>" << endl;
                cout << "<a href=\"#\">Account</a>\n"
                     << "<a href=\"cart.cgi?userID=" << result[0]["userID"] << "\">Cart</a>\n"
                     << "<a href=\"index.html\">Log out</a>\n"
                << "</div>" << endl;

            
                cout << "<div class=\"main\">\n";
                // Add labels to the rest
                cout << "<h1>Account</h1>"
                    << "<form action=\"update.cgi\" method=\"POST\">\n"
                    << "Account number: <input type=\"text\" name=\"userID\" id=\"user_id\" value=\""<<result[0]["userID"]<<"\" readonly><br>"
                    << "Username*: <input type=\"text\" name=\"username\" placeholder=\"Username\" value=\""<<result[0]["username"]<<"\" required><br>\n"
                    << "E-Mail: <input type=\"email\" name=\"email\" placeholder=\"Email\" value=\""<<result[0]["email"]<<"\"><br>\n"
                    << "Password (hidden)*: <input type=\"password\" name=\"password\" placeholder=\"Password\" value=\""<<result[0]["password"]<<"\" id=\"pass\" required>"
                    << "<input type=\"checkbox\" onclick=\"toggle()\"><br><br><br>\n"
                    << "First name*: <input type=\"text\" name=\"first_name\" placeholder=\"First name\" value=\""<<result[0]["firstName"]<<"\" required><br>\n"
                    << "Last name*: <input type=\"text\" name=\"last_name\" placeholder=\"Last name\" value=\""<<result[0]["lastName"]<<"\" required><br><br>\n"
                    << "Address: <input type=\"text\" name=\"address\" placeholder=\"Address\" value=\""<<result[0]["address"]<<"\"><br>\n"
                    << "City: <input type=\"text\" name=\"city\" placeholder=\"City\" value=\""<<result[0]["city"]<<"\"><br>\n"
                    << "State*: <input type=\"text\" name=\"state\" list=\"statelist\" placeholder=\"ST\" value=\""<<result[0]["state"]<<"\" required>\n"
                    << "<datalist id=\"statelist\" name=\"statelist\">\n";
                    stateList();
                    cout << "</datalist>\n"
                    << "<br>\n"
                    << "Zipcode: <input type=\"text\" name=\"zipcode\" placeholder=\"Zipcode\" value=\""<<result[0]["zipcode"]<<"\"><br>\n" 
                    << "<p>NOTES:</p>"
                    << "<ul><li>Any value marked with an asterisk (*) cannot be left blank.</li>"
                    << "<li>If you wish to leave a value empty, enter 'NULL'.</li></ul>"
                    << "<input type=\"submit\" value=\"Update Information\">\n</form>\n"
                    << "<form action=\"account-delete.cgi\" method=\"POST\">"
                    << "<input type=\"hidden\" name=\"userID\" value=\""<<result[0]["userID"]<<"\">"
                    << "<input style=\"background-color: red;\" type=\"submit\" value=\"Delete Account\">\n</form>\n" << endl;
                cout << "</div>" << endl;
            } else {
                cout << "Error with query" << endl;
            }
        } else {
            cout << "Error with connection" << endl;
        }

    } else {
        cout << "<div class=\"main skinny\"><h1>Login failed</h1>\n<p>" << endl;
        if (userID == -10) {
            cout << "We were unable to confirm your login information. "
                 << "Please go to the link below to confirm your login." << endl;
        } else if (userID == -11) { cout << "The provided username was not found in our database." << endl; }
        else if (userID == -12) { cout << "The provided password does not match the provided user." << endl; }
        else if (userID == -90) { cout << "The connection to our database failed. Sorry for the inconvenience." << endl; }
        else if (userID == -91) {
            cout << "An issue occured when trying to retrieve information from our database."
                 << "Sorry for the inconvenience." << endl;
        } else { cout << "Unknown invalid code returned. Sorry for the inconvenience." << endl; }

        cout << "</p><br>\n<a href=\"confirm.html\">Confirm login</a>" << endl;
        cout << "<br><a href=\"../index.html\">Return to home page</a></div>" << endl;
    }


    cout << "</body>\n";

    cout << "<script>"
    << "function toggle() {\n"
    << "\tvar p = document.getElementById(\"pass\");\n"
    << "\tif (p.type === \"password\") {p.type = \"text\"}\n"
    << "\telse {p.type = \"password\"}\n}"
    << "</script>" << endl;
    
    
    cout << "</html>" << endl;
    return 0;
}

// IF RETURNS VALUE > 0 IS GOOD OTHERWISE NOT
//   RETURN VAL IS USER ID
int loginValid(map<string,string> Post) {
    if (Post.find("username") != Post.end() && Post.find("password") != Post.end()) {
        mysqlpp::Connection conn(false); // Exceptions suppressed to keep code simple
        if(conn.connect("cse278", "localhost", "cse278", "MonSepM1am1")) {

            mysqlpp::Query query = conn.query();
            query << "SELECT userID, username, password FROM 01671166_Users;";
            query.parse();

            mysqlpp::StoreQueryResult result = query.store();
            if(result) {

                for (int i = 0; i < result.num_rows(); i++) {
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

void stateList() {
    cout << "<option value=\"AL\">Alabama</option>"
                <<"<option value=\"AK\">Alaska</option>"
                <<"<option value=\"AZ\">Arizona</option>"
                <<"<option value=\"AR\">Arkansas</option>"
                <<"<option value=\"CA\">California</option>"
                <<"<option value=\"CO\">Colorado</option>"
                <<"<option value=\"CT\">Connecticut</option>"
                <<"<option value=\"DE\">Delaware</option>"
                <<"<option value=\"DC\">District Of Columbia</option>"
                <<"<option value=\"FL\">Florida</option>"
                <<"<option value=\"GA\">Georgia</option>"
                <<"<option value=\"HI\">Hawaii</option>"
                <<"<option value=\"ID\">Idaho</option>"
                <<"<option value=\"IL\">Illinois</option>"
                <<"<option value=\"IN\">Indiana</option>"
                <<"<option value=\"IA\">Iowa</option>"
                <<"<option value=\"KS\">Kansas</option>"
                <<"<option value=\"KY\">Kentucky</option>"
                <<"<option value=\"LA\">Louisiana</option>"
                <<"<option value=\"ME\">Maine</option>"
                <<"<option value=\"MD\">Maryland</option>"
                <<"<option value=\"MA\">Massachusetts</option>"
                <<"<option value=\"MI\">Michigan</option>"
                <<"<option value=\"MN\">Minnesota</option>"
                <<"<option value=\"MS\">Mississippi</option>"
                <<"<option value=\"MO\">Missouri</option>"
                <<"<option value=\"MT\">Montana</option>"
                <<"<option value=\"NE\">Nebraska</option>"
                <<"<option value=\"NV\">Nevada</option>"
                <<"<option value=\"NH\">New Hampshire</option>"
                <<"<option value=\"NJ\">New Jersey</option>"
                <<"<option value=\"NM\">New Mexico</option>"
                <<"<option value=\"NY\">New York</option>"
                <<"<option value=\"NC\">North Carolina</option>"
                <<"<option value=\"ND\">North Dakota</option>"
                <<"<option value=\"OH\">Ohio</option>"
                <<"<option value=\"OK\">Oklahoma</option>"
                <<"<option value=\"OR\">Oregon</option>"
                <<"<option value=\"PA\">Pennsylvania</option>"
                <<"<option value=\"RI\">Rhode Island</option>"
                <<"<option value=\"SC\">South Carolina</option>"
                <<"<option value=\"SD\">South Dakota</option>"
                <<"<option value=\"TN\">Tennessee</option>"
                <<"<option value=\"TX\">Texas</option>"
                <<"<option value=\"UT\">Utah</option>"
                <<"<option value=\"VT\">Vermont</option>"
                <<"<option value=\"VA\">Virginia</option>"
                <<"<option value=\"WA\">Washington</option>"
                <<"<option value=\"WV\">West Virginia</option>"
                <<"<option value=\"WI\">Wisconsin</option>"
                <<"<option value=\"WY\">Wyoming</option>\n" << endl;
}
