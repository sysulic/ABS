(define (problem prob00)
(:domain gripper)
(:objects 
	ball1 - ball
	ball2 - ball
	ball3 - ball
	ball4 - ball
	ball5 - ball
	rooma - room
	roomb - room
	left - gripper
	right - gripper 
)
(:init
	(at ball1 rooma)
	(at ball2 rooma)
	(at ball3 rooma)
	(at ball4 rooma)
	(at ball5 rooma)
	(at-robby rooma)
	(free left)
	(free right)
)
(:goal (and (at ball1 roomb)
			(at ball2 roomb)
			(at ball3 roomb)
			(at ball4 roomb)
			(at ball5 roomb)          
		    (free left)
		    (free right))	
) 	
)