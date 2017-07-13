
#include "roj-hough-transform.hh"

/* ************************************************************************************************************************* */
/**
 * @type: constructor
 * @brief: This is a constructor of roj_hough_transform based on Fourier spectrum. 
 *
 * @param [in] a_energy: an energy distribution for transformation.
 * @param [in] a_frequency_conf:  
 * @param [in] a_chirprate_conf: 
 * @param [in] a_treshold: a treshold which allows to accelerate computing (default is 0.0) 
 */
roj_hough_transform :: roj_hough_transform(roj_real_matrix* a_energy, roj_array_config a_frequency_conf, roj_array_config a_chirprate_conf, double a_treshold){

  if (a_energy==NULL)
    call_error("arg is null");

  if(a_treshold<0.0)
    call_warning("a given treshold is negative");

  verify_config(a_frequency_conf);
  verify_config(a_chirprate_conf);

  m_frequency_conf = a_frequency_conf;
  m_chirprate_conf = a_chirprate_conf;
  m_treshold = a_treshold;
  
  m_energy = new roj_real_matrix(a_energy);
  m_transform = reassign_energy(); 
}

/**
 * @type: destructor
 * @brief: This is a spectrum deconstructor. It also release memory for lines. 
 */
roj_hough_transform :: ~roj_hough_transform (){

  delete m_transform;
  delete m_energy;
}

/* ************************************************************************************************************************* */
/**
 * @type: private
 * @brief: This routine allows to compute the Hough transform as roj_real_matrix. 
 *
 * @return: an energy of the Hough transform.
 */
roj_real_matrix* roj_hough_transform :: reassign_energy(){
     
  roj_image_config output_conf;
  output_conf.time = m_frequency_conf;
  output_conf.frequency = m_chirprate_conf;
  roj_real_matrix* output = new roj_real_matrix(output_conf);
    
  roj_image_config energy_conf = m_energy->get_config();
  for(int t=0; t<energy_conf.time.length; t++){

    double time = m_energy->get_time_by_index(t);
    for(int f=0; f<energy_conf.frequency.length; f++){

      if(m_energy->m_data[t][f]<=m_treshold)
	continue;
      
      double frequency = m_energy->get_frequency_by_index(f);
      for(int c=0; c<m_chirprate_conf.length; c++){

	double chirp_rate = output->get_frequency_by_index(c);
	double init_frequency = frequency - chirp_rate * time;
	
	int k = output->get_index_by_time(init_frequency);
	if(output->check_in_time_index(k))
	  output->m_data[k][c] += m_energy->m_data[t][f];
      }
    }

    print_progress(t+1, energy_conf.time.length, "hough");
  }
  
  print_progress(0, 0, "hough");
  return output;
}

/* ************************************************************************************************************************* */
/**
 * @type: private
 * @brief: This routine allows to compute a single point of the Hough transform. 
 *
 * @param [in] a_init_frequency:  
 * @param [in] a_chirp_rate: 
 * @param [in] a_treshold: a treshold which allows to ignore some energy points (default is 0.0) 
 *
 * @return: an energy of the Hough transform.
 */
double roj_hough_transform :: calc_single_point(double a_init_frequency, double a_chirp_rate, double a_treshold){
    
  double energy = 0.0;
  
  roj_image_config energy_conf = m_energy->get_config();
  double f_delta = (energy_conf.frequency.max - energy_conf.frequency.min)/(energy_conf.frequency.length-1);
  for(int t=0; t<energy_conf.time.length; t++){

    double time = m_energy->get_time_by_index(t);
    double frequency = a_init_frequency + time * a_chirp_rate;
    double f_findex = (frequency-energy_conf.frequency.min)/f_delta;

    double tmp = 0.0;
    if(m_energy->check_in_frequency_index(floor(f_findex)))
      tmp += m_energy->m_data[t][(int)floor(f_findex)] * (f_findex - floor(f_findex));
    
    if(m_energy->check_in_frequency_index(ceil(f_findex)))
      tmp += m_energy->m_data[t][(int)ceil(f_findex)] * (ceil(f_findex) - f_findex);
    
    if (tmp>=a_treshold)
      energy += tmp;
  }
  
  return energy;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine allows to compute a chirp-rate for the maximal Hough energy using bi-section method. 
 *
 * @param [in] a_max_error: interval for bi-section method.
 *
 * @return: estimated chirp-rate.
 */
double roj_hough_transform :: get_major_chirp_rate(double a_max_error){

  if (a_max_error<=0.0)
    call_error("max error <= 0");

  int max_f = 0, max_c = 0;
  for(int f=0; f<m_frequency_conf.length; f++){
    for(int c=0; c<m_chirprate_conf.length; c++){

      if(m_transform->m_data[f][c] > m_transform->m_data[max_f][max_c]){
	max_f = f;
	max_c = c;
      }
    }
  }

  double i_freq = m_transform->get_time_by_index(max_f);
  double c_min, c_max, de_min, de_max;
  if (max_c==0){
    c_min = m_transform->get_frequency_by_index(max_c);
    c_max = m_transform->get_frequency_by_index(max_c+1);
  }
  else{
    if (max_c==m_chirprate_conf.length-1){
      c_min = m_transform->get_frequency_by_index(max_c-1);
      c_max = m_transform->get_frequency_by_index(max_c);

    }
    else{
      c_min = m_transform->get_frequency_by_index(max_c-1);
      c_max = m_transform->get_frequency_by_index(max_c+1);
    }
  }

  double dc = a_max_error/64;
  de_min = calc_single_point(i_freq, c_min+dc, m_treshold) - calc_single_point(i_freq, c_min-dc, m_treshold);
  de_max = calc_single_point(i_freq, c_max+dc, m_treshold) - calc_single_point(i_freq, c_max-dc, m_treshold);

  while(c_max-c_min>a_max_error){

    double c_new = (c_min+c_max)/2;
    double de_new = calc_single_point(i_freq, c_new+dc, m_treshold) -  calc_single_point(i_freq, c_new-dc, m_treshold);

    if(de_new*de_min<0){
      c_max = c_new;
      de_max = de_new;
    }
    else{
      c_min = c_new;
      de_min = de_new;
    }
  }
  
  return (c_min+c_max)/2;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine saves array to a TXT file. The results can be simply drawn using Gnuplot.
 *
 * @param [in] a_fname: Name of saved file.
 */
void roj_hough_transform :: save (char* a_fname){
  
  /* use roj_real_matrix method */
  m_transform->save(a_fname);
}
