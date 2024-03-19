@echo
echo starting servlet server server

rem # clear environment
set tmp=
set temp=
set prompt=
set PATH=
set CLASSPATH=

rem # Set the complete path to a Java Runtime Environment 
rem # This should be the location of .\bin\java.exe...
rem # NOTE: ommit trailing '\'!!!!
SET JRE=C:\Progra~1\javasoft\jre\1.2
SET LIB=%JRE%\lib

SET CLASSPATH=.\java\classes;.\java\lib\jsdk.jar;%LIB%\rt.jar;%LIB%\jaws.jar;%LIB%\i18n.jar

%JRE%\bin\java.exe -cp %CLASSPATH% comal.ServletServer -d .\java\classes -p 8080