==========================================
 Web Comal - ReadMe
 PandD Software - Rotterdam (Netherlands)
==========================================

Index
-----
1. What's in this package?
2. How
3. Disclaimer
4. Final


============================
 1. What's in this package?
============================
The WebCOMALxx.zip contains the following files and directories

file\path			description
------------------------------- ------------------------------------------------
\comal\				Comal directory
\comal\internet.mod		New and improved internet module: 
				  more information is provided from the servlet
\comal\sample.cml		Sample program that illustrates the use of 
				  the internet.mod
\comal\sample2.cml		Sample program illustrating Comal's CHAIN command
\comal\samplet.cml		Sample program in WebCOMAL.doc
\comal\*.cml                    Other programs
\comal\lst			Directory for listed Comal files
...
\java\				Java directory
\classes			Compiled java classes root
\classes\com			Commercial....
\classes\com\borland		Borland uniquely reverse DNS!!
\classes\com\borland\jb		Jbuilder path
\classes\com\borland\jb.Diagnostic.class
				Class for assertion (forget to compile without it...)
\classes\comal\			Comal class root
\classes\comal\ComalServlet.class
				The servlet that executes the COMAL interpreter
\classes\comal\ComalServletException.class
				The comal servlet exception class
\classes\comal\ReadBufferThread.class
				The separate process that handles the io.
\classes\comal\ServletServer.class
				The servlet server that loads the servlet(s)...
\lib\				Library path	
\lib\jsdk.jar			Servlet class library 
				  (not the most recent one but it should work...)
\source\                        Source path
\source\comal			Same as classes\comal but now with source
...
\html\                          Directory for HTML files
\html\WebComal.html	        The web page calling the Comal servlet!!!!!
\html\WebChain.html		Web page illustrating Comal's CHAIN command
\html\test.html                 Simple caller (see webCOMAL.doc)           
...
\startcs.bat			Batch file for starting the COMAL Servlet Server
\startcs.pif                    Program information file for (DOS) batch process
\readme.txt			What you're reading now
...
\webCOMAL.doc                   Document file in Word 7 format (read it!)


========
 2. How
========
All you have to do is the following.

- Extract the contents of the WebComal.zip to any directory.

First: the Java Stuff

- You should have a JRE 1.2 (at least) available. 
  (downloadable at http://www.javasoft.com)

- Change the JRE environment variable in the start.bat that you just extracted
  and make sure it points to a Java Runtime Environment.
  
  If you installed the JRE 1.2.2 to the default install directory for instance 
  the JRE variable shoud be something like:
  
  SET JRE=C:\Progra~1\JavaSoft\JRE\1.2
  
  Just to be sure, enter only 8.3 names... !!!
  
- Change the port address for the servlet server if necessary 
  (use '-p 8080' or another port)
  
- Use the startcs.bat to start up the servlet server.

Second: The Comal stuff

- Copy the files (internet.mod, sample.cml, etc.) from the just extracted 
  directory to your COMAL interpreter root (eg. 'C:\COMAL\').

- If you copy the files to another location be sure you change ALL 
  the 'C:\COMAL\' occurences in the html\WebComal.html to the directory 
  you copied the files to.
  Do the same with other .html files

Third: The HTML stuff

- Check if the host and port name in the WebComal.html are correct. 
  And change them if necessary.
  
- Is the servlet server running? See last item in First.

- Use either InterneExplorer (or Netscape) to display the HTML file.

- Press the submit button or the highlighted link....


===============
 3. Disclaimer
===============
In no event the authors and/or PandD Software will be liable to you for any 
special, consequential, direct, indirect or similar damages.


==========
 4. Final
==========
It all should work now.....I hope.
Good luck.


============
Information:
PandD Software - Rotterdam (Netherlands)
E-mail: comal@pandd.demon.nl

[dkpk22032000]