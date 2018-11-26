/*
 * jpegint.h
 *
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file provides common declarations for the various JPEG modules.
 * These declarations are considered internal to the JPEG library; most
 * applications using the library shouldn't need to include this file.
 */


/* Declarations for both compression & decompression */

typedef enum {			/* Operating modes for buffer controllers */
	JBUF_PASS_THRU,		/* Plain stripwise operation */
	/* Remaining modes require a full-image buffer to have been created */
	JBUF_SAVE_SOURCE,	/* Run source subobject only, save output */
	JBUF_CRANK_DEST,	/* Run dest subobject only, using saved data */
	JBUF_SAVE_AND_PASS	/* Run both subobjects, save output */
} J_BUF_MODE;

/* Values of global_state field (jdapi.c has some dependencies on ordering!) */
#define CSTATE_START	100	/* after create_compress */
#define CSTATE_SCANNING	101	/* start_compress done, write_scanlines OK */
#define CSTATE_RAW_OK	102	/* start_compress done, write_raw_data OK */
#define CSTATE_WRCOEFS	103	/* jpeg_write_coefficients done */
#define DSTATE_START	200	/* after create_decompress */
#define DSTATE_INHEADER	201	/* reading header markers, no SOS yet */
#define DSTATE_READY	202	/* found SOS, ready for start_decompress */
#define DSTATE_PRELOAD	203	/* reading multiscan file in start_decompress*/
#define DSTATE_PRESCAN	204	/* performing dummy pass for 2-pass quant */
#define DSTATE_SCANNING	205	/* start_decompress done, read_scanlines OK */
#define DSTATE_RAW_OK	206	/* start_decompress done, read_raw_data OK */
#define DSTATE_BUFIMAGE	207	/* expecting jpeg_start_output */
#define DSTATE_BUFPOST	208	/* looking for SOS/EOI in jpeg_finish_output */
#define DSTATE_RDCOEFS	209	/* reading file in jpeg_read_coefficients */
#define DSTATE_STOPPING	210	/* looking for EOI in jpeg_finish_decompress */


/* Declarations for decompression modules */

void prepare_for_output_pass (j_decompress_ptr cinfo);
void finish_output_pass (j_decompress_ptr cinfo);

/* Master control module */
struct jpeg_decomp_master {
  /* State variables made visible to other modules */
  boolean is_dummy_pass;	/* True during 1st pass for 2-pass quant */
};

/* Input control module */

void reset_input_controller (j_decompress_ptr cinfo);
void start_input_pass_input (j_decompress_ptr cinfo);
void finish_input_pass (j_decompress_ptr cinfo);

int consume_input_master(j_decompress_ptr cinfo);

struct jpeg_input_controller {
  /* State variables made visible to other modules */
  boolean has_multiple_scans;	/* True if file has multiple scans */
  boolean eoi_reached;		/* True when EOI has been consumed */
  boolean consume_using_coefficient;
};

/* Main buffer control (downsampled-data buffer) */
struct jpeg_d_main_controller {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo, J_BUF_MODE pass_mode));
  JMETHOD(void, process_data, (j_decompress_ptr cinfo, //SINGLE
			       JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
			       JDIMENSION out_rows_avail));
};

/* Coefficient buffer control */
typedef enum {
    DEFAULT,
    DUMMY
}consume_data_t;

void start_input_pass_coef (j_decompress_ptr cinfo);
int consume_data_master(j_decompress_ptr cinfo);
void start_output_pass (j_decompress_ptr cinfo);

typedef enum {
	DEFAULT_DECOMPRESS,
	ONEPASS,
	SMOOTH
}decompress_data_type;

int decompress_data_master(decompress_data_type type, j_decompress_ptr cinfo, JSAMPIMAGE output_buf);


struct jpeg_d_coef_controller {
	decompress_data_type d_type;
  /* Pointer to array of coefficient virtual arrays, or NULL if none */
  jvirt_barray_ptr *coef_arrays;
  consume_data_t consume_data_type;

};

typedef enum {
	PROC_PREPASS,
	PROC_ONEPASS,
	PROC_TWOPASS,
	PROC_UPSAMPLE,
} post_proc_data_func_type;

void
post_process_1pass (j_decompress_ptr cinfo,
										JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
										JDIMENSION in_row_groups_avail,
										JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
										JDIMENSION out_rows_avail);

void
post_process_2pass (j_decompress_ptr cinfo,
										JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
										JDIMENSION in_row_groups_avail,
										JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
										JDIMENSION out_rows_avail);

void
post_process_prepass (j_decompress_ptr cinfo,
											JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
											JDIMENSION in_row_groups_avail,
											JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
											JDIMENSION out_rows_avail);

void post_process_master(post_proc_data_func_type func_type, j_decompress_ptr cinfo,
																 JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
																 JDIMENSION in_row_groups_avail,
																 JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
																 JDIMENSION out_rows_avail);

/* Decompression postprocessing (color quantization buffer control) */
struct jpeg_d_post_controller {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo, J_BUF_MODE pass_mode));

	post_proc_data_func_type post_proc_func;
};

void reset_marker_reader (j_decompress_ptr cinfo);

/* Marker reading & parsing */
struct jpeg_marker_reader {
  /* Read markers until SOS or EOI.
   * Returns same codes as are defined for jpeg_consume_input:
   * JPEG_SUSPENDED, JPEG_REACHED_SOS, or JPEG_REACHED_EOI.
   */
  JMETHOD(int, read_markers, (j_decompress_ptr cinfo)); //EASY
  /* Read a restart marker --- exported for use by entropy decoder only */
  jpeg_marker_parser_method read_restart_marker;

  /* State of marker reader --- nominally internal, but applications
   * supplying COM or APPn handlers might like to know the state.
   */
  boolean saw_SOI;		/* found SOI? */
  boolean saw_SOF;		/* found SOF? */
  int next_restart_num;		/* next restart number expected (0-7) */
  unsigned int discarded_bytes;	/* # of bytes skipped looking for a marker */
};

/* Entropy decoding */
struct jpeg_entropy_decoder {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));
  JMETHOD(boolean, decode_mcu, (j_decompress_ptr cinfo, //SINGLE
				JBLOCKROW *MCU_data));

  /* This is here to share code between baseline and progressive decoders; */
  /* other modules probably should not use it */
  boolean insufficient_data;	/* set TRUE after emitting warning */
};

/* Inverse DCT (also performs dequantization) */
typedef JMETHOD(void, inverse_DCT_method_ptr,
		(j_decompress_ptr cinfo, jpeg_component_info * compptr,
		 JCOEFPTR coef_block,
		 JSAMPARRAY output_buf, JDIMENSION output_col));

struct jpeg_inverse_dct {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));
  /* It is useful to allow each component to have a separate IDCT method. */
  inverse_DCT_method_ptr inverse_DCT[MAX_COMPONENTS];
};

/* Upsampling (note that upsampler must also call color converter) */

typedef enum {
  SEP_UPSAMPLE,
  MERGED_V2UPSAMPLE,
  MERGED_V1UPSAMPLE
} upsample_func_type;

void sep_upsample (j_decompress_ptr cinfo,
JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
JDIMENSION in_row_groups_avail,
        JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
        JDIMENSION out_rows_avail);

void merged_1v_upsample (j_decompress_ptr cinfo,
JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
JDIMENSION in_row_groups_avail,
        JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
        JDIMENSION out_rows_avail);

void merged_2v_upsample (j_decompress_ptr cinfo,
JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
JDIMENSION in_row_groups_avail,
        JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
        JDIMENSION out_rows_avail);

void upsample_master(upsample_func_type type, j_decompress_ptr cinfo,
                     JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
                     JDIMENSION in_row_groups_avail,
                     JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
                     JDIMENSION out_rows_avail);

struct jpeg_upsampler {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));

  upsample_func_type func_type;

  boolean need_context_rows;	/* TRUE if need rows above & below */
};

/* Colorspace conversion */
struct jpeg_color_deconverter {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));
  JMETHOD(void, color_convert, (j_decompress_ptr cinfo, //SINGLE
				JSAMPIMAGE input_buf, JDIMENSION input_row,
				JSAMPARRAY output_buf, int num_rows));
};

/* Color quantization or color precision reduction */

void new_color_map_1_quant (j_decompress_ptr cinfo);
void new_color_map_2_quant (j_decompress_ptr cinfo);

typedef enum{
	DEFAULT_ONE,
	DEFAULT_TWO,
	QUANT_ONE,

} finish_pass_type;

void finish_pass1 (j_decompress_ptr cinfo);
void finish_pass2 (j_decompress_ptr cinfo);
void finish_pass_1_quant(j_decompress_ptr cinfo);


struct jpeg_color_quantizer {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo, boolean is_pre_scan)); //Multi
  JMETHOD(void, color_quantize, (j_decompress_ptr cinfo, //Single
				 JSAMPARRAY input_buf, JSAMPARRAY output_buf,
				 int num_rows));
	finish_pass_type fin_pass_type;
	int new_color_map_quant_index;

};


/* Miscellaneous useful macros */

#undef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#undef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))


/* We assume that right shift corresponds to signed division by 2 with
 * rounding towards minus infinity.  This is correct for typical "arithmetic
 * shift" instructions that shift in copies of the sign bit.  But some
 * C compilers implement >> with an unsigned shift.  For these machines you
 * must define RIGHT_SHIFT_IS_UNSIGNED.
 * RIGHT_SHIFT provides a proper signed right shift of an INT32 quantity.
 * It is only applied with constant shift counts.  SHIFT_TEMPS must be
 * included in the variables of any routine using RIGHT_SHIFT.
 */

#ifdef RIGHT_SHIFT_IS_UNSIGNED
#define SHIFT_TEMPS	INT32 shift_temp;
#define RIGHT_SHIFT(x,shft)  \
	((shift_temp = (x)) < 0 ? \
	 (shift_temp >> (shft)) | ((~((INT32) 0)) << (32-(shft))) : \
	 (shift_temp >> (shft)))
#else
#define SHIFT_TEMPS
#define RIGHT_SHIFT(x,shft)	((x) >> (shft))
#endif


/* Short forms of external names for systems with brain-damaged linkers. */

#ifdef NEED_SHORT_EXTERNAL_NAMES
#define jinit_compress_master	jICompress
#define jinit_c_master_control	jICMaster
#define jinit_c_main_controller	jICMainC
#define jinit_c_prep_controller	jICPrepC
#define jinit_c_coef_controller	jICCoefC
#define jinit_color_converter	jICColor
#define jinit_downsampler	jIDownsampler
#define jinit_forward_dct	jIFDCT
#define jinit_huff_encoder	jIHEncoder
#define jinit_phuff_encoder	jIPHEncoder
#define jinit_marker_writer	jIMWriter
#define jinit_master_decompress	jIDMaster
#define jinit_d_main_controller	jIDMainC
#define jinit_d_coef_controller	jIDCoefC
#define jinit_d_post_controller	jIDPostC
#define jinit_input_controller	jIInCtlr
#define jinit_marker_reader	jIMReader
#define jinit_huff_decoder	jIHDecoder
#define jinit_phuff_decoder	jIPHDecoder
#define jinit_inverse_dct	jIIDCT
#define jinit_upsampler		jIUpsampler
#define jinit_color_deconverter	jIDColor
#define jinit_1pass_quantizer	jI1Quant
#define jinit_2pass_quantizer	jI2Quant
#define jinit_merged_upsampler	jIMUpsampler
#define jinit_memory_mgr	jIMemMgr
#define jdiv_round_up		jDivRound
#define jround_up		jRound
#define jcopy_sample_rows	jCopySamples
#define jcopy_block_row		jCopyBlocks
#define jzero_far		jZeroFar
#define jpeg_zigzag_order	jZIGTable
#define jpeg_natural_order	jZAGTable
#endif /* NEED_SHORT_EXTERNAL_NAMES */


/* Compression module initialization routines */
//EXTERN(void) jinit_compress_master JPP((j_compress_ptr cinfo));
//EXTERN(void) jinit_c_master_control JPP((j_compress_ptr cinfo,
//					 boolean transcode_only));
//EXTERN(void) jinit_c_main_controller JPP((j_compress_ptr cinfo,
//					  boolean need_full_buffer));
//EXTERN(void) jinit_c_prep_controller JPP((j_compress_ptr cinfo,
//					  boolean need_full_buffer));
//EXTERN(void) jinit_c_coef_controller JPP((j_compress_ptr cinfo,
//					  boolean need_full_buffer));
//EXTERN(void) jinit_color_converter JPP((j_compress_ptr cinfo));
//EXTERN(void) jinit_downsampler JPP((j_compress_ptr cinfo));
//EXTERN(void) jinit_forward_dct JPP((j_compress_ptr cinfo));
//EXTERN(void) jinit_huff_encoder JPP((j_compress_ptr cinfo));
//EXTERN(void) jinit_phuff_encoder JPP((j_compress_ptr cinfo));
//EXTERN(void) jinit_marker_writer JPP((j_compress_ptr cinfo));
/* Decompression module initialization routines */
EXTERN(void) jinit_master_decompress JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_d_main_controller JPP((j_decompress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_d_coef_controller JPP((j_decompress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_d_post_controller JPP((j_decompress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_input_controller JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_marker_reader JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_huff_decoder JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_phuff_decoder JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_inverse_dct JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_upsampler JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_color_deconverter JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_1pass_quantizer JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_2pass_quantizer JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_merged_upsampler JPP((j_decompress_ptr cinfo));
/* Memory manager initialization */
EXTERN(void) jinit_memory_mgr JPP((j_common_ptr cinfo));

/* Utility routines in jutils.c */
EXTERN(long) jdiv_round_up JPP((long a, long b));
EXTERN(long) jround_up JPP((long a, long b));
EXTERN(void) jcopy_sample_rows JPP((JSAMPARRAY input_array, int source_row,
				    JSAMPARRAY output_array, int dest_row,
				    int num_rows, JDIMENSION num_cols));
EXTERN(void) jcopy_block_row JPP((JBLOCKROW input_row, JBLOCKROW output_row,
				  JDIMENSION num_blocks));
EXTERN(void) jzero_far JPP((void FAR * target, size_t bytestozero));
/* Constant tables in jutils.c */
#if 0				/* This table is not actually needed in v6a */
extern const int jpeg_zigzag_order[]; /* natural coef order to zigzag order */
#endif
extern const int jpeg_natural_order[]; /* zigzag coef order to natural order */

/* Suppress undefined-structure complaints if necessary. */

#ifdef INCOMPLETE_TYPES_BROKEN
#ifndef AM_MEMORY_MANAGER	/* only jmemmgr.c defines these */
struct jvirt_sarray_control { long dummy; };
struct jvirt_barray_control { long dummy; };
#endif
#endif /* INCOMPLETE_TYPES_BROKEN */
