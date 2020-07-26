/*
call the program as follows:

./convert.sh Ada_1A_007Sel_U.txt > _Ada_1A_007Sel_U.txt
./exe-mibw _Ada_1A_007Sel_U.txt 300 120

## ./exe-ostd fname offset(in sec.) window(in sec.)
*/

#include <roj/roj.h>

#include <iostream>
using namespace std;

int main(int argc, char **argv){
  if (argc != 4){
    cerr << "Wrong request!! 3 additional args are expected\n";
    throw;
  }
  
  roj_real_array *arr = new roj_real_array (argv[1]);
  roj_complex_signal *sig = new roj_complex_signal (arr);

  roj_hilbert_equiv *hilbert = new roj_hilbert_equiv(sig);
  roj_complex_signal *xsig = hilbert->get_equivalent();
  roj_complex_signal *icf = xsig -> get_instantaneous_complex_frequency();

  double offset = atof(argv[2]);
  double window = atof(argv[3]);
  icf->cut(offset, offset+window);
  roj_signal_config conf = icf->get_config();
    
  double mass_ibw = 0.0;
  for (int n=0; n<conf.length; n++){
    double ibw = creal(icf->m_waveform[n]);
    if (ibw>0) mass_ibw += ibw;
    else mass_ibw -= ibw;
  }

  cout << "Final mean:\toffset: "
       << offset << "\twindow: " << window << "\tresult: "
       << mass_ibw / conf.length << " rad/s\n";

  return 0;
}
