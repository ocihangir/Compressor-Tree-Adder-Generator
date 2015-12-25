#include <iostream>
#include <vector>
#include <math.h>

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
};

struct DOT
{
    int rank;
};

struct LogicSet
{
    vector<DOT> dots;
};

struct LAYER
{
    vector<LogicSet> lSets;
};



const int multA = 12;
const int multB = 12;

const int M = 6;
const int N = 4;
const int k = 3;


void printList(vector<int> &prList);
LAYER generatePartialProducts(int,int);
int findTallestColumn(vector<int> &colList);
vector<int> generateLayer(vector<int> &input);
vector<GPC> generateCoveringGPCs(int M, int N);
vector<GPC> generatePrimitiveGPCs(vector<GPC> &covGPCs);
void printGPCs(vector<GPC> &gpcList);
void sortGPCs(vector<GPC> &listGPC);
void printLayers(vector<LAYER> &layerList);

vector<GPC> gpcList;
vector<LAYER> layers;

int main( int argc, char *argv[] )
{
    // *** GPC creation
    vector<GPC> covGPCs = generateCoveringGPCs(M, N);
    
    cout << "Covering GPC List : " << endl;
    printGPCs(covGPCs);
    
    vector<GPC> primGPCs = generatePrimitiveGPCs(covGPCs);
    
    cout << "Primitive GPC List : " << endl;
    printGPCs(primGPCs);
    // ***
    
    
    // Generate partial products as Layer0
    LAYER partialProducts = generatePartialProducts(multA,multB);
    layers.push_back(partialProducts);
    
    printLayers(layers);
        
    // printList(partialProducts);
    
    // generateLayer(partialProducts);
    
    
    
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

vector<int> generateLayer(vector<int> &input)
{
    vector<int> genLayer(input.size() + N);
    
    int tallest = findTallestColumn(input);
    
    cout << "Tallest column : " << tallest << endl;
    
    return genLayer;
}

int findTallestColumn(vector<int> &colList)
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
    
    for (vector<LAYER>::iterator it=layerList.begin();it!=layerList.end(); ++it)
    {
        cout << "LAYER - " << endl;
        for (vector<LogicSet>::iterator ls=(*it).lSets.begin();ls!=(*it).lSets.end(); ++ls)
        {
            for (vector<DOT>::iterator dot=(*ls).dots.begin();dot!=(*ls).dots.end(); ++dot)
                cout << (*dot).rank << " ";
            cout << endl;
        }
     }   
    cout << endl;
}

// Prints layer
void printList(vector<int> &prList)
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


