/* *************************************************** *
 * This file is a part of ccROJ project (version 0-48) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

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

  if (a_energy==NULL){
    call_warning("in roj_hough_transform :: roj_hough_transform");
    call_error("arg is null");
  }

  verify_config(a_frequency_conf);
  verify_config(a_chirprate_conf);
  m_frequency_conf = a_frequency_conf;
  m_chirprate_conf = a_chirprate_conf;

  if(a_treshold<0.0){
    call_warning("in roj_hough_transform :: roj_hough_transform");
    call_error("treshold < 0");
  }

  if(a_treshold>=1.0){
    call_warning("in roj_hough_transform :: roj_hough_transform");
    call_error("treshold >= 1");
  }

  m_treshold = a_treshold * a_energy->get_max();
  m_transform = reassign_energy(a_energy); 
}

/**
 * @type: constructor
 * @brief: This is a constructor of roj_hough_transform based on Fourier spectrum. 
 *
 * @param [in] a_sdelay: 
 * @param [in] a_ifreq: 
 * @param [in] a_energy: an energy distribution for transformation.
 * @param [in] a_frequency_conf:  
 * @param [in] a_chirprate_conf: 
 * @param [in] a_treshold: a treshold which allows to accelerate computing (default is 0.0) 
 */
roj_hough_transform :: roj_hough_transform(roj_real_matrix* a_sdelay, roj_real_matrix* a_ifreq, roj_real_matrix* a_energy, roj_array_config a_frequency_conf, roj_array_config a_chirprate_conf, double a_treshold){

  if (a_sdelay==NULL){
    call_warning("in roj_hough_transform :: roj_hough_transform");
    call_error("arg is null");
  }

  if (a_ifreq==NULL){
    call_warning("in roj_hough_transform :: roj_hough_transform");
    call_error("arg is null");
  }

  if (a_energy==NULL){
    call_warning("in roj_hough_transform :: roj_hough_transform");
    call_error("arg is null");
  }

  verify_config(a_frequency_conf);
  verify_config(a_chirprate_conf);
  m_frequency_conf = a_frequency_conf;
  m_chirprate_conf = a_chirprate_conf;

  if(a_treshold<0.0){
    call_warning("in roj_hough_transform :: roj_hough_transform");
    call_error("treshold < 0");
  }

  if(a_treshold>=1.0){
    call_warning("in roj_hough_transform :: roj_hough_transform");
    call_error("treshold >= 1");
  }

  m_treshold = a_treshold * a_energy->get_max();    
  m_transform = reassign_energy(a_sdelay, a_ifreq, a_energy); 
}

/**
 * @type: destructor
 * @brief: This is a spectrum deconstructor. It also release memory for lines. 
 */
roj_hough_transform :: ~roj_hough_transform (){

  delete m_transform;
  //  delete m_energy;
}

/* ************************************************************************************************************************* */
/**
 * @type: private
 * @brief: This routine allows to compute the Hough transform as roj_real_matrix. 
 *
 * @return: an energy of the Hough transform.
 */
roj_real_matrix* roj_hough_transform :: reassign_energy(roj_real_matrix* a_energy){
     
  roj_image_config output_conf;
  output_conf.x = m_frequency_conf;
  output_conf.y = m_chirprate_conf;
  roj_real_matrix* output = new roj_real_matrix(output_conf);
    
  roj_image_config energy_conf = a_energy->get_config();
  for(int t=0; t<energy_conf.x.length; t++){

    double time = a_energy->get_x_by_index(t);
    for(int f=0; f<energy_conf.y.length; f++){

      if(a_energy->m_data[t][f]<=m_treshold)
	continue;
      
      double frequency = a_energy->get_y_by_index(f);
      for(int c=0; c<m_chirprate_conf.length; c++){

	double chirp_rate = output->get_y_by_index(c);
	double init_frequency = frequency - chirp_rate * time;
	
	int k = output->get_index_by_x(init_frequency);
	// printf("%d %d %g\n", k, c, a_energy->m_data[t][f]);
	
	if(output->check_in_x_index(k))
	  output->m_data[k][c] += a_energy->m_data[t][f];
      }
    }

    print_progress(t+1, energy_conf.x.length, "hough");
  }
  
  print_progress(0, 0, "hough");
  return output;
}


/**
 * @type: private
 * @brief: This routine allows to compute the Hough transform as roj_real_matrix. 
 *
 * @return: an energy of the Hough transform.
 */
roj_real_matrix* roj_hough_transform :: reassign_energy(roj_real_matrix* a_sdelay, roj_real_matrix* a_ifreq, roj_real_matrix* a_energy){

  roj_image_config output_conf;
  output_conf.x = m_frequency_conf;
  output_conf.y = m_chirprate_conf;
  roj_real_matrix* output = new roj_real_matrix(output_conf);

  roj_image_config energy_conf = a_energy->get_config();

  if (!a_sdelay->compare_config(energy_conf)){
    call_warning("in roj_hough_transform :: reassign_energy");    
    call_error("images are not compact");
  }

  if (!a_ifreq->compare_config(energy_conf)){
    call_warning("in roj_hough_transform :: reassign_energy");    
    call_error("images are not compact");
  }

  for(int t=0; t<energy_conf.x.length; t++){

    double time = a_energy->get_x_by_index(t);
    for(int f=0; f<energy_conf.y.length; f++){
      
      if(a_energy->m_data[t][f]<=m_treshold)
	continue;

      double ctime = time + a_sdelay->m_data[t][f];
      double ifreq = a_ifreq->m_data[t][f];

      for(int c=0; c<m_chirprate_conf.length; c++){

	double chirp_rate = output->get_y_by_index(c);
	double init_frequency = ifreq - chirp_rate * ctime;
	
	int k = output->get_index_by_x(init_frequency);
	if(output->check_in_x_index(k))
	  output->m_data[k][c] += a_energy->m_data[t][f];
      }
    }

    print_progress(t+1, energy_conf.x.length, "hough");
  }  
  
  print_progress(0, 0, "hough");
  return output;
}
