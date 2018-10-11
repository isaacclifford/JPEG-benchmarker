/////*
//// * jcapimin.c
//// *
//// * Copyright (C) 1994-1998, Thomas G. Lane.
//// * This file is part of the Independent JPEG Group's software.
//// * For conditions of distribution and use, see the accompanying README file.
//// *
//// * This file contains application interface code for the compression half
//// * of the JPEG library.  These are the "minimum" API routines that may be
//// * needed in either the normal full-compression case or the transcoding-only
//// * case.
//// *
//// * Most of the routines intended to be called directly by an application
//// * are in this file or in jcapistd.c.  But also see jcparam.c for
//// * parameter-setup helper routines, jcomapi.c for routines shared by
//// * compression and decompression, and jctrans.c for the transcoding case.
//// */
//
//#define JPEG_INTERNALS
//#include "jinclude.h"
//#include "jpeglib.h"
//
//
//
///*
// * Write a special marker.
// * This is only recommended for writing COM or APPn markers.
// * Must be called after jpeg_start_compress() and before
// * first call to jpeg_write_scanlines() or jpeg_write_raw_data().
// */
//
//GLOBAL(void)
//jpeg_write_marker (j_compress_ptr cinfo, int marker,
//		   const JOCTET *dataptr, unsigned int datalen)
//{
//  JMETHOD(void, write_marker_byte, (j_compress_ptr info, int val));
//
//  if (cinfo->next_scanline != 0 ||
//      (cinfo->global_state != CSTATE_SCANNING &&
//       cinfo->global_state != CSTATE_RAW_OK &&
//       cinfo->global_state != CSTATE_WRCOEFS))
//    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
//
//  (*cinfo->marker->write_marker_header) (cinfo, marker, datalen);
//  write_marker_byte = cinfo->marker->write_marker_byte;	/* copy for speed */
//  while (datalen--) {
//    (*write_marker_byte) (cinfo, *dataptr);
//    dataptr++;
//  }
//}
//
///* Same, but piecemeal. */
//
//GLOBAL(void)
//jpeg_write_m_header (j_compress_ptr cinfo, int marker, unsigned int datalen)
//{
//  if (cinfo->next_scanline != 0 ||
//      (cinfo->global_state != CSTATE_SCANNING &&
//       cinfo->global_state != CSTATE_RAW_OK &&
//       cinfo->global_state != CSTATE_WRCOEFS))
//    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
//
//  (*cinfo->marker->write_marker_header) (cinfo, marker, datalen);
//}
//
//GLOBAL(void)
//jpeg_write_m_byte (j_compress_ptr cinfo, int val)
//{
//  (*cinfo->marker->write_marker_byte) (cinfo, val);
//}