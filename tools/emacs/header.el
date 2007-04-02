;;
;; julien quintard's emacs configuration
;;
;; the header file generator is based on the following environment variables:
;;
;;   EC_DEVELOPER:   the developer name
;;

;;
;; each time we close emacs, the header is updated.
;;

(setq write-file-hooks (cons 'ec-update-header write-file-hooks))

;;
;; defines the comment characters used by each language.
;;

(setq
   ec-c-list                 '( (o . "/*") (i . " *") (c . " */") )
   ec-css-list               '( (o . "/*") (i . " *") (c . " */") )
   ec-cpp-list               '( (o . "//") (i . "//") (c . "//") )
   ec-ada-list               '( (o . "--") (i . "--") (c . "--") )
   ec-java-list              '( (o . "//") (i . "//") (c . "//") )
   ec-latex-list             '( (o . "%%") (i . "%%") (c . "%%") )
   ec-lisp-list              '( (o . ";;") (i . ";;") (c . ";;") )
   ec-scheme-list            '( (o . ";;") (i . ";;") (c . ";;") )
   ec-xdefault-list          '( (o . "!!") (i . "!!") (c . "!!") )
   ec-makefile-list          '( (o . "#") (i . "#") (c . "#") )
   ec-shell-list             '( (o . "#") (i . "#") (c . "#") )
   ec-perl-list              '( (o . "#") (i . "#") (c . "#") )
   ec-text-list              '( (o . "#") (i . "#") (c . "#") )
   ec-fundamental-list       '( (o . "#") (i . "#") (c . "#") )
   ec-html-list              '( (o . "<!--") (i . "  --") (c . "-->"))
   ec-nroff-list             '( (o . "\\\"") (i . "\\\"") (c . "\\\""))
)

;;
;; links a language to a comment style.
;;

(setq ec-modes-list '(
   ("C"                  . ec-c-list)
   ("CSS"                . ec-c-list)
   ("C++"                . ec-cpp-list)
   ("Ada"                . ec-ada-list)
   ("java"               . ec-java-list)
   ("LaTeX"              . ec-latex-list)
   ("latex"              . ec-latex-list)
   ("TeX"                . ec-latex-list)
   ("Lisp"               . ec-lisp-list)
   ("Lisp Interaction"   . ec-lisp-list)
   ("Scheme"             . ec-scheme-list)
   ("Emacs-Lisp"         . ec-lisp-list)
   ("xdefault"           . ec-xdefault-list)
   ("Makefile"           . ec-makefile-list)
   ("Text"               . ec-text-list)
   ("Fundamental"        . ec-fundamental-list)
   ("HTML"               . ec-html-list)
   ("Nroff"              . ec-nroff-list)
   ("Shell-script"       . ec-shell-list)
   ("Perl"               . ec-perl-list)
   ("CPerl"              . ec-perl-list)
  )
)

;;
;; gets a special characters sequence: o, i or c for open-comment, in-comment
;; and close-comment.
;;

(defun ec-comment (a)
  (interactive)
  (cdr (assoc a (eval (cdr (assoc mode-name ec-modes-list))))))

;;
;; generates the file header.
;;

(defun ec-generate-header ()
   (interactive)
   (goto-char (point-min))
   (let ((project-name ""))
    (setq project-name (read-from-minibuffer
                        (format "project name: ")
                       )
    )
    (setq license-name (read-from-minibuffer
                        (format "license name: ")
                       )
    )
    (insert-string (ec-comment 'o))
    (newline)
    (insert-string (concat
                    (ec-comment 'i)
                    " ---------- header "
                    (make-string
		     (- 79 (+ (length (ec-comment 'i))
			      (length " ---------- header ")
			   )
		     )
		    ?-)
                   )
    )
    (newline)
    (insert-string (ec-comment 'i))
    (newline)
    (insert-string (concat
                    (ec-comment 'i)
                    " project       "
                    project-name
                   )
    )
    (newline)
    (insert-string (ec-comment 'i))
    (newline)
    (insert-string (concat
                    (ec-comment 'i)
                    " license       "
                    license-name
                   )
    )
    (newline)
    (insert-string (ec-comment 'i))
    (newline)
    (insert-string (concat
                    (ec-comment 'i)
                    " file          "
                    (buffer-file-name)
                   )
    )
    (newline)
    (insert-string (ec-comment 'i))
    (newline)
    (insert-string (concat
                    (ec-comment 'i)
                    " created       "
                    (getenv "EC_DEVELOPER")
                    "   "
                    "["
                    (downcase (current-time-string))
                    "]"
                   )
    )
    (newline)
    (insert-string (concat
                    (ec-comment 'i)
                    " updated       "
                    (getenv "EC_DEVELOPER")
                    "   "
                    "["
                    (downcase (current-time-string))
                    "]"
                   )
    )
    (newline)
    (insert-string (ec-comment 'c))
    (newline)
   )
)

;;
;; updates the header.
;;

(defun ec-update-header ()
   (interactive)
   (save-excursion
    (if (buffer-modified-p)
     (progn
      (goto-char (point-min))

      (if (search-forward
           (concat
            (ec-comment 'i)
            " ---------- header "
            (make-string
	     (- 79 (+ (length (ec-comment 'i))
		      (length " ---------- header ")
	           )
	     )
	    ?-)
           )
          nil t)
       (progn
        (if (search-forward
             (concat
              (ec-comment 'i)
              " file          "
             )
            nil t)
         (progn
          (delete-region
           (progn (beginning-of-line) (point))
           (progn (end-of-line) (point))
          )
          (insert-string (concat
                          (ec-comment 'i)
                          " file          "
                          (buffer-file-name)
                         )
          )
         )
        )
        (if (search-forward
             (concat
              (ec-comment 'i)
              " updated       "
             )
            nil t)
         (progn
          (delete-region
           (progn (beginning-of-line) (point))
           (progn (end-of-line) (point))
          )
          (insert-string (concat
                          (ec-comment 'i)
                          " updated       "
                          (getenv "EC_DEVELOPER")
                          "   "
                          "["
                          (downcase (current-time-string))
                          "]"
                        )
          )
         )
        )
       )
      )

     )
    )
  )
)

;;
;; generates a section header.
;;

(defun ec-generate-section ()
   (interactive)
   (let ((section-name ""))
    (setq section-name (read-from-minibuffer
                        (format "section name: ")
                       )
    )
    (insert-string (ec-comment 'o))
    (newline)
    (insert-string (concat
                    (ec-comment 'i)
                    " ---------- "
		    section-name
		    " "
                    (make-string
		     (- 79 (+ (length (ec-comment 'i))
			      (+ (length " ---------- ")
				 (+ (length section-name)
				    (length " ")
				 )
			      )
			   )
		     )
		     ?-)
                   )
    )
    (newline)
    (insert-string (ec-comment 'c))
    (newline)
   )
)

;;
;; binding to generate the file header.
;;

(global-set-key [(control c)(control h)] 'ec-generate-header)

;;
;; binding to generate a section header.
;;

(global-set-key [(control c)(s)] 'ec-generate-section)

;;
;; binding to go to a specific line.
;;

(global-set-key [(control c)(control g)] 'goto-line)

;;
;; prints column and line number.
;;

(column-number-mode t)
(line-number-mode t)

;;
;; visible bell.
;;

(setq visible-bell t)

;;
;; displays time.
;;

(setq display-time-24hr-format t)
(standard-display-european 1)
(display-time)

;;
;; show the in relation parentheses.
;;

(show-paren-mode t)

;;
;; syntaxic colorisation.
;;

(global-font-lock-mode)

;;
;; no blank line at the end of the file.
;;

(setq-default next-line-add-newlines nil)

;;
;; no scroll bar.
;;

;;(scroll-bar-mode nil)

;;
;; no menu bar.
;;

(menu-bar-mode nil)

;;
;; no tool bar.
;;

(tool-bar-mode nil)

;;
;; bindings for home and end keys.
;;

(global-set-key [home] 'beginning-of-buffer)
(global-set-key [end] 'end-of-buffer)

;;
;; 80 columns alignment [XXX]
;;
;; (setq tefault-major-mode 'text-mode)
;; (setq text-mode-hook 'turn-on-auto-fill)
;; (setq fill-column 80)
