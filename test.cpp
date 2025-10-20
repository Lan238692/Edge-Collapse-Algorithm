
#include "function.h"

bool ispress;
double sumx,sumy;
double startx,starty;
double deltax,deltay;

double zoom;
double smooth_ratio;
double s_count;

int mh,mw;//Store the size of the window

struct e** edge;
struct p** point;
struct f** face;

void init(void)
{
	zoom=0.5;
	mh=mw=600;
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH,GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

}

void display(void)
{	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
    glLoadIdentity();
	
	//Draw a coordinate axis
	glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1000, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0,0,0);
    glVertex3f(0,1000,0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1000);
    glEnd();

	srand(0);
    glBegin(GL_TRIANGLES);
	for(int i=0;face[i]!=NULL;i++)
	{
		glBegin(GL_TRIANGLES);
		glClearColor(0,0,0,0);
		
		//Color the faces randomly
		GLfloat t=(GLfloat)rand()/(GLfloat)32767;
		if(t<0.1)
			t+=0.1;
        glColor4f(1,1,1,t);
		glVertex3f(face[i]->p1->x,face[i]->p1->y,face[i]->p1->z);
		glVertex3f(face[i]->p2->x,face[i]->p2->y,face[i]->p2->z);
		glVertex3f(face[i]->p3->x,face[i]->p3->y,face[i]->p3->z);
		glEnd();
	}
    glFlush();
} 

void reshape(int w,int h)
{
	mw=w;mh=h;
	display();
	glViewport(0,0,(GLsizei) w,(GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1, 1000.0);
	
	
	//Set camera
	gluLookAt(zoom*(50)*sin((double)((sumx+deltax)/100+1)), zoom*(50+sumy+deltay), zoom*(50)*cos((double)((sumx+deltax)/100+1)), 0.0, 0.0, 0.0, 0.0, 5, 0.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
} 

void rezoom()
{
	glViewport(0,0,(GLsizei) mw,(GLsizei) mh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(60.0, (GLfloat) mw/(GLfloat) mh, 1, 10000.0);
    gluLookAt(zoom*(50)*sin((double)((sumx+deltax)/100+1)), zoom*(50+sumy+deltay), zoom*(50)*cos((double)((sumx+deltax)/100+1)), 0.0, 0.0, 0.0, 0.0, 5, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	display();
    glLoadIdentity();
} 

//Move the view by dragging and dropping the mouse
void OnMouse(int button,int state,int x,int y)
{
	if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
	{
		startx=x;
		starty=y;
		ispress=1;
	}
	if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP)
	{
		ispress=0;
		sumx+=deltax;
		sumy+=deltay;
		deltax=deltay=0;
		rezoom();
	}
}
void OnMove(int x,int y)
{
	if(ispress==1)
	{
		deltax=x-startx;
		deltay=y-starty;
		rezoom();
	}
}
//Optional function
void OnKeyboard(unsigned char key, int x, int y)
{
    if (key == 'q')
	{
		zoom*=0.97;
		rezoom();
    }
    if (key == 'w')
	{
		if(zoom<5)
			zoom*=1.05;
		rezoom();
    }
	if (key == 'd')
	{
		int count=getfacecount(face);
		cout<<"The graph has "<<count<<" faces, please input the simplified ratio (between 0~1):";
		double p;
		cin>>p;
		if(p>0&&p<1)
		{
			p=(int)(count*p);
			int i(0);
			while(getfacecount(face)>p&&i<100)
			{
				getqmatrix(face,point);
				del_edge(point,edge,face);
				i++;
			}
			rezoom();
			cout<<"The graph has "<<getfacecount(face)<<" faces left now."<<endl;
		}
	}
	if(key=='s')
	{
		if(getfacecount(face)>1)
		{
			getqmatrix(face,point);
			del_edge(point,edge,face);
			rezoom();
			cout<<"The graph has "<<getfacecount(face)<<" faces left now."<<endl<<endl;
		}
		else
		{
			cout<<"Can not simplify anymore!"<<endl;
		}
	}
	if(key=='x')
	{
		char* filename=new char[100];
		cout<<"Please input filename for saving: ";
		cin>>filename;
		savefile(point,edge,face,filename);
	}
	if(key=='z')
	{
		char* filename=new char[100];
		cout<<"Please input filename for loading('example1.obj', 'example2.obj' for default): ";
		cin>>filename;
		loadfile(point,edge,face,filename);
		rezoom();
		cout<<"load finished"<<endl;
	}
	if(key=='c')
	{
		if(smooth_ratio==0)
		{
			smooth_ratio=0.01;
		}
		smooth(point,edge,smooth_ratio);
		rezoom();
	}
	if(key=='t')
	{
		for(int i=0;point[i]!=NULL;i++)
		{
			point[i]->x++;
			rezoom();
		}
	}
	if(key=='y')
	{
		for(int i=0;point[i]!=NULL;i++)
		{
			point[i]->x--;
			rezoom();
		}
	}
	if(key=='u')
	{
		for(int i=0;point[i]!=NULL;i++)
		{
			point[i]->y++;
			rezoom();
		}
	}
	if(key=='i')
	{
		for(int i=0;point[i]!=NULL;i++)
		{
			point[i]->y--;
			rezoom();
		}
	}
	if(key=='o')
	{
		for(int i=0;point[i]!=NULL;i++)
		{
			point[i]->z++;
			rezoom();
		}
	}
	if(key=='p')
	{
		for(int i=0;point[i]!=NULL;i++)
		{
			point[i]->z--;
			rezoom();
		}
	}
	if(key=='v')
	{
		del_vertex(point,edge,face);
		rezoom();
	}
}



int main(int argc,char** argv)
{
	//store the information of points, edges and faces
	point=new p*[MAX];
	edge=new e*[MAX];
	face=new f*[MAX];

	loadfile(point,edge,face,"example3.obj");
	printf(face);
	printe(edge);
	cout<<"Welcome!"<<endl;
	cout<<"S: simplify once"<<endl;
	cout<<"D: simplify by ratio"<<endl;
	cout<<"Q: zoom in"<<endl;
	cout<<"W: zoom out"<<endl;
	cout<<"X: save"<<endl;
	cout<<"Z: load"<<endl;
	cout<<"C: smooth"<<endl;
	cout<<"V: vertex delete"<<endl;
	cout<<"T Y U I O P: translation"<<endl;
	cout<<"Drag your mouse while pressing left: spin"<<endl;
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH ); 
	glutInitWindowSize(600,600);
	glutInitWindowPosition(800,100);
	glutCreateWindow("RESULT");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(OnMouse);
	glutMotionFunc(OnMove);
	glutKeyboardFunc(OnKeyboard);
	glutMainLoop();

	return 0;
}