/*
call the program as follows:

./convert.sh Ada_1A_007Sel_U.txt > _Ada_1A_007Sel_U.txt
./exe-ostd _Ada_1A_007Sel_U.txt

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

  roj_array_config ring_conf = {128, -M_PI, M_PI};
  roj_real_array *ring_n = new roj_real_array (ring_conf);
  roj_real_array *ring_v = new roj_real_array (ring_conf);
  roj_real_array *ring_v2 = new roj_real_array (ring_conf);
  
  for (int n=0; n<conf.length; n++){
    double argval = carg(xsig->m_waveform[n]);
    double absval = cabs(xsig->m_waveform[n]);
  
    add_sample_to_profile(argval, 1, ring_n);
    add_sample_to_profile(argval, absval, ring_v);
    add_sample_to_profile(argval, absval*absval, ring_v2);
  }

  double mass = 0.0;
  double avg = 0.0;

  for (int n=0; n<ring_conf.length; n++){
    double ev = ring_v->m_data[n] / ring_n->m_data[n];
    double ev2 = ring_v2->m_data[n] / ring_n->m_data[n];
    double s = sqrt(ev2 - ev * ev);
    double cv = 100 * s / ev;
    cout << n << "` " << cv << "%\n";

    avg += cv * ring_n->m_data[n];
    mass += ring_n->m_data[n];    
  }

  cout << "==============================\n";
  cout << "O-STD (normalized):\t" << avg / mass << "%\n";
    
  return 0;
}
