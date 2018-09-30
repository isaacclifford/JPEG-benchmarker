//
// Created by Isaac Sy on 9/27/2018 for the JPEG-benchmarker project.
//
#ifndef JPEG_BENCHMARKER_JMEMMGR_H
#define JPEG_BENCHMARKER_JMEMMGR_H

#include "jinclude.h"
#include "jpeglib.h"
#include "jmemsys.h"		/* import the system-dependent declarations */

JBLOCKARRAY alloc_barray (j_common_ptr cinfo, int pool_id,
        JDIMENSION blocksperrow, JDIMENSION numrows);

jvirt_sarray_ptr request_virt_sarray (j_common_ptr cinfo, int pool_id, boolean pre_zero,
                      JDIMENSION samplesperrow, JDIMENSION numrows, JDIMENSION maxaccess);

jvirt_barray_ptr request_virt_barray (j_common_ptr cinfo, int pool_id, boolean pre_zero,
                      JDIMENSION blocksperrow, JDIMENSION numrows, JDIMENSION maxaccess);

void realize_virt_arrays (j_common_ptr cinfo);

JSAMPARRAY access_virt_sarray (j_common_ptr cinfo, jvirt_sarray_ptr ptr,
                      JDIMENSION start_row, JDIMENSION num_rows, boolean writable);

JBLOCKARRAY access_virt_barray (j_common_ptr cinfo, jvirt_barray_ptr ptr,
                                JDIMENSION start_row, JDIMENSION num_rows,
                                boolean writable);

void free_pool (j_common_ptr cinfo, int pool_id);

void self_destruct (j_common_ptr cinfo);



#endif //JPEG_BENCHMARKER_JMEMMGR_H
