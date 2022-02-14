/* first probe of programming with c++ */

class GraphObject{
protected:
	double x, y;	
	int color;
public:
	GraphObject(double ax, double ay, int clr) 
		: x(ax), y(ay), color(clr) {}
	virtual ~GraphObject() {}
	virtual void Show() = 0;
	virtual void Hide() = 0;
	void Move(double nx, double ny);
};

class Pixel : public GraphObject {
public:
	Pixel(double x, double y, int col)
		: GraphObject(x, y, col) {}
	virtual ~Pixel() {}
	virtual void Show();
	virtual void Hide();
};

class Circle : public GraphObject {
	double rad;
public:
	Circle(double x, double y, int col, double srad) 
		: GraphObject(x, y, col), rad(srad) {}
	virtual ~Circle() {}
	virtual void Show();
	virtual void Hide();
};


	
void GraphObject::Move(double nx, double ny)
{
	Hide();
	x = nx;
	y = ny;
	Show();
}

class PolygonalChain : public GraphObject {
	struct Vertex {
		double dx, dy;
		Vertex *next;
	};
	Vertex *first;
public:
	PolygonalChain(double b, double x, int color)
		: GraphObject(x, y, color), first(0) {}
	virtual ~PolygonalChain();
	void AddVertex(double adx, double ady);
	virtual void Show();
	virtual void Hide();
};

void PolygonalChain::AddVertex(double adx, double ady)
{
	Vertex *tmp = new Vertex;
	tmp->dx = adx;
	tmp->dy = ady;
	tmp->next = first;
	first = tmp;
}

PolygonalChain::~PolygonalChain()
{
	while(first) {
		Vertex *tmp = first;
		first = first->next;
		delete tmp;
	}
}

class Square : public PolygonalChain {
public:
	Square(double x, double y, int color, double a)
		: PolygonalChain(x, y, color) 
	{
		AddVertex(0,0);
		AddVertex(0,a);
		AddVertex(a,a);
		AddVertex(a,0);
		AddVertex(0,0);
	}
};		
