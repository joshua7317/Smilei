#include "DensityProfile2D.h"

using namespace std;

double DensityProfile2D::operator() (PicParams* params, unsigned int ispec, vector<double> x_cell) {
    double fx, fy;
    
    SpeciesStructure par=params->species_param[ispec];

    // Constant density profile
    // ------------------------
    if (par.plasma_geometry=="constant") {
        // x-direction
        if (   (x_cell[0]>par.vacuum_length[0])
            && (x_cell[0]<par.vacuum_length[0]+par.plasma_length[0]) ) {
            fx = 1.0;
        } else {
            fx = 0.0;
        }
        // y-direction
        if (   (x_cell[1]>par.vacuum_length[1])
            && (x_cell[1]<par.vacuum_length[1]+par.plasma_length[1]) ) {
            fy = 1.0;
        } else {
            fy = 0.0;
        }
        // x-y direction
        return fx*fy;
    }
    
    // Trapezoidal density profile
    // ---------------------------
    else if (par.plasma_geometry=="trap") {
        
        // x-direction
        
        // vacuum region
        if ( x_cell[0] < par.vacuum_length[0] ) {
            fx = 0.0;
        }
        // linearly increasing density
        else if ( x_cell[0] < par.vacuum_length[0]+par.slope_length[0] ) {
            fx = (x_cell[0]-par.vacuum_length[0]) / par.slope_length[0];
        }
        // density plateau
        else if ( x_cell[0] < par.vacuum_length[0]+par.plasma_length[0]-par.slope_length[0] ) {
            fx = 1.0;
        }
        // linearly decreasing density
        else if ( x_cell[0] < par.vacuum_length[0]+par.plasma_length[0] ) {
            fx = 1.0 - ( x_cell[0] - (par.vacuum_length[0]+par.plasma_length[0]-par.slope_length[0]) )
            /            par.slope_length[0];
        }
        // beyond the plasma
        else {
            fx = 0.0;
        }
        
        // y-direction
        
        // vacuum region
        if ( x_cell[1] < par.vacuum_length[1] ) {
            fy = 0.0;
        }
        // linearly increasing density
        else if ( x_cell[1] < par.vacuum_length[1]+par.slope_length[1] ) {
            fy = (x_cell[1]-par.vacuum_length[1]) / par.slope_length[1];
        }
        // density plateau
        else if ( x_cell[1] < par.vacuum_length[1]+par.plasma_length[1]-par.slope_length[1] ) {
            fy = 1.0;
        }
        // linearly decreasing density
        else if ( x_cell[1] < par.vacuum_length[1]+par.plasma_length[1] ) {
            fy = 1.0 - ( x_cell[1] - (par.vacuum_length[1]+par.plasma_length[1]-par.slope_length[1]) )
            /            par.slope_length[1];
        }
        // beyond the plasma
        else {
            fy = 0.0;
        }
        
        // x-y directions
        return fx*fy;
    }
    
    //2D gaussian profile
    else if (par.plasma_geometry=="gaussian") {
        vector<double> lt(2);
        lt[0]=par.cut[0]*par.sigma[0];
        lt[1]=par.cut[1]*par.sigma[1];
        //x direction
        // vacuum region
        if ( x_cell[0] < par.vacuum_length[0] ) {
            fx=0.0;
        }
        //gaussian
        else if(x_cell[0] < par.vacuum_length[0]+lt[0] ) {
            fx= exp(-(x_cell[0]-par.vacuum_length[0]-lt[0])*(x_cell[0]-par.vacuum_length[0]-lt[0])/(2*par.sigma[0]*par.sigma[0]));
            
        }
        else if(x_cell[0] < par.vacuum_length[0]+lt[0]+par.plateau[0]) {
            fx= 1.0;
        }
        else if(x_cell[0] < par.vacuum_length[0]+ 2*lt[0]+par.plateau[0]) {
            fx = exp(-(x_cell[0]-par.vacuum_length[0]-lt[0]-par.plateau[0])*(x_cell[0]-par.vacuum_length[0]-lt[0]-par.plateau[0])/(2*par.sigma[0]*par.sigma[0]));
        }
        
        else{
            fx= 0.0;
        }
        
        
        //y direction
        // vacuum region
        if ( x_cell[1] < par.vacuum_length[1] ) {
            fy=0.0;
        }
        //gaussian
        else if(x_cell[1] < par.vacuum_length[1]+lt[1] ) {
            fy= exp(-(x_cell[1]-par.vacuum_length[1]-lt[1])*(x_cell[1]-par.vacuum_length[1]-lt[1])/(2*par.sigma[1]*par.sigma[1]));
            
        }
        else if(x_cell[1] < par.vacuum_length[1]+lt[1]+par.plateau[1]) {
            fy= 1.0;
        }
        else if(x_cell[1] < par.vacuum_length[1]+ 2*lt[1]+par.plateau[1]) {
            fy= exp(-(x_cell[1]-par.vacuum_length[1]-lt[1]-par.plateau[1])*(x_cell[1]-par.vacuum_length[1]-lt[1]-par.plateau[1])/(2*par.sigma[1]*par.sigma[1]));
        }
        
        else{
            fy= 0.0;
        }
        return fx*fy;
        
    }
    
    // Plasma crossing along x direction density profiles
    // --------------------------------------------------
    else if (par.plasma_geometry=="crossx") {
        
        // FIRST CONSIDER SPECIES 0 & 1
        if (ispec<2) {
            // x-direction
            if (   (x_cell[0]>par.vacuum_length[0])
                && (x_cell[0]<par.vacuum_length[0]+par.plasma_length[0]) ) {
                fx = 1.0;
            } else {
                fx = 0.0;
            }
            // y-direction
            if (   (x_cell[1]>par.vacuum_length[1])
                && (x_cell[1]<par.vacuum_length[1]+par.plasma_length[1]) ) {
                fy = 1.0;
            } else {
                fy = 0.0;
                
            }
            // x-y direction
            return fx*fy;
            
            
        }
        // THEN CONSIDER SPECIES 3 & 4
        else if (ispec<4) {
            // x-direction
            if (   (x_cell[0]>par.vacuum_length[0]+par.plasma_length[0])
                && (x_cell[0]<par.vacuum_length[0]+2.0*par.plasma_length[0]) ) {
                fx = 1.0;
            } else {
                fx = 0.0;
            }
            // y-direction
            if (   (x_cell[1]>par.vacuum_length[1])
                && (x_cell[1]<par.vacuum_length[1]+par.plasma_length[1]) ) {
                fy = 1.0;
            } else {
                fy = 0.0;
            }
            // x-y direction
            return fx*fy;
            
        }
    }//end crossx
    
    
    // Plasma crossing along y direction density profiles
    // --------------------------------------------------
    else if (par.plasma_geometry=="crossy") {
        
        // FIRST CONSIDER SPECIES 0 & 1
        if (ispec<2) {
            // x-direction
            if (   (x_cell[0]>par.vacuum_length[0])
                && (x_cell[0]<par.vacuum_length[0]+par.plasma_length[0]) ) {
                fx = 1.0;
            } else {
                fx = 0.0;
            }
            // y-direction
            if (   (x_cell[1]>par.vacuum_length[1])
                && (x_cell[1]<par.vacuum_length[1]+par.plasma_length[1]) ) {
                fy = 1.0;
            } else {
                fy = 0.0;
            }
            // x-y direction
            return fx*fy;
            
            
        }
        // THEN CONSIDER SPECIES 3 & 4
        else if (ispec<4) {
            // x-direction
            if (   (x_cell[0]>par.vacuum_length[0])
                && (x_cell[0]<par.vacuum_length[0]+par.plasma_length[0]) ) {
                fx = 1.0;
            } else {
                fx = 0.0;
            }
            // y-direction
            if (   (x_cell[1]>par.vacuum_length[1]+par.plasma_length[1])
                && (x_cell[1]<par.vacuum_length[1]+2.0*par.plasma_length[1]) ) {
                fy = 1.0;
            } else {
                fy = 0.0;
            }
            // x-y direction
            
            return fx*fy;
        }
    }//end crossx
    
    
    
    
    // Plasma density profile corresponding to Fukuda et al., Phys. Rev. Lett. 103, 165002 (2012)
    // used in simulations for Anna Levy
    // ------------------------------------------------------------------------------------------
    else if (par.plasma_geometry=="fukuda") {
        
        // x-direction
        if (x_cell[0]<2.0*M_PI*2.0) {
            fx = 0.0;
        }
        else if (x_cell[0]<2.0*M_PI*13.0) {
            fx = 0.2;
        }
        else if (x_cell[0]<2.0*M_PI*20.0) {
            fx = 0.2 + 0.8*(x_cell[0]-2.0*M_PI*13.0)/(2.0*M_PI*7.0);
        }
        else if (x_cell[0]<2.0*M_PI*65.0) {
            fx = 1.0;
        }
        else if (x_cell[0]<2.0*M_PI*82.0) {
            fx = 1.0 - 0.8*(x_cell[0]-2.0*M_PI*65.0)/(2.0*M_PI*17.0);
        }
        else if (x_cell[0]<2.0*M_PI*112.0) {
            fx = 0.2;
        }
        else {
            fx = 0.0;
        }
        
        // y-direction: constant density
        fy = 1.0;
        
        // x-y direction
        return fx*fy;
        
    }//end fukuda
    
    
    
    // Other profiles: not defined
    // ---------------------------
    else {
        ERROR("Density profile " << par.plasma_geometry <<" not yet defined in 2D");
        return 0.0;
    }

}
