package comal;

import com.borland.jb.util.Diagnostic;
import javax.servlet.*;
import javax.servlet.http.*;
import java.io.*;
import java.util.*;

public class ComalServlet extends HttpServlet
{
  // constant used for COMAL interpreter init parameter
  private static final String sCOMAL_EXE   = "COMAL_EXE";
  // constant used for COMAL program parameter
  private static final String sCOMAL_PRG   = "COMAL_PRG";
  // constant used for environment variable for temporary input filename
  private static final String sSERVLET_IN  = "SERVLET_IN";
  // constant used for environment variable for temporary output filename
  private static final String sSERVLET_OUT = "SERVLET_OUT";
  // constant with DOS Console startup
  private static final String sCOMMAND     = "COMMAND.COM /C";
  // constant prefix for temporary input file
  private static final String sPREFIX_IN   = "CSI";
  // constant prefix for temporary output file
  private static final String sPREFIX_OUT  = "CSO";

  // Full path to Comal interpreter
  // (a single \ indicates in Java an escape character like in C e.g. \t for tab)
  private String comalCmd = "";
  //Full path to Comal program
  private String comalPrg = "";

  // temporaray comal input file
  private File comalInFile;
  // temporary comal output file
  private File comalOutFile;

  /**
   * Initializes the Servelet.
   *
   * This method is called by the Java Servlet enabled web server to initialize
   * the servlet.
   *
   * In our case the servlet will use the COMAL_EXE parameter that should be
   * entered in the Java Servlet enabled web server when it is installed.
   *
   * @param config is the avaible servlet configuration
   */
  public void init(ServletConfig config) throws ServletException
  {
    // call inherited init
    super.init(config);
    try
    {
      // try to load init parameter
      comalCmd = getInitParameter(sCOMAL_EXE);
      // error is reported
      if (comalCmd == null)
        throw new ComalServletException(this, "No COMAL interpreter provided in servlet init param: " + sCOMAL_EXE);
    }
    catch (Exception e)
    {
      // error handled and printed
      e.printStackTrace();
    }
  }

  /**
   * Simple method that returns some information about this servlet
   *
   * @returns a string describing this servlet
   */
  public String getServletInfo()
  {
    return "ComalServlet is a servlet that starts the COMAL interpreter to perform web scripting using the COMAL language.";
  }

  /**
   * The main method for any servlet.
   *
   * If any HTTP method is called e.g. POST or GET this method is called.
   * A parameter is read that should give the path to the COMAL program that
   * should be loaded.
   *
   * @param request hold information about the input from the client(browser)
   *        side.
   * @param response is the portal back to the client (browser)
   *        The response has a outputstream that can be used to write text or
   *        HTML to.
   */
  public void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
  {
    Diagnostic.precondition(request != null, "(PK) null as HttpServletRequest not expected!");
    Diagnostic.precondition(response != null, "(PK) null as HttpServletResponse not expected!");

    // refresh comal prg variable
    comalPrg = null;

    try
    {
      // if comal interpreter path was not loaded try a non-init parameter...
      if (comalCmd == null)
      {
        comalCmd = request.getParameter(sCOMAL_EXE);
        if (comalCmd == null)
          // error is reported
          throw new ComalServletException(this, "No comal program provided in "+ sCOMAL_EXE + " <PARAM> tag.");

      }
      // locate the full path for the requested comal program in the <PARAM> tag
      // with the value COMAL_PRG
      comalPrg = request.getParameter(sCOMAL_PRG);
      if (comalPrg == null)
        // error is reported
        throw new ComalServletException(this, "No comal program provided in "+ sCOMAL_PRG + " <PARAM> tag.");
    }
    catch (Exception e)
    {
      // error handled and printed both to console of web server and to the
      // outputstream of the servlet: The client (browser) will show this...
      writeException(response.getWriter(), e);
      e.printStackTrace();
    }

    // start comal
    startComal(request, response);

    // close the response outputstream.
    response.getWriter().close();
  }

  /**
   * The COMAL process is started.
   *
   * @param request hold information about the input from the client(browser)
   *        side.
   * @param response is the portal back to the client (browser)
   *        The response has a outputstream that can be used to write text or
   *        HTML to.
   */
  private void startComal(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
  {
    Diagnostic.precondition(request != null, "(PK) null as HttpServletRequest not expected!");
    Diagnostic.precondition(response != null, "(PK) null as HttpServletResponse not expected!");
    try
    {
      // create the temporary files for respectively in and output for COMAL
      comalInFile = File.createTempFile(sPREFIX_IN,"");
      comalOutFile = File.createTempFile(sPREFIX_OUT,"");

      // write the information that can be obtained from the request.
      // that is information about the clien side, servlet host etc.
      PrintWriter info = new PrintWriter(new FileWriter(comalInFile));
      writeInfoToComal(request, info);
      info.close();

      // set the full command path
      // should be something like: COMMAND /C C:\COMAL\COMAL.EXE C:\COMAL\inettest.cml
      String cmd = sCOMMAND + " " + comalCmd + " " + comalPrg;
      // set the environment variables containing the in and output file paths
      String env[] = new String[2];
      env[0] =  sSERVLET_IN +  "=" + comalInFile.getAbsolutePath() + ".";
      env[1] =  sSERVLET_OUT + "=" + comalOutFile.getAbsolutePath() + ".";

      // start the process
      // COMAL is started in a seperate process
      Process comalProcess = Runtime.getRuntime().exec(cmd, env);
      // JAVA immediately returns...and continues the code

      // start up to seperate processes that read as input the ouput from COMAL
      // and possible error coming from the process.
      ReadBufferThread in = new ReadBufferThread(comalProcess.getInputStream(), response.getWriter());
      ReadBufferThread errors = new ReadBufferThread(comalProcess.getErrorStream(), response.getWriter());

      // this statement will keep Java from continueing until COMAL has stopped.
      comalProcess.waitFor();
      // tell the two processes that are reading output the can stop when done.
      in.stop();
      errors.stop();

      // this is is necessary to wait until the processes really have died!!!
      while (in.isRunning() || errors.isRunning())
      {
        // the current process will wait...
        Thread.currentThread().sleep(0);
      }

      // if the running COMAL caused a DOS exit value... (neccesary?)
      if (comalProcess.exitValue() != 0)
      {
         throw new ComalServletException(this, "Command error " + comalProcess.exitValue());
      }

      // read COMAL output file and write it as output to the client (browser)
      // until all is read.
      LineNumberReader fi = new LineNumberReader(new FileReader(comalOutFile));
      while ( fi.ready() )
      {
        response.getWriter().println(fi.readLine());
      }
      fi.close();

      // delete the two temporary files
      if (!comalInFile.delete()) Diagnostic.out.println("No!");
      if (!comalOutFile.delete()) Diagnostic.out.println("No!");
    }
    catch (Exception e)
    {
      writeException(response.getWriter(), e);
    }
  }

  /**
   * Writes any exception to the output using a HTML format
   *
   * @param out is the output buffer
   * @param e is the exception that is written to ouput
   */
  private static void writeException(PrintWriter out, Exception e)
  {
    Diagnostic.precondition(out != null, "(PK) null as PrintWriter not expected!");
    Diagnostic.precondition(e != null, "(PK) null as Exception not expected!");
    out.println("<H2>Comal Servlet Exception:</H2>");
    out.println("<PRE>");
    e.printStackTrace(out);
    out.println("</PRE>");
  }

  /**
   * Gives back a string that will never be <code>null</code>
   *
   * @param s is the string that is returned when s != <code>null</code>
   * @return a non-<code>null</code> string
   */
  private String notNullString(String s)
  {
    if (s != null)
      return s;
    else
     return "";
  }

  /**
   * Write information about e.g. the client(browser)-side as output
   *
   * The information that is written is information about the client-host,
   * the server host, the protocol and more...
   *
   * If for instance a for is used the <INPUT...> tags are al read and also
   * written as output.
   *
   */
  private void writeInfoToComal(HttpServletRequest request, PrintWriter out) throws ServletException, IOException
  {
    Diagnostic.precondition(request != null, "(PK) null as HttpServletRequest not expected!");
    Diagnostic.precondition(out != null, "(PK) null as PrintWriter not expected!");
    // write client side information
    out.println( notNullString(request.getAuthType()) );
    out.println( notNullString(request.getCharacterEncoding()));
    out.println( request.getContentLength());
    out.println( notNullString(request.getContentType()));
    out.println( notNullString(request.getMethod()) );
    out.println( notNullString(request.getPathInfo()));
    out.println( notNullString(request.getPathTranslated()));
    out.println( notNullString(request.getProtocol()));
    out.println( notNullString(request.getQueryString()));
    out.println( notNullString(request.getRemoteAddr()));
    out.println( notNullString(request.getRemoteHost()));
    out.println( notNullString(request.getRemoteUser()));
    out.println( notNullString(request.getScheme()));
    if ((getServletConfig() != null) && (getServletContext()) != null)
      out.println( notNullString(getServletConfig().getServletContext().getServerInfo()));
    else
      out.println("");
    out.println( notNullString(request.getServerName()));
    out.println( request.getServerPort());
    out.println( notNullString(request.getServletPath()));

    writeParameters(request, out);
    writeHeaders(request, out);

    // be sure buffer is written
    out.flush();
  }

  private void writeParameters(HttpServletRequest request, PrintWriter out)
  {
    Diagnostic.precondition(request != null, "(PK) null as HttpServletRequest not expected!");
    Diagnostic.precondition(out != null, "(PK) null as PrintWriter not expected!");
    // get all available <PARAM> and <INPUT> tag names and values
    Vector params = new Vector();
    for (Enumeration e = request.getParameterNames(); e.hasMoreElements();)
    {
      String name = (String) e.nextElement();
      String value = request.getParameter(name);
      params.add(name+'='+notNullString(value));
    }
    // write down the number of available parameters
    out.println(params.size());
    // write down each parameter as "name=value"
    for (Enumeration e = params.elements(); e.hasMoreElements();)
    {
      String param= (String) e.nextElement();
      out.println(param);
    }
  }

  private void writeHeaders(HttpServletRequest request, PrintWriter out)
  {
    Diagnostic.precondition(request != null, "(PK) null as HttpServletRequest not expected!");
    Diagnostic.precondition(out != null, "(PK) null as PrintWriter not expected!");
    // get all available headers from request
    Vector headers = new Vector();
    for (Enumeration e = request.getHeaderNames(); e.hasMoreElements();)
    {
      String name = (String) e.nextElement();
      String value = request.getHeader(name);
      headers.add(name+'='+notNullString(value));
    }
    // write down the number of availeble headers
    out.println(headers.size());
    // write down each header as "name=value"
    for (Enumeration e = headers.elements(); e.hasMoreElements();)
    {
      String header = (String) e.nextElement();
      out.println(header);
    }
  }
}


/**
 * Simple Exception for this servlet
 */
class ComalServletException extends UnavailableException
{
  public ComalServletException(Servlet servlet, String s)
  {
    super(servlet, s);
  }
}


class ReadBufferThread implements Runnable
{
  // local thread
  private Thread localThread;
  // line number reader
  private LineNumberReader in;
  // print writer for output
  private PrintWriter out;
  // running flag
  private boolean running;
  // empty flag for inputstream
  private boolean empty;

  /**
   * Constructs the ReadBufferThread
   *
   * @param is is the InnputStream that is read.
   * @oaram out is the PrintWriter used for output the information read.
   */
  ReadBufferThread(InputStream is, PrintWriter out)
  {
    Diagnostic.precondition(is != null, "(PK) null as InputStream not expected!");
    Diagnostic.precondition(out != null, "(PK) null as PrintWriter not expected!");
    this.in = new LineNumberReader(new InputStreamReader(new BufferedInputStream(is)));
    this.out = out;
    localThread = new Thread(this);
    start();
  }

  /**
   * Start reading by starting the local thread
   * Some initialization is done to bee able to stop reading and fully read
   * the buffer before doing so.
   */
  public void start()
  {
    localThread.start();
    running = true;
    empty = false;
  }

  /**
   * Stop reading.
   *
   * The thread will die when the buffer is empty.
   *
   * It might be necessary to wait until thread really is stopped:
   * <code>
   *  ReadBufferThread foo = new ReadBufferThread(fooIn, fooPrintWriter);
   *  ...
   *  ...
   *  foo.stop();
   *  while (foo.isRunning())
   *  {
   *     Thread.currentThread().sleep(0);
   *  }
   * </code>
   */
  public synchronized void stop()
  {
    running = false;
  }

  /**
   * Is there still being read?
   *
   * As long as the read buffer is not empty we will continue to run....
   *
   * @return the state of still reading.
   */
  public boolean isRunning()
  {
    return running || !empty;
  }

  /**
   * Actual reading of the buffer.
   *
   * As long as we are running the input stream is read.
   */
  public void run()
  {
    try
    {
      while (isRunning())
      {
        Diagnostic.out.print(".");
        empty = !in.ready();
        // do we have a read inputstream?
        if ( !empty )
        {
          String s = in.readLine();
          // only output non null strings...
          if (s != null)
            out.println(s);
        }
      }
      Diagnostic.out.println(":");
    }
    catch (Exception e)
    {
      e.printStackTrace();
    }
  }
}


