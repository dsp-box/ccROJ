/* *************************************************** *
 * This file is a part of ccROJ project (version 0-44) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-hilbert-equiv.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_hilbert_equiv based on Fourier spectrum. 
*
* @param [in] a_signal: A pointer to a signal which is transformed.
*/
roj_hilbert_equiv :: roj_hilbert_equiv (roj_complex_signal* a_signal){

  if(a_signal->check_imag())
    call_warning("transformed signal is has complex values");
  
  m_signal = new roj_complex_signal(a_signal);
 
  m_equivalent = NULL;
  m_transform = NULL;
}

/**
* @type: destructor
* @brief: This is a spectrum deconstructor. It also release memory for lines. 
*/
roj_hilbert_equiv :: ~roj_hilbert_equiv (){

  delete m_signal;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine returns signal parameters as a structure.
*
* @return: A pointer to the structure which contains the configuration.
*/
roj_signal_config roj_hilbert_equiv :: get_config (){
  
  return m_signal->get_config();
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function returns the analytic signal (signal equivalent) using Fourier transform.
*
* @return: A pointer to the resultant signal object.
*/
roj_complex_signal* roj_hilbert_equiv :: get_equivalent (){
  
  if(m_equivalent==NULL){
    roj_fourier_spectrum *spectrum = m_signal->get_spectrum(); 
    roj_signal_config *conf = spectrum->get_config();
    int half = conf->length/2;
    for(int n=0; n<half; n++)
      spectrum->m_spectrum[n] = 0.0;
    m_equivalent = spectrum->get_signal();
    delete spectrum;
    delete conf;
  }
  
  return m_equivalent;
}

