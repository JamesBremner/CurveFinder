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
    string myName;
    string myC;
    cLine( json::Array& coords )
    {
        json::Array c1 = coords[0];
        x1 = c1[0];
        y1 = c1[1];
        c1 = coords[1];
        x2 = c1[0];
        y2 = c1[1];
    }
    cLine( const string& name, const string& sc )
    : myName( name )
    , myC( sc )
    {

    }
    string Text()
    {
        stringstream ss;
        //ss << x1 <<" "<< y1 <<" "<< x2 <<" "<< y2;
        ss << myName <<": " << myC;
        return ss.str();
    }
};

vector < cLine > vLine;

void ReadKML()
{
    ifstream inf("data/LRS_Routes.kml");
    if( ! inf.is_open() )
    {
        cout << "cannot open kml file\n";
        exit(1);
    }
    string str;
    string route_name;
    string sc;
    bool placemark = false;
    while( getline(inf, str ))
    {
        if( ! placemark ) {
        if( str.find("<Placemark>") == -1 )
            continue;
        }
        placemark = true;
        if( str.find("</Placemark>") != -1 ) {
            placemark = false;
            continue;
        }
        if( str.find("FULL_ROUTE_NAME") != -1 )
        {
            int n = str.find(">");
            int m = str.find("<",n+1);
            route_name = str.substr(n+1,m-n-1);
        }
        int n = str.find("<coordinates>");
        if( n != -1 )
        {
            n += 13;
            int m = str.find("</coordinates>");
            sc = str.substr(n,m-n);
            //cout << route_name <<":"<<sc<<"\n";
            vLine.push_back( cLine( route_name, sc ) );
        }

        //cout << str << "\n";
    }
    }

int main()
{
    // read lines from shapefile into vector

//    ifstream inf("shapefile.json");
//    string str;
//    while( getline(inf, str ))
//    {
//        json::Value shapefile = json::Deserialize(str);
//        if (shapefile.GetType() == json::NULLVal)
//        {
//            std::cout << "cannot read shapefile\n";
//            exit(1);
//        }
//        json::Array jc = shapefile["coordinates"];
//        vLine.push_back( cLine( jc ) ) ;
//    }

    ReadKML();

    // Display lines stored in vector

    int k = 0;
    for( auto& l : vLine ) {
        if( k++ > 5 )
            break;
        cout << l.Text() << "\n\n";
    }

    return 0;
}
