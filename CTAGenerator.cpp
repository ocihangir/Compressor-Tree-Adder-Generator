#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
// A patch for to_string issue on g++
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "opencv2/opencv.hpp"

using namespace std;

struct GPC // Generalized Parallel Counter
{   int a;
    int b;
    int c;
    int x;
    double ratio;
    int coveringGPC;
    GPC(int k, int l, int m, int q, int cov){
            a = k;
            b = l;
            c = m;
            x = q;
            coveringGPC = cov;
            
            ratio = static_cast<double> (k + l + m) / static_cast<double>(q);
        }
    GPC operator=(GPC u)
        {
            this->a = u.a;
            this->b = u.b;
            this->c = u.c;
            this->x = u.x;
            this->ratio = u.ratio;
            this->coveringGPC = u.coveringGPC;
            
            return *this;
        }
    bool operator==(GPC u)
        {
            if ((this->a == u.a) &&
                (this->b == u.b) && 
                (this->c == u.c))
                return true;
            return false;
        }
};



struct DOT
{
    int rank;
    string name;
    string inputLutName;
};

struct LUT
{
    vector<DOT> inputDots;
    vector<DOT> outputDots;
    int targetGPC;
    string uniqueName;
};

struct LogicSet
{
    vector<DOT> dots;
};

struct LAYER
{
    vector<LogicSet> lSets;
    vector<LUT> luts;
};



int multA = 12;
int multB = 12;

const int M = 6;
const int N = 4;
int k = 3;

// Open CV
int squareWidth = 20;
int squareHeight = 20;

void printList(vector<int> prList);
LAYER generatePartialProducts(int,int,ostringstream &file_out);
int findTallestColumn(vector<int> colList);
LAYER compressLayer(LAYER input, int layerNumber, ostringstream &file_out);
vector<GPC> generateCoveringGPCs(int M, int N);
vector<GPC> generatePrimitiveGPCs(vector<GPC> &covGPCs);
vector<GPC> generateGPCs(int M, int N);
void printGPCs(vector<GPC> &gpcList);
void sortGPCs(vector<GPC> &listGPC);
void sortGPCsByCover(vector<GPC> &listGPC);
void printLayers(vector<LAYER> &layerList);
bool testPattern(vector<int> &rankList, GPC targetGPC, int targetRank);
void generateLUT(LAYER &input, vector<int> &rankList, GPC targetGPC, int selectedRank, LAYER &genLayer, int layerNumber, int lutNumber, ostringstream &file_out);
void connectDotsDirectly(LAYER &input, vector<int> &rankList, int selectedRank, LAYER &genLayer);
vector<int> generateRankList(LAYER input);
uint64_t generateLUTOutput(int a, int b, int c, int q);
int findTallestColumnSize(vector<int> colList);
void generateFinalAdder(LAYER sumLayer, int k, ostringstream &file_out);

void drawLayers(cv::Mat &image, vector<LAYER> layers);

vector<GPC> gpcList;
vector<LAYER> layers;

int main( int argc, char *argv[] )
{
    if (argc > 1)
    {
        multA = atoi(argv[1]);
        multB = atoi(argv[2]);
    }
    
    if (argc > 3)
        k = atoi(argv[3]);
        
    string fname;
    fname = "mult_" + patch::to_string(multA) + "x" + patch::to_string(multB) + "_lut6.v";
    
    cout << "Generating " << patch::to_string(multA) << "x" << patch::to_string(multB) << "multiplier unit." << endl;
    cout << "File name : " << fname << endl << endl;
    
    ostringstream file_out;
    file_out << "// (* RLOC_ORIGIN = \"X0Y0\" *)" << endl;
    file_out << "module mult_" << patch::to_string(multA) << "x" << patch::to_string(multB) << "_lut6(in0, in1, mult_out);" << endl << endl;
    file_out << "input [" << patch::to_string(multA - 1) << ":0] in0;" << endl;
    file_out << "input [" << patch::to_string(multA - 1) << ":0] in1;" << endl;
    file_out << "output [" << patch::to_string(multB * 2) << ":0] mult_out;" << endl << endl;
    
    // Create GPC tabel
    gpcList = generateGPCs(M, N);
    printGPCs(gpcList);
    
    // Generate partial products as Layer0
    LAYER partialProducts = generatePartialProducts(multA,multB,file_out);
    layers.push_back(partialProducts);
    
    int layerNumber = 0;
    // Generate layers
    while(findTallestColumnSize(generateRankList(layers.back()))>k)
    {
        LAYER res = compressLayer(layers.back(), layerNumber++, file_out);
        layers.push_back(res);
    }
    
    generateFinalAdder(layers.back(), k, file_out);
    
    file_out << "endmodule" << endl;
    
    
    ofstream myfile;
    myfile.open (fname.c_str());
    myfile << file_out.str();
    myfile.close();
    
    int totalColSize = 0;
    for(int i=0;i<layers.size();i++)
        totalColSize+=(findTallestColumnSize(generateRankList(layers[i]))+1);
    
    cout << "DONE!" << endl;
    
    cv::Mat image(totalColSize*squareHeight,(multA+multB+2)*squareWidth,CV_8UC3,cv::Scalar(255,255,255));
    drawLayers(image, layers);
    cv::imshow("Multiplier", image);
    
    string imname;
    imname = "mult_" + patch::to_string(multA) + "x" + patch::to_string(multB) + "_lut6.jpg";
    imwrite( imname, image );
    
    
    cv::waitKey(0);
    
    // printLayers(layers);
    
    // printList(generateRankList(layers.back()));
}

void drawLayers(cv::Mat &image, vector<LAYER> layers)
{
    cv::RNG rng( 0xFFFFFFFF );
    unordered_map<string, cv::Scalar> color4luts;
    for (int lyrCnt=0;lyrCnt<layers.size();lyrCnt++)
        for (int lut = 0; lut<layers[lyrCnt].luts.size(); lut++)
            color4luts[layers[lyrCnt].luts[lut].uniqueName] = cv::Scalar( rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) );
                
    
    int prevHeight = 0;
    for (int lyrCnt=0;lyrCnt<layers.size();lyrCnt++)
    {
        vector<int> rankList = generateRankList(layers[lyrCnt]);
        for (int i=0;i<rankList.size();i++)
            rankList[i] = 0;
            
        for (int lset=0; lset<layers[lyrCnt].lSets.size(); lset++)
        {
            for (int dot=0;dot<layers[lyrCnt].lSets[lset].dots.size();dot++)
            {
                
                if (lyrCnt+1<layers.size())
                for (int lut=0;lut<layers[lyrCnt+1].luts.size();lut++)
                {
                    for (int lutdot=0;lutdot<layers[lyrCnt+1].luts[lut].inputDots.size();lutdot++)
                    {
                        if (layers[lyrCnt].lSets[lset].dots[dot].name == layers[lyrCnt+1].luts[lut].inputDots[lutdot].name)
                        {
                            int thickness = -1;
                            int lineType = 8;
                            cv::rectangle( image,
                            cv::Point( (layers[lyrCnt].lSets[lset].dots[dot].rank * squareWidth), (prevHeight*squareHeight) + (rankList[layers[lyrCnt].lSets[lset].dots[dot].rank]*squareHeight) ),
                            cv::Point( (layers[lyrCnt].lSets[lset].dots[dot].rank * squareWidth) + squareWidth, (prevHeight*squareHeight) + (rankList[layers[lyrCnt].lSets[lset].dots[dot].rank]*squareHeight) + squareHeight),
                            color4luts[layers[lyrCnt+1].luts[lut].uniqueName],
                            thickness,
                            lineType );
                        }
                    }
                }
                cv::Point pt = cv::Point((layers[lyrCnt].lSets[lset].dots[dot].rank * squareWidth)+(squareWidth/2), (prevHeight*squareHeight) + (rankList[layers[lyrCnt].lSets[lset].dots[dot].rank]*squareHeight) + (squareHeight/2));
                int thickness = -1;
                int lineType = 8;
                cv::circle(image,pt,squareHeight/6,cv::Scalar(0,0,0),thickness,lineType );
                rankList[layers[lyrCnt].lSets[lset].dots[dot].rank]++;
            }
        }
        prevHeight += (findTallestColumnSize(rankList) + 1);
    }
    
    cv::flip(image,image,1);
}

void generateFinalAdder(LAYER sumLayer, int k, ostringstream &file_out)
{
    vector<int> rankList = generateRankList(sumLayer);
    
    // Generate final adder
    for (int i=0; i<k; i++)
        file_out << "wire [" << patch::to_string(rankList.size()-1) << ":0] adderIn" << patch::to_string(i) << ";" << endl;
        
    file_out << "wire [" << patch::to_string(rankList.size()) << ":0] adderOut;" << endl << endl;
    
    string sep[k] = "";
    
    bool adder0Added = false;
    bool closeLoop = false;
    ostringstream adder[k];
    for (int i = rankList.size()-1; i>=0; i--)
    {
        int addCount = k;
        for (int j = 0; j<sumLayer.lSets.size(); j++)
        {
            for ( int u = 0; u<sumLayer.lSets[j].dots.size(); u++)
            {
                if (sumLayer.lSets[j].dots[u].rank == i)
                {
                    adder[(addCount--)-1] << sep[addCount-1] << sumLayer.lSets[j].dots[u].name;
                    sep[addCount] = ",";
                }
            }
        }
        if (addCount>0)
            for (int m=addCount;m>0;m--)
            {
                adder[(m)-1] << sep[m-1] << "1'b0";
                sep[m-1] = ",";
            }
    }
    
    for (int i=0;i<k;i++)
        file_out << "assign adderIn" << i << " = {" << adder[i].str() << "};" << endl;
    
    file_out << "assign adderOut = adderIn0";
    for (int i=1;i<k;i++)
        file_out << "+ adderIn" << i;
    
    file_out << ";" << endl;
    file_out << "assign mult_out = adderOut;" << endl << endl;
}

vector<GPC> generateGPCs(int M, int N)
{
    
    cout << "Creating GPC tables..." << endl;
    cout << "GPC table file name : gpc_modules.v" << endl << endl;
    
    vector<GPC> covGPCs = generateCoveringGPCs(M, N);
    
    cout << "Covering GPC List : " << endl;
    printGPCs(covGPCs);
    
    vector<GPC> primGPCs = generatePrimitiveGPCs(covGPCs);
    
    cout << "Primitive GPC List : " << endl;
    
    // Eliminate repeats in GPC list
    for (int i=0;i<primGPCs.size()-1;i++)
        for (int j=i+1;j<primGPCs.size();j++)
        {
            if (primGPCs[i] == primGPCs[j])
                {
                    primGPCs.erase(primGPCs.begin() + j);
                    // cout << "Removed repeating gpc (" << patch::to_string(primGPCs[j].a) << "," << patch::to_string(primGPCs[j].b) << "," << patch::to_string(primGPCs[j].c) << ";" << patch::to_string(primGPCs[j].x) << ") - " << patch::to_string(primGPCs[j].coveringGPC) << endl;
                    i = 0;
                    j = 1;
                    break;
                }
            }
    
    std::ostringstream gpc_modules;
    gpc_modules << "// GPC modules for LUT6 " << endl;
    gpc_modules << "// This file is generated by CTAGenerator " << endl << endl;
    
    for (vector<GPC>::iterator it=primGPCs.begin();it!=primGPCs.end(); ++it)
    {
        gpc_modules << "// GPC(" << (*it).a << "," << (*it).b << "," << (*it).c << ";" << (*it).x << ")" << " compression ratio : " << (*it).ratio << endl;
        gpc_modules << "module gpc" << (*it).a << (*it).b << (*it).c << "(in, out);" << endl << endl;
        gpc_modules << "input [" << patch::to_string((*it).a + (*it).b + (*it).c - 1) << ":0] in;" << endl;
        gpc_modules << "output [" << patch::to_string((*it).x-1) << ":0] out;" << endl << endl;
        if ((covGPCs[(*it).coveringGPC].a == (*it).a) &&
            (covGPCs[(*it).coveringGPC].b == (*it).b) &&
            (covGPCs[(*it).coveringGPC].c == (*it).c) &&
            (covGPCs[(*it).coveringGPC].x == (*it).x))
        { // It is covering GPC
            
            
            for (int i=0; i<(*it).x; i++)
            {
                gpc_modules << "// (* RLOC = \"X0Y0\" *)" << endl;
                gpc_modules << "LUT6 #(.INIT(64'h" << hex << generateLUTOutput((*it).a, (*it).b, (*it).c, i) << dec << "))";
                gpc_modules << " LUT6_" << i << "_ (.O(out[" << i << "]), ";
                string sep = "";
                for (int j=0; j < ((*it).a + (*it).b + (*it).c); j++)
                {
                    gpc_modules << sep << " .I" << patch::to_string(j) << "(in[" << patch::to_string(j) << "])";
                    sep = ",";
                }
                
                // Tie unused LUT6 inputs to 0
                for (int j=5; j>((*it).a + (*it).b + (*it).c)-1; j--)
                    gpc_modules << sep << " .I" << patch::to_string(j) << "(1'b0)";
                
                    
                gpc_modules << ");" << endl << endl;
            }
        } else { // It is generated GPC
            if( (*it).x < covGPCs[(*it).coveringGPC].x )
                gpc_modules << "wire [" << patch::to_string(covGPCs[(*it).coveringGPC].x - 1) << ":0] temp;" << endl;
                
            gpc_modules << "gpc" << covGPCs[(*it).coveringGPC].a << covGPCs[(*it).coveringGPC].b << covGPCs[(*it).coveringGPC].c << " gpc" << covGPCs[(*it).coveringGPC].a << covGPCs[(*it).coveringGPC].b << covGPCs[(*it).coveringGPC].c << "_inst(.in({";
            
            int inCount = (*it).a + (*it).b + (*it).c;
            
            int zeros = covGPCs[(*it).coveringGPC].a - (*it).a;
            for (int i=0;i<zeros;i++)
                gpc_modules << "1'b0,";
            for (int i=(*it).a;i>0;i--)
                gpc_modules << "in[" << patch::to_string(--inCount) << "],";
            
            zeros = covGPCs[(*it).coveringGPC].b - (*it).b;
            for (int i=0;i<zeros;i++)
                gpc_modules << "1'b0,";
            for (int i=(*it).b;i>0;i--)
                gpc_modules << "in[" << patch::to_string(--inCount) << "],";
            
            string sep = "";
            zeros = covGPCs[(*it).coveringGPC].c - (*it).c;
            for (int i=0;i<zeros;i++)
            {
                gpc_modules << sep << "1'b0";
                sep = ",";
            }
            for (int i=(*it).c;i>0;i--)
            {
                gpc_modules << sep << "in[" << patch::to_string(--inCount) << "]";
                sep = ",";
            }
                
            gpc_modules << "}), .out(";
            if ( (*it).x < covGPCs[(*it).coveringGPC].x )
                gpc_modules << "temp" << "));" << endl;
            else
                gpc_modules << "out" << "));" << endl;
            
            if( (*it).x < covGPCs[(*it).coveringGPC].x )
                gpc_modules << "assign out = temp[" << patch::to_string((*it).x - 1) << ":0];" << endl;
        }
        gpc_modules << "endmodule" << endl << endl;
    }
    ofstream myfile;
    myfile.open ("gpc_modules.v");
    myfile << gpc_modules.str();
    myfile.close();
    
    return primGPCs;
}

uint64_t generateLUTOutput(int a, int b, int c, int q)
{
    unsigned long long int res = 0;
    
    for (int i=0; i<64; i++)
    {
        int val=i;
        int bit_val=0;
        int aVal = a;
        int bVal = b;
        int cVal = c;
        while(cVal-->0)
        {
            bit_val += (val % 2) * 1;
            val = (val>>1);
        }
        while(bVal-->0)
        {
            bit_val += (val % 2) * 2;
            val = (val>>1);
        }
        while(aVal-->0)
        {
            bit_val += (val % 2) * 4;
            val = (val>>1);
        }
        res += static_cast<uint64_t>(static_cast<uint64_t>((bit_val >> q) % 2) << i);
    }
    
    return res;
}

vector<GPC> generateCoveringGPCs(int M, int N)
{
    vector<GPC> resGPCs;
    
    // This function is supposed to generate
    // covering GPCs for M and N. At the moment, 
    // GPCs are hardcoded for N=3 and N=4
    
    resGPCs.push_back(GPC(0,0,6,3,0));
    resGPCs.push_back(GPC(0,1,5,3,1));
    resGPCs.push_back(GPC(0,2,3,3,2));
    // resGPCs.push_back(GPC(0,1,2,3,3));
    // resGPCs.push_back(GPC(0,0,3,2,4));
    if (N>3)
    {
        resGPCs.push_back(GPC(0,2,4,4,3));
        resGPCs.push_back(GPC(1,1,4,4,4));
        resGPCs.push_back(GPC(0,3,3,4,5));
        resGPCs.push_back(GPC(1,2,3,4,6));
        resGPCs.push_back(GPC(2,1,3,4,7));
        resGPCs.push_back(GPC(0,4,2,4,8));
        resGPCs.push_back(GPC(1,3,2,4,9));
        resGPCs.push_back(GPC(2,2,2,4,10));
        // resGPCs.push_back(GPC(1,1,2,4,13));
        // resGPCs.push_back(GPC(0,3,1,3,11));
    }
    
    return resGPCs;
}

vector<GPC> generatePrimitiveGPCs(vector<GPC> &covGPCs)
{
    vector<GPC> primGPCs;
    
    for (int i=0; i<covGPCs.size(); i++)
    {
        GPC target = covGPCs[i];
        
        for (int a=target.a;a>=0;a--)
        {
            for (int b=target.b;b>=0;b--)
            {
                if ((b==0) && (a>0)) // rule b cannot be 0 if a is greater than 0
                    break;
                for (int c=target.c;c>1;c--) // rule c > 1
                {
                    int x = 0;
                    int q = 4*a + 2*b + c;
                    if (q < 4)
                        x = 2;
                    else if (q < 8)
                        x = 3;
                    else if (q < 16)
                        x = 4;
                    else
                        x = 0;
                    GPC resGPC(a,b,c,x,i);
                    if (resGPC.ratio > 1)
                        primGPCs.push_back(resGPC);
                }
            }
        }
    }
    
    sortGPCs(primGPCs);
    
    sortGPCsByCover(primGPCs);
    
    return primGPCs;
}

// Sorts GPCs by their compression ratio
void sortGPCs(vector<GPC> &listGPC)
{
    bool swapped = false;
    do
    {
        swapped = false;
        for (int i=1;i<listGPC.size();i++)
        {
            if(listGPC[i].ratio>listGPC[i-1].ratio)
            {
                GPC tmpGPC = listGPC[i-1];
                listGPC[i-1] = listGPC[i];
                listGPC[i] = tmpGPC;
                swapped = true;
            }
        }
    }
    while(swapped == true);
    
}

// Sorts GPCs by their covering dots
void sortGPCsByCover(vector<GPC> &listGPC)
{
    bool swapped = false;
    do
    {
        swapped = false;
        for (int i=1;i<listGPC.size();i++)
        {
            if(listGPC[i].ratio!=listGPC[i-1].ratio)
                continue;
            if((listGPC[i].a+listGPC[i].b+listGPC[i].c)>(listGPC[i-1].a+listGPC[i-1].b+listGPC[i-1].c))
            {
                GPC tmpGPC = listGPC[i-1];
                listGPC[i-1] = listGPC[i];
                listGPC[i] = tmpGPC;
                swapped = true;
            }
        }
    }
    while(swapped == true);
    
}

// Generates partial products
LAYER generatePartialProducts(int A, int B, ostringstream &file_out)
{
    vector<int> pp;
    
    
    vector<LogicSet> lSets;
    
    for (int i=0; i<A; i++)
    {
        vector<DOT> dots;
        
        file_out << "wire [" << patch::to_string(B-1) << ":0] pp" << patch::to_string(i) << ";" << endl;
        file_out << "assign pp" << patch::to_string(i) << " = (in1[" << patch::to_string(i) << "]) ? in0 : " << patch::to_string(B) << "'b0;" << endl;
        
        for (int j=0; j<B; j++)
        {
            DOT dot;
            dot.rank = i+j;
            dot.name = "pp" + patch::to_string(i) + "[" + patch::to_string(j) + "]";
            dots.push_back(dot);
        }
            
        LogicSet lSet;
        lSet.dots = dots;
        lSets.push_back(lSet);
    }
    
    LAYER resLayer;
    resLayer.lSets = lSets;
    
    return resLayer;
}

vector<int> generateRankList(LAYER input)
{
    vector<int> rankList;
    for (vector<LogicSet>::iterator it=input.lSets.begin();it!=input.lSets.end(); ++it)
        for (vector<DOT>::iterator dot=(*it).dots.begin();dot!=(*it).dots.end(); ++dot)
        {
            while (rankList.size() < (*dot).rank + 1)
                rankList.push_back(0);
            rankList[(*dot).rank]++;
        }
        return rankList;
}

LAYER compressLayer(LAYER input, int layerNumber, ostringstream &file_out)
{
    LAYER genLayer;
    vector<int> rankList = generateRankList(input);
    
    int lutNumber = 0;
    
    int bestGPC = gpcList.size();
    int selectedRank = rankList.size();
    do
    {
        bestGPC = gpcList.size();
        int tallest = findTallestColumn(rankList);
        for (int i=0;i<3;i++)
        {
            for(int gpcIndex=0;gpcIndex<gpcList.size();gpcIndex++)
            {
                
                if (tallest - i >= 0)
                {
                    
                    if (testPattern(rankList, gpcList[gpcIndex], tallest-i))
                    {
                        if (gpcIndex<bestGPC)
                        {
                            bestGPC = gpcIndex;
                            selectedRank = tallest-i;
                            break;
                        }
                    }
                } else
                    break;
            }
        }
        if (bestGPC < gpcList.size())
        {
            generateLUT(input, rankList, gpcList[bestGPC], selectedRank, genLayer, layerNumber, lutNumber, file_out);
            
            lutNumber++;
        }
         else
            connectDotsDirectly(input, rankList, tallest, genLayer);
    }while(rankList[findTallestColumn(rankList)]>0);
    
    return genLayer;
}

void connectDotsDirectly(LAYER &input, vector<int> &rankList, int selectedRank, LAYER &genLayer)
{
            for (int ls=0;ls<input.lSets.size(); ++ls)
            {
                for (int dot=0; dot<input.lSets[ls].dots.size(); ++dot)
                {
                    if (input.lSets[ls].dots[dot].rank == selectedRank)
                    {
                        DOT tmp_dot;
                        LogicSet lSet;
                        tmp_dot.rank = selectedRank;
                        tmp_dot.name = input.lSets[ls].dots[dot].name;
                        lSet.dots.push_back(tmp_dot);
                        genLayer.lSets.push_back(lSet);
                        input.lSets[ls].dots.erase(input.lSets[ls].dots.begin() + dot);
                        rankList[selectedRank]--;
                        break;
                    }
                }
                
            }
}

void generateLUT(LAYER &input, vector<int> &rankList, GPC targetGPC, int selectedRank, LAYER &genLayer, int layerNumber, int lutNumber, ostringstream &file_out)
{
    file_out << "wire [" << patch::to_string(targetGPC.x - 1) << ":0] gpcOutL" << patch::to_string(layerNumber) << "_" << patch::to_string(lutNumber) << ";" << endl << endl;
    file_out << "// (* RLOC = " << "\"X" << patch::to_string(layerNumber) << "Y" << patch::to_string(lutNumber) << "\" *)" << endl;
    file_out << "gpc" << patch::to_string(targetGPC.a) << patch::to_string(targetGPC.b) << patch::to_string(targetGPC.c);
    file_out << " gpcL" << patch::to_string(layerNumber) << "_" << patch::to_string(lutNumber) << " ({";
    
    LUT genLUT;
    genLUT.uniqueName = "gpcL" + patch::to_string(layerNumber) + "_" + patch::to_string(lutNumber);
    string sep = "";
    for (int rankIndex = 2; rankIndex>=0; rankIndex--)
    {
        int numberOfDots2Connect = targetGPC.a;
        if (rankIndex == 1)
            numberOfDots2Connect = targetGPC.b;
        else if (rankIndex == 0)
            numberOfDots2Connect = targetGPC.c;

        while(numberOfDots2Connect>0)
        {
            
            for (int ls=0;ls<input.lSets.size(); ++ls)
            {
                for (int dot=0; dot<input.lSets[ls].dots.size(); ++dot)
                {
                    if (input.lSets[ls].dots[dot].rank == selectedRank+rankIndex)
                    {
                        file_out << sep << input.lSets[ls].dots[dot].name;
                        sep = ",";
                        
                        genLUT.inputDots.push_back(input.lSets[ls].dots[dot]);
                        layers[layerNumber].lSets[ls].dots[dot].inputLutName = genLUT.uniqueName;
                        input.lSets[ls].dots.erase(input.lSets[ls].dots.begin() + dot);
                        rankList[selectedRank+rankIndex]--;
                        numberOfDots2Connect--;
                        break;
                    }
                }
                
                if (numberOfDots2Connect==0)
                    break;
                
            }
        }
    }
    file_out << "},gpcOutL" << patch::to_string(layerNumber) << "_" << patch::to_string(lutNumber) << ");" << endl;
    
    LogicSet lSet;
    for (int i=0; i<targetGPC.x; i++)
    {
        DOT tmp_dot;
        tmp_dot.rank = selectedRank + i;
        tmp_dot.name = "gpcOutL" + patch::to_string(layerNumber) + "_" + patch::to_string(lutNumber) + "[" + patch::to_string(i) + "]";
        
        lSet.dots.push_back(tmp_dot);
        genLUT.outputDots.push_back(tmp_dot);
    }
    genLayer.lSets.push_back(lSet);
    genLayer.luts.push_back(genLUT);
}

bool testPattern(vector<int> &rankList, GPC targetGPC, int targetRank)
{
    if (rankList[targetRank]>=targetGPC.c)
    {
        if (targetGPC.b > 0)
        {
            if (rankList.size() > targetRank + 1)
            {
                if (rankList[targetRank+1]>=targetGPC.b)
                {
                    if (targetGPC.a > 0)
                    {
                        if (rankList.size() > targetRank + 2)
                        {
                            if (rankList[targetRank+2]>=targetGPC.a)
                            {
                                return true;
                            }
                        }
                    } else
                        return true;
                }
            }
        } else
            return true;
    }
    return false;
}

int findTallestColumnSize(vector<int> colList)
{
    int tallest = colList[0];
    int tallestIndex = 0;
    for (int i = 1; i<colList.size(); i++)
    {
        if (colList[i] > tallest)
        {
            tallestIndex = i;
            tallest = colList[i];
        }
    }
    
    return tallest;
}

int findTallestColumn(vector<int> colList)
{
    int tallest = colList[0];
    int tallestIndex = 0;
    for (int i = 1; i<colList.size(); i++)
    {
        if (colList[i] > tallest)
        {
            tallestIndex = i;
            tallest = colList[i];
        }
    }
    
    return tallestIndex;
}

void printLayers(vector<LAYER> &layerList)
{
    cout << endl;
    int lcount = 0;
    for (vector<LAYER>::iterator it=layerList.begin();it!=layerList.end(); ++it)
    {
        cout << "LAYER - " << lcount++ << endl;
        for (vector<LogicSet>::iterator ls=(*it).lSets.begin();ls!=(*it).lSets.end(); ++ls)
        {
            for (vector<DOT>::iterator dot=(*ls).dots.begin();dot!=(*ls).dots.end(); ++dot)
                cout << (*dot).name << " ";
            cout << " - rank : " << (*ls).dots[0].rank << endl;
        }
     }   
    cout << endl;
}

// Prints layer
void printList(vector<int> prList)
{
    cout << endl;
    
    for (vector<int>::iterator it=prList.begin();it!=prList.end(); ++it)
        cout << *it << " ";
        
    cout << endl;
}

void printGPCs(vector<GPC> &gpcList)
{
    cout << endl;
    
    cout.precision(2);
    
    for (vector<GPC>::iterator it=gpcList.begin();it!=gpcList.end(); ++it)
        cout << "GPC(" << (*it).a << "," << (*it).b << "," << (*it).c << ";" << (*it).x << ")" << " ratio:" << fixed << (*it).ratio << " covering GPC: " << (*it).coveringGPC << " " << endl;
        
    cout << endl;
}


