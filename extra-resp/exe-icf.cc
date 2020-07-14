/*
call the program as follows:

./exe-icf > sig.txt 
gnuplot -e "plot 'sig.txt' u 1:2 with lines; pause -1"

*/

#include <roj/roj.h>

#include <iostream>
using namespace std;

int main(void){

  /* generate signal: 2-wave signal */
  /* only to verify ICF */
  
  roj_signal_config conf;
  conf.start = 0.0;
  conf.rate = 100.0;
  conf.length = 10000;

  roj_complex_signal* sig = new roj_complex_signal(conf);  
  double freq1 = 1.0, freq2 = 2.0;
  
  for (int n=0; n<conf.length; n++)
    if (n < conf.length/2)
      sig->m_waveform[n] = cexp(2.0J * M_PI * freq1 * (double)n / conf.rate);
    else
      sig->m_waveform[n] = cexp(2.0J * M_PI * freq2 * (double)n / conf.rate);
 
  /* transform and print */
  
  roj_hilbert_equiv *hilbert = new roj_hilbert_equiv(sig);
  roj_complex_signal *xsig = hilbert->get_equivalent();
  
  roj_complex_signal *icf = xsig -> get_instantaneous_complex_frequency();

  for (int n=0; n<conf.length; n++)
    cout << (double)n / conf.rate << " " << cimag(icf->m_waveform[n]) << "\n";
  
  return 0;
}
