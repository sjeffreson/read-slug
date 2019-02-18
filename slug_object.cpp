#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <slug.H>
#include <slug_cluster.H>
#include <slug_predefined.H>
#include <vector>
#include "slug_object.h"

// Method to reconstruct the slug_cluster object from a serialized buffer
void slug_object::reconstruct_cluster(char *buf) {
  cluster = new slug_cluster((slug_cluster_buffer *) buf,
                             slug_predef.imf("chabrier", 9.0, POISSON),
                             slug_predef.tracks("modp020.dat"),
                             slug_predef.specsyn("sb99",
                                                 slug_predef.tracks("modp020.dat"),
                                                 slug_predef.imf("chabrier", 9.0, POISSON)),
                             slug_predef.filter_set("QH0"), 
                             nullptr, nullptr, 
                             slug_predef.yields("SNII_Sukhbold16_nodecay"),
                             nullptr,
                             slug_predef.ostreams, nullptr, true);

}

// Method to return the member functions of the slug_cluster
int slug_object::get_stoch_sn() { return cluster->get_stoch_sn(); }
double slug_object::get_age() { return cluster->get_age(); }
double slug_object::get_birth_mass() { return cluster->get_birth_mass(); }
double slug_object::get_stellar_mass() { return cluster->get_stellar_mass(); }
double slug_object::get_photometry_QH0() { return cluster->get_photometry()[0]; }
double slug_object::get_metalmass() { // sum all yields and subtract H and He
    double metalmass = 0.;
    const std::vector<double> yields = cluster->get_yields();
    double yield_H1 = cluster->get_yields()[cluster->isotope_index(1,1)];
    double yield_H2 = cluster->get_yields()[cluster->isotope_index(1,2)];
    double yield_He3 = cluster->get_yields()[cluster->isotope_index(2,3)];
    double yield_He4 = cluster->get_yields()[cluster->isotope_index(2,4)];
    for (std::vector<double>::size_type i=0; i<yields.size(); i++) {
        metalmass += yields[i];
    }
    metalmass = metalmass - yield_H1 - yield_H2 - yield_He3 - yield_He4;
    assert(metalmass >= 0);
    return metalmass;
}
double slug_object::get_ironyield() { return cluster->get_yields()[cluster->isotope_index(25,56)]; }

int slug_object::buffer_size() { return cluster->buffer_size(); }
void slug_object::print_buffer(char* buffer, int buffer_size) {
    for(int i = 0; i < buffer_size; i++)
        printf("0x%02X ",static_cast<unsigned char>(buffer[i]));
    printf("\n");
}

extern "C" {
    slug_object *slug_object_new() { return new slug_object(); }
    void slug_object_delete(slug_object *S) { delete S; } 
    void slug_reconstruct_cluster(slug_object *S, char *buf) { S->reconstruct_cluster(buf); }
    int slug_get_stoch_sn(slug_object *S, char *buf) {
        S->reconstruct_cluster(buf);
        return S->get_stoch_sn();
    }
    double slug_get_age(slug_object *S, char *buf) {
        S->reconstruct_cluster(buf);
        return S->get_age();
    }
    double slug_get_birth_mass(slug_object *S, char *buf) {
        S->reconstruct_cluster(buf);
        return S->get_birth_mass();
    }
    double slug_get_stellar_mass(slug_object *S, char *buf) {
        S->reconstruct_cluster(buf);
        return S->get_stellar_mass();
    }
    double slug_get_photometry_QH0(slug_object *S, char *buf) {
        S->reconstruct_cluster(buf);
        return S->get_photometry_QH0();
    }
    double slug_get_metalmass(slug_object *S, char *buf) {
        S->reconstruct_cluster(buf);
        return S->get_metalmass();
    }
    double slug_get_ironyield(slug_object *S, char *buf) {
        S->reconstruct_cluster(buf);
        return S->get_ironyield();
    }

    int slug_buffer_size(slug_object *S) {return S->buffer_size();}
    void slug_print_buffer(slug_object *S, char* buffer, int buffer_size) { S->print_buffer(buffer, buffer_size); }
}   