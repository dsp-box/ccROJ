/*
call the program as follows:

./convert.sh Ada_1A_007Sel_U.txt > _Ada_1A_007Sel_U.txt
./exe-argand _Ada_1A_007Sel_U.txt > sig.txt
gnuplot -e "plot 'sig.txt' u 2:3 with lines; pause -1"

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

  for (int n=0; n<conf.length; n++)
    cout << (double)n * conf.rate << " " 
	 <<  creal(xsig->m_waveform[n]) << " "
	 <<  cimag(xsig->m_waveform[n]) << "\n";
    
  return 0;
}
