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
			       <unistd.h>
			       <algorithm>)

		      (include "vis_01_simple.hpp")
		      #+gui (include "imtui/imtui.h"
			       "imtui/imtui-impl-ncurses.h"
			       "imtui-demo.h")

		      #+nil (include "wchar.h"
			       "locale.h"
			       "ncurses.h")
		      
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
		       (setlocale LC_ALL (string ""))
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

		      #+gui (do0
		       (IMGUI_CHECKVERSION)
		       (ImGui--CreateContext)
		       (let ((screen (ImTui_ImplNcurses_Init true)))
			 (ImTui_ImplText_Init) )
		       )

		      #+nil (let ((wstr (curly 9474 "L'\0'")))
				      (declare (type (array wchar_t 2) wstr))
				      (mvaddwstr 0 0 wstr)
				      (refresh))
		      
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
			    (let (("const nbuf" (* 200 4))
				  (input ;("std::array<std::complex<float>,nbuf>")
				    (static_cast<fftwf_complex*> (fftwf_malloc (* nbuf (sizeof fftwf_complex)))))
				  (output 
				    (static_cast<fftwf_complex*> (fftwf_malloc (* nbuf (sizeof fftwf_complex)))))
				  ;(aoutput ("std::array<std::array<float,nbuf>,8>"))
				  (uoutput)
				  (plan_start (dot ("std::chrono::high_resolution_clock::now")
					    (time_since_epoch)
					    )))
			      (declare (type (array uint8_t (* 3 8 nbuf)) uoutput))
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
				  (let ((count 0))
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
					     (fftwf_execute plan)
					     (dotimes (i nbuf)
					       (let ((v (std--min 255s0 (* (/ 255 15s0) (std--log (+ (* (aref (aref output i) 0)
											 (aref (aref output i) 0))
										      (* (aref (aref output i) 1)
											 (aref (aref output i) 1))
										      ))))))
						(setf ;(aref (aref aoutput (% count 8)) i)
						 (aref uoutput (+ 0 (* 3 (+ i (* nbuf (% count 8)))))) v
						 (aref uoutput (+ 1 (* 3 (+ i (* nbuf (% count 8)))))) v
						 (aref uoutput (+ 2 (* 3 (+ i (* nbuf (% count 8)))))) v
						 ))))
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

					       (when (== 0 (% count 8))
						 (usleep 16000)
						 
						 (emit_image uoutput nbuf 8))
						;; https://stackoverflow.com/questions/23864446/terminal-animation-is-clearing-screen-right-approach
						;; https://en.wikipedia.org/wiki/ANSI_escape_code
						(when (== 0 (% count (* 8 30)))
						  (usleep 16000)
						  (<< std--cout ;(string "\\x1b[H\\x1b[J")
						      (string "\\x1b[H")))
						 #+nil ,(logprint "" `(compute_perc
								    sample_perc
								    compute_samp_dur
								    #+nil (* (/ 255 15.2) (deref (std--max_element (dot aoutput (begin))
												   (dot aoutput (end)))))
					
								    ))
						 ;(<< std--cout count std--endl)
					  
						)
					      #+gui	(do0 
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
					      #+nil (<< std--cout (string-u8 "▒▓▒▓▒▓▒▓")
							std--endl)
					
					     )
				 

					    ))))
				
				 (do0
				  (fftwf_destroy_plan plan)
				  (fftwf_free input)
				  (fftwf_free output))))))

			  #+gui (do0
			   (ImTui_ImplText_Shutdown)
			   (ImTui_ImplNcurses_Shutdown))
			  
			  )

			 (iio_context_destroy ctx)

			 			 ))
		      
		      (return 0)))))


    (define-module
       `(simple ()
	      (do0
	       
	    
		    (include <iostream>
			     <chrono>
			     <thread>
			     <sys/mman.h>
			     <fcntl.h>
			     <unistd.h>
			     <array>
			     <bitset>
			     <cmath>
			     <map>
			     )


		    
		    "using namespace std::chrono_literals;"
		    " "

		     (split-header-and-code
		     (do0
		      "// header"

		      " "

		      )
		     (do0
		      "// implementation"
		      (include "vis_01_simple.hpp")
		      (include "vis_02_complex.hpp")
		      ))


	       
  
		(do0
		 "uint8_t *img;"

		  (defclass CharData ()
		    "public:"
		    (defmethod CharData (codepoint)
		      (declare
		       (type int codepoint)
		       (construct (codePoint codepoint))
		       (values :constructor)))
		    "std::array<int,3> fgColor = std::array<int,3>{0,0,0};"
		   "std::array<int,3> bgColor = std::array<int,3>{0,0,0};"
					 "int codePoint;"
		    
					 ))

		(defun createCharData_simple (img w h x0 y0 codepoint pattern
					      )
		  (declare (type uint8_t* img)
			   (type int w h x0 y0 codepoint pattern)
			   (values CharData))
		  (let ((result (CharData codepoint))
			(fg_count (* 4 4))
			(bg_count (* 4 4))
			(mask (hex #x80000000)))
		    (declare (type "const int" fg_count bg_count))
		    (let ((*avg (result.bgColor.data)))
		     (dotimes (y 4)
		       (dotimes (x 4)
			 (dotimes (i 3)
			   (incf (aref avg i)
				 (aref img (+ i (* 3 (+ x0 x (* w (+ y0 y)))))))))))
		    (let ((*avg1 (result.fgColor.data)))
		     (dotimes (y 4)
		       (dotimes (x 4)
			 (dotimes (i 3)
			   (incf (aref avg1 i)
				 (aref img (+ i (* 3 (+ x0 x (* w (+ y0 y 4)))))))))))
		    (do0 (comments "average color for each bucket")
			 
			 (dotimes (i 3)
			   (setf (aref result.bgColor i)
				 (/ (aref result.bgColor i)
				    bg_count)
				 ))
			 (dotimes (i 3)
			   (setf (aref result.fgColor i)
				 (/ (aref result.fgColor i)
				    fg_count)
				 )))
		    (return result)))
		

		(do0
		 (defun sqr (x)
		   (declare (values float)
			    (type float x))
		   (return (* x x)))
	
		 (defun clamp_byte (value)
		   (declare (inline)
			    (type int value)
			    (values int))
		   (if (< 0 value)
		       (if (< value 255)
			   (return value)
			   (return 255))
		       (return 0)))
		 (defun emit_color (r g b bg)
		   (declare (type int r g b)
			    (type bool bg)
			    )
		   (if bg
		       (<< std--cout (string
				      "\\x1b[48;2;")
			   r (string ";")
			   g (string ";")
			   b (string "m"))
		       (<< std--cout (string
				      "\\x1b[38;2;")
			   r (string ";")
			   g (string ";")
			   b (string "m")))
		)

		 

		 (defun emitCodepoint (codepoint)
		   (declare (type int codepoint)
			    )
		   (when (< codepoint 128)
		     (<< std--cout (static_cast<char> codepoint))
		     return)
		   (when (< codepoint 0x7ff)
		     (<< std--cout (static_cast<char> (logior #xc0 (>> codepoint 6))))
		     (<< std--cout (static_cast<char> (logior #x80 (logand codepoint #x3f))))
		     return)
		   (when (< codepoint 0xffff)
		     (<< std--cout (static_cast<char> (logior #xe0 (>> codepoint 12))))
		     (<< std--cout (static_cast<char> (logior #x80 (logand (>> codepoint 6) #x3f))))
		     (<< std--cout (static_cast<char> (logior #x80 (logand codepoint #x3f))))
		     return)
		   (when (< codepoint 0x10ffff)
		     (<< std--cout (static_cast<char> (logior #xf0 (>> codepoint 18))))
		     (<< std--cout (static_cast<char> (logior #x80 (logand (>> codepoint 12) #x3f))))
		     (<< std--cout (static_cast<char> (logior #x80 (logand (>> codepoint 6) #x3f))))
		     (<< std--cout (static_cast<char> (logior #x80 (logand codepoint #x3f))))
		     return)
		   (<< std--cerr (string "error")))
		 (defun emit_image (img w h)
		   (declare (type uint8_t* img)
			    (type int w h))
		   (let ((lastCharData (CharData 0)))
		     (for ((= "int y" 0)
			   (<= y (- h 8))
			   (incf y 8))
			  (for ((= "int x" 0)
				(<= x (- w 4))
				(incf x 4))
			       (let ((charData
				      (findCharData img w h x y)
				      #+nil (createCharData_simple img w h x y (hex #x2584) (hex #x0000ffff))))
				 (when (or (== 0 x)
					   (!= charData.bgColor
					       lastCharData.bgColor))
				   (emit_color (aref charData.bgColor 0)
					       (aref charData.bgColor 1)
					       (aref charData.bgColor 2)
					       true))
				 (when (or (== 0 x)
					   (!= charData.fgColor
					       lastCharData.fgColor))
				   (emit_color (aref charData.fgColor 0)
					       (aref charData.fgColor 1)
					       (aref charData.fgColor 2)
					       false))
				 (emitCodepoint charData.codePoint)
				 (setf lastCharData charData)
				 
				 )
			       )
			  (<< std--cout (string "\\x1b[0m")
			      std--endl)
			  ))))

		    
		    (defun main2 (argc argv
				 )
		      (declare (type int argc)
			       (type char** argv)
			       (values int))
		      ;,(logprint "start" `(argc (aref argv 0)))
		      (let ((fd (--open (string "img.raw")
					O_RDONLY))
			    ("const w" 300)
			    ("const h" 200)
			    (img (reinterpret_cast<uint8_t*>
				  (mmap nullptr
					(* w h 3)
					PROT_READ
					(logior MAP_FILE MAP_SHARED)
					fd 0))))
			(dotimes (i 10)
			  (emit_image img w h))
			(munmap img (* w h 3)
				)
			(--close fd)
			)
		      (return 0)))))

    (define-module
       `(complex (
	       )
	      (do0
	       
	    
		    (include <iostream>
			     <chrono>
			     <thread>
			     <sys/mman.h>
			     <fcntl.h>
			     <unistd.h>
			     <array>
			     <bitset>
			     <cmath>
			     <map>
			     )


		    
		    "using namespace std::chrono_literals;"
		    " "

		     (split-header-and-code
		     (do0
		      "// header"

		      " "

		      )
		     (do0
		      "// implementation"
		      (include "vis_01_simple.hpp")
		      ))


	       
		     #-nil
		     ,(let ((l `((#x00000000 #x00a0)
				       (#x0000000f #x2581) ;; lower 1/8
				       (#x000000ff #x2582)
			       (#x00000fff #x2583)
			       (#x0000ffff #x2584)
			       (#x000fffff #x2585)
			       (#x00ffffff #x2586)
			       (#x0fffffff #x2587)
			       (#xeeeeeeee #x258a)
			       (#xcccccccc #x258c)
			       (#x88888888 #x258e)
			       (#x0000cccc #x2596)
			       (#x00003333 #x2597)
			       (#xcccc0000 #x2598)
			       (#xcccc3333 #x259a)
			       (#x33330000 #x259d)
			       (#x3333cccc #x259e)
			       (#x3333ffff #x259f)
			       (#x000ff000 #x2501)
			       (#x66666666 #x2503)
			       (#x00077666 #x250f)
			       (#x000ee666 #x2513)
			       (#x66677000 #x2517)
			       (#x666ee000 #x251b)
			       (#x66677666 #x2523)
			       (#x666ee666 #x252b)
			       (#x000ff666 #x2533)
			       (#x666ff000 #x253b)
			       (#x666ff666 #x254b)
			       (#x000cc000 #x2578)
			       (#x00066000 #x2579)
			       (#x00033000 #x257a)
			       (#x00066000 #x257b)
			       (#x06600660 #x254f)
			        (#x000f0000 #x2500)
			        (#x0000f000 #x2500)
			       (#x44444444 #x2502)
			       (#x22222222 #x2502)
			       (#x000e0000 #x2574)
			       (#x0000e000 #x2574)
			       (#x44440000 #x2575)
			       (#x22220000 #x2575)
			       (#x00030000 #x2576)
			       (#x00003000 #x2576)
			       (#x00004444 #x2577)
			       (#x00002222 #x2577)
			       (#x44444444 #x23a2)
			       (#x22222222 #x23a5)
			        (#x0f000000 #x23ba)
			       (#x00f00000 #x23bb)
			        (#x00000f00 #x23bc)
			        (#x000000f0 #x23bd)
			        	 (#x00066000 #x25aa)
				 ))
			  )

		       `(do0
			 (let ((BITMAPS (curly ,@(loop for (e f) in l
						       appending
						       `((hex ,e)
							 (hex ,f)))))
			       (BITMAPS_COUNT ,(* 2 (length l))))
			   (declare (type (array "const unsigned int" ,(* 2 (length l))) BITMAPS)
				    (type "const int" BITMAPS_COUNT))))

		      )
	

		(defun createCharData (img w h x0 y0 codepoint pattern)
		  (declare (type uint8_t* img)
			   (type int w h x0 y0 codepoint pattern)
			   (values CharData))
		  (let ((result (CharData codepoint))
			(fg_count 0)
			(bg_count 0)
			(mask (hex #x80000000)))
		    (dotimes (y 8)
		      (dotimes (x 4)
			(let ((avg))
			  (declare (type int* avg))
			  (if (logand pattern mask)
			      (do0 (setf avg (result.fgColor.data))
				   (incf fg_count))
			      (do0 (setf avg (result.bgColor.data))
				   (incf bg_count)))
			  (dotimes (i 3)
			    (incf  (aref avg i)
				  (aref img (+ i (* 3 (+ x0 x (* w (+ y0 y)))))))

			    )
			  (setf mask (>> mask 1)))))
		   (do0 (comments "average color for each bucket")
			 
			 (unless (== 0 bg_count)
			   (dotimes (i 3)
			     (setf (aref result.bgColor i)
				   (/ (aref result.bgColor i)
				      bg_count)
				   )))
			 (unless (== 0 fg_count)
			   (dotimes (i 3)
			     (setf (aref result.fgColor i)
				   (/ (aref result.fgColor i)
				      fg_count)
				   ))))
		    (return result)))

		(defun findCharData (img w h x0 y0 )
		  (declare (values CharData)
			   (type uint8_t* img)
			   (type int w h x0 y0))
		  (let ((min (curly 255 255 255))
			(max (curly  0 0 0))
			(count_per_color ("std::map<long,int>")))
		    (declare (type (array int 3) min max))
		    (do0
		     (comments "max and min value for each color channel")
		     (dotimes (y 8)
		       (dotimes (x 4)
			 (let ((color 0))
			   (dotimes (i 3)
			     (let ((d (static_cast<int> (aref img (+ i (* 3 (+ x0 x (* w (+ y0 y)))))))))
			       (setf (aref min i)
				     (std--min (aref min i)
					       d))
			       (setf (aref max i)
				     (std--max (aref max i)
					       d))
			       ;; create 32bit rgb value
			       (setf color (logior (<< color 8)
						   d))))
			   (incf (aref count_per_color color))))))

		    (do0
		     (let ((color_per_count ("std::multimap<int,long>")))
		       (for-range (;(kv :type "const auto&")
				   ("[k,v]" :type "const auto&")
				   count_per_color)
				  (color_per_count.insert
				   #+nil("std::pair<int,long>"
				    kv->second
				    kv->first)
				   ("std::pair<int,long>"
				    v k)))))

		    (do0
		     ;; what is this? some kind of histogram?
		     (let ((iter (color_per_count.rbegin))
			   (count2 iter->first)
			   (max_count_color1 iter->second)
			   (max_count_color2 max_count_color1))
		       (unless (== "(++iter)"
				   (color_per_count.rend))
			 (incf count2 iter->first)
			 (setf max_count_color2 iter->second))))

		    (do0
		     (let ((bits 0)
			   (direct (< (/ (* 8 4) 2) count2)))
		       (if direct
			   (dotimes (y 8)
			     (dotimes (x 4)
			       (setf bits (<< bits 1))
			       (let ((d1 0)
				     (d2 0))
				 (dotimes (i 3)
				   (let ((shift (- 16 (* 8 i)))
					 (c1 (logand (>> max_count_color1
							 shift)
						     255))
					 (c2 (logand (>> max_count_color2
							 shift)
						     255))
					 (c (aref img (+ i (* 3 (+ x0 x
								   (* w (+ y0 y))) ))))
					 )
				     (incf d1 (* (- c1 c)
						 (- c1 c)))
				     (incf d2 (* (- c2 c)
						 (- c2 c)))))
				 (when (< d2 d1)
				   (setf bits (logior bits 1))))))
			   (do0
			    (comments "determine channel with greatest range")
			    (let ((splitIndex 0)
				  (bestSplit 0))
			      (dotimes (i 3)
				(let ((delta (- (aref max i)
						       (aref min i))))
				 (when (< bestSplit delta)
				   (setf bestSplit delta
					 splitIndex i))))
			      (comments "split at middle instead of median")
			      (let ((splitValue
				      (+ (aref min splitIndex)
					 (/ bestSplit 2))))
				(comments "bitmap using split and sum the color for both buckets")
				(dotimes (y 8)
				  (dotimes (x 4)
				    (setf bits (<< bits 1))
				    (when (< splitValue
					     (aref img (+ splitIndex
							  (* 3 (+ x0 x
								  (* w (+ y0 y)))))))
				      (setf bits (logior 1 bits)))))))))))

		    (do0
		     (comments "find the best bitmap match by counting bits that don't match, including the inverted bitmaps")
		     (let ((best_diff (int 8))
			   (best_pattern ("static_cast<unsigned int>" (hex #xffff)))
			   (codepoint (hex #x2584))
			   (inverted false))
		       
		       (dotimes (ii (/ BITMAPS_COUNT 2))
			 (let ((i (* 2 ii))
			       (pattern (aref BITMAPS i)))
			   (dotimes (j 2)
			     (let ((diff (int (dot (std--bitset<32>
						(logxor pattern
							bits))
					       (count)))))
			       (when (< diff best_diff)
				 (comments "pattern might be inverted")
				 (setf best_pattern (aref BITMAPS i)
				       codepoint (aref BITMAPS (+ i 1))
				       best_diff diff
				       inverted (!= best_pattern pattern)))
			       (setf pattern ~pattern)))
			   ))
		       (when direct
			 (let ((result (CharData 0)))
			   (when inverted
			     (let ((tmp max_count_color1))
			       (setf max_count_color1 max_count_color2
				     max_count_color2 tmp))
			     )
			   (dotimes (i 3)
			     (let ((shift (- 16 (* 8 i))))
			       (setf (dot result (aref fgColor i))
				     (logand (>> max_count_color2 shift) 255))
			       (setf (dot result (aref bgColor i))
				     (logand (>> max_count_color1 shift) 255))
			       ))
			   (setf result.codePoint codepoint)
			   (return result)))
		       (return (createCharData img w h x0 y0 codepoint best_pattern))
		       ))
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



 
