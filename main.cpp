#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

class cLine
{
public:
    double x1, y1, x2, y2;
    string myName;
    string myC;
//    cLine( json::Array& coords )
//    {
//        json::Array c1 = coords[0];
//        x1 = c1[0];
//        y1 = c1[1];
//        c1 = coords[1];
//        x2 = c1[0];
//        y2 = c1[1];
//    }
    cLine( const string& name, const string& sc )
        : myName( name )
        , myC( sc )
    {

    }
    cLine( double ix1, double iy1, double ix2, double iy2 )
        : x1( ix1 )
        , y1( iy1 )
        , x2( ix2 )
        , y2( iy2 )
    {

    }
    string Text()
    {
        stringstream ss;
        ss << x1 <<" "<< y1 <<" "<< x2 <<" "<< y2;
        return ss.str();
    }
};

class cPlaceMark
{
public:
    string myName;
    string myC;
    vector < cLine > myLine;
    vector < double > myAngles;

    cPlaceMark()
    {

    }
    cPlaceMark( const string& name, const string& sc )
        : myName( name )
        , myC( sc )
    {
        Parse();
        Angles();
    }
    string Text()
    {
        stringstream ss;
        ss << "\n" << myName << ": (" << myLine.size() << ")\n";
//        int k = -1;
//        for( auto& l : myLine )
//        {
//            if( k++ == 1 )
//            {
//                k = 0;
//                ss << "\n";
//            }
//            ss << l.Text() << " || ";
//        }
        ss << "Angles: ";
        for( auto& a : myAngles )
        {
            ss << a << " ";
        }
        return ss.str();
    }
    void Add( const cLine& line )
    {
        myLine.push_back( line );
    }
    void Angles()
    {
        myAngles.clear();
        double x1,y1,x2,y2,x3,y3;
        int k = 0;
        for( auto& l : myLine )
        {
            if( k++ == 0 )
            {
                x1 = l.x2;
                y1 = l.y2;
                x2 = l.x1;
                y2 = l.y1;
            }
            else
            {
                k = 0;
                x3 = l.x2;
                y3 = l.y2;

                float dx21 = x2-x1;
                float dx31 = x3-x1;
                float dy21 = y2-y1;
                float dy31 = y3-y1;
                float m12 = sqrt( dx21*dx21 + dy21*dy21 );
                float m13 = sqrt( dx31*dx31 + dy31*dy31 );
                float theta = 57.2958 * acos( (dx21*dx31 + dy21*dy31) / (m12 * m13) );
                if( theta > 90 )
                    theta = 180 - theta;
                myAngles.push_back( theta );
            }
        }
    }

    vector< double >& getAngles()
    {
        return myAngles;
    }

private:
    void Parse()
    {
        double x1,y1;
        int k = 0;
        int n = 0;
        string t = myC;
        do
        {
            double x = atof(t.c_str() );
            int m = t.find( ",", n );
            double y = atof(t.substr(m+1).c_str());

            if( k++ > 1 )
            {
                myLine.push_back( cLine( x1, y1, x, y ));
            }

            x1 = x;
            y1 = y;

            n = t.find(" ");
            t = t.substr(n+1);
        }
        while( n != -1 );
    }

};

vector < cPlaceMark > vPlaceMark;

void Test()
{
    cPlaceMark pm;
    pm.Add( cLine(0,0,1,0));
    pm.Add( cLine(1,0,2,0.05));
    pm.Angles();
}
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
        if( ! placemark )
        {
            if( str.find("<Placemark>") == -1 )
                continue;
        }
        placemark = true;
        if( str.find("</Placemark>") != -1 )
        {
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
            //vLine.push_back( cLine( route_name, sc ) );
            cPlaceMark pm( route_name, sc );
            cout << pm.Text() << "\n";
            vPlaceMark.push_back( pm );
        }

        //cout << str << "\n";
    }
}

int main()
{
    Test();

    ReadKML();

    vector <double> AllAngles;
    for( auto& pm : vPlaceMark )
    {
        AllAngles.insert( AllAngles.end(), pm.getAngles().begin(), pm.getAngles().end() );
    }
    double max = *max_element( AllAngles.begin(), AllAngles.end() );
    vector<int> histo( 100 );
    for( auto a : AllAngles )
    {
        histo[ (int)a ]++;
    }
    for( int k = 0; k < 99; k++ )
    {
        cout << k <<" " << histo[k] << "\n";
    }
    return 0;
}
