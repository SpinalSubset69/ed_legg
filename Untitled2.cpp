#include <iostream> 
#include <vector>
#include <algorithm>
#include <ctime> 
#include <cstring> 
#include <cmath> 
#include <climits> 
using namespace std;


/** Parameters **/
int TOTAL_POPULATION = 500; 
double MUTATION_PROBABILITY = 0.07;
int NUMBER_OF_GENERATIONS = 1000;
int ELITE = 1; // the best ones which always pass to the next generation


struct Point{
    double x, y;
    Point(){}
    Point(double _x, double _y): x(_x), y(_y) {}
    double dist(Point p){
        return hypot(p.x - x, p.y - y );
    }
};

struct Population{

    private:
    
    vector<Point> &points;
    vector<vector<int> > routes;
    vector<double> prob;
    int n; // size of the route
    
    public:
    /** Freeing memory **/
    ~Population(){
        routes.clear();
        prob.clear();
    }
    
    /** Constructors **/
    Population(vector<Point> &_points): points(_points){}  
    Population(int routeSize, vector<Point> &_points): n(routeSize), points(_points){}  
    Population(int popuSize, int routeSize, vector<Point> &_points): points(_points) {
        
        n = routeSize;
        routes.clear();
        vector<int> ind; // indexes to shuffle
        for(int i = 0; i < n; i++) ind.push_back(i);
        
        // random init
        for(int i = 0; i < popuSize; i++){
            random_shuffle ( ind.begin(), ind.end() );
            vector<int> newRoute = ind;
            routes.push_back( newRoute );
        }
    }
    
    vector<vector<int> > getRoutes() const{
        return routes;
    }
    
    int getRouteSize() const{
        return n;
    }
    
    vector<double> getProb() const{
        return prob;
    }
    
    void operator=(const Population& pob){
        routes = pob.getRoutes();
        n = pob.getRouteSize();
        prob = pob.getProb();
    }
    
    // OX1 type crossover
    void crossover(vector<int> &p1, vector<int> &p2,
        vector<int> &h1, vector<int> &h2
        ){
        h1.clear();
        h2.clear();
        int a = rand()%(n-1), b = rand()%(n-1) ;
        if( a > b ) swap(a,b);
        
        bool oc1[n];
        bool oc2[n];
        memset(oc1, 0, sizeof(oc1));
        memset(oc2, 0, sizeof(oc2));
        
        for(int i = 0; i < n; i++){
            h1.push_back( -1 );
            h2.push_back( -1 );
        }
        
        for(int i = a+1; i <= b; i++){
            h1[i] = p1[i];
            oc1[ p1[i] ] = 1;
            h2[i] = p2[i];
            oc2[ p2[i] ] = 1;
        }        
        int i1 = (b+1)%n, i2 = (b+1)%n;
        for(int i = 0; i < n; i++){
            if( !oc1[ p2[ (b+1+i)%n ] ] ){
                h1[i1] = p2[ (b+1+i)%n ];
                i1 = (i1+1)%n;
            }
            if( !oc2[ p1[ (b+1+i)%n ] ] ){
                h2[i2] = p1[ (b+1+i)%n ];
                i2 = (i2+1)%n;
            }
        }
        /*
        cout << "indices: " << a << " " << b<<endl;
        for(int i = 0; i < n; i++){
            cout << h1[i] << "\t" << h2[i] << "\tp:\t"<< p1[i] <<"\t"<< p2[i]<<endl;
        }*/
        
        /** mutation **/
        double r;
        r = (double)rand()/(double)RAND_MAX;
        if( r <= MUTATION_PROBABILITY ){
            a = rand()%(n);
            b = rand()%(n);
            swap(h1[a], h1[b]);
        }
        
        r = (double)rand()/(double)RAND_MAX;
        if( r <= MUTATION_PROBABILITY ){            
            a = rand()%(n);
            b = rand()%(n);
            swap(h2[a], h2[b]);
        }
    }
    
    Population evolve(){
        Population pSiguiente = Population(n, points);
        order();
        int cant;
        for(cant = 0; cant < ELITE && cant < (int)routes.size(); cant++){
            vector<int> nRuta = routes[cant];
            pSiguiente.addRoute( nRuta );
        }
        
        /** Fitness proportionate selection or roulette-wheel selection **/
        for(; ELITE < (int)routes.size() ;){
            vector<int> h1,h2;
            double r;
            
            r = (double)rand()/(double)RAND_MAX;
            int ind1 = lower_bound( prob.begin(), prob.end(), r) - prob.begin();
            
            int ind2 = ind1;
            while( ind2 == ind1){
                r = (double)rand()/(double)RAND_MAX;
                ind2 = lower_bound( prob.begin(), prob.end(), r) - prob.begin();
            }
            
            //cout <<"a cruzar: " << ind1<<" " << ind2 << endl;
            
            crossover( routes[ind1], routes[ind2], h1, h2 );
            pSiguiente.addRoute( h1 );
            if( pSiguiente.getSize() == (int)routes.size() ) break;
            pSiguiente.addRoute( h2 );
            if( pSiguiente.getSize() == (int)routes.size() ) break;
        }
        return pSiguiente;
    }
    
    /**  This is the total distance of the i-th chromosome **/
    double getNoFitness(int i){
        if( i >= 0 && i < routes.size() ){
            double noFitness = 0;
            for(int j = 0; j < n; j++){
                noFitness += points[routes[i][j]].dist( points[routes[i][(j+1)%n] ] )  ;
            }
            return noFitness;
        }else return -1.0;
    }
    
    void order(){
        vector<pair<double,int> > ind;
        double totalFitness = 0;
        for(int i = 0; i < (int)routes.size(); i++){
            double noFitness = getNoFitness(i);
            double fitness = 1.0/(double)noFitness;
            totalFitness += fitness;
            ind.push_back( make_pair(-fitness, i) );
        }
        sort( ind.begin(), ind.end() );
        vector<vector<int> > rutas2;
        double ac = 0;
        prob.clear();  
        for(int i = 0; i < (int)routes.size(); i++){
            rutas2.push_back(routes[ ind[i].second ]);
            ac+= -ind[i].first;
            prob.push_back( ac/totalFitness );
        }        
        routes = rutas2;          
        
    }
    
    void addRoute(vector<int> &nRuta){
        routes.push_back(nRuta);
    }
    
    int getSize(){
        return (int)routes.size();
    }
    
    /** You have to order before calling to this method **/
    vector<Point> getFirst(){
        vector<Point> res;
        for(int i = 0; i < n; i++) res.push_back( points[ routes[0][i] ]);
        return res;
    }
    
    vector<int> getFirstIndexes(){
        return routes[0];
    }
    
    Point getPoint(int i){
        return points[i];
    }
    
};

/** Genetic algorithm **/
vector<Point> ga_tsp(vector<Point> &points){
    int n = (int)points.size();    
    
    Population pActual (TOTAL_POPULATION, n, points);
     
    pActual.order();   
    vector<Point> result = pActual.getFirst(); 
    vector<int> rIndexes = pActual.getFirstIndexes();
       
    cout << "Route/cycle more with more fitness on initial population with distance:"<< pActual.getNoFitness(0)<< endl;
    for(int i = 0; i < result.size(); i++) {
        cout<< rIndexes[i] << " : "<< result[i].x << " " << result[i].y << endl;
    }
    
    cout << "\nGeneration" << "\t"<<"distance found" << endl;
    
    for(int iter = 0; iter < NUMBER_OF_GENERATIONS; iter++){
        Population pNext = pActual.evolve();  
            
        pActual = pNext;
        
        pActual.order();
        if(iter%100 == 0 || iter == NUMBER_OF_GENERATIONS - 1) cout <<iter <<"\t"<< pActual.getNoFitness(0) << endl;
    }
    pActual.order();    
    return pActual.getFirst();
}

int main(){

    ios_base::sync_with_stdio(true);
    
    srand ( unsigned ( time(0) ) );

    vector<Point> points;

    /** Reading input **/
    double x, y;
    while( cin >>x >> y ){ 
        points.push_back( Point(x,y) );
    }
    vector<Point> result = ga_tsp( points );
    
    cout << "\nRoute/cycle more with more fitness found:"<< endl;
    for(int i = 0; i < result.size(); i++) {
        cout<< result[i].x << " " << result[i].y << endl;
    }
}
