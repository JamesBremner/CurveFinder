#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include "json.h"

using namespace std;

class cLine
{
public:
    double x1, y1, x2, y2;
    cLine( json::Array& coords )
    {
        json::Array c1 = coords[0];
        x1 = c1[0];
        y1 = c1[1];
        c1 = coords[1];
        x2 = c1[0];
        y2 = c1[1];
    }
    string Text()
    {
        stringstream ss;
        ss << x1 <<" "<< y1 <<" "<< x2 <<" "<< y2;
        return ss.str();
    }
};

vector < cLine > vLine;

int main()
{
    // read lines from shapefile into vector

    ifstream inf("shapefile.json");
    string str;
    while( getline(inf, str ))
    {
        json::Value shapefile = json::Deserialize(str);
        if (shapefile.GetType() == json::NULLVal)
        {
            std::cout << "cannot read shapefile\n";
            exit(1);
        }
        json::Array jc = shapefile["coordinates"];
        vLine.push_back( cLine( jc ) ) ;
    }

    // Display lines stored in vector

    for( auto& l : vLine )
        cout << l.Text() << "\n";

    return 0;
}
