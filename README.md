# SITE SUMMARY

Zamazor (my site’s name) is an e-commerce web application. You can create an account and log
in, and you can browse the products available. Some product recommendations are available on
the home page, and you can view different categories of items if you wish (you can sort items in
these categories). If you want to view an item’s properties in more detail, you can click on an
item to go to its page, learn more about it and add it to your cart. When you have added items
that you want to your cart, you can check those items out, which will remove them from their
database. The site also allows you to modify your account information any time you want using
the account menu, or even delete it if necessary.

# TABLES

**01671 166_Users**

This table contains all of the user information. This table is used in login, registration, modifying
account information, or viewing products (so that they can be added to the right cart). The userID
is the primary identifier for a user.


**01671 166_Products**

This table contains all of the product information. This table is used in viewing products - the
homepage, category pages, product pages and the cart. The pID is the primary identifier for a
product.

**01671 166_CartInv**

This table contains all of the cart information. It specifies which products are in which users’ cart
(along with quantity). It is used when adding items to a cart, viewing items in a cart and checking
items out of a cart. There is no primary identifier here as all of the columns can have duplicate
values in practical use.


[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=9257712&assignment_repo_type=AssignmentRepo)
