//
// Created by Isaac Sy on 9/27/2018 for the JPEG-benchmarker project.
//
#ifndef JPEG_BENCHMARKER_JMEMMGR_H
#define JPEG_BENCHMARKER_JMEMMGR_H

#include "jinclude.h"
#include "jpeglib.h"
#include "jmemsys.h"		/* import the system-dependent declarations */


void free_pool (j_common_ptr cinfo, int pool_id);

void self_destruct (j_common_ptr cinfo);

JBLOCKARRAY access_virt_barray (j_common_ptr cinfo, jvirt_barray_ptr ptr,
                                JDIMENSION start_row, JDIMENSION num_rows,
                                boolean writable);



#endif //JPEG_BENCHMARKER_JMEMMGR_H
