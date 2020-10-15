// SCRIPT  TESTED
::using(eSOLIB);

lib := LIB("libglfw.so");
lib2 := LIB("libGL.so.1.0.0");

var srv = SRV(8000,"srv");
GL_COLOR_BUFFER_BIT := 0x00004000

lib.load("glfwInit",true,0);
lib.load("glfwCreateWindow",true,5);
lib.load("glfwMakeContextCurrent",false,1);
lib.load("glfwWindowShouldClose",true,1);
lib.load("glfwSwapBuffers",false,1);
lib.load("glfwPollEvents",false,0);
lib.load("glfwTerminate",false,0);

lib2.load("glLoadIdentity",false,0);
lib2.load("glColor3f",false,3);
lib2.load("glVertex3f",false,3);
lib2.load("glTranslatef",false,3);
lib2.load("glBegin",false,1);
lib2.load("glEnd",false,0);
lib2.load("glClearColor",false,4);

lib2.load("glMatrixMode",false,1);
lib2.load("glLoadIdentity",false,0);
lib2.load("glOrtho",false,6);
lib2.load("glMatrixMode",false,1);
lib2.load("glLoadIdentity",false,0);
lib2.load("glRotatef",false,4);
lib2.load("glViewport",false,4);


lib2.load("glClear",0,1);

GL_TRIANGLES   :=  0x0004
KKK := 0;

function main(x)
{
    println("program gl window");

   var ii = glfwInit();

   var window = glfwCreateWindow(640, 480, "Hello World", null, null);
   if (window==null)
   {
       println("---------iit terminated ");
       glfwTerminate();
       return false;
   }

   glfwMakeContextCurrent(window);

   while (!glfwWindowShouldClose(window))
   {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0,0.0,0.5,0.0);
        var ratio = 640.0 / 480.0;

        glViewport(0, 0, 600, 400);
        glMatrixMode(0x1701);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0);

        glMatrixMode(0x1700);
        glLoadIdentity();
        glRotatef(KKK++, 0.0, 0.0, 1.0);

        glBegin(GL_TRIANGLES);
        glVertex3f(-0.6, -0.4, 0.0);
        glVertex3f(0.0, 0.60, 0.0);
        glVertex3f(0.60, -0.40, 0.0);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
   }

   glfwTerminate();
   return false;
}
