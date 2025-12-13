(require "asdf")

(defun day2-parse (file)
  (loop with s = (uiop:read-file-string file)
	and i = 0
	while (< i (length s))
	collect (multiple-value-bind (a j)
		    (parse-integer s :start i :junk-allowed t)
		  (multiple-value-bind (b k)
		      (parse-integer s :start (1+ j) :junk-allowed t)
		    (setf i (1+ k))
		    (list a b)))))

(defun digits (n)
  (loop for d from 0
	while (> n 0)
	do (setf n (truncate n 10))
	finally (return d)))

(defun make-invalid (n)
  (+ (* n (expt 10 (digits n))) n))

(defun next-invalid-core (n)
  (let* ((d (digits n)) (half-length (expt 10 (truncate d 2))))
    (cond ((oddp d) half-length)
	  (t (let* ((core (truncate n half-length)))
	       (if (>= (make-invalid core) n)
		   core
		   (1+ core)))))))

(defun day2-1 (file)
  (loop for (lo hi) in (day2-parse file)
	summing (loop for i from (next-invalid-core lo)
		      for curr = (make-invalid i)
		      while (<= curr hi)
		      summing curr)))

(defun make-ninvalid (num n)
  (loop for i from 0 below n
       summing (* (expt 10 (* i (digits num))) num)))

(defun next-ninvalid-core (num n)
  (let* ((d (digits num)) (core-length (expt 10 (truncate d n))))
    (if (= 0 (mod d n))
	(let ((core (truncate num (expt core-length (- n 1)))))
	  (if (>= (make-ninvalid core n) num)
	      core
	      (1+ core)))
	core-length)))

(defun day2-2 (file)
  (reduce #'+
	  (delete-duplicates
	   (loop for (lo hi) in (day2-parse file)
		 ;; Collect over all possible fragment length
		 appending (loop for n from 2 to (digits hi)
				 ;; Collect over all value in range
				 appending (loop for i from (next-ninvalid-core lo n)
						 for curr = (make-ninvalid i n)
						 while (<= curr hi)
						 collecting curr))))))
