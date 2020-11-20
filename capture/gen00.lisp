(declaim (optimize 
	  (safety 3)
	  (speed 0)
	  (debug 3)))

(eval-when (:compile-toplevel :execute :load-toplevel)
     (ql:quickload "cl-cpp-generator2")
     (ql:quickload "cl-ppcre"))

(in-package :cl-cpp-generator2)



(setf *features* (union *features* `(:gui)))

(setf *features* (set-difference *features*
				 '(:gui)))

(progn
  (defparameter *source-dir* #P"/home/martin/stage/build_pluto_firmware/capture/source/")
  
  (defparameter *day-names*
    '("Monday" "Tuesday" "Wednesday"
      "Thursday" "Friday" "Saturday"
      "Sunday"))
  
  (progn
    ;; collect code that will be emitted in utils.h
    (defparameter *utils-code* nil)
    (defun emit-utils (&key code)
      (push code *utils-code*)
      " ")
    (defparameter *global-code* nil)
    (defun emit-global (&key code)
      (push code *global-code*)
      " "))
  (progn
    
    (defparameter *module-global-parameters* nil)
    (defparameter *module* nil)
    
    (defun logprint (msg &optional rest)
      `(do0
	" "
	#-nolog
	(do0
					;("std::setprecision" 3)
	 (<< "std::cout"
	     ;;"std::endl"
	     ("std::setw" 10)
	     (dot ("std::chrono::high_resolution_clock::now")
		  (time_since_epoch)
		  (count))
					;,(g `_start_time) 
	     
	     (string " ")
	     ("std::this_thread::get_id")
	     (string " ")
	     __FILE__
	     (string ":")
	     __LINE__ 
	     (string " ")
	     __func__
	     (string " ")
	     (string ,msg)
	     (string " ")
	     ,@(loop for e in rest appending
		    `(("std::setw" 8)
					;("std::width" 8)
		      (string ,(format nil " ~a='" (emit-c :code e)))
		      ,e
		      (string "::")
		      (dot (typeid ,e)
			   (name))
		      (string "'")))
	     #+nil,@(loop for e in rest appending
		    `(("std::setw" 8)
					;("std::width" 8)
		      (string ,(format nil " ~a='" (emit-c :code e)))
		      ,e
		      (string "'")))
	     "std::endl"
	     "std::flush"))))
    (defun iio (code)
      `(unless ,code
	 ,(logprint (format nil "~a" code))))

    (defun make-filter (pass &key (poles 2) (fc 0.01))
    ;; https://www.analog.com/media/en/technical-documentation/dsp-book/dsp_book_Ch20.pdf
    (let ((l `((low 2 0.01
		   :a (8.66487e-4 1.732678e-3 8.663387e-4)
		   :b (1.919129e0 -9.225943e-1))
	       (low 2 0.025
		   :a (5.112374e-3 1.022475e-2 5.112374e-3)
		   :b (1.797154 -8.176033e-1))
	       (low 2 0.05
		   :a (1.868823e-2 3.737647e-2 1.868823e-2)
		   :b (1.593937 -6.686903e-1))
	       (low 2 0.075
		   :a (3.869430e-2 7.738860e-2 3.869430e-2)
		   :b (1.392667 -5.474446e-1))
	      (low 2 0.1
		   :a (6.372802e-2 1.274560e-1 6.372802e-2)
		   :b (1.194365e0 -4.492774e-1))
	      (low 2 0.4
		   :a (6.362308e-1 1.272462e0 6.362308e-1)
		   :b (-1.125379e0 -4.195441e-1))
	      (high 2 0.01
		    :a (9.567529e-1 -1.913506e0 9.567529e-1)
		    :b (1.911437e0 -9.155749e-1))
	      (high 2 0.40
		    :a (6.372801e-2 -1.274560e-1 6.372801e-2)
		    :b (-1.194365e0 -4.492774e-1)))))
     `(lambda (xn)
	(declare (type float xn)
		 (values float))
	,@(remove-if #'null
		     (loop for e in l
			   collect
			   (destructuring-bind (pass_ poles_ fc_ &key a b) e
			     (when (and (eq pass pass_)
					(eq poles poles_)
					(eq fc fc_))
			       `(do0
				(comments ,(format nil "filter_~a_~a_~2,'0d_real" poles pass (floor (* 100 fc))))
				
				(let ((yn1 0s0)
				      (yn2 0s0)
				      
				      (xn1 0s0)
				      (xn2 0s0)
				      
				      (yn (+ (* ,(elt a 0) xn)
					      (* ,(elt a 1) xn1)
					      (* ,(elt a 2) xn2)
					      (* ,(elt b 0) yn1)
					      (* ,(elt b 1) yn2))))
				  (declare (type "static float" yn1 yn2 xn1 xn2 xn3)
					   (type float yn))

				  (setf 
					xn2 xn1
					xn1 xn
					yn2 yn1
					yn1 yn)
				  (return yn))))))))))
    ;; (,(make-filter 'low :fc 0.01) (aref ,(g `_iqdata) (+ 0 (* 2 i))))
    
    (defun emit-globals (&key init)
      (let ((l `((_start_time ,(emit-c :code `(typeof (dot ("std::chrono::high_resolution_clock::now")
							   (time_since_epoch)
							   (count)))))
		 ,@(loop for e in *module-global-parameters* collect
			(destructuring-bind (&key name type default)
			    e
			  (declare (ignorable default))
			  `(,name ,type))))))
	(if init
	    `(curly
	      ,@(remove-if
		 #'null
		 (loop for e in l collect
		      (destructuring-bind (name type &optional value) e
			(declare (ignorable type))
			(when value
			  `(= ,(format nil ".~a" (elt (cl-ppcre:split "\\[" (format nil "~a" name)) 0)) ,value))))))
	    `(do0
	      (include <chrono>)
	      (defstruct0 State
		  ,@(loop for e in l collect
 			 (destructuring-bind (name type &optional value) e
			   (declare (ignorable value))
			   `(,name ,type))))))))
    (defun define-module (args)
      "each module will be written into a c file with module-name. the global-parameters the module will write to will be specified with their type in global-parameters. a file global.h will be written that contains the parameters that were defined in all modules. global parameters that are accessed read-only or have already been specified in another module need not occur in this list (but can). the prototypes of functions that are specified in a module are collected in functions.h. i think i can (ab)use gcc's warnings -Wmissing-declarations to generate this header. i split the code this way to reduce the amount of code that needs to be recompiled during iterative/interactive development. if the module-name contains vulkan, include vulkan headers. if it contains glfw, include glfw headers."
      (destructuring-bind (module-name global-parameters module-code) args
	(let ((header ()))
	  #+nil (format t "generate ~a~%" module-name)
	  (push `(do0
		  " "
		  (include "utils.h")
		  " "
		  (include "globals.h")
		  " "
		  ;(include "proto2.h")
		  " ")
		header)
	  (unless (cl-ppcre:scan "main" (string-downcase (format nil "~a" module-name)))
	    (push `(do0 "extern State state;")
		  header))
	  (push `(:name ,module-name :code (do0 ,@(reverse header) ,module-code))
		*module*))
	(loop for par in global-parameters do
	     (destructuring-bind (parameter-name
				  &key (direction 'in)
				  (type 'int)
				  (default nil)) par
	       (declare (ignorable direction))
	       (push `(:name ,parameter-name :type ,type :default ,default)
		     *module-global-parameters*))))))
  (defun g (arg)
    `(dot state ,arg))

  (let*  ()
    
    
    
    (define-module
       `(base ((_code_version :type "std::string")
	       (_code_repository :type "std::string")
	       (_code_generation_time :type "std::string")
	       (_code_author :type "std::string")
	       (_code_license :type "std::string")
		 )
	      (do0
	       
	    
		    (include <iostream>
			     <chrono>
			     <thread>
			     
			     )

		     (do0
		     ;; https://raw.githubusercontent.com/analogdevicesinc/libiio/master/examples/ad9361-iiostream.c
		      (include <iio.h>
			       <array>
			       <math.h>
			       <complex>
			       ;<fftw3.h>
					;<omp.h>
			       <unistd.h>
			       <algorithm>
			       <vector>)

		      

		      (include "vis_01_server.hpp")
		      
		     "#define MHz(x) ((long long)(x*1000000.0 + .5))"
		     "#define GHz(x) ((long long)(x*1000000000.0 + .5))"
		     "enum iodev{RX,TX};"
		     (defstruct0 stream_cfg
			 (bw_hz "long long")
		       (fs_hz "long long")
		       (lo_hz "long long")
		       (rfport "const char*")
		       ))
		    "using namespace std::chrono_literals;"
		    " "

		    (split-header-and-code
		     (do0
		      "// header"
		      )
		     (do0
		      "// implementation"

		      ))
		    "State state;"

		    (space "struct __attribute__ ((packed)) sdriq_header_t"
			   (progn
			     "uint32_t samplerate;"
			     "uint64_t center_frequency;"
			     "uint64_t timestamp;"
			     "uint32_t samplesize;"
			     "uint32_t padding;"
			     "uint32_t crc32;"))
		    
		    #+nil
		    (defstruct0 sdriq_header_t 
		      (samplerate uint32_t)
		      (center_frequency uint64_t)
		      (timestamp uint64_t)
		      (samplesize uint32_t)
		      (padding uint32_t)
		      (crc32 uint32_t)
		      )
		    
		    (defun main (argc argv
				 )
		      (declare (type int argc)
			       (type char** argv)
			       (values int))

		      (do0
		       (setlocale LC_ALL (string ""))
		       (setf ,(g `_code_version)
                             (string ,(let ((str (with-output-to-string (s)
                                                   (sb-ext:run-program "/usr/bin/git" (list "rev-parse" "HEAD") :output s))))
					(subseq str 0 (1- (length str))))))

                       (setf
			,(g `_code_repository) (string ,(format nil "https://github.com/plops/build_pluto_firmware/tree/master/capture"))
			,(g `_code_author )(string "Martin Kielhorn <kielhorn.martin@gmail.com>")
			,(g `_code_license )(string "GPL v3")
			,(g `_code_generation_time) 
			(string ,(multiple-value-bind
                                       (second minute hour date month year day-of-week dst-p tz)
                                     (get-decoded-time)
                                   (declare (ignorable dst-p))
                                   (format nil "~2,'0d:~2,'0d:~2,'0d of ~a, ~d-~2,'0d-~2,'0d (GMT~@d)"
                                           hour
                                           minute
                                           second
                                           (nth day-of-week *day-names*)
                                           year
                                           month
                                           date
                                           (- tz)))))

                       (setf ,(g `_start_time) (dot ("std::chrono::high_resolution_clock::now")
                                                    (time_since_epoch)
                                                    (count))))

		      ,(logprint "start" `(argc (aref argv 0)
						))
		      ,@(loop for e in `(_start_time _code_version _code_repository
						     _code_generation_time _code_author
						     _code_license)
			      collect
			      (logprint ""
					`(,(g e))))

		
		      
		      (do0
		
		       ;;https://analogdevicesinc.github.io/libiio/master/libiio/index.html
		       (let ((ctx (iio_create_default_context)))
			 (do0
			  (let ((major (uint 0))
				(minor (uint 0))
				(git_tag))
			    (declare (type (array char 8) git_tag))
			    (iio_library_get_version &major &minor git_tag)
			   ,(logprint "" `(major minor git_tag)))
			  (unless ctx
			    ,(logprint "create_default" `(ctx)))
			  ,(logprint ""
				     `((iio_context_get_devices_count ctx)))
			  #+nil (let ((n (iio_context_get_devices_count ctx))
				(dev ("std--array<iio_device*,n>")))
			    
			   (dotimes (i n)
			     (setf (aref dev i)
				   (iio_context_get_device ctx i)))
			    )
			  ,@(loop for (e f) in `((rx cf-ad9361-lpc)
						 (phy ad9361-phy))
				  collect
				  `(let ((,e (iio_context_find_device ctx (string ,f))))
				     ,(iio e)
				     ,(logprint (format nil "~a" e)
						`((iio_device_get_attrs_count ,e)))))
			  (let ((rx_lo_freq 2467000000ULL)
				(rx_lo_freq_MHz (/ rx_lo_freq 1e6)))
			   ;(comments "rx lo freq to 2.42GHz")
			   ,(logprint "" `(rx_lo_freq_MHz))
			   (iio_channel_attr_write_longlong
			       (iio_device_find_channel phy
							(string "altvoltage0")
							true)
			       (string "frequency")
			       rx_lo_freq
			       ))
			  (let ((rx_rate 61440000UL
					 ;20000000
					 )
				(rx_rate_MSps (/ rx_rate 1e6)))
			   ;(comments "rx baseband rate 5MSPS")
			   ,(logprint "" `(rx_rate_MSps))
			   (iio_channel_attr_write_longlong
			    (iio_device_find_channel phy
						     (string "voltage0")
						     false)
			    (string "sampling_frequency")
			    rx_rate
			    ))
			  ,(logprint "get channels count..")
			  (let ((n_chan (iio_device_get_channels_count rx))
				)
			    ,(logprint "" `(n_chan))
			    ,@(loop for e in `(rx_i rx_q) and i from 0 collect
				    `(let ((,e (iio_device_get_channel rx ,i)))
				       ,(logprint (format nil "~a ~a" e i) `((iio_channel_get_attrs_count ,e)))))
			    (iio_channel_enable rx_i)
			    (iio_channel_enable rx_q)
			    ,(logprint "iq channels enabled")
			    (let (("const nbuf" (* 48 64 4096) ;; 48 64
						;4096
						))
			      ,(logprint "create buffer")
			      (let ((rxbuf (iio_device_create_buffer rx nbuf false))
				    (sample_and_compute_start
				      (dot ("std::chrono::high_resolution_clock::now")
					   (time_since_epoch)))
				   (sample_start sample_and_compute_start)
				   (compute_start sample_and_compute_start))
				(do0
				 #+nil(do0
				  (dot ,(g `_iq_out) (push_back 42
								))
						(dot ,(g `_iq_out) (push_back 43))
						)
				 (let ((server_thread (run_server_in_new_thread)))
				   ,(logprint "server started")
				   )
				 (let ((count 0))
				  (while true ;
				   ;dotimes (j 1)

				   ; ,(logprint "308" `(count))
				    
				   (setf sample_start (dot ("std::chrono::high_resolution_clock::now")
							   (time_since_epoch)
							   ))
				   (let (
					 (nbytes (iio_buffer_refill rxbuf))
					 (time_now (dot ("std::chrono::high_resolution_clock::now")
							(time_since_epoch)
							))
					 (sample_dur (dot (- time_now
							     sample_start)
							  (count)))
					 (step (iio_buffer_step rxbuf))
					 (end (iio_buffer_end rxbuf))
					 (start (static_cast<uint8_t*>
						 (iio_buffer_first rxbuf rx_i)))
					 (i 0)
					;  (rate_MSamp_per_sec (/ (* 1d3 nbuf) dur))
					 )
				     #+nil(do0
				      (comments "open server and wait for client to obtain rxbuf"
						"sdriq file format"
						"https://github.com/f4exb/sdrangel/tree/master/plugins/samplesource/fileinput")
				      (let ((header (sdriq_header_t (curly rx_rate
									   rx_lo_freq
									   0
									   16
									   0
									   895232605  ;; crc computed with rescuesdriq
									   ))))
					,(logprint "" `((sizeof header)))
					))
				     (do0
				      (setf compute_start
					    (dot ("std::chrono::high_resolution_clock::now")
						 (time_since_epoch)
						 ))
				      (let ((ma 0)
					    (old 0s0)
					    (trig 0)
					    (trig1 0)
					    ;(outiq (std--vector<int16_t>))
					    )
				       (do0
				       
					;"#pragma omp parallel"

					"#pragma GCC ivdep"
					#-nil
					(for ((= "uint8_t* p" start)
					      (< p end)
					      (incf p step))
					     (let ((si (aref (reinterpret_cast<int16_t*> p) 0))
						   (sq (aref (reinterpret_cast<int16_t*> p) 1))
						   (m (+ (* si si)
							 (* sq sq)))
						   (mlow
						     (,(make-filter 'low :fc 0.01) m)
						     ;(filter_2_low_01_real m)
						     )))
					     (when (< ma mlow)
					       (setf ma mlow))
					     (when (and (== trig 0)
							(< old 4000) ;; trigger on positive edge
							(<= 4000 mlow))
					       (setf trig i))
					     #+nil (when (< trig 0)
					       ;; get a few more samples after trig1
					       (do0
						
						(dot ,(g `_iq_out) (push_back si))
						(dot ,(g `_iq_out) (push_back sq))
						;(outiq.push_back si)
						;(outiq.push_back sq)
						(incf trig)
						#+nil (when (== trig 0)
							(dotimes (i (* 16 4096))
							  (outiq.push_back 0)
							  (outiq.push_back 0)
							  ))))
					     
					     (when (< 0 trig)
					        #+nil (do0
						(outiq.push_back si)
						(outiq.push_back sq))
					      #-nil (do0
						(dot ,(g `_iq_out) (push_back si))
						(dot ,(g `_iq_out) (push_back sq))
						)
					       (when (and (< 2000 old) ;; trigger1 on negative edge (only when trig0 has been found)
							  (<= mlow 2000))
						 (setf trig1 i)
						 (let ((pulse_ms (/ (- trig1 trig) 61.44e3)))
						  ,(logprint "" `(ma trig trig1 pulse_ms)))
						 ;; read a few more samples (if trig is set negative here)
						 (setf trig 0)
						 ))
					     (incf i)
					     (setf old mlow)
					     )
					#+nil ,(logprint "finished" `(ma trig trig1 ;(outiq.size)
								   )))
					
					)
				      #+nil (create_server ;(reinterpret_cast<uint8_t*> &header)
						      ; (sizeof header)
						       (reinterpret_cast<uint8_t*> (outiq.data))
						       (* 2 (outiq.size))
					;(+ (* 4 trig) start) (* 4 (- trig1 trig))
						       #+nil (- nbytes
									       (* 4 trig)
									       ))
				     )
				     (let ((compute_end (dot ("std::chrono::high_resolution_clock::now")
							     (time_since_epoch)
							     ))
					   (compute_dur (dot (- compute_end compute_start)
							     (count)))
					   (compute_samp_dur (dot (- compute_end
								     sample_start)
								  (count)))
					   (compute_perc (/ (* 100 compute_dur)
							    compute_samp_dur))
					   (sample_perc (/ (* 100 sample_dur)
							   compute_samp_dur)))
				       
				       (do0
					(incf count)

					)
				       
				       
				       )
				     

				     )
				   ,(logprint "374" `(count nbytes compute_perc sample_perc))
				   )))
			       )))

			  
			  )
			 (do0
			  (setf ,(g `_server_keep_running ) false)
			  (when (server_thread.joinable)
			    (server_thread.join)))
			 (iio_context_destroy ctx)

			 			 ))
		      
		      (return 0)))))

    (define-module
	`(server ((_iq_out :type "tsqueue<uint16_t>")
		  (_server_keep_running :type bool))
	      (do0
	       (include <sys/types.h>
			<sys/socket.h>
			<netinet/in.h>
			<unistd.h>
			<arpa/inet.h>)
	    
		    (include <iostream>
			     <chrono>
			     <thread>
			     
			     )

		    

		    ;"tsqueue<uint16_t> iq_out;"

		    (comments "http://www.linuxhowtos.org/data/6/server.c")

		    
		    
		    (defun create_server (;header nbytes_header
					;  buf nbytes
					  )
		      (declare (type uint8_t* buf header)
			       (type size_t nbytes_header nbytes))
		      (setf ,(g `_server_keep_running ) true)
		      (let ((fd (socket AF_INET SOCK_STREAM 0))
			    (portno 1234)
			    (server_addr "{}")
			    (client_addr "{}"))
			(declare (type "struct sockaddr_in" server_addr client_addr))
			(setf server_addr.sin_family AF_INET
			      server_addr.sin_addr.s_addr INADDR_ANY
			      server_addr.sin_port (htons portno))
			,(logprint "attempt bind" `(portno))
			(when (<  (bind fd
					("reinterpret_cast<struct sockaddr*>" &server_addr)
					(sizeof server_addr))
				  0)
			  ,(logprint "bind failed")
			  )
			(listen fd 5)
			,(logprint "initiate accept")
			(while ,(g `_server_keep_running)
			 (let ((client_len (sizeof client_addr))
			       (fd1 (accept fd
					    ("reinterpret_cast<struct sockaddr*>" &client_addr)
					    &client_len)))
			   #-nil (when (< fd1 0)
				   ,(logprint "accept failed")
				   )
			   (let ((client_addr_buffer))
			     (declare (type (array char INET_ADDRSTRLEN) client_addr_buffer))
			     (inet_ntop AF_INET &client_addr.sin_addr client_addr_buffer (sizeof client_addr_buffer))
			    ,(logprint "accept" `(client_addr_buffer
						  client_addr.sin_port)))

			   #+nil
			   (let ((nh (write fd1 header nbytes_header)))
			     (when (< nh 0)
			       ,(logprint "writing header failed"))
			     )
			   ,(logprint "enter transmission loop" `((dot ,(g `_iq_out) (empty))
								  (dot ,(g `_iq_out) (front))
								  (dot ,(g `_iq_out) (back))))

			   (do0 (when (dot ,(g `_iq_out) (empty))
				  (dot ,(g `_iq_out) (wait_while_empty)))
				;,(logprint "attempt to write")
				(while (not (dot ,(g `_iq_out) (empty)))
				       (let ((msg (dot ,(g `_iq_out) (pop_front))))
					 ;;,(logprint "prepare for write" `(msg))
					 (let ((n (write fd1 (reinterpret_cast<uint8_t*> &msg) 2)))
					   (when (< n 0)
					     ,(logprint "write failed"))
					   ;,(logprint "bytes written: " `(n))
					   ))))
			   (close fd1)))
			(do0 
			 (close fd))))

		    (defun run_server_in_new_thread ()
		      (declare (values "std::thread"))
		      ,(logprint "attempt to start a thread with the server")
		      (return (std--thread create_server))
		      )
		    )))

    
    
    (define-module
       `(tsqueue () ;; thread safe queue
	      (do0
	       
	       
		    (include <iostream>
			     <chrono>
			     <thread>
			     <deque>
			     <mutex>
			     <condition_variable>)

		    #+nil
		    (include <boost/asio.hpp>
			     <boost/asio/ts/buffer.hpp>
			     <boost/asio/ts/internet.hpp>)
	
		    " "

		    (split-header-and-code
		     (do0
		      "// header"
		      (defclass+ (tsqueue :template "typename T") ()
			"public:"
			"tsqueue() = default;"
			"tsqueue(const tsqueue<T>&) = delete;"
			(defmethod ~tsqueue ()
			  (declare (virtual)
				   (values :constructor))
			  (clear))
			#+nil(defmethod empty ()
				    (declare (values bool)
					     (const))
				    (let ((lock (std--scoped_lock mux_deq)))
				      (return (dot deq (empty)))))
			,@(loop for e in
				`((front "const T&")
				  (back "const T&")
				  (empty bool)
				  ;(size size_t)
				  (clear void)
				  (pop_front "T" :code (let ((el (std--move
								  (deq.front))))
							 (deq.pop_front)
							 (return el)))
				  (pop_back "T" :code (let ((el (std--move
								  (deq.back))))
							 (deq.pop_back)
							(return el)))
				  
				  (push_back void
					    :params ((item "const T&"))
					    :code (do0
						   (deq.emplace_back
						    (std--move item))
						   (let ((ul (std--unique_lock<std--mutex> mux_blocking)))
						     (cv_blocking.notify_one))))
				  (push_front void
					    :params ((item "const T&"))
					    :code (do0
						   (deq.emplace_front
						    (std--move item))
						   (let ((ul (std--unique_lock<std--mutex> mux_blocking)))
						     (cv_blocking.notify_one))))
				  (wait_while_empty void
					    :params ()
					    :code (while (empty)
						   (let ((ul (std--unique_lock<std--mutex> mux_blocking)))
						     (cv_blocking.wait ul)))))
				collect
				(destructuring-bind (name type &key params code)
				    e
				 `(defmethod ,name ,(mapcar #'first params)
				    (declare (values ,type)
					     ,@(loop for (e f) in params
						     collect
						     `(type ,f ,e)))
				    (let ((lock (std--scoped_lock mux_deq)))
				      ,(if code
					   `(do0
					     ,code)
					   `(return (dot deq (,name))))))))
			
			"protected:"
			"std::mutex mux_deq;"
			"std::mutex mux_blocking;"
			"std::condition_variable cv_blocking;"
			"std::deque<T> deq;")
		      )
		     (do0
		      "// implementation"
		      ))
		    )))

    )
  
  (progn
    (progn ;with-open-file
      #+nil (s (asdf:system-relative-pathname 'cl-cpp-generator2
					(merge-pathnames #P"proto2.h"
							 *source-dir*))..
	 :direction :output
	 :if-exists :supersede
	 :if-does-not-exist :create)
      #+nil (format s "#ifndef PROTO2_H~%#define PROTO2_H~%~a~%"
		    (emit-c :code `(include <cuda_runtime.h>
					    <cuda.h>
					    <nvrtc.h>)))

      ;; include file
      ;; http://www.cplusplus.com/forum/articles/10627/
      
      (loop for e in (reverse *module*) and i from 0 do
	   (destructuring-bind (&key name code) e
	     
	     (let ((cuda (cl-ppcre:scan "cuda" (string-downcase (format nil "~a" name)))))
	       
	       (unless cuda
		 #+nil (progn (format t "emit function declarations for ~a~%" name)
			      (emit-c :code code
				      :hook-defun #'(lambda (str)
						      (format t "~a~%" str))
				      :header-only t))
		 #+nil (emit-c :code code
			 :hook-defun #'(lambda (str)
					 (format s "~a~%" str)
					 )
			 :hook-defclass #'(lambda (str)
					    (format s "~a;~%" str)
					    )
			 :header-only t
			 )
		 (let* ((file (format nil
				      "vis_~2,'0d_~a"
				      i name
				      ))
			(file-h (string-upcase (format nil "~a_H" file)))
			(fn-h (format nil "~a/~a.hpp"
				      *source-dir* file)))
		   (with-open-file (sh fn-h
				       :direction :output
				       :if-exists :supersede
				       :if-does-not-exist :create)
		     (format sh "#ifndef ~a~%" file-h)
		     (format sh "#define ~a~%" file-h)
		     
		     (emit-c :code code
			     :hook-defun #'(lambda (str)
					     (format sh "~a~%" str)
					     )
			     :hook-defclass #'(lambda (str)
						(format sh "~a;~%" str)
						)
			     :header-only t
			     )
		     (format sh "#endif")
		     )
		   (sb-ext:run-program "/usr/bin/clang-format"
				       (list "-i"  (namestring fn-h)
				   
				   ))))
	       
	       #+nil (format t "emit cpp file for ~a~%" name)
	       (write-source (format nil
				     "~a/vis_~2,'0d_~a.~a"
				     *source-dir* i name
				     (if cuda
					 "cu"
					 "cpp"))
			     code))))
      #+nil (format s "#endif"))
    (write-source (asdf:system-relative-pathname
		   'cl-cpp-generator2
		   (merge-pathnames #P"utils.h"
				    *source-dir*))
		  `(do0
		    "#ifndef UTILS_H"
		    " "
		    "#define UTILS_H"
		    " "
		    (include <vector>
			     <array>
			     <iostream>
			     <iomanip>)
		    
		    " "
		    (do0
		     
		     " "
		     ,@(loop for e in (reverse *utils-code*) collect
			  e))
		    " "
		    "#endif"
		    " "))
    (write-source (asdf:system-relative-pathname 'cl-cpp-generator2 (merge-pathnames
								     #P"globals.h"
								     *source-dir*))
		  `(do0
		    "#ifndef GLOBALS_H"
		    " "
		    "#define GLOBALS_H"
		    " "

		    (include "vis_02_tsqueue.hpp")
		    #+nil (include <complex>)
		    #+nil (include <deque>
			     <map>
			     <string>)
		    #+nil (include <thread>
			     <mutex>
			     <queue>
			     <condition_variable>
			     )
		    " "

		    " "
		    ;(include "proto2.h")
		    " "
		    ,@(loop for e in (reverse *global-code*) collect
			 e)

		    " "
		    ,(emit-globals)
		    " "
		    "#endif"
		    " "))))



 
