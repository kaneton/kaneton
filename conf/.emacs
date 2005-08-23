;;
;; emacs configuration
;; 
;; .emacs
;; 
;; /root
;; 
;; made by mycure
;;         quintard julien   [quinta_j@epita.fr]
;; 
;; started on    Wed Apr  7 22:04:30 2004   mycure
;; last update   Tue Feb 22 11:40:25 2005   mycure
;;

; load .emacs-std-comment.el
(load-file "~/.emacs-std-comment.el")

; goto
(global-set-key [(control c)(control g)] 'goto-line)

; print column and line number
(column-number-mode t)
(line-number-mode t)

; visible bell
(setq visible-bell t)

; completion
;(global-set-key [(`)] 'dabbrev-expand)

; syntax colors
;(global-font-lock-mode t)
;(setq font-lock-maximum-decoration t)

; display time
(setq display-time-24hr-format t)
(standard-display-european 1)
(display-time)

; no blank lines at the end of the file
(setq-default next-line-add-newlines nil)

; no scroll bar
(scroll-bar-mode nil)

; no menu bar
(menu-bar-mode nil)

; no tool bar
; (tool-bar-mode nil)

; compression mode
(auto-compression-mode t)

; wheel
(defun up-slightly () (interactive) (scroll-up 5))
(defun down-slightly () (interactive) (scroll-down 5))
(global-set-key [mouse-4] 'down-slightly)
(global-set-key [mouse-5] 'up-slightly)

(defun up-one () (interactive) (scroll-up 1))
(defun down-one () (interactive) (scroll-down 1))
(global-set-key [S-mouse-4] 'down-one)
(global-set-key [S-mouse-5] 'up-one)

(defun up-a-lot () (interactive) (scroll-up))
(defun down-a-lot () (interactive) (scroll-down))
(global-set-key [C-mouse-4] 'down-a-lot)
(global-set-key [C-mouse-5] 'up-a-lot)

; home / end
(global-set-key [home] 'beginning-of-buffer)
(global-set-key [end] 'end-of-buffer)
