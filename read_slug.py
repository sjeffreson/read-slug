from warnings import warn
from ctypes import *

import h5py
import numpy as np

import os, sys

def loadlib():
	libslug = cdll.LoadLibrary('/home/hd/hd_hd/hd_vm149/read_slug/libslug_object.so')
	if libslug is None:
		raise ImportError("unable to load libslug_object")

	libslug.slug_object_new.argtypes = ()
	libslug.slug_object_new.restype = c_void_p

	libslug.slug_print_buffer.argtypes = c_void_p, c_char_p, c_int
	libslug.slug_print_buffer.restype = c_void_p

	libslug.slug_reconstruct_cluster.argtypes = c_void_p, c_char_p
	libslug.slug_reconstruct_cluster.restype = c_void_p

	libslug.slug_get_stoch_sn.argtypes = c_void_p, c_char_p
	libslug.slug_get_stoch_sn.restype = c_int

	libslug.slug_get_age.argtypes = c_void_p, c_char_p
	libslug.slug_get_age.restype = c_double

	libslug.slug_get_birth_mass.argtypes = c_void_p, c_char_p
	libslug.slug_get_birth_mass.restype = c_double

	libslug.slug_get_stellar_mass.argtypes = c_void_p, c_char_p
	libslug.slug_get_stellar_mass.restype = c_double

	libslug.slug_get_photometry_QH0.argtypes = c_void_p, c_char_p
	libslug.slug_get_photometry_QH0.restype = c_double

	libslug.slug_get_metalmass.argtypes = c_void_p, c_char_p
	libslug.slug_get_metalmass.restype = c_double

	libslug.slug_object_delete.argtypes = c_void_p,
	libslug.slug_object_delete.restype = None

	return libslug

class Slug:
    def __init__(self):
        self.obj = libslug.slug_object_new()

    def __del__(self):
        libslug.slug_object_delete(self.obj)
        
    def print_buffer(self, slug_buffer):
        libslug.slug_print_buffer(self.obj, slug_buffer, len(slug_buffer))
    
    def reconstruct(self, slug_buffer):
        libslug.slug_reconstruct_cluster(self.obj, slug_buffer)

    def stoch_sn(self, slug_buffer):
    	return libslug.slug_get_stoch_sn(self.obj, slug_buffer)

    def age(self, slug_buffer):
    	return libslug.slug_get_age(self.obj, slug_buffer)

    def birth_mass(self, slug_buffer):
    	return libslug.slug_get_birth_mass(self.obj, slug_buffer)

    def stellar_mass(self, slug_buffer):
    	return libslug.slug_get_stellar_mass(self.obj, slug_buffer)

    def photometry_QH0(self, slug_buffer):
    	return libslug.slug_get_photometry_QH0(self.obj, slug_buffer)

    def metalmass(self, slug_buffer):
    	return libslug.slug_get_metalmass(self.obj, slug_buffer)

def buf_from_snap(snap):
	snapdata = h5py.File(snap, 'r')
	stars = snapdata['PartType4']
	slug_buf_list = bytearray(stars['SlugOb'].value[100].tobytes())
	slug_buf = [c_char(chr(slug_buf_list[i])) for i in range(len(slug_buf_list))]
	return (c_char * len(slug_buf))(*slug_buf)

def slug_properties(snap):
	global libslug
	libslug = loadlib()
	s = Slug()

	snapdata = h5py.File(snap, 'r')
	stars = snapdata['PartType4']
	star_slugsizes = stars['SlugSize'].value
	star_PIDs = stars['ParticleIDs'].value[star_slugsizes != 7575]
	star_SlugObs = stars['SlugOb'].value[star_slugsizes != 7575]
	no_stars = len(star_SlugObs)
	print no_stars

	slug_buf_lists = [bytearray(item.tobytes()) for item in star_SlugObs]
	print "step 1"
	slug_bufs = [map(chr, item) for item in slug_buf_lists]
	print "step 2"
	slug_bufs_c = [(c_char * len(item))(*item) for item in slug_bufs]
	print "step 3"

	slug_dict = {
		'PID': [],
		'stoch_sn': [],
		'age': [],
		'birth_mass': [],
		'stellar_mass': [],
		'photometry_QH0': [],
		'metalmass': []
	}
	for star_PID, slug_buf_c in zip(star_PIDs, slug_bufs_c):
		slug_dict['PID'].append(star_PID)
		slug_dict['stoch_sn'].append(s.stoch_sn(slug_buf_c))
		slug_dict['age'].append(s.age(slug_buf_c))
		slug_dict['birth_mass'].append(s.birth_mass(slug_buf_c))
		slug_dict['stellar_mass'].append(s.stellar_mass(slug_buf_c))
		slug_dict['photometry_QH0'].append(s.photometry_QH0(slug_buf_c))
		slug_dict['metalmass'].append(s.metalmass(slug_buf_c))

	print "writing..."
	f = open('snap'+sys.argv[1].rsplit('snap',1)[1].rsplit('.hdf5',1)[0]+'.txt', 'w')
	f.write("# PID stoch_sn age birth_mass stellar_mass photometry_QH0 metalmass\n")
	for i in range(len(slug_dict['PID'])):
		f.write(
			str(slug_dict['PID'][i]) + ' ' +
			str(slug_dict['stoch_sn'][i]) + ' ' +
			str(slug_dict['age'][i]) + ' ' +
			str(slug_dict['birth_mass'][i]) + ' ' +
			str(slug_dict['stellar_mass'][i]) + ' ' +
			str(slug_dict['photometry_QH0'][i]) + ' ' +
			str(slug_dict['metalmass'][i]) + '\n'
		)
	f.close()
	print "finished!"

slug_dict = slug_properties(sys.argv[1])

