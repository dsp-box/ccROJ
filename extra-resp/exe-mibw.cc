/*
call the program as follows:

./convert.sh Ada_1A_007Sel_U.txt > _Ada_1A_007Sel_U.txt
./exe-mibw _Ada_1A_007Sel_U.txt

*/

#include <roj/roj.h>

#include <iostream>
using namespace std;

int main(int argc, char **argv){
  if (argc != 2){
    cerr << "No input file error!!\n";
    throw;
  }
  
  roj_real_array *arr = new roj_real_array (argv[1]);
  roj_complex_signal *sig = new roj_complex_signal (arr);
  roj_signal_config conf = sig->get_config();

  roj_hilbert_equiv *hilbert = new roj_hilbert_equiv(sig);
  roj_complex_signal *xsig = hilbert->get_equivalent();
  roj_complex_signal *icf = xsig -> get_instantaneous_complex_frequency();

  double mass_ibw = 0.0;
  for (int n=0; n<conf.length; n++){
    double ibw = creal(icf->m_waveform[n]);
    if (ibw>0) mass_ibw += ibw;
    else mass_ibw -= ibw;
  }

  cout << "==============================\n";
  cout << "M-IBW:\t" << mass_ibw / conf.length << " per sample\n";
    
  return 0;
}
