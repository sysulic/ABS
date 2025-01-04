(define (problem prob01)
(:domain gripper)
(:objects 
	ball1 - ball
	ball2 - ball
	ball3 - ball
	ball4 - ball
	ball5 - ball
	ball6 - ball
	ball7 - ball
	ball8 - ball
	room1 - room
	room2 - room
	gripper1 - gripper
	gripper2 - gripper
)
(:init
	(at ball1 room1)
	(at ball2 room1)
	(at ball3 room1)
	(at ball4 room1)
	(at ball5 room1)
	(at ball6 room1)
	(at ball7 room1)
	(at ball8 room1)
	(white ball1)
	(white ball2)
	(white ball3)
	(white ball4)
	(black ball5)
	(black ball6)
	(black ball7)
	(black ball8)
	(free gripper1)
	(free gripper2)
	(he gripper1)
	(he gripper2)
	(at-robby room1)
)
(:goal (and (at ball1 room2)
			(at ball2 room2)
			(at ball3 room2)
			(at ball4 room2) 
			(at ball5 room2) 
			(at ball6 room2) 
			(at ball7 room2) 
			(at ball8 room2) 
			)	
) 	
)