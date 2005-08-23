;;
;; emacs configuration
;; 
;; .emacs-std-comment.el
;; 
;; path          /root
;; 
;; made by mycure
;;         quintard julien   [quinta_j@epita.fr]
;; 
;; started on    Wed Apr  7 22:04:48 2004   mycure
;; last update   Mon Dec 20 23:56:43 2004   mycure
;;

(setq write-file-hooks (cons 'update-std-header write-file-hooks))

(setq std-c-alist               '( (cs . "/*") (cc . " * ") (ce . " */") )
      std-css-alist             '( (cs . "/*") (cc . " * ") (ce . " */") )
      std-cpp-alist             '( (cs . "//") (cc . "// ") (ce . "//") )
      std-pov-alist             '( (cs . "//") (cc . "// ") (ce . "//") )
      std-java-alist            '( (cs . "//") (cc . "// ") (ce . "//") )
      std-latex-alist		'( (cs . "%%") (cc . "%% ") (ce . "%%") )
      std-lisp-alist            '( (cs . ";;") (cc . ";; ") (ce . ";;") )
      std-xdefault-alist	'( (cs . "!!") (cc . "!! ") (ce . "!!") )
      std-pascal-alist		'( (cs . "{ ") (cc . "   ") (ce . "}" ) )
      std-makefile-alist        '( (cs . "##") (cc . "## ") (ce . "##") )
      std-text-alist		'( (cs . "##") (cc . "## ") (ce . "##") )
      std-fundamental-alist     '( (cs . "##") (cc . "## ") (ce . "##") )
      std-html-alist		'( (cs . "<!--") (cc . "  -- ") (ce . "-->"))
      std-nroff-alist		'( (cs . "\\\"") (cc . "\\\" ") (ce . "\\\""))
      std-sscript-alist         '( (cs . "#!/bin/sh")  (cc . "## ") (ce . "##") )
      std-perl-alist            '( (cs . "#!/usr/bin/perl -w")  (cc . "## ")(ce . "##") )
      std-cperl-alist           '( (cs . "#!/usr/bin/perl -w")  (cc . "## ")(ce . "##") ))

(setq std-modes-alist '(("C"                	. std-c-alist)
			("CSS"                	. std-c-alist)
			("PoV"                	. std-pov-alist)
                        ("C++"              	. std-cpp-alist)
                        ("Lisp"             	. std-lisp-alist)
                        ("Lisp Interaction" 	. std-lisp-alist)
                        ("Emacs-Lisp"       	. std-lisp-alist)
                        ("Fundamental"      	. std-fundamental-alist)
                        ("Shell-script"     	. std-sscript-alist)
                        ("Makefile"         	. std-makefile-alist)
			("Perl"            	. std-cperl-alist)
			("CPerl"            	. std-cperl-alist)
			("xdefault"         	. std-xdefault-alist)
			("java"             	. std-java-alist)
			("latex"	    	. std-latex-alist)
			("Pascal"	    	. stdp-ascal-alist)
			("Text"	            	. std-text-alist)
			("HTML"	            	. std-html-alist)
			("Nroff"	    	. std-nroff-alist)
			("TeX"			. std-latex-alist)
			("LaTeX"		. std-latex-alist)))

(defun std-get (a)
  (interactive)
  (cdr (assoc a (eval (cdr (assoc mode-name std-modes-alist))))))

(defun update-std-header ()
  (interactive)
  (save-excursion
    (if (buffer-modified-p)
	(progn
	  (goto-char (point-min))
	  (if (search-forward " path          /" nil t)
	      (progn
		(delete-region
		 (progn (beginning-of-line) (point)) 
		 (progn (end-of-line) (point)))
		(insert-string (concat (std-get 'cc)
				       "path          "
				       (getenv "PWD")))))
	  (if (search-forward " last update   " nil t)
	      (progn
		(delete-region 
		 (progn (beginning-of-line) (point)) 
		 (progn (end-of-line) (point)))
		(insert-string (concat (std-get 'cc)
				       "last update   "
				       (current-time-string)
				       "   "
				       (getenv "NICKNAME")))
		(message "last modification header field updated"))))))
  nil)


(defun std-file-header ()
  (interactive)
  (goto-char (point-min))
  (let ((projname "_")(location "_"))
    (setq projname (read-from-minibuffer
		    (format "type project name: ")))
    (insert-string (std-get 'cs))
    (newline)
    (insert-string (concat (std-get 'cc)
			   "copyright "
			   (getenv "REALNAME")))
    (newline)
    (insert-string (std-get 'cc))
    (newline)
    (insert-string (concat (std-get 'cc)
			   projname))
    (newline)
    (insert-string (std-get 'cc))
    (newline)
    (insert-string (concat (std-get 'cc)
			   (buffer-name)))
    (newline)
    (insert-string (std-get 'cc))
    (newline)
    (insert-string (concat (std-get 'cc)
			   "path          "
			   (getenv "PWD")))
    (newline)
    (insert-string (std-get 'cc))
    (newline)
    (insert-string (concat (std-get 'cc)
			   "made by "
			   (getenv "NICKNAME")))
    (newline)
    (insert-string (concat (std-get 'cc)
			   "        "
			   (getenv "REALNAME")
			   "   ["
			   (getenv "EMAIL")
			   "]"))
    (newline)
    (insert-string (std-get 'cc))
    (newline)
    (insert-string (concat (std-get 'cc)
                           "started on    "
                           (current-time-string)
                           "   "
                           (getenv "NICKNAME")))
    (newline)
    (insert-string (concat (std-get 'cc)
			   "last update   "
                           (current-time-string)
                           "   "
			   (getenv "NICKNAME")))
    (newline)
    (insert-string (std-get 'ce))
    (newline)
))

;; extended bindings for this package and for commenting code

(global-set-key "h" 'update-std-header)
(global-set-key "" 'std-file-header)
