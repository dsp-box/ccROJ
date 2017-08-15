/* *************************************************** *
 * This file is a part of ccROJ project (version 0-45) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-window-gener.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_window_generator.
*
* @param [in] a_rate: Sampling rate.
*/
roj_window_generator :: roj_window_generator (double a_rate){
  
  if(a_rate<=0){
    call_warning("in roj_window_generator :: roj_window_generator");    
    call_error("rate <= 0");
  }
  m_rate = a_rate;
  
  /* defaults */
  m_type = ROJ_BH_WINDOW;
  m_crate = 0.0;
  m_length = 3;
}

/**
* @type: destructor
* @brief: The deconstructor is default.
*/

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine returns window sampling rate.
*
* @return: Sampling rate.
*/
double roj_window_generator :: get_rate (){
  
  return m_rate;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: Type setter. Checking is during generation.
*
* @param [in] a_code: A code of type.
*/
void roj_window_generator :: set_type (char a_code){
  
  m_type = a_code;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: Order setter, filter length.
*
* @param [in] a_order: Window length.
*/
void roj_window_generator :: set_length (int a_length){

  if(a_length<=0){
    call_warning("in roj_window_generator :: set_length");    
    call_error("a_length <= 0");
  }
  m_length = a_length;
}

/**
* @type: method
* @brief: This routine returns window length in samples.
*
* @return: Window length.
*/
int roj_window_generator :: get_length (){

  return m_length;
}


/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine returns window duration in seconds.
*
* @return: Window duration.
*/
double roj_window_generator :: calc_duration (){

  /* len - 0.5 <- why ??? */
  if (m_type==ROJ_BH_WINDOW) 
    return ((double)m_length) / m_rate;
  
  else{
    call_warning("in roj_window_generator :: calc_duration");    
    call_error("have to be implemented");
  }
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine returns window chirp-rate in Hz/s.
*
* @return: Window chirp-rate.
*/
double roj_window_generator :: get_chirp_rate (){

  return m_crate;
}    

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine updates window chirp-rate in Hz/s.
*
* @param [in] a_crate: window chirp-rate.
*
* @return: Window chirp-rate.
*/
void roj_window_generator :: set_chirp_rate (double a_crate){

  m_crate = a_crate;
}    

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This function returns empty however configured window as roj_complex_signal.
*
* @return: A pointer to empty window (roj_complex_signal object).
*/
roj_complex_signal* roj_window_generator :: get_empty_window (){

  roj_signal_config sig_conf;
  sig_conf.start = -0.5 * (double)m_length / m_rate; 
  sig_conf.length = m_length; 
  sig_conf.rate = m_rate; 
  
  roj_complex_signal* win = new roj_complex_signal(sig_conf);
  return win;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function returns window as roj_complex_signal.
*
* @param [in] a_d_order (default 0): An order of of window derivative with respect to time.
* @param [in] a_t_order (default 0): An order of power of timeramp which is multiplied by the window waveform.
*
* @return: A pointer to the window object (roj_complex_signal object).
*/
roj_complex_signal* roj_window_generator :: get_window (int a_d_order, int a_t_order){

  if(a_t_order<0){
    call_warning("in roj_window_generator :: get_window ");    
    call_error("time order < 0");
  }

  if(a_d_order<0){
    call_warning("in roj_window_generator :: get_window ");    
    call_error("derivative order < 0");
  }

  roj_complex_signal* win = NULL;
  switch(m_type){
  case ROJ_BH_WINDOW:
    switch(a_d_order){
    case 0:
      win = get_blackman_harris_win();
      break;
    case 1:
      win = get_blackman_harris_dwin();
      break;
    case 2:
      win = get_blackman_harris_d2win();
      break;
    default:
      call_warning("in roj_window_generator :: get_window ");    
      call_error("derivative for this order for this window type is not defined");
    }
    break;
  default:
    call_warning("in roj_window_generator :: get_window ");    
    call_error("unknown window type");
  }
  
  /* multiply by timeramp */
  if(a_t_order>0)
    for(int n=0;n<m_length;n++){
    
      double ramp = (double)n/(m_length-1) - 0.5;  
      ramp *= calc_duration();

      win->m_waveform[n] *= pow(ramp, a_t_order) ; 
    }    

  return win;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function returns window gain.
*
* @param [in] a_d_order (default 0): An order of of window derivative with respect to time.
* @param [in] a_t_order (default 0): An order of power of timeramp which is multiplied by the window waveform.
*
* @return: The window gain.
*/
double roj_window_generator :: calc_gain (int a_d_order, int a_t_order){

  roj_complex_signal* win = get_window(a_d_order, a_t_order);

  double total = 0.0;
  for(int n=0; n<m_length; n++)
    total += cabs(win->m_waveform[n]);

  delete win;
  return total;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This function creates the Blackman-Harris window.
*
* @return: The window as roj_complex_signal object.
*/
roj_complex_signal* roj_window_generator :: get_blackman_harris_win (){

  /* create window object for return */
  roj_complex_signal* win = get_empty_window();
  
  /* fill waveform */
  for(int n=0;n<m_length;n++)
    win->m_waveform[n] = calc_blackman_harris(n, m_length, 0);
     
  /* for chirplet transform */
  if(m_crate!=0.0){
    double half_len = (double)(m_length-1)/2; 

    for(int n=0;n<m_length;n++){
      double time = (double)(-half_len + n) / m_rate;
      double arg = -M_PI*m_crate * pow(time, 2.0); 
      complex double crr = cexp(1I*arg);
      win->m_waveform[n] *= crr;
    }
  }
  
  return win;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This function creates the first-order derivative of the Blackman-Harris window.
*
* @return: The window as roj_complex_signal object.
*/
roj_complex_signal* roj_window_generator :: get_blackman_harris_dwin (){

  /* create output window object  */
  roj_complex_signal* win = get_empty_window();
  
  /* fill waveform */
  for(int n=0;n<m_length;n++){
    win->m_waveform[n] = calc_blackman_harris(n, m_length, 1);
    win->m_waveform[n] /= calc_duration();
  }    
  
  /* for chirplet transform */
  if(m_crate!=0.0){

    double half_len = (double)(m_length-1)/2; 
    for(int n=0;n<m_length;n++){
      /* time fo window generation */
      double ramp = calc_duration() * ((double)n/(m_length-1) - 0.5); 

      /* get single d window coefficient */
      complex double tmp = calc_blackman_harris(n, m_length, 0);      
      tmp *= TWO_PI * m_crate; 
      tmp *= ramp;      
      tmp *= -1I;
      
      win->m_waveform[n] += tmp;

      double time = (double)(-half_len + n) / m_rate;
      double arg = -M_PI * m_crate * pow(time, 2.0); 
      complex double crr = cexp(1I*arg);      
      win->m_waveform[n] *= crr;
    }
  }

  return win;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This function creates the second-order derivative of the Blackman-Harris window.
*
* @return: The window as roj_complex_signal object.
*/
roj_complex_signal* roj_window_generator :: get_blackman_harris_d2win (){

  /* create window object for return */
  roj_complex_signal* win = get_empty_window();
    
  /* fill waveform */
  for(int n=0;n<m_length;n++){
    win->m_waveform[n] = calc_blackman_harris(n, m_length, 2);      
    win->m_waveform[n] /= pow(calc_duration(), 2.0);
  }    
  
  if(m_crate!=0.0){

    double half_len = (double)(m_length-1)/2; 
    for(int n=0;n<m_length;n++){       
      /* time fo window generation */
      double ramp = calc_duration() * ((double)n/(m_length-1) - 0.5); 

      /* get single d2 window coefficient */
      complex double tmp = calc_blackman_harris(n, m_length, 1);      
      tmp *= TWO_PI * m_crate;
      tmp *= ramp / calc_duration();
      tmp *= -2I;
      
      win->m_waveform[n] += tmp;

      tmp = calc_blackman_harris(n, m_length, 0);  
      tmp *= -1I  -  TWO_PI * m_crate * pow(ramp, 2.0);
      tmp *=  m_crate * TWO_PI;
      
      win->m_waveform[n] += tmp;

      double time = (double)(-half_len + n) / m_rate;
      double arg = -M_PI * m_crate * pow(time, 2.0); 
      complex double crr = cexp(1I*arg);      
      win->m_waveform[n] *= crr;
    }
  }

  return win;
}
