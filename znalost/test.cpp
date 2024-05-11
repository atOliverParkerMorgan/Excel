#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>

using namespace std;

class A
{
public:
    A ( int x = 67 ) { m_X = x; }
    void print ( void ) const { cout << m_X; }
private:
    int m_X;
};

class B : public A
{
public:
    B ( int x, int y ) { m_Y = y; }
    void print ( void ) const { A::print (); cout << m_Y; }
private:
    int m_Y;
};

void foo ( B & val )
{
    val . print ( );
}

int main ( void )
{
    B test ( 5, 27 );

    foo ( test );
    return 0;
}