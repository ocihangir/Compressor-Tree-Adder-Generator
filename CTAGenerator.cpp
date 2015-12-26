#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>
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
};

struct LogicSet
{
    vector<DOT> dots;
};

struct LAYER
{
    vector<LogicSet> lSets;
};

struct LUT
{
    vector<DOT> inputDots;
    vector<DOT> outputDots;
    int typeGPC;
};

const int multA = 12;
const int multB = 12;

const int M = 6;
const int N = 4;
const int k = 2;


void printList(vector<int> prList);
LAYER generatePartialProducts(int,int);
int findTallestColumn(vector<int> colList);
LAYER compressLayer(LAYER input, int layerNumber);
vector<GPC> generateCoveringGPCs(int M, int N);
vector<GPC> generatePrimitiveGPCs(vector<GPC> &covGPCs);
vector<GPC> generateGPCs(int M, int N);
void printGPCs(vector<GPC> &gpcList);
void sortGPCs(vector<GPC> &listGPC);
void printLayers(vector<LAYER> &layerList);
bool testPattern(vector<int> &rankList, GPC targetGPC, int targetRank);
void generateLUT(LAYER &input, vector<int> &rankList, GPC targetGPC, int selectedRank, LAYER &genLayer, int layerNumber, int lutNumber);
void connectDotsDirectly(LAYER &input, vector<int> &rankList, int selectedRank, LAYER &genLayer);
vector<int> generateRankList(LAYER input);

vector<GPC> gpcList;
vector<LAYER> layers;

int main( int argc, char *argv[] )
{
    // Create GPC tabel
    gpcList = generateGPCs(M, N);
    printGPCs(gpcList);
    
    // Generate partial products as Layer0
    LAYER partialProducts = generatePartialProducts(multA,multB);
    layers.push_back(partialProducts);
    
    int layerNumber = 1;
    // Generate layers
    while(findTallestColumn(generateRankList(layers.back()))>k)
    {
        LAYER res = compressLayer(layers.back(), layerNumber++);
        layers.push_back(res);
    }
    printLayers(layers);
    
    printList(generateRankList(layers.back()));
    
}

vector<GPC> generateGPCs(int M, int N)
{
    vector<GPC> covGPCs = generateCoveringGPCs(M, N);
    
    cout << "Covering GPC List : " << endl;
    printGPCs(covGPCs);
    
    vector<GPC> primGPCs = generatePrimitiveGPCs(covGPCs);
    
    cout << "Primitive GPC List : " << endl;
    
    
    // Eliminate repeats
    for (int i=0;i<primGPCs.size()-1;i++)
    {
        for (int j=i+1;j<primGPCs.size();j++)
            if (primGPCs[i] == primGPCs[j])
                primGPCs.erase(primGPCs.begin() + j);
    }
    
    return primGPCs;
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
                    if (q < 8)
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
    
    return primGPCs;
}

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

// Generates partial products
LAYER generatePartialProducts(int A, int B)
{
    vector<int> pp;
    
    
    vector<LogicSet> lSets;
    
    for (int i=0; i<A; i++)
    {
        vector<DOT> dots;
        
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

LAYER compressLayer(LAYER input, int layerNumber)
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
            generateLUT(input, rankList, gpcList[bestGPC], selectedRank, genLayer, layerNumber, lutNumber);
            
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

void generateLUT(LAYER &input, vector<int> &rankList, GPC targetGPC, int selectedRank, LAYER &genLayer, int layerNumber, int lutNumber)
{
    cout << "(* RLOC = " << "\"X" << layerNumber << "Y" << lutNumber << "\" *)" << endl;
    cout << "gpc" << targetGPC.a << targetGPC.b << targetGPC.c;
    cout << " gpcL" << layerNumber << "_" << lutNumber << " ({";
    
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
                        cout << input.lSets[ls].dots[dot].name;
                        cout << ",";
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
    cout << "},gpcOutL" << patch::to_string(layerNumber) << "_" << patch::to_string(lutNumber) << ");" << endl;
    
    LogicSet lSet;
    for (int i=0; i<targetGPC.x; i++)
    {
        DOT tmp_dot;
        tmp_dot.rank = selectedRank + i;
        tmp_dot.name = "gpcOutL" + patch::to_string(layerNumber) + "_" + patch::to_string(lutNumber) + "[" + patch::to_string(i) + "]";
        
        lSet.dots.push_back(tmp_dot);
    }
    genLayer.lSets.push_back(lSet);
    
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


