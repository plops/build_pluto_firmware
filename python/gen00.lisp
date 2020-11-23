(eval-when (:compile-toplevel :execute :load-toplevel)
  (ql:quickload "cl-py-generator")
  (ql:quickload "alexandria"))
(in-package :cl-py-generator)



(progn
  (defparameter *path* "/home/martin/stage/build_pluto_firmware/python")
  (defparameter *code-file* "run_00_start")
  (defparameter *source* (format nil "~a/source/~a" *path* *code-file*))
  (defparameter *host*
    "10.1.99.12")
  (defparameter *inspection-facts*
    `((10 "")))

  (defparameter *day-names*
    '("Monday" "Tuesday" "Wednesday"
      "Thursday" "Friday" "Saturday"
      "Sunday"))

     
  (let* (
	 
	 (code
	  `(do0
	    (do0 "# %% imports"
		 (do0
		  
		  (imports (matplotlib))
                                        ;(matplotlib.use (string "QT5Agg"))
					;"from matplotlib.backends.backend_qt5agg import (FigureCanvas, NavigationToolbar2QT as NavigationToolbar)"
					;"from matplotlib.figure import Figure"
		  (imports ((plt matplotlib.pyplot)
			    (animation matplotlib.animation) 
                            ;(xrp xarray.plot)
			    ))
                  
		  (plt.ion)
					;(plt.ioff)
		  ;;(setf font (dict ((string size) (string 6))))
		  ;; (matplotlib.rc (string "font") **font)
		  )
		 (imports (		;os
			   sys
			   time
					;docopt
			   pathlib
			   (np numpy)
			   ;serial
			   (pd pandas)
			   (xr xarray)
			   (xrp xarray.plot)
			   ;skimage.restoration
					;(u astropy.units)
					; EP_SerialIO
			   scipy.ndimage
			   scipy.optimize
			   numpy.fft
					;nfft
			   ;sklearn
			   ;sklearn.linear_model
			   ;itertools
			   ;datetime
			   ;dask.distributed
			   ;(da dask.array)
			   ))
		 

		 (setf
	       _code_git_version
		  (string ,(let ((str (with-output-to-string (s)
					(sb-ext:run-program "/usr/bin/git" (list "rev-parse" "HEAD") :output s))))
			     (subseq str 0 (1- (length str)))))
		  _code_repository (string ,(format nil "https://github.com/plops/build_pluto_firmware/python/tree/master/python/source/run_00_start.py")
					   )

		  _code_generation_time
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
		 (do0

		  (setf a (np.fromfile (string "/home/martin/stage/build_pluto_firmware/capture_simulator/source/o_4channels"
					;"/home/martin/o.sdriq"
					       )
				       np.int16))
		  (setf b (+ (* 1s0 (aref a (slice (+ 32 0) (* 2 23330) 2)))
			     (* 1j (* 1s0 (aref a (slice (+ 32 1) (* 2 23330) 2))))))
		  (setf n (len b))
		  
		  (do0
		   (plt.semilogy (np.fft.fftfreq n) (np.abs (np.fft.fft b)
								  )
				 )
		   (plt.grid))


		  (setf q 0.244663
			osc (np.exp (* 2j np.pi q (np.arange n))))
		  (plt.semilogy (np.fft.fftfreq n) (np.abs (np.fft.fft osc)
								  )
				 )

		  )))
	   ))
    (write-source (format nil "~a/source/~a" *path* *code-file*) code)))

