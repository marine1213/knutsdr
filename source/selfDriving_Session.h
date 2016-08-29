#ifndef	_SESSION_H_
#define _SESSION_H_

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <queue>

using namespace std;


class Point{
    public:
	float x,y;

	Point():x(0),y(0){};	
	Point(int ix,int iy):x((float)ix),y((float)iy){};
	Point(float ix,float iy):x(ix),y(iy){};

	Point delta(Point *p){
	    return Point(p->x - this->x,p->y - this->y);
	}

	float multi(Point *p){
	    return (this->x*p->x + this->y*p->y);
	}

	float distance(Point *p){
	    Point temp = delta(p);
	    return sqrt(temp.x*temp.x + temp.y*temp.y);
	}

	float cosine(Point *p1, Point *p2){
	    Point v1 = delta(p1);
	    Point v2 = delta(p2);
	    float mulCoor = v1.multi(&v2);
	    float d1 = distance(p1);
	    float d2 = distance(p2);
	    float mulLen  = d1*d2;
	    float cosVal  = mulCoor/mulLen;
	    return cosVal;
	}
	
	float angle(Point *p1,Point *p2){
	    float d1 = distance(p1);
	    float d2 = distance(p2);
	    
	    Point v1 = Point(p1->x - this->x, this->y - p1->y);
	    Point v2 = Point(p2->x - this->x, this->y - p2->y);

	    float a1 = v1.multi(&v2) / (d1*d2);
	    float angle = acos(a1) * 180 / 3.14;
	    if((v2.x*v1.y - v2.y*v1.x)<=0) 
		angle = 360 - angle;
	    if(angle > 180)
		angle -= 360;

	    return angle;
	}
};

class Line{
    public:
	Point vtcp;
	Point p;
	float cosAlpha;
	float sinAlpha;

	Line(Point *p1, Point *p2){
	    vtcp = p1->delta(p2);
	    p = *p1;
	    Point yAxis = Point(0,1);
	    float angle = Point().angle(&yAxis, &vtcp);
	    cout<<" angle = "<< angle << endl;
	    float cosin = Point().cosine(&yAxis, &vtcp);
	    cout<<" cosine = "<< cosin<< endl;
	}
};
class QArray{
    private:
	Point 	*data;
	int	lastId;
	int	qsize;
	int	num;
    public:
	QArray(int s):lastId(0),qsize(s),num(0){ data = (Point*)malloc(s*sizeof(Point));};
		~QArray()	{ free(data);	};
	void 	add(Point p)	{ data[lastId%qsize] = p; lastId++; num += (num + 1) > qsize? 0 : 1; };
	Point 	front(int id)	{ return data[(lastId + 1 + id) % qsize];};
	Point 	back(int id)	{ return data[(lastId - id) % qsize];};
	int 	size() 		{ return num; };
	
};
class Session{
    private:
	char *id;
    public:
	queue<Point> target;
	queue<char*> inf;
	queue<char*> msg;

	const char* getId() {return id;};
	Session(char *sessionId){
	    id = (char*)malloc(sizeof(sessionId));
	    strcpy(id,sessionId);
	};

	~Session(){
	    if(id)		delete id;
	    freeQueue(inf);
	    freeQueue(msg);
	};
	
	void freeQueue(queue<char*> q){
		while(!q.empty()){
		    char* c = q.front();
		    if(c)
			free(c);
		    q.pop();
		}	
    	}
};
#endif