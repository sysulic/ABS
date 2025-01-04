(define (domain gripper)
	(:requirements :strips :typing)
	(:types
		ball - object
		gripper - object
		room - object)
	(:predicates
		(at-robby ?r - room)
		(at ?b - ball ?r - room)
		(free ?g - gripper)
		(carry ?b - ball ?g - gripper))
	(:action move
		:parameters (?from - room ?to - room)
		:precondition (at-robby ?from)
		:effect  (and (at-robby ?to)  
			(not (at-robby ?from))))
	(:action pick
		:parameters (?b - ball ?r - room ?g - gripper)
		:precondition (and (at ?b ?r) 
			(at-robby ?r) 
			(free ?g))
		:effect (and (carry ?b ?g)
			(not (at ?b ?r))
			(not (free ?g))))
	(:action drop
		:parameters  (?b - ball ?r - room ?g - gripper)
		:precondition (and (at-robby ?r) 
			(carry ?b ?g))
		:effect (and (at ?b ?r) 
			(free ?g)
			(not (carry ?b ?g))))
)