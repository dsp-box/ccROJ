
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

  verify_config(a_frequency_conf);
  verify_config(a_chirprate_conf);
  m_frequency_conf = a_frequency_conf;
  m_chirprate_conf = a_chirprate_conf;

  if(a_treshold<0.0)
    call_error("treshold < 0");
  if(a_treshold>=1.0)
    call_error("treshold >= 1");
  m_treshold = a_treshold * a_energy->get_max();
    
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
  output_conf.x = m_frequency_conf;
  output_conf.y = m_chirprate_conf;
  roj_real_matrix* output = new roj_real_matrix(output_conf);
    
  roj_image_config energy_conf = m_energy->get_config();
  for(int t=0; t<energy_conf.x.length; t++){

    double time = m_energy->get_x_by_index(t);
    for(int f=0; f<energy_conf.y.length; f++){

      if(m_energy->m_data[t][f]<=m_treshold)
	continue;
      
      double frequency = m_energy->get_y_by_index(f);
      for(int c=0; c<m_chirprate_conf.length; c++){

	double chirp_rate = output->get_y_by_index(c);
	double init_frequency = frequency - chirp_rate * time;
	
	int k = output->get_index_by_x(init_frequency);
	if(output->check_in_x_index(k))
	  output->m_data[k][c] += m_energy->m_data[t][f];
      }
    }

    print_progress(t+1, energy_conf.x.length, "hough");
  }
  
  print_progress(0, 0, "hough");
  return output;
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

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine allows to compute a center of gravity. 
 *
 * @param [in] a_treshold: a treshold which allows to ignore some energy points (default is 0.0) 
 *
 * @return: estimated chirp-rate.
 */
double roj_hough_transform :: get_chirp_rate_gravity_center(double a_treshold){

  if (a_treshold<0.0)
    call_error("treshold < 0");
  if (a_treshold>=1.0)
    call_error("treshold >= 1");

  double treshold = a_treshold * m_transform->get_max();
  roj_image_config conf = m_transform->get_config();
  
  double p = 0.0;
  double e = 0.0;
    
  for(int k=0; k<conf.y.length; k++){
    double c = m_transform->get_y_by_index(k);    
    
    for(int n=0; n<conf.x.length; n++){

      if(m_transform->m_data[n][k]<treshold)
	continue;
      
      p += c * m_transform->m_data[n][k];
      e += m_transform->m_data[n][k];
    }
  }

  return p/e;
}

