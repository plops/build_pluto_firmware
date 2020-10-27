(declaim (optimize 
	  (safety 3)
	  (speed 0)
	  (debug 3)))

(eval-when (:compile-toplevel :execute :load-toplevel)
     (ql:quickload "cl-cpp-generator2")
     (ql:quickload "cl-ppcre"))

(in-package :cl-cpp-generator2)



(setf *features* (union *features* `()))

(setf *features* (set-difference *features*
				 '()))

(progn
  (defparameter *source-dir* #P"/home/martin/stage/build_pluto_firmware/pluto_tui/source/")
  
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
		      (string "'")))
	     "std::endl"
	     "std::flush"))))
    (defun iio (code)
      `(unless ,code
	 ,(logprint (format nil "~a" code))))

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
			       <fftw3.h>
			       ;<omp.h>
			       )

		      (include "imtui/imtui.h"
			       "imtui/imtui-impl-ncurses.h"
			       "imtui-demo.h")
		      
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
		    
		    (defun main (argc argv
				 )
		      (declare (type int argc)
			       (type char** argv)
			       (values int))

		      (do0
		       (setf ,(g `_code_version)
                             (string ,(let ((str (with-output-to-string (s)
                                                   (sb-ext:run-program "/usr/bin/git" (list "rev-parse" "HEAD") :output s))))
					(subseq str 0 (1- (length str))))))

                       (setf
			,(g `_code_repository) (string ,(format nil "https://github.com/plops/build_pluto_firmware/tree/master/pluto_tui"))
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
		       (IMGUI_CHECKVERSION)
		       (ImGui--CreateContext)
		       (let ((screen (ImTui_ImplNcurses_Init true)))
			 (ImTui_ImplText_Init) )
		       )

		      (do0
		       #+nil(let ((rxcfg (stream_cfg)))
			 ,@(loop for (e f) in `((bw_hz (MHz 2))
						(fs_hz (MHz 2.5))
						(lo_hz (GHz 2.5))
						(rfport (string "A_BALANCED")))
				 collect
				 `(setf (dot rxcfg ,e)
					,f)))
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
			  (let ((n_chan (iio_device_get_channels_count rx)))
			    ,(logprint "" `(n_chan))
			    ,@(loop for e in `(rx_i rx_q) and i from 0 collect
				    `(let ((,e (iio_device_get_channel rx ,i)))
				       ,(logprint (format nil "~a ~a" e i) `((iio_channel_get_attrs_count ,e)))))
			    (iio_channel_enable rx_i)
			    (iio_channel_enable rx_q)
			    (let (("const nbuf" 1024)
				   (input ;("std::array<std::complex<float>,nbuf>")
				     (static_cast<fftwf_complex*> (fftwf_malloc (* nbuf (sizeof fftwf_complex)))))
				   (output 
				     (static_cast<fftwf_complex*> (fftwf_malloc (* nbuf (sizeof fftwf_complex)))))
				  (plan_start (dot ("std::chrono::high_resolution_clock::now")
					    (time_since_epoch)
					    )))
			      (let ((plan (fftwf_plan_dft_1d nbuf input output
							     FFTW_FORWARD
							     ;FFTW_EXHAUSTIVE
							     ;FFTW_MEASURE
							     FFTW_ESTIMATE
							     ))
				    (plan_end (dot ("std::chrono::high_resolution_clock::now")
					    (time_since_epoch)
					    ))
				    (plan_duration (dot (- plan_end plan_start)
							(count))))
				,(logprint "" `(nbuf plan_duration (* 8 (sizeof fftwf_complex))))
			       (let (
				     (rxbuf (iio_device_create_buffer rx nbuf false))
				     (sample_and_compute_start
				       (dot ("std::chrono::high_resolution_clock::now")
					    (time_since_epoch)
					    ))
				     (sample_start sample_and_compute_start)
				     (compute_start sample_and_compute_start))
				 (do0
				  (do0
			"bool demo = true;"
			)
				  
				  (while true ;dotimes (j 100)
				    
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
				 
				      (do0
				       (setf compute_start
					     (dot ("std::chrono::high_resolution_clock::now")
						  (time_since_epoch)
						  ))
					;"#pragma omp parallel"
				       "#pragma GCC ivdep"
			               (for ((= "uint8_t* p" start)
					     (< p end)
					     (incf p step))
					    (let ((si (aref (reinterpret_cast<int16_t*> p) 0))
						  (sq (aref (reinterpret_cast<int16_t*> p) 1))))
					    (setf (aref (aref input i) 0)
						  
						  si)
					    (setf (aref (aref input i) 1)
						  
						  sq)
					    (incf i)
					    
					    )
				       (fftwf_execute plan))
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
			       (ImTui_ImplNcurses_NewFrame)
			       (ImTui_ImplText_NewFrame)
			       (do0
				(ImGui--NewFrame)
				(ImGui--SetNextWindowPos (ImVec2 4 2)
							 ImGuiCond_Once)
				(ImGui--SetNextWindowSize (ImVec2 50s0 10s0)
							  ImGuiCond_Once)
				(ImGui--Begin (string "pluto rx"))
				,@(loop for (e f) in `((compute_perc "%2lld%%")
						       (sample_perc "%2lld%%")
						       (compute_samp_dur "%lld ns"))
					collect
					`(ImGui--Text (string ,(format nil "~17a: ~a" e f))  ,e))
				(ImGui--Text (string-u8 "▒▓"))
				(ImGui--End))
			       (ImTui--ShowDemoWindow &demo)
			       (ImGui--Render)
			       (ImTui_ImplText_RenderDrawData (ImGui--GetDrawData) screen)
			       (ImTui_ImplNcurses_DrawScreen)
			       )
					
					#+nil ,(logprint "" `(compute_perc
							sample_perc
							compute_samp_dur
					
							)))
				 

				      )))

				 (do0
				  (fftwf_destroy_plan plan)
				  (fftwf_free input)
				  (fftwf_free output))))))

			  (do0
			   (ImTui_ImplText_Shutdown)
			   (ImTui_ImplNcurses_Shutdown))
			  
			  )

			 (iio_context_destroy ctx)

			 			 ))
		      
		      (return 0)))))

    
    
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



 
