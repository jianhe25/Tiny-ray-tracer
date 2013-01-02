// Readfile definitions 

class scene
{
private:
	bool readvals (stringstream &s, const int numvals, GLfloat * values) ;

public:
	scene(arguments);
	~scene();

	void readfile (const char * filename) {} ;
	/* data */
};


