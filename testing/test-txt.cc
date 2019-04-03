/* internal roj headers */
#include "../roj.hh"

int main(void){


  roj_real_array *arr_re = new roj_real_array("real.txt");  
  roj_real_array *arr_im = new roj_real_array("imag.txt");
  
  roj_complex_signal* signal_from_wav = new roj_complex_signal(arr_re, arr_im);

  return 0;
}
