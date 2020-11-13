#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emscripten.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/polygon_mesh_processing.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <iostream>

template <class Poly>
typename Poly::Halfedge_handle make_cube_3( Poly& P) {
    // appends a cube of size [0,1]^3 to the polyhedron P.
    CGAL_precondition( P.is_valid());
    typedef typename Poly::Point_3         Point;
    typedef typename Poly::Halfedge_handle Halfedge_handle;
    Halfedge_handle h = P.make_tetrahedron( Point( 1, 0, 0),
                                            Point( 0, 0, 1),
                                            Point( 0, 0, 0),
                                            Point( 0, 1, 0));
    Halfedge_handle g = h->next()->opposite()->next();             // Fig. (a)
    P.split_edge( h->next());
    P.split_edge( g->next());
    P.split_edge( g);                                              // Fig. (b)
    h->next()->vertex()->point()     = Point( 1, 0, 1);
    g->next()->vertex()->point()     = Point( 0, 1, 1);
    g->opposite()->vertex()->point() = Point( 1, 1, 0);            // Fig. (c)
    Halfedge_handle f = P.split_facet( g->next(),
                                       g->next()->next()->next()); // Fig. (d)
    Halfedge_handle e = P.split_edge( f);
    e->vertex()->point() = Point( 1, 1, 1);                        // Fig. (e)
    P.split_facet( e, f->next()->next());                          // Fig. (f)
    CGAL_postcondition( P.is_valid());
    return h;
}


typedef CGAL::Simple_cartesian<double>     Kernel;
typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
typedef Kernel::Point_3                    Point_3;
typedef Polyhedron::Halfedge_handle        Halfedge_handle;
typedef Polyhedron::Vertex_iterator        Vertex_iterator;
typedef Polyhedron::Facet_iterator                   Facet_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;
typedef Kernel::Vector_3  Vector_3;
typedef Kernel::Aff_transformation_3  Aff_transformation_3;



float displayBuffer[1000];

Nef_polyhedron translate(Nef_polyhedron shape, int x, int y, int z){
    
    Aff_transformation_3 aff(CGAL::TRANSLATION, Vector_3(x, y, z));
    shape.transform(aff);
    
    return shape;
}

extern "C"
{

int displayShape(){
    
    Polyhedron P;
    Halfedge_handle h = make_cube_3( P);
    
    Nef_polyhedron N1(P);
    Nef_polyhedron initial(P);
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                printf("i:  %i\n", i);
                Nef_polyhedron N2(initial);
                N2 = translate(N2,2*i,2*j,2*k);
                N1 += N2;
            }
        }
    }
    
    Polyhedron P2;
    N1.convert_to_polyhedron(P2);
    
    
    printf("Facets before: %lu\n", P2.size_of_facets());
    
    CGAL::Polygon_mesh_processing::triangulate_faces(P2);
    
    printf("Facets after: %lu\n", P2.size_of_facets());
    
    int index = 0;
    
    for (  Facet_iterator i = P2.facets_begin(); i != P2.facets_end(); ++i) {
        
        Halfedge_facet_circulator j = i->facet_begin();
        
        Vertex_iterator v = j->vertex();
        
        displayBuffer[index] = v->point()[0];
        displayBuffer[index+1] = v->point()[1];
        displayBuffer[index+2] = v->point()[2];
        
        j++;
        
        Vertex_iterator w = j->vertex();
        
        displayBuffer[index+3] = w->point()[0];
        displayBuffer[index+4] = w->point()[1];
        displayBuffer[index+5] = w->point()[2];
        
        j++;
        
        Vertex_iterator x = j->vertex();
        
        displayBuffer[index+6] = x->point()[0];
        displayBuffer[index+7] = x->point()[1];
        displayBuffer[index+8] = x->point()[2];
        
        index = index + 9;
    }
    
    return index;
}

float * getDisplayBufferAddress(){
    return displayBuffer;
}

}


// Init circle data and start render - JS
int main(){

    
    printf("Loading...\n");
    
    EM_ASM({ init(); });
    
    
    return 0;
}
