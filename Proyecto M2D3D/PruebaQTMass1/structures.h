#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <vector>
#include <string>

struct DataParam {
    int Height;
    int Width;
    int Depth;
    double Qmin;
    double Qmax;
    double Qstep;
    std::vector<std::string> FileNames;
    std::vector<int> Dividers;

};



#endif // STRUCTURES_H
