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
  (defparameter *source-dir* "/home/martin/stage/build_pluto_firmware/gl_osci/source/")
  
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
  (defun guard (code &key (debug t))
      `(do0
	#+lock-debug ,(if debug
			  (logprint (format nil "hold guard on ~a" (cl-cpp-generator2::emit-c :code code))
				    `())
			  "// no debug")
	#+eou ,(if debug
		   `(if (dot ,code ("std::mutex::try_lock"))
			(do0
			 (dot ,code (unlock)))
			(do0
			 ,(logprint (format nil "have to wait on ~a" (cl-cpp-generator2::emit-c :code code))
				    `())))
		   "// no debug")
	"// no debug"
	,(format nil
		 "std::lock_guard<std::mutex> guard(~a);"
		 (cl-cpp-generator2::emit-c :code code))))
  (defun lock (code &key (debug t))
      `(do0
	#+lock-debug ,(if debug
			  (logprint (format nil "hold lock on ~a" (cl-cpp-generator2::emit-c :code code))
				    `())
			  "// no debug")

	#+nil (if (dot ,code ("std::mutex::try_lock"))
		  (do0
		   (dot ,code (unlock)))
		  (do0
		   ,(logprint (format nil "have to wait on ~a" (cl-cpp-generator2::emit-c :code code))
			      `())))
	
	,(format nil
		 "std::unique_lock<std::mutex> lk(~a);"
		 
		 (cl-cpp-generator2::emit-c :code code))
	))

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
  
  (let*  ()
    (define-module
       `(base ((_main_version :type "std::string")
		    (_code_repository :type "std::string")
	       (_code_generation_time :type "std::string")
	        (_cursor_xpos :type double)
	      (_cursor_ypos :type double)
		 )
	      (do0
	       
	    
		    (include <iostream>
			     <chrono>
			     <thread>
			     
			     )

		    " "
		    (include "vis_01_glfw_window.hpp"
			     "vis_02_draw.hpp"
			     "vis_03_gui.hpp"
			     "vis_04_client.hpp")


		    "using namespace std::chrono_literals;"
		    " "

		    (split-header-and-code
		     (do0
		      "// header"

		      )
		     (do0
		      "// implementation"

		      ))

		     (let ((state ,(emit-globals :init t)))
		       (declare (type "State" state)))
		     
		    ;"std::vector<char> buffer(20*1024);"

		    (defun mainLoop ()
		      (while (not (glfwWindowShouldClose ,(g `_window)))
			     (glfwPollEvents)
			     (glfwGetCursorPos ,(g `_window)
					       (ref ,(g `_cursor_xpos))
					       (ref ,(g `_cursor_ypos)))
			     (drawFrame)
			     (drawGui)
			     (glfwSwapBuffers ,(g `_window))
			     ))
		    

		    (defun main (argc argv
				 )
		      (declare (type int argc)
			       (type char** argv)
			       (values int))
		      (setf
		 
		 
                 ,(g `_code_repository) (string ,(format nil "github.com/plops/build_pluto_firmware"))
		 
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
					     (count)))
		      ,(logprint "start" `(argc (aref argv 0)))
		      (do0
		       (do0
			(initWindow)
			(initGui)
			(initDraw
			 )
			(initClient)
			(mainLoop))
		       (do0
			(cleanupDraw)
			(cleanupGui)
			(cleanupWindow)))
		      
		      (return 0)))))

    (define-module
      `(glfw_window
	((_window :direction 'out :type GLFWwindow* )
	 (_framebufferResized :direction 'out :type bool))
	(do0
	 (defun keyCallback (window key scancode action mods)
	   (declare (type GLFWwindow* window)
		    (type int key scancode action mods))
	   (when (and (or (== key GLFW_KEY_ESCAPE)
			  (== key GLFW_KEY_Q))
		      (== action GLFW_PRESS))
	     (glfwSetWindowShouldClose window GLFW_TRUE))
	   (do0
	    (when (and (== key GLFW_KEY_UP)
		       (== action GLFW_PRESS))
	      (incf ,(g `_sample_offset)))
	    (when (and (== key GLFW_KEY_DOWN)
		       (== action GLFW_PRESS))
	      (decf ,(g `_sample_offset))))

	   (do0
	    (when (and (== key GLFW_KEY_1)
		       (== action GLFW_PRESS))
	      (setf ,(g `_sample_threshold)
		    (* ,(g `_sample_threshold) (- 1 .1))))
	    (when (and (== key GLFW_KEY_2)
		       (== action GLFW_PRESS))
	      (setf ,(g `_sample_threshold)
		    (* ,(g `_sample_threshold) (+ 1 .1)))
	      ))
	   )
	 (defun errorCallback (err description)
	   (declare (type int err)
		    (type "const char*" description))
	   ,(logprint "error" `(err description)))
	 (defun framebufferResizeCallback (window width height)
	   (declare (values "static void")
		    ;; static because glfw doesnt know how to call a member function with a this pointer
		    (type GLFWwindow* window)
		    (type int width height))
	   ,(logprint "resize" `(width height))
	   (let ((app ("(State*)" (glfwGetWindowUserPointer window))))
	     (setf app->_framebufferResized true)))
	 (defun initWindow ()
	   (declare (values void))
	   (when (glfwInit)
	     (do0
	      
	      (glfwSetErrorCallback errorCallback)
	      
	      (glfwWindowHint GLFW_CONTEXT_VERSION_MAJOR 2)
	      (glfwWindowHint GLFW_CONTEXT_VERSION_MINOR 0)
	      
	      (glfwWindowHint GLFW_RESIZABLE GLFW_TRUE)
	      (let ((label)
		    )
		(declare (type "std::stringstream" label))
		(<< label
		    (string "glfw oscilloscope [")
		    ,(g `_code_generation_time)
		    (string "] git:")
		    "std::fixed"
		    ("std::setprecision" 3)
		    ,(g `_main_version)
		    ))
	      
	      (setf ,(g `_window) (glfwCreateWindow 930 930
						    (dot label (str) (c_str))
						    
						    NULL
						    NULL))
	      ,(logprint "initWindow" `(,(g `_window)
					 (glfwGetVersionString)))
	      ;; store this pointer to the instance for use in the callback
	      (glfwSetKeyCallback ,(g `_window) keyCallback)
	      (glfwSetWindowUserPointer ,(g `_window) (ref state))
	      (glfwSetFramebufferSizeCallback ,(g `_window)
					      framebufferResizeCallback)
	      (glfwMakeContextCurrent ,(g `_window))
	      (glfwSwapInterval 1)
	      )))
	 (defun cleanupWindow ()
	   (declare (values void))
	   (glfwDestroyWindow ,(g `_window))
	   (glfwTerminate)
	   ))))
    
    (define-module
      `(draw ((_fontTex :direction 'out :type GLuint)
	      (_draw_mutex :type "std::mutex")
	      (_draw_display_log :type bool)
	      ,@(loop for e in `(offset_x offset_y scale_x scale_y alpha marker_x)
		   collect
		     `(,(format nil "_draw_~a" e) :type float))
	      (_screen_offset :type glm--vec2)
	      (_screen_start_pan :type glm--vec2)
	      (_screen_scale :type float)
	      (_screen_grid :type float)
	      (_snapped_world_cursor :type glm--vec2)
	      (_sample_offset :type float)
	      (_sample_threshold :type float)
	      (_sample_data :type "std::vector<float>")
	      (_sample_binary :type "std::vector<uint8_t>")
	      (_sample_valid :type "std::vector<uint8_t>"))
	     (do0
					;,(emit-global :code `(include <glm/vec2.hpp>))
	      (include <algorithm>)

	      
	      
	      #+nil (do0 "// initialize static varibles"
			 (setf "float Shape::world_scale" 1s0
			       "glm::vec2 Shape::world_offset" (curly 0 0)))
	      
	      (defun uploadTex (image w h)
		(declare (type "const void*" image)
			 (type int w h))
		(glGenTextures 1 (ref ,(g `_fontTex)))
		(glBindTexture GL_TEXTURE_2D ,(g `_fontTex))
		(glTexParameteri GL_TEXTURE_2D GL_TEXTURE_MIN_FILTER GL_LINEAR)
		(glTexParameteri GL_TEXTURE_2D GL_TEXTURE_MAG_FILTER GL_LINEAR)
		(glTexImage2D GL_TEXTURE_2D 0 GL_RGBA w h 0 GL_RGBA GL_UNSIGNED_BYTE image))

	      (defun screen_width ()
		(declare (values int))
		(let ((width 0)
		      (height 0))
		  (declare (type int width height))
		  (glfwGetFramebufferSize ,(g `_window)
					  &width
					  &height)
		  (return width)))
	      (defun screen_height ()
		(declare (values int))
		(let ((width 0)
		      (height 0))
		  (declare (type int width height))
		  (glfwGetFramebufferSize ,(g `_window)
					  &width
					  &height)
		  (return height)))

	      (defun get_mouse_position ()
		(declare (values "glm::vec2"))
		(let ((x 0d0)
		      (y 0d0))
		  (glfwGetCursorPos ,(g `_window)
				    &x &y)
		  (return (glm--vec2 (curly (static_cast<float> x)
					    (static_cast<float> y))))))
	      (defun draw_circle (sx sy rad)
		(declare (type float sx sy rad))
		(glBegin GL_LINE_LOOP)
		,@(let ((n 113))
		    (loop for i upto n collect
			 `(progn
			    (let ((arg ,(/ (* 1s0 i) (+ n 1)))
				  )
			      (glVertex2f (+ sx (* rad (sinf (* 2 M_PI arg))))
					  (+ sy (* rad (cosf (* 2 M_PI arg)))))))))
		(glEnd))
	      (defun initDraw ()
		(setf ,(g `_sample_offset) 0s0
		      ,(g `_sample_threshold) .01s0)
		(progn
		  ,(guard (g `_draw_mutex))
		  (setf ,(g `_draw_display_log) true)
		  ,@(loop for (e f) in `((offset_x -.03)
					 (offset_y -.44)
					 (scale_x .22s0)
					 (scale_y .23s0)
					 (alpha .19s0)
					 (marker_x 100s0))
		       collect
			 `(setf ,(g (format nil "_draw_~a" e)) ,f)))
					;(glEnable GL_TEXTURE_2D)
		#+nil (glEnable GL_DEPTH_TEST)
		(do0 (glHint GL_LINE_SMOOTH GL_NICEST)
		     (do0 (glEnable GL_BLEND)
			  (glBlendFunc GL_SRC_ALPHA
				       GL_ONE_MINUS_SRC_ALPHA)))
		(glClearColor 0 0 0 1)
		(setf ,(g `_framebufferResized) true)
		(setf ,(g `_screen_offset) (curly 0s0 0s0)
		      ,(g `_screen_start_pan) (curly 0s0 0s0)
		      ,(g `_screen_scale) 10s0
		      ,(g `_screen_grid) 1s0
		      
		      )

		(do0
		 "// default offset to middle of screen"
		 (setf ,(g `_screen_offset) (curly (/ (static_cast<float> (/ (screen_width) -2))
						      ,(g `_screen_scale))
						   (/ (static_cast<float> (/ (screen_height) -2))
						      ,(g `_screen_scale)))
		       )
                 )
		,(logprint "screen"
			   `(,@(loop for e in `((aref _screen_offset 0)
						(aref _screen_offset 1)
						(aref _screen_start_pan 0)
						(aref _screen_start_pan 1)
						_screen_scale
						_screen_grid) collect
				    (g e))))
		)

	      (defun world_to_screen (v screeni screenj)
		(declare (type "const glm::vec2 &" v)
			 (type "int&" screeni screenj))
		(setf screeni (static_cast<int> (* (- (aref v 0)
						      (aref ,(g `_screen_offset) 0))
						   ,(g `_screen_scale)))
		      screenj (static_cast<int> (* (- (aref v 1)
						      (aref ,(g `_screen_offset) 1))
						   ,(g `_screen_scale)))))

	      (defun screen_to_world (screeni screenj v)
		(declare (type "glm::vec2 &" v)
			 (type "int" screeni screenj))
		
		(setf (aref v 0) (+ (/ (static_cast<float> screeni)
				       ,(g `_screen_scale))
				    (aref ,(g `_screen_offset) 0))
		      (aref v 1) (+ (/ (static_cast<float> screenj)
				       ,(g `_screen_scale))
				    (aref ,(g `_screen_offset) 1))
		      ))
	      
	      (defun cleanupDraw ()
		(glDeleteTextures 1 (ref ,(g `_fontTex))))
	      (defun drawFrame ()

		(when ,(g `_framebufferResized)
		  (do0
		   (setf ,(g `_framebufferResized) false)
		   (let ((width 0)
			 (height 0))
		     (declare (type int width height))
		     (while (or (== 0 width)
				(== 0 height))
		       (glfwGetFramebufferSize ,(g `_window)
					       &width
					       &height)
                       
		       (glViewport 0 0 width height)
		       
		       (do0 (glMatrixMode GL_PROJECTION)
			    (glPushMatrix)
			    (glLoadIdentity)
			    (glOrtho 0s0 width height 0s0 -1s0 1s0)
					;(glOrtho -1s0 1s0 -1s0 1s0 -1s0 1s0)
			    )

		       (do0
			#+nil ,(logprint "drawFrame resize" `((screen_width) width
							(screen_height) height))
			"// default offset to middle of screen"
			(setf ,(g `_screen_offset) (curly (/ (static_cast<float> (/ (screen_width) -2))
							     ,(g `_screen_scale))
							  (/ (static_cast<float> (/ (screen_height) -2))
							     ,(g `_screen_scale)))))
		       
		       (do0 (glMatrixMode GL_MODELVIEW)
			    (glPushMatrix)
			    (glLoadIdentity))))))

		
		(glClear (logior GL_COLOR_BUFFER_BIT
				 GL_DEPTH_BUFFER_BIT))

		(do0
		 (let ((mouse_state (glfwGetMouseButton ,(g `_window)
							GLFW_MOUSE_BUTTON_MIDDLE))
		       (old_mouse_state GLFW_RELEASE)
		       (mouse_pos (get_mouse_position)))
		   (declare (type "static int" old_mouse_state))
		   
		   (when (and (== mouse_state GLFW_PRESS) ;; new press
			      (== old_mouse_state GLFW_RELEASE))
					;,(logprint "left mouse is pressed")
		     (setf ,(g `_screen_start_pan) mouse_pos))

		   (when (and (== mouse_state GLFW_PRESS) ;; button is being held
			      (== old_mouse_state GLFW_PRESS))
		     ;;,(logprint "left mouse is held")
		     (decf ,(g `_screen_offset)
			   (/ (- mouse_pos ,(g `_screen_start_pan))
			      ,(g `_screen_scale)))
		     (setf ,(g `_screen_start_pan) mouse_pos))

		   (do0
		    ;; zoom
		    (let ((mouse_before_zoom (glm--vec2))
			  (zoom_speed .05s0))
		      (screen_to_world (static_cast<int> (aref mouse_pos 0))
				       (static_cast<int> (aref mouse_pos 1))
				       mouse_before_zoom)

		      (progn
			(let ((key_state (glfwGetKey ,(g `_window)
						     GLFW_KEY_PERIOD)))
			  (when (== key_state GLFW_PRESS)
			    ;; zoom out with .
			    (setf ,(g `_screen_scale)
				  (* (- 1s0 zoom_speed)  ,(g `_screen_scale))))))
		      (progn
			(let ((key_state (glfwGetKey ,(g `_window)
						     GLFW_KEY_COMMA)))
			  (when (== key_state GLFW_PRESS)
			    ;; zoom in with ,
			    (setf ,(g `_screen_scale)
				  (* (+ 1s0 zoom_speed) ,(g `_screen_scale))))))
		      (let ((mouse_after_zoom (glm--vec2)))
			(screen_to_world (static_cast<int> (aref mouse_pos 0))
					 (static_cast<int> (aref mouse_pos 1))
					 mouse_after_zoom)
			(incf ,(g `_screen_offset)
			      (- mouse_before_zoom
				 mouse_after_zoom))

			(do0
			 "// compute snapped world cursor"
			 ,@(loop for i below 2 collect
				`(setf (aref ,(g `_snapped_world_cursor ) ,i)
				       (floorf (* (+ .5s0 (aref mouse_after_zoom ,i))
						  ,(g `_screen_grid)))))))))

		  
		   
		   (setf old_mouse_state mouse_state)))


		(do0
		 ;; get visible world
		 (let ((world_top_left (glm--vec2))
		       (world_bottom_right (glm--vec2)))
		   (screen_to_world 0 0 world_top_left)
		   (screen_to_world (screen_width)
				    (screen_height)
				    world_bottom_right)
		   ,@(loop for (edge op) in `((world_top_left floor)
					      (world_bottom_right ceil))
			append
			  (loop for i below 2 collect
			       `(setf (aref ,edge ,i)
				      (,op (aref ,edge ,i)))))
		   (do0
		    ;; world axes
		    (let ((sx 0)
			  (sy 0)
			  (ex 0)
			  (ey 0))
		      (do0 ;; y axis
		       (world_to_screen (curly 0 (aref world_top_left 1)) sx sy)
		       (world_to_screen (curly 0 (aref world_bottom_right 1)) ex ey)
		       (glColor4f .8 .3 .3 1)
		       (glEnable GL_LINE_STIPPLE)
		       (glLineStipple 1 (hex #xF0F0))
		       (glBegin GL_LINES)
		       (glVertex2f sx sy)
		       (glVertex2f ex ey)
		       (glEnd)
					;(glLineStipple 1 (hex #xFFFF))
		       )
		      (do0 ;; x axis
		       (world_to_screen (curly (aref world_top_left 0) 0) sx sy)
		       (world_to_screen (curly (aref world_bottom_right 0) 0) ex ey)
		       (glColor4f .8 .3 .3 1)
					;(glLineStipple 1 (hex #xF0F0))
		       (glBegin GL_LINES)
		       (glVertex2f sx sy)
		       (glVertex2f ex ey)
		       (glEnd))
		      
		      (do0
		       ;; grid axes
		       (glColor4f .3 .3 .3 1)
		       (glLineStipple 1 (hex #xAAAA))
		       (glBegin GL_LINES)
		       
		       ,@(loop for i from -100 upto 100 collect
			    ;; parallel to x axis
			      `(do0 (world_to_screen (curly (aref world_top_left 0) ,i) sx sy)
				    (world_to_screen (curly (aref world_bottom_right 0) ,i) ex ey)
				    (glVertex2f sx sy)
				    (glVertex2f ex ey)))
		       ,@(loop for i from -100 upto 100 collect
			    ;; parallel to y axis
			      `(do0 (world_to_screen (curly ,i (aref world_top_left 1)) sx sy)
				    (world_to_screen (curly ,i (aref world_bottom_right 1)) ex ey)
				    (glVertex2f sx sy)
				    (glVertex2f ex ey)))
		       (glEnd)
		       (do0 (glLineStipple 1 (hex #xFFFF))
			    (glDisable GL_LINE_STIPPLE))

		       (do0
			"// draw snapped cursor circle"
			(world_to_screen ,(g `_snapped_world_cursor) sx sy)
			(glColor3f 1s0 1s0 0s0)
			(draw_circle sx sy 3)
			))))))

		(do0 ;; mouse cursor
		 (glColor4f 1 1 1 1)
		 
		 (do0
		  (glBegin GL_LINES)
		  (let ((x ,(g `_cursor_xpos))
			(y ,(g `_cursor_ypos))
			(h (screen_height))
			(w (screen_width)))
	
		    (do0
		     (glVertex2d x (* 0 h))
		     (glVertex2d x (* 1 h))
		     (glVertex2d (* 0 w) y)
		     (glVertex2d (* 1 w) y)))
		  (glEnd)))
		(do0
		 ;; draw graph
		 (when ,(g `_iqdata_bytes)
		   (let ((n (/ ,(g `_iqdata_bytes) (* 2 2)))
			 (q (/ 1s2))
			 (s (/ -100s0 40s3)))
		     (do0 ;; plot I data
		      (glColor3f .7s0 .1s0 .1s0)
		      (glBegin GL_LINE_STRIP)
		      (dotimes (i n)
			(do0 (setf x (* q i)
				   y (* s (aref ,(g `_iqdata) (+ 0 (* 2 i)))))
			     (world_to_screen (curly x y)
					      sx sy)
			     
			     (glVertex2f sx sy)
			     ))
		      (glEnd))

		     (do0 ;; plot smoothed I data
		      (glColor3f 1s0 .3s0 .3s0)
		      (glBegin GL_LINE_STRIP)
		      (dotimes (i n)
			(do0 (setf x (* q i)
				   y (* s (,(make-filter 'low :fc 0.01) (aref ,(g `_iqdata) (+ 0 (* 2 i))))))
			     (world_to_screen (curly x y)
					      sx sy)
			     
			     (glVertex2f sx sy)
			     ))
		      (glEnd))

		     
		     
		     (do0 ;; plot Q data
		      (glColor3f .1s0 .7s0 .1s0)
		      (glBegin GL_LINE_STRIP)
		      (dotimes (i n)
			
			
			(do0 (setf x (* q i)
				   y (* s (aref ,(g `_iqdata) (+ 1 (* 2 i)))))
			     (world_to_screen (curly x y)
					      sx sy)
			     
			     (glVertex2f sx sy)
			     ))
		      (glEnd))

		     (do0 ;; plot smoothed Q data
		      (glColor3f .3s0 1s0 .3s0)
		      (glBegin GL_LINE_STRIP)
		      (dotimes (i n)
			(do0 (setf x (* q i)
				   y (* s (,(make-filter 'low :fc 0.01) (aref ,(g `_iqdata) (+ 1 (* 2 i))))))
			     (world_to_screen (curly x y)
					      sx sy)
			     
			     (glVertex2f sx sy)
			     ))
		      (glEnd))

		     (do0 ;; plot magnitude data
		      (glColor3f .8s0 .8s0 1s0)
		      (glBegin GL_LINE_STRIP)
		      (dotimes (i n)
			
			
			(do0 (setf x (* q i)
				   y (* s (std--sqrt (+ (* (aref ,(g `_iqdata) (+ 1 (* 2 i)))
						      (aref ,(g `_iqdata) (+ 1 (* 2 i))))
						   (* (aref ,(g `_iqdata) (+ 0 (* 2 i)))
						      (aref ,(g `_iqdata) (+ 0 (* 2 i))))))))
			     (world_to_screen (curly x y)
					      sx sy)
			     
			     (glVertex2f sx sy)
			     ))
		      (glEnd))

		     (do0 ;; plot phase data
		      (glColor3f 1s0 .8s0 .4s0)
		      (glBegin GL_LINE_STRIP)
		      (dotimes (i n)
			(do0 (setf x (* q i)
				   y (+ -1 (* (/ -.5s0 M_PI) (+ M_PI (atan2 (aref ,(g `_iqdata) (+ 1 (* 2 i)))
									    (aref ,(g `_iqdata) (+ 0 (* 2 i))))))))
			     (world_to_screen (curly x y)
					      sx sy)
			     (glVertex2f sx sy)))
		      (glEnd))

		     (do0 ;; plot time derivative of phase data
		      (glColor4f .2s0 1s0 .6s0 .3s0)
		      (glLineWidth 2)
		      (glBegin GL_LINE_STRIP)
		      (let ((old_i 0s0)
			    (old_q 0s0)
			    )
		       (dotimes (i n)
			 (do0 (setf x (* q i)
				    )
			    
			      (let ((smooth_i (* s (,(make-filter 'low :fc 0.025) (aref ,(g `_iqdata) (+ 0 (* 2 i))))))
				    (smooth_q (* s (,(make-filter 'low :fc 0.025) (aref ,(g `_iqdata) (+ 1 (* 2 i))))))
				    (di_dt (- smooth_i old_i))
				    (dq_dt (- smooth_q old_q))
				    (bot (+ (* smooth_i smooth_i)
					    (* smooth_q smooth_q)))
				    ;; imaginary part of complex division
				    (dphase (/ (- (* dq_dt smooth_i)
						  (* di_dt smooth_q))
					       bot)))
			       
				)
			      (setf old_i smooth_i
				    old_q smooth_q)
			      
			      (if (< ,(g `_sample_threshold)
				     (std--abs dphase))
				  (glColor4f 1s0 .2s0 .6s0 .3s0)
				  (glColor4f .2s0 1s0 .6s0 .3s0))

			      (world_to_screen (curly x (+ 3 (* -30 dphase)))
					       sx sy)
			     
			      (glVertex2f sx sy))
			 )
			)
		      (glEnd))

		     (do0 ;; plot time derivative of phase data sampled
		      (glColor3f .2s0 1s0 .6s0)
		      (glLineWidth 3)
		      (glBegin GL_LINE_STRIP)
		      (let ((sample 0)
			    (old_sample 0))
			(dot ,(g `_sample_data)
			     (clear))
		       (dotimes (i n)
			 (do0 (setf x (* q i))
			      (do0
			       (if (== 0 (% (static_cast<int> (round (/ (+ i ,(g `_sample_offset)) 61.44))) 2))
				   (do0 (setf old_sample sample)
					(setf sample 1))
				   (do0 (setf old_sample sample)
					(setf sample 0))))
			      (let ((smooth_i (* s (,(make-filter 'low :fc 0.025) (aref ,(g `_iqdata) (+ 0 (* 2 i))))))
				    (smooth_q (* s (,(make-filter 'low :fc 0.025) (aref ,(g `_iqdata) (+ 1 (* 2 i))))))
				    (di_dt (- smooth_i old_i))
				    (dq_dt (- smooth_q old_q))
				    (bot (+ (* smooth_i smooth_i)
					    (* smooth_q smooth_q)))
				    ;; imaginary part of complex division
				    (dphase (/ (- (* dq_dt smooth_i)
						  (* di_dt smooth_q))
					       bot))))
			      (setf old_i smooth_i
				    old_q smooth_q)
			      (world_to_screen (curly x (+ 3 (* -30 (std--abs (- sample old_sample)) dphase)))
					       sx sy)
			      (when (- sample old_sample)
				(dot ,(g `_sample_data)
				     (push_back dphase))
				(if (== 0 (% (dot ,(g `_sample_data)
						  (size))
					     8))
				    (if (< ,(g `_sample_threshold)
				     (std--abs dphase))
				  (glColor4f 1s0 .2s0 .6s0 .9s0)
				  (glColor4f .2s0 1s0 .6s0 .9s0))
				    (if (< ,(g `_sample_threshold)
				     (std--abs dphase))
				  (glColor4f 1s0 .2s0 .6s0 .3s0)
				  (glColor4f .2s0 1s0 .6s0 .3s0)))
				)
			      
			      (glVertex2f sx sy))))
		      (glEnd)
		      (let ((bit_count 0)
			    ;(byte_count 0)
			    (byte (uint8_t 0))
			    (byte_valid (uint8_t 0))
			    )
		       (dot ,(g `_sample_binary)
			    (clear))
			(dot ,(g `_sample_valid)
			    (clear))
			
			(for-range (v ,(g `_sample_data)
				   )
				   (let (
					 (current_bit 0)
					 (current_valid 0))
				     (when (< ,(g `_sample_threshold)
					      (std--abs v))
				       (setf current_valid 1)
				       (if (< 0 v)
					    (do0
					     (setf current_bit 0))
					    (do0
					     (setf current_bit 1))))
			       
			       
			       (setf byte
				     (logior byte
					     (* current_bit
						(<< 1 bit_count)))
				     )
				     (setf byte_valid
				     (logior byte_valid
					     (* current_valid
						(<< 1 bit_count)))
				     )
			       (incf bit_count)
			       (when (== bit_count 8)
				 (setf bit_count 0)
				 (dot ,(g `_sample_binary)
				      (push_back byte))
				 (dot ,(g `_sample_valid)
				      (push_back byte_valid))
				 (setf byte 0
				       byte_valid 0))
				     ))
			)
		      )

		     #+nil (do0 ;; plot 1MSPS lines
		      (glColor4f .9s0 1s0 .9s0 .3s0)
		      (glLineWidth 1)
		      (glBegin GL_LINE_STRIP)
		      (dotimes (i n)
			(do0 (setf x (* q i))

			     (setf y 0s0)
			     (when (== 0 (% (static_cast<int> (round (/ (+ i ,(g `_sample_offset)) 61.44))) 2))
			       (setf y 7s0))
			     
			     (world_to_screen (curly x y)
					      sx sy)
			     
			     (glVertex2f sx sy)
			     ))
		      (glEnd))

		     ))
		 )))))


    (define-module
      `(gui ((_gui_mutex :type "std::mutex")
	     (_gui_request_diff_reset :type bool))
	    (do0
	     "// https://youtu.be/nVaQuNXueFw?t=317"
	     "// https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html"
	     (include <imgui.h>
		      <backends/imgui_impl_glfw.h>
		      <backends/imgui_impl_opengl2.h>)
	     (include <algorithm>
		      <string>)
	     (include <iostream>
		      <fstream>)
	     (defun initGui ()
	       ,(logprint "initGui" '())
	       (IMGUI_CHECKVERSION)
	       ("ImGui::CreateContext")
	       
	       (ImGui_ImplGlfw_InitForOpenGL ,(g `_window)
					     true)
	       (ImGui_ImplOpenGL2_Init)
	       ("ImGui::StyleColorsDark"))
	     (defun cleanupGui ()
	       (ImGui_ImplOpenGL2_Shutdown)
	       (ImGui_ImplGlfw_Shutdown)
	       ("ImGui::DestroyContext"))
	    
	     
	     (defun drawGui ()
	       #+nil (<< "std::cout"
			 (string "g")
			 "std::flush")
	       
	       (ImGui_ImplOpenGL2_NewFrame)
	       (ImGui_ImplGlfw_NewFrame)
	       ("ImGui::NewFrame")

	       ,(let ((n-lines 3))
		 `(do0
		  (ImGui--Begin (string "snapped_cursor"))

		  (ImGui--Text (string "x: %04d y: %04d")
			       (static_cast<int> (aref ,(g `_snapped_world_cursor) 0))
			       (static_cast<int> (aref ,(g `_snapped_world_cursor) 1)))
		  (ImGui--Text (string "iqdata_bytes: %d")
			       ,(g `_iqdata_bytes))
		  (ImGui--Text (string "sample_offset: %f")
			       ,(g `_sample_offset))
		  (ImGui--Text (string "sample_threshold: %f")
			       ,(g `_sample_threshold))
		  (ImGui--Text (string "sample_binary size: %zu")
			       (dot ,(g `_sample_binary)
				    (size)))
		   (do0 ;; state machine to search for a run of at
		       ;; least 8 invalid bytes. the valid bits behind
		       ;; this are payload
		   (let ((first_valid_byte 0)
			 (first_valid_bit 0)
			 (zero_count 0)
			 (byte_count 0)
			 (fsm_state 0))
		     ;; 0 .. search for zero (if found go to 1, else stay in 0)
		     ;; 1 .. search for 8 or more zeros (if 8 zeros found goto 2, if zero count it and stay in 1,  if non-zero goto 0)
		     ;; 2 .. cross remaining zeros (if non-zero goto 3, if zero stay in 2)
		     ;; 3 .. payload starts (return payload byte and bit)
		     (when (< 12 (dot ,(g `_sample_valid)
				      (size)))
		       (dotimes (i (dot ,(g `_sample_valid)
					(size)))
			 ; for-range (v ,(g `_sample_valid))
			 (let ((v (aref ,(g `_sample_valid ) i)))
			   (case fsm_state
				   (0 (progn
					(when (== 0 v)
					  (setf fsm_state 1
						zero_count 1))
					break))
				   (1 (progn
					(if (== 0 v)
					    (incf zero_count)
					    (setf fsm_state 0))
					(when (== zero_count 8)
					  (setf fsm_state 2))
					break))
				   (2 (progn
					(when (< 0 v)
					  (setf fsm_state 3))
					break))
				   ))
			 
			 (incf byte_count)
			 (when (== 3 fsm_state)
			   break))))
		   (ImGui--Text (string "first valid byte: %d")
				byte_count)
		   )

		  ,@(loop for j below n-lines collect
			  `(ImGui--Text (string " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x")
					,@(loop for i below 16 collect
						`(aref ,(g `_sample_binary)
						       (+ byte_count ,(+ (* j 16) i))))))
		  (ImGui--Text (string "valid")
			       )
		  ,@(loop for j below n-lines collect
			  `(ImGui--Text (string " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x")
					,@(loop for i below 16 collect
						`(aref ,(g `_sample_valid)
						       (+ byte_count ,(+ (* j 16) i))))))

		  (do0 
		   (ImGui--Text (string "ascii")
				)
		   ,@(loop for j below n-lines collect
			   `(ImGui--Text (string " %2c %2c %2c %2c %2c %2c %2c %2c      %2c %2c %2c %2c %2c %2c %2c %2c")
					 ,@(loop for i below 16 collect
						 `(aref ,(g `_sample_binary)
							(+ byte_count ,(+ (* j 16) i)))))))
		  (do0 
		   (ImGui--Text (string "ascii~")
				)
		   ,@(loop for j below n-lines collect
			   `(ImGui--Text (string " %2c %2c %2c %2c %2c %2c %2c %2c      %2c %2c %2c %2c %2c %2c %2c %2c")
					 ,@(loop for i below 16 collect
						 `(~ (aref ,(g `_sample_binary)
							 (+ byte_count ,(+ (* j 16) i))))))))

		 
		  (ImGui--End)))	       
	       
	       (let ((b true))
		 ("ImGui::ShowDemoWindow" &b))
	       ("ImGui::Render")
	       (ImGui_ImplOpenGL2_RenderDrawData
		("ImGui::GetDrawData"))
	       ))))
    
    (define-module
	`(client ((_iqdata :direction 'out :type "std::array<int16_t,1024*128*2>")
		  (_iqdata_bytes :direction 'out :type int))
	      (do0
	       (include <sys/types.h>
			<sys/socket.h>
			<netinet/in.h>
			<unistd.h>
			<netdb.h>
			<strings.h>)
	    
		    (include <iostream>
			     <chrono>
			     <thread>
			     
			     )

		    (comments "http://www.linuxhowtos.org/data/6/server.c"
			      "http://www.linuxhowtos.org/data/6/client.c")
		    (defun initClient ()  ;(header nbytes_header buf nbytes)
		      (declare (type uint8_t* buf header)
			       (type size_t nbytes_header nbytes))
		      (let ((fd (socket AF_INET SOCK_STREAM 0))
			    (portno 1234)
			    (server (gethostbyname (string "localhost" ;"192.168.2.1"
						    )))
			    (server_addr "{}")
			    ;(client_addr "{}")
			    )
			(declare (type "struct sockaddr_in" server_addr client_addr))
			(setf server_addr.sin_family AF_INET
			      
			      server_addr.sin_port (htons portno))
			(bcopy (static_cast<char*> server->h_addr)
			       (reinterpret_cast<char*> &server_addr.sin_addr.s_addr
						   )
			       server->h_length)
			(when (<  (connect fd
					("reinterpret_cast<struct sockaddr*>" &server_addr)
					(sizeof server_addr))
				  0)
			  ,(logprint "connect failed"))
			
			(let ((bytes_remaining (* 2 (dot ,(g `_iqdata)
							 (size))))
			      (offset_bytes 0)
			      ;(retries 4)
			      ))
			(while (and bytes_remaining
				    ;retries
				    )
			       (let ( ;(buffer ("std::array<int16_t,1024*128*2>"))
					(n (read fd
						 (+ (reinterpret_cast<uint8_t*> (dot ,(g `_iqdata)
										     (data)))
						    offset_bytes)
						 bytes_remaining)))
				 ,(logprint "read returned" `(n bytes_remaining offset_bytes))
				 (when (== n 0)
				   ,(logprint "socket empty?")
				   break)
				    (when (< n 0)
				      ,(logprint "socket read failed")
				      break)
				 (do0 
				      (decf bytes_remaining n)
				      (incf offset_bytes n)
					;(decf retries)
				      (setf ,(g `_iqdata_bytes) (- (* 2 (dot ,(g `_iqdata)
									     (size)))
								   bytes_remaining))
				      )))
			(close fd))))))

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
				   
				   ))
		   )
		 

		 )
	       
	       #+nil (format t "emit cpp file for ~a~%" name)
	       (write-source (format nil
				     "~a/vis_~2,'0d_~a.~a"
				     *source-dir* i name
				     (if cuda
					 "cu"
					 "cpp"))
			     code))))
      #+nil (format s "#endif"))
    (write-source (merge-pathnames #P"utils.h"
				   *source-dir*)
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
    (write-source (merge-pathnames
		   #P"globals.h"
		   *source-dir*)
		  `(do0
		    "#ifndef GLOBALS_H"
		    " "
		    "#define GLOBALS_H"
		    " "

		    #+nil (include <complex>)
		    #+nil (include <deque>
			     <map>
			     <string>)
		     (include <thread>
			      <mutex>
			     ;<queue>
			     ;<condition_variable>
			     )
		     " "
		     (include <GLFW/glfw3.h>
			      <glm/geometric.hpp>
			      <glm/vec2.hpp>
			      <glm/vec4.hpp>)

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




 
