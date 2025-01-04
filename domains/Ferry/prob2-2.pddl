(define (problem prob02-l3-c5)
(:domain ferry)
(:objects 
		  l0 - location
		  l1 - location
		  l2 - location 
		  l3 - location 
		  l4 - location 
		  l5 - location 
          c0 - car 
          c1 - car
          c2 - car 
          c3 - car 
          c4 - car
)
(:init
	(empty-ferry)
	(at c0 l0)
	(at c1 l1)
	(at c2 l2)
	(at c3 l3)
	(at c4 l4)
	(at-ferry l0)
)
(:goal
	(and
		(at c0 l5)
		(at c1 l5)
		(at c2 l5)
		(at c3 l5)
		(at c4 l5)
)
)
)


