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

typedef enum {
	START_PASS_PHUFF_DECODER,
	START_PASS_MAIN,
	START_PASS_DPOST,
	START_PASS_2_QUANT,
	START_PASS_MERGED_UPSAMPLE,
	START_PASS_UPSAMPLE,
	START_PASS,
	START_PASS_DCOLOR,
	START_PASS_1_QUANT,
	START_PASS_HUFF_DECODER,
} start_pass_func_type;

void start_pass_phuff_decoder (j_decompress_ptr cinfo);

void start_pass_main (j_decompress_ptr cinfo, J_BUF_MODE pass_mode);

void start_pass_dpost (j_decompress_ptr cinfo, J_BUF_MODE pass_mode);

void start_pass_2_quant (j_decompress_ptr cinfo, boolean is_pre_scan);

void start_pass_merged_upsample (j_decompress_ptr cinfo);

void start_pass_upsample (j_decompress_ptr cinfo);


void start_pass (j_decompress_ptr cinfo);

void start_pass_dcolor (j_decompress_ptr cinfo);

void start_pass_1_quant (j_decompress_ptr cinfo, boolean is_pre_scan);

void start_pass_huff_decoder (j_decompress_ptr cinfo);

typedef enum {
	CONTEXT_MAIN,
	SIMPLE_MAIN,
	CRANK_POST
} process_data_func_type;

void process_data_master(process_data_func_type type, j_decompress_ptr cinfo,
			       JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
			       JDIMENSION out_rows_avail);

/* Main buffer control (downsampled-data buffer) */
struct jpeg_d_main_controller {
	start_pass_func_type start_pass_type;
	process_data_func_type func_type;
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
	start_pass_func_type start_pass_type;

	post_proc_data_func_type post_proc_func;
};

void reset_marker_reader (j_decompress_ptr cinfo);
int read_markers (j_decompress_ptr cinfo);

/* Marker reading & parsing */
struct jpeg_marker_reader {

  /* Read a restart marker --- exported for use by entropy decoder only */
  jpeg_marker_parser_method_func_type read_restart_marker;

  /* State of marker reader --- nominally internal, but applications
   * supplying COM or APPn handlers might like to know the state.
   */
  boolean saw_SOI;		/* found SOI? */
  boolean saw_SOF;		/* found SOF? */
  int next_restart_num;		/* next restart number expected (0-7) */
  unsigned int discarded_bytes;	/* # of bytes skipped looking for a marker */
};

typedef enum {
	DC_FIRST,
	AC_FIRST,
	DC_REFINE,
	AC_REFINE,
	DECODE_MCU_DEFAULT
} decode_mcu_func_type;

boolean decode_mcu_DC_first(j_decompress_ptr cinfo,
                            JBLOCKROW *MCU_data);
boolean decode_mcu_AC_first(j_decompress_ptr cinfo,
                             JBLOCKROW *MCU_data);
boolean decode_mcu_DC_refine (j_decompress_ptr cinfo,
                              JBLOCKROW *MCU_data);
boolean decode_mcu_AC_refine(j_decompress_ptr cinfo,
                             JBLOCKROW *MCU_data);
boolean decode_mcu(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);

boolean decode_mcu_master(decode_mcu_func_type type, j_decompress_ptr cinfo,
                          JBLOCKROW *MCU_data);


/* Entropy decoding */
struct jpeg_entropy_decoder {
	decode_mcu_func_type decode_mcu_type;
	start_pass_func_type start_pass_type;

  /* This is here to share code between baseline and progressive decoders; */
  /* other modules probably should not use it */
  boolean insufficient_data;	/* set TRUE after emitting warning */
};

/* Inverse DCT (also performs dequantization) */
typedef enum {
	JPEG_IDCT_1x1,
	JPEG_IDCT_2x2,
	JPEG_IDCT_4x4,
	JPEG_IDCT_ISLOW,
	JPEG_IDCT_IFAST,
	JPEG_IDCT_FLOAT,
	DEFAULT_NULL
}inverse_DCT_method_func_type;

void inverse_DCT_master(inverse_DCT_method_func_type type, j_decompress_ptr cinfo, jpeg_component_info * compptr,
		 JCOEFPTR coef_block,
		 JSAMPARRAY output_buf, JDIMENSION output_col);

struct jpeg_inverse_dct {
	start_pass_func_type start_pass_type;
  /* It is useful to allow each component to have a separate IDCT method. */
  inverse_DCT_method_func_type inverse_DCT[MAX_COMPONENTS];
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
	start_pass_func_type start_pass_type;
  upsample_func_type func_type;

  boolean need_context_rows;	/* TRUE if need rows above & below */
};

typedef enum {
  GRAYSCALE_CONVERT,
  YCC_RGB_CONVERT,
  GRAY_RGB_CONVERT,
  YCCK_CMYK_CONVERT,
  NULL_CONVERT
}color_convert_func_type;

void color_convert_master(color_convert_func_type type, j_decompress_ptr cinfo,
				JSAMPIMAGE input_buf, JDIMENSION input_row,
				JSAMPARRAY output_buf, int num_rows);

/* Colorspace conversion */
struct jpeg_color_deconverter {
  start_pass_func_type start_pass_type;
  color_convert_func_type color_convert_type;
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

typedef enum {
	PRESCAN_QUANTIZE,
	PASS2_FS_DITHER,
	PASS2_NO_DITHER,
	COLOR_QUANTIZE3,
	COLOR_QUANTIZE,
	QUANTIZE3_ORD_DITHER,
	QUANTIZE_ORD_DITHER,
	QUANTIZE_FS_DITHER
} color_quantize_func_type;

void prescan_quantize (j_decompress_ptr cinfo, JSAMPARRAY input_buf,
									JSAMPARRAY output_buf, int num_rows);

void pass2_fs_dither (j_decompress_ptr cinfo,
								 JSAMPARRAY input_buf, JSAMPARRAY output_buf, int num_rows);

void pass2_no_dither (j_decompress_ptr cinfo,
								 JSAMPARRAY input_buf, JSAMPARRAY output_buf, int num_rows);

void color_quantize3 (j_decompress_ptr cinfo, JSAMPARRAY input_buf,
								 JSAMPARRAY output_buf, int num_rows);

void color_quantize (j_decompress_ptr cinfo, JSAMPARRAY input_buf,
								JSAMPARRAY output_buf, int num_rows);

void quantize3_ord_dither (j_decompress_ptr cinfo, JSAMPARRAY input_buf,
											JSAMPARRAY output_buf, int num_rows);

void quantize_ord_dither (j_decompress_ptr cinfo, JSAMPARRAY input_buf,
                     JSAMPARRAY output_buf, int num_rows);

void quantize_fs_dither (j_decompress_ptr cinfo, JSAMPARRAY input_buf,
										JSAMPARRAY output_buf, int num_rows);

void color_quantize_master(color_quantize_func_type type, j_decompress_ptr cinfo, JSAMPARRAY input_buf,
				JSAMPARRAY output_buf, int num_rows);

struct jpeg_color_quantizer {
  start_pass_func_type start_pass_type;
  color_quantize_func_type func_type;
  finish_pass_type fin_pass_type;
  int new_color_map_quant_index;

};

void start_pass_master(start_pass_func_type type, j_decompress_ptr cinfo, void* pass_mode);


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
