#ifndef slug_object_h
#define slug_object_h
#ifdef __cplusplus
#include <slug.H>
class slug_object {  
public:
  
  // Constructor 
  slug_object() { 
    cluster = nullptr;
  }
  
  // Destructor; this frees the slug_cluster object
  virtual ~slug_object() {
    delete cluster;
    cluster = nullptr;
  }
  
  // Method to construct the slug_cluster object from particle mass
  
  // Method to reconstruct the slug_cluster object from a serialized buffer
  void reconstruct_cluster(char *buf);

  // Method to return the member functions of the slug_cluster
  int get_stoch_sn();
  double get_age();
  double get_birth_mass();
  double get_stellar_mass();
  double get_photometry_QH0(); // ionising luminosity [photon/s]
  double get_metalmass();
  double get_ironyield();
  int buffer_size();
  void print_buffer(char* buffer, int buffer_size);
  
protected:
  
 // This is a pointer to the slug_cluster object
  slug_cluster *cluster;
  
};
#else
typedef struct slug_object slug_object; 
#endif

// File wrapper
#ifdef __cplusplus
extern "C" {
#endif

	slug_object* slug_object_new();
    void slug_object_delete(slug_object *SlugOb);
    void slug_reconstruct_cluster(slug_object *SlugOb, char *buf);
    int slug_get_stoch_sn(slug_object *S, char *buf);
    double slug_get_age(slug_object *S, char *buf);
    double slug_get_birth_mass(slug_object *S, char *buf);
    double slug_get_stellar_mass(slug_object *S, char *buf);
    double slug_get_photometry_QH0(slug_object *S, char *buf);
    double slug_get_metalmass(slug_object *S, char *buf);
    double slug_get_ironyield(slug_object *S, char *buf);

    int slug_buffer_size(slug_object *SlugOb);
    void slug_print_buffer(slug_object *SlugOb, char* buffer, int buffer_size);
	
#ifdef __cplusplus
}
#endif
#endif
