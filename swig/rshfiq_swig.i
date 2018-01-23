/* -*- c++ -*- */

#define RSHFIQ_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "rshfiq_swig_doc.i"

%{
#include "rshfiq/rshfiq.h"
%}


%include "rshfiq/rshfiq.h"
GR_SWIG_BLOCK_MAGIC2(rshfiq, rshfiq);
