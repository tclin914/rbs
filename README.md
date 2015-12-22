# Remote Batch System

Write a CGI program to receive an HTTP request described as follows.<br>
The parameters of the HTTP request are:

    h1=140.113.210.101   # the first server's IP.
    p1=7000              # the first server's port.
    f1=batch_file1       # the batch file name redirected to the first ras server.
    h2=140.113.210.103   # the second server's IP.
    p2=7000              # the second server's port.
    f2=batch_file2       # the batch file name redirected to the second ras server.
    h3=140.113.210.103   # the third server's IP.
    p3=7000              # the third server's port.
    f3=batch_file3       # the batch file name redirected to the third rwg server.
    h4=                  # no more server
    h5=                  # no more server

Then, the CGI program connects to the three ras or rwg servers and the redirects the batch file (stored in the HTTP server) as input to these servers.<br>
When receiving messages, send these messages back to the browser as the returning web page.<br>
Note that the returning web page include three columns, one for displaying the returning messages from the each server.

A sample of the web page is given on the board.

        <meta http-equiv="Content-Type" content="text/html; charset=big5" />
        <title>Network Programming Homework 3</title>
        </head>
        <body bgcolor=#336699>
        <font face="Courier New" size=2 color=#FFFF99>
        <table width="800" border="1">
        <tr>
        <td>140.113.210.145</td><td>140.113.210.145</td><td>140.113.210.145</td></tr>
        <tr>
        <td valign="top" id="m0"></td><td valign="top" id="m1"></td><td valign="top" id="m2"></td></tr>
        </table>
        <script>document.all['m0'].innerHTML += "****************************************<br>";</script>
        <script>document.all['m0'].innerHTML += "** Welcome to the information server. **<br>";</script>
        <script>document.all['m0'].innerHTML += "****************************************<br>";</script>
        <script>document.all['m1'].innerHTML += "****************************************<br>";</script>
        <script>document.all['m1'].innerHTML += "** Welcome to the information server. **<br>";</script>
        <script>document.all['m1'].innerHTML += "****************************************<br>";</script>
        <script>document.all['m0'].innerHTML += "% <b>ls</b><br>";</script>
        <script>document.all['m1'].innerHTML += "% <b>removetag test.html</b><br>";</script>
        <script>document.all['m1'].innerHTML += "<br>";</script>
        <script>document.all['m1'].innerHTML += "Test<br>";</script>
        <script>document.all['m1'].innerHTML += "This is a test program<br>";</script>
        <script>document.all['m1'].innerHTML += "for ras.<br>";</script>
        <script>document.all['m1'].innerHTML += "<br>";</script>
        <script>document.all['m1'].innerHTML += "<br>";</script>
        <script>document.all['m1'].innerHTML += "% <b></b><br>";</script>
        ....
        </font>
        </body>
        </html>

## Requirements

* The number of target servers is not greater than 5.
* The information about batch is sent by users by ``GET`` method.
* The parameters will be either complete or all missing.<br>
    e.g, http://yourserver/?h1=140.113.111.111&p1=5555&f1=t1.txt&h2=&p2=&f2
    in this case parameter for server1 is complete, parameters for server2 is missing,<br> 
    i.e, you don't need to consider the case of partly missing parameters.
* Your CGI program should show one table which lists the results from your ras/rwg servers.
* A large file will be tested, that is, you should not suppose that write will always success. You should handle write event properly.
