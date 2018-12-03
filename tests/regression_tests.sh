dir_name="output"
mkdir -p -- "$dir_name"
mkdir -p -- "$dir_name/comp_out"

echo "--Colors 10--"
djpeg -colors 10 wedge_small.jpg > $dir_name/wedge_small_10colors.jpg
./cjpeg -quality 100 $dir_name/wedge_small_10colors.jpg > $dir_name/comp_out/wedge_small_10colors_comp.jpg

djpeg -colors 10 yos_large.jpg > $dir_name/yos_large_fast.jpg > $dir_name/yos_large_10colors.jpg
./cjpeg -quality 100 $dir_name/yos_large_10colors.jpg > $dir_name/comp_out/yos_large_10colors_comp.jpg


echo "--Greyscale--"
djpeg -greyscale wedge_small.jpg > $dir_name/wedge_small_grey.jpg
./cjpeg -quality 100 $dir_name/wedge_small_grey.jpg > $dir_name/comp_out/wedge_small_grey_comp.jpg

djpeg -greyscale yos_large.jpg > $dir_name/yos_large_grey.jpg > $dir_name/yos_large_grey.jpg
./cjpeg -quality 100 $dir_name/yos_large_grey.jpg > $dir_name/comp_out/yos_large_grey_comp.jpg


echo "--Fast--"
djpeg -fast wedge_small.jpg > $dir_name/wedge_small_fast.jpg
./cjpeg -quality 100 $dir_name/wedge_small_fast.jpg > $dir_name/comp_out/wedge_small_fast_comp.jpg

djpeg -fast yos_large.jpg > $dir_name/yos_large_fast.jpg > $dir_name/yos_large_fast.jpg
./cjpeg -quality 100 $dir_name/yos_large_fast.jpg > $dir_name/comp_out/yos_large_fast_comp.jpg


echo "--Tenth Scale--"
djpeg -scale 1/10 wedge_small.jpg > $dir_name/wedge_small_tenth_scale.jpg
./cjpeg -quality 100 $dir_name/wedge_small_tenth_scale.jpg > $dir_name/comp_out/wedge_small_tenth_scale_comp.jpg

djpeg -scale 1/10 yos_large.jpg > $dir_name/yos_large_tenth_scale.jpg > $dir_name/yos_large_tenth_scale.jpg
./cjpeg -quality 100 $dir_name/yos_large_tenth_scale.jpg > $dir_name/comp_out/yos_large_tenth_scale_comp.jpg


echo "--BMP--"
djpeg -bmp wedge_small.jpg > $dir_name/wedge_small_bmp.bmp
./cjpeg -quality 100 $dir_name/wedge_small_bmp.bmp > $dir_name/comp_out/wedge_small_bmp_comp.jpg

djpeg -bmp yos_large.jpg > $dir_name/yos_large.jpg > $dir_name/yos_large_bmp.bmp
./cjpeg -quality 100 $dir_name/yos_large_bmp.bmp > $dir_name/comp_out/yos_large_bmp_comp.jpg


echo "--GIF--"
djpeg -gif wedge_small.jpg > $dir_name/wedge_small_gif.gif
./cjpeg -quality 100 $dir_name/wedge_small_gif.gif > $dir_name/comp_out/wedge_small_gif_comp.jpg

djpeg -gif yos_large.jpg > $dir_name/yos_large_gif.gif
./cjpeg -quality 100 $dir_name/yos_large_gif.gif > $dir_name/comp_out/yos_large_gif_comp.jpg


echo "--DCT INT--"
djpeg -dct int wedge_small.jpg > $dir_name/wedge_small_dct_int.jpg
./cjpeg -quality 100 $dir_name/wedge_small_dct_int.jpg > $dir_name/comp_out/wedge_small_dct_int_comp.jpg

djpeg -dct int yos_large.jpg > $dir_name/yos_large_dct_int.jpg
./cjpeg -quality 100 $dir_name/yos_large_dct_int.jpg > $dir_name/comp_out/yos_large_dct_int_comp.jpg

