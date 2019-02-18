# Description
This is a small python class for reconstructing and reading information from the SLUG object buffers contained in Arepo snapshots, by interfacing with the SLUG C++ code

## Note
Not tested, and no error outputs yet, sorry!

# Usage
## Compiling
Run the following commands in the ```read_slug``` directory
```
g++ -std=c++11 -c -fPIC slug_object.cpp -o slug_object.o -I$PATH_TO_SLUG/slug2/src -I$PATH_TO_BOOST/boost_1_67_0 -I/usr/include -L$PATH_TO_SLUG/slug2 -L$PATH_TO_BOOST/boost_1_67_0/stage/lib -L/usr/lib64 -lslug -lboost_system -lboost_filesystem -lboost_regex
```

followed by
```
g++ -std=c++11 -shared -Wl,-soname=libslug_object.so -o libslug_object.so slug_object.o -I$PATH_TO_SLUG/slug2/src -I$PATH_TO_BOOST/boost_1_67_0 -I/usr/include -L$PATH_TO_SLUG/slug2 -L$PATH_TO_BOOST/boost_1_67_0/stage/lib -L/usr/lib64 -lslug -lboost_system -lboost_filesystem -lboost_regex
```

## Running the python script
Choose a snapshot that has star particles with slug objects attached (Arepo output) and run
```
python2.7 read_slug.py "PATH_TO_SNAPSHOT/snap_x.hdf5"
```
and the script will output a dictionary of the form
```
slug_dict = {
	'PID': [star/cluster PIDs],
	'stoch_sn': [total number of SNe from clusters up to this time],
	'age': [age of clusters (years)],
	'birth_mass': [mass of clusters at birth (solar masses)],
	'stellar_mass': [current mass of clusters (solar masses)],
	'photometry_QH0': [number of ionising photons currently output by clusters (per second)],
	'metalmass': [total mass of metals output by clusters up to this time]
}
```

You can also run results for individual clusters, if necessary, using the iPython notebook read_slug.ipynb
