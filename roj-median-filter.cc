/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-median-filter.hh"

/* ************************************************************************************************************************* */
/* internal function */
int segment_comparer (const void * a_arg1, const void * a_arg2){

  roj_median_segment* _a = *(roj_median_segment**)a_arg1;
  roj_median_segment* _b = *(roj_median_segment**)a_arg2;
  
  if(_a->val < _b->val)
    return -1;
  
  if(_a->val == _b->val)
    return 0;
  
  return 1;
}

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_median_filter. 
*
* @param [in] a_width: A width (like order, in samples) of filter mask.
* @param [in] a_height: A height (like order, in samples) of filter mask.
*/
roj_median_filter :: roj_median_filter (int a_width, int a_height){
  
  if(a_width<=0){
    call_warning("in roj_median_filter :: roj_median_filter");
    call_error("length <= 0");
  }
  m_width = 2 * a_width + 1;
  
  if(a_height<1){
    call_warning("in roj_median_filter :: roj_median_filter");
    call_error("height <= 0");
  }
  m_height = 2 * a_height + 1;
  
  m_counter = 0;
  m_length = 2 * m_height * m_width;
  m_buffer = new roj_median_segment* [m_length];
  for(int n=0; n<m_length; n++)
    m_buffer[n] = NULL;
}

/**
* @type: destructor
* @brief: This is an filter deconstructor. It also release internal buffers. 
*/
roj_median_filter :: ~roj_median_filter (){

  clean_buffer();
  delete [] m_buffer;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine performs median filtering for an array whose type is roj_real_array.
*
* @param [in] a_arr: A given array for filtering.
* @param [in] a_hop: A hop size between steps of filtering.
*
* @return: The filtered output array.
*/
roj_real_array* roj_median_filter :: filtering (roj_real_array* a_arr, int a_hop){
  
  if(a_arr == NULL){
    call_warning("in roj_median_filter :: filtering");
    call_error("arg is NULL");
  }

  if(a_hop<=0){
    call_warning("in roj_median_filter :: filtering");
    call_error("hop not positive");
  }
  
  double *buffer = new double[m_width];

  roj_array_config conf = a_arr->get_config();
  if(m_width>conf.length){
    call_warning("in roj_median_filter :: filtering");
    call_error("array is too short");
  }

  roj_array_config out_conf;
  out_conf.length = conf.length/a_hop;
  double new_delta = a_hop * a_arr->get_delta();
  out_conf.max = conf.min + new_delta*(out_conf.length-1);
  out_conf.min = conf.min;

  roj_real_array* output = new roj_real_array(out_conf);

  int byte_size = m_width * sizeof(double);
  int half = (m_width-1) / 2;

  for(int n=0; n<out_conf.length; n++){
    memset(buffer, 0x0, byte_size);
    
    if(a_hop*n<half){      
      int byte_size2 = (a_hop*n+half+1) * sizeof(double);
      memcpy(buffer, a_arr->m_data, byte_size2);
      qsort(buffer, a_hop*n+half+1, sizeof(double), value_comparer);
      output->m_data[n] = buffer[(a_hop*n+half+1)/2];
    }
    else
      if(a_hop*n>=conf.length-half){      
	int byte_size2 = (conf.length-a_hop*n+half) * sizeof(double);
	memcpy(buffer, &a_arr->m_data[a_hop*n-half-1], byte_size2);
	qsort(buffer, conf.length-a_hop*n+half, sizeof(double), value_comparer);
	output->m_data[n] = buffer[(conf.length-a_hop*n+half)/2];
      }
      else{
	memcpy(buffer, &a_arr->m_data[a_hop*n-half], byte_size);
	qsort(buffer, m_width, sizeof(double), value_comparer);
	output->m_data[n] = buffer[half];	
      }
    
    print_progress(n+1, out_conf.length, "median");
  }
  print_progress(0, 0, "median");

  delete [] buffer;
  return output;
}


/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine performs a smart median filtering for an array whose type is roj_real_array.
*
* @param [in] a_arr: A given array for filtering.
* @param [in] a_hop: A hop size between steps of filtering.
*
* @return: The filtered output array.
*/
roj_real_array* roj_median_filter :: smart_filtering (roj_real_array* a_arr, int a_hop){
  
  if(a_arr == NULL){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("arg is NULL");
  }

  if(a_hop<=0){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("hop not positive");
  }

  roj_array_config conf = a_arr->get_config();
  if(m_width*a_hop>conf.length){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("array is too short");
  }

  roj_real_array* output = new roj_real_array(conf);

  int half = (m_width-1) / 2;
  for(int h=0; h<a_hop; h++){
    for(int n=h; n<conf.length; n+=a_hop){
      clean_buffer_flags();

      for(int k=-half;k<=half; k++){
	
	int index = n+k*a_hop;
	if (index < 0) continue;
	if (index >= conf.length) continue;
	
	add_segment(a_arr->m_data[index], index, 1);
      }     
      
      remove_false_segments();
      order_buffer_segments();
      output->m_data[n] = get_median_value();

      print_progress(h*conf.length+n+1, a_hop*conf.length, "median");
    }    
  }
  print_progress(0, 0, "median");
  clean_buffer();
  
  return output;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine performs a smart median filtering for a signal whose type is roj_complex_signal.
*
* @param [in] a_sig: A given signal for filtering.
* @param [in] a_hop: A hop size between steps of filtering.
*
* @return: The filtered output signal.
*/
roj_complex_signal* roj_median_filter :: smart_filtering (roj_complex_signal* a_sig, int a_hop){

  if(a_sig == NULL){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("arg is NULL");
  }

  if(a_hop<=0){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("hop not positive");
  }

  roj_signal_config sig_conf = a_sig->get_config();
  roj_array_config arr_conf = convert_config(sig_conf);
  
  roj_real_array *re_orr = NULL;
  if(a_sig->check_real()){
    roj_real_array *re_arr = a_sig->get_real();
    re_orr = smart_filtering (re_arr, a_hop);
    delete re_arr;
  }
  else
    re_orr = new roj_real_array(arr_conf);
  
  roj_real_array *im_orr = NULL;
  if(a_sig->check_imag()){
    roj_real_array *im_arr = a_sig->get_imag();
    roj_real_array *im_orr = smart_filtering (im_arr, a_hop);
    delete im_arr;
  }

  roj_complex_signal* out_signal = new roj_complex_signal(re_orr, im_orr);

  delete re_orr;
  if (im_orr)
    delete im_orr;

  return out_signal;
}

/**
* @type: method
* @brief: This routine performs a smart median filtering for an image whose type is roj_real_matrix.
*
* @param [in] a_arr: A given image for filtering.
* @param [in] a_hop: A horizontal hop size between steps of filtering.
* @param [in] a_hop: A vertical hop size between steps of filtering.
*
* @return: The filtered output image.
*/
roj_real_matrix* roj_median_filter :: smart_filtering (roj_real_matrix* a_matrix, int a_hop, int a_vop){

  if(a_matrix == NULL){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("arg is NULL");
  }

  if(a_hop<=0){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("hop not positive");
  }

  if(a_vop<=0){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("hop not positive");
  }

  roj_image_config conf = a_matrix->get_config();
  if(m_width*a_hop>conf.x.length){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("image width is too short");
  }

  if(m_height*a_vop>conf.y.length){
    call_warning("in roj_median_filter :: smart_filtering");
    call_error("image height is too short");
  }

  roj_real_matrix* output = new roj_real_matrix(conf);

  int half = (m_width-1) / 2;
  int valf = (m_height-1) / 2;

  print_progress(0, conf.x.length, "median");
  for(int h=0; h<a_hop; h++){
    for(int v=0; v<a_vop; v++){
      
      for(int x=h; x<conf.x.length; x+=a_hop){
	for(int y=v; y<conf.y.length; y+=a_vop){
	  clean_buffer_flags();

	  for(int n=-half;n<=half; n++){

	    int hindex = x+n*a_hop;
	    if (hindex < 0) continue;
	    if (hindex >= conf.x.length) continue;

	    for(int k=-valf;k<=valf; k++){
	
	      int vindex = y+k*a_vop;
	      if (vindex < 0) continue;
	      if (vindex >= conf.y.length) continue;
	      
	      add_segment(a_matrix->m_data[hindex][vindex], hindex, vindex);
	    }     
	  }
	  
	  remove_false_segments();
	  order_buffer_segments();
	  output->m_data[x][y] = get_median_value();
	}

	print_progress((v+h*a_vop)*conf.x.length+x+1, a_vop*a_hop*conf.x.length, "median");
      }
      
    }
  }
  print_progress(0, 0, "median");
  clean_buffer();
  
  return output;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine allows to reset.
*/
void roj_median_filter :: reset (){

  clean_buffer ();
}

/**
* @type: private
* @brief: This routine is internal private procedure for flags cleanning.
*/
void roj_median_filter :: clean_buffer_flags (){
  
  for(int n=0; n<m_counter; n++)
    m_buffer[n]->flag = false;  
}

/**
* @type: private
* @brief: This routine is internal private procedure for buffer cleanning.
*/
void roj_median_filter :: clean_buffer (){

  for(int n=0; n<m_length; n++)
    if(m_buffer[n] != NULL){
      delete m_buffer[n];
      m_buffer[n] = NULL;
    }
  
  m_counter = 0;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This routine is internal private procedure for ordering.
*/
void roj_median_filter :: order_buffer_segments (){

  int first_free = -1;
  int new_counter = 0;
  for(int n=0; n<m_counter; n++){
    if(m_buffer[n]!=NULL)
      new_counter++;
    
    if(m_buffer[n]==NULL and first_free<0)
      first_free = n;
  
    if(m_buffer[n]!=NULL and first_free>=0){
      m_buffer[first_free] = m_buffer[n];
      m_buffer[n] = NULL;
      first_free++;
    }
  }

  m_counter = new_counter;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This routine is internal private procedure for special segment manipulation - removing.
*/
void roj_median_filter :: remove_false_segments (){
  
  for(int n=0; n<m_counter; n++){
    if(m_buffer[n]->flag==false){
      
      delete m_buffer[n];
      m_buffer[n] = NULL;
    }
  }
}

/**
* @type: private
* @brief: This routine is internal private procedure for special segment manipulation - creating and adding.
*
* @param [in] a_value: Sample value.
* @param [in] a_xcoor: X coordinate.
* @param [in] a_ycoor: Y coordinate.
*/
void roj_median_filter :: add_segment (double a_value, int a_xcoor, int a_ycoor){
 
  for(int n=0; n<m_counter; n++){
    
    if (m_buffer[n]->x == a_xcoor){
      if (m_buffer[n]->y == a_ycoor){
	m_buffer[n]->flag = true;
	return;
      }
    }
  }
  
  if(m_counter>=m_length){
    call_warning("in roj_median_filter :: add_segment");
    call_error("segment buffer is full");
  }

  roj_median_segment* segment = new roj_median_segment;
  segment->val = a_value;
  segment->flag = true;
  segment->x = a_xcoor;
  segment->y = a_ycoor;
  m_buffer[m_counter] = segment;
  m_counter++;
}

/**
* @type: private
* @brief: This routine is internal private procedure - get median value from an internal  buffer.
*
* @return: The median value.
*/
double roj_median_filter :: get_median_value (){
  
  qsort(m_buffer, m_counter, sizeof(roj_median_segment*), &segment_comparer);
  return m_buffer[m_counter/2]->val;  
}

