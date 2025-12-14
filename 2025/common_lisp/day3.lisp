(require "asdf")

(defun day3-parse (file)
  (loop for line in (uiop:read-file-lines file)
	collect (loop for c across line
		      collect (digit-char-p c))))

(defun max-with-index (numbers)
  (loop
    with max-value = (nth 0 numbers)
    with max-index = 0
    for n in numbers
    for i from 0
    do (when (> n max-value)
	 (setf max-value n)
	 (setf max-index i))
    finally (return (values max-value max-index))))

(defun max-jolts (batteries)
  (multiple-value-bind (a i) (max-with-index (butlast batteries))
    (+ (* 10 a) (reduce #'max (subseq batteries (1+ i))))))

(defun day3-1 (file)
  (loop for batteries in (day3-parse file)
	summing (max-jolts batteries)))

(defun max-jolts-n (batteries n)
  (if (= n 1)
      (reduce #'max batteries)
      (multiple-value-bind (a i) (max-with-index (butlast batteries (1- n)))
	(+ (* (expt 10 (1- n)) a) (max-jolts-n (subseq batteries (1+ i)) (1- n))))))

(defun day3-2 (file)
  (loop for batteries in (day3-parse file)
	summing (max-jolts-n batteries 12)))
