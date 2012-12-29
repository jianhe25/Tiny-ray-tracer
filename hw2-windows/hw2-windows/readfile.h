// Readfile definitions 

void matransform (stack<mat4> &transfstack, GLfloat * values) ;
void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, GLfloat * values) ;
void readfile (const char * filename) ;
